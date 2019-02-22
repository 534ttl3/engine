
#include "CObjectImp.h"
#include "datastructures.h"

#include <glm/glm.hpp>

#include <iostream>
#include <bitset>
#include <fstream>
using namespace std;


bool CObjectImp::ReadObjectFromBinFile(const std::string& filename)
{
    ifstream bin_in(filename.c_str(), ios::in | ios::binary);
    if(!bin_in.good())
    {
        cout << "bin_in not good" << endl;
        return false;
    }
    // first read the number of meshes (uint32_t)
    uint32_t numofmeshes = 0;
    bin_in.read((char*)&numofmeshes, sizeof(uint32_t));
    if(!numofmeshes)
    {
        cout << "Error ReadFileFromBinFormat: numofmeshes=" << numofmeshes << endl;
        return false;
    }

    // next read all the individual meshes
    for(unsigned int i=0; i<numofmeshes; i++)
    {
        vector<VertexData> vertices;
        vector<unsigned int> indices;
        vector<TextureData*> textures;

        uint32_t id;
        bin_in.read((char*)&id, sizeof(uint32_t));
        uint32_t numofvertices = 0;
        bin_in.read((char*)&numofvertices, sizeof(uint32_t));
        if(!numofvertices)
        {
            cout << "Error ReadFileFromBinFormat: numofvertices=" << numofvertices << endl;
            return false;
        }
        uint32_t numofindices;
        bin_in.read((char*)&numofindices, sizeof(uint32_t));
        if(!numofindices)
        {
            cout << "Error ReadFileFromBinFormat: numofindices=" << numofindices << endl;
            return false;
        }

        uint32_t numoftextures;
        bin_in.read((char*)&numoftextures, sizeof(uint32_t));
        if(numoftextures > 1)
        {
            cout << "Error ReadFileFromBinFormat: numoftextures=" << numoftextures << endl;
            return false;
        }

        // next read material properties
        MaterialData material;
        cout << "reading material" << endl;
        unsigned char materialbitmask = MATERIAL_AMBIENT | MATERIAL_DIFFUSE | MATERIAL_SPECULAR
                                        | MATERIAL_SHININESS;
        bin_in.read((char*)&material.diffuse, sizeof(vec4f));
        bin_in.read((char*)&material.ambient, sizeof(vec4f));
        bin_in.read((char*)&material.specular, sizeof(vec4f));
        bin_in.read((char*)&material.shininess, sizeof(float));
        bin_in.read((char*)&material.emissive, sizeof(vec4f));

        // read vertex attribute attributebitmask
        unsigned char attributebitmask = 0;
        bin_in.read((char*)&attributebitmask, sizeof(unsigned char));
        if(!attributebitmask)
        {
            cout << "Error ReadFileFromBinFormat: reading the attributebitmask failed" << endl;
            return false;
        }

        cout << "number of vertices: " << numofvertices << endl;
        for(unsigned int j=0; j<numofvertices; j++)
        {
            VertexData vertex;

            // fill rest with some data
            vec3f position;
            vec3f normal;
            float U = 0;
            float V = 0;
            vec3f tangent;
            vec3f bitangent;
            vec4f color = vec4f(1.0f, 1.0f, 1.0f, 1.0f);

            if(attributebitmask & A_POS)
            {
                bin_in.read((char*)&position, sizeof(vec3f));
            }
            if(attributebitmask & A_NOR)
            {
                bin_in.read((char*)&normal, sizeof(vec3f));
            }
            if(attributebitmask & A_UV)
            {
                bin_in.read((char*)&U, sizeof(float));
                bin_in.read((char*)&V, sizeof(float));
            }
            if(attributebitmask & A_TAN)
            {
                bin_in.read((char*)&tangent, sizeof(vec3f));
            }
            if(attributebitmask & A_BIT)
            {
                bin_in.read((char*)&bitangent, sizeof(vec3f));
            }
            if(attributebitmask & A_COL)
            {
                bin_in.read((char*)&color, sizeof(vec4f));
            }

            to_glm3f(vertex.position, position);
            to_glm3f(vertex.normal, normal);
            vertex.U = U;
            vertex.V = V;
            to_glm3f(vertex.tangent, tangent);
            to_glm3f(vertex.bitangent, bitangent);
            to_glm4f(vertex.color, color);

            vertices.push_back(vertex);
        }

        cout << "number of indices:" << numofindices << endl;
        for(unsigned int i=0; i<numofindices; i++)
        {
            uint32_t index;
            bin_in.read((char*)&index, sizeof(uint32_t));
            indices.push_back(index);
        }

        cout << "number of textures: " << numoftextures << endl;
        for(unsigned int j=0; j<numoftextures; j++)
        {
            TextureData* ptexdata = new TextureData();

            bin_in.read((char*)&ptexdata->numofcolors, sizeof(uint32_t));
            cout << "numofcolors " << ptexdata->numofcolors << endl;

            bin_in.read((char*)&ptexdata->width,  sizeof(uint32_t));
            cout << "width " << ptexdata->width << endl;

            bin_in.read((char*)&ptexdata->height, sizeof(uint32_t));
            cout << "height " << ptexdata->height << endl;

            unsigned char* tmppixels = new unsigned char[ptexdata->numofcolors*
                    ptexdata->width*
                    ptexdata->height];
            bin_in.read((char*)tmppixels,
                        ptexdata->width*ptexdata->height*ptexdata->numofcolors*sizeof(unsigned char));

            ptexdata->AssignRawPixels(tmppixels);
            delete tmppixels;

            textures.push_back(ptexdata);
        }


        meshes.push_back(new MeshData(id, attributebitmask, materialbitmask, &vertices, &indices, &material, &textures));
    }

    bin_in.close();
    return true;
}


