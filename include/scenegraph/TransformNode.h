#ifndef TRANSFORMNODE_H
#define TRANSFORMNODE_H

#include "Matrixstack.h"
#include "SceneNode.h"

#include <glm/glm.hpp>

class TransformNode : public SceneNode
{
public:
    TransformNode(glm::mat4* _matrix)
    {
        m_matrix = _matrix;
    }

    void Update()
    {
        glm::mat4 result;
        if(GetMatrixStackSize())
            result = GetUpmostMatrix()*(*m_matrix);
        else
            result = glm::mat4(1.0f);

        PushMatrix(result);
        SceneNode::Update();
        PopMatrix();
    }

private:
    glm::mat4* m_matrix;
};

#endif // TRANSFORMNODE_H
