#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#include "window/WindowHandler.h"

#include <glm/glm.hpp>

#include <iostream>
#include <stdlib.h>
#include <map>
#include <vector>
#include <cstdint>
#include <string>

using namespace std;

struct shader_bitmask
{
    shader_bitmask() { }
    shader_bitmask(unsigned char attributes, unsigned char matrices, unsigned int light, unsigned int material)
    {
        p[0] = attributes;
        p[1] = matrices;
        p[2] = light;
        p[3] = material;
    }
    unsigned char p[4];
};

struct shaderstruct
{
    unsigned int programid;
    unsigned char bitmask[4];
};

// potential attributes
const unsigned char A_POS = 0b00000001;
const unsigned char A_NOR = 0b00000010;
const unsigned char A_UV  = 0b00000100;
const unsigned char A_TAN = 0b00001000;
const unsigned char A_BIT = 0b00010000;
const unsigned char A_COL = 0b00100000;

// potential matrices to be passed to shader
const unsigned char MATRIX_M   = 0b00000001;
const unsigned char MATRIX_V   = 0b00000010;
const unsigned char MATRIX_P   = 0b00000100;
const unsigned char MATRIX_N   = 0b00001000;
const unsigned char MATRIX_MVP = 0b00010000;

// potential lights
const unsigned char LIGHT_POINT       = 0b00000001;
const unsigned char LIGHT_DIRECTIONAL = 0b00000010;
const unsigned char LIGHT_SPOT        = 0b00000100;

// potential materials
const unsigned char MATERIAL_AMBIENT   = 0b00000001;
const unsigned char MATERIAL_DIFFUSE   = 0b00000010;
const unsigned char MATERIAL_SPECULAR  = 0b00000100;
const unsigned char MATERIAL_SHININESS = 0b00001000;


// for shader selection
const unsigned char POS                    = A_POS;
const unsigned char POS_COL                = A_POS |                                A_COL;
const unsigned char POS_NOR                = A_POS | A_NOR;
const unsigned char POS_NOR_COL            = A_POS | A_NOR |                        A_COL;
const unsigned char POS_NOR_UV             = A_POS | A_NOR | A_UV;
const unsigned char POS_NOR_UV_TAN_BIT     = A_POS | A_NOR | A_UV | A_TAN | A_BIT;
const unsigned char POS_NOR_UV_TAN_BIT_COL = A_POS | A_NOR | A_UV | A_TAN | A_BIT | A_COL;


unsigned int get_pos_offset();
unsigned int get_nor_offset();
unsigned int get_uv_offset();
unsigned int get_tan_offset();
unsigned int get_bit_offset();
unsigned int get_col_offset();

unsigned char GetLightTypeByString(const std::string& str);

struct vec3f
{
    vec3f(float _x, float _y, float _z) : x(_x), y(_y), z(_z) { }
    vec3f() { }
    float x, y, z;
};

struct vec4f
{
    vec4f(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) { }
    vec4f() { }
    float x, y, z, w;
};

struct vec2f
{
    vec2f(float _x, float _y) : x(_x), y(_y) { }
    vec2f() { }
    float x, y;
};


void to_glm3f(glm::vec3& _glm, vec3f& vec);
void to_glm4f(glm::vec4& _glm, vec4f& vec);


// light sources
struct LightData
{
    unsigned int index;
    unsigned char type;
    glm::vec3 position;
    glm::vec3 intensity;
    glm::vec3 direction;
    float exponent;
    float cutoff;
};

struct MaterialData
{
    glm::vec4 diffuse;
    glm::vec4 ambient;
    glm::vec4 specular;
    float shininess;
    glm::vec4 emissive;
};

struct VertexData
{
    glm::vec3 position;
    glm::vec3 normal;
    float U, V;
    glm::vec3 tangent;
    glm::vec3 bitangent;
    glm::vec4 color;
};

struct TextureData
{
    TextureData()
    {
        numofcolors = 0;
        width = 0;
        height = 0;
        rawpixels = nullptr;
    }

    // call this after numofcolors, width and height are set
    void AssignRawPixels(unsigned char* _pixels)
    {
        if(numofcolors==0 || height==0 || width==0)
        {
            cout << "Error AllocatePixelMemory(): "
                 << " numofcolors=" << numofcolors
                 << " height=" << height
                 << " width=" << width << endl;
        }

        rawpixels = new unsigned char[width*height*numofcolors];

        for(unsigned int i=0; i<width*height*numofcolors; i++)
            rawpixels[i] = _pixels[i];
    }

    ~TextureData()
    {
        delete rawpixels;
    }

    uint32_t numofcolors;
    uint32_t width;
    uint32_t height;
    unsigned char* rawpixels;
};

struct BoundingSphere
{
    glm::vec3 center;
    float radius;
};


void CalcBoundingSphere(BoundingSphere& sphere, std::vector<VertexData>& vertices);

