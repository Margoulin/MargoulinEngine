#ifndef __RESOURCE_HPP__
#define __RESOURCE_HPP__

#include "CoreMinimal.hpp"

class Resource
{
public:
	enum ResourceType
	{
		RESOURCE_EMPTY,
		RESOURCE_MESH,
		RESOURCE_SKELETALMESH,
		RESOURCE_MATERIAL,
		RESOURCE_TEXTURE
	};

	Resource() = default;
	Resource(const Resource&) = delete;
	Resource(Resource&&) = delete;
	~Resource() = default;

	virtual auto	Load() -> void = 0;
	virtual auto	Unload() -> void = 0;
	virtual auto	Shutdown() -> void = 0;

	auto	SetName(MString const& value) -> void { name = value; }

	auto	GetName() const -> MString const& { return name; }
	auto	GetType() const -> ResourceType { return type; }
	auto	GetRawData() const -> void* { return resourceData; }

	auto	operator = (const Resource&)->Resource& = delete;
	auto	operator = (Resource&&)->Resource& = delete;

protected:
	MString		name;
	void*			resourceData;
	ResourceType	type;
	bool			loaded = false;
	bool			unloadable = false;

private:

#ifdef _DEBUG

public:
	virtual auto	ImGuiUpdate() -> void = 0;

#endif // _DEBUG

};


#endif /*__RESOURCE_HPP__*/