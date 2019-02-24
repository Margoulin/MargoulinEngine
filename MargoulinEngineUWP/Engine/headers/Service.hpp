#ifndef __SERVICE_HPP__
#define __SERVICE_HPP__

class Service
{
public:
	virtual ~Service() {}

	virtual auto	Initialize() -> void { initialized = true; }
	virtual auto	Shutdown() -> void = 0;
	virtual auto	Update() -> void = 0;

	auto	IsInitialize() const -> bool { return initialized; }

	auto	SetUpdateOrderIndex(unsigned int const value) -> void { updateOrderIndex = value; }
	auto	SetShutdownOrderIndex(unsigned int const value) -> void { shutdownOrderIndex = value; }

	auto	GetUpdateOrderIndex() const -> unsigned int { return updateOrderIndex; }
	auto	GetShutdownOrderIndex() const -> unsigned int { return shutdownOrderIndex; }
protected:
	unsigned int	shutdownOrderIndex = 1;
	unsigned int	updateOrderIndex = 0;
	bool			initialized = false;

#ifdef _DEBUG
public:
	virtual auto	ImGuiUpdate() -> void = 0;
#endif // _DEBUG

};


#endif /*__SERVICE_HPP__*/