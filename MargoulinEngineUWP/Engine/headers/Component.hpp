#ifndef __COMPONENT_HPP__
#define __COMPONENT_HPP__

#include "Object.hpp"

class Node;

class Component : public Object
{
public:
	Component() = default;
	Component(const Component&) = delete;
	Component(Component&&) = delete;
	~Component() = default;

	enum COMPONENT_TYPE
	{
		GRAPHIC
	};

	virtual auto	Shutdown() -> void = 0;

	virtual auto	SetNode(Node* value) -> void { attachedNode = value; }
	virtual auto	GetType() const -> COMPONENT_TYPE const { return type; }
	
	virtual auto	GetObjectType() const -> ObjectType const { return COMPONENT; }

	auto	operator = (const Component&)->Component& = delete;
	auto	operator = (Component&&)->Component& = delete;

protected:
	COMPONENT_TYPE	type;
	Node*			attachedNode = nullptr;

private:

#ifdef _DEBUG

public:
	virtual auto	ImGuiUpdate() -> void {}

	virtual auto	GetComponentTypeName() -> char* = 0;

#endif // _DEBUG


};


#endif /*__COMPONENT_HPP__*/