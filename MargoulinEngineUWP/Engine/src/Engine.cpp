#include "Engine.hpp"

#include "Scene.hpp"
#include "Service.hpp"
#include "ServiceApplication.hpp"
#include "GraphicalLibrary.hpp"
#include "ResourcesManager.hpp"
#include "InputManager.hpp"

#include "MaterialResource.hpp"
#include "MeshComponent.hpp"
#include <imgui.h>

#include "imgui_impl_dx11.h"
#include "Timer.hpp"

#include <algorithm>
#include "Logger.hpp"
#include "RendererPipeline.hpp"
#include "Window.hpp"
#include "Gamepad.hpp"
#include "ObjectManager.hpp"
#include "Renderer2DComponent.hpp"
#include "CameraComponent.hpp"
#include "Profiler.hpp"
#include "Node.hpp"

#include "Maths/Math.hpp"

Engine* Engine::instance = nullptr;

auto	Engine::GetInstance() -> Engine*
{
	if (!instance)
		instance = new Engine();
	return instance;
}

auto	Engine::Initialize(Window* window) -> void
{
	Profiler*	profiler = new Profiler();
	profiler->Initialize();
	AddService("Profiler", profiler);

	ObjectManager*	objMgr = NEW ObjectManager();
	objMgr->Initialize();
	objMgr->SetUpdateOrderIndex(1);
	AddService("Object Manager", (Service*)objMgr);

	ResourcesManager*	rManager = NEW ResourcesManager();
	rManager->Initialize();
	rManager->SetShutdownOrderIndex(2);
	AddService("Resources Manager", (Service*)rManager);

	Logger* logger = NEW Logger();
	AddService("Logger", (Service*)logger);

	GraphicalLibrary* renderer = NEW GraphicalLibrary();
	renderer->Initialize(window);
	AddService("Renderer", (Service*)renderer);
		
	InputManager* inputManager = NEW InputManager();
	inputManager->SetUpdateOrderIndex(2);
	AddService("Input Manager", (Service*)inputManager);

#ifndef UWP
	Window::eventCallback = [&value = (*inputManager)](HWND hwnd, UINT uint, WPARAM wparam, LPARAM lparam) { return value.MessageHandler(hwnd, uint, wparam, lparam);};
#endif

	currentScene = NEW Scene();	

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
	if (currentScene)
	{
		currentScene->Shutdown();
		DEL(currentScene);
	}

	std::vector<Service*>	tempServices;
	for (auto&& serv : services)
		tempServices.push_back(serv.second);
	services.clear();
	std::sort(tempServices.begin(), tempServices.end(),
		[](Service* lhs, Service* rhs) { return lhs->GetShutdownOrderIndex() > rhs->GetShutdownOrderIndex(); });

	for (auto&& serv : tempServices)
	{
		serv->Shutdown();
		DEL(serv);
	}

	delete instance;
}

auto	Engine::Update() -> bool
{
	if (!engineClock.CanUpdate())
		return false;

	std::vector<Service*>	tempServices;
	for (auto&& serv : services)
		tempServices.push_back(serv.second);
	std::sort(tempServices.begin(), tempServices.end(),
		[](Service* lhs, Service* rhs) { return lhs->GetUpdateOrderIndex() > rhs->GetUpdateOrderIndex(); });

	for (auto& service : tempServices)
		service->Update();

	return true;
}

