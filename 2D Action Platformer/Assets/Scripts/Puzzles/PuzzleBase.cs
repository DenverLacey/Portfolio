using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PuzzleBase : MonoBehaviour {
    
    [System.Serializable]
    public enum CompleteAction {
        NONE,
        OPEN_DOOR
    }

    [System.Serializable]
    public enum CompleteEffect {
        NONE,
        FOCUS_DOOR
    }

    [Header("Base Variables")]
    public CompleteAction m_completeAction;

    public CompleteEffect m_completeEffect;
    public float m_completeEffectDuration;

    public bool m_deactivateLinkedObjectsOnComplete;
    public bool m_reactivateLinkedObjectsOnReset;
    public List<Activatable> m_linkedObjects = new List<Activatable>();

    [HideInInspector]
    public DoorActor m_door;

    private bool m_complete;

    void Start() {
        if (m_completeEffect == CompleteEffect.FOCUS_DOOR) {
            m_door.m_autoDestroy = false;
        }

        if (m_door) {
            m_door.Link(ActivateDoorOnComplete);
        }
    }

    public bool Complete() {
        if (!m_complete) {
            m_complete = true;
            OnComplete();
            return true;
        }
        return false;
    }

    bool ActivateDoorOnComplete() {
        return m_complete;
    }

    protected virtual void OnComplete() {
        if (m_deactivateLinkedObjectsOnComplete) {
            m_linkedObjects.ForEach(o => o.Deactivate());
        }

        switch (m_completeAction) {
            case CompleteAction.NONE:
                // do nothing
                break;

            case CompleteAction.OPEN_DOOR:
                // door takes care of it in its FixedUpdate
                break;

            default:
                Debug.LogError("Couldn't determine complete action", this);
                break;
        }

        switch (m_completeEffect) {
            case CompleteEffect.NONE:
                // do nothing
                break;

            case CompleteEffect.FOCUS_DOOR: {
                TrackingCamera cam = FindObjectOfType<TrackingCamera>();
                cam.m_target = m_door.transform;
                FindObjectOfType<PlayerActor>().CanMove = false;
                StartCoroutine(ResetCompleteEffect());
                break;
            }

            default:
                Debug.LogError("Couldn't determine complete effect", this);
                break;
        }
    }

    protected virtual IEnumerator ResetCompleteEffect() {
        yield return new WaitForSeconds(m_completeEffectDuration);

        if (m_reactivateLinkedObjectsOnReset) {
            m_linkedObjects.ForEach(o => o.Reactivate());
        }

        switch (m_completeEffect) {
            case CompleteEffect.NONE:
                // do nothing
                break;

            case CompleteEffect.FOCUS_DOOR: {
                TrackingCamera cam = FindObjectOfType<TrackingCamera>();
                cam.m_target = FindObjectOfType<PlayerActor>().transform;
                FindObjectOfType<PlayerActor>().CanMove = true;
                m_door.ManualDestroy();
                break;
            }

            default:
                Debug.LogError("Couldn't determine complete effect", this);
                break;
        }
    }
}
