#ifndef TOOLS_H
#define TOOLS_H

#include "datastructures.h"
#include <string>
#include <SDL2/SDL_ttf.h>

bool FileExists(const std::string& filename);

void BufferAxes();
void DrawAxes();

void ToggleWireframe();

bool CompareBitmask(unsigned char a[], unsigned char b[]);

void LoadTTF_Font(unsigned char type, const std::string& filename, unsigned int size);
TTF_Font* GetTTF_Font(unsigned char type);

void FilpTexCoordsHor(vec2f texcoords[], unsigned int numofelem);
void FilpTexCoordsVer(vec2f texcoords[], unsigned int numofelem);

void ClampFloat(float* source, float min_val, float max_val);
void ClampInt(int* source, int min_val, int max_val);

std::string ToString(int val);
std::string ToString(float val);

#endif // TOOLS_H
