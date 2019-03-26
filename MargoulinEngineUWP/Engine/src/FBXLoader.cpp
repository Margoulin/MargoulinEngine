#include "FBXLoader.hpp"

#include "../OpenFBX/ofbx.h"
#include <iostream>

#include "Mesh.hpp"
#include "MeshResource.hpp"
#include "Engine.hpp"
#include "ResourcesManager.hpp"

#include "SkeletalMesh.hpp"
#include "SkeletalMeshResource.hpp"

auto	FBXLoader::LoadFBXFromFile(MString const& filename) -> void
{
	struct TempSkin
	{
		float	weights[4];
		unsigned int	indices[4];
		int count = 0;
	};

	FILE* fp;
	fopen_s(&fp, filename.Str(), "rb");
	if (!fp) 
		return;

	fseek(fp, 0, SEEK_END);
	long file_size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	auto* content = NEW ofbx::u8[file_size];
	fread(content, 1, file_size, fp);
	auto* g_scene = ofbx::load((ofbx::u8*)content, file_size);

	ResourcesManager* rsMgr = Engine::GetInstance()->GetService<ResourcesManager>("Resources Manager");
	unsigned int meshID = rsMgr->CreateResource<SkeletalMeshResource>();
	SkeletalMeshResource*	meshRsc = rsMgr->GetResource<SkeletalMeshResource>(meshID);
	
	SkeletalMesh&	skelMesh = meshRsc->GetSkeletalMeshData();
	meshRsc->SetName(filename.Str());

	std::vector<MString>	bones;

	for (int pos = 0; pos < g_scene->getMeshCount(); pos++)
	{
		auto* fbxMesh = g_scene->getMesh(pos);
		auto* geo = fbxMesh->getGeometry();
		auto* skin = geo->getSkin();

		ofbx::Vec3 const* const vertices = geo->getVertices();
		ofbx::Vec3 const* const normals = geo->getNormals();
		ofbx::Vec2 const* const uvs = geo->getUVs();
		for (int pos = 0; pos < geo->getVertexCount(); pos++)
		{
			skelMesh.AddVertice(Vector3F((float)vertices[pos].x, (float)vertices[pos].y, (float)vertices[pos].z));
			//subMeshData->AddNormal(Vector3F((float)normals[pos].x, (float)normals[pos].y, (float)normals[pos].z));
			//subMeshData->AddTexCoords(Vector2F((float)uvs[pos].x, (float)uvs[pos].y));
			skelMesh.AddIndice(pos);
		}

		for (int clustIdx = 0; clustIdx < skin->getClusterCount(); clustIdx++)
		{
			auto* tempCluster = skin->getCluster(clustIdx);
			bones.push_back(tempCluster->getLink()->name);
		}

		std::vector<TempSkin>	tempSkins(skelMesh.GetVerticesCount());

		for (int clustIdx = 0; clustIdx < skin->getClusterCount(); clustIdx++)
		{
			auto* tempCluster = skin->getCluster(clustIdx);
			if (tempCluster->getIndicesCount() == 0)
				continue;
			int idx = -1;
			for (int pos = 0; pos < bones.size(); pos++)
			{
				if (bones[pos] == MString(tempCluster->getLink()->name))
				{
					idx = pos;
					break;
				}
			}

			const int* indices = tempCluster->getIndices();
			const double* weights = tempCluster->getWeights();
			for (int pos = 0; pos < tempCluster->getIndicesCount(); pos++)
			{
				int index = indices[pos];
				float weight = (float)weights[pos];
				TempSkin& skin = tempSkins[index];
				if (skin.count < 4)
				{
					skin.weights[skin.count] = weight;
					skin.indices[skin.count] = idx;
					skin.count++;
				}
				else
				{
					int min = 0;
					for (int pos2 = 0; pos2 < 4; pos2++)
					{
						if (skin.weights[pos2] < skin.weights[min])
							min = pos2;
					}
					if (skin.weights[min] < weight)
					{
						skin.weights[min] = weight;
						skin.indices[min] = idx;
					}
				}
			}
		}	
		for (auto& skin : tempSkins)
		{
			float sum = 0;
			sum += skin.weights[0] + skin.weights[1] + skin.weights[2] + skin.weights[3];
			for (int pos = 0; pos < 4; pos++)
				skin.weights[pos] /= sum;
			Skin* s = skelMesh.AddSkin();
			memcpy(s->clusterIndices, skin.indices, sizeof(unsigned int) * 4);
			memcpy(s->clusterWeights, skin.weights, sizeof(float) * 4);
		}

		Rig* rig = skelMesh.CreateRig(skin->getClusterCount());
		for (int clust = 0; clust < skin->getClusterCount(); clust++)
		{
			Bone* bone = rig->GetBone(clust);
			bone->Name = skin->getCluster(clust)->getLink()->name;
			
			auto tempMat = skin->getCluster(clust)->getTransformLinkMatrix();

			for (int f = 0; f < 16; f++)
				bone->TPoseMatrix[f] = tempMat.m[f];
		}

		for (int clust = 0; clust < skin->getClusterCount(); clust++)
		{
			auto* parent = skin->getCluster(clust)->getLink()->getParent();
			if (parent)
			{
				for (int tempBone = 0; tempBone < rig->GetBoneCount(); tempBone++)
				{
					if (rig->GetBone(tempBone)->Name == parent->name)
					{
						rig->GetBone(clust)->ParentIdx = tempBone;
						break;
					}
				}
			}
		}

		AnimationPose* bindPose = skelMesh.GetBindPose();
		for (unsigned int boneId = 0; boneId < rig->GetBoneCount(); boneId++)
		{
			Bone* bone = rig->GetBone(boneId);
			std::vector<Matrix4x4F>	matrices;
			matrices.push_back(bone->TPoseMatrix);
			int idxToCheck = bone->ParentIdx;
			while(idxToCheck != -1)
			{
				Bone* parentBone = rig->GetBone(idxToCheck);
				idxToCheck = parentBone->ParentIdx;
				matrices.push_back(parentBone->TPoseMatrix);
			}

			Matrix4x4F finalMat = Matrix4x4F::identity;
			for (auto& mat : matrices)
				finalMat *= mat;
			finalMat = Matrix4x4F::Transpose(finalMat);
			finalMat = Matrix4x4F::identity;
			for (int f = 0; f < 16; f++)
				bindPose->bonesFloat.push_back(finalMat[f]);
		}

		for (int i = 0; i < rig->GetBoneCount(); i++)
		{
			Skin& s = skelMesh.GetSkin(i);
			s.clusterWeights[0] = 1.0f;
			s.clusterWeights[1] = 0.0f;
			s.clusterWeights[2] = 0.0f;
			s.clusterWeights[3] = 0.0f;
		}

		std::vector<Bone>	bones;
		for (int clust = 0; clust < skin->getClusterCount(); clust++)
			bones.push_back(*rig->GetBone(clust));
	}

	DEL(content);
	g_scene->destroy();
}