//
//bool CObjectImp::WriteMeshesToBinaryFile(const std::string& filename)
//{
//    // format:
//    // 0: number of meshes (uint32_t)
//    // id (uint_32), number of vertices(uint32_t), number of indices, number of textures(uint32_t) (*number of meshes)
//    // MeshData (*number of meshes)
//    //       => id
//    //       => vertex => y/n: pos, normal, uv, tangent, bitangent, color (*number of vertices) => number of vertices*sizeof(..._VD) as offset to next
//    //       => index: (uint32_t) (*number of indices)
//    //       => texture: => numofcolors (uint32_t) => width (uint32_t), height(uint32_t), rawpixels (bytesize*char) (*number of textures)
//
//    ofstream bin_out(filename.c_str(), ios::out | ios::binary);
//    if(bin_out.good())
//    {
//        cout << "WriteMeshesToBinaryFile " << filename << endl;
//        // seekp to the beginning of the file
//        bin_out.seekp(0);
//        // write number of meshes (uint32_t)
//        uint32_t numofmeshes = meshes.size();
//        bin_out.write((char*)&numofmeshes, sizeof(uint32_t));
//        // now write number of vertices(uint32_t), number of indices (uint32_t), number of textures(uint32_t) (*number of meshes)
//
//        for(unsigned int i=0; i<meshes.size(); i++)
//        {
//            cout << "Id[" << i << "] " << meshes[i]->id << endl;
//            uint32_t writebuf = meshes[i]->id;
//            bin_out.write((char*)&writebuf, sizeof(uint32_t));
//            writebuf = meshes[i]->vertices.size();
//            bin_out.write((char*)&writebuf, sizeof(uint32_t));
//            writebuf = meshes[i]->indices.size();
//            bin_out.write((char*)&writebuf, sizeof(uint32_t));
//            writebuf = meshes[i]->textures.size();
//            bin_out.write((char*)&writebuf, sizeof(uint32_t));
//
//            // next write material properties
//            /*
//                glm::vec4 diffuse;
//                glm::vec4 ambient;
//                glm::vec4 specular;
//                float shininess;
//                glm::vec4 emissive;
//            */
//            cout << "writing material" << endl;
//            bin_out.write((char*)&meshes[i]->material, sizeof(MaterialData));
//
//            // write vertex data of this mesh
//            cout << "writing attributes: " << endl;
//            if(meshes[i]->attributes & A_POS)
//                cout << "A_POS set" << endl;
//            if(meshes[i]->attributes & A_NOR)
//                cout << "A_NOR set" << endl;
//            if(meshes[i]->attributes & A_UV)
//                cout << "A_UV set" << endl;
//            if(meshes[i]->attributes & A_TAN)
//                cout << "A_TAN set" << endl;
//            if(meshes[i]->attributes & A_BIT)
//                cout << "A_BIT set" << endl;
//            if(meshes[i]->attributes & A_COL)
//                cout << "A_COL set" << endl;
//
//            unsigned char attr_bitmask = meshes[i]->attributes;
//            bin_out.write((char*)&attr_bitmask, sizeof(unsigned char));
//
//            cout << "writing vertex data" << endl;
//            cout << "number of vertices: " << meshes[i]->vertices.size() << endl;
//            for(unsigned int j=0; j<meshes[i]->vertices.size(); j++)
//            {
//                if(meshes[i]->attributes & A_POS)
//                    bin_out.write((char*)&meshes[i]->vertices.at(j).position, sizeof(glm::vec3));
//                if(meshes[i]->attributes & A_NOR)
//                    bin_out.write((char*)&meshes[i]->vertices.at(j).normal, sizeof(glm::vec3));
//                if(meshes[i]->attributes & A_UV)
//                {
//                    bin_out.write((char*)&meshes[i]->vertices.at(j).U, sizeof(float));
//                    bin_out.write((char*)&meshes[i]->vertices.at(j).V, sizeof(float));
//                }
//                if(meshes[i]->attributes & A_TAN)
//                    bin_out.write((char*)&meshes[i]->vertices.at(j).tangent, sizeof(glm::vec3));
//                if(meshes[i]->attributes & A_BIT)
//                    bin_out.write((char*)&meshes[i]->vertices.at(j).bitangent, sizeof(glm::vec3));
//                if(meshes[i]->attributes & A_COL)
//                    bin_out.write((char*)&meshes[i]->vertices.at(j).color, sizeof(glm::mat4));
//            }
//
//            cout << "writing index data" << endl;
//            cout << "number of indices: " << meshes[i]->indices.size() << endl;
//
//            for(unsigned int j=0; j<meshes[i]->indices.size(); j++)
//            {
//                bin_out.write((char*)&meshes[i]->indices.at(j), sizeof(uint32_t));
//            }
//
//            cout << "writing texture data" << endl;
//            cout << "number of textures: " << meshes[i]->textures.size() << endl;
//
//            for(unsigned int j=0; j<meshes[i]->textures.size(); j++)
//            {
//                cout << "numofcolors " << meshes[i]->textures.at(j)->numofcolors << endl;
//                bin_out.write((char*)&meshes[i]->textures.at(j)->numofcolors, sizeof(uint32_t));
//
//                cout << "width " << meshes[i]->textures.at(j)->width << endl;
//                bin_out.write((char*)&meshes[i]->textures.at(j)->width,  sizeof(uint32_t));
//
//                cout << "height " << meshes[i]->textures.at(j)->height << endl;
//                bin_out.write((char*)&meshes[i]->textures.at(j)->height, sizeof(uint32_t));
//
//                bin_out.write((char*)meshes[i]->textures.at(j)->rawpixels,
//                              meshes[i]->textures.at(j)->width*meshes[i]->textures.at(j)->height
//                              *meshes[i]->textures.at(j)->numofcolors * sizeof(unsigned char));
//            }
//        }
//        bin_out.close();
//
//        cout << "number of meshes: " << meshes.size() << endl;
//
//        unsigned int numoftex = 0;
//        for(unsigned int i=0; i<meshes.size(); i++)
//        {
//            numoftex += meshes[i]->textures.size();
//        }
//
//        cout << "absolute number of textures: " << numoftex << endl;
//
//        return true;
//    }
//
//    cout << "file not good" << endl;
//    return false;
//}
