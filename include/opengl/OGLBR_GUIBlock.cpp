#include "OGLBR_GUIBlock.h"
#include "../Tools.h"
#include "shaderhandling.h"

#include <bitset>
#include <GL/glew.h>
#include <SDL2/SDL_image.h>


OGLBR_GUIBlock::OGLBR_GUIBlock(GUIBlock* _blockdata)
{
    if(_blockdata)
        blockdata = _blockdata;
    else
        cout << "Error OGLBR_GUIBlock: no valid blockdata" << endl;
    VAO = 0;
    IBO = 0;
}

void OGLBR_GUIBlock::Buffer()
{
    if(VAO != 0)
    {
        glDeleteVertexArrays(1, &VAO);
        VAO = 0;
    }

    unsigned int shaderID;
    shader_bitmask bitmask(A_POS | A_COL, 0, 0, 0);
    if(!(shaderID = GetShaderByBitmask(bitmask)))
    {
        cout << "Error OGLBR_GUIBlock::Render(): "
             << "No shader set for bitmask ";
        PrintBitmask(bitmask);
    }
    else
        shading_state = bitmask;

    // now vertex and color buffer
    vec3f vertices[4]; //0 bottom left corner
    //1 top left corner
    //2 top right corner
    //3 bottom right corner

    vec3f rpos = vec3f(-1.0 + float(blockdata->x)/float(WINDOW_WIDTH)*2.0,
                       1.0 - float(blockdata->y)/float(WINDOW_HEIGHT)*2.0,
                       float(blockdata->z));

    float rwidth = float(blockdata->width)/float(WINDOW_WIDTH)*2;
    float rheight = float(blockdata->height)/float(WINDOW_HEIGHT)*2;
    vertices[0] = vec3f(rpos.x,        rpos.y-rheight, rpos.z);
    vertices[1] = vec3f(rpos.x,        rpos.y,         rpos.z);
    vertices[2] = vec3f(rpos.x+rwidth, rpos.y,         rpos.z);
    vertices[3] = vec3f(rpos.x+rwidth, rpos.y-rheight, rpos.z);

    unsigned int indices[] = {0,1,2, // first triangle (bottom left - top left - top right)
                              0,2,3
                             }; // second triangle (bottom left - top right - bottom right)

    vec4f colors[4];
    colors[0] = blockdata->color;
    colors[1] = blockdata->color;
    colors[2] = blockdata->color;
    colors[3] = blockdata->color;

    if(VAO == 0)
        glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    if(VBOs.size() == 0)
    {
        VBOs.resize(2, 0);
        glGenBuffers(2, &VBOs[0]);
    }

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    if(glGetAttribLocation(shaderID, "vPosition") < 0)
        cout << "Error OGLBR_GUIBlock::Buffer(): vPosition not declared in shader" << endl;
    else
        glVertexAttribPointer(glGetAttribLocation(shaderID, "vPosition"),
                              3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_DYNAMIC_DRAW);
    if(glGetAttribLocation(shaderID, "vColor") < 0)
        cout << "Error OGLBR_GUIBlock::Buffer(): vColor not declared in shader" << endl;
    else
        glVertexAttribPointer(glGetAttribLocation(shaderID, "vColor"),
                              4, GL_FLOAT, GL_FALSE, 0, 0);

    if(IBO == 0)
        glGenBuffers(1, &IBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

    glBindVertexArray(0);
}

void OGLBR_GUIBlock::Render()
{
    unsigned int shaderID = GetShaderByBitmask(shading_state);

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);

    glUseProgram(shaderID);
    glBindVertexArray(VAO);

    glEnableVertexAttribArray(glGetAttribLocation(shaderID, "vPosition"));
    glEnableVertexAttribArray(glGetAttribLocation(shaderID, "vColor"));

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(glGetAttribLocation(shaderID, "vPosition"));
    glDisableVertexAttribArray(glGetAttribLocation(shaderID, "vColor"));

    glBindVertexArray(0);
}
