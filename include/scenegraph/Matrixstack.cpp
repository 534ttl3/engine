#include "Matrixstack.h"

#include <iostream>
#include <stack>

using namespace std;

std::stack<glm::mat4> matrices;

unsigned int GetMatrixStackSize()
{
    return matrices.size();
}


void PushMatrix(glm::mat4& _matrix)
{
    matrices.push(_matrix);
}

void PopMatrix()
{
    if(matrices.size() == 0)
    {
        cout << "error PopMatrix(): stack already empty" << endl;
    }
    else
    {
        matrices.pop();
    }
}

glm::mat4 GetUpmostMatrix()
{
    if(matrices.size() != 0)
    {
        return matrices.top();
    }

    cout << "error GetUpmostMatrix(): there are 0 matrices" << endl;
    // this will let it crash();
    return matrices.top();
}
