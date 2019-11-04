using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(AudioSource))]
public class AudioManager : MonoBehaviour
{
    private static AudioManager m_instance;
    public static AudioManager Instance { get => m_instance; }

    public GameObject m_oneShotPF;
    public AudioClip[] m_bckgrndMusic;
    private AudioSource m_musicSource;

    float m_defaultVolume;
    float m_timer;

    int m_currentBckgrndMusic;

    private AudioManager() {}

    void Awake() {
        if (m_instance == null) {
            m_instance = this;
        }
        else if (Instance != null) {
            Destroy(gameObject);
        }
    }

    void Start() {
        m_musicSource = GetComponent<AudioSource>();

        m_currentBckgrndMusic = Random.Range(0, m_bckgrndMusic.Length);

        m_musicSource.clip = m_bckgrndMusic[m_currentBckgrndMusic];
        m_musicSource.Play();

        m_defaultVolume = m_musicSource.volume;
        m_timer = m_musicSource.clip.length - 1f;
    }

    void Update() {
        m_timer -= Time.deltaTime;

        if (m_timer <= 0) {
            StartCoroutine(Transition(m_bckgrndMusic[SelectRandomBackgroundMusic()]));
            m_timer = float.MaxValue;
        }
    }

    #region MUSIC FUNCTIONS
        public void PlayMusic() {
            if (!m_musicSource.isPlaying) {
                m_musicSource.Play();
            }
        }

        public void StopMusic() {
            if (m_musicSource.isPlaying) {
                m_musicSource.Stop();
            }
        }

        public void PauseMusic() {
            if (m_musicSource.isPlaying) {
                m_musicSource.Pause();
            }
        }

        public void UnPauseMusic() {
            if (!m_musicSource.isPlaying) {
                m_musicSource.UnPause();
            }
        }

        IEnumerator Transition(AudioClip clip, float fadeOut = 0.005f) {
            while (m_musicSource.volume > 0) {
                m_musicSource.volume -= fadeOut;
                yield return new WaitForEndOfFrame();
            }

            m_musicSource.volume = 0f;
            m_musicSource.Stop();
            m_musicSource.clip = clip;
            m_musicSource.volume = m_defaultVolume;
            m_musicSource.Play();
            m_timer = m_musicSource.clip.length - 1f;
        }
    #endregion
    #region AUDIO FUNCTIONS
        public void PlayAt(AudioClip clip, Vector3 position, float volume = 1f) {
            OneShotSFX newAudio = Instantiate(m_oneShotPF, position, Quaternion.identity).GetComponent<OneShotSFX>();
            newAudio.Init(clip, volume);
        }
    #endregion

    private int SelectRandomBackgroundMusic() {
        int newIndex = 0;
        newIndex = Random.Range(0, m_bckgrndMusic.Length);
        newIndex = newIndex != m_currentBckgrndMusic ? newIndex : (newIndex + 1) % m_bckgrndMusic.Length;
        m_currentBckgrndMusic = newIndex;
        return m_currentBckgrndMusic;
    }
}


