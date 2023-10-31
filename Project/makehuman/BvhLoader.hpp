#ifndef MH_BVH_LOADER_HPP
#define MH_BVH_LOADER_HPP

#include <string>
#include <fstream>
#include <functional>
#include "Bone.hpp"

#define MH_BONE_X_POS 1
#define MH_BONE_Y_POS 2
#define MH_BONE_Z_POS 3
#define MH_BONE_X_ROT 4
#define MH_BONE_Y_ROT 5
#define MH_BONE_Z_ROT 6

namespace mh
{
    class BvhLoader
    {
        private:
            double frameTime;
            bool status;
            std::ifstream* reader;
            Bone* root;
            void initialize();
            bool readBoneMotion(double* frameTime = nullptr);
            Bone* readBoneRelation(std::ifstream& fileReader);
            void setMotion(Bone* bone, int frameIndex, glm::mat4 modelMatrix);
        public:
            int ends;
            int boneCount;
            int frameCount;
            int motionIndex;
            int channelTotal;
            float* motion;
            int* visits;
            BvhLoader();
            BvhLoader(const std::string& path);
            virtual ~BvhLoader();
            bool load(const std::string& path);
            bool visit(Bone* bone, std::function<bool(Bone*)> visitor);
            void setFrame(Bone* bone, int frameIndex);
            void clearVisits();
            void resetMatrices(Bone* bone = nullptr);
            double getFrameTime() const;
            bool getValidity() const;
            Bone* getRoot() const;
    };
}

#endif

