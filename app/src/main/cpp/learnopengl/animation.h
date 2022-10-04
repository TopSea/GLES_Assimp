#pragma once

#include <vector>
#include <map>
#include <glm/glm.hpp>
#include <assimp/scene.h>
#include "bone.h"
#include <functional>
#include "animdata.h"
#include "model_animation.h"
#include "assimp_glm_helpers.h"
#include "my_log.h"

struct AssimpNodeData
{
	glm::mat4 transformation;
	std::string name;
	int childrenCount;
	std::vector<AssimpNodeData> children;
};

class Animation
{
public:
	Animation() = default;

	Animation(const std::string& animationPath, Model* model)
	{
//        LOGE("Animation %lu", m_Bones.size());
        m_Bones.clear();
		Assimp::Importer importer;
        const aiScene* scene;
        try {
            scene = importer.ReadFile(animationPath, aiProcess_ValidateDataStructure);
        } catch (...) {
            LOGE("Something wrong with animation.");
        }
        if (scene == nullptr) {
            LOGE("importer.GetErrorString() %s", importer.GetErrorString());
        }

        assert(scene && scene->mRootNode);
        auto animation = scene->mAnimations[0];
        m_Duration = animation->mDuration;

        m_TicksPerSecond = animation->mTicksPerSecond;
        aiMatrix4x4 globalTransformation = scene->mRootNode->mTransformation;
        globalTransformation = globalTransformation.Inverse();
        ReadHeirarchyData(m_RootNode, scene->mRootNode);
        ReadMissingBones(animation, *model);
	}

	~Animation()
	{
	}

	Bone* FindBone(const std::string& name)
	{
//		LOGE("m_Bones.size() %lu", m_Bones.size());

		auto iter = std::find_if(m_Bones.begin(), m_Bones.end(),
			[&](const Bone& Bone)
			{
//				LOGE("name.c_str() %s", name.c_str());
//				LOGE("sizeof(m_BoneInfoMap) %d", Bone.GetBoneName() == name);
				return Bone.GetBoneName() == name;
			}
		);
		if (iter == m_Bones.end()) return nullptr;
		else return &(*iter);
	}

	
	inline float GetTicksPerSecond() { return m_TicksPerSecond; }
	inline float GetDuration() { return m_Duration;}
	inline const AssimpNodeData& GetRootNode() { return m_RootNode; }
	std::map<std::string,BoneInfo>& GetBoneIDMap()
	{
		return m_BoneInfoMap;
	}

private:
    float m_Duration;
    int m_TicksPerSecond;
    std::vector<Bone> m_Bones;
    AssimpNodeData m_RootNode;
    std::map<std::string, BoneInfo> m_BoneInfoMap;

	void ReadMissingBones(const aiAnimation* animation, Model& model)
	{
//        LOGE("m_Bones.size() %lu", m_Bones.size());
        m_Bones.clear();
		int size = animation->mNumChannels;

		auto& boneInfoMap = model.GetBoneInfoMap();//getting m_BoneInfoMap from Model class
		int& boneCount = model.GetBoneCount(); //getting the m_BoneCounter from Model class

		//reading channels(bones engaged in an animation and their keyframes)
		for (int i = 0; i < size; i++)
		{
			auto channel = animation->mChannels[i];
			std::string boneName = channel->mNodeName.data;

			if (boneInfoMap.find(boneName) == boneInfoMap.end())
			{
				boneInfoMap[boneName].id = boneCount;
				boneCount++;
				LOGE("boneInfoMap.find %s", boneName.c_str());
			}
			m_Bones.push_back(Bone(channel->mNodeName.data,
				boneInfoMap[channel->mNodeName.data].id, channel));
		}
//        LOGE("ReadMissingBones::::m_Bones.size() %lu", m_Bones.size());

		m_BoneInfoMap = boneInfoMap;
	}

	void ReadHeirarchyData(AssimpNodeData& dest, const aiNode* src)
	{
//        LOGE("ReadHeirarchyData %lu", m_Bones.size());
		assert(src);

		dest.name = src->mName.data;
		dest.transformation = AssimpGLMHelpers::ConvertMatrixToGLMFormat(src->mTransformation);
		dest.childrenCount = src->mNumChildren;

		for (int i = 0; i < src->mNumChildren; i++)
		{
			AssimpNodeData newData;
			ReadHeirarchyData(newData, src->mChildren[i]);
			dest.children.push_back(newData);
		}
	}
};

