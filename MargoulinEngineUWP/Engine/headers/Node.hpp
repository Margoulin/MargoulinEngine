#ifndef __NODE_HPP__
#define __NODE_HPP__

#include "NodeTransform.hpp"
#include "Object.hpp"

#include <vector>

class Scene;
class Component;

class Node : public Object
{
public:
	Node();
	Node(const Node&) = delete;
	Node(Node&&) = delete;
	~Node() = default;

	auto	Initialize() -> void {}
	auto	Shutdown() -> void {}
	auto	Update() -> void {}

	auto	DestroyNode(bool removeFromParent = false) -> void;

	auto	AddComponent(Component* value) -> void;
	auto	AddChild(Node* value) -> void;
	auto	RemoveChild(Node* value) -> void;
	auto	DetachChild(Node* value) -> void;
	auto	DetachComponent(Component* comp) -> void;

	auto	SetScene(Scene* value) -> void { scene = value; }
	auto	SetName(MString const& value) -> void { name = value; }
	auto	SetParent(Node* value) -> void { parent = value; }

	auto	GetScene() -> Scene* { return this->scene; }
	auto	GetChildrens() const -> std::vector<Node*> const& { return childrens; }
	auto	GetParent() const -> Node* { return parent; }
	auto	GetName() const -> MString const& { return name; }
	auto	GetTransform() -> Transform* { return &transformation; }

	virtual auto	GetObjectType() const -> ObjectType const { return ObjectType::NODE; }
	virtual auto	GetObjectTypeName() const -> MString const override { return "Node"; }

	auto	operator = (const Node&)->Node& = delete;
	auto	operator = (Node&&)->Node& = delete;

protected:

private:
	std::vector<Node*>		childrens;
	std::vector<Component*>	components;
	Transform				transformation;
	MString				name = "Default Name";
	Scene*					scene = nullptr;
	Node*					parent = nullptr;

#ifdef _DEBUG

public:
	virtual auto	ImGuiUpdate() -> void;

#endif
};


#endif /*__NODE_HPP__*/