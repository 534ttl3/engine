
#include "../window/WindowHandler.h"
#include "../input/InputHandler.h"

#include "camerahandling.h"

#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>



#include <iostream>
using namespace std;

#include <SDL2/SDL.h>

const float _PI = (float) 3.141592;

unsigned char cameracontrols[3];

float mouseHorDivergence = 0.0f;
float mouseVerDivergence = 0.0f;

glm::mat4 ProjectionMatrix;
glm::mat4 ViewMatrix;
glm::vec3 CameraPosition;
glm::vec3 SphereCenter;
glm::vec3 UpVector;

// some moving parameters
float rho;
float theta; // horizontal spherical coordinate
float phi;   // vertical spherical coordinate
float hor_speed;
float ver_speed;

int prevwheel_x = 0;
int prevwheel_y = 0;

void InitCamera()
{
    rho = 3.0f;
    // initial spherical coordinates
    phi = _PI/2.0;
    theta = 0.0f;

    // Look to the origin
    SphereCenter = glm::vec3(0, 0, 0);
}
//
//void LeftMouseButtonIn(bool _set, WindowHandler* window)
//{
//    if(_set == true)
//    {
////        leftmousebuttonin = true;
//        // Get window size
//        int window_w;
//        int window_h;
//        SDL_GetWindowSize(window->GetCurrentWindow(), &window_w, &window_h);
//
//        // reset mouse to origin
//        SDL_WarpMouseInWindow(window->GetCurrentWindow(), (int)window_w/2, (int)window_h/2);
//        prevwheel_x = (int) window_w/2;
//        prevwheel_y = (int) window_h/2;
//    }
//}

void UpdateCamera(WindowHandler* window, InputHandler* input)
{
    // Get window size
    int window_w;
    int window_h;
    SDL_GetWindowSize(window->GetCurrentWindow(), &window_w, &window_h);
//
//    if(input->mousestates[MS_LEFT_PRESSED])
//    {
//        // Get mouse position
//        int mouse_x, mouse_y;
//        SDL_GetMouseState(&mouse_x, &mouse_y);
//
//        // reset mouse to origin
//        SDL_WarpMouseInWindow(window->GetCurrentWindow(), (int)window_w/2, (int)window_h/2);
//
//        // only mouse divergence
//        mouseHorDivergence = (float)(window_w / 2.0 - mouse_x );
//        mouseVerDivergence = (float)(window_h / 2.0 - mouse_y );
//    }

    if(!input->IsGUIPressed())
    {
        if(input->mousestates[MS_LEFT])
        {
            int wheel_x, wheel_y;
            SDL_GetMouseState(&wheel_x, &wheel_y);
            if(wheel_x != prevwheel_x || wheel_y != prevwheel_y)
            {
                mouseHorDivergence = (float)(prevwheel_x-wheel_x);
                mouseVerDivergence = (float)(prevwheel_y-wheel_y);
            }
            prevwheel_x = wheel_x;
            prevwheel_y = wheel_y;
        }
        else
        {
            SDL_GetMouseState(&prevwheel_x, &prevwheel_y);
        }

        // calc speed depending on different mouse divergences
        float abshor = std::abs(mouseHorDivergence);
        float absver = std::abs(mouseVerDivergence);
        hor_speed = 0.001 + 0.0001f * abshor;
        ver_speed = 0.001f + 0.0001f * absver;

        float horizontal_step = mouseHorDivergence * hor_speed;
        float vertical_step   = mouseVerDivergence * ver_speed;

        theta += horizontal_step;
        phi   += vertical_step;

        // avoid upside-down
        if(phi > _PI ) phi = _PI;
        if(phi < 0.0f) phi = 0.0001f;

        // note: in OpenGL, there is a right handed coordinate system
        CameraPosition.x = SphereCenter.x + rho * sin(phi) * sin(theta);
        CameraPosition.y = SphereCenter.y + rho * cos(phi);
        CameraPosition.z = SphereCenter.z + rho * sin(phi) * cos(theta);

        // update projection matrix
        ProjectionMatrix = glm::perspective(80.0f, float(WINDOW_WIDTH / WINDOW_HEIGHT), 0.1f, 100.0f);

        // Camera matrix
        ViewMatrix = glm::lookAt(
                         CameraPosition,        // Camera is here
                         SphereCenter,          // and looks here : at the same position, plus "direction"
                         glm::vec3(0, 1, 0)     // Head is up (set to 0,-1,0 to look upside-down)
                     );

        mouseHorDivergence = 0.0;
        mouseVerDivergence = 0.0;
    }
}

glm::mat4 GetViewMatrix()
{
    return ViewMatrix;
}

glm::mat4 GetProjectionMatrix()
{
    return ProjectionMatrix;
}

float movespeed = 0.1;

void MoveForward()
{
    // vector CameraPosition->SphereCenter
    SphereCenter = SphereCenter + ( glm::normalize(SphereCenter - CameraPosition) * movespeed );
}

void MoveBackward()
{
    SphereCenter = SphereCenter - ( glm::normalize(SphereCenter - CameraPosition) * movespeed );
}
