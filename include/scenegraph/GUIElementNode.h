#ifndef GUIELEMENTNODE_H
#define GUIELEMENTNODE_H

#include "SceneNode.h"
#include "../datastructures.h"

class OGLBR_GUIElem;

class GUIElementNode : public SceneNode
{
public:
    GUIElementNode(GUIElement* _elem);
    void Update();

private:
    GUIElement* elem;
    OGLBR_GUIElem* brt;
};

#endif // GUIELEMENTNODE_H
