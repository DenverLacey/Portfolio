using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnemyActor : MonoBehaviour
{
    [Header("Movement")]
    public float m_movSpeed;
    public float m_jmpSpeed;

    private int m_numContacts;
    public bool isGrounded { get => m_numContacts > 0; }

    [Header("Attack")]
    public float m_range;
    public float m_attckForce;
    public Vector2 m_attckDirection;
    [Space]
    public int m_hitFreezeFrames;
    public float m_hitShakeMag;
    public float m_hitShakeRghness;
    public float m_hitShakeFadeIn;
    public float m_hitShakeFadeOut;
    public Color m_hitColour;
    [Space]
    public float m_deathTime;
    public float m_deathTimeBonus = 3f;

    private float m_deathTimer;

    private bool m_isDead;
    public bool isDead {
        get => m_isDead;
        set {
            m_isDead = value;
            m_animator.SetTrigger("Hit");
            m_animator.SetBool("Dying", value);
        }
    }

    [Space]
    public GameObject m_smokePS;
    public Vector3 m_smokeOffset;

    [Header("SFX")]
    public AudioClip m_hitClip;
    public float m_hitVolume;

    // COLLIDERS //
    private CapsuleCollider2D m_collider;
    private Rigidbody2D m_rb;

    // GRAPHICS //
    private SpriteRenderer m_renderer;
    private Animator m_animator;

    PlayerActor m_player;

    // Start is called before the first frame update
    void Start() {
        m_player = FindObjectOfType<PlayerActor>();
        m_rb = GetComponent<Rigidbody2D>();
        m_renderer = GetComponent<SpriteRenderer>();
        m_collider = GetComponent<CapsuleCollider2D>();
        m_animator = GetComponent<Animator>();
        GetComponentInChildren<GroundDetector>().Link(UpdateContacts);
        StartSmoke();
    }

    // Update is called once per frame
    void Update() {
        if (!m_isDead && !m_player.IsDead) {
            #region FACE PLAYER
                Vector3 playerLookDir = m_player.transform.position - transform.position + Vector3.up * 1f;
                if (playerLookDir.x < 0) { m_renderer.flipX = true; } else { m_renderer.flipX = false; }
            #endregion
        
            if (m_player.CanMove) {
                m_rb.drag = 1;
                m_rb.gravityScale = 1;
                #region MOVE TO PLAYER
                    Vector2 desiredDir = new Vector2(m_player.transform.position.x - transform.position.x, 0f).normalized;
                    Vector2 desiredVel = desiredDir * m_movSpeed * Time.deltaTime;
                    m_rb.AddForce(desiredVel, ForceMode2D.Force);
                    m_animator.SetInteger("AnimState", 1);
                #endregion
                #region JUMP
                    if (Vector3.Distance(transform.position, m_player.transform.position) <= m_range && isGrounded) {
                        if (m_player.transform.position.y > transform.position.y + 0.5f) {
                            m_rb.AddForce(Vector2.up * m_jmpSpeed * Time.fixedDeltaTime);
                            m_animator.SetTrigger("Jump");
                        }
                    }
                #endregion
                #region ATTACK
                    if (Vector3.Distance(transform.position, m_player.transform.position) <= m_range) {
                        if (m_animator.GetCurrentAnimatorStateInfo(0).IsName("skeletonBase_Attack") && m_animator.GetNextAnimatorStateInfo(0).IsName("skeletonBase_Idle")) {
                            m_player.TakeHit(m_attckDirection, m_attckForce);
                            m_animator.SetBool("Attacking", false);
                        }
                        else {
                            m_animator.SetTrigger("Attack");
                            m_animator.SetBool("Attacking", true);
                        }
                    }
                #endregion
            }
            else {
                m_rb.drag = 10;
                m_rb.gravityScale = 10;
                m_animator.SetInteger("AnimState", 0);
            }
        }
        else {
            m_deathTimer += Time.deltaTime;
            if (m_deathTimer >= m_deathTime) {
                m_collider.size = new Vector2(0.055f, 0.0495f);
                m_collider.direction = CapsuleDirection2D.Horizontal;
                m_collider.offset = new Vector2(m_collider.offset.x, 0.025f);
            }

            if (isGrounded) {
                if (m_deathTimer >= m_deathTime) {
                    m_rb.simulated = false;
                    m_collider.enabled = false;
                    Destroy(this);
                    m_deathTimer = 0f;
                }
            }
        }
    }

    public void TakeHit(Vector2 direction, float force) {
        m_rb.velocity = Vector2.zero;
        m_rb.AddForce(direction * force, ForceMode2D.Impulse);
        isDead = true;
        JuiceManager.Instance.StartImpact(m_hitFreezeFrames, m_hitShakeMag, m_hitShakeRghness, m_hitShakeFadeIn, m_hitShakeFadeOut);
        AudioManager.Instance.PlayAt(m_hitClip, transform.position, m_hitVolume);
        FindObjectOfType<LevelTimer>().AddBonus(m_deathTimeBonus);
        StartCoroutine(DisplayHit(.25f));
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

    IEnumerator DisplayHit(float seconds) {
        m_renderer.color = m_hitColour;
        yield return new WaitForSeconds(seconds);
        m_renderer.color = Color.white;
    }

    public void Activate(bool state) {
        StartSmoke();
        gameObject.SetActive(state);
    }

    void StartSmoke() {
        ParticleSystem smoke = Instantiate(m_smokePS, transform.position + m_smokeOffset, Quaternion.identity).GetComponent<ParticleSystem>();
        Destroy(smoke.gameObject, smoke.main.duration);   
    }

    public void OnAttack() {
        if (Vector3.Distance(transform.position, m_player.transform.position) <= m_range) {
            m_player.TakeHit(m_attckDirection, m_attckForce);
        }
    }
}
