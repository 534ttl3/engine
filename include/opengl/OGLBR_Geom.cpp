#include "../datastructures.h"
#include "shaderhandling.h"
#include "../camera/camerahandling.h"
#include "../scenegraph/Matrixstack.h"
#include "OGLBR_Geom.h"

#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



#include <iostream>
#include <stdio.h>
using namespace std;

#include <SDL2/SDL.h>

#include <GL/glew.h>

OGLBR_Geom::OGLBR_Geom(SceneData* _scenedata, unsigned int _object_id, unsigned int _mesh_id)
{
    // force to MINIMAL_SHADER first
    shading_state = shader_bitmask(A_POS, MATRIX_M | MATRIX_V | MATRIX_P, 0, 0);
    scenedata = _scenedata;
    object_id = _object_id;
    mesh_id   = _mesh_id;
}

void OGLBR_Geom::Buffer()
{
    MeshData* cmesh = scenedata->objects[object_id]->meshes[mesh_id];
    cout << "OGLBR_Geom::Buffer()" << endl;
    cout << "checking if shader is available" << endl;
    cout << "bitmask: " << endl;
    shader_bitmask mesh_bitmask = cmesh->bitmask;
    PrintBitmask(mesh_bitmask);
    if(GetShaderByBitmask(mesh_bitmask))
    {
        cout << "shader available" << endl;
        shading_state = mesh_bitmask;
    }
    else
    {
        cout << "shader not available, forcing to MINIMAL_SHADER \a" << endl;
        FORCE_ONE_MESH_TO_MINIMAL_SHADER(shading_state);
    }

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    if(cmesh->textures.size() == 1)
    {
        cout << "buffering one texture" << endl;
        // for now only one texture
        unsigned int textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        if(cmesh->textures[0]->numofcolors == 3)
            glTexImage2D(GL_TEXTURE_2D, 0,  GL_RGB, cmesh->textures[0]->width,
                         cmesh->textures[0]->height, 0, GL_RGB,
                         GL_UNSIGNED_BYTE, cmesh->textures[0]->rawpixels);
        else if(cmesh->textures[0]->numofcolors == 4)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, cmesh->textures[0]->width,
                         cmesh->textures[0]->height, 0, GL_RGBA,
                         GL_UNSIGNED_BYTE, cmesh->textures[0]->rawpixels);
        else
            cout << "Error: could not buffer texture one, is not truecolor"
                 << endl;

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        textureIDs.push_back(textureID);
    }
    else if(cmesh->textures.size() > 1)
    {
        cout << "Error: multiple textures are not yet supported" << endl;
        // this should not go unhandled
    }


    // attribute bitmask
    unsigned int shaderID = 0;
    if(shaderID = GetShaderByBitmask(shading_state))
        cout << "shader available" << endl;
    else
        cout << "shader not available \a" << endl; // todo:

    unsigned int IBO = 0;
    glGenBuffers(1, &IBO); // indices always
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(unsigned int)*cmesh->indices.size(),
                 &cmesh->indices.front(), GL_STATIC_DRAW);

    unsigned int VBO_position = 0;
    if(shading_state.p[0] & A_POS)
    {
        cout << "buffer position" << endl;
        glGenBuffers(1, &VBO_position); // position always
        glBindBuffer(GL_ARRAY_BUFFER, VBO_position);
        glBufferData(GL_ARRAY_BUFFER,
                     sizeof(VertexData)*cmesh->vertices.size(),
                     &cmesh->vertices.front(), GL_STATIC_DRAW);
        glVertexAttribPointer(glGetAttribLocation(shaderID, "vPosition"),
                              3, GL_FLOAT, GL_FALSE, sizeof(VertexData),
                              (void*) get_pos_offset());
    }
    else
        cout << "Error OGLBR_Geom::Buffer(): no position set" << endl;

    unsigned int VBO_normal = 0;
    if(shading_state.p[0] & A_NOR)
    {
        cout << "buffer normal" << endl;
        glGenBuffers(1, &VBO_normal); // normal
        glBindBuffer(GL_ARRAY_BUFFER, VBO_normal);
        glBufferData(GL_ARRAY_BUFFER,
                     sizeof(VertexData)*cmesh->vertices.size(),
                     &cmesh->vertices.front(), GL_STATIC_DRAW);
        glVertexAttribPointer(glGetAttribLocation(shaderID, "vNormal"),
                              3, GL_FLOAT, GL_FALSE, sizeof(VertexData),
                              (void*) get_nor_offset());
    }

    unsigned int VBO_uv = 0;
    if(shading_state.p[0] & A_UV)
    {
        cout << "buffer UVs" << endl;
        glGenBuffers(1, &VBO_uv); // UV
        glBindBuffer(GL_ARRAY_BUFFER, VBO_uv);
        glBufferData(GL_ARRAY_BUFFER,
                     sizeof(VertexData)*cmesh->vertices.size(),
                     &cmesh->vertices.front(), GL_STATIC_DRAW);
        glVertexAttribPointer(glGetAttribLocation(shaderID, "vTexCoord"),
                              2, GL_FLOAT, GL_FALSE, sizeof(VertexData),
                              (void*) get_uv_offset());
    }

    unsigned int VBO_tangent = 0;
    if(shading_state.p[0] & A_TAN)
    {
        cout << "buffer tangents" << endl;
        glGenBuffers(1, &VBO_tangent); // tangent
        glBindBuffer(GL_ARRAY_BUFFER, VBO_tangent);
        glBufferData(GL_ARRAY_BUFFER,
                     sizeof(VertexData)*cmesh->vertices.size(),
                     &cmesh->vertices.front(), GL_STATIC_DRAW);
        glVertexAttribPointer(glGetAttribLocation(shaderID, "vTangent"),
                              3, GL_FLOAT, GL_FALSE, sizeof(VertexData),
                              (void*) get_tan_offset());
    }

    unsigned int VBO_bitangent = 0;
    if(shading_state.p[0] & A_BIT)
    {
        cout << "buffer bitangents" << endl;
        glGenBuffers(1, &VBO_bitangent); // bitangent
        glBindBuffer(GL_ARRAY_BUFFER, VBO_bitangent);
        glBufferData(GL_ARRAY_BUFFER,
                     sizeof(VertexData)*cmesh->vertices.size(),
                     &cmesh->vertices.front(), GL_STATIC_DRAW);
        glVertexAttribPointer(glGetAttribLocation(shaderID, "vBitangent"),
                              3, GL_FLOAT, GL_FALSE, sizeof(VertexData),
                              (void*) get_bit_offset());
    }

    unsigned int VBO_color = 0;
    if(shading_state.p[0] & A_COL)
    {
        cout << "buffer color" << endl;
        glGenBuffers(1, &VBO_color); // color
        glBindBuffer(GL_ARRAY_BUFFER, VBO_color);
        glBufferData(GL_ARRAY_BUFFER,
                     sizeof(VertexData)*cmesh->vertices.size(),
                     &cmesh->vertices.front(), GL_STATIC_DRAW);
        glVertexAttribPointer(glGetAttribLocation(shaderID, "vColor"),
                              4, GL_FLOAT, GL_FALSE, sizeof(VertexData),
                              (void*) get_col_offset());
    }

    glBindVertexArray(0);
}

