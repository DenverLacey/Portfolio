using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;

public class InteractableSign : InteractableBase
{
    private TextMeshPro m_dialogueBox;

    void Start() {

        m_dialogueBox = GetComponentInChildren<TextMeshPro>();
        m_dialogueBox.enabled = false;
    }

    protected override void InteractionEnter(GameObject other) {

        m_dialogueBox.enabled = true;
    }

    protected override void InteractionExit(GameObject other) {

        m_dialogueBox.enabled = false;
    }
}
