
// Author: Pierce Brooks

#include <cstdlib>
#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <assimp/Exporter.hpp>
#include <assimp/Importer.hpp>
#include <assimp/SceneCombiner.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include "BvhLoader.hpp"
#include "VoxelGrid.hpp"
#include "ObjModel.hpp"

glm::mat4 convertAiMatrix4x4ToGlmMat4(const aiMatrix4x4& from)
{
    glm::mat4 to;

    to[0][0] = from.a1; to[0][1] = from.b1; to[0][2] = from.c1; to[0][3] = from.d1;
    to[1][0] = from.a2; to[1][1] = from.b2; to[1][2] = from.c2; to[1][3] = from.d2;
    to[2][0] = from.a3; to[2][1] = from.b3; to[2][2] = from.c3; to[2][3] = from.d3;
    to[3][0] = from.a4; to[3][1] = from.b4; to[3][2] = from.c4; to[3][3] = from.d4;

    return to;
}

aiMatrix4x4 convertGlmMat4ToAiMatrix4x4(const glm::mat4& from)
{
    aiMatrix4x4 to;

    to.a1 = from[0][0]; to.b1 = from[0][1]; to.c1 = from[0][2]; to.d1 = from[0][3];
    to.a2 = from[1][0]; to.b2 = from[1][1]; to.c2 = from[1][2]; to.d2 = from[1][3];
    to.a3 = from[2][0]; to.b3 = from[2][1]; to.c3 = from[2][2]; to.d3 = from[2][3];
    to.a4 = from[3][0]; to.b4 = from[3][1]; to.c4 = from[3][2]; to.d4 = from[3][3];

    return to;
}

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
    mh::ObjModel* model = nullptr;
    mh::BvhLoader* loader = nullptr;
    std::vector<std::string> args;

    std::cout << argc << std::endl;

    if (argc == 11)
    {
        clock_t start, ends;
        start = time(NULL);

        bool mesh_file_fix = false;
        bool bone_file_fix = false;
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

        if (bone_file.size() > 4)
        {
            std::string extension = bone_file.substr(bone_file.size()-4);
            if (extension == ".bvh")
            {
                std::ofstream output;
                loader = new mh::BvhLoader();
                if (!loader->load(bone_file))
                {
                    //std::cout << path << std::endl;
                    delete loader;
                    loader = nullptr;
                }
                else
                {
                    extension = ".txt";
                    output.open(bone_file+extension);
                    if (!output.is_open())
                    {
                        delete loader;
                        loader = nullptr;
                    }
                    else
                    {
                        //loader->getRoot()->setTail(loader->getRoot()->getChild(0)->getHead());
                        loader->visit(nullptr, [&](mh::BvhBone* bone)
                        {
                            std::string boneName = bone->getName();
                            if ((boneName.size() > 3) && (boneName.substr(0, 3) == "End"))
                            {
                                return true;
                            }
                            output << "b," << boneName << "," << bone->getHead().x << "," << -bone->getHead().z << "," << bone->getHead().y << "," << bone->getTail().x << "," << -bone->getTail().z << "," << bone->getTail().y << std::endl;
                            return true;
                        });
                        output.close();
                        bone_file += extension;
                        bone_file_fix = true;
                    }
                }
            }
        }

        if (mesh_file.size() > 4)
        {
            std::string extension = mesh_file.substr(mesh_file.size()-4);
            if (extension == ".obj")
            {
                model = new mh::ObjModel();
                if (!model->loadFromFile(mesh_file))
                {
                    delete model;
                    model = nullptr;
                }
                else
                {
                    extension = ".txt";
                    if (!model->saveToFile(mesh_file+extension, true, false, false, true, ",", ",", ","))
                    {
                        delete model;
                        model = nullptr;
                    }
                    else
                    {
                        mesh_file += extension;
                        mesh_file_fix = true;
                    }
                }
            }
        }

        mh::VoxelGrid grid(mesh_file, bone_file, weight_file, max_grid_num, max_diffuse_loop, max_sample_num, max_influence, max_fall_off, sharpness, detect_solidify == "y");
        grid.calculate_all_voxel_darkness();
        grid.diffuse_all_heats();
        grid.generate_weight_for_vertices();
        if (!grid.export_bone_weights())
        {
            if (loader != nullptr)
            {
                delete loader;
                loader = nullptr;
            }
            if (model != nullptr)
            {
                delete model;
                model = nullptr;
            }
            return -1;
        }

        ends = time(NULL);
        int seconds_elapsed = (int)(ends - start);
        std::cout << "Running Time : " << seconds_elapsed / 60 << " minutes and "
                  << seconds_elapsed % 60 << " seconds" << std::endl;

        if ((loader != nullptr) && (model != nullptr))
        {
            std::ifstream input;
            input.open(weight_file);
            if (input.is_open())
            {
                std::vector<std::string> boneNames;
                std::map<std::string, std::map<unsigned int, float>> bones;
                while (input.good())
                {
                    std::string line = "";
                    std::stringstream stream;
                    std::vector<std::string> splits;
                    if (!std::getline(input, line))
                    {
                        break;
                    }
                    stream = std::stringstream(line);
                    for (std::string split = ""; std::getline(stream, split, ',');)
                    {
                        splits.push_back(split);
                    }
                    if (splits.empty())
                    {
                        continue;
                    }
                    if (splits.front() == "b")
                    {
                        if (bones.find(splits.back()) == bones.end())
                        {
                            boneNames.push_back(splits.back());
                            bones[boneNames.back()] = std::map<unsigned int, float>();
                        }
                    }
                    else if ((splits.front() == "w") && (splits.size() > 3))
                    {
                        unsigned int boneName = static_cast<unsigned int>(atoi(splits[2].c_str()));
                        if (boneName < boneNames.size())
                        {
                            bones[boneNames[boneName]][static_cast<unsigned int>(atoi(splits[1].c_str()))] = static_cast<float>(atof(splits[3].c_str()));
                        }
                    }
                }
                input.close();
                if (mesh_file_fix)
                {
                    mesh_file = mesh_file.substr(0, mesh_file.size()-4);
                }
                if (!bones.empty())
                {
                    Assimp::Exporter* exporter = new Assimp::Exporter();
                    Assimp::Importer* importer = new Assimp::Importer();
                    if (importer->ReadFile(mesh_file, aiProcessPreset_TargetRealtime_MaxQuality|aiProcess_FlipUVs|aiProcess_LimitBoneWeights) != nullptr)
                    {
                        aiScene* scene = importer->GetOrphanedScene();
                        if (scene != nullptr)
                        {
                            aiScene* sceneOther = nullptr;
                            aiNode* root = scene->mRootNode;
                            if ((root != nullptr) && (loader->getRoot() != nullptr))
                            {
                                root = new aiNode(std::string("_"));
                                root->mNumChildren = 2;
                                root->mChildren = new aiNode*[2];
                                root->mChildren[0] = new aiNode(loader->getRoot()->getName());
                                root->mChildren[1] = scene->mRootNode;
                                root->mChildren[0]->mParent = root;
                                scene->mRootNode->mParent = root;
                                scene->mRootNode = root;
                                root = root->mChildren[0];
                            }
                            if (scene->mNumMeshes > 0)
                            {
                                aiMesh* mesh = scene->mMeshes[0];
                                if (mesh->mNumBones == 0)
                                {
                                    mesh->mNumBones = static_cast<unsigned int>(boneNames.size());
                                    mesh->mBones = new aiBone*[mesh->mNumBones];
                                    for (unsigned int i = 0; i < mesh->mNumBones; ++i)
                                    {
                                        unsigned int j = 0;
                                        aiBone* bone = new aiBone();
                                        mesh->mBones[i] = bone;
                                        bone->mName.Set(boneNames[i].c_str());
                                        bone->mNumWeights = static_cast<unsigned int>(bones[boneNames[i]].size());
                                        bone->mWeights = new aiVertexWeight[bone->mNumWeights];
                                        for (auto iter = bones[boneNames[i]].begin(); iter != bones[boneNames[i]].end(); iter++)
                                        {
                                            bone->mWeights[j].mVertexId = iter->first;
                                            bone->mWeights[j].mWeight = iter->second;
                                            j++;
                                        }
                                    }
                                }
                            }
                            if (bone_file_fix)
                            {
                                bone_file = bone_file.substr(0, bone_file.size()-4);
                                delete importer;
                                importer = new Assimp::Importer();
                                if (importer->ReadFile(bone_file, aiProcessPreset_TargetRealtime_MaxQuality|aiProcess_FlipUVs|aiProcess_LimitBoneWeights) != nullptr)
                                {
                                    sceneOther = importer->GetOrphanedScene();
                                    if (sceneOther != nullptr)
                                    {
                                        if ((sceneOther->mRootNode != nullptr) && (root != nullptr) && (loader->getRoot() != nullptr))
                                        {
                                            if ((strcmp(sceneOther->mRootNode->mName.C_Str(), root->mName.C_Str()) == 0) && (strcmp(loader->getRoot()->getName().c_str(), root->mName.C_Str()) == 0))
                                            {
                                                if ((root->mParent == scene->mRootNode) && (root == scene->mRootNode->mChildren[0]))
                                                {
                                                    delete root;
                                                    root = sceneOther->mRootNode;
                                                    scene->mRootNode->mChildren[0] = root;
                                                }
                                            }
                                        }
                                        scene->mNumAnimations = sceneOther->mNumAnimations;
                                        scene->mAnimations = sceneOther->mAnimations;
                                    }
                                }
                            }
                            exporter->Export(scene, "gltf2", mesh_file+".gltf");
                            if (sceneOther != nullptr)
                            {
                                if ((root == sceneOther->mRootNode) && (loader->getRoot() != nullptr))
                                {
                                    root = new aiNode(loader->getRoot()->getName());
                                    scene->mRootNode->mChildren[0] = root;
                                }
                                delete sceneOther;
                            }
                            scene->mNumAnimations = 0;
                            scene->mAnimations = nullptr;
                            delete scene;
                        }
                    }
                    delete exporter;
                    delete importer;
                }
            }
        }

        if (loader != nullptr)
        {
            delete loader;
            loader = nullptr;
        }
        if (model != nullptr)
        {
            delete model;
            model = nullptr;
        }

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