struct MeshData
{
    MeshData(unsigned int _id, unsigned char _attributebitmask, unsigned char _materialbitmask,
             std::vector<VertexData>* vertexdata,
             std::vector<unsigned int>* indexdata,
             MaterialData* _materialdata=nullptr, std::vector<TextureData*>* texturedata=nullptr)
    {
        id = _id;
        bitmask.p[0] = _attributebitmask;
        bitmask.p[3] = _materialbitmask; // the other two are assigned later, in the in the CSceneImp
        vertices=*vertexdata;
        indices=*indexdata;
        if(texturedata)
            textures=*texturedata;
        material = *_materialdata;
        tr_matrix = glm::mat4(1.0f);

        cout << "number of vertices in MeshData: " << vertices.size() << endl;
        cout << "number of indices  in MeshData: " << indices.size() << endl;
        cout << "number of textures in MeshData: " << textures.size() << endl;

        CalcBoundingSphere(boundingsphere, vertices);

        cout.precision(1);
        cout << fixed;
        cout << "Bounding Sphere: Center: " << boundingsphere.center.x << " | "
             << boundingsphere.center.y << " | "
             << boundingsphere.center.z << " | "
             << ", Radius: " << boundingsphere.radius << endl;
    }

    ~MeshData()
    {
        for(unsigned int i=0; i<textures.size(); i++)
            delete textures[i];
    }

    glm::mat4 tr_matrix;
    MaterialData material;
    shader_bitmask bitmask;

    BoundingSphere boundingsphere;
    unsigned int id;
    std::vector<VertexData> vertices;
    std::vector<TextureData*> textures;
    std::vector<unsigned int> indices;
};

struct ObjectData
{
    // copy all meshes CObjectImp has imported
    ObjectData()
    {
    }

    ~ObjectData()
    {
        for(unsigned int i=0; i<meshes.size(); i++)
            delete meshes[i];
    }

    MeshData* GetMeshById(unsigned int _id)
    {
        for(std::vector<MeshData*>::iterator it=meshes.begin();
            it!=meshes.end(); it++)
        {
            if((*it)->id==_id)
                return (*it);
        }

        cout << "Error GetMeshById: no mesh found with id " << _id << endl;
        exit(1);
    }

    unsigned int id;
    glm::mat4 tr_matrix;
    std::vector<MeshData*> meshes;
    std::vector<unsigned int> lightindices;
};

// fonts
enum FONTS
{
    FONT_ARIAL_12 = 0,
    FONT_ARIAL_20,
    FONT_TIMES_NEW_ROMAN_12,
    FONT_TIMES_NEW_ROMAN_20,
};

enum GUI_ELEMENT
{
    GUI_ELEMENT_TASK_TEXT = 0,
    GUI_ELEMENT_TASK_WIDGET = 1,
};


enum GUI_ELEMENT_TASK
{
    GUI_TEXT_FIRST_CONSTANT = 0,

    GUI_TEXT_CAPTION_FIRST_CONSTANT,
    GUI_TEXT_MODIFY_MATERIAL_CAPTION,
    GUI_TEXT_BLOCK_APPSTATUS_CAPTION,
    GUI_TEXT_SELECT_OBJECT_CAPTION,
    GUI_TEXT_SELECT_MESH_CAPTION,
    GUI_TEXT_MODIFY_LIGHT_CAPTION,
    GUI_TEXT_CAPTION_LAST_CONSTANT,

    GUI_TEXT_FPS,

    GUI_TEXT_MODIFIER_FIRST_CONSTANT,

    GUI_TEXT_SELECT_OBJECT_COUNT,
    GUI_TEXT_SELECT_MESH_COUNT,

    GUI_TEXT_MODIFY_MATERIAL_AMBIENT,
    GUI_TEXT_MODIFY_MATERIAL_DIFFUSE,
    GUI_TEXT_MODIFY_MATERIAL_SPECULAR,
    GUI_TEXT_MODIFY_MATERIAL_SHININESS,
    GUI_TEXT_MODIFY_LIGHT_AMBIENT,
    GUI_TEXT_MODIFY_LIGHT_DIFFUSE,
    GUI_TEXT_MODIFY_LIGHT_SPECULAR,
    GUI_TEXT_MODIFIER_LAST_CONSTANT,
    GUI_TEXT_LAST_CONSTANT,
};

bool IsTextModifier(GUI_ELEMENT_TASK task);

enum GUI_BLOCK_TASK
{
    GUI_BLOCK_APPSTATUS = 0,
    GUI_BLOCK_SELECT_OBJECT,
    GUI_BLOCK_MODIFY_FIRST_CONSTANT,
    GUI_BLOCK_MODIFY_MATERIAL,
    GUI_BLOCK_MODIFY_LIGHT,
    GUI_BLOCK_MODIFY_LAST_CONSTANT,
};

struct GUIElement
{
    GUIElement()
    {
        changed = true;    // it is in front of the Block, so z is less than block's z
        x=0;
        y=0;
        z=0.0;
    }

    bool IsGUIElementText(GUIElement* elem)
    {
        return (elem->task>GUI_TEXT_FIRST_CONSTANT && elem->task<GUI_TEXT_LAST_CONSTANT);
    }

