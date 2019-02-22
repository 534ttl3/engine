#ifndef SHADERHANDLING_H
#define SHADERHANDLING_H

#include "../ScenePropertiesData.h"
#include "../datastructures.h"

#include <string>
#include <vector>
#include <map>

unsigned int GetShaderByBitmask(shader_bitmask& si);

bool LoadShaderFromFile(const std::string& vs_path, const std::string& fs_path, shader_bitmask& si);

void PrintBitmask(shader_bitmask& bitmask);

void FORCE_ONE_MESH_TO_MINIMAL_SHADER(shader_bitmask& sbm);
void FORCE_ALL_MESHES_TO_MINIMAL_SHADER(SceneData& scenedata);

#endif // SHADERHANDLING_H
