#ifndef __MESHCOMPONENT_HPP__
#define __MESHCOMPONENT_HPP__

#include "GraphicComponent.hpp"

#include <string>

class Model;

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

	virtual auto	Draw() -> void;
	virtual auto	Shutdown() -> void;

	auto	SetMeshType(MESH_TYPE const value) -> void { MType = value; }
	auto	SetCustomMesh(unsigned int const& value) -> void { meshID = value; }
	auto	SetMaterial(unsigned int const& matID) -> void { mat = matID; }

	//auto	GetMaterial() const -> Material* { return Mat; }
	auto	GetMeshType() const -> MESH_TYPE const { return MType; }
	
	virtual auto	GetObjectType() const -> ObjectType const { return MESH_COMPONENT; }
	virtual auto	GetObjectTypeName() const -> std::string const override { return "Mesh Component"; }

	auto	operator = (const MeshComponent&)->MeshComponent& = delete;
	auto	operator = (MeshComponent&&)->MeshComponent& = delete;

protected:

private:
	MESH_TYPE		MType = NONE;
	unsigned int	mat;
	unsigned int	meshID = 0;

#ifdef _DEBUG

public:
	virtual auto	ImGuiUpdate() -> void override;
	virtual auto	GetComponentTypeName() -> char* { return "Mesh Component"; }

#endif // _DEBUG

};


#endif /*__MESHCOMPONENT_HPP__*/