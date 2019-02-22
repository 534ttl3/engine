#include "GUIBlockNode.h"
#include "../opengl/OGLBR_GUIBlock.h"

GUIBlockNode::GUIBlockNode(GUIBlock* _block)
{
    if(_block)
        block=_block;
    else
        cout << "Error GUIBlockNode: no valid GUIBlock" << endl;

    bufferrenderer = new OGLBR_GUIBlock(block);
    bufferrenderer->Buffer();
}

void GUIBlockNode::Update()
{
    if(block)
    {
        if(block->HasSomeElementChanged())
        {
            block->RearrangeElements();
            bufferrenderer->Buffer();
        }

        bufferrenderer->Render();
        SceneNode::Update();
    }
    else
    {
        cout << "Error GUIBlockNode: no valid GUIBlock" << endl;
        SceneNode::Release();
    }
}

GUIBlockNode::~GUIBlockNode()
{
    delete block;
    delete bufferrenderer;
}

