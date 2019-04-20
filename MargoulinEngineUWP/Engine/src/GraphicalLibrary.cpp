#include "GraphicalLibrary.hpp"

#include "MemoryMacro.hpp"

#include "ShaderFactory.hpp"
#include "Context.hpp"
#include "Window.hpp"
#include "RendererPipeline.hpp"
#include "GPUBufferFactory.hpp"
#include "ShaderFactory.hpp"
#include "FragmentShader.hpp"

#ifndef VITA
#include "Window.hpp"
#include "D3D11Context.hpp"
#include "D3D11Renderer.hpp"
#include "D3D11ShaderFactory.hpp"
#include "D3D11BufferFactory.hpp"
#include "imgui_impl_dx11.h"
#include "D3D11PixelShader.hpp"
#include "D3D11VertexShader.hpp"
#include "D3D11Buffer.hpp"
#else
#include "GXMContext.hpp"
#include "GXMShaderFactory.hpp"
#include "GXMRenderer.hpp"
#include "GXMBufferFactory.hpp"
#include "GXMBuffer.hpp"
#endif

#include "Shader.hpp"
#include "ResourcesManager.hpp"
#include "Material.hpp"
#include "Engine.hpp"
#include "MaterialResource.hpp"

#include "Mesh.hpp"
#include "SkeletalMesh.hpp"
#include "SkeletalMeshResource.hpp"

#include "Logger.hpp"

auto	GraphicalLibrary::Initialize(Window* window) -> void
{
	this->window = window;
	
#ifndef VITA
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

	D3D11Buffer* skelMeshBuffer = (D3D11Buffer*)d3d11BufferFactory->GenerateBuffer(16 + 99 * 16 * sizeof(float));
	skelMeshBuffer->SetBufferBindIndex(2);
	pipeline->SetSkeletalMeshBuffer(skelMeshBuffer);

	//D3D11Buffer* lineBuffer = (D3D11Buffer*)d3d11BufferFactory->GenerateVertexBuffer(99 * (sizeof(Vector3F) + sizeof(Vector4F)), sizeof(Vector3F) + sizeof(Vector4F), pipeline->GetLineVerticesBuffer(), true);
	D3D11Buffer* lineBuffer = (D3D11Buffer*)d3d11BufferFactory->GenerateVertexBuffer(2 * (sizeof(Vector3F) + sizeof(Vector4F)), sizeof(Vector3F) + sizeof(Vector4F), pipeline->GetLineVerticesBuffer(), true);
	pipeline->SetLineBuffer(lineBuffer);

#ifdef UWP
	ImGui_ImplDX11_Init(d11Context->GetDevice(), d11Context->GetDeviceContext());
#else
	ImGui_ImplDX11_Init(window->GetWindow(), d11Context->GetDevice(), d11Context->GetDeviceContext());
#endif

#elif VITA
	GXMContext* gxmContext = NEW GXMContext();
	context = gxmContext;
	context->Initialize(window);

	GXMShaderFactory* shadFactory = NEW GXMShaderFactory();
	shaderFactory = shadFactory;
	shadFactory->SetContext(context);

	GXMRenderer* pipe = NEW GXMRenderer();
	pipeline = pipe;
	pipe->SetContext(gxmContext);

	GXMBufferFactory* buffFact = NEW GXMBufferFactory();
	bufferFactory = buffFact;

	GXMVertexShader* clearVtxShad = (GXMVertexShader*)(shadFactory->CreateClearVertexShader());
	pipe->SetClearVertexShader(clearVtxShad);
	GXMFragmentShader* clearFragShad = (GXMFragmentShader*)(shadFactory->CreateClearFragmentShader());
	pipe->SetClearFragmentShader(clearFragShad);

#endif /*VITA*/

	shaders.push_back(shaderFactory->CreateBasicVertexShader());
	Shader* unlitColorShader = shaderFactory->CreateUnlitColorShader();
	shaders.push_back(unlitColorShader);

	ResourcesManager* rsMgr = Engine::GetInstance()->GetService<ResourcesManager>("Resources Manager");
	unsigned int matId = rsMgr->CreateResource<MaterialResource>();
	MaterialResource* matRes = (MaterialResource*)rsMgr->GetResource(matId);
	matRes->SetName("BasicMaterial");
	Material* newMat = NEW Material();
#ifndef VITA
	D3D11Buffer* matBuffer = (D3D11Buffer*)bufferFactory->GenerateBuffer(sizeof(Vector4F));
	matBuffer->SetBufferBindIndex(3);
	newMat->SetConstantBuffer(matBuffer);
#endif /*VITA*/
	newMat->attachedShader = (FragmentShader*)unlitColorShader;
	matRes->SetMaterialData(newMat);
	newMat->unlitColor = Vector4F(1.0f, 0.0f, 0.0f, 1.0f);

	shaders.push_back(shaderFactory->CreateVertexTextureShader());
	shaders.push_back(shaderFactory->CreatePixelTextureShader());
	shaders.push_back(shaderFactory->CreateVertexSkinningShader());
	shaders.push_back(shaderFactory->CreateLineVertexShader());

#ifdef VITA

	shaders.push_back((Shader*)clearVtxShad);
	shaders.push_back((Shader*)clearFragShad);

#endif

#ifdef _DEBUG
	//d11Context->MarkD3D11ObjectName(newMat->GetConstantBuffer(), MString("Default Vertex Shader"));
#endif
}

