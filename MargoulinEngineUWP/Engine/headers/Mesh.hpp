#ifndef __MESH_HPP__
#define __MESH_HPP__

#include <vector>
#include "SubMeshData.hpp"
#include "CoreMinimal.hpp"

#include "cereal/archives/binary.hpp"
#include "cereal/types/vector.hpp"

struct SerializeMesh
{
	std::vector<SSubMesh>	meshes;

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(meshes);
	}

};

class Mesh
{
public:
	Mesh() = default;
	Mesh(const Mesh&) = delete;
	Mesh(Mesh&&) = delete;
	~Mesh() = default;

	auto	IsInitialized() const -> bool const { return init; }

	auto	Shutdown() -> void;

	auto	AddMesh(SubMeshData* val) -> void { meshes.push_back(val); }

	auto	GetSubMeshNbr() const -> unsigned int { return (unsigned int)meshes.size(); }
	auto	GetSubMeshTab() -> SubMeshData** { return meshes.data(); }

	auto	operator = (const Mesh&)->Mesh& = delete;
	auto	operator = (Mesh&&)->Mesh& = delete;

protected:

private:
	std::vector<SubMeshData*>	meshes;
	bool						init = false;
};


#endif /*__MESH_HPP__*/