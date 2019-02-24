#ifndef __SKELETAL_MESH_RESOURCE_HPP__
#define __SKELETAL_MESH_RESOURCE_HPP__

#include "Resource.hpp"

#include "SkeletalMesh.hpp"

class SkeletalMeshResource : public Resource
{
public:
	SkeletalMeshResource() { type = ResourceType::RESOURCE_SKELETALMESH; }
	SkeletalMeshResource(const SkeletalMeshResource&) = delete;
	SkeletalMeshResource(SkeletalMeshResource&&) = delete;
	~SkeletalMeshResource() = default;

	virtual auto	Load() -> void {}
	virtual auto	Unload() -> void {}
	virtual auto	Shutdown() -> void {}

	auto	GetSkeletalMeshConstData() const -> SkeletalMesh const& { return skeletalMeshData; }
	auto	GetSkeletalMeshData() -> SkeletalMesh& { return skeletalMeshData; }

	auto	operator = (const SkeletalMeshResource&)->SkeletalMeshResource& = delete;
	auto	operator = (SkeletalMeshResource&&)->SkeletalMeshResource& = delete;

protected:

private:
	SkeletalMesh	skeletalMeshData;

#ifdef _DEBUG

public:
	virtual auto	ImGuiUpdate() -> void {}

#endif // _DEBUG
};


#endif /*__SKELETAL_MESH_RESOURCE_HPP__*/