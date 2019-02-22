#ifndef OGLBR_GUIELEM_H
#define OGLBR_GUIELEM_H

#include "../datastructures.h"

class OGLBR_GUIElem
{
public:
    OGLBR_GUIElem(GUIElement* _elem);
    ~OGLBR_GUIElem();
    void Buffer();
    void Render();
private:
    GUIElement* elem;
    shader_bitmask shading_state; // either normal or forced to MINIMAL_SHADER
    unsigned int tid;
    unsigned int VAO;
    vector<unsigned int> VBOs;
    unsigned int IBO;
};

#endif // OGLBR_GUIText_H
