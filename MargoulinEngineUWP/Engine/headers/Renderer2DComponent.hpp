#ifndef __RENDERER2DCOMPONENT_HPP__
#define __RENDERER2DCOMPONENT_HPP__

#include "GraphicComponent.hpp"
#include "Maths/Vector.hpp"
#include "PolygonRenderResource.hpp"

class Renderer2DComponent : public GraphicComponent
{
public:
	Renderer2DComponent() : edgedRadius(10.0f, 10.0f) { }
	Renderer2DComponent(const Renderer2DComponent&) = delete;
	Renderer2DComponent(Renderer2DComponent&&) = delete;
	~Renderer2DComponent() = default;

	enum Renderer2DType
	{
		NONE,
		RECTANGLE,
		CIRCLE,
		LINE,
		POLYGON
	};

	virtual	auto	Draw() -> void;
	
	virtual auto	GetObjectType() const -> ObjectType const { return RENDERER_2D_COMPONENT; }
	virtual auto	GetObjectTypeName() const -> MString const override { return "Renderer 2D Component"; }

	auto	operator = (const Renderer2DComponent&)->Renderer2DComponent& = delete;
	auto	operator = (Renderer2DComponent&&)->Renderer2DComponent& = delete;

protected:

private:
	Renderer2DType	currentType;

	Vector4F		color;
	bool			filled = true;

	Vector2F		rectangleSize;
	Vector2F		rectanglePosition;
	bool			roundEdges = false;
	Vector2F		edgedRadius;

	Vector2F		circlePosition;
	Vector2F		circleRadius;

	Vector2F		linePoint;
	Vector2F		linePoint2;

	PolygonRenderResource	polygon;

#ifdef _DEBUG
public:
	virtual auto	ImGuiUpdate() -> void override;
	virtual auto	GetComponentTypeName() -> char* { return "Renderer 2D Component"; }
#endif

};


#endif /*__RENDERER2DCOMPONENT_HPP__*/