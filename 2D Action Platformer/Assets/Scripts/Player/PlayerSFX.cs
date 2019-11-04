using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(PlayerActor))]
public class PlayerSFX : MonoBehaviour
{
    [Header("Footsteps")]
    public AudioClip m_movClip;
    public float m_movVolume;

    [Space]
    public float m_footstepDelay;

    [Header("Jump")]
    public AudioClip m_jmpClip;
    public float m_jmpVolume;

    [Header("Attack")]
    public AudioClip m_attckClip;
    public float m_attckVolume;

    [Header("Hit")]
    public AudioClip m_hitClip;
    public float m_hitVolume;

    [Header("Death")]
    public AudioClip m_deathClip;
    public float m_deathVolume;

    private PlayerActor m_playerActor;
    private float m_footstepTimer;

    private bool m_playerHurting;

    private int m_playerLives;

    // Start is called before the first frame update
    void Start() {
        m_playerActor = GetComponent<PlayerActor>();
        m_footstepTimer = m_footstepDelay / 2f;
        m_playerLives = m_playerActor.m_health;
    }

    public void PlayAttack() {
        AudioManager.Instance.PlayAt(m_attckClip, transform.position, m_attckVolume);
    }

    public void PlayJump() {
        AudioManager.Instance.PlayAt(m_jmpClip, transform.position, m_jmpVolume);
    }

    public void PlayHit() {
        AudioManager.Instance.PlayAt(m_hitClip, transform.position, m_hitVolume);
    }

    public void PlayDeath() {
        AudioManager.Instance.PlayAt(m_deathClip, transform.position, m_deathVolume);
    }

    public void PlayFootstep() {
        AudioManager.Instance.PlayAt(m_movClip, transform.position, m_movVolume);
    }
}