auto	GraphicalLibrary::Shutdown() -> void
{
#ifndef VITA
	ImGui_ImplDX11_Shutdown();
#endif

	for (auto& shader : shaders)
		shaderFactory->DeleteShader(shader);
#ifndef VITA
	D3D11Renderer* pipe = (D3D11Renderer*)pipeline;
	pipe->Shutdown();
#endif
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
	if (lastVertexShaderUse && shaders[0] != lastVertexShaderUse)
		pipeline->RebindCamera();
	lastVertexShaderUse = shaders[0];
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

auto	GraphicalLibrary::DrawSkeletalMesh(Matrix4x4F const& modelMat, SkeletalMeshResource* meshRes, MaterialResource* matRes) -> void
{
	shaderFactory->BindShader(shaders[4]);
	if (lastVertexShaderUse && shaders[4] != lastVertexShaderUse)
		pipeline->RebindCamera();
	lastVertexShaderUse = shaders[4];
	shaderFactory->BindShader(matRes->GetMaterialData()->attachedShader);

	if (!meshRes->GetSkeletalMeshConstData().IsInVRAM())
	{
		meshRes->GetSkeletalMeshData().SetIndexBuffer(bufferFactory->GenerateIndexBuffer(&meshRes->GetSkeletalMeshData()));
		meshRes->GetSkeletalMeshData().SetVertexBufferArray(bufferFactory->GenerateVertexBufferArray(2, &meshRes->GetSkeletalMeshData()));
		meshRes->GetSkeletalMeshData().SetInVRAM(true);
	}

	//pipeline->drawData(&meshRes->GetSkeletalMeshData(), matRes->GetMaterialData(), modelMat);
	
	Rig* rig = meshRes->GetSkeletalMeshConstData().GetRig();
	for (int pos = 0; pos < rig->GetBoneCount(); pos++)
	{
		if (rig->GetBone(pos)->ParentIdx != -1)
		{
			Vector3F from = Matrix4x4F::GetPositionFromModelMatrix(rig->GetBone(pos)->TPoseMatrix);
			Vector3F to = Matrix4x4F::GetPositionFromModelMatrix(rig->GetBone(rig->GetBone(pos)->ParentIdx)->TPoseMatrix);
			from = from * 0.01f;
			to = to * 0.01f;
			DrawLine(from, to, Vector3F(1.0f, 0.0f, 0.0f));
		}
	}

}
	
auto	GraphicalLibrary::DrawTexture(Vector4F const& screenRect, TextureRenderDataBase const& renderData) -> void
{
	shaderFactory->BindShader(shaders[2]);
	if (lastVertexShaderUse && shaders[2] != lastVertexShaderUse)
		pipeline->RebindCamera();
	lastVertexShaderUse = shaders[2];
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
#ifndef VITA
		((D3D11Buffer*)(textureMesh->GetVertexBuffer()))->SetStride(5 * sizeof(float));
#endif
		textureMesh->SetInVRAM(true);
	}

	pipeline->drawTexture(screenRect, textureMesh, renderData);
}

auto	GraphicalLibrary::DrawLine(Vector3F const& firstPoint, Vector3F const& secondPoint, Vector3F const& color) -> void
{
	shaderFactory->BindShader(shaders[5]);
	if (lastVertexShaderUse && shaders[5] != lastVertexShaderUse)
		pipeline->RebindCamera();
	lastVertexShaderUse = shaders[5];
	shaderFactory->BindShader(shaders[1]);
	pipeline->draw3DLine(firstPoint, secondPoint, color);
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

#include <imgui/imgui.h>

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