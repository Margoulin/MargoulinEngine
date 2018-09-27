#ifndef __MATERIALRESOURCE_HPP__
#define __MATERIALRESOURCE_HPP__

#include "Resource.hpp"

class Material;

class MaterialResource : public Resource
{
public:
	MaterialResource() { type = Resource::RESOURCE_MATERIAL; }
	MaterialResource(const MaterialResource&) = delete;
	MaterialResource(MaterialResource&&) = delete;
	~MaterialResource() = default;

	virtual auto	Load() -> void {}
	virtual auto	Unload() -> void {}
	virtual auto	Shutdown() -> void;

	auto	SetMaterialData(Material* value) -> void { materialData = value; }

	auto	GetMaterialData() const -> Material* { return materialData; }

	auto	operator = (const MaterialResource&)->MaterialResource& = delete;
	auto	operator = (MaterialResource&&)->MaterialResource& = delete;

protected:

private:
	Material*	materialData = nullptr;

#ifdef _DEBUG
	virtual auto	ImGuiUpdate() -> void;
#endif // _DEBUG

};


#endif /*__MATERIALRESOURCE_HPP__*/