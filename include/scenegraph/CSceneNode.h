#ifndef CSceneNode_H
#define CSceneNode_H


#include "SceneNode.h"
#include <list>

class CSceneNode : public SceneNode
{
public:
    void Update()
    {

        for(std::list<SceneNode*>::iterator it = m_children.begin();
                it != m_children.end(); it++)
        {
            (*it)->Update();
        }
    }
};

#endif // CSceneNode_H
