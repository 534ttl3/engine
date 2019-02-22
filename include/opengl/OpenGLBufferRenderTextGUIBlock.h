#ifndef OPENGLBUFFERRENDERTEXT_H
#define OPENGLBUFFERRENDERTEXT_H

#include "../datastructures.h"

class OpenGLBufferRenderText
{
public:
    OpenGLBufferRenderText(GUIElementText* _text);
    void Buffer();
    void Render();
private:
    GUIElementText* text;
    shader_bitmask shading_state; // either normal or forced to MINIMAL_SHADER
    unsigned int tid;
    unsigned int VAO;
};

#endif // OPENGLBUFFERRENDERTEXT_H
