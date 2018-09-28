#include "Engine.hpp"

#include "Scene.hpp"
#include "Node.hpp"
#include "Service.hpp"
#include "GraphicalLibrary.hpp"
#include "ResourcesManager.hpp"
#include "InputManager.hpp"

#include "MeshComponent.hpp"
#include <imgui.h>

#include "imgui_impl_dx11.h"
#include "Timer.h"

#include "ObjLoader.hpp"
#include "D3D11Context.hpp"
#include <algorithm>
#include "Logger.hpp"
#include "RendererPipeline.hpp"
#include "Window.hpp"
#include "Gamepad.hpp"
#include "ObjectManager.hpp"
#include "MeshComponent.hpp"
#include "Renderer2DComponent.hpp"

#include "FBXLoader.hpp"

Engine* Engine::instance = nullptr;

auto	Engine::GetInstance() -> Engine*
{
	if (!instance)
		instance = new Engine();
	return instance;
}

auto	Engine::Initialize(Window* window) -> void
{
	ObjectManager*	objMgr = new ObjectManager();
	objMgr->Initialize();
	AddService("Object Manager", (Service*)objMgr);

	ResourcesManager*	rManager = new ResourcesManager();
	rManager->Initialize();
	rManager->SetShutdownOrderIndex(1);
	AddService("Resources Manager", (Service*)rManager);

	Logger* logger = new Logger();
	AddService("Logger", (Service*)logger);

	GraphicalLibrary* renderer = new GraphicalLibrary();
	renderer->Initialize(window);
	AddService("Renderer", (Service*)renderer);
		
	InputManager* inputManager = new InputManager();
	AddService("Input Manager", (Service*)inputManager);
#ifndef UWP
	Window::eventCallback = [&value = (*inputManager)](HWND hwnd, UINT uint, WPARAM wparam, LPARAM lparam) { return value.MessageHandler(hwnd, uint, wparam, lparam);};
#endif

	currentScene = new Scene();

	ObjLoader::LoadObjFromFile("Sherlock.objfile");
	ObjLoader::LoadObjFromFile("23FEF89A_FFFF_00000E11.objfile");

	FBXLoader::LoadFBXFromFile("sherlock.fbx");

	engineClock.SetFramerateLimit(60);
	initialized = true;

#ifdef _DEBUG
	for (unsigned int pos = 0; pos < 49; pos++)
	{
		framerates[pos] = 0.0f;
		frametimes[pos] = 0.0f;
	}
#endif
}

auto	Engine::Shutdown() -> void
{
	ImGui_ImplDX11_Shutdown();
	std::vector<Service*>	tempServices;
	for (auto&& serv : services)
		tempServices.push_back(serv.second);
	services.clear();
	std::sort(tempServices.begin(), tempServices.end(),
		[](Service* lhs, Service* rhs) { return lhs->GetShutdownOrderIndex() > rhs->GetShutdownOrderIndex(); });

	for (auto&& serv : tempServices)
	{
		serv->Shutdown();
		delete serv;
	}

	if (currentScene)
	{
		currentScene->Shutdown();
		delete currentScene;
	}
	delete instance;
}

auto	Engine::Update() -> bool
{
	if (!engineClock.CanUpdate())
		return false;

	InputManager* mgr = GetService<InputManager>("Input Manager");
	mgr->Update();

#ifdef _DEBUG
	if (!xboxOneDebug)
		return true;
	Gamepad* gamepad = mgr->GetGamepad(0);
	if (gamepad)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[0] = gamepad->GetCurrentState().aButton;

		imGuiCursor.x += gamepad->GetCurrentState().leftStickXAxis * 2.0f;
		imGuiCursor.y -= gamepad->GetCurrentState().leftStickYAxis * 2.0f;

		if (imGuiCursor.x < 0.0f)
			imGuiCursor.x = 0.0f;
		else if (imGuiCursor.x > 1920.0f)
			imGuiCursor.x = 1920.0f;
		if (imGuiCursor.y < 0.0f)
			imGuiCursor = 0.0f;
		else if (imGuiCursor.y > 1080.0f)
			imGuiCursor = 1080.0f;
	}

#endif // XBOXONE
	return true;
}

#include "Debug.hpp"

