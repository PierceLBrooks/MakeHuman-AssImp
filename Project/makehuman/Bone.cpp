
#include "Bone.hpp"
#include <iostream>

mh::Bone::Bone()
{
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
    channelCount = 0;
    channelOrder = nullptr;
    identifier = -1;
    name = "";
	rotation = glm::mat4(1.0f);
	translation = glm::mat4(1.0f);
	head = glm::vec4(x, y, z, 1.0f);
	tail = glm::vec4(x, y, z, 1.0f);
	parent = nullptr;
}

mh::Bone::~Bone()
{
    if (channelOrder != nullptr)
    {
        delete[] channelOrder;
        channelOrder = nullptr;
    }
    for (int i = 0; i < getChildCount(); ++i)
    {
        delete children[i];
    }
    children.clear();
}

void mh::Bone::setName(const std::string& name)
{
	this->name = name;
}

void mh::Bone::setChannelCount(int channelCount)
{
	this->channelCount = channelCount;
}

void mh::Bone::setIdentifier(int identifier)
{
	this->identifier = identifier;
}

int mh::Bone::getChannelCount() const
{
	return channelCount;
}

void mh::Bone::addChild(Bone* child)
{
    if (child == nullptr)
    {
        return;
    }
    child->parent = this;
	children.push_back(child);
}

void mh::Bone::setPositions(const glm::mat4& modelMatrix)
{
    /*const float* matrix = static_cast<const float*>(glm::value_ptr(modelMatrix));
    for (int i = 0; i < 16; ++i)
    {
        std::cout << matrix[i] << ",";
    }
    std::cout << getName() << std::endl;*/
    if (parent != nullptr)
    {
        tail = parent->getHead();
    }
    head = modelMatrix*glm::vec4(x, y, z, 1.0f);
}

void mh::Bone::setRotation(const glm::mat4& rotation)
{
	this->rotation = rotation;
}

void mh::Bone::setTranslation(const glm::mat4& translation)
{
	this->translation = translation;
}

void mh::Bone::setTranslation()
{
	glm::mat4 translation = glm::mat4(1.0f);
	this->translation = glm::translate(translation, getPosition());
}

const glm::mat4& mh::Bone::getRotation() const
{
	return rotation;
}

const glm::mat4& mh::Bone::getTranslation() const
{
	return translation;
}

const glm::vec4& mh::Bone::getHead() const
{
    return head;
}

const glm::vec4& mh::Bone::getTail() const
{
    return tail;
}

glm::vec3 mh::Bone::getPosition() const
{
    return glm::vec3(x, y, z);
}

const std::string& mh::Bone::getName() const
{
	return name;
}

int mh::Bone::getIdentifier() const
{
	return identifier;
}

int mh::Bone::getChildCount() const
{
	return children.size();
}

mh::Bone* mh::Bone::getChild(int index) const
{
    if (getChildCount() < index)
    {
        return nullptr;
    }
	return children[index];
}

mh::Bone* mh::Bone::getParent() const
{
    return parent;
}

