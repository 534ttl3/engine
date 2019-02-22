#ifndef OBJECTNODE_H
#define OBJECTNODE_H

#include <list>

#include "SceneNode.h"

class ObjectNode : public SceneNode
{
public:
    void Update()
    {
        for(std::list<SceneNode*>::iterator it = m_children.begin();
                it != m_children.end();
                it++)
        {
            (*it)->Update();
        }
    }
};


#endif // OBJECTNODE_H