auto	Engine::Draw() -> void
{
	auto* rend = GetService<GraphicalLibrary>("Renderer");
	rend->GetRenderPipeline()->BeginRender();

	ObjectManager* objMgr = GetService<ObjectManager>("Object Manager");
	std::vector<Object*> objects = objMgr->GetObjectsOfType(ObjectType::MESH_COMPONENT);
	for (auto& obj : objects)
		((MeshComponent*)obj)->Draw();
	std::vector<Object*> objects2D = objMgr->GetObjectsOfType(ObjectType::RENDERER_2D_COMPONENT);
	for (auto& obj : objects2D)
		((Renderer2DComponent*)obj)->Draw();

#ifdef _DEBUG
	DrawImGui();
#endif
	rend->GetRenderPipeline()->EndRender();

#ifdef _DEBUG
	if (xboxOneDebug)
	{
		rend->GetRenderPipeline()->DrawFilledRectangle(imGuiCursor - Vector2F(3.0f, 3.0f), Vector2F(6.0f, 6.0f), Vector4F(0.0f, 0.0f, 0.0f, 1.0f));
		rend->GetRenderPipeline()->DrawFilledRectangle(imGuiCursor - Vector2F(2.5f, 2.5f), Vector2F(5.0f, 5.0f), Vector4F(1.0f, 1.0f, 1.0f, 1.0f));
	}
#endif

	rend->GetRenderPipeline()->Present();
}

auto	Engine::AddService(std::string const& name, Service* service) -> void
{
	services.insert(std::make_pair(name, service));
#ifdef _DEBUG
	debugServicesWindows.insert(std::make_pair(name, false));
#endif // _DEBUG
}

auto	Engine::RemoveServiceEntry(std::string const& value) -> void
{
	services.erase(services.find(value));
#ifdef _DEBUG
	debugServicesWindows.erase(debugServicesWindows.find(value));
#endif // _DEBUG
}

auto	Engine::RemoveService(std::string const& value) -> void
{
	auto it = services.find(value);
	it->second->Shutdown();
	delete it->second;
#ifdef _DEBUG
	debugServicesWindows.erase(debugServicesWindows.find(value));
#endif // _DEBUG
}

#ifdef _DEBUG

auto	Engine::DrawImGui() -> void
{
	CTimer	timer;
	timer.Start();
	ImGui_ImplDX11_NewFrame();
	ImGuiIO& io = ImGui::GetIO();

	if (xboxOneDebug)
	{
		io.MousePos.x = imGuiCursor.x * 2.0f;
		io.MousePos.y = imGuiCursor.y * 2.0f;
	}

	for (unsigned int pos = 0; pos < 49; pos++)
	{
		framerates[pos] = framerates[pos + 1];
		frametimes[pos] = frametimes[pos + 1];
	}
	framerates[49] = io.Framerate;
	frametimes[49] = engineClock.GetDeltaTime() * 1000.0f;	

	ImGui::Begin("Margoulin Engine", &windowOpened, ImGuiWindowFlags_MenuBar);
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Services"))
		{
			for (auto& service : services)
			{
				if (ImGui::MenuItem(service.first.c_str()))
				{
					auto it = debugServicesWindows.find(service.first);
					if (it != debugServicesWindows.end())
						(*it).second = !(*it).second;
				}
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Other Menu"))
		{
			if (ImGui::MenuItem("Current Scene"))
				debugSceneWindow = !debugSceneWindow;
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", engineClock.GetDeltaTime() * 1000.0f, ImGui::GetIO().Framerate);
	ImGui::Text("ImGui Render Duration %.5f ms", lastImGuiRenderDuration);
	ImGui::PlotLines("Framerate", framerates, 50, 0, "", 0.0f, 90.0f, ImVec2(0.0f, 80.0f));// , values_offset, "avg 0.0", -1.0f, 1.0f, ImVec2(0, 80));
	ImGui::PlotLines("FrameTimes", frametimes, 50, 0, "", engineClock.GetMinFrametimeRange() * 1000.0f, engineClock.GetMaxFrametimeRange() * 1000.0f, ImVec2(0.0f, 80.0f));// , values_offset, "avg 0.0", -1.0f, 1.0f, ImVec2(0, 80));
	ImGui::End();
	
	for (auto& service : debugServicesWindows)
	{
		if (service.second)
		{
			auto it = services.find(service.first);
			if (it != services.end())
			{
				ImGui::Begin(service.first.c_str());
				(*it).second->ImGuiUpdate();
				ImGui::End();
			}
		}
	}

	if (debugSceneWindow && currentScene)
	{
		ImGui::Begin("Scene");
		currentScene->ImGuiUpdate();
		ImGui::End();
	}


	ImGui::Render();
	timer.Stop();
	lastImGuiRenderDuration = timer.GetDuration();
}

#endif // _DEBUG