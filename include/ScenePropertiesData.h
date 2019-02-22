#ifndef SCENEPROPERTIESDATA_H
#define SCENEPROPERTIESDATA_H

#include "opengl/shaderlist.h"

#include <glm/glm.hpp>

#include <string>
#include <vector>

struct ObjectProperties
{
    unsigned int index;
    std::string file_path;
    glm::vec3 translation;
    float rot_angle;
    glm::vec3 rotation;
    glm::vec3 scale;
    std::vector<unsigned int> lightindices;    // indices of lights affecting this object
};

struct SceneLightProperties
{
    unsigned int index;
    unsigned char type;
    glm::vec3 position;
    glm::vec3 intensity;
    glm::vec3 direction;
    float exponent;
    float cutoff;
};

#endif // SCENEPROPERTIESDATA_H
