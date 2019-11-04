using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(AudioSource))]
public class OneShotSFX : MonoBehaviour
{
    private AudioSource m_audioSource;

    bool m_playing;

    public OneShotSFX Init(AudioClip clip, float volume = 1f) {

        m_audioSource = GetComponent<AudioSource>();
        m_audioSource.clip = clip;
        m_audioSource.volume = volume;
        m_audioSource.Play();
        m_playing = true;
        return this;
    }

    void Update() {

        if (m_playing && !m_audioSource.isPlaying) {
            Destroy(gameObject);
        }
    }
}
