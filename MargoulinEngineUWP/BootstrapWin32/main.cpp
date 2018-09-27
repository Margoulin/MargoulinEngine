#include "../Engine/headers/Window.hpp"
#include "../Engine/headers/Engine.hpp"

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int)
{
	int exitCode = 0;

	Window* window = Window::CreateWindowInstance();

	ShowWindow(window->GetWindow(), SW_SHOWDEFAULT);
	UpdateWindow(window->GetWindow());
	
	Engine* engine = Engine::GetInstance();
	engine->Initialize(window);

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

		if (engine->Update())
			engine->Draw();
	}
	AnimateWindow(window->GetWindow(), 200, AW_HIDE | AW_BLEND);

	engine->Shutdown();
	window->Shutdown();
	delete window;

	return exitCode;
}