using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class InteractableBase : MonoBehaviour
{
    [System.Serializable]
    public enum InteractionMode {
        ACKNOWLEDGE,
        IGNORE
    };

    [Header("Base Variables")]
    public string m_targetTag;
    public InteractionMode m_interactionMode;

    protected virtual void InteractionEnter(GameObject other) {}
    protected virtual void InteractionExit(GameObject other) {}
    protected virtual void InteractionStay(GameObject other) {}

    protected virtual void OnTriggerEnter2D(Collider2D other) {

        switch (m_interactionMode) {
            case InteractionMode.ACKNOWLEDGE:
                if (other.tag == m_targetTag) {
                    InteractionEnter(other.gameObject);
                }
                break;

            case InteractionMode.IGNORE:
                if (other.tag != m_targetTag) {
                    InteractionEnter(other.gameObject);
                }
                break;

            default:
                Debug.LogError("Couldn't determine tag.", other.gameObject);
                break;
        }
    }

    protected virtual void OnTriggerExit2D(Collider2D other) {

        switch (m_interactionMode) {
            case InteractionMode.ACKNOWLEDGE:
                if (other.tag == m_targetTag) {
                    InteractionExit(other.gameObject);
                }
                break;

            case InteractionMode.IGNORE:
                if (other.tag != m_targetTag) {
                    InteractionExit(other.gameObject);
                }
                break;

            default:
                Debug.LogError("Couldn't determine tag.", other.gameObject);
                break;
        }
    }

    protected virtual void OnTriggerStay2D(Collider2D other) {

        switch (m_interactionMode) {
            case InteractionMode.ACKNOWLEDGE:
                if (other.tag == m_targetTag) {
                    InteractionStay(other.gameObject);
                }
                break;

            case InteractionMode.IGNORE:
                if (other.tag != m_targetTag) {
                    InteractionStay(other.gameObject);
                }
                break;

            default:
                Debug.LogError("Couldn't determine tag.", other.gameObject);
                break;
        }
    }
}
