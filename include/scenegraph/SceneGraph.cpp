#include "ObjectNode.h"
#include "GeomNode.h"
#include "GUIElementNode.h"
#include "GUIBlockNode.h"
#include "CSceneNode.h"
#include "TransformNode.h"
#include "../datastructures.h"

#include "SceneGraph.h"

void SceneGraph::AssignSceneGraph(SceneData* scene_data)
{
    // create CSceneNode
    CSceneNode* c_scene_node = new CSceneNode();

    for(unsigned int i=0; i<scene_data->objects.size(); i++)
    {
        TransformNode* object_transform_node = new TransformNode(&scene_data->objects[i]->tr_matrix);

        ObjectNode* object_node = new ObjectNode();

        for(unsigned int j=0; j<scene_data->objects[i]->meshes.size(); j++)
        {
            // in case the single meshes of an object are transformed
            TransformNode* mesh_transform_node = new TransformNode(&scene_data->objects[i]->meshes[j]->tr_matrix);

            // assign a poitner to scene to the GeomNode calling the Renderer
            GeomNode* mesh_node = new GeomNode(scene_data, scene_data->objects[i]->id, scene_data->objects[i]->meshes[j]->id);

            // make the mesh node a child of the mesh transform node
            mesh_transform_node->AddChild(mesh_node);
            // make the mesh transform node a child of the object node
            object_node->AddChild(mesh_transform_node);
        }

        // make the object node a child of the object transform node
        object_transform_node->AddChild(object_node);

        // make the object transform node a child of the c_scene node
        c_scene_node->AddChild(object_transform_node);
    }

    // switch node for GUIBlockNodes
    SceneNode* guiblockswitch = new SceneNode();
    for(std::vector<GUIBlock*>::iterator it=scene_data->gui->blocks.begin();
            it != scene_data->gui->blocks.end(); it++)
    {
        GUIBlockNode* guiblocknode = new GUIBlockNode(*it);
        for(std::vector<GUIElement*>::iterator jt=(*it)->elements.begin();
                jt != (*it)->elements.end(); jt++)
        {
            GUIElementNode* elemnode = new GUIElementNode(*jt);
            guiblocknode->AddChild(elemnode);
        }

        guiblockswitch->AddChild(guiblocknode);
    }

    // create transformation node for whole scene
    TransformNode* scene_transform_node = new TransformNode(&scene_data->tr_matrix);

    // make the c_scene node a child of the scene transformation node
    scene_transform_node->AddChild(c_scene_node);

    // make the scene transform node a child of the root node
    SceneNode* root = new SceneNode();
    root->AddChild(scene_transform_node);
    root->AddChild(guiblockswitch);
    SetRootNode(root);
}

void SceneGraph::SetRootNode(SceneNode* _root)
{
    root = _root;
}

void SceneGraph::UpdateGraph()
{
    // then render geometry
    if(root != nullptr)
        root->Update();
    else
        cout << "Error SceneGraph::UpdateGraph(): no root node set" << endl;
}

SceneNode* SceneGraph::GetRootNode()
{
    return root;
}
