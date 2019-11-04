using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(Rigidbody2D))]
public class HomingMissile : MonoBehaviour
{
    public float m_movSpeed;
    public float m_rotSpeed;
    public float m_expForce;
    public Transform m_target;
    public Vector2 m_offset;
    public GameObject m_explosionPF;

    private Rigidbody2D m_rb;

    PlayerActor m_player;

    void Start() {
        m_rb = GetComponent<Rigidbody2D>();
    }

    public void Init(float movSpeed, float rotSpeed, Transform target, Vector2 offset, GameObject explosionPrefab) {
        m_movSpeed = movSpeed;
        m_rotSpeed = rotSpeed;
        m_target = target;
        m_offset = offset;
        m_explosionPF = explosionPrefab;

        m_player = m_target.GetComponent<PlayerActor>();
    }

    void FixedUpdate() {
        // if player can't move
        if (m_player && !m_player.CanMove || m_player.IsDead) {
            Destroy(gameObject);
        }

        // rotate
        Vector2 direction = (Vector2)m_target.position + m_offset - m_rb.position;
        direction.Normalize();
        float rotateAmount = Vector3.Cross(direction, transform.up).z;
        m_rb.angularVelocity = -rotateAmount * m_rotSpeed;
       
        // move
        m_rb.velocity = transform.up * m_movSpeed * Time.deltaTime;
    }

    void OnCollisionEnter2D(Collision2D info) {
        if (info.collider.tag == "Player") {
            PlayerActor player = info.collider.gameObject.GetComponent<PlayerActor>();
            if (player) {
                player.TakeHit(Vector3.up, m_expForce);
            }
        }

        GameObject ps = Instantiate(m_explosionPF, info.GetContact(0).point, Quaternion.FromToRotation(Vector3.up, info.GetContact(0).normal));
        Destroy(ps, 4f);
        Destroy(gameObject);
    }
}
