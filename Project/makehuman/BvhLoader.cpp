
#include "BvhLoader.hpp"
#include <iostream>

mh::BvhLoader::BvhLoader()
{
    initialize();
}

mh::BvhLoader::BvhLoader(const std::string& path)
{
    initialize();
    load(path);
}

mh::BvhLoader::~BvhLoader()
{
    if (root != nullptr)
    {
        delete root;
        root = nullptr;
    }
    if (reader != nullptr)
    {
        if (reader->is_open())
        {
            reader->close();
        }
        delete reader;
        reader = nullptr;
    }
    if (motion != nullptr)
    {
        delete[] motion;
        motion = nullptr;
    }
    if (visits != nullptr)
    {
        delete[] visits;
        visits = nullptr;
    }
}

void mh::BvhLoader::initialize()
{
    status = false;
    reader = nullptr;
    root = nullptr;
    ends = 0;
    boneCount = 0;
    frameCount = 0;
    motionIndex = 0;
    channelTotal = 0;
    frameTime = 0.0;
    motion = nullptr;
    visits = nullptr;
}

bool mh::BvhLoader::visit(BvhBone* bone, std::function<bool(BvhBone*)> visitor)
{
    if (bone == nullptr)
    {
        if (root == nullptr)
        {
            return false;
        }
        bone = root;
    }
    if (bone == root)
    {
        clearVisits();
    }
    if (!visitor(bone))
    {
        if (bone == root)
        {
            clearVisits();
        }
        return false;
    }
    visits[bone->getIdentifier()] += 1;
    for (int i = 0; i < bone->getChildCount(); ++i)
    {
        BvhBone* child = bone->getChild(i);
        if (child == nullptr)
        {
            continue;
        }
        if (visits[child->getIdentifier()] <= 0)
        {
            if (!visit(child, visitor))
            {
                break;
            }
        }
    }
    if (bone == root)
    {
        clearVisits();
    }
    return true;
}

bool mh::BvhLoader::load(const std::string& path)
{
    if (reader != nullptr)
    {
        return false;
    }
    status = true;
    reader = new std::ifstream();
    reader->open(path, std::ifstream::in);
    if (reader->is_open())
    {
        std::string temp = "";
        while (reader->good())
        {
            *reader >> temp;
            if (temp.compare("ROOT") == 0)
            {
                if (root == nullptr)
                {
                    root = readBoneRelation(*reader);
                }
                break;
            }
        }
        visit(nullptr, [&](BvhBone* bone)
        {
            bones.push_back(bone);
            return true;
        });
        if ((root == nullptr) || (!readBoneMotion(&frameTime)) || (!reader->good()))
        {
            status = false;
            return false;
        }
        if (motion != nullptr)
        {
            setFrame(nullptr, 0);
            setFrame(root, 0);
        }
        return true;
    }
    status = false;
    delete reader;
    reader = nullptr;
    return false;
}

bool mh::BvhLoader::readBoneMotion(double* frameTime)
{
    int offset = 0;
    int index = 0;
    int boneIndex = 0;
    int frameCounter = 0;
    std::string temp = "";
    while (reader->good())
    {
        *reader >> temp;
        if (temp.compare("MOTION") == 0)
        {
            break;
        }
    }
    if (temp.compare("MOTION") != 0)
    {
        return false;
    }
    *reader >> temp >> frameCount;
    if (motion != nullptr)
    {
        delete[] motion;
    }
    motion = new float[frameCount*channelTotal];
    for (int i = 0; i < frameCount*channelTotal; ++i)
    {
        motion[i] = 0.0f;
    }
    *reader >> temp >> temp >> this->frameTime;
    if (frameTime != nullptr)
    {
        *frameTime = this->frameTime;
    }
    while ((index != frameCount*channelTotal) && (reader->good()))
    {
        *reader >> motion[index];
        //std::cout << motion[index] << " " << index << " " << offset << " " << boneIndex << " " << bones[boneIndex]->getChannelCount() << std::endl;
        ++index;
        if (boneIndex < bones.size())
        {
            if (index-offset == bones[boneIndex]->getChannelCount())
            {
                //std::cout << bones[boneIndex]->getName() << " @ " << index << std::endl;
                bones[boneIndex]->addMotion(frameCounter, motion+(index-bones[boneIndex]->getChannelCount()));
                do
                {
                    ++boneIndex;
                    if (boneIndex >= bones.size())
                    {
                        break;
                    }
                } while (bones[boneIndex]->getChannelCount() == 0);
                offset = index;
            }
        }
        if (index%channelTotal == 0)
        {
            boneIndex = 0;
            ++frameCounter;
        }
    }
    if (!reader->good())
    {
        return false;
    }
    return true;
}

