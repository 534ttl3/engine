#ifndef MESHNODE_H
#define MESHNODE_H

#include "SceneNode.h"
#include "../datastructures.h"
#include "../opengl/OGLBR_Geom.h"

class GeomNode : public SceneNode
{
public:

    GeomNode(SceneData* scenedata, unsigned int object_id, unsigned int mesh_id)
    {
        bufferrenderer = new OGLBR_Geom(scenedata, object_id, mesh_id);
        // fill OpenGL Buffers and set changed to false because all changes of the
        // attribute data has been applied
        mesh = scenedata->objects[object_id]->meshes[mesh_id];
        bufferrenderer->Buffer();
    }

    ~GeomNode()
    {
        delete bufferrenderer;
    }


    void Update()
    {
        // OpenGL Shader passing and Render calls
        bufferrenderer->Render();
        // CSceneNode::Update(); I don't really know what that should be doing
    }

private:
    MeshData* mesh;
    OGLBR_Geom* bufferrenderer;
};

#endif // MESHNODE_H
