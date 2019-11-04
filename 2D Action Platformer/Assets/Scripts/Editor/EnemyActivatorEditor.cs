using UnityEngine;
using UnityEditor;

[CustomEditor(typeof(EnemyActivator))]
public class EnemyActivatorEditor : Editor
{

    public override void OnInspectorGUI() {
        base.OnInspectorGUI();

        EnemyActivator enemyActivator = (EnemyActivator)target;

        if (enemyActivator.m_openDoor) {
            bool allowSceneObjects = !EditorUtility.IsPersistent(target);
            enemyActivator.m_doorActor = (DoorActor)EditorGUILayout.ObjectField("Door Actor", enemyActivator.m_doorActor, typeof(DoorActor), allowSceneObjects);
        }
        else {
            enemyActivator.m_doorActor = null;
        }
    }
}
