#ifndef SCENEHANDLER_H
#define SCENEHANDLER_H

#include "datastructures.h"

#include <vector>

class CSceneImp;
class SceneGraph;

class SceneHandler
{
public:
    SceneHandler();
    ~SceneHandler();
    void LoadScene(unsigned int id);
    void BufferToSceneGraph(unsigned int id);
    void ActivateScene(unsigned int id);
    int GetActiveSceneID();
    void UpdateScene();
    SceneData* GetCurrentScene();
private:
    std::vector<SceneData*> SceneDataStorer;
    int activescene;
    std::vector<SceneGraph*> scenegraphs;
};

#endif // SCENEHANDLER_H
