
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
#include "BvhLoader.hpp"
#include "VoxelGrid.hpp"

int run(const std::vector<std::string>& args)
{
    Assimp::Exporter* exporter = new Assimp::Exporter();
    Assimp::Importer* importerLeft = new Assimp::Importer();
    Assimp::Importer* importerRight = new Assimp::Importer();
    const aiScene* sceneLeft = nullptr;
    const aiScene* sceneRight = nullptr;
    aiScene* scene = nullptr;
    std::vector<aiScene*> scenes;
    sceneLeft = importerLeft->ReadFile(args[1], aiProcessPreset_TargetRealtime_MaxQuality|aiProcess_FlipUVs|aiProcess_LimitBoneWeights);
    if (sceneLeft == nullptr)
    {
        std::cout << importerLeft->GetErrorString() << std::endl;
        delete importerLeft;
        delete importerRight;
        delete exporter;
        return -1;
    }
    sceneRight = importerRight->ReadFile(args[2], aiProcessPreset_TargetRealtime_MaxQuality|aiProcess_FlipUVs|aiProcess_LimitBoneWeights);
    if (sceneRight == nullptr)
    {
        std::cout << importerRight->GetErrorString() << std::endl;
        importerLeft->FreeScene();
        delete importerLeft;
        delete importerRight;
        delete exporter;
        return -2;
    }
    scenes.push_back(importerLeft->GetOrphanedScene());
    scenes.push_back(importerRight->GetOrphanedScene());
    for (int i = 0; i < scenes.size(); ++i)
    {
        std::string path = args[i+1];
        if (path.size() > 4)
        {
            std::string extension = path.substr(path.size()-4);
            if (extension == ".bvh")
            {
                mh::BvhLoader* loader = new mh::BvhLoader();
                if (!loader->load(path))
                {
                    //std::cout << path << std::endl;
                    delete loader;
                    continue;
                }
                loader->visit(nullptr, [&](mh::BvhBone* bone){std::cout << "b," << bone->getName() << "," << bone->getTail().x << "," << bone->getTail().y << "," << bone->getTail().z << "," << bone->getHead().x << "," << bone->getHead().y << "," << bone->getHead().z << std::endl; return true;});
                delete loader;
            }
            else
            {
                //std::cout << extension << std::endl;
            }
        }
        /*for (int j = 0; j < scenes[i]->mNumAnimations; ++j)
        {
            const aiAnimation* animation = scenes[i]->mAnimations[j];
            std::cout << animation->mTicksPerSecond << " @ " << j << std::endl;
        }*/
        while (scenes[i]->mNumMeshes > 1)
        {
            scenes[i]->mNumMeshes -= 1;
        }
    }
    Assimp::SceneCombiner::MergeScenes(&scene, scenes, 0);
    importerLeft->FreeScene();
    importerRight->FreeScene();
    if (scene == nullptr)
    {
        delete importerLeft;
        delete importerRight;
        delete exporter;
        return -3;
    }
    while (scene->mNumMeshes > 2)
    {
        scene->mNumMeshes -= 1;
    }
    /*for (int i = 0; i < scene->mNumMeshes; ++i)
    {
        aiMesh* mesh = scene->mMeshes[i];
        std::cout << "Mesh $ \"" << args[i+1] << "\" # " << i << " & " << mesh->mNumVertices << " ^ " << mesh->mNumFaces << std::endl;
        if (args[i+1].find(".bvh") == std::string::npos)
        {
            continue;
        }
        for (int j = 0; j < mesh->mNumBones; ++j)
        {
            aiBone* bone = mesh->mBones[j];
            std::cout << "Bone $ \"" << bone->mName.C_Str() << "\" | " << scene->mRootNode->FindNode(bone->mName)->mNumMeshes << std::endl;
            for (int k = 0; k < bone->mNumWeights; ++k)
            {
                aiVertexWeight* weight = &bone->mWeights[k];
                aiVector3D* vertex = &mesh->mVertices[weight->mVertexId];
                std::cout << "Weight @ " << weight->mVertexId << " = " << vertex->x << "," << vertex->y << "," << vertex->z << " | " << weight->mWeight << std::endl;
            }
        }
    }*/
    if (exporter->Export(scene, "gltf2", std::string(args[0])+".gltf") != aiReturn_SUCCESS)
    {
        std::cout << exporter->GetErrorString() << std::endl;
        delete scene;
        delete importerLeft;
        delete importerRight;
        delete exporter;
        return -4;
    }
    delete scene;
    delete importerLeft;
    delete importerRight;
    delete exporter;
    return 0;
}

int main(int argc, char** argv)
{
    int result = 0;
    std::vector<std::string> args;
    if (argc == 11) {
	    clock_t start, ends;
	    start = time(NULL);

	    std::string mesh_file = argv[1];
	    std::string bone_file = argv[2];
	    std::string weight_file = argv[3];
	    int max_grid_num = atoi(argv[4]);
	    int max_diffuse_loop = atoi(argv[5]);
	    int max_sample_num = atoi(argv[6]);
	    int max_influence = atoi(argv[7]);
	    float max_fall_off = atof(argv[8]);
	    int sharpness = atoi(argv[9]);
	    std::string detect_solidify = argv[10];

	    mh::VoxelGrid grid(mesh_file, bone_file, weight_file, max_grid_num, max_diffuse_loop, max_sample_num, max_influence, max_fall_off, sharpness, detect_solidify == "y");
	    grid.calculate_all_voxel_darkness();
	    grid.diffuse_all_heats();
	    grid.generate_weight_for_vertices();
	    grid.export_bone_weights();

	    ends = time(NULL);
	    int seconds_elapsed = (int)(ends - start);
	    std::cout << "Running Time : " << seconds_elapsed / 60 << " minutes and "
	              << seconds_elapsed % 60 << " seconds" << std::endl;

	    return 0;
    }
    for (int i = 0; i < argc; ++i)
    {
        if ((i != 0) && (strstr(argv[i], ".bvh") != NULL) && (strstr(argv[i], ".obj") != NULL))
        {
            continue;
        }
        args.push_back(std::string(argv[i]));
        //std::cout << args.back() << std::endl;
    }
    if (argc >= 3)
    {
        result = run(args);
    }
    //std::cout << std::to_string(result) << std::endl;
    return result;
}

