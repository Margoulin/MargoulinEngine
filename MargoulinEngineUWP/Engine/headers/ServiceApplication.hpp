#ifndef __SERVICE_APPLICATION_HPP__
#define __SERVICE_APPLICATION_HPP__

#include "Service.hpp"

class ServiceApplication : public Service
{
public:
	ServiceApplication() = default;
	ServiceApplication(const ServiceApplication&) = delete;
	ServiceApplication(ServiceApplication&&) = delete;
	~ServiceApplication() = default;

	virtual auto	Draw() -> void = 0;

	auto	operator = (const ServiceApplication&)->ServiceApplication& = delete;
	auto	operator = (ServiceApplication&&)->ServiceApplication& = delete;

protected:

private:

};


#endif /*__SERVICE_APPLICATION_HPP__*/