// WindowHandler.h //
#ifndef WINDOWHANDLER_H
#define WINDOWHANDLER_H

#include <SDL2/SDL.h>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

class WindowHandler
{
public:
    void CreateOpenGLWindow(const char* title, int xpos, int ypos, int width, int height);
    void SwapWindowBuffers();
    void DestroyCurrentWindow();
    void SetFullscreen(bool fullscreen);
    SDL_Window* GetCurrentWindow();
private:
    SDL_Window* current_window;
};

#endif // WINDOWHANDLER_H
