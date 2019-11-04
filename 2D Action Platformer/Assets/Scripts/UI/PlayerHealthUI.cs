using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using TMPro;

[RequireComponent(typeof(TextMeshPro))]
public class PlayerHealthUI : MonoBehaviour
{
    private PlayerActor m_player;
    private TextMeshPro m_textBox;

    void Start() {

        m_player = FindObjectOfType<PlayerActor>();
        m_textBox = GetComponent<TextMeshPro>();
    }

    void FixedUpdate() {

        m_textBox.text = m_player.m_health.ToString();
    }
}
