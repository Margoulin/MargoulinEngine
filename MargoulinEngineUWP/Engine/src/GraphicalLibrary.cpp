#include "GraphicalLibrary.hpp"

#include "MemoryMacro.hpp"

#include "D3D11Context.hpp"
#include "Window.hpp"
#include "D3D11Renderer.hpp"
#include "imgui_impl_dx11.h"
#include "Shader.hpp"
#include "ResourcesManager.hpp"
#include "Material.hpp"
#include "Engine.hpp"
#include "MaterialResource.hpp"

#include "D3D11PixelShader.hpp"
#include "D3D11VertexShader.hpp"

auto	GraphicalLibrary::Initialize(Window* window) -> void
{
	this->window = window;
	
	D3D11Context* d11Context = NEW D3D11Context();
	context = d11Context;
	context->Initialize(window);

	D3D11Renderer*	pipe = NEW D3D11Renderer();
	pipeline = (RendererPipeline*)pipe;
	pipe->SetContext(d11Context);
	pipe->Initialize();

#ifdef UWP
	ImGui_ImplDX11_Init(d11Context->GetDevice(), d11Context->GetDeviceContext());
#else
	ImGui_ImplDX11_Init(window->GetWindow(), d11Context->GetDevice(), d11Context->GetDeviceContext());
#endif


	D3D11VertexShader* staticMeshVertexShader = NEW D3D11VertexShader();
	staticMeshVertexShader->InitializeDefaultVertexShader(d11Context->GetDevice());
	shaders.push_back((Shader*)staticMeshVertexShader);

	D3D11PixelShader* unlitColorShader = NEW D3D11PixelShader();
	unlitColorShader->InitializeUnlitColorPixelShader(d11Context->GetDevice(), (D3D11VertexShader*)shaders[0]);
	shaders.push_back((Shader*)unlitColorShader);

	ResourcesManager* rsMgr = Engine::GetInstance()->GetService<ResourcesManager>("Resources Manager");
	unsigned int matId = rsMgr->CreateMaterialResource();
	MaterialResource* matRes = (MaterialResource*)rsMgr->GetResource(matId);
	matRes->SetName("BasicMaterial");
	Material* newMat = NEW Material();
	newMat->attachedShader = unlitColorShader;
	newMat->Initialize(d11Context->GetDevice());
	matRes->SetMaterialData(newMat);

#ifdef _DEBUG
	d11Context->MarkD3D11ObjectName(staticMeshVertexShader->GetShader(), MString("Default Vertex Shader"));
	d11Context->MarkD3D11ObjectName(unlitColorShader->GetShader(), MString("Unlit Color Shader"));
	d11Context->MarkD3D11ObjectName(newMat->GetConstantBuffer(), MString("Default Vertex Shader"));
#endif
}

auto	GraphicalLibrary::Shutdown() -> void
{
	for (auto& shader : shaders)
	{
		if (shader->GetShaderType() == Shader::ShaderType::VERTEX)
			((D3D11VertexShader*)shader)->Shutdown();
		else if (shader->GetShaderType() == Shader::ShaderType::FRAGMENT)
			((D3D11PixelShader*)shader)->Shutdown();
		DEL(shader);
	}
	D3D11Renderer* pipe = (D3D11Renderer*)pipeline;
	pipe->Shutdown();
	DEL(pipeline);
	context->Shutdown();
	DEL(context);
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