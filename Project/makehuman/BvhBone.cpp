
#include "BvhBone.hpp"
#include <iostream>

mh::BvhBone::BvhBone()
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

mh::BvhBone::~BvhBone()
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

void mh::BvhBone::setName(const std::string& name)
{
	this->name = name;
}

void mh::BvhBone::setChannelCount(int channelCount)
{
	this->channelCount = channelCount;
}

void mh::BvhBone::setIdentifier(int identifier)
{
	this->identifier = identifier;
}

int mh::BvhBone::getChannelCount() const
{
	return channelCount;
}

void mh::BvhBone::addChild(BvhBone* child)
{
    if (child == nullptr)
    {
        return;
    }
    child->parent = this;
	children.push_back(child);
}

void mh::BvhBone::setPositions(const glm::mat4& modelMatrix)
{
    /*const float* matrix = static_cast<const float*>(glm::value_ptr(modelMatrix));
    for (int i = 0; i < 16; ++i)
    {
        std::cout << matrix[i] << ",";
    }
    std::cout << getName() << std::endl;*/
    if (getChildCount() > 0)
    {
        tail = getChild(0)->getHead();
    }
    head = modelMatrix*glm::vec4(x, y, z, 1.0f);
}

void mh::BvhBone::setRotation(const glm::mat4& rotation)
{
	this->rotation = rotation;
}

void mh::BvhBone::setTranslation(const glm::mat4& translation)
{
	this->translation = translation;
}

void mh::BvhBone::setTranslation()
{
	glm::mat4 translation = glm::mat4(1.0f);
	this->translation = glm::translate(translation, getPosition());
}

const glm::mat4& mh::BvhBone::getRotation() const
{
	return rotation;
}

const glm::mat4& mh::BvhBone::getTranslation() const
{
	return translation;
}

const glm::vec4& mh::BvhBone::getHead() const
{
    return head;
}

const glm::vec4& mh::BvhBone::getTail() const
{
    return tail;
}

void mh::BvhBone::setHead(const glm::vec4& head)
{
    this->head = head;
}

void mh::BvhBone::setTail(const glm::vec4& tail)
{
    this->tail = tail;
}

glm::vec3 mh::BvhBone::getPosition() const
{
    return glm::vec3(x, y, z);
}

const std::string& mh::BvhBone::getName() const
{
	return name;
}

int mh::BvhBone::getIdentifier() const
{
	return identifier;
}

int mh::BvhBone::getChildCount() const
{
	return children.size();
}

mh::BvhBone* mh::BvhBone::getChild(int index) const
{
    if (getChildCount() < index)
    {
        return nullptr;
    }
	return children[index];
}

mh::BvhBone* mh::BvhBone::getParent() const
{
    return parent;
}

