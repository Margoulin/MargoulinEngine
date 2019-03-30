#ifndef __TetrisService_HPP__
#define __TetrisService_HPP__

#include "ServiceApplication.hpp"

#include "MUtils/Maths/Transform.hpp"

class SkeletalMeshResource;
class MaterialResource;
class TextureResource;
class D3D11Renderer;
class Node;

class TetrisService : public ServiceApplication
{
public:
	TetrisService() = default;
	TetrisService(const TetrisService&) = delete;
	TetrisService(TetrisService&&) = delete;
	~TetrisService() = default;

	virtual auto	Initialize() -> void;
	virtual auto	Shutdown() -> void;
	virtual auto	Update() -> void;
	virtual auto	Draw() -> void;

	auto	operator = (const TetrisService&)->TetrisService& = delete;
	auto	operator = (TetrisService&&)->TetrisService& = delete;

#ifdef _DEBUG
	virtual auto	ImGuiUpdate() -> void;
#endif // _DEBUG

protected:

private:
	TextureResource*	spritesheet = nullptr;
	D3D11Renderer*		d3d11Renderer = nullptr;

	Node*					cameraNode = nullptr;
	SkeletalMeshResource* meshRes = nullptr;
	MaterialResource*	matRes = nullptr;
	MTransform fbxTransform;
};
	
extern "C"
{
	_declspec(dllexport)	Service* CreateApplicationService() { return new TetrisService(); }
}

#endif /*__TetrisService_HPP__*/