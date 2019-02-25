#ifndef __MESHCOMPONENT_HPP__
#define __MESHCOMPONENT_HPP__

#include "GraphicComponent.hpp"

class MeshComponent final
	: public GraphicComponent
{
public:
	MeshComponent() = default;
	MeshComponent(const MeshComponent&) = delete;
	MeshComponent(MeshComponent&&) = delete;
	~MeshComponent() = default;

	enum MESH_TYPE
	{
		CUBE, PYRAMID, SPHERE, CUSTOM, NONE
	};

	auto	SetMeshType(MESH_TYPE const value) -> void { meshType = value; }
	auto	SetCustomMesh(unsigned int const& value) -> void { meshID = value; }
	auto	SetMaterial(unsigned int const& matID) -> void { mat = matID; }

	auto	GetMaterial() const -> unsigned int const& { return mat; }
	auto	GetMeshType() const -> MESH_TYPE const { return meshType; }
	auto	GetMesh() const -> unsigned int const& { return meshID; }

	virtual auto	GetObjectType() const -> ObjectType const { return MESH_COMPONENT; }
	virtual auto	GetObjectTypeName() const -> MString const override { return "Mesh Component"; }

	auto	operator = (const MeshComponent&)->MeshComponent& = delete;
	auto	operator = (MeshComponent&&)->MeshComponent& = delete;

protected:

private:
	MESH_TYPE		meshType = NONE;
	unsigned int	mat;
	unsigned int	meshID = 0;

#ifdef _DEBUG

public:
	virtual auto	ImGuiUpdate() -> void override;
	virtual auto	GetComponentTypeName() -> char* { return "Mesh Component"; }

#endif // _DEBUG

};


#endif /*__MESHCOMPONENT_HPP__*/