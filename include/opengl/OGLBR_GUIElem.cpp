#include "OGLBR_GUIElem.h"
#include "../Tools.h"
#include "shaderhandling.h"
#include "../window/WindowHandler.h"

#include <bitset>
#include <GL/glew.h>
#include <SDL2/SDL_image.h>


OGLBR_GUIElem::OGLBR_GUIElem(GUIElement* _elem)
{
    elem = _elem;

    if(TTF_WasInit() == false)
    {
        if(TTF_Init() == -1)
            cout << "TTF_Init() " << TTF_GetError() << endl;
    }
    tid = 0;
    VAO = 0;
    IBO = 0;
}

OGLBR_GUIElem::~OGLBR_GUIElem()
{
    for(unsigned int i=0; i<VBOs.size(); i++)
        glDeleteBuffers(1, &VBOs[i]);
    glDeleteBuffers(1, &IBO);
    glDeleteTextures(1, &tid);
    glDeleteVertexArrays(1, &VAO);
}

void OGLBR_GUIElem::Buffer()
{
    unsigned int shaderID;
    shader_bitmask bitmask(A_POS | A_UV, 0, 0, 0);
    if(!(shaderID = GetShaderByBitmask(bitmask)))
    {
        cout << "Error OGLBR_GUIElem::Render(): "
             << "No shader set for bitmask ";
        PrintBitmask(bitmask);
    }
    else
        shading_state = bitmask;

//    static int i=0;
//    i++;
//    if(i>=10)
//        i=0;
//
//    cout << "buffer " << i << endl;

    bool istext = false;
    bool iswidget = false;
    if(elem->task > GUI_TEXT_FIRST_CONSTANT && elem->task < GUI_TEXT_LAST_CONSTANT)
        istext = true;
    else
        cout << "Error Buffer: invalid GUI_ELEMENT_TASK" << endl;

    SDL_Surface* surface = nullptr;
    if(istext)
    {
        GUIElementText* telem = (GUIElementText*)elem;
        SDL_Color color = {telem->r, telem->g, telem->b};
        if(!(surface = TTF_RenderUTF8_Blended_Wrapped(GetTTF_Font(telem->style), telem->text.c_str(), color, 2000)))
        {
            cout << "Error TTF_RenderUTF8_Blended: " << TTF_GetError() << endl;
            exit(1);
        }
    }
    else if(iswidget) // only load widget texture once
    {
        if(!(surface = IMG_Load(((GUIElementWidget*)elem)->texturename.c_str())))
        {
            cout << "Error IMG_Load: " << IMG_GetError() << endl;
            exit(1);
        }
    }

    // get the number of channels in the SDL surface
    int nOfColors = surface->format->BytesPerPixel;
    unsigned int texture_format;
    if (nOfColors == 4)     // contains an alpha channel
    {
        if (surface->format->Rmask == 0x000000ff)
            texture_format = GL_RGBA;
        else
            texture_format = GL_BGRA;
    }
    else if (nOfColors == 3)       // no alpha channel
    {
        if (surface->format->Rmask == 0x000000ff)
            texture_format = GL_RGB;
        else
            texture_format = GL_BGR;
    }
    else
        cout << "warning: the image is not truecolor..  this will probably break" << endl;

    if(tid == 0)
        glGenTextures(1, &tid);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tid);
    glTexImage2D(GL_TEXTURE_2D, 0, nOfColors, surface->w, surface->h, 0,
                 texture_format, GL_UNSIGNED_BYTE, surface->pixels);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    int samplerloc;
    if((samplerloc = glGetUniformLocation(shaderID, "Tex1")) >= 0)
        glUniform1i(samplerloc, 0);
    else
        cout << "Error glGetUniformLocation(): Tex1 not declared" << endl;

    vec3f rpos = vec3f(-1.0 + float(elem->x)/float(WINDOW_WIDTH)*2.0,
                       1.0 - float(elem->y)/float(WINDOW_HEIGHT)*2.0,
                       float(elem->z));
    elem->width  = surface->w;
    elem->height = surface->h;
    float rwidth = float(surface->w)/float(WINDOW_WIDTH)*2;
    float rheight = float(surface->h)/float(WINDOW_HEIGHT)*2;
    SDL_FreeSurface(surface);


    // now vertex and texcoord buffer
    vec3f vertices[4]; //0 bottom left corner
    //1 top left corner
    //2 top right corner
    //3 bottom right corner

    vertices[0] = vec3f(rpos.x,        rpos.y-rheight, rpos.z);
    vertices[1] = vec3f(rpos.x,        rpos.y,         rpos.z);
    vertices[2] = vec3f(rpos.x+rwidth, rpos.y,         rpos.z);
    vertices[3] = vec3f(rpos.x+rwidth, rpos.y-rheight, rpos.z);

    vec2f texcoords[4];
    texcoords[0] = vec2f(0, 0);
    texcoords[1] = vec2f(0, 1);
    texcoords[2] = vec2f(1, 1);
    texcoords[3] = vec2f(1, 0);

    FilpTexCoordsHor(texcoords, 4);

    unsigned int indices[] = {0,1,2, // first triangle (bottom left - top left - top right)
                              0,2,3
                             }; // second triangle (bottom left - top right - bottom right)

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
        cout << "Error OGLBR_GUIElem::Buffer(): vPosition not declared in shader" << endl;
    else
        glVertexAttribPointer(glGetAttribLocation(shaderID, "vPosition"),
                              3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texcoords), texcoords, GL_DYNAMIC_DRAW);
    if(glGetAttribLocation(shaderID, "vTexCoord") < 0)
        cout << "Error OGLBR_GUIElem::Buffer(): vTexCoord not declared in shader" << endl;
    else
        glVertexAttribPointer(glGetAttribLocation(shaderID, "vTexCoord"),
                              2, GL_FLOAT, GL_FALSE, 0, 0);

    if(IBO == 0)
        glGenBuffers(1, &IBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

    glBindVertexArray(0);
}

void OGLBR_GUIElem::Render()
{
    unsigned int shaderID = GetShaderByBitmask(shading_state);

    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);

    glUseProgram(shaderID);
    glBindVertexArray(VAO);

    glEnableVertexAttribArray(glGetAttribLocation(shaderID, "vPosition"));
    glEnableVertexAttribArray(glGetAttribLocation(shaderID, "vTexCoord"));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tid);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(glGetAttribLocation(shaderID, "vPosition"));
    glDisableVertexAttribArray(glGetAttribLocation(shaderID, "vTexCoord"));

    glBindVertexArray(0);
}
