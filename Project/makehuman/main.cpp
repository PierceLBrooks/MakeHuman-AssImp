
// Author: Pierce Brooks

#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>
#include <assimp/Exporter.hpp>
#include <assimp/Importer.hpp>
#include <assimp/SceneCombiner.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

int run(const std::vector<std::string>& args)
{
    Assimp::Exporter exporter;
    Assimp::Importer* importerLeft = new Assimp::Importer();
    Assimp::Importer* importerRight = new Assimp::Importer();
    const aiScene* sceneLeft = nullptr;
    const aiScene* sceneRight = nullptr;
    aiScene* scene = nullptr;
    std::vector<aiScene*> scenes;
    //std::cout << "Hello, world!" << std::endl;
    sceneLeft = importerLeft->ReadFile(args[1], aiProcessPreset_TargetRealtime_MaxQuality|aiProcess_FlipUVs|aiProcess_LimitBoneWeights);
    if (sceneLeft == nullptr)
    {
        std::cout << importerLeft->GetErrorString() << std::endl;
        return -1;
    }
    sceneRight = importerRight->ReadFile(args[2], aiProcessPreset_TargetRealtime_MaxQuality|aiProcess_FlipUVs|aiProcess_LimitBoneWeights);
    if (sceneRight == nullptr)
    {
        std::cout << importerRight->GetErrorString() << std::endl;
        importerLeft->FreeScene();
        return -2;
    }
    scenes.push_back(importerLeft->GetOrphanedScene());
    scenes.push_back(importerRight->GetOrphanedScene());
    Assimp::SceneCombiner::MergeScenes(&scene, scenes, 0);
    //importerLeft.FreeScene();
    //importerRight.FreeScene();
    if (scene == nullptr)
    {
        return -3;
    }
    if (exporter.Export(scene, "gltf2", std::string(args[0])+".gltf") != aiReturn_SUCCESS)
    {
        std::cout << exporter.GetErrorString() << std::endl;
        return -4;
    }
    delete scene;
    //delete importerLeft;
    //delete importerRight;
    return 0;
}

int main(int argc, char** argv)
{
    int result = 0;
    std::vector<std::string> args;
    for (int i = 0; i < argc; ++i)
    {
        args.push_back(std::string(argv[i]));
        std::cout << args.back() << std::endl;
    }
    if (argc >= 3)
    {
        result = run(args);
    }
    std::cout << std::to_string(result) << std::endl;
    return result;
}

