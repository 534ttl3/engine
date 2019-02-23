#ifndef CSceneImp_H
#define CSceneImp_H

#include "ScenePropertiesData.h"
#include "datastructures.h"

#include <string>
#include <vector>


class CSceneImp
{
public:
    std::vector<ObjectProperties> ObjectPropertiesStorer;
    std::vector<SceneLightProperties> SceneLightPropertiesStorer;

    bool LoadComplexSceneFromFile(const std::string& filename, SceneData* scenedata);

    std::vector<ObjectProperties>& GetObjectPropertiesStorer()
    {
        return ObjectPropertiesStorer;
    }
    std::vector<SceneLightProperties>& GetSceneLightPropertiesStorer()
    {
        return SceneLightPropertiesStorer;
    }
private:
    bool ParseComplexSceneFile(const std::string& filepath);
    bool ParseSceneLight(std::string line);
    bool ParseSceneObject(std::string line);
};
#endif // CSceneImp_H
