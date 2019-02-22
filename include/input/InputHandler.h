#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include "../datastructures.h"

#include <SDL2/SDL.h>

enum KEYSTATES
{
    KS_0 = 0, KS_1, KS_2, KS_3, KS_4, KS_5, KS_6, KS_7, KS_8, KS_9,
    KS_A, KS_B, KS_C, KS_D, KS_E, KS_F, KS_G, KS_H, KS_I, KS_J,
    KS_K, KS_L, KS_M, KS_N, KS_O, KS_P, KS_Q, KS_R, KS_S, KS_T,
    KS_U, KS_V, KS_W, KS_X, KS_Y, KS_Z, KS_SHIFT, KS_STRG, KS_SPACE, KS_RETURN,
    KS_UP, KS_DOWN, KS_LEFT, KS_RIGHT, KS_KP_PLUS, KS_KP_MINUS, KS_LAST_CONSTANT
};

enum KEYEVENT_PRESSED
{
    KP_0 = 0, KP_1, KP_2, KP_3, KP_4, KP_5, KP_6, KP_7, KP_8, KP_9,
    KP_A, KP_B, KP_C, KP_D, KP_E, KP_F, KP_G, KP_H, KP_I, KP_J,
    KP_K, KP_L, KP_M, KP_N, KP_O, KP_P, KP_Q, KP_R, KP_S, KP_T,
    KP_U, KP_V, KP_W, KP_X, KP_Y, KP_Z, KP_SHIFT, KP_STRG, KP_SPACE, KP_RETURN,
    KP_UP, KP_DOWN, KP_LEFT, KP_RIGHT, KP_KP_PLUS, KP_KP_MINUS, KP_LAST_CONSTANT
};

enum KEYEVENT_RELEASED
{
    KR_0 = 0, KR_1, KR_2, KR_3, KR_4, KR_5, KR_6, KR_7, KR_8, KR_9,
    KR_A, KR_B, KR_C, KR_D, KR_E, KR_F, KR_G, KR_H, KR_I, KR_J,
    KR_K, KR_L, KR_M, KR_N, KR_O, KR_P, KR_Q, KR_R, KR_S, KR_T,
    KR_U, KR_V, KR_W, KR_X, KR_Y, KR_Z, KR_SHIFT, KR_STRG, KR_SPACE, KR_RETURN,
    KR_UP, KR_DOWN, KR_LEFT, KR_RIGHT, KR_KR_PLUS, KR_KR_MINUS, KR_LAST_CONSTANT
};

enum MOUSESTATES
{
    MS_LEFT = 0, MS_WHEEL, MS_RIGHT, MS_LAST_CONSTANT
};

enum MOUSEEVENT_PRESSED
{
    MP_LEFT = 0, MP_WHEEL, MP_RIGHT, MP_LAST_CONSTANT
};

enum MOUSEEVENT_RELEASED
{
    MR_LEFT = 0, MR_WHEEL, MR_RIGHT, MR_LAST_CONSTANT
};

const float INPUT_STEP = 0.01;

class GraphicsApplication;

class InputHandler
{
public:
    InputHandler();
    // dependent on SDL environment
    void ListenForEvents(GraphicsApplication* pApp);
    void ResetTriggeredEvents();

    void AssignKeyboardAndMouseEvents();
    void InitClick(SceneData* sd);
    void ReleaseClick();
    GUIElement* GetClickedElement(GUIData* gui);
    void ActivateElementModifier(GUI_ELEMENT_TASK, GUIData* gui);
    void Pick3dMesh(GraphicsApplication* pApp);
    bool IsGUIPressed();

    bool IsKeyPressed(KEYSTATES kcode);
    bool IsKeyPressTriggered(KEYEVENT_PRESSED ptkey);
    bool IsKeyReleaseTriggered(KEYEVENT_RELEASED rtkey);
    bool IsMousePressed(MOUSESTATES mcode);
    bool IsMousePressTriggered(MOUSEEVENT_PRESSED ptmouse);
    bool IsMouseReleaseTriggered(MOUSEEVENT_RELEASED rtmouse);

    SDL_Event current_event;
    bool keystates[KS_LAST_CONSTANT];
    bool mousestates[MS_LAST_CONSTANT];
    bool keyevent_pressed[KP_LAST_CONSTANT];
    bool keyevent_released[KR_LAST_CONSTANT];
    bool mouseevent_pressed[MP_LAST_CONSTANT];
    bool mouseevent_released[MR_LAST_CONSTANT];

    bool guielementmodifieractive[GUI_TEXT_LAST_CONSTANT];

    bool guipressed;
    GUIElement* activeelement;

    int last_hor;
    int cur_hor;

    GUIData* gui;


    // picked objects/meshes
    int c_obj_id;
    int c_mesh_id;
};

#endif // INPUTHANDLER_H