mh::BvhBone* mh::BvhLoader::readBoneRelation(std::ifstream& reader)
{
    std::string temp1 = "";
    std::string temp2 = "";
    std::string temp3 = "";
    BvhBone* newBone = new BvhBone();
    reader >> temp1;
    newBone->setName(temp1);
    newBone->setIdentifier(boneCount);
    ++boneCount;
    while (reader.good())
    {
        reader >> temp1;
        if (temp1.compare("OFFSET") == 0)
        {
            reader >> newBone->x >> newBone->y >> newBone->z;
            newBone->setTranslation();
        }
        else if (temp1.compare("CHANNELS") == 0)
        {
            int channelCount;
            reader >> channelCount;
            channelTotal += channelCount;
            newBone->setChannelCount(channelCount);
            if (newBone->channelOrder != nullptr)
            {
                delete[] newBone->channelOrder;
            }
            newBone->channelOrder = new int[channelCount];
            for (int i = 0; i < channelCount; ++i)
            {
                reader >> temp1;
                if (temp1.compare("Xposition") == 0)
                {
                    newBone->channelOrder[i] = MH_BONE_X_POS;
                }
                else if (temp1.compare("Yposition") == 0)
                {
                    newBone->channelOrder[i] = MH_BONE_Y_POS;
                }
                else if (temp1.compare("Zposition") == 0)
                {
                    newBone->channelOrder[i] = MH_BONE_Z_POS;
                }
                else if (temp1.compare("Xrotation") == 0)
                {
                    newBone->channelOrder[i] = MH_BONE_X_ROT;
                }
                else if (temp1.compare("Yrotation") == 0)
                {
                    newBone->channelOrder[i] = MH_BONE_Y_ROT;
                }
                else if (temp1.compare("Zrotation") == 0)
                {
                    newBone->channelOrder[i] = MH_BONE_Z_ROT;
                }
            }
        }
        else if (temp1.compare("JOINT") == 0)
        {
            BvhBone* childBone = readBoneRelation(reader);
            if (childBone != nullptr)
            {
                newBone->addChild(childBone);
            }
        }
        else if (temp1.compare("End") == 0)
        {
            BvhBone* childBone = new BvhBone();
            childBone->setName(temp1+std::to_string(ends));
            childBone->setIdentifier(boneCount);
            ++ends;
            ++boneCount;
            reader >> temp1 >> temp2 >> temp3;
            reader >> childBone->x >> childBone->y >> childBone->z;
            reader >> temp1;
            newBone->addChild(childBone);
        }
        else if (temp1.compare("}") == 0)
        {
            return newBone;
        }
    }
    delete newBone;
    return nullptr;
}

void mh::BvhLoader::clearVisits()
{
    if (boneCount <= 0)
    {
        return;
    }
    if (visits == nullptr)
    {
        visits = new int[boneCount];
    }
    for (int i = 0; i < boneCount; ++i)
    {
        visits[i] = 0;
    }
}

