#ifndef __ENGINE_HPP__
#define __ENGINE_HPP__

#include "CoreMinimal.hpp"
#include <map>
#include <vector>

#include "Clock.hpp"

class Scene;
class Service;
class ServiceApplication;
class Window;

class Engine
{
public:
	Engine() = default;
	Engine(const Engine&) = delete;
	Engine(Engine&&) = delete;
	~Engine() = default;

	static auto	GetInstance() -> Engine*;

	auto	Initialize(Window* context) -> void;
	auto	Shutdown() -> void;

	auto	Update() -> bool;
	auto	Draw() -> void;

	auto	AddService(MString const& name, Service* service) -> void;
	template<typename T>
	auto	GetService(MString const& name) const -> T*
	{
		auto value = services.find(name);
		if (value != services.end())
			return (T*)value->second;
		return nullptr;
	}

	auto	GetRawService(MString const& name) const -> Service* 
	{
		auto value = services.find(name.Str());
		if (value != services.end())
			return value->second;
		return nullptr;
	}

	auto	GetApplicationServices() const-> std::vector<ServiceApplication*>;

	auto	RemoveServiceEntry(MString const& value) -> void;
	auto	RemoveService(MString const& value) -> void;

	auto	IsInitialized() const -> bool const { return initialized; }
	auto	GetScene() const -> Scene* { return currentScene; }

	auto	operator = (const Engine&)->Engine& = delete;
	auto	operator = (Engine&&)->Engine& = delete;

private:
	static Engine*					instance;
	
	Clock							engineClock;
	std::map<MString, Service*>	services;
	Scene*							currentScene;
	Vector3F						editorCameraPosition;
	Vector3F						editorCameraRotation;
	bool							initialized = false;

#ifdef _DEBUG

	auto						DrawImGui() -> void;
	float						lastImGuiRenderDuration = 0.0f;
	bool						windowOpened = true;
	bool						debugSceneWindow = true;
	float						framerates[100];
	float						frametimes[100];
	std::map<MString, bool>	debugServicesWindows;

#endif // _DEBUG
};


#endif /*__ENGINE_HPP__*/