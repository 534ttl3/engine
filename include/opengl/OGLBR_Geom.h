#ifndef OGLBR_Geom_H
#define OGLBR_Geom_H

#include <vector>

#include "shaderhandling.h"

struct MeshData;

class OGLBR_Geom
{
public:
    OGLBR_Geom(SceneData* _scenedata, unsigned int _object_id, unsigned int _mesh_id);
    ~OGLBR_Geom();
    void Buffer();
    void Render();
private:
    SceneData* scenedata;
    shader_bitmask shading_state; // either normal or forced to MINIMAL_SHADER
    int object_id;
    int mesh_id;

    unsigned int VAO;
    std::vector<unsigned int> textureIDs;
};

#endif // OPENGLBUFFERRENDER_H