    unsigned int x, y;
    float z;
    unsigned int width;     // height and width determined by image's height and width
    unsigned int height;
    bool changed;
    GUI_ELEMENT_TASK task;
};

struct GUIElementWidget : GUIElement
{
    GUIElementWidget(GUI_ELEMENT_TASK _task, const std::string& _texturename)
    {
        task = _task;
        texturename=_texturename;
    }
    std::string texturename;
};

struct GUIElementText : GUIElement
{
    GUIElementText(GUI_ELEMENT_TASK _task, char _style, const std::string& _text, unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a)
    {
        text = _text;
        task = _task;
        style = _style;
        r = _r;
        g = _g;
        b = _b;
        a = _a;
    }

    void SetText(const std::string& _text)
    {
        text = _text;
        changed = true;
    }

    std::string& GetText(GUIElementText* elemtext)
    {
        if(IsGUIElementText(((GUIElement*)elemtext)))
            return elemtext->text;
        else
        {
            cout << "Error GetText: invalid GUIElementText task: " << elemtext->task << endl;
            exit(1);
        }
    }

    unsigned char style;
    unsigned char r, g, b, a;
    std::string text;
};

const unsigned int GUI_STYLE_ELEMENT_SPACE = 50;
const unsigned int GUI_STYLE_BLOCK_INNER_PADDING= 5;

struct GUIBlock
{
    // z indicates the z index, usually z values should be in the 0.00001 - 0.99999 range
    GUIBlock(GUI_BLOCK_TASK btask, unsigned int _x, unsigned int _y, float _z, vec4f _color)
    {
        if(z<0.0 || z>1.0)
        {
            cout << "Error GUIBlock: z(should be in the 0.01-0.99 range)=" << z << endl;
            exit(1);
        }
        task = btask; x=_x; y=_y; z=_z; color=_color; width=0; height=0;
    }

    ~GUIBlock()
    {
        elements.clear();
    }


    void SetElement(GUIElement* elem)
    {
        elements.push_back(elem);
    }

    GUIElementText* GetElementText(GUI_ELEMENT_TASK etask)
    {
        for(std::vector<GUIElement*>::iterator it=elements.begin();
            it!=elements.end(); it++)
        {
            if((*it)->task==etask)
                return ((GUIElementText*)(*it));
        }

        cout << "Error GetElementText: invalid task: " << etask << endl;
        return 0;
    }

    bool HasSomeElementChanged()
    {
        for(std::vector<GUIElement*>::iterator it = elements.begin();
                it != elements.end(); it++)
        {
            if((*it)->changed)
                return true;
        }
    }

    void RearrangeElements()
    {
        unsigned int i = 0;
        unsigned int currentheight = 0;
        unsigned int blockwidth = 0;
        // rearrange all childs
        for(std::vector<GUIElement*>::iterator it = elements.begin();
            it != elements.end(); it++)
        {
            (*it)->x = x;
            if((*it)->width > blockwidth)
                blockwidth = (*it)->width;
            (*it)->y = y + currentheight;
            i++;
            (*it)->z = z - 0.0001;
            currentheight += (*it)->height;
        }

        height = currentheight;
        width = blockwidth;
    }

    vec4f color;
    unsigned int x, y;
    float z;
    unsigned int width;
    unsigned int height;
    // tasks (constant), text
    GUI_BLOCK_TASK task;
    std::vector<GUIElement*> elements;
};


struct GUIData
{
    GUIData() {}
    ~GUIData() {}
    void SetBlock(GUIBlock* _block)
    {
        if(_block)
            blocks.push_back(_block);
        else
            cout << "Error SetBlock: invalid GUIBlock" << endl;
    }

    GUIBlock* GetBlock(GUI_BLOCK_TASK btask)
    {
        for(std::vector<GUIBlock*>::iterator it=blocks.begin();
            it!=blocks.end(); it++)
        {
            if((*it)->task==btask)
                return (*it);
        }

        cout << "Error GetBlock: invalid GUI_BLOCK_TASK:" << btask << endl;
        exit(1);
    }

    // tasks (constant), block
    std::vector<GUIBlock*> blocks;
};

struct SceneData
{
    SceneData()
    {
        // identity matrix
        gui = new GUIData();
        tr_matrix = glm::mat4(1.0f);
    }

    ~SceneData()
    {
        for(unsigned int i=0; i<objects.size(); i++)
            delete objects[i];
        for(unsigned int i=0; i<lights.size(); i++)
            delete lights[i];
    }

    ObjectData* GetObjectById(unsigned int _id)
    {
        for(std::vector<ObjectData*>::iterator it=objects.begin();
            it!=objects.end(); it++)
        {
            if((*it)->id==_id)
                return (*it);
        }

        cout << "Error GetObjectById: no object found to id " << _id << endl;
        exit(1);
    }

    GUIData* gui;
    glm::mat4 tr_matrix;
    std::vector<ObjectData*> objects;
    std::vector<LightData*> lights;
};

#endif // DATASTRUCTURES_H
