#ifndef __SCENE_HPP__
#define __SCENE_HPP__

class Node;

class Scene
{
public:
	Scene();
	Scene(const Scene&) = delete;
	Scene(Scene&&) = delete;
	~Scene() = default;

	auto	AddNode(Node* value) -> void;
	auto	FindNode(unsigned int const& value)->Node*;

	auto	GetRoot() -> Node* { return root; }

	auto	Shutdown() -> void;
	auto	Update() -> void {}

	auto	operator = (const Scene&)->Scene& = delete;
	auto	operator = (Scene&&)->Scene& = delete;

protected:

private:
	Node*				root;

#ifdef _DEBUG

public:
	auto	ImGuiUpdate() -> void;
#endif // _DEBUG

};


#endif /*__SCENE_HPP__*/