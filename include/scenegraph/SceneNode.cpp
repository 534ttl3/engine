#include "SceneNode.h"

SceneNode::~SceneNode()
{
    Destroy();
}

void SceneNode::Release()
{
    delete this;
}

void SceneNode::Update()
{
    for(std::list<SceneNode*>::iterator it = m_children.begin();
            it != m_children.end();
            it++)
        (*it)->Update();
}

//void SceneNode::UpdateLights()
//{
//    for(std::list<SceneNode*>::iterator it = m_children.begin();
//        it != m_children.end();
//        it++)
//    (*it)->UpdateLights();
//}

void SceneNode::Destroy()
{
    for(std::list<SceneNode*>::iterator it = m_children.begin();
            it != m_children.end();
            it++)
        (*it)->Release();

    m_children.clear();
}

void SceneNode::AddChild(SceneNode* pNode)
{
    m_children.push_back(pNode);
}
