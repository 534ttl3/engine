#ifndef CAMERAHANDLING_H
#define CAMERAHANDLING_H

#include <glm/glm.hpp>


enum CAMERA_CONTROLS
{
    // moving the mouse without pressing a button moves the camera
    CONTROL_GAME_THIRD_PERSON = 0,
    // moving the mouse holding the mousewheel moves the camera
    CONTROL_INSPECT_MODEL
};

class WindowHandler;
class InputHandler;


void InitCamera();

void SetDistance(float distance);

void UpdateCamera(WindowHandler* window, InputHandler* input);

glm::mat4 GetProjectionMatrix();
glm::mat4 GetViewMatrix();

//void MouseIn(bool _set, WindowHandler* window);
//bool IsMouseIn();



void MoveForward();

void MoveBackward();

#endif // CAMERAHANDLING_H
