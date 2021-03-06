#ifndef __LOGGER_HPP__
#define __LOGGER_HPP__

#include "Service.hpp"

#include <MUtils/String.hpp>
#include <vector>

class Logger : public Service
{
public:
	Logger() = default;
	Logger(const Logger&) = delete;
	Logger(Logger&&) = delete;
	~Logger() = default;

	static auto	Log(MString const& log)-> void;

	virtual auto	Initialize() -> void;
	virtual auto	Shutdown() -> void { instance = nullptr; }
	virtual auto	Update() -> void {}

	auto	operator = (const Logger&)->Logger& = delete;
	auto	operator = (Logger&&)->Logger& = delete;

protected:

private:
	static std::vector<MString>	logs;
	static	Logger*				instance;

#ifdef _DEBUG
public:
	virtual auto	ImGuiUpdate() -> void;
#endif
};


#endif /*__LOGGER_HPP__*/