using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Events;

[RequireComponent(typeof(Collider2D))]
public class GroundDetector : MonoBehaviour
{
    public delegate void OnDetection(int addedContacts); 
    private OnDetection m_onDetected;

    public void Link(OnDetection func) {
        m_onDetected = func;
    }

    void OnTriggerEnter2D(Collider2D other) {
        if (other.tag == "Solid") {
            m_onDetected(1);
        }
    }

    void OnTriggerExit2D(Collider2D other) {
        if (other.tag == "Solid") {
            m_onDetected(-1);
        }
    }
}
