#ifndef __SKELETALMESH_HPP__
#define __SKELETALMESH_HPP__

#include <vector>

#include "CoreMinimal.hpp"
#include "SubMeshData.hpp"

class SkeletalMesh
{
public:
	SkeletalMesh() = default;
	SkeletalMesh(const SkeletalMesh&) = delete;
	SkeletalMesh(SkeletalMesh&&) = delete;
	~SkeletalMesh() = default;

	auto	AddSubMesh() -> SubMeshData* { meshes.push_back(NEW SubMeshData()); return meshes[meshes.size() - 1]; }

	auto	operator = (const SkeletalMesh&)->SkeletalMesh& = delete;
	auto	operator = (SkeletalMesh&&)->SkeletalMesh& = delete;

protected:

private:
	std::vector<SubMeshData*>	meshes;
};


#endif /*__SKELETALMESH_HPP__*/