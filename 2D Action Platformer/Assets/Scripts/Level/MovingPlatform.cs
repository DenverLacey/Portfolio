using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MovingPlatform : MonoBehaviour
{
    public float m_movSpeed;

    public float m_range;
    public Vector3[] m_points;
    private int m_currentIndex;

    private PlayerActor m_player;

    void Start() {
        m_player = FindObjectOfType<PlayerActor>();
    }

    // Update is called once per frame
    void Update() {
        
        Vector3 desiredDir = (m_points[m_currentIndex] - transform.position).normalized;

        transform.Translate(desiredDir * m_movSpeed * Time.deltaTime);

        if (Vector3.Distance(transform.position, m_points[m_currentIndex]) <= m_range) {
            if (m_currentIndex >= m_points.Length - 1) {
                m_currentIndex = 0;
            }
            else {
                m_currentIndex++;
            }
        }
    }
}
