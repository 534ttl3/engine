#ifndef SCENENODE_H
#define SCENENODE_H

#include <list>

// base class of all nodes a scene contains

class SceneNode
{
public:
    // destructor - calls Destroy();
    virtual ~SceneNode();
    // release this object from memory with all its child nodes
    void Release(); // delete calls destructor which destroys child nodes

    // update our scene node
    virtual void Update(); // update gets overwritten by derived node classes

    // destroy all children
    void Destroy();

    // add a child to our custody
    void AddChild(SceneNode* pNode);
protected:
    // list of all children
    std::list<SceneNode*> m_children;
};


#endif // SCENENODE_H
