#ifndef __CORE_MACROS_HPP__
#define __CORE_MACROS_HPP__

#include "Engine.hpp"
#include "GraphicalLibrary.hpp"
#include "Logger.hpp"

#define ENGINE_INSTANCE() (Engine::GetInstance())
#define SERVICE(serviceName) ( ENGINE_INSTANCE->GetRawService(serviceName))
#define RENDERER() ( ENGINE_INSTANCE()->GetService<GraphicalLibrary>("Renderer"))
#define	LOG(value) (Logger::Log(value)) 

#endif /*__CORE_MACROS_HPP__*/