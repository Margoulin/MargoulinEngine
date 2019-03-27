#ifndef __CONTEXT_HPP__
#define __CONTEXT_HPP__

#include <MUtils/Maths/Vector.hpp>

class Window;

class Context
{
public:
	Context() = default;
	Context(const Context&) = delete;
	Context(Context&&) = delete;
	~Context() = default;

	virtual auto	Initialize(Window* window) -> void { this->window = window; }
	virtual auto	Shutdown() -> void = 0;
	virtual auto	Swap() -> void = 0;
	
#ifdef UWP 
	virtual auto	SetWindow() -> void = 0;
#endif
	virtual auto	Resize(Vector2F const& size) -> void = 0;

	auto	operator = (const Context&)->Context& = delete;
	auto	operator = (Context&&)->Context& = delete;

protected:
	Window*	window;

private:

#ifdef _DEBUG
public:
	auto	ImGuiUpdate() -> void {}
#endif
};


#endif /*__CONTEXT_HPP__*/