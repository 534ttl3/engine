
#include "GUIElementNode.h"

#include "../opengl/OGLBR_GUIElem.h"

GUIElementNode::GUIElementNode(GUIElement* _elem)
{
    if(_elem)
        elem = _elem;
    else
    {
        cout << "Error GUIElementNode: invalid GUIElementNode" << endl;
        Release();
    }
    // OpenGL context has to be initialized first

    brt = new OGLBR_GUIElem(_elem);
    brt->Buffer();
}

void GUIElementNode::Update()
{
    if(elem->changed)
    {
        brt->Buffer();
        elem->changed = false;
    }

    brt->Render();
    // update children, which is nonsense
    // since this textnode has no children
    SceneNode::Update();
}
