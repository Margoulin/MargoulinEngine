#include "Mesh.hpp"

auto	Mesh::Shutdown() -> void
{
	for (auto && subMesh : meshes)
	{
		subMesh->Shutdown();
		DEL(subMesh);
	}
}