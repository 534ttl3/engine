
#include "../Tools.h"
#include "../camera/camerahandling.h"
#include "../opengl/OGLContextHandler.h"
#include "../GraphicsApplication.h"
#include "../window/WindowHandler.h"
#include "../SceneHandler.h"
#include "../datastructures.h"

#include "InputHandler.h"

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <SDL2/SDL.h>

using namespace std;

InputHandler::InputHandler()
{
    for(unsigned int i=0; i<KS_LAST_CONSTANT; i++)
        keystates[i] = false;
    for(unsigned int i=0; i<MS_LAST_CONSTANT; i++)
        mousestates[i] = false;
    for(unsigned int i=0; i<GUI_TEXT_LAST_CONSTANT; i++)
        guielementmodifieractive[i] = false;
    guipressed = false;
    last_hor = 0;
    cur_hor = 0;

    c_obj_id = 0;
    c_mesh_id = 0;
}

void InputHandler::ResetTriggeredEvents()
{
    // keyboard
    for(unsigned int i=0; i<KP_LAST_CONSTANT; i++)
        keyevent_pressed[i] = false;
    for(unsigned int i=0; i<KR_LAST_CONSTANT; i++)
        keyevent_released[i] = false;
    // mouse
    for(unsigned int i=0; i<MP_LAST_CONSTANT; i++)
        mouseevent_pressed[i] = false;
    for(unsigned int i=0; i<MR_LAST_CONSTANT; i++)
        mouseevent_released[i] = false;
}

bool InputHandler::IsKeyPressed(KEYSTATES kcode)
{
    return keystates[kcode];
}

bool InputHandler::IsKeyPressTriggered(KEYEVENT_PRESSED ptkey)
{
    return keyevent_pressed[ptkey];
}

bool InputHandler::IsKeyReleaseTriggered(KEYEVENT_RELEASED rtkey)
{
    return keyevent_released[rtkey];
}

bool InputHandler::IsMousePressed(MOUSESTATES mcode)
{
    return mousestates[mcode];
}

bool InputHandler::IsMousePressTriggered(MOUSEEVENT_PRESSED ptmouse)
{
    return mouseevent_pressed[ptmouse];
}

bool InputHandler::IsMouseReleaseTriggered(MOUSEEVENT_RELEASED rtmouse)
{
    return mouseevent_released[rtmouse];
}


