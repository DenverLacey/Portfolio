using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TrackingCamera : MonoBehaviour
{
    [Header("Destination")]
    public Transform m_target;
    public Vector3 m_offset;
    
    [Header("Movement")]
    public float m_dampTime;
    public float m_maxSpeed;

    private Vector3 m_velocity = Vector3.zero;

    // Update is called once per frame
    void FixedUpdate() {
        transform.position = Vector3.SmoothDamp(transform.position, m_target.position + m_offset, ref m_velocity, m_dampTime * Time.deltaTime, m_maxSpeed * Time.deltaTime);
    }
}
