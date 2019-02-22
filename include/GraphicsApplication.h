// GraphicsApplication.h //
#ifndef GRAPHICSAPPLICATION_H
#define GRAPHICSAPPLICATION_H

class WindowHandler;
class OGLContextHandler;
class InputHandler;
class SceneHandler;

// class for main application
class GraphicsApplication
{
public:
    GraphicsApplication(); // sets running to true
    ~GraphicsApplication();
    // Calls initialization functions for various subclasses
    void InitGraphicsEnvironment();
    // creates Scene, loads models
    void LoadScenes();
    // after glew init
    void BufferScenes();
    // main drawing function
    void UpdateGraphics();
    // sets running to false quitting main loop
    void EndGraphicsApplication();
    // some cleanup function
    void CleanAndCloseGraphicsApplication();
    // checks weather running==true, quits main loop when false
    bool IsRunning();

    void LoadShaders();

    OGLContextHandler* GetOGLContextHandler();
    SceneHandler* GetSceneHandler();
    InputHandler* GetInputHandler();
    WindowHandler* GetWindowHandler();
private:
    OGLContextHandler* m_OGLContextHandler;
    WindowHandler* m_windowhandler;
    InputHandler* m_eventlistener;
    SceneHandler* m_scenehandler;

    // running status
    bool running;
};


#endif // GRAPHICSAPPLICATION_H ///
