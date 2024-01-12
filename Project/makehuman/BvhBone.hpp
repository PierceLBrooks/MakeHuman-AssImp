#ifndef MH_BVH_BONE_HPP
#define MH_BVH_BONE_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <string>
#include <map>

namespace mh
{
    class BvhBone
    {
        private:
	        std::string name;
	        int identifier;
	        glm::vec4 head;
	        glm::vec4 tail;
	        glm::mat4 matrix;
	        glm::mat4 rotation;
	        glm::mat4 translation;
	        std::vector<BvhBone*> children;
	        std::map<int, float*> motion;
	        int channelCount;
	    protected:
	        BvhBone* parent;
        public:
	        float x;
	        float y;
	        float z;
	        int* channelOrder;
	        BvhBone();
	        virtual ~BvhBone();
	        void addMotion(int frameIndex, float* motion);
	        float* getMotion(int frameIndex);
	        void setName(const std::string& name);
	        void setChannelCount(int channelCount);
	        void setIdentifier(int identifier);
	        int getChannelCount() const;
	        void addChild(BvhBone* child);
	        void setPositions(const glm::mat4& modelMatrix);
	        void setRotation(const glm::mat4& rotation);
	        void setTranslation(const glm::mat4& translation);
	        void setTranslation();
	        const glm::mat4& getMatrix() const;
	        const glm::mat4& getRotation() const;
	        const glm::mat4& getTranslation() const;
	        const glm::vec4& getHead() const;
	        const glm::vec4& getTail() const;
	        void setHead(const glm::vec4& head);
	        void setTail(const glm::vec4& tail);
	        glm::vec3 getPosition() const;
	        const std::string& getName() const;
	        int getIdentifier() const;
	        int getChildCount() const;
	        BvhBone* getChild(int index) const;
	        BvhBone* getParent() const;
    };
}

#endif

