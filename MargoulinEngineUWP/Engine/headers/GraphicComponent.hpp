#ifndef __GRAPHICCOMPONENT_HPP__
#define __GRAPHICCOMPONENT_HPP__

#include "Component.hpp"

class GraphicComponent : public Component
{
public:
	GraphicComponent() { type = GRAPHIC; }
	GraphicComponent(const GraphicComponent&) = delete;
	GraphicComponent(GraphicComponent&&) = delete;
	~GraphicComponent() = default;

	virtual auto	Shutdown() -> void = 0;
	
	virtual	auto	Draw() -> void = 0;
	
	virtual auto	GetObjectType() const -> ObjectType const { return GRAPHIC_COMPONENT; }
	virtual auto	GetObjectTypeName() const -> std::string const override { return "Graphic Component"; }

	auto	operator = (const GraphicComponent&)->GraphicComponent& = delete;
	auto	operator = (GraphicComponent&&)->GraphicComponent& = delete;

protected:

private:

};


#endif /*__GRAPHICCOMPONENT_HPP__*/