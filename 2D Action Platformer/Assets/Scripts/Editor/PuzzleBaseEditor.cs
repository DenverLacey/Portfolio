using UnityEditor;
using UnityEngine;

[CustomEditor(typeof(PuzzleBase), true)]
public class PuzzleBaseEditor : Editor
{
    public override void OnInspectorGUI() {
        base.OnInspectorGUI();

        PuzzleBase pz = (PuzzleBase)target;

        if (pz.m_completeAction == PuzzleBase.CompleteAction.OPEN_DOOR) {
            bool allowSceneObjects = !EditorUtility.IsPersistent(target);
            pz.m_door = (DoorActor)EditorGUILayout.ObjectField("Door Actor", pz.m_door, typeof(DoorActor), allowSceneObjects);
        }
        else {
            pz.m_door = null;
        }
    }
}