void InputHandler::AssignKeyboardAndMouseEvents()
{
    // only short events like pressed or released
    ResetTriggeredEvents();

    while(SDL_PollEvent(&current_event))
    {
        switch(current_event.type)
        {
        case SDL_KEYDOWN:
            switch(current_event.key.keysym.sym)
            {
            case SDLK_0: keystates[KS_0] = true; keyevent_pressed[KP_0] = true; break;
            case SDLK_1: keystates[KS_1] = true; keyevent_pressed[KP_1] = true; break;
            case SDLK_2: keystates[KS_2] = true; keyevent_pressed[KP_2] = true; break;
            case SDLK_3: keystates[KS_3] = true; keyevent_pressed[KP_3] = true; break;
            case SDLK_4: keystates[KS_4] = true; keyevent_pressed[KP_4] = true; break;
            case SDLK_5: keystates[KS_5] = true; keyevent_pressed[KP_5] = true; break;
            case SDLK_6: keystates[KS_6] = true; keyevent_pressed[KP_6] = true; break;
            case SDLK_7: keystates[KS_7] = true; keyevent_pressed[KP_7] = true; break;
            case SDLK_8: keystates[KS_8] = true; keyevent_pressed[KP_8] = true; break;
            case SDLK_9: keystates[KS_9] = true; keyevent_pressed[KP_9] = true; break;

            case SDLK_a: keystates[KS_A] = true; keyevent_pressed[KP_A] = true; break;
            case SDLK_b: keystates[KS_B] = true; keyevent_pressed[KP_B] = true; break;
            case SDLK_c: keystates[KS_C] = true; keyevent_pressed[KP_C] = true; break;
            case SDLK_d: keystates[KS_D] = true; keyevent_pressed[KP_D] = true; break;
            case SDLK_e: keystates[KS_E] = true; keyevent_pressed[KP_E] = true; break;
            case SDLK_f: keystates[KS_F] = true; keyevent_pressed[KP_F] = true; break;
            case SDLK_g: keystates[KS_G] = true; keyevent_pressed[KP_G] = true; break;
            case SDLK_h: keystates[KS_H] = true; keyevent_pressed[KP_H] = true; break;
            case SDLK_i: keystates[KS_I] = true; keyevent_pressed[KP_I] = true; break;
            case SDLK_j: keystates[KS_J] = true; keyevent_pressed[KP_J] = true; break;
            case SDLK_k: keystates[KS_K] = true; keyevent_pressed[KP_K] = true; break;
            case SDLK_l: keystates[KS_L] = true; keyevent_pressed[KP_L] = true; break;
            case SDLK_m: keystates[KS_M] = true; keyevent_pressed[KP_M] = true; break;
            case SDLK_n: keystates[KS_N] = true; keyevent_pressed[KP_N] = true; break;
            case SDLK_o: keystates[KS_O] = true; keyevent_pressed[KP_O] = true; break;
            case SDLK_p: keystates[KS_P] = true; keyevent_pressed[KP_P] = true; break;
            case SDLK_q: keystates[KS_Q] = true; keyevent_pressed[KP_Q] = true; break;
            case SDLK_r: keystates[KS_R] = true; keyevent_pressed[KP_R] = true; break;
            case SDLK_s: keystates[KS_S] = true; keyevent_pressed[KP_S] = true; break;
            case SDLK_t: keystates[KS_T] = true; keyevent_pressed[KP_T] = true; break;
            case SDLK_u: keystates[KS_U] = true; keyevent_pressed[KP_U] = true; break;
            case SDLK_v: keystates[KS_V] = true; keyevent_pressed[KP_V] = true; break;
            case SDLK_w: keystates[KS_W] = true; keyevent_pressed[KP_W] = true; break;
            case SDLK_x: keystates[KS_X] = true; keyevent_pressed[KP_X] = true; break;
            case SDLK_y: keystates[KS_Y] = true; keyevent_pressed[KP_Y] = true; break;
            case SDLK_z: keystates[KS_Z] = true; keyevent_pressed[KP_Z] = true; break;

            // Funktionstasten
            case SDLK_RETURN: keystates[KS_RETURN] = true; keyevent_pressed[KP_RETURN] = true; break;
            case SDLK_UP: keystates[KS_UP] = true; keyevent_pressed[KP_UP] = true; break;
            case SDLK_DOWN: keystates[KS_DOWN] = true; keyevent_pressed[KP_DOWN] = true; break;
            }
            break;
        case SDL_KEYUP:
            switch(current_event.key.keysym.sym)
            {
            case SDLK_0: keystates[KS_0] = false; keyevent_released[KP_0] = true; break;
            case SDLK_1: keystates[KS_1] = false; keyevent_released[KP_1] = true; break;
            case SDLK_2: keystates[KS_2] = false; keyevent_released[KP_2] = true; break;
            case SDLK_3: keystates[KS_3] = false; keyevent_released[KP_3] = true; break;
            case SDLK_4: keystates[KS_4] = false; keyevent_released[KP_4] = true; break;
            case SDLK_5: keystates[KS_5] = false; keyevent_released[KP_5] = true; break;
            case SDLK_6: keystates[KS_6] = false; keyevent_released[KP_6] = true; break;
            case SDLK_7: keystates[KS_7] = false; keyevent_released[KP_7] = true; break;
            case SDLK_8: keystates[KS_8] = false; keyevent_released[KP_8] = true; break;
            case SDLK_9: keystates[KS_9] = false; keyevent_released[KP_9] = true; break;

            case SDLK_a: keystates[KS_A] = false; keyevent_released[KP_A] = true; break;
            case SDLK_b: keystates[KS_B] = false; keyevent_released[KP_B] = true; break;
            case SDLK_c: keystates[KS_C] = false; keyevent_released[KP_C] = true; break;
            case SDLK_d: keystates[KS_D] = false; keyevent_released[KP_D] = true; break;
            case SDLK_e: keystates[KS_E] = false; keyevent_released[KP_E] = true; break;
            case SDLK_f: keystates[KS_F] = false; keyevent_released[KP_F] = true; break;
            case SDLK_g: keystates[KS_G] = false; keyevent_released[KP_G] = true; break;
            case SDLK_h: keystates[KS_H] = false; keyevent_released[KP_H] = true; break;
            case SDLK_i: keystates[KS_I] = false; keyevent_released[KP_I] = true; break;
            case SDLK_j: keystates[KS_J] = false; keyevent_released[KP_J] = true; break;
            case SDLK_k: keystates[KS_K] = false; keyevent_released[KP_K] = true; break;
            case SDLK_l: keystates[KS_L] = false; keyevent_released[KP_L] = true; break;
            case SDLK_m: keystates[KS_M] = false; keyevent_released[KP_M] = true; break;
            case SDLK_n: keystates[KS_N] = false; keyevent_released[KP_N] = true; break;
            case SDLK_o: keystates[KS_O] = false; keyevent_released[KP_O] = true; break;
            case SDLK_p: keystates[KS_P] = false; keyevent_released[KP_P] = true; break;
            case SDLK_q: keystates[KS_Q] = false; keyevent_released[KP_Q] = true; break;
            case SDLK_r: keystates[KS_R] = false; keyevent_released[KP_R] = true; break;
            case SDLK_s: keystates[KS_S] = false; keyevent_released[KP_S] = true; break;
            case SDLK_t: keystates[KS_T] = false; keyevent_released[KP_T] = true; break;
            case SDLK_u: keystates[KS_U] = false; keyevent_released[KP_U] = true; break;
            case SDLK_v: keystates[KS_V] = false; keyevent_released[KP_V] = true; break;
            case SDLK_w: keystates[KS_W] = false; keyevent_released[KP_W] = true; break;
            case SDLK_x: keystates[KS_X] = false; keyevent_released[KP_X] = true; break;
            case SDLK_y: keystates[KS_Y] = false; keyevent_released[KP_Y] = true; break;
            case SDLK_z: keystates[KS_Z] = false; keyevent_released[KP_Z] = true; break;

            // Funktionstasten
            case SDLK_RETURN: keystates[KS_RETURN] = false; keyevent_released[KP_RETURN] = true; break;
            case SDLK_UP: keystates[KS_UP] = false; keyevent_released[KP_UP] = true; break;
            case SDLK_DOWN: keystates[KS_DOWN] = false; keyevent_released[KP_DOWN] = true; break;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            switch(current_event.button.button)
            {
            case SDL_BUTTON_LEFT:   mousestates[MS_LEFT] = true; mouseevent_pressed[MP_LEFT] = true; break;
            case SDL_BUTTON_MIDDLE: mousestates[MS_WHEEL] = true; mouseevent_pressed[MP_WHEEL] = true; break;
            case SDL_BUTTON_RIGHT:  mousestates[MS_RIGHT] = true; mouseevent_pressed[MP_RIGHT] = true; break;
            }
            break;
        case SDL_MOUSEBUTTONUP:
            switch(current_event.button.button)
            {
            case SDL_BUTTON_LEFT:   mousestates[MS_LEFT] = false; mouseevent_released[MP_LEFT] = true; break;
            case SDL_BUTTON_MIDDLE: mousestates[MS_WHEEL] = false; mouseevent_released[MP_WHEEL] = true; break;
            case SDL_BUTTON_RIGHT:  mousestates[MS_RIGHT] = false; mouseevent_released[MP_RIGHT] = true; break;
            }
            break;
        }
    }
}

void InputHandler::ListenForEvents(GraphicsApplication* pApp)
{
    guipressed = false;
    SceneData* sd = pApp->GetSceneHandler()->GetCurrentScene();

    AssignKeyboardAndMouseEvents();

    // handle events
    bool ObjSelectChanged = false;
    bool MeshSelectChanged = false;
    if(IsKeyPressTriggered(KP_X))
    {
        cout << "x" << endl;
        pApp->EndGraphicsApplication();
    }

    if(IsMousePressTriggered(MP_LEFT))
    {
        InitClick(pApp->GetSceneHandler()->GetCurrentScene());
    }

    if(IsKeyPressTriggered(KP_UP) != IsKeyPressTriggered(KP_DOWN))
    {
        if(guielementmodifieractive[GUI_TEXT_SELECT_OBJECT_COUNT])
        {
            if(IsKeyPressTriggered(KP_UP)) c_obj_id++;
            if(IsKeyPressTriggered(KP_DOWN)) c_obj_id--;

            MeshSelectChanged = true;
            ClampInt(&c_obj_id, 0, sd->objects.size()-1);
            sd->gui->GetBlock(GUI_BLOCK_SELECT_OBJECT)->GetElementText(GUI_TEXT_SELECT_OBJECT_COUNT)->SetText(string(ToString(c_obj_id)));
            // load mesh to gui
            sd->gui->GetBlock(GUI_BLOCK_SELECT_OBJECT)->GetElementText(GUI_TEXT_SELECT_MESH_COUNT)->SetText(string(ToString(c_mesh_id) + "/" + ToString((int)sd->GetObjectById(c_obj_id)->meshes.size())));
        }
        else if(guielementmodifieractive[GUI_TEXT_SELECT_MESH_COUNT])
        {
            if(IsKeyPressTriggered(KP_UP)) c_mesh_id++;
            if(IsKeyPressTriggered(KP_DOWN)) c_mesh_id--;
            MeshSelectChanged = true;

            ClampInt(&c_mesh_id, 0, sd->objects[c_obj_id]->meshes.size()-1);
        }
    }

    ObjectData* activeobj = sd->objects[c_obj_id];
    MeshData* activemesh = sd->objects[c_obj_id]->meshes[c_mesh_id];

    if(MeshSelectChanged)
    {
        // update all gauges
        char tbuf[50];
        MaterialData cmat = activemesh->material;
        sprintf(tbuf, "ambient: %.2f %.2f %.2f", float(cmat.ambient.x), float(cmat.ambient.y), float(cmat.ambient.z));
        sd->gui->GetBlock(GUI_BLOCK_MODIFY_MATERIAL)->GetElementText(GUI_TEXT_MODIFY_MATERIAL_AMBIENT)->SetText(string(tbuf));
        sprintf(tbuf, "diffuse: %.2f %.2f %.2f", float(cmat.diffuse.x), float(cmat.diffuse.y), float(cmat.diffuse.z));
        sd->gui->GetBlock(GUI_BLOCK_MODIFY_MATERIAL)->GetElementText(GUI_TEXT_MODIFY_MATERIAL_DIFFUSE)->SetText(string(tbuf));
        sprintf(tbuf, "specular: %.2f %.2f %.2f", float(cmat.specular.x), float(cmat.specular.y), float(cmat.specular.z));
        sd->gui->GetBlock(GUI_BLOCK_MODIFY_MATERIAL)->GetElementText(GUI_TEXT_MODIFY_MATERIAL_SPECULAR)->SetText(string(tbuf));
        sprintf(tbuf, "shininess: %.2f", float(cmat.shininess));
        sd->gui->GetBlock(GUI_BLOCK_MODIFY_MATERIAL)->GetElementText(GUI_TEXT_MODIFY_MATERIAL_SHININESS)->SetText(string(tbuf));
    }

    if(IsKeyPressed(KS_W)) MoveForward();
    if(IsKeyPressed(KS_S)) MoveBackward();

    if(IsKeyPressed(KS_UP) != IsKeyPressed(KS_DOWN))
    {
        // modify materials
        float input_step;
        if(IsKeyPressed(KS_DOWN))
            input_step = -INPUT_STEP;
        else if(IsKeyPressed(KS_UP))
            input_step = INPUT_STEP;

        bool r = IsKeyPressed(KS_1);
        bool g = IsKeyPressed(KS_2);
        bool b = IsKeyPressed(KS_3);

        guipressed = true;
        char tbuf[30];
        if(guielementmodifieractive[GUI_TEXT_MODIFY_MATERIAL_AMBIENT])
        {
            if(!r && !g && !b)
                activemesh->material.ambient += input_step;
            if(r && !g && !b)
                activemesh->material.ambient.x += input_step;
            if(!r && g && !b)
                activemesh->material.ambient.y += input_step;
            if(!r && !g && b)
                activemesh->material.ambient.z += input_step;

            ClampFloat(&activemesh->material.ambient.x, 0.0, 1.0);
            ClampFloat(&activemesh->material.ambient.y, 0.0, 1.0);
            ClampFloat(&activemesh->material.ambient.z, 0.0, 1.0);

            MaterialData cmat = activemesh->material;
            sprintf(tbuf, "ambient: %.2f %.2f %.2f", float(cmat.ambient.x), float(cmat.ambient.y), float(cmat.ambient.z));
            sd->gui->GetBlock(GUI_BLOCK_MODIFY_MATERIAL)->GetElementText(GUI_TEXT_MODIFY_MATERIAL_AMBIENT)->SetText(string(tbuf));
        }

        if(guielementmodifieractive[GUI_TEXT_MODIFY_MATERIAL_DIFFUSE])
        {
            if(!r && !g && !b)
                activemesh->material.diffuse += input_step;
            if(r && !g && !b)
                activemesh->material.diffuse.x += input_step;
            if(!r && g && !b)
                activemesh->material.diffuse.y += input_step;
            if(!r && !g && b)
                activemesh->material.diffuse.z += input_step;

            ClampFloat(&activemesh->material.diffuse.x, 0.0, 1.0);
            ClampFloat(&activemesh->material.diffuse.y, 0.0, 1.0);
            ClampFloat(&activemesh->material.diffuse.z, 0.0, 1.0);

            MaterialData cmat = activemesh->material;
            sprintf(tbuf, "diffuse: %.2f %.2f %.2f", float(cmat.diffuse.x), float(cmat.diffuse.y), float(cmat.diffuse.z));
            sd->gui->GetBlock(GUI_BLOCK_MODIFY_MATERIAL)->GetElementText(GUI_TEXT_MODIFY_MATERIAL_DIFFUSE)->SetText(string(tbuf));
        }

        if(guielementmodifieractive[GUI_TEXT_MODIFY_MATERIAL_SPECULAR])
        {
            if(!r && !g && !b)
                activemesh->material.specular += input_step;
            if(r && !g && !b)
                activemesh->material.specular.x += input_step;
            if(!r && g && !b)
                activemesh->material.specular.y += input_step;
            if(!r && !g && b)
                activemesh->material.specular.z += input_step;

            ClampFloat(&activemesh->material.specular.x, 0.0, 1.0);
            ClampFloat(&activemesh->material.specular.y, 0.0, 1.0);
            ClampFloat(&activemesh->material.specular.z, 0.0, 1.0);

            MaterialData cmat = activemesh->material;
            sprintf(tbuf, "specular: %.2f %.2f %.2f", float(cmat.specular.x), float(cmat.specular.y), float(cmat.specular.z));
            sd->gui->GetBlock(GUI_BLOCK_MODIFY_MATERIAL)->GetElementText(GUI_TEXT_MODIFY_MATERIAL_SPECULAR)->SetText(string(tbuf));
        }

        if(guielementmodifieractive[GUI_TEXT_MODIFY_MATERIAL_SHININESS])
        {
            activemesh->material.shininess += input_step;

            ClampFloat(&activemesh->material.shininess, 0.0, 30.0);
            MaterialData cmat = activemesh->material;
            sprintf(tbuf, "shininess: %.2f", float(cmat.shininess));
            sd->gui->GetBlock(GUI_BLOCK_MODIFY_MATERIAL)->GetElementText(GUI_TEXT_MODIFY_MATERIAL_SHININESS)->SetText(string(tbuf));
        }
    }
}


GUIElement* InputHandler::GetClickedElement(GUIData* gui)
{
    int cx, cy;
    SDL_GetMouseState(&cx, &cy);
    for(std::vector<GUIBlock*>::iterator it = gui->blocks.begin();
        it != gui->blocks.end(); it++)
    {
        GUIBlock* b = (*it);
        if(cx>b->x && cx<b->x+b->width && cy>b->y && cy<b->y+b->height) // block is clicked
        {
            guipressed = true;
            for(std::vector<GUIElement*>::iterator jt = b->elements.begin();
                jt != b->elements.end(); jt++)
            {
                GUIElement* e = (*jt);
                if(cx>e->x && cx<e->x+e->width && cy>e->y && cy<e->y+e->height) // element clicked
                    return e;
            }
        }
    }
    return nullptr;
}

void InputHandler::InitClick(SceneData* sd)
{
    GUIElement* e = nullptr;
    if((e = GetClickedElement(sd->gui)))
    {
        ActivateElementModifier(e->task, sd->gui);
    }
}

void InputHandler::ReleaseClick()
{
//    for(unsigned int i=0; i<GUI_TEXT_LAST_CONSTANT; i++)
//    {
//        guielementmodifieractive[i] = false;
//    }
//    guipressed = false;
}


void InputHandler::ActivateElementModifier(GUI_ELEMENT_TASK etask, GUIData* gui)
{
    for(unsigned int i=0; i<GUI_TEXT_LAST_CONSTANT; i++)
        guielementmodifieractive[i] = false;

    guielementmodifieractive[etask] = true;

    bool success = false;
    // mark it as active (change its color), unmark all others
    for(std::vector<GUIBlock*>::iterator it = gui->blocks.begin();
        it != gui->blocks.end(); it++)
    {
        for(std::vector<GUIElement*>::iterator jt = (*it)->elements.begin();
            jt != (*it)->elements.end(); jt++)
        {
            if(IsTextModifier((*jt)->task))
            {
                if((*jt)->task == etask)
                {
                    ((GUIElementText*)(*jt))->r = 0;
                    ((GUIElementText*)(*jt))->g = 200;
                    ((GUIElementText*)(*jt))->b = 255;
                    ((GUIElementText*)(*jt))->a = 255;
                    (*jt)->changed = true;
                }
                else
                {
                    ((GUIElementText*)(*jt))->r = 200;
                    ((GUIElementText*)(*jt))->g = 200;
                    ((GUIElementText*)(*jt))->b = 200;
                    ((GUIElementText*)(*jt))->a = 200;
                    (*jt)->changed = true;
                }
            }
        }
    }
}


bool InputHandler::IsGUIPressed()
{
    return guipressed;
}

void InputHandler::Pick3dMesh(GraphicsApplication* pApp)
{

}