auto	Engine::Draw() -> void
{
	auto* rend = GetService<GraphicalLibrary>("Renderer");
	rend->GetRenderPipeline()->BeginRender();

	ResourcesManager* resMgr = GetService<ResourcesManager>("Resources Manager");
	ObjectManager* objMgr = GetService<ObjectManager>("Object Manager");

	std::vector<Object*> cameras = objMgr->GetObjectsOfType(ObjectType::CAMERA_COMPONENT);
	
	for (auto& camObj : cameras)
	{
		CameraComponent* camComp = (CameraComponent*)camObj;
		rend->GetRenderPipeline()->BindCamera(camComp->GetProjectionMatrix(rend->GetWindow()->GetSize()), camComp->GetViewMatrix());

		std::vector<Object*> objects = objMgr->GetObjectsOfType(ObjectType::MESH_COMPONENT);
		for (auto& obj : objects)
		{
			MeshComponent* meshComp = (MeshComponent*)obj;
			MeshResource* meshResource = nullptr;
			MaterialResource* matResource = nullptr;
			if (meshComp->GetMeshType() == MeshComponent::MESH_TYPE::CUSTOM)
				meshResource = (MeshResource*)resMgr->GetResource(meshComp->GetMesh());
			else if (meshComp->GetMeshType() == MeshComponent::MESH_TYPE::CUBE)
				meshResource = (MeshResource*)resMgr->GetDefaultMeshResource(0);
			matResource = (MaterialResource*)resMgr->GetResource(meshComp->GetMaterial());
			if (matResource && meshResource)
				rend->DrawMesh(meshComp->GetNode()->GetTransform()->GetGlobalMatrix(), meshResource, matResource);
		}
		std::vector<Object*> objects2D = objMgr->GetObjectsOfType(ObjectType::RENDERER_2D_COMPONENT);
		for (auto& obj : objects2D)
			((Renderer2DComponent*)obj)->Draw();
	}

	std::vector<ServiceApplication*>	apps = GetApplicationServices();
	for (auto& serv : apps)
		serv->Draw();

#ifdef _DEBUG

	if (cameras.empty())
	{
		Quaternion	quatRot = Quaternion::Euler(editorCameraRotation * degToRad);
		Vector3F up = quatRot * Vector3F::up;
		Vector3F forward = quatRot * Vector3F::back;
		Vector2F windSize = rend->GetWindow()->GetSize();

		Matrix4x4F view =  Matrix4x4F::LookAt(editorCameraPosition, up, editorCameraPosition + forward);
		Matrix4x4F proj = Matrix4x4F::Perspective(90.0f, windSize.x / windSize.y, 0.01f, 100.0f);
		rend->GetRenderPipeline()->BindCamera(proj, view);
		std::vector<Object*> objects = objMgr->GetObjectsOfType(ObjectType::MESH_COMPONENT);
		for (auto& obj : objects)
		{
			MeshComponent* meshComp = (MeshComponent*)obj;
			MeshResource* meshResource = nullptr;
			MaterialResource* matResource = nullptr;
			if (meshComp->GetMeshType() == MeshComponent::MESH_TYPE::CUSTOM)
				meshResource = (MeshResource*)resMgr->GetResource(meshComp->GetMesh());
			else if (meshComp->GetMeshType() == MeshComponent::MESH_TYPE::CUBE)
				meshResource = (MeshResource*)resMgr->GetDefaultMeshResource(0);
			matResource = (MaterialResource*)resMgr->GetResource(meshComp->GetMaterial());
			if (matResource && meshResource)
				rend->DrawMesh(meshComp->GetNode()->GetTransform()->GetGlobalMatrix(), meshResource, matResource);
		}
		std::vector<Object*> objects2D = objMgr->GetObjectsOfType(ObjectType::RENDERER_2D_COMPONENT);
		for (auto& obj : objects2D)
			((Renderer2DComponent*)obj)->Draw();
	}

	DrawImGui();
#endif
	rend->GetRenderPipeline()->EndRender();

#if defined(_DEBUG) && defined(_XBOX_ONE)
		Vector2F imGuiCursor = GetService<InputManager>("Input Manager")->imGuiCursor;
		rend->GetRenderPipeline()->DrawFilledRectangle(imGuiCursor - Vector2F(3.0f, 3.0f), Vector2F(6.0f, 6.0f), Vector4F(0.0f, 0.0f, 0.0f, 1.0f));
		rend->GetRenderPipeline()->DrawFilledRectangle(imGuiCursor - Vector2F(2.5f, 2.5f), Vector2F(5.0f, 5.0f), Vector4F(1.0f, 1.0f, 1.0f, 1.0f));
#endif

	rend->EndOfFrame();

	rend->GetRenderPipeline()->Present();
}

auto	Engine::AddService(MString const& name, Service* service) -> void
{
	services.insert(std::make_pair(name, service));
#ifdef _DEBUG
	debugServicesWindows.insert(std::make_pair(name, false));
#endif // _DEBUG
}
	
auto	Engine::GetApplicationServices() const -> std::vector<ServiceApplication*>
{
	std::vector<ServiceApplication*> ret;
	for (auto& serv : services)
	{
		ServiceApplication* app = dynamic_cast<ServiceApplication*>(serv.second);
		if (app)
			ret.push_back(app);
	}
	return ret;
}

auto	Engine::RemoveServiceEntry(MString const& value) -> void
{
	services.erase(services.find(value));
#ifdef _DEBUG
	debugServicesWindows.erase(debugServicesWindows.find(value));
#endif // _DEBUG
}

auto	Engine::RemoveService(MString const& value) -> void
{
	auto it = services.find(value);
	it->second->Shutdown();
	DEL(it->second);
#ifdef _DEBUG
	debugServicesWindows.erase(debugServicesWindows.find(value));
#endif // _DEBUG
}

#ifdef _DEBUG

auto	Engine::DrawImGui() -> void
{
	Timer	timer;
	timer.Start();
	ImGui_ImplDX11_NewFrame();
	ImGuiIO& io = ImGui::GetIO();

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
				if (ImGui::MenuItem(service.first.Str()))
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
	
	ImGui::DragFloat3("Editor Camera Position", &editorCameraPosition.x);
	ImGui::DragFloat3("Editor Camera Rotation", (float*)(&editorCameraRotation.x), 0.5f, -360.0f, 360.0f);

	ImGui::End();
	
	for (auto& service : debugServicesWindows)
	{
		if (service.second)
		{
			auto it = services.find(service.first);
			if (it != services.end())
			{
				ImGui::Begin(service.first.Str());
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