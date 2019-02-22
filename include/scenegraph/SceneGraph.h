#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

struct SceneData;

#include "SceneNode.h"

class SceneGraph
{
public:
    void AssignSceneGraph(SceneData* scene_data);
    void SetRootNode(SceneNode* _root);
    void UpdateGraph();
    SceneNode* GetRootNode();

private:
    SceneNode* root;
};

#endif // SCENEGRAPH_H
