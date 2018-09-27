#ifndef __SERVICE_HPP__
#define __SERVICE_HPP__

#include <string>

class Service
{
public:
	virtual ~Service() {}

	virtual auto	Initialize() -> void = 0;
	virtual auto	Shutdown() -> void = 0;

#ifdef _DEBUG
	virtual auto	ImGuiUpdate() -> void = 0;
#endif // _DEBUG

	auto	IsInitialize() const -> bool { return Initialized; }

	auto	SetName(std::string const& value) -> void { name = value; }
	auto	SetShutdownOrderIndex(unsigned int const value) -> void { shutdownOrderIndex = value; }

	auto	GetName() const -> std::string const& { return name; }
	auto	GetShutdownOrderIndex() const -> unsigned int { return shutdownOrderIndex; }
protected:
	bool			Initialized = false;
	std::string		name;
	unsigned int	shutdownOrderIndex = 0;
private:
};


#endif /*__SERVICE_HPP__*/