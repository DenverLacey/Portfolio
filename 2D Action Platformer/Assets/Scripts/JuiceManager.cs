using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class JuiceManager : MonoBehaviour
{
    private static JuiceManager m_instance;
    public static JuiceManager Instance { get => m_instance; }
    
    private float m_defaultFixedDeltaTime;

    private EZCameraShake.CameraShakeInstance m_shakeInstance;
    private float m_shakeInstanceFadeOut;

    void Awake() {

        if (m_instance == null) {
            m_instance = this;
        }
        else if (Instance != null) {
            Destroy(gameObject);
        }
    }

    private JuiceManager() {}

    void Start() {
        m_defaultFixedDeltaTime = Time.fixedDeltaTime;
    }

    public void StartCameraShake(float shakeMag, float shakeRghness, float fadeIn, float fadeOut) {
        m_shakeInstance = EZCameraShake.CameraShaker.Instance.StartShake(shakeMag, shakeRghness, fadeIn);
        m_shakeInstanceFadeOut = fadeOut;
    }

    public void EndCameraShake() {
        m_shakeInstance.StartFadeOut(m_shakeInstanceFadeOut);
    }

    public void StartImpact(int freezeFrames, float shakeMag, float shakeRghness, float fadeIn, float fadeOut) {
        FreezeForFrames(freezeFrames);
        EZCameraShake.CameraShaker.Instance.ShakeOnce(shakeMag, shakeRghness, fadeIn, fadeOut);
    }

    public void FreezeForFrames(int frames) {
        Time.timeScale = 0f;
        Time.fixedDeltaTime = 0f;
        StartCoroutine(StopFreezingIn(frames));
    }

    IEnumerator StopFreezingIn(int frames) {
        for (int i = 0; i < frames; i++) {
            yield return new WaitForEndOfFrame();
        }

        Time.timeScale = 1;
        Time.fixedDeltaTime = m_defaultFixedDeltaTime;
    }
}
