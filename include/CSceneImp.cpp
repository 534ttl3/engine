
#include "Tools.h"

#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "opengl/shaderhandling.h"
#include "CObjectImp.h"
#include "CSceneImp.h"

#include <iostream>
#include <vector>
#include <fstream>
#include <boost/regex.hpp>

using namespace std;

bool CSceneImp::LoadComplexSceneFromFile(const string& filename, SceneData* scenedata)
{
    if(ParseComplexSceneFile(filename))
    {
        cout << "parsing " << filename << " finished: "  << endl
             << "Number of Objects: " << ObjectPropertiesStorer.size() << endl;

        std::vector<LightData*> lights;
        std::vector<ObjectData*> objects;

        for(std::vector<SceneLightProperties>::iterator it = SceneLightPropertiesStorer.begin();
                it != SceneLightPropertiesStorer.end(); it++)
        {
            LightData* light = new LightData();
            light->index = it->index;
            light->type = it->type;
            light->position = it->position;
            light->intensity = it->intensity;
            light->direction = it->direction;
            light->exponent = it->exponent;
            light->cutoff = it->cutoff;
            lights.push_back(light);
        }

        // now load objects of a scene and push them into vector
        for(vector<ObjectProperties>::iterator it = ObjectPropertiesStorer.begin();
                it != ObjectPropertiesStorer.end(); it++)
        {
            ObjectData* object = new ObjectData();
            CObjectImp* obj_importer = new CObjectImp();
            if(obj_importer->ReadObjectFromBinFile(it->file_path.c_str()))
            {
                cout << "Object " << it->file_path << " successfully imported" << endl;
            }
            else
            {
                cout << "Object " << it->file_path << " import failed" << endl;
                return false;
            }

            object->meshes = obj_importer->GetMeshes();

            unsigned char lightbitmask = 0;
            for(unsigned int i=0; i<it->lightindices.size(); i++) // go through lightindices array
            {
                // check if that particular light does iven exist
                for(unsigned int j=0; j<lights.size(); j++)
                {
                    if(it->lightindices[i] == lights[j]->index)
                    {
                        lightbitmask |= lights[j]->type;
                        object->lightindices.push_back(lights[j]->index);
                    }
                }
            }

            // assign matrices- and light bitmasks
            for(unsigned int i=0; i<object->meshes.size(); i++)
            {
                // just always pass model view and projection matrix to shader
                object->meshes[i]->bitmask.p[1] = MATRIX_P | MATRIX_V | MATRIX_M;
                object->meshes[i]->bitmask.p[2] = lightbitmask;
            }
            object->id = it->index;
            object->meshes = obj_importer->GetMeshes();
            for(unsigned int i=0; i<object->meshes.size(); i++)
            {
                PrintBitmask(object->meshes[i]->bitmask);
            }

            // calculate transformation matrix
            glm::mat4 tr_matrix = glm::mat4(1.0f);
            tr_matrix = glm::translate(tr_matrix, it->translation);
            if(abs(0.0-it->rotation.x)>0.0000001 || abs(0.0-it->rotation.y)>0.0000001 || abs(0.0-it->rotation.z)>0.0000001)
                tr_matrix = glm::rotate(tr_matrix, it->rot_angle, it->rotation);
            tr_matrix = glm::scale(tr_matrix, it->scale);

            object->tr_matrix = tr_matrix;

            objects.push_back(object);
        }

        scenedata->lights = lights;
        scenedata->objects = objects;
        return true;
    }

    cout << "File " << filename << " not properly parsed" << endl;
    return false;
}

bool CSceneImp::ParseComplexSceneFile(const string& filepath)
{
    cout << "---------------------------" << endl << "parsing file " << filepath << endl;
    if(FileExists(filepath))
        cout  << "File " << filepath << " exists" << endl;
    else
    {
        cout  << "Err: File " << filepath << " does not exist" << endl;
        return false;
    }

    ifstream in(filepath.c_str(), ios::in);
    string line;
    while(!in.eof())
    {
        static int i = 0;
        i++;
        cout << "get line " << i << endl;
        getline(in, line);
        // if its not a comment line
        if(line.find("//") == string::npos)
        {
            // parse for instructions
            if(line.find("#L") != string::npos)
            {
                if(ParseSceneLight(line))
                {
                    cout << "SceneLight parsed: " << SceneLightPropertiesStorer.back().index << endl;
                }
                else
                {
                    cout << "SceneLight not properly parsed" << endl;
                }
            }
            if(line.find("#O") != string::npos)
            {
                if(ParseSceneObject(line))
                {
                    cout  << "Sceneobject parsed: " << ObjectPropertiesStorer.back().index << endl;
                }
                else
                {
                    cout << "Sceneobject " << ObjectPropertiesStorer.size()
                         << " not properly parsed" << endl;
                    cout << "Sceneobject " << ObjectPropertiesStorer.size()
                         << " not properly parsed" << endl;
                    return false;
                }
            }
        }
    }
    return true;
}

