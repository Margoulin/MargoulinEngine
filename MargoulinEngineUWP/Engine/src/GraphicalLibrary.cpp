#include "GraphicalLibrary.hpp"

#include "MemoryMacro.hpp"

#include "D3D11Context.hpp"
#include "Window.hpp"
#include "D3D11Renderer.hpp"
#include "D3D11ShaderFactory.hpp"
#include "D3D11BufferFactory.hpp"
#include "imgui_impl_dx11.h"
#include "Shader.hpp"
#include "ResourcesManager.hpp"
#include "Material.hpp"
#include "Engine.hpp"
#include "MaterialResource.hpp"

#include "D3D11PixelShader.hpp"
#include "D3D11VertexShader.hpp"
#include "D3D11Buffer.hpp"
#include "Mesh.hpp"

auto	GraphicalLibrary::Initialize(Window* window) -> void
{
	this->window = window;
	
	D3D11Context* d11Context = NEW D3D11Context();
	context = d11Context;
	context->Initialize(window);

	D3D11ShaderFactory* shadFactory = NEW D3D11ShaderFactory();
	shaderFactory = (ShaderFactory*)shadFactory;
	shadFactory->SetContext(context);

	D3D11Renderer*	pipe = NEW D3D11Renderer();
	pipeline = (RendererPipeline*)pipe;
	pipe->SetContext(d11Context);
	pipe->Initialize();

	D3D11BufferFactory* d3d11BufferFactory = NEW D3D11BufferFactory();
	bufferFactory = d3d11BufferFactory;
	d3d11BufferFactory->SetContext(context);

	D3D11Buffer* modelBuffer = (D3D11Buffer*)d3d11BufferFactory->GenerateBuffer(sizeof(Matrix4x4F));
	modelBuffer->SetBufferBindIndex(1);
	pipeline->SetModelBuffer(modelBuffer);
	
	D3D11Buffer* viewProjBuffer = (D3D11Buffer*)d3d11BufferFactory->GenerateBuffer(sizeof(ViewProjectionConstantBuffer));
	pipeline->SetViewProjBuffer(viewProjBuffer);

#ifdef UWP
	ImGui_ImplDX11_Init(d11Context->GetDevice(), d11Context->GetDeviceContext());
#else
	ImGui_ImplDX11_Init(window->GetWindow(), d11Context->GetDevice(), d11Context->GetDeviceContext());
#endif


	shaders.push_back(shaderFactory->CreateBasicVertexShader());
	Shader* unlitColorShader = shaderFactory->CreateUnlitColorShader();
	shaders.push_back(unlitColorShader);

	ResourcesManager* rsMgr = Engine::GetInstance()->GetService<ResourcesManager>("Resources Manager");
	unsigned int matId = rsMgr->CreateMaterialResource();
	MaterialResource* matRes = (MaterialResource*)rsMgr->GetResource(matId);
	matRes->SetName("BasicMaterial");
	Material* newMat = NEW Material();
	newMat->attachedShader = (FragmentShader*)unlitColorShader;
	newMat->Initialize(d11Context->GetDevice());
	matRes->SetMaterialData(newMat);

	shaders.push_back(shaderFactory->CreateVertexTextureShader());
	shaders.push_back(shaderFactory->CreatePixelTextureShader());

#ifdef _DEBUG
	d11Context->MarkD3D11ObjectName(newMat->GetConstantBuffer(), MString("Default Vertex Shader"));
#endif
}

auto	GraphicalLibrary::Shutdown() -> void
{
	ImGui_ImplDX11_Shutdown();

	for (auto& shader : shaders)
		shaderFactory->DeleteShader(shader);
	D3D11Renderer* pipe = (D3D11Renderer*)pipeline;
	pipe->Shutdown();
	DEL(pipeline);
	context->Shutdown();
	DEL(shaderFactory);
	DEL(bufferFactory);
	DEL(context);
	window->Shutdown();
}

auto	GraphicalLibrary::DrawMesh(Matrix4x4F const& modelMat, MeshResource* meshRes, MaterialResource* matRes) -> void
{
	shaderFactory->BindShader(shaders[0]);
	shaderFactory->BindShader(matRes->GetMaterialData()->attachedShader);

	SubMeshData** subMeshes = meshRes->GetMeshData()->GetSubMeshTab();
	for (unsigned int pos = 0; pos < meshRes->GetMeshData()->GetSubMeshNbr(); pos++)
	{
		if (!subMeshes[pos]->IsInVRAM())
		{
			subMeshes[pos]->SetIndexBuffer(bufferFactory->GenerateIndexBuffer(subMeshes[pos]));
			subMeshes[pos]->SetVertexBuffer(bufferFactory->GenerateVertexBuffer(subMeshes[pos]));
			subMeshes[pos]->SetInVRAM(true);
		}
	}

	pipeline->drawData(meshRes->GetMeshData(), matRes->GetMaterialData(), modelMat);
}
	
auto	GraphicalLibrary::DrawTexture(Vector4F const& screenRect, TextureRenderData const& renderData) -> void
{
	shaderFactory->BindShader(shaders[2]);
	shaderFactory->BindShader(shaders[3]);
	
	Vector2F halfScreen = window->GetSize() * 0.5f;
	float left = -halfScreen.x + screenRect.x;
	float right = left + screenRect.w;
	float top = halfScreen.y - screenRect.y;
	float bottom = top - screenRect.z;

	ResourcesManager* rsMgr = Engine::GetInstance()->GetService<ResourcesManager>("Resources Manager");
	SubMeshData* textureMesh = rsMgr->GetDefaultMeshResource(1)->GetMeshData()->GetSubMeshTab()[0];
	textureMesh->SetVertice(0, Vector3F(left, top, 0.0f));
	textureMesh->SetVertice(1, Vector3F(right, top, 0.0f));
	textureMesh->SetVertice(2, Vector3F(right, bottom, 0.0f));
	textureMesh->SetVertice(3, Vector3F(left, bottom, 0.0f));
	textureMesh->SetUV(0, Vector2F(renderData.textureRect.x, renderData.textureRect.y));
	textureMesh->SetUV(1, Vector2F(renderData.textureRect.w, renderData.textureRect.y));
	textureMesh->SetUV(2, Vector2F(renderData.textureRect.w, renderData.textureRect.z));
	textureMesh->SetUV(3, Vector2F(renderData.textureRect.x, renderData.textureRect.z));
	if (!textureMesh->IsInVRAM())
	{
		textureMesh->SetIndexBuffer(bufferFactory->GenerateIndexBuffer(textureMesh));
		textureMesh->SetVertexBuffer(bufferFactory->GenerateVertexBuffer(textureMesh, true));
		((D3D11Buffer*)(textureMesh->GetVertexBuffer()))->SetStride(5 * sizeof(float));
		textureMesh->SetInVRAM(true);
	}

	pipeline->drawTexture(screenRect, textureMesh, renderData);
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