void OGLBR_Geom::Render()
{
    MeshData* cmesh = scenedata->objects[object_id]->meshes[mesh_id];
    unsigned int shaderID = GetShaderByBitmask(shading_state);
    glUseProgram(shaderID);
    // PrintBitmask(shading_state);

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);

//    // render texture to framebuffer
//    GLuint FramebufferName = 0;
//    glGenFramebuffers(1, &FramebufferName);
//    glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
//    // depth texture
//    GLuint depthTexture;
//    glGenTextures(1, &depthTexture);
//    glBindTexture(GL_TEXTURE_2D. depthTexture);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 1024, 1024, 0,
//                 GL_DEPTH_COMPONENT, GL_FLOAT, 0);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//
//    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);
//
//    glDrawBuffer(GL_NONE); // no color buffer is drawn to
//
//    // always check if the framebuffer is ok
//
//    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
//    {
//        cout << "Error glCheckFramebufferStatus: framebuffer not ok" << endl;
//        exit(1);
//    }
//
//    glm::vec3 lightInvDir = glm::vec3(0.5f, 2, 2);



    glm::mat4 M = GetUpmostMatrix();
    glm::mat4 V = GetViewMatrix();
    glm::mat4 P = GetProjectionMatrix();

    if(shading_state.p[1] & (MATRIX_M | MATRIX_V | MATRIX_P))
    {
        glUniformMatrix4fv(glGetUniformLocation(shaderID, "M"), 1, GL_FALSE, glm::value_ptr(M));
        glUniformMatrix4fv(glGetUniformLocation(shaderID, "V"), 1, GL_FALSE, glm::value_ptr(V));
        glUniformMatrix4fv(glGetUniformLocation(shaderID, "P"), 1, GL_FALSE, glm::value_ptr(P));
    }

    // light sources
    ObjectData* cobject = scenedata->objects[object_id];
    for(unsigned int i=0; i<cobject->lightindices.size(); i++)
    {
        if(shading_state.p[2] & LIGHT_POINT)
        {
            unsigned int curr_index = cobject->lightindices[i];
            glm::vec3 position = glm::vec3( M*V*glm::vec4(scenedata->lights[curr_index]->position, 1.0f));
            glUniform3fv(glGetUniformLocation(shaderID, "LightPosition"), 1, glm::value_ptr(position));
            glUniform3fv(glGetUniformLocation(shaderID, "LightIntensity"), 1, glm::value_ptr(scenedata->lights[curr_index]->intensity));
        }
        if(shading_state.p[2] & LIGHT_DIRECTIONAL)
        {
            cout << "directional light not yet supported" << endl;
            SDL_Quit();
        }
        if(shading_state.p[2] & LIGHT_SPOT)
        {
            cout << "spot light not yet supported" << endl;
            SDL_Quit();
        }
    }

    // material
    if(shading_state.p[3] & MATERIAL_DIFFUSE)
    {
//        vec3f diffuse(0.3, 0.3, 0.3);
        glm::vec3 diffuse = glm::vec3(cmesh->material.diffuse.x,
                                      cmesh->material.diffuse.y,
                                      cmesh->material.diffuse.z);
        glUniform3fv(glGetUniformLocation(shaderID, "Kd"), 1, &diffuse.x);
    }
    if(shading_state.p[3] & MATERIAL_AMBIENT)
    {
//        vec3f ambient(0.6, 0.6, 0.6);
        glm::vec3 ambient = glm::vec3(cmesh->material.ambient.x,
                                      cmesh->material.ambient.y,
                                      cmesh->material.ambient.z);
        glUniform3fv(glGetUniformLocation(shaderID, "Ka"), 1, &ambient.x);
    }
    if(shading_state.p[3] & MATERIAL_SPECULAR)
    {
//        vec3f specular(0.0, 0.0, 0.0);
        glm::vec3 specular = glm::vec3(cmesh->material.specular.x,
                                       cmesh->material.specular.y,
                                       cmesh->material.specular.z);
        glUniform3fv(glGetUniformLocation(shaderID, "Ks"), 1, &specular.x);
    }
    if(shading_state.p[3] & MATERIAL_SHININESS)
    {
//        float shininess = 0.5;
        float shininess = cmesh->material.shininess;
        glUniform1fv(glGetUniformLocation(shaderID, "Shininess"), 1, &shininess);
    }


    // bind the meshe's vao
    glBindVertexArray(VAO);

    // enable attributes
    if(shading_state.p[0] & A_POS)
        glEnableVertexAttribArray(glGetAttribLocation(shaderID, "vPosition"));
    if(shading_state.p[0] & A_NOR)
        glEnableVertexAttribArray(glGetAttribLocation(shaderID, "vNormal"));
    if(shading_state.p[0] & A_UV)
        glEnableVertexAttribArray(glGetAttribLocation(shaderID, "vUV"));
    if(shading_state.p[0] & A_TAN)
        glEnableVertexAttribArray(glGetAttribLocation(shaderID, "vTangent"));
    if(shading_state.p[0] & A_BIT)
        glEnableVertexAttribArray(glGetAttribLocation(shaderID, "vBitangent"));
    if(shading_state.p[0] & A_COL)
        glEnableVertexAttribArray(glGetAttribLocation(shaderID, "vColor"));

    glDrawElements(GL_TRIANGLES, cmesh->indices.size(),
                   GL_UNSIGNED_INT, nullptr);

    // disable attributes
    if(shading_state.p[0] & A_POS)
        glDisableVertexAttribArray(glGetAttribLocation(shaderID, "vPosition"));
    if(shading_state.p[0] & A_NOR)
        glDisableVertexAttribArray(glGetAttribLocation(shaderID, "vNormal"));
    if(shading_state.p[0] & A_UV)
        glDisableVertexAttribArray(glGetAttribLocation(shaderID, "vUV"));
    if(shading_state.p[0] & A_TAN)
        glDisableVertexAttribArray(glGetAttribLocation(shaderID, "vTangent"));
    if(shading_state.p[0] & A_BIT)
        glDisableVertexAttribArray(glGetAttribLocation(shaderID, "vBitangent"));
    if(shading_state.p[0] & A_COL)
        glDisableVertexAttribArray(glGetAttribLocation(shaderID, "vColor"));

    // unbind vao
    glBindVertexArray(0);
}

OGLBR_Geom::~OGLBR_Geom()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(textureIDs.size(), &textureIDs.front());
}
