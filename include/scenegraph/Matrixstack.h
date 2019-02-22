#ifndef MATRIXSTACK_H
#define MATRIXSTACK_H

#include <glm/glm.hpp>

// matrix stack handling
void PushMatrix(glm::mat4& _matrix);
void PopMatrix();
glm::mat4 GetUpmostMatrix();

unsigned int GetMatrixStackSize();

#endif // MATRIXSTACK_H