void mh::BvhLoader::resetMatrices(BvhBone* bone)
{
    if (bone == nullptr)
    {
        if (root == nullptr)
        {
            return;
        }
        bone = root;
    }
    if (visits == nullptr)
    {
        clearVisits();
    }
    bone->setRotation(glm::mat4(1.0f));
    bone->setTranslation();
    visits[bone->getIdentifier()] += 1;
    for (int i = 0; i < bone->getChildCount(); ++i)
    {
        BvhBone* child = bone->getChild(i);
        if (child == nullptr)
        {
            continue;
        }
        if (visits[child->getIdentifier()] <= 0)
        {
            resetMatrices(child);
        }
    }
}

void mh::BvhLoader::setFrame(BvhBone* bone, int frameIndex)
{
    if (bone == nullptr)
    {
        if (root == nullptr)
        {
            return;
        }
        bone = root;
        clearVisits();
        resetMatrices(bone);
    }
    clearVisits();
    if (frameIndex >= frameCount)
    {
        return;
    }
    motionIndex = frameIndex*channelTotal;
    setMotion(bone, frameIndex, glm::mat4(1.0f));
}

void mh::BvhLoader::setMotion(BvhBone* bone, int frameIndex, glm::mat4 modelMatrix)
{
    if (motion == nullptr)
    {
        //std::cout << "No motion!" << std::endl;
        return;
    }
    if (bone == nullptr)
    {
        bone = root;
    }
    if (bone != nullptr)
    {
        int* channelOrder = bone->channelOrder;
        if (channelOrder != nullptr)
        {
            for (int i = 0; i < bone->getChannelCount(); ++i)
            {
                bool success = true;
                if (channelOrder[i] == MH_BONE_X_POS)
                {
                    bone->setTranslation(glm::translate(bone->getTranslation(), glm::vec3(motion[motionIndex], 0.0f, 0.0f)));
                }
                else if (channelOrder[i] == MH_BONE_Y_POS)
                {
                    bone->setTranslation(glm::translate(bone->getTranslation(), glm::vec3(0.0f, motion[motionIndex], 0.0f)));
                }
                else if (channelOrder[i] == MH_BONE_Z_POS)
                {
                    bone->setTranslation(glm::translate(bone->getTranslation(), glm::vec3(0.0f, 0.0f, motion[motionIndex])));
                }
                else if (channelOrder[i] == MH_BONE_X_ROT)
                {
                    bone->setRotation(glm::rotate(bone->getRotation(), glm::radians(motion[motionIndex]), glm::vec3(1.0f, 0.0f, 0.0f)));
                }
                else if (channelOrder[i] == MH_BONE_Y_ROT)
                {
                    bone->setRotation(glm::rotate(bone->getRotation(), glm::radians(motion[motionIndex]), glm::vec3(0.0f, 1.0f, 0.0f)));
                }
                else if (channelOrder[i] == MH_BONE_Z_ROT)
                {
                    bone->setRotation(glm::rotate(bone->getRotation(), glm::radians(motion[motionIndex]), glm::vec3(0.0f, 0.0f, 1.0f)));
                }
                else
                {
                    success = false;
                }
                if (success)
                {
                    ++motionIndex;
                }
            }
        }
        else
        {
            //std::cout << "No channel order @ \"" << bone->getName() << "\"" << std::endl;
        }
        bone->setPositions(modelMatrix);
        modelMatrix = modelMatrix*bone->getTranslation()*bone->getRotation();
        visits[bone->getIdentifier()] += 1;
        for (int i = 0; i < bone->getChildCount(); ++i)
        {
            BvhBone* child = bone->getChild(i);
            if (child == nullptr)
            {
                continue;
            }
            if (visits[child->getIdentifier()] <= 0)
            {
                setMotion(child, frameIndex, modelMatrix);
            }
        }
    }
}

double mh::BvhLoader::getFrameTime() const
{
    return frameTime;
}

bool mh::BvhLoader::getValidity() const
{
    return status;
}

mh::BvhBone* mh::BvhLoader::getRoot() const
{
    return root;
}

