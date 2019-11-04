using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(Collider2D))]
[RequireComponent(typeof(Rigidbody2D))]
public class EnemyActivator : MonoBehaviour
{
    public List<EnemyActor> m_enemyActors = new List<EnemyActor>();
    public AudioClip m_enemyActivateClip;
    public float m_enemyActivateVolume;

    [Space]
    public bool m_openDoor;

    [HideInInspector]
    public DoorActor m_doorActor;

    void Awake() {

        foreach (EnemyActor e in m_enemyActors) {
            e.gameObject.SetActive(false);
        }
    }

    void Start() {

        if (m_doorActor != null) {
            m_doorActor.Link(AllDead);
        }
    }

    void OnTriggerEnter2D(Collider2D other) {

        if (other.tag == "Player") {
            PlayerActor player = other.gameObject.GetComponent<PlayerActor>();
            if (player.CanMove && !player.IsDead) {
                foreach (EnemyActor e in m_enemyActors) {
                    e.Activate(true);
                }
                player.SetCurrentEnemies(m_enemyActors);
                if (!AllDead()) { AudioManager.Instance.PlayAt(m_enemyActivateClip, transform.position, m_enemyActivateVolume); }
            }
        }
    }

    void OnTriggerExit2D(Collider2D other) {

        if (other.tag == "Player") {
            PlayerActor player = other.gameObject.GetComponent<PlayerActor>();
            if (player.CanMove && !player.IsDead) {
                player.SetCurrentEnemies(new List<EnemyActor>());
                foreach(EnemyActor e in m_enemyActors) {
                    e.Activate(false);
                }
                if (!AllDead()) { AudioManager.Instance.PlayAt(m_enemyActivateClip, transform.position, m_enemyActivateVolume); }
            }
        }
    }

    bool AllDead() {

        return m_enemyActors.TrueForAll(e => e.isDead); 
    }
}
