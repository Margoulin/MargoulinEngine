#ifndef __EXPORTEDFUNCTIONS_HPP__
#define __EXPORTEDFUNCTIONS_HPP__

#include "Engine.hpp"
#include "Window.hpp"

#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>  

extern "C"
{
	_declspec(dllexport)	Engine*	CreateEngine() 
	{ 
#ifdef _PROFILING_ENABLED_
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
		return Engine::GetInstance(); 
	}
	_declspec(dllexport)	bool	UpdateEngine() { return Engine::GetInstance()->Update(); }
	_declspec(dllexport)	void	EngineDraw() { Engine::GetInstance()->Draw(); }
	_declspec(dllexport)	void	EngineShutdown() { Engine::GetInstance()->Shutdown();	}
	_declspec(dllexport)	void	InitializeEngine(Window* win) { Engine::GetInstance()->Initialize(win); }
#ifndef UWP
	_declspec(dllexport)	LRESULT	CallWindowCallback(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) { if (Window::eventCallback) return Window::eventCallback(hwnd, msg, wParam, lParam); }
#endif
}

#endif /*__EXPORTEDFUNCTIONS_HPP__*/