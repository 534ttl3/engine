// OGLContextHandler.h //
#ifndef OGLContextHandler_H
#define OGLContextHandler_H

#include <SDL2/SDL.h>

class WindowHandler;

class OGLContextHandler
{
public:
    // init GLEW
    void InitGlew();
    // creates OpenGL Context, requires WindowHandler
    void CreateOpenGLContext(WindowHandler* hwindow);
    // sets viewing rectangle, calls glViewport
    void SetViewport(WindowHandler* hwindow);
    // sets OpenGL Attribute, requires SDL context
    void SetOpenGLAttribute(SDL_GLattr attr, unsigned int value);
};
#endif // OGLContextHandler_H
