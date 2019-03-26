#ifndef __SKELETALMESH_HPP__
#define __SKELETALMESH_HPP__

#include <vector>

#include "CoreMinimal.hpp"
#include "SubMeshData.hpp"

struct Bone
{
	Matrix4x4F	TPoseMatrix = Matrix4x4F::identity;
	MString		Name;
	int			ParentIdx = -1;
};

class Rig
{
public:
	Rig(unsigned int size);
	Rig(const Rig&) = delete;
	Rig(Rig&&) = delete;
	~Rig();

	Bone*			GetBone(unsigned int idx) const { return &bones[idx]; }
	unsigned int	GetBoneCount() const { return boneCount; }

private:
	Bone*			bones = nullptr;
	unsigned int	boneCount = 0;
};

struct Skin
{
	unsigned int	clusterIndices[4];
	float			clusterWeights[4];
};
	
struct AnimationPose
{
	std::vector<float>	bonesFloat;
};

class SkeletalMesh
{
public:
	SkeletalMesh() = default;
	SkeletalMesh(const SkeletalMesh&) = delete;
	SkeletalMesh(SkeletalMesh&&) = delete;
	~SkeletalMesh() = default;

	Skin*	AddSkin() { skins.push_back(Skin()); return &skins[skins.size() - 1]; }
	void	AddVertice(Vector3F const& value) { vertices.push_back(value); }
	void	AddIndice(unsigned int const& value) { indices.push_back(value); }

	Rig*	CreateRig(unsigned int size) { return rig = NEW Rig(size); }
	void	ReserveSkinArray(unsigned int size) { skins.reserve(size); }

	void	Shutdown();

	auto	SetVertexBufferArray(GPUBufferArray* buffer) -> void { verticesBufferArray = buffer; }
	auto	SetIndexBuffer(GPUBuffer* buffer) -> void { indicesBuffer = buffer; }
	void	SetInVRAM(bool value) { inVRAM = value; }

	auto	GetIndexBuffer() const -> GPUBuffer* { return indicesBuffer; }
	auto	GetVertexBufferArray() const-> GPUBufferArray* { return verticesBufferArray; }

	auto	GetBindPose() -> AnimationPose* { return &bindPose; }
	auto	GetRig() const -> Rig* { return rig; }
	auto	GetSkin(unsigned int const& idx) -> Skin& { return skins[idx]; }
	//auto	GetVerticesDataSize() const -> unsigned int { return vertices.size() * sizeof(Vector3F) + normals.size() * sizeof(Vector3F) + uv.size() * sizeof(Vector2F); }
	auto	GetVerticesDataSize() const -> unsigned int { return vertices.size() * sizeof(Vector3F); }
	auto	GetVerticesCount() const -> unsigned int const { return (unsigned int)vertices.size(); }
	//auto	GetNormalsCount() const -> unsigned int const { return (unsigned int)normals.size(); }
	auto	GetIndicesCount() const -> unsigned int const { return (unsigned int)indices.size(); }
	//auto	GetUVCount() const -> unsigned int const { return (unsigned int)uv.size(); }
	auto	GetVertices() -> Vector3F* { return &vertices[0]; }
	auto	GetIndices() -> unsigned int* { return &indices[0]; }
	auto	GetSkins() -> Skin* { return skins.data(); }
	//auto	GetUV() -> Vector2F* { return &uv[0]; }
	bool	IsInVRAM() const { return inVRAM; }

	auto	operator = (const SkeletalMesh&)->SkeletalMesh& = delete;
	auto	operator = (SkeletalMesh&&)->SkeletalMesh& = delete;

protected:

private:
	std::vector<Vector3F>		vertices;
	std::vector<Skin>	skins;
	std::vector<unsigned int>	indices;
	unsigned int				bonesCount = 0;
	GPUBufferArray*				verticesBufferArray = nullptr;
	GPUBuffer*					indicesBuffer = nullptr;
	Rig*						rig = nullptr;
	AnimationPose				bindPose;

	bool	inVRAM = false;
};


#endif /*__SKELETALMESH_HPP__*/