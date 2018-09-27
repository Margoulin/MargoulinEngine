#ifndef __ENGINE_HPP__
#define __ENGINE_HPP__

#include <map>

#include "Clock.hpp"
#include "Maths/Vector.hpp"

class Scene;
class Service;
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

	auto	AddService(std::string const& name, Service* service) -> void;
	template<typename T>
	auto	GetService(std::string const& name) const -> T*
	{
		auto value = services.find(name);
		if (value != services.end())
			return (T*)value->second;
		return nullptr;
	}

	auto	RemoveServiceEntry(std::string const& value) -> void;
	auto	RemoveService(std::string const& value) -> void;

	auto	IsInitialized() const -> bool const { return initialized; }
	auto	GetScene() -> Scene* { return currentScene; }

	auto	operator = (const Engine&)->Engine& = delete;
	auto	operator = (Engine&&)->Engine& = delete;

	
protected:

private:
#ifdef _DEBUG

	auto						DrawImGui() -> void;
	float						lastImGuiRenderDuration = 0.0f;
	bool						windowOpened = true;
	bool						debugSceneWindow = false;
	float						framerates[100];
	float						frametimes[100];
	std::map<std::string, bool>	debugServicesWindows;
	Vector2F					imGuiCursor;
public:
	bool						xboxOneDebug = false;

#endif // _DEBUG

	static Engine*					instance;
	Clock							engineClock;
	std::map<std::string, Service*>	services;
	Scene*							currentScene;
	bool							initialized = false;
};


#endif /*__ENGINE_HPP__*/