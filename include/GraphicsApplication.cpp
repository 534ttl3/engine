
#include "window/WindowHandler.h"
#include "opengl/OGLContextHandler.h"
#include "input/InputHandler.h"
#include "SceneHandler.h"

#include "opengl/shaderhandling.h"

#include "GraphicsApplication.h"

#include <iostream>
#include <stdio.h>
#include <vector>

#include <GL/glew.h>
#include <SDL2/SDL.h>
using namespace std;

GraphicsApplication::GraphicsApplication()
{
    m_OGLContextHandler = new OGLContextHandler();
    m_windowhandler = new WindowHandler();
    m_eventlistener = new InputHandler();
    m_scenehandler = new SceneHandler();

    running = true;
}

void GraphicsApplication::LoadScenes()
{
    m_scenehandler->LoadScene(0);
}

void GraphicsApplication::InitGraphicsEnvironment()
{
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        cout << "Error SDL_Init(): not correctly initialized" << endl;
    }
    else
    {
        cout << "SDL_Init(SDL_INIT_EVERYTHING) successful" << endl;
    }
    m_windowhandler->CreateOpenGLWindow("title", 500, 100, 1280, 720);
    m_OGLContextHandler->CreateOpenGLContext(m_windowhandler);
    m_OGLContextHandler->SetOpenGLAttribute(SDL_GL_DOUBLEBUFFER, 1);
    m_OGLContextHandler->SetOpenGLAttribute(SDL_GL_RED_SIZE, 8);
    m_OGLContextHandler->SetOpenGLAttribute(SDL_GL_GREEN_SIZE, 8);
    m_OGLContextHandler->SetOpenGLAttribute(SDL_GL_BLUE_SIZE, 8);
    m_OGLContextHandler->SetOpenGLAttribute(SDL_GL_ALPHA_SIZE, 8);
    m_OGLContextHandler->SetOpenGLAttribute(SDL_GL_DEPTH_SIZE, 16);
    m_OGLContextHandler->InitGlew();
    m_OGLContextHandler->SetViewport(m_windowhandler);
}

void GraphicsApplication::UpdateGraphics()
{
    m_scenehandler->UpdateScene();
}

bool GraphicsApplication::IsRunning()
{
    return running;
}

void GraphicsApplication::EndGraphicsApplication()
{
    running = false;
}

void GraphicsApplication::CleanAndCloseGraphicsApplication()
{
    SDL_DestroyWindow(m_windowhandler->GetCurrentWindow());
    SDL_Quit();
}

InputHandler* GraphicsApplication::GetInputHandler()
{
    return m_eventlistener;
}

WindowHandler* GraphicsApplication::GetWindowHandler()
{
    return m_windowhandler;
}

OGLContextHandler* GraphicsApplication::GetOGLContextHandler()
{
    return m_OGLContextHandler;
}

SceneHandler* GraphicsApplication::GetSceneHandler()
{
    return m_scenehandler;
}


GraphicsApplication::~GraphicsApplication()
{
    delete m_OGLContextHandler;
    delete m_windowhandler;
    delete m_eventlistener;
    delete m_scenehandler;
}


