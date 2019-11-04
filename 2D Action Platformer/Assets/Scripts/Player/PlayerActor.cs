using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

[RequireComponent(typeof(PlayerSFX))]
public class PlayerActor : MonoBehaviour
{
    [Header("Input")]
    public KeyCode m_jmpButton;
    public KeyCode m_attckButton;
    public KeyCode m_useButton;

    [Header("Movement")]
    public float m_movSpeed;
    public float m_jmpSpeed;

    private int m_numContacts;
    public bool isGrounded { get => m_numContacts > 0; }

    private bool m_canMove = true;

    public bool CanMove { 
        get => m_canMove; 
        set {
            m_canMove = value;

            if (!value) {
                m_rb.drag = 7.5f;
            }
            else {
                m_rb.drag = 0.0f;
            }
        }
    }

    private bool m_isDead = false;
    public bool IsDead { 
        get => m_isDead;
        set {
            m_isDead = value;
            m_animator.SetBool("Dead", value);

            if (m_isDead == true) {
                m_rb.drag = 7.5f;
                m_rb.gravityScale = 7.5f;
            }
            else {
                m_rb.drag = 0f;
                m_rb.gravityScale = 1f;
            }
        }     
    }
    
    [Header("Health")]
    public int m_maxHealth;
    public int m_respawnTime;
    public Color m_hitColour;
    
    private Vector3 m_respawnPoint;

    [HideInInspector]
    public int m_health;

    [HideInInspector]
    public bool m_isHit;

    [Header("Attack")]
    public Vector2 m_range;
    public float m_attckForce;
    public Vector2 m_attckDirection;
    [Space]
    public int m_hitFreezeFrames;
    public float m_hitShakeMag;
    public float m_hitShakeRghness;
    public float m_hitShakeFadeIn;
    public float m_hitShakeFadeOut;

    private float m_attackTimer;
    private List<EnemyActor> m_currentEnemies = new List<EnemyActor>();

    // RIGIDBODY //
    private Rigidbody2D m_rb;
    private float m_movInput;
    private bool m_jmpInput;

    // GRAPHICS //
    private SpriteRenderer m_renderer;
    private Animator m_animator;

    // SFX //
    private PlayerSFX m_sfx;

    // Start is called before the first frame update
    void Start() {
        m_rb = GetComponent<Rigidbody2D>();
        m_renderer = GetComponent<SpriteRenderer>();
        m_animator = GetComponent<Animator>();
        m_sfx = GetComponent<PlayerSFX>();
        m_health = m_maxHealth;
        GetComponentInChildren<GroundDetector>().Link(UpdateContacts);
    }

    // Update is called once per frame
    void Update() {
        #region CURSOR LOCK
            Cursor.visible = false;
            Cursor.lockState = CursorLockMode.Locked;
        #endregion

        m_movInput = 0.0f;

        if (CanMove && !IsDead) {
            #region MOVEMENT
                // get movement input
                m_movInput = Input.GetAxis("Horizontal");

                // flip image if going forwards
                if (m_movInput > 0) {
                    m_renderer.flipX = true;
                }
                else if (m_movInput < 0) {
                    m_renderer.flipX = false;
                }

                if (Input.GetKeyDown(m_jmpButton) && isGrounded) {
                    m_jmpInput = true;
                    m_sfx.PlayJump();
                }
            #endregion
            #region ATTACK
                if (Input.GetKeyDown(m_attckButton)) {
                    m_animator.SetTrigger("Attack");
                    m_sfx.PlayAttack();

                    
                }

                if (m_animator.GetCurrentAnimatorStateInfo(0).IsName("LightGuard_Attack")) {
                    m_animator.SetBool("Attacking", true);
                } 
                else {
                    m_animator.SetBool("Attacking", false);
                }
            #endregion
        }

        // set animator state
        if (m_movInput != 0) { 
            m_animator.SetInteger("AnimState", 1);
        } 
        else { 
            m_animator.SetInteger("AnimState", 0); 
        }

        if (m_animator.GetCurrentAnimatorStateInfo(0).IsName("LightGuard_Death")) {
            IsDead = true;
        }
        else if (!m_animator.GetCurrentAnimatorStateInfo(0).IsName("LightGuard_Recover")) {
            IsDead = false;
        }
    }

    void FixedUpdate() {
        if (CanMove) {
            // movement
            m_rb.velocity = new Vector2(m_movInput * m_movSpeed * Time.fixedDeltaTime, m_rb.velocity.y);

            // jump
            if (m_jmpInput) {
                m_rb.AddForce(new Vector2(0f, m_jmpSpeed * Time.fixedDeltaTime), ForceMode2D.Impulse);
                m_jmpInput = false;
            }
        }
    }

