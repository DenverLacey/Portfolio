using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(Collider2D))]
[RequireComponent(typeof(Rigidbody2D))]
public class HomingMissileLauncherActivator : MonoBehaviour
{
    public List<HomingMissileLauncher> m_launchers = new List<HomingMissileLauncher>();

    void OnTriggerEnter2D(Collider2D info) {

        if (info.tag == "Player") {
            foreach(HomingMissileLauncher l in m_launchers) {
                l.Activate();
            }
        }
    }

    void OnTriggerExit2D(Collider2D info) {

        if (info.tag == "Player") {
            foreach(HomingMissileLauncher l in m_launchers) {
                l.Deactivate();
            }
        }
    }
}
