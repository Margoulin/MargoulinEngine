#ifndef __GXM_CONTEXT_HPP__
#define __GXM_CONTEXT_HPP__

#include "Context.hpp"

class GXMContext final : public Context
{
public:
	GXMContext() = default;
	GXMContext(const GXMContext&) = delete;
	GXMContext(GXMContext&&) = delete;
	~GXMContext() = default;

	virtual auto	Initialize(Window* win) -> void {}
	virtual auto	Shutdown() -> void {}

	virtual auto	Swap() -> void {}
	virtual auto	Resize(Vector2F const&) -> void {} //USELESS BECAUSE OF FIXED SIZE

	auto	operator = (const GXMContext&)->GXMContext& = delete;
	auto	operator = (GXMContext&&)->GXMContext& = delete;

protected:

private:

};


#endif /*__GXM_CONTEXT_HPP__*/