#include "processing.h"

#include <glm/glm.hpp>

void SetFlatNormals(MeshData* mesh)
{
    // get faces
    for(unsigned int i=0; i<mesh->indices.size(); i+=3)
    {
        glm::vec3 v1 = mesh->vertices[mesh->indices[i+1]].position - mesh->vertices[mesh->indices[i]].position;
        glm::vec3 v2 = mesh->vertices[mesh->indices[i+2]].position - mesh->vertices[mesh->indices[i]].position;

        glm::vec3 flatnormalv = glm::normalize(glm::cross(v2, v1));

        mesh->vertices[mesh->indices[i]].normal = flatnormalv;
        mesh->vertices[mesh->indices[i+1]].normal = flatnormalv;
        mesh->vertices[mesh->indices[i+2]].normal = flatnormalv;
    }
}
