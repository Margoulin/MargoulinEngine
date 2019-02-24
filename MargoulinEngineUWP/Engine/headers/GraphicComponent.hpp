#ifndef __GRAPHICCOMPONENT_HPP__
#define __GRAPHICCOMPONENT_HPP__

#include "Component.hpp"

class GraphicComponent : public Component
{
public:
	GraphicComponent() = default;
	GraphicComponent(const GraphicComponent&) = delete;
	GraphicComponent(GraphicComponent&&) = delete;
	~GraphicComponent() = default;

	virtual	auto	Draw() -> void = 0;
	
	virtual auto	GetComponentType() const -> COMPONENT_TYPE const { return COMPONENT_TYPE::GRAPHIC; }
	virtual auto	GetObjectType() const -> ObjectType const { return GRAPHIC_COMPONENT; }
	virtual auto	GetObjectTypeName() const -> MString const override { return "Graphic Component"; }

	auto	operator = (const GraphicComponent&)->GraphicComponent& = delete;
	auto	operator = (GraphicComponent&&)->GraphicComponent& = delete;

protected:

private:

};


#endif /*__GRAPHICCOMPONENT_HPP__*/