bool CSceneImp::ParseSceneObject(string line)
{
    boost::regex e( "(#O)[{]([0-9]{1,5})[;]([A-Za-z_:./ ]*)[;]"
                    "([0-9-]{1,5}[.][0-9-]{1,5})[ ]([0-9-]{1,5}[.][0-9-]{1,5})[ ]([0-9-]{1,5}[.][0-9-]{1,5})[;]"   // translate
                    "([0-9-]{1,5}[.][0-9-]{1,5})[;]([0-9-]{1,5}[.][0-9-]{1,5})[ ]([0-9-]{1,5}[.][0-9-]{1,5})[ ]([0-9-]{1,5}[.][0-9-]{1,5})[;]"   // rotate
                    "([0-9-]{1,5}[.][0-9-]{1,5})[ ]([0-9-]{1,5}[.][0-9-]{1,5})[ ]([0-9-]{1,5}[.][0-9-]{1,5})[;]" // scale
                    "([0-9 ]*)[}]"); // lights affecting object

    string line_fromfile = line;

    boost::match_results<string::const_iterator> results;
    if(boost::regex_match(line_fromfile, results, e))
    {
        ObjectProperties objp;
        // index
        objp.index = (unsigned int) atoi(string(results[2]).c_str());
        cout << "object index: " << objp.index << " ";

        // path to object file
        if(!FileExists(string(results[3])))
        {
            cout << "file does not exist: " << string(results[3]) << endl;
            return false;
        }
        else
            objp.file_path = string(results[3]);
        cout << "file_path: " << objp.file_path << endl;

        float x = atof(string(results[4]).c_str());
        float y = atof(string(results[5]).c_str());
        float z = atof(string(results[6]).c_str());
        objp.translation = glm::vec3(x, y, z);
        cout << " translation: (" << objp.translation.x << "|" << objp.translation.y << "|" << objp.translation.z << ") ";

        objp.rot_angle = atof(string(results[7]).c_str());
        x = atof(string(results[8]).c_str());
        y = atof(string(results[9]).c_str());
        z = atof(string(results[10]).c_str());
        objp.rotation = glm::vec3(x, y, z);
        cout << "rot_angle: " << objp.rot_angle << " rot: (" << objp.rotation.x << "|" << objp.rotation.y << "|" << objp.rotation.z << ") ";

        x = atof(string(results[11]).c_str());
        y = atof(string(results[12]).c_str());
        z = atof(string(results[13]).c_str());
        objp.scale = glm::vec3(x, y, z);
        cout << "scale: (" << objp.scale.x << "|" << objp.scale.y << "|" << objp.scale.z << ") " << endl;

        string s = string(results[14]);
        string delim = " ";
        vector<unsigned int> lindices;
        for(unsigned int i=0; i<s.length(); i++)
        {
            string sub = "";
            while(s[i]>=48 && s[i]<=57)
                sub += s[i++];

            if(sub.length()>0)
                lindices.push_back((unsigned int)atoi(sub.c_str()));
        }

        objp.lightindices.assign(lindices.begin(), lindices.end());
        for(unsigned int i=0; i<objp.lightindices.size(); i++)
            cout << lindices[i] << " ";
        cout << endl;


        ObjectPropertiesStorer.push_back(objp);
        return true;
    }

    return false;
}


bool CSceneImp::ParseSceneLight(string line)
{
    cout << "Parsing SceneLight" << endl;

    // index, type, position, intensity, direction, exponent, cutoff, translation, rotation angle, rotation, scale
    boost::regex e("(#L)[{]([0-9]{1,5})[;]([A-Z]*)[;]"
                   "([0-9]{1,5}[.][0-9]{1,5})[ ]([0-9]{1,5}[.][0-9]{1,5})[ ]([0-9]{1,5}[.][0-9]{1,5})[;]"  // position
                   "([0-9]{1,5}[.][0-9]{1,5})[ ]([0-9]{1,5}[.][0-9]{1,5})[ ]([0-9]{1,5}[.][0-9]{1,5})[;]"  // intensity
                   "([0-9]{1,5}[.][0-9]{1,5})[ ]([0-9]{1,5}[.][0-9]{1,5})[ ]([0-9]{1,5}[.][0-9]{1,5})[;]"  // direction
                   "([0-9]{1,5}[.][0-9]{1,5})[;]"                                                          // exponent
                   "([0-9]{1,5}[.][0-9]{1,5})[}]"                                                          // cutoff
                  );

    string line_fromfile = line;

    cout << line_fromfile << endl;
    cout << line << endl;

    boost::match_results<string::const_iterator> results;
    if(!boost::regex_match(line_fromfile, results, e))
    {
        cout << "regex failed" << endl;
        return false;
    }

    SceneLightProperties lightp;
    lightp.index = (unsigned int) atoi(string(results[2]).c_str());
    cout << "light index: " << lightp.index << " ";

    if(!(lightp.type = GetLightTypeByString(string(results[3]))))
    {
        cout << "Error ParseSceneLight(): lightp.type=" << lightp.type << endl;
        return false;
    }

    lightp.position.x = atof(string(results[4]).c_str());
    lightp.position.y = atof(string(results[5]).c_str());
    lightp.position.z = atof(string(results[6]).c_str());
    cout << lightp.position.x << " " << lightp.position.y << " " << lightp.position.z << endl;

    lightp.intensity.x = atof(string(results[7]).c_str());
    lightp.intensity.y = atof(string(results[8]).c_str());
    lightp.intensity.z = atof(string(results[9]).c_str());
    cout << lightp.intensity.x << " " << lightp.intensity.y << " " << lightp.intensity.z << endl;

    lightp.direction.x = atof(string(results[10]).c_str());
    lightp.direction.y = atof(string(results[11]).c_str());
    lightp.direction.z = atof(string(results[12]).c_str());
    cout << lightp.direction.x << " " << lightp.direction.y << " " << lightp.direction.z << endl;

    lightp.exponent = atof(string(results[13]).c_str());
    cout << lightp.exponent << endl;

    lightp.cutoff = atof(string(results[14]).c_str());
    cout << lightp.cutoff << endl;

    SceneLightPropertiesStorer.push_back(lightp);
    return true;
}

