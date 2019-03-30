#ifndef __POLYGONRENDERRESOURCE_HPP__
#define __POLYGONRENDERRESOURCE_HPP__

#include <vector>
#include <MUtils/Maths/Vector.hpp>
#include <d2d1.h>
#include <wrl/client.h>

class PolygonRenderResource
{
public:
	PolygonRenderResource() = default;
	PolygonRenderResource(const PolygonRenderResource&) = delete;
	PolygonRenderResource(PolygonRenderResource&&) = delete;
	~PolygonRenderResource() = default;

	auto	AddPoint(Vector2F const& point) -> void;
	auto	ConstructGeometry() -> void;

	auto	ClearGeometry() -> void;

	auto	GetGeometry() const -> ID2D1Geometry* { return polygonGeometry.Get(); }

	auto	operator = (const PolygonRenderResource&)->PolygonRenderResource& = delete;
	auto	operator = (PolygonRenderResource&&)->PolygonRenderResource& = delete;

private:
	std::vector<Vector2F>						points;
	Microsoft::WRL::ComPtr<ID2D1PathGeometry>	polygonGeometry = nullptr;
	bool										isConstructionDirty = false;

#ifdef _DEBUG
public:
	auto	ImGuiUpdate() -> void;
private:
	Vector2F		tempPointValue;
	unsigned int	idxToRemove = 0;
#endif // _DEBUG

};

#endif /*__POLYGONRENDERRESOURCE_HPP__*/