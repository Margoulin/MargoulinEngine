#ifndef __SKELETAL_MESH_COMPONENT_HPP__
#define __SKELETAL_MESH_COMPONENT_HPP__

#include "GraphicComponent.hpp"

class SkeletalMeshComponent : public GraphicComponent
{
public:
	SkeletalMeshComponent() = default;
	SkeletalMeshComponent(const SkeletalMeshComponent&) = delete;
	SkeletalMeshComponent(SkeletalMeshComponent&&) = delete;
	~SkeletalMeshComponent() = default;
	
	virtual auto	Draw() -> void;

	auto	SetMesh(unsigned int const& value) -> void { meshID = value; }

	virtual auto	GetObjectType() const -> ObjectType const { return SKELETAL_MESH_COMPONENT; }
	virtual auto	GetObjectTypeName() const -> MString const override { return "Skeletal Mesh Component"; }

	auto	operator = (const SkeletalMeshComponent&)->SkeletalMeshComponent& = delete;
	auto	operator = (SkeletalMeshComponent&&)->SkeletalMeshComponent& = delete;

protected:

private:
	unsigned int meshID = 0;

#ifdef _DEBUG

public:
	virtual auto	GetComponentTypeName() -> char* { return "Skeletal Mesh Component"; }

#endif // _DEBUG

};


#endif /*__SKELETAL_MESH_COMPONENT_HPP__*/