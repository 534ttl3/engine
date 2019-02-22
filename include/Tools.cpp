
#include "datastructures.h"
#include "camera/camerahandling.h"
#include "opengl/shaderhandling.h"

#include "Tools.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>
#include <fstream>
#include <bitset>
#include <GL/glew.h>
using namespace std;

bool FileExists(const std::string& filename)
{
    std::ifstream in(filename.c_str(), std::ios::in);
    return in.good();
}

bool CompareBitmask(unsigned char a[], unsigned char b[])
{
    bool _equal = false;
    for(unsigned int i=0; i<4; i++)
    {
        if(a[i] == b[i])
            _equal = true;
        else
            return false;
    }
    return true;
}

void FilpTexCoordsHor(vec2f texcoords[], unsigned int numofelem)
{
    for(unsigned int i=0; i<numofelem; i++)
        texcoords[i].y *= -1;
}

void FilpTexCoordsVer(vec2f texcoords[], unsigned int numofelem)
{
    for(unsigned int i=0; i<numofelem; i++)
        texcoords[i].x *= -1;
}

std::map<unsigned char, TTF_Font*> fontstorer;
void LoadTTF_Font(unsigned char type, const std::string& filename, unsigned int size)
{
    if(TTF_WasInit() == false)
    {
        if(TTF_Init() == -1)
            cout << "TTF_Init() " << TTF_GetError() << endl;
    }

    TTF_Font* font = TTF_OpenFont(filename.c_str(), size);
    if(font)
    {
        fontstorer.insert(std::pair<unsigned char, TTF_Font*>(type, font));
        cout << "Font " << bitset<8>(type) << " successfully loaded" << endl;
    }
    else
        cout << "TTF_OpenFont(): " << TTF_GetError() << endl;
}

TTF_Font* GetTTF_Font(unsigned char type)
{
    if(fontstorer[type])
    {
        return fontstorer[type];
    }
    else
        cout << "Error GetTTF_Font: font " << bitset<8>(type) << " not loaded" << endl;
}

void ClampFloat(float* source, float min_val, float max_val)
{
    if(*source < min_val)
        *source = min_val;
    else if(*source > max_val)
        *source = max_val;
}

void ClampInt(int* source, int min_val, int max_val)
{
    if(*source < min_val)
        *source = min_val;
    else if(*source > max_val)
        *source = max_val;
}

std::string ToString(int val)
{
    char buf[30];
    sprintf(buf, "%d", val);
    return string(buf);
}

std::string ToString(float val)
{
    char buf[30];
    sprintf(buf, "%.2f", val);
    return string(buf);
}


float axes_vertices[6*3] =  //X-Axis
{
    10.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    //Y-Axis
    0.0f, 10.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    //X-Axis
    0.0f, 0.0f, 10.0f,
    0.0f, 0.0f, -1.0f
};

float axes_colors[6*4] =  //red
{
    1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f,
    //green
    0.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f,
    //blue
    0.0f, 0.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f
};

unsigned int VAO_Axes;

void BufferAxes()
{
    shader_bitmask bitmask(A_POS | A_COL, MATRIX_M | MATRIX_V | MATRIX_P, 0, 0);
    unsigned int shaderID = GetShaderByBitmask(bitmask);
    if(!shaderID)
        cout << "Error: LoadAxes() invalid shaderID " << endl;
    int vertexloc = glGetAttribLocation(shaderID, "vPosition");
    int colorloc  = glGetAttribLocation(shaderID, "vColor");

    // load axes
    glGenVertexArrays(1, &VAO_Axes);
    glBindVertexArray(VAO_Axes);

    unsigned int VBO_vertices;
    glGenBuffers(1, &VBO_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_vertices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(axes_vertices), axes_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(vertexloc, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

    unsigned int VBO_color;
    glGenBuffers(1, &VBO_color);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_color);
    glBufferData(GL_ARRAY_BUFFER, sizeof(axes_colors), axes_colors, GL_STATIC_DRAW);
    glVertexAttribPointer(colorloc, 4, GL_FLOAT, GL_FALSE, 0, (void*) 0);

    glBindVertexArray(0);
}

void DrawAxes()
{
    shader_bitmask bitmask(A_POS | A_COL, MATRIX_M | MATRIX_V | MATRIX_P, 0, 0);
    unsigned int shaderID = GetShaderByBitmask(bitmask);

    int vertexloc = glGetAttribLocation(shaderID, "vPosition");
    int colorloc  = glGetAttribLocation(shaderID, "vColor");
    int Mloc = glGetUniformLocation(shaderID, "M");
    int Vloc = glGetUniformLocation(shaderID, "V");
    int Ploc = glGetUniformLocation(shaderID, "P");

    glm::mat4 P = GetProjectionMatrix();
    glm::mat4 V = GetViewMatrix();
    glm::mat4 M = glm::mat4(1.0f);
    // Remember, matrix multiplication is the other way around

    // Test for troubleshooting
    // M = glm::mat4(1.0f);
    // V = glm::mat4(1.0f);
    // P = glm::mat4(1.0f);

    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

    glUseProgram(shaderID);

    // pass M, V, P matrix
    glUniformMatrix4fv(Mloc, 1, GL_FALSE, glm::value_ptr(M));
    glUniformMatrix4fv(Vloc, 1, GL_FALSE, glm::value_ptr(V));
    glUniformMatrix4fv(Ploc, 1, GL_FALSE, glm::value_ptr(P));

    // render axes
    glBindVertexArray(VAO_Axes);

    glEnableVertexAttribArray(vertexloc);
    glEnableVertexAttribArray(colorloc);

    glDrawArrays(GL_LINES, 0, 6);

    glDisableVertexAttribArray(vertexloc);
    glDisableVertexAttribArray(colorloc);

    glBindVertexArray(0);
}

bool wireframe = false;
void ToggleWireframe()
{
    if(!wireframe)
    {
        wireframe = true;
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        wireframe = false;
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}
