#ifndef __MESHRESOURCE_HPP__
#define __MESHRESOURCE_HPP__

#include "Resource.hpp"

class Mesh;

class MeshResource : public Resource
{
public:
	MeshResource() { type = ResourceType::RESOURCE_MESH; }
	MeshResource(const MeshResource&) = delete;
	MeshResource(MeshResource&&) = delete;
	~MeshResource() = default;

	virtual auto	Load() -> void;
	virtual auto	Unload() -> void;
	virtual auto	Shutdown() -> void;

	auto	SetMeshData(Mesh* meshValue) -> void { meshData = meshValue; }

	auto	GetMeshData() const -> Mesh* { return meshData; }

	auto	operator = (const MeshResource&)->MeshResource& = delete;
	auto	operator = (MeshResource&&)->MeshResource& = delete;

protected:

private:
	Mesh*		meshData = nullptr;

#ifdef _DEBUG

public:
	virtual auto	ImGuiUpdate() -> void;

#endif // _DEBUG

};


#endif /*__MESHRESOURCE_HPP__*/