using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Activatable : MonoBehaviour
{
    protected bool m_active;

    public virtual void Activate() {
        m_active = true;
    }

    public virtual void Deactivate() {
        m_active = false;
    }

    public virtual void Reactivate() {
        Activate();
    }
}
