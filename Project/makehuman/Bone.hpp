#ifndef MH_BONE_HPP
#define MH_BONE_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <string>

namespace mh
{
    class Bone
    {
        private:
	        std::string name;
	        int identifier;
	        glm::vec4 head;
	        glm::vec4 tail;
	        glm::mat4 rotation;
	        glm::mat4 translation;
	        std::vector<Bone*> children;
	        int channelCount;
	    protected:
	        Bone* parent;
        public:
	        float x;
	        float y;
	        float z;
	        int* channelOrder;
	        Bone();
	        virtual ~Bone();
	        void setName(const std::string& name);
	        void setChannelCount(int channelCount);
	        void setIdentifier(int identifier);
	        int getChannelCount() const;
	        void addChild(Bone* child);
	        void setPositions(const glm::mat4& modelMatrix);
	        void setRotation(const glm::mat4& rotation);
	        void setTranslation(const glm::mat4& translation);
	        void setTranslation();
	        const glm::mat4& getRotation() const;
	        const glm::mat4& getTranslation() const;
	        const glm::vec4& getHead() const;
	        const glm::vec4& getTail() const;
	        glm::vec3 getPosition() const;
	        const std::string& getName() const;
	        int getIdentifier() const;
	        int getChildCount() const;
	        Bone* getChild(int index) const;
	        Bone* getParent() const;
    };
}

#endif

