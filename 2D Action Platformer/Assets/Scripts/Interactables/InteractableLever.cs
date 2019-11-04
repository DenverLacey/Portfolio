using UnityEngine;
using TMPro;

public class InteractableLever : InteractableBase {
    
    [Header("Lever Variables")]
    public Sprite[] m_sprites;

    private bool m_pulled;
    private bool m_near;
    private TextMeshPro m_text;
    private SpriteRenderer m_sprite;
    private PlayerActor m_playerActor;

    public bool isPulled { get => m_pulled; }

    void Start() {
        
        m_sprite = GetComponentInChildren<SpriteRenderer>();
        m_text = GetComponentInChildren<TextMeshPro>();
        m_text.enabled = false;
    }

    void Update() {

        if (m_near) {
            if (Input.GetKeyDown(m_playerActor.m_useButton)) {
                m_pulled = !m_pulled;
                m_sprite.sprite = m_sprites[ToInt(m_pulled)];
            }
        }
    }

    protected override void InteractionEnter(GameObject other) {

        m_playerActor = other.GetComponent<PlayerActor>();
        m_text.text = m_playerActor.m_useButton.ToString();
        m_text.enabled = true;
        m_near = true;
    }

    protected override void InteractionExit(GameObject other) {

        m_playerActor = null;
        m_text.text = "";
        m_text.enabled = false;
        m_near = false;
    }

    protected override void OnTriggerStay2D(Collider2D other) {}
    private int ToInt(bool b) { return b ? 1 : 0; }
}
