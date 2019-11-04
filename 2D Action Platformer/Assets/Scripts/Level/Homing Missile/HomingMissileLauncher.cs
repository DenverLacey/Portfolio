using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class HomingMissileLauncher : Activatable
{
    public float m_fireDelay;
    public float m_missileMovSpeed;
    public float m_missileRotSpeed;
    public GameObject m_missilePF;
    public GameObject m_exposionPF;
    public Transform m_target;
    public Vector2 m_offset;
    private float m_timer;

    PlayerActor m_player;

    void Start() {
        PlayerActor m_player = m_target.GetComponent<PlayerActor>();
    }

    void Update() {
        if (m_active) {
            m_timer += Time.deltaTime;

            if (m_timer >= m_fireDelay) {
                GameObject go = Instantiate(m_missilePF, transform.position, Quaternion.identity);

                HomingMissile missile = go.GetComponent<HomingMissile>();
                missile.transform.up = m_target.position + (Vector3)m_offset - transform.position;
                missile.Init(m_missileMovSpeed, m_missileRotSpeed, m_target, m_offset, m_exposionPF);
                m_timer = 0f;
            }
        }
    }

    public override void Activate() {
        base.Activate();
        m_timer = m_fireDelay;
    }

    public override void Reactivate() {
        base.Activate();
        m_timer = 0.0f;
    }
}
