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
		GRAPHIC,
		CAMERA
	};

	virtual auto Initialize() -> void {}
	virtual auto Shutdown() -> void {}
	virtual auto Update() -> void {}
	virtual auto DestroyComponent(bool removeFromNode = false) -> void;

	virtual auto	SetNode(Node* value) -> void { attachedNode = value; }
	virtual auto	GetComponentType() const -> COMPONENT_TYPE const = 0;
	
	virtual auto	GetObjectType() const -> ObjectType const { return COMPONENT; }

	auto	operator = (const Component&)->Component& = delete;
	auto	operator = (Component&&)->Component& = delete;

protected:
	Node*			attachedNode = nullptr;

private:

#ifdef _DEBUG

public:
	virtual auto	ImGuiUpdate() -> void;

	virtual auto	GetComponentTypeName() -> char* = 0;

#endif // _DEBUG


};


#endif /*__COMPONENT_HPP__*/