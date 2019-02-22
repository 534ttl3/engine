#include "CSceneImp.h"
#include "scenegraph/SceneGraph.h"
#include "scenegraph/SceneNode.h"

#include "SceneHandler.h"

SceneHandler::SceneHandler()
{
    // no scene is active
    activescene = -1;
}

void SceneHandler::LoadScene(unsigned int id)
{
    CSceneImp* c_scene_importer = new CSceneImp();
    SceneData* sc_data = new SceneData();
    c_scene_importer->LoadComplexSceneFromFile("assets/scene1", sc_data);
    SceneDataStorer.push_back(sc_data);
}

void SceneHandler::BufferToSceneGraph(unsigned int id)
{
    SceneGraph* scene_graph = new SceneGraph();
    scene_graph->AssignSceneGraph(SceneDataStorer[id]);
    scenegraphs.push_back(scene_graph);
}

void SceneHandler::ActivateScene(unsigned int id)
{
    activescene = id;
}

int SceneHandler::GetActiveSceneID()
{
    if(activescene != -1)
        return activescene;
    else
        cout << "Error GetActiveSceneID: no scene is active" << endl;
}

void SceneHandler::UpdateScene()
{
    scenegraphs[activescene]->UpdateGraph();
}

SceneData* SceneHandler::GetCurrentScene()
{
    return SceneDataStorer[activescene];
}

SceneHandler::~SceneHandler()
{
    for(unsigned int i=0; i<scenegraphs.size(); i++)
        delete scenegraphs[i];
}
