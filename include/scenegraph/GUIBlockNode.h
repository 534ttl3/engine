#ifndef GUIBLOCKNODE_H
#define GUIBLOCKNODE_H

#include "SceneNode.h"
#include "../datastructures.h"

class OGLBR_GUIBlock;

class GUIBlockNode : public SceneNode
{
public:
    GUIBlockNode(GUIBlock* _block);
    ~GUIBlockNode();
    void Update();
private:
    GUIBlock* block;
    OGLBR_GUIBlock* bufferrenderer;
};

#endif // GUIBLOCKNODE_H
