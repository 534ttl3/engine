
#include "../datastructures.h"
#include "shaderhandling.h"
#include "../ScenePropertiesData.h"
#include "../Tools.h"

#include <iostream>
#include <string>
#include <fstream>
#include <bitset>
#include <vector>
#include <bitset>

#include <GL/glew.h>
using namespace std;

// stores all imported shaders and holds their programIDs and bitmasks
std::vector<shaderstruct> shaderstorer;

unsigned int GetShaderByBitmask(shader_bitmask& si)
{
    unsigned int i;
    for(i=0; i<shaderstorer.size(); i++)
    {
        if(CompareBitmask(si.p, shaderstorer[i].bitmask))
        {
            return shaderstorer[i].programid;
        }
    }
    return 0;
}

void PrintBitmask(shader_bitmask& bitmask)
{
    for(unsigned int i=0; i<4; i++)
    {
        cout << bitset<8>(bitmask.p[i]) << " ";
    }
    cout << endl;
}

void FORCE_ONE_MESH_TO_MINIMAL_SHADER(shader_bitmask& sbm)
{
    sbm = shader_bitmask(A_POS, MATRIX_M | MATRIX_V | MATRIX_P, 0, 0);
}

void FORCE_ALL_MESHES_TO_MINIMAL_SHADER(SceneData& scenedata)
{
    for(unsigned int i=0; i<scenedata.objects.size(); i++)
    {
        for(unsigned int j=0; j<scenedata.objects[i]->meshes.size(); j++)
        {
            scenedata.objects[i]->meshes[j]->bitmask = shader_bitmask(A_POS, MATRIX_M | MATRIX_V | MATRIX_P, 0, 0);
        }
    }
}

bool LoadShaderFromFile(const std::string& vs_path, const std::string& fs_path, shader_bitmask& si)
{
    cout << "loading shader for bitmask " << endl;
    for(unsigned int i=0; i<4; i++)
    {
        cout << bitset<8>(si.p[i]) << " ";
    }
    cout << endl << vs_path << " " << fs_path << " " << endl;
    // Create the shaders
    unsigned int VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    unsigned int FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    // Read the Vertex Shader code from the file
    string VertexShaderCode;
    ifstream VertexShaderStream(vs_path.c_str(), ios::in);
    if(VertexShaderStream.is_open())
    {
        string Line = "";
        while(getline(VertexShaderStream, Line))
            VertexShaderCode += "\n" + Line;
        VertexShaderStream.close();
    }
    else
    {
        cout << "Impossible to open " << vs_path
             << ". Are you in the right directory? " << endl;
        return false;
    }

    // Read the Fragment Shader code from the file
    string FragmentShaderCode;
    ifstream FragmentShaderStream(fs_path.c_str(), ios::in);
    if(FragmentShaderStream.is_open())
    {
        string Line = "";
        while(getline(FragmentShaderStream, Line))
            FragmentShaderCode += "\n" + Line;
        FragmentShaderStream.close();
    }

    GLint Result = GL_FALSE;
    int InfoLogLength;

    // Compile Vertex Shader
    cout << "Compiling shader : " << vs_path << " ";
    char const * VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
    glCompileShader(VertexShaderID);

    // Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 )
    {
        vector<char> VertexShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
        cout << &VertexShaderErrorMessage[0] << endl;
    }

    // Compile Fragment Shader
    cout << "Compiling shader : " << fs_path << " ";
    char const * FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
    glCompileShader(FragmentShaderID);

    // Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 )
    {
        vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
        cout << &FragmentShaderErrorMessage[0] << endl;
    }

    // Link the program
    cout << "Linking program" << endl;
    unsigned int ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);

    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 )
    {
        vector<char> ProgramErrorMessage(InfoLogLength+1);
        glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
        cout << &ProgramErrorMessage[0] << endl;
    }

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    shaderstruct newshader;
    for(unsigned int i=0; i<4; i++)
        newshader.bitmask[i] = si.p[i];
    newshader.programid = ProgramID;
    shaderstorer.push_back(newshader);

    return true;
}

