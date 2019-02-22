
#include "WindowHandler.h"

#include <SDL2/SDL.h>

void WindowHandler::CreateOpenGLWindow(const char* title, int xpos, int ypos, int width, int height)
{
    current_window = SDL_CreateWindow(title, xpos, ypos, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
}

void WindowHandler::SetFullscreen(bool fullscreen)
{
    if(fullscreen == true)
        SDL_SetWindowFullscreen(current_window, SDL_WINDOW_FULLSCREEN);
    else
        SDL_SetWindowDisplayMode(current_window, 0);
}

void WindowHandler::SwapWindowBuffers()
{
    SDL_GL_SwapWindow(current_window);
}

void WindowHandler::DestroyCurrentWindow()
{
    SDL_DestroyWindow(current_window);
}

SDL_Window* WindowHandler::GetCurrentWindow()
{
    return current_window;
}
