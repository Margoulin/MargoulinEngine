#include "../Engine/headers/Window.hpp"
#include "../Engine/headers/Engine.hpp"
#include "../Engine/headers/Service.hpp"

LRESULT(*windowCallback)(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) = nullptr;
bool gameLoaded = false;

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (gameLoaded)
	{
		if (windowCallback)
			windowCallback(hWnd, msg, wParam, lParam);
	}
	else
	{
		if (Window::eventCallback)
			Window::eventCallback(hWnd, msg, wParam, lParam);
	}

	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int)
{
	int exitCode = 0;

	Window* window = Window::CreateWindowInstance(WndProc);

	ShowWindow(window->GetWindow(), SW_SHOWDEFAULT);
	UpdateWindow(window->GetWindow());

	Engine* engine = nullptr;

	bool(*updateEngine)() = nullptr;
	void(*engineDraw)() = nullptr;
	void(*engineShutdown)() = nullptr;
	HMODULE	appHandle = LoadLibraryA("Application.dll");
	if (appHandle)
	{
		Service*(*createAppService)() = nullptr;
		Engine*(*createEngine)() = nullptr;
		void(*initEngine)(Window* win) = nullptr;

		createAppService = (Service*(*)())GetProcAddress(appHandle, "CreateApplicationService");
		createEngine = (Engine*(*)())GetProcAddress(appHandle, "CreateEngine");
		initEngine = (void(*)(Window*))GetProcAddress(appHandle, "InitializeEngine");
		updateEngine = (bool(*)())GetProcAddress(appHandle, "UpdateEngine");
		engineDraw = (void(*)())GetProcAddress(appHandle, "EngineDraw");
		engineShutdown = (void(*)())GetProcAddress(appHandle, "EngineShutdown");
		windowCallback = (LRESULT(*)(HWND, UINT, WPARAM, LPARAM))GetProcAddress(appHandle, "CallWindowCallback");
		
		engine = createEngine();
		initEngine(window);
		Service* appService = nullptr;
		appService = createAppService();
		appService->Initialize();
		engine->AddService("Application", appService);
		gameLoaded = true;
	}

	if (!engine)
	{
		engine = Engine::GetInstance();
		engine->Initialize(window);
	}

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			continue;
		}

		if (gameLoaded)
		{
			if (updateEngine())
				engineDraw();
		}
		else
		{
			if (engine->Update())
				engine->Draw();
		}
	}
	AnimateWindow(window->GetWindow(), 200, AW_HIDE | AW_BLEND);

	if (gameLoaded)
		engineShutdown();
	else
		engine->Shutdown();
	window->Shutdown();
	delete window;

	if (appHandle)
		FreeLibrary(appHandle);

	return exitCode;
}