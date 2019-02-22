#ifndef OGLBR_GUIBlock_H
#define OGLBR_GUIBlock_H

#include "../datastructures.h"

class GUIBlock;

class OGLBR_GUIBlock
{
public:
    OGLBR_GUIBlock(GUIBlock* _blockdata);
    void Buffer();
    void Render();
private:
    shader_bitmask shading_state;
    GUIBlock* blockdata;
    unsigned int VAO;
    vector<unsigned int> VBOs;
    unsigned int IBO;
};

#endif // OGLBR_GUIBlock_H
