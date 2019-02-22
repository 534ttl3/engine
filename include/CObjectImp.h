#ifndef COMPLEXOBJECTLOADER_H
#define COMPLEXOBJECTLOADER_H

#include "datastructures.h"

#include <vector>

class CObjectImp
{
    std::vector<MeshData*> meshes;
public:
//    bool WriteMeshesToBinaryFile(const std::string& filename);
    bool ReadObjectFromBinFile(const std::string& filename);

    std::vector<MeshData*>& GetMeshes()
    {
        return meshes;
    }

    ~CObjectImp()
    {
        for(unsigned int i=0; i<meshes.size(); i++)
            delete meshes[i];
    }
};

#endif
