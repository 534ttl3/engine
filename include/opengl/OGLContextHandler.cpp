
#include "../window/WindowHandler.h"

#include "OGLContextHandler.h"

#include <GL/glew.h>
#include <iostream>
using namespace std;

void OGLContextHandler::InitGlew()
{
    // init Glew
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        // Problem: glewInit failed, something is seriously wrong.
        cout << "Error: " << glewGetErrorString(err) << endl;
    }
    else
    {
        cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << endl;
    }
}


void OGLContextHandler::CreateOpenGLContext(WindowHandler* hwindow)
{
    SDL_GL_CreateContext(hwindow->GetCurrentWindow());
}

void OGLContextHandler::SetViewport(WindowHandler* hwindow)
{
    int width, height;
    SDL_GetWindowSize(hwindow->GetCurrentWindow(), &width, &height);
    cout << width << " " << height << endl;
    glViewport(0, 0, 1280, 720);
}

void OGLContextHandler::SetOpenGLAttribute(SDL_GLattr attr, unsigned int value)
{
    SDL_GL_SetAttribute(attr, value);
}
