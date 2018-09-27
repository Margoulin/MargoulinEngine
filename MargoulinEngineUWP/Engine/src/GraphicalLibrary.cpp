#include "GraphicalLibrary.hpp"

#include "D3D11Context.hpp"
#include "Window.hpp"
#include "D3D11Renderer.hpp"
#include "imgui_impl_dx11.h"
#include "Shader.hpp"
#include "ResourcesManager.hpp"
#include "Material.hpp"
#include "Engine.hpp"
#include "MaterialResource.hpp"

auto	GraphicalLibrary::Initialize(Window* window) -> void
{
	this->window = window;
	
	D3D11Context* d11Context = new D3D11Context();
	context = d11Context;
	context->Initialize(window);

	D3D11Renderer*	pipe = new D3D11Renderer();
	pipeline = (RendererPipeline*)pipe;
	pipe->SetContext(d11Context);
	pipe->Initialize();

#ifdef UWP
	ImGui_ImplDX11_Init(d11Context->GetDevice(), d11Context->GetDeviceContext());
#else
	ImGui_ImplDX11_Init(window->GetWindow(), d11Context->GetDevice(), d11Context->GetDeviceContext());
#endif


	Shader* unlitColorShader = new Shader();
	unlitColorShader->Initialize(d11Context->GetDevice());
	shaders.push_back(unlitColorShader);

	ResourcesManager* rsMgr = Engine::GetInstance()->GetService<ResourcesManager>("Resources Manager");
	unsigned int matId = rsMgr->CreateMaterialResource();
	MaterialResource* matRes = (MaterialResource*)rsMgr->GetResource(matId);
	matRes->SetName("BasicMaterial");
	Material* newMat = new Material();
	newMat->attachedShader = unlitColorShader;
	matRes->SetMaterialData(newMat);
}

auto	GraphicalLibrary::Shutdown() -> void
{
	for (auto& shader : shaders)
	{
		shader->Shutdown();
		delete shader;
	}
	context->Shutdown();
	window->Shutdown();
}

#ifdef UWP
auto	GraphicalLibrary::UpdateWindowHandle(IUnknown* value, Vector2F const& size, float dpi) -> void
{
	window->UpdateHandle(value);
	window->SetSize(size);
	window->SetDPI(dpi);
	context->SetWindow();
}
#endif

#include <imgui.h>

#ifdef _DEBUG
auto	GraphicalLibrary::ImGuiUpdate() -> void
{
	if (ImGui::TreeNode("Window"))
	{
		window->ImGuiUpdate();
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Renderer pipeline"))
	{
		pipeline->ImGuiUpdate();
		ImGui::TreePop();
	}
	ImGui::Value("Shaders count : ", (unsigned int)shaders.size());
	if (ImGui::TreeNode("Shaders"))
	{
		for (auto& shader : shaders)
			shader->ImGuiUpdate();
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Context"))
	{
		context->ImGuiUpdate();
		ImGui::TreePop();
	}
}
#endif