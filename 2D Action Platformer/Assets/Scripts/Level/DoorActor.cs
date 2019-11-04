using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(BoxCollider2D))]
public class DoorActor : MonoBehaviour
{
    public delegate bool OpenCondition();

    public float m_movSpeed;
    public Vector3 m_openPosition;
    public float m_disableDistance;

    public AudioClip m_openingSound;
    public float m_openingSoundVolume = 1.0f;

    public float m_doorShakeMag;
    public float m_doorShakeRghness;
    public float m_doorShakeFadeIn;
    public float m_doorShakeFadeOut;

    private bool m_active;
    private bool m_activated;
    private OpenCondition m_condition = null;
    private BoxCollider2D m_collider;

    private Vector3 m_movDirection;

    public bool isActive { get => m_active; set => m_active = value; }

    public bool m_autoDestroy = true;

    void Start() {
        m_collider = GetComponent<BoxCollider2D>();
        m_movDirection = m_openPosition - transform.position;
        m_movDirection.Normalize();
    }

    public void Link(OpenCondition condition) {
        m_condition = condition;
    }

    public void Unlink() {
        m_condition = null;
    }

    public void ManualDestroy() {
        Destroy(gameObject);
    }

    void FixedUpdate() {
        if (!m_active) {
            if (m_condition != null && m_condition() && !m_activated) {
                m_active = true;
                m_activated = true;
                AudioManager.Instance.PlayAt(m_openingSound, transform.position, m_openingSoundVolume);
                JuiceManager.Instance.StartCameraShake(m_doorShakeMag, m_doorShakeRghness, m_doorShakeFadeIn, m_doorShakeFadeOut);
            }
        }
        else {
            m_collider.enabled = false;
            transform.Translate(m_movDirection * m_movSpeed * Time.deltaTime);

            if (Vector3.Distance(transform.position, m_openPosition) <= m_disableDistance) {
                m_active = false;
                JuiceManager.Instance.EndCameraShake();

                if (m_autoDestroy) {
                    Destroy(gameObject);
                }
            }
        }
    }
}