    void OnCollisionEnter2D(Collision2D collision) {
        var platform = collision.gameObject.GetComponent<MovingPlatform>();
        if (platform) {
            transform.parent = platform.transform;
        }
    }

    void OnCollisionExit2D(Collision2D collision) {
        transform.parent = null;
    }

    void OnTriggerEnter2D(Collider2D other) {
        switch (other.tag) {
            case "Obstacle":
                if (!IsDead) { 
                    Die();
                    JuiceManager.Instance.StartImpact(m_hitFreezeFrames, m_hitShakeMag, m_hitShakeRghness * 2f, m_hitShakeFadeIn, m_hitShakeFadeOut);
                }
                break;

            case "Checkpoint":
                if (!IsDead) { m_respawnPoint = other.transform.position; }
                break;

            default:
                // do nothing
                break;
        }
    }

    void Die() {
        m_health--;
        IsDead = true;
        m_rb.velocity = Vector2.zero;
        m_rb.angularVelocity = 0f;

        m_animator.SetBool("Attacking", false);
        m_animator.SetBool("Grounded", false);
        m_animator.SetTrigger("Death");

        m_sfx.PlayDeath();

        StartCoroutine(DisplayHit(.25f));
        StartCoroutine(Respawn(m_respawnTime));
    }

    public void TakeHit(Vector2 direction, float force) {
        if (!m_isHit && CanMove) {
            if (m_health - 1 <= 0) {
                Die();
                return;
            }

            m_health--;
            m_rb.AddForce(direction * force, ForceMode2D.Impulse);
            m_isHit = true;
            FindObjectOfType<LevelTimer>().AddPenalty(3f);
            m_animator.SetTrigger("Hurt");
            m_animator.SetBool("Hurting", true);
            m_sfx.PlayHit();
            JuiceManager.Instance.StartImpact(m_hitFreezeFrames, m_hitShakeMag, m_hitShakeRghness, m_hitShakeFadeIn, m_hitShakeFadeOut);
            StartCoroutine(DisplayHit(.25f));
            StartCoroutine(HitRecover(.5f));
        }
    }

    IEnumerator Respawn(float seconds) {
        yield return new WaitForSeconds(seconds * .75f);

        if (m_health <= 0) {
            SceneManager.LoadScene(SceneManager.GetActiveScene().buildIndex, LoadSceneMode.Single);
        }

        transform.SetPositionAndRotation(m_respawnPoint, Quaternion.identity);
        yield return new WaitForSeconds(seconds * .25f);
        m_animator.SetTrigger("Recover");
    }

    IEnumerator HitRecover(float seconds) {
        yield return new WaitForSeconds(seconds);
        m_isHit = false;
        m_animator.SetBool("Hurting", false);
    }

    IEnumerator DisplayHit(float seconds) { 
        m_renderer.color = m_hitColour;
        yield return new WaitForSeconds(seconds);
        m_renderer.color = Color.white;
    }

    void UpdateContacts(int addedContacts) {
        m_numContacts += addedContacts;

        if (m_numContacts > 0) {
            m_animator.SetBool("Grounded", true);
        }
        else {
            m_animator.SetBool("Grounded", false);
            m_animator.SetTrigger("Jump");
        }
    }

    public void SetCurrentEnemies(List<EnemyActor> newEnemies) {
        m_currentEnemies = newEnemies;
    }

    /// ANIMATION FUNCTIONS ///
    public void OnAttack() {
        foreach (var enemy in m_currentEnemies) {
            if (Vector3.Distance(transform.position, enemy.transform.position) <= m_range.x && !enemy.isDead) {
                Vector3 dirToEnemy = enemy.transform.position - transform.position;
                if (dirToEnemy.y < m_range.y && dirToEnemy.y > -m_range.y) {
                    if (m_renderer.flipX) {
                        if (dirToEnemy.x > 0) {
                            enemy.TakeHit(m_attckDirection, m_attckForce);
                        }
                    }
                    else {
                        if (dirToEnemy.x < 0) {
                            enemy.TakeHit(new Vector2(-m_attckDirection.x, m_attckDirection.y), m_attckForce);
                        }
                    }
                }
            }
        }
        m_currentEnemies.RemoveAll(e => e.isDead);
    }

    public void OnFootstep() {
        m_sfx.PlayFootstep();
    }
}
