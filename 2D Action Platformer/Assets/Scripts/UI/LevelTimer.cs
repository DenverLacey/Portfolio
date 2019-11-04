using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;

[RequireComponent(typeof(TextMeshProUGUI))]
public class LevelTimer : MonoBehaviour
{
    public int m_flashes = 3;
    public float m_flashDelay = 0.1f;
    public Color m_bonusFlashColour = Color.green;
    public Color m_penaltyFlashColour = Color.red;

    private float m_timeElapsed;
    private TextMeshProUGUI m_text;

    private bool m_isRunning;

    void Start() {
        m_text = GetComponent<TextMeshProUGUI>();
    }

    // Update is called once per frame
    void Update() {
        
        if (m_isRunning) {
            m_timeElapsed += Time.deltaTime;
        }

        string timeStr = ((int)(m_timeElapsed / 60f)).ToString("00") + ':' + 
		((int)(m_timeElapsed % 60f)).ToString("00") + "." + 
		Mathf.Clamp(((m_timeElapsed % 60f) - ((int)(m_timeElapsed % 60f))) * 100, 0f, 99f).ToString("00");
        
        m_text.text = timeStr;
    }

    public void StartTimer() {

        m_isRunning = true;
    }

    public void AddPenalty(float penalty) {

        m_timeElapsed += penalty;

        StopAllCoroutines();
        StartCoroutine(Flash(m_penaltyFlashColour));
    }

    public void AddBonus(float bonus) {

        m_timeElapsed -= bonus;
        StopAllCoroutines();
        StartCoroutine(Flash(m_bonusFlashColour));
    }

    IEnumerator Flash(Color colour) {

        for (int f = 0; f < m_flashes; f++) {
            m_text.color = colour;
            yield return new WaitForSeconds(m_flashDelay);
            m_text.color = Color.white;
            yield return new WaitForSeconds(m_flashDelay);
        }
    }
}
