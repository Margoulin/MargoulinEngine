#include "TetrisService.h"

#include "CoreMinimal.hpp"

#include "ExportedFunctions.h"
#include "Engine.hpp"
#include "Logger.hpp"

#include "TextureLoader.hpp"
#include "ResourcesManager.hpp"
#include "TextureResource.hpp"
#include "D3D11Renderer.hpp"
#include "ObjLoader.hpp"
#include "FBXLoader.hpp"
#include "D3D11TextureData.hpp"

#include "MUtils/Maths/Math.hpp"

#include "MemoryMacro.hpp"
#include "CoreMacros.hpp"

#include "Node.hpp"
#include "CameraComponent.hpp"
#include "ObjectManager.hpp"

auto	TetrisService::Initialize() -> void
{
	TextureLoader::LoadTextureFromFile("Resources/Spritesheet.png");

	ResourcesManager* rsMgr = ENGINE_INSTANCE()->GetService<ResourcesManager>("Resources Manager");
	spritesheet = rsMgr->GetResource<TextureResource>(1);
	auto* rend = (D3D11Renderer*)RENDERER()->GetRenderPipeline();
	rend->InitializeTexture(spritesheet);

	//ObjLoader::LoadObjFromFile("Sherlock.objfile");
	//ObjLoader::LoadObjFromFile("23FEF89A_FFFF_00000E11.objfile");

	//FBXLoader::LoadFBXFromFile("sherlock.fbx");
	
	FBXLoader::LoadFBXFromFile("Jog_Fwd_Rifle.fbx");
	meshRes = rsMgr->GetResource<SkeletalMeshResource>(2);
	matRes = rsMgr->GetResource<MaterialResource>(0);

	ObjectManager* objMgr = Engine::GetInstance()->GetService<ObjectManager>("Object Manager");
	cameraNode = objMgr->Create<Node>();
	cameraNode->SetName("CameraNode");
	cameraNode->Initialize();
	cameraNode->AddComponent(objMgr->Create<CameraComponent>());
	
	cameraNode->GetTransform()->SetPosition(Vector3F(0.0f, -1.0f, 1.0f));
	cameraNode->GetTransform()->SetRotation(Quaternion::Euler(Vector3F(90.0f, 0.0f, 0.0f) * degToRad));
	fbxTransform.SetScale(Vector3F(0.01f, 0.01f, 0.01f));
	//fbxTransform.SetRotation(Quaternion::Euler(Vector3F(270.0f, 0.0f, 0.0f) * degToRad));
}

auto	TetrisService::Shutdown() -> void
{
}

auto	TetrisService::Update() -> void
{
}

auto	TetrisService::Draw() -> void
{
	auto val = ((D3D11TextureData*)(spritesheet->GetTextureData()))->CreateTextureRenderData(Vector4F(8.0f, 6.0f, 16.0f, 16.0f));
	auto* rend = RENDERER();
	rend->DrawSkeletalMesh(fbxTransform.GetLocalMatrix(), meshRes, matRes);
	rend->DrawTexture(Vector4F(100.0f, 100.0f, 16.0f, 16.0f), val);
}

#include <ImGui/imgui.h>
#ifdef _DEBUG

void ImGUIMTransform(MTransform* tr)
{
	Vector3F position = tr->GetPosition();
	Vector3F scale = tr->GetScale();
	ImGui::InputFloat3("Position", (float*)(&position.x));
	ImGui::InputFloat3("Scale", (float*)(&scale.x));

	Vector3F	eulerAngles = tr->GetEulerRotation();
	Vector3F tempEuler = eulerAngles * radToDeg;
	ImGui::DragFloat3("Euler Angles", (float*)(&tempEuler.x), 0.5f, -360.0f, 360.0f);
	tr->SetPosition(position);
	tr->SetScale(scale);
	tr->SetRotation(Quaternion::Euler(tempEuler * degToRad));
}

auto	TetrisService::ImGuiUpdate() -> void
{
	ImGui::Text("FBX");
	ImGUIMTransform(cameraNode->GetTransform());
	//ImGUIMTransform(&fbxTransform);
}

#endif // _DEBUG