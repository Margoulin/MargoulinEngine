#ifndef __RESOURCESMANAGER_HPP__
#define __RESOURCESMANAGER_HPP__

#include "MemoryMacro.hpp"
#include "Service.hpp"

#include "Resource.hpp"
#include "Material.hpp"
#include "MeshResource.hpp"
#include <map>

class MeshResource;

class ResourcesManager : public Service
{
public:
	ResourcesManager() = default;
	ResourcesManager(const ResourcesManager&) = delete;
	ResourcesManager(ResourcesManager&&) = delete;
	~ResourcesManager() = default;

	virtual auto	Initialize() -> void;
	virtual auto	Shutdown() -> void;
	virtual auto	Update() -> void {}

	template<typename Rs>
	auto	CreateResource() -> unsigned int
	{
		Rs* newResource = NEW Rs();
		return addResource((Resource*)newResource);
	}

	auto	GetDefaultMeshResource(unsigned int const& id) const -> MeshResource* { return &defaultMeshes[id]; }
	auto	GetResource(unsigned int const& resourceID) const -> Resource*;
	template<typename Rs>
	auto	GetResource(unsigned int const& resourceID) const -> Rs* 
	{
		auto it = resources.find(resourceID);
		if (it != resources.end())
			return (Rs*)(*it).second;
		return nullptr;
	}

#ifdef _DEBUG
	virtual auto	ImGuiUpdate() -> void;

	char objFilepath[128] = "";
#endif // _DEBUG

	auto	operator = (const ResourcesManager&)->ResourcesManager& = delete;
	auto	operator = (ResourcesManager&&)->ResourcesManager& = delete;

protected:

private:
	auto	addResource(Resource* res) -> unsigned int;

	MeshResource*						defaultMeshes;
	Material*							defaultMaterial[1];
	std::map<unsigned int, Resource*>	resources;
	unsigned int			totID = 0;
};


#endif /*__RESOURCESMANAGER_HPP__*/