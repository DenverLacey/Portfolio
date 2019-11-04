using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PuzzleLevers : PuzzleBase
{
    [Header("Lever Variables")]
    public List<InteractableLever> m_levers = new List<InteractableLever>();

    void FixedUpdate() {

        if (m_levers.TrueForAll(l => l.isPulled)) {
            Complete();
        }
    }
}
