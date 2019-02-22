#include "include/input/InputHandler.h"
#include "include/window/WindowHandler.h"
#include "include/opengl/OGLContextHandler.h"
#include "include/opengl/shaderhandling.h"
#include "include/SceneHandler.h"
#include "include/GraphicsApplication.h"
#include "include/camera/camerahandling.h"
#include "include/Tools.h"

#include <iostream>
#include <cstdio>
#include <string>
#include <SDL2/SDL.h>
#include <GL/glew.h>

using namespace std;

GraphicsApplication* pApp;

int main(int argc, char* argv[])
{
    pApp = new GraphicsApplication();
    pApp->LoadScenes();
    cout << "logics done" << endl;

    // Creates OpenGL window, Creates OpenGL context, sets OpenGL attribute Doubelbuffering,
    // inits GLEW, sets viewport
    pApp->InitGraphicsEnvironment();
    // set scene as active
    pApp->GetSceneHandler()->ActivateScene(0);

    shader_bitmask bitmask(A_POS | A_NOR, MATRIX_M | MATRIX_V | MATRIX_P, LIGHT_POINT, MATERIAL_AMBIENT | MATERIAL_DIFFUSE | MATERIAL_SPECULAR | MATERIAL_SHININESS);
    LoadShaderFromFile("shaders/pos_nor_phong_point_lighting.vs", "shaders/pos_nor_phong_point_lighting.fs", bitmask);

//    shader_bitmask bitmask(A_POS | A_NOR, MATRIX_M | MATRIX_V | MATRIX_P, LIGHT_POINT, MATERIAL_AMBIENT | MATERIAL_DIFFUSE | MATERIAL_SPECULAR | MATERIAL_SHININESS);
//    LoadShaderFromFile("shaders/per_vertex_directional_light.vs", "shaders/per_vertex_directional_light.fs", bitmask);

    shader_bitmask bitmask1(A_POS, MATRIX_M | MATRIX_V | MATRIX_P, 0, 0);
    LoadShaderFromFile("shaders/pos_shader.vs", "shaders/pos_shader.fs", bitmask1);

    shader_bitmask bitmask2(A_POS | A_COL, MATRIX_M | MATRIX_V | MATRIX_P, 0, 0);
    LoadShaderFromFile("shaders/pos_col_shader.vs", "shaders/pos_col_shader.fs", bitmask2);

    shader_bitmask bitmask3(A_POS | A_UV, 0, 0, 0);
    LoadShaderFromFile("shaders/GUIElementText_shader.vs", "shaders/GUIElementText_shader.fs", bitmask3);

    shader_bitmask bitmask4(A_POS | A_COL, 0, 0, 0);
    LoadShaderFromFile("shaders/GUIBlock_shader.vs", "shaders/GUIBlock_shader.fs", bitmask4);

//     FORCE_ALL_MESHES_TO_MINIMAL_SHADER(*pApp->GetSceneHandler()->SceneDataStorer[0]);

    LoadTTF_Font(FONT_ARIAL_20, "assets/fonts/arial.ttf", 20);
    LoadTTF_Font(FONT_TIMES_NEW_ROMAN_20, "assets/fonts/timesnewroman.ttf", 20);

    GUIBlock* appstatus = new GUIBlock(GUI_BLOCK_APPSTATUS, 10, 10, 0.1, vec4f(1, 1, 1, 0.5));
    GUIElementText* appstatuscap = new GUIElementText(GUI_TEXT_BLOCK_APPSTATUS_CAPTION, FONT_TIMES_NEW_ROMAN_20, "APPSTATUS", 0, 0, 0, 0);
    GUIElementText* fpstext = new GUIElementText(GUI_TEXT_FPS, FONT_ARIAL_20, "FPS: ", 200, 200, 200, 200);
    appstatus->SetElement(appstatuscap);
    appstatus->SetElement(fpstext);
    pApp->GetSceneHandler()->GetCurrentScene()->gui->SetBlock(appstatus);

    GUIBlock* selectthings = new GUIBlock(GUI_BLOCK_SELECT_OBJECT, 10, 100, 0.1, vec4f(1, 1, 1, 0.5));
    GUIElementText* selobjcap = new GUIElementText(GUI_TEXT_SELECT_OBJECT_CAPTION, FONT_TIMES_NEW_ROMAN_20, "Selected Object: ", 0, 0, 0, 0);
    GUIElementText* selobjcount = new GUIElementText(GUI_TEXT_SELECT_OBJECT_COUNT, FONT_ARIAL_20, "-", 200, 200, 200, 200);
    GUIElementText* selmeshcap = new GUIElementText(GUI_TEXT_SELECT_MESH_CAPTION, FONT_TIMES_NEW_ROMAN_20, "Selected Mesh: (ID/last ID)", 0, 0, 0, 0);
    GUIElementText* selmeshcount = new GUIElementText(GUI_TEXT_SELECT_MESH_COUNT, FONT_TIMES_NEW_ROMAN_20, "-", 200, 200, 200, 200);

    selectthings->SetElement(selobjcap);
    selectthings->SetElement(selobjcount);
    selectthings->SetElement(selmeshcap);
    selectthings->SetElement(selmeshcount);

    pApp->GetSceneHandler()->GetCurrentScene()->gui->SetBlock(selectthings);

    GUIBlock* matstatus = new GUIBlock(GUI_BLOCK_MODIFY_MATERIAL, 10, 300, 0.1, vec4f(0.5, 0.8, 1, 0.4));
    GUIElementText* matcap = new GUIElementText(GUI_TEXT_MODIFY_MATERIAL_CAPTION, FONT_TIMES_NEW_ROMAN_20, "MATERIAL", 0, 0, 0, 0);
    GUIElementText* matamb = new GUIElementText(GUI_TEXT_MODIFY_MATERIAL_AMBIENT, FONT_ARIAL_20, "ambient:", 200, 200, 200, 200);
    GUIElementText* matdif = new GUIElementText(GUI_TEXT_MODIFY_MATERIAL_DIFFUSE, FONT_ARIAL_20, "diffuse:", 200, 200, 200, 200);
    GUIElementText* matspe = new GUIElementText(GUI_TEXT_MODIFY_MATERIAL_SPECULAR, FONT_ARIAL_20, "specular:", 200, 200, 200, 200);
    GUIElementText* matshi = new GUIElementText(GUI_TEXT_MODIFY_MATERIAL_SHININESS, FONT_ARIAL_20, "shininess:", 200, 200, 200, 200);

    matstatus->SetElement(matcap);
    matstatus->SetElement(matamb);
    matstatus->SetElement(matdif);
    matstatus->SetElement(matspe);
    matstatus->SetElement(matshi);

    pApp->GetSceneHandler()->GetCurrentScene()->gui->SetBlock(matstatus);


    pApp->GetSceneHandler()->BufferToSceneGraph(0);
    BufferAxes();
    InitCamera();

    glDepthFunc(GL_LEQUAL);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    unsigned int framecount = 0;
    unsigned int startticks = 0;
    while(pApp->IsRunning())
    {
        if(framecount == 0)
            startticks = SDL_GetTicks();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        pApp->GetInputHandler()->ListenForEvents(pApp);

        UpdateCamera(pApp->GetWindowHandler(), pApp->GetInputHandler());
        pApp->UpdateGraphics();
        DrawAxes();
        pApp->GetWindowHandler()->SwapWindowBuffers();

        framecount++;

        if((SDL_GetTicks()-startticks)>=1000)
        {
            char framecountstr[10];
            sprintf(framecountstr, "FPS: %d", framecount);
            pApp->GetSceneHandler()->GetCurrentScene()->gui->GetBlock(GUI_BLOCK_APPSTATUS)->GetElementText(GUI_TEXT_FPS)->SetText(string(framecountstr));
            framecount = 0;
        }
    }

    return 0;
}
