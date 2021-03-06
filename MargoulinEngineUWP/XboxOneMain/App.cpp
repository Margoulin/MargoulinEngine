﻿#include "pch.h"
#include "App.h"

#include <ppltasks.h>

#include <string>
#include "InputManager.hpp"
#include "GraphicalLibrary.hpp"
#include "Context.hpp"
#include "D3D11Context.hpp"
#include "Window.hpp"
#include "ObjLoader.hpp"
#include "Node.hpp"
#include "UWPGamepad.hpp"

using namespace UWPBootstrap;

using namespace concurrency;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::UI::Core;
using namespace Windows::UI::Input;
using namespace Windows::System;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Display;

// The main function is only used to initialize our IFrameworkView class.
[Platform::MTAThread]
int main(Platform::Array<Platform::String^>^)
{
	auto direct3DApplicationSource = ref new Direct3DApplicationSource();
	CoreApplication::Run(direct3DApplicationSource);
	return 0;
}

IFrameworkView^ Direct3DApplicationSource::CreateView()
{
	return ref new App();
}

//ENGINE SOME INITIALIZE SHIT

App::App() :
	m_windowClosed(false),
	m_windowVisible(true),
	engine(new Engine)
{
#ifndef _XBOX_ONE
	Windows::UI::ViewManagement::ApplicationView::PreferredLaunchViewSize = Windows::Foundation::Size(960.0f, 544.0f);
	Windows::UI::ViewManagement::ApplicationView::PreferredLaunchWindowingMode = Windows::UI::ViewManagement::ApplicationViewWindowingMode::PreferredLaunchViewSize;
#endif
	
	Platform::String^ localfolder = Package::Current->InstalledLocation->Path;//Windows::ApplicationModel::Package::Current->InstalledLocation->Path; // Windows::Storage::ApplicationData::Current->LocalFolder->Path;
	ObjLoader::LocalFolderName = MString(std::string(localfolder->Begin(), localfolder->End()).c_str());
}

//ENGINE SETUP

// The first method called when the IFrameworkView is being created.
void App::Initialize(CoreApplicationView^ applicationView)
{
	// Register event handlers for app lifecycle. This example includes Activated, so that we
	// can make the CoreWindow active and start rendering on the window.
	applicationView->Activated +=
		ref new TypedEventHandler<CoreApplicationView^, IActivatedEventArgs^>(this, &App::OnActivated);

	CoreApplication::Suspending +=
		ref new EventHandler<SuspendingEventArgs^>(this, &App::OnSuspending);

	CoreApplication::Resuming +=
		ref new EventHandler<Platform::Object^>(this, &App::OnResuming);

	//Windows::ApplicationModel::Package::Current->InstalledLocation
	Platform::String^ localfolder = Windows::ApplicationModel::Package::Current->InstalledLocation->Path;

	Windows::Gaming::Input::Gamepad::GamepadAdded +=
		ref new EventHandler<Windows::Gaming::Input::Gamepad^>(this, &App::OnGamepadAdded);
	Windows::Gaming::Input::Gamepad::GamepadRemoved +=
		ref new EventHandler<Windows::Gaming::Input::Gamepad^>(this, &App::OnGamepadRemoved);

	// At this point we have access to the device. 
	// We can create the device-dependent resources.
	//m_deviceResources = std::make_shared<DX::DeviceResources>();
	engine = Engine::GetInstance();
	engine->Initialize(new Window);

	HMODULE appHandle = LoadPackagedLibrary(L"Application.dll", 0);
	bool(*updateEngine)() = nullptr;
	void(*engineDraw)() = nullptr;
	void(*engineShutdown)() = nullptr;
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
		//windowCallback = (LRESULT(*)(HWND, UINT, WPARAM, LPARAM))GetProcAddress(appHandle, "CallWindowCallback");

		engine = createEngine();
		//initEngine(window);
		//Service* appService = nullptr;
		//appService = createAppService();
		//appService->Initialize();
		//engine->AddService("Application", appService);
		//gameLoaded = true;
	}

	for (auto iter = Windows::Gaming::Input::Gamepad::Gamepads->First(); iter->HasCurrent; iter->MoveNext())
		createGamepad(iter->Current);
}

//ENGINE WINDOW_SETUP

// Called when the CoreWindow object is created (or re-created).
void App::SetWindow(CoreWindow^ window)
{
	window->SizeChanged +=
		ref new TypedEventHandler<CoreWindow^, WindowSizeChangedEventArgs^>(this, &App::OnWindowSizeChanged);

	window->VisibilityChanged +=
		ref new TypedEventHandler<CoreWindow^, VisibilityChangedEventArgs^>(this, &App::OnVisibilityChanged);

	window->Closed +=
		ref new TypedEventHandler<CoreWindow^, CoreWindowEventArgs^>(this, &App::OnWindowClosed);
	window->KeyDown +=
		ref new TypedEventHandler<CoreWindow^, Windows::UI::Core::KeyEventArgs^>(this, &App::OnKeyDown);
	window->PointerPressed +=
		ref new TypedEventHandler<CoreWindow^, Windows::UI::Core::PointerEventArgs^>(this, &App::OnPointerDown);
	window->PointerMoved +=
		ref new TypedEventHandler<CoreWindow^, Windows::UI::Core::PointerEventArgs^>(this, &App::OnPointerMoved);
	window->PointerReleased +=
		ref new TypedEventHandler<CoreWindow^, Windows::UI::Core::PointerEventArgs^>(this, &App::OnPointerUp);

	DisplayInformation^ currentDisplayInformation = DisplayInformation::GetForCurrentView();
	float temp = currentDisplayInformation->LogicalDpi;

	currentDisplayInformation->DpiChanged +=
		ref new TypedEventHandler<DisplayInformation^, Object^>(this, &App::OnDpiChanged);

	currentDisplayInformation->OrientationChanged +=
		ref new TypedEventHandler<DisplayInformation^, Object^>(this, &App::OnOrientationChanged);

	DisplayInformation::DisplayContentsInvalidated +=
		ref new TypedEventHandler<DisplayInformation^, Object^>(this, &App::OnDisplayContentsInvalidated);

	//m_deviceResources->SetWindow(window);

	GraphicalLibrary* lib = engine->GetService<GraphicalLibrary>("Renderer");
#ifdef _XBOX_ONE
	lib->UpdateWindowHandle((IUnknown*)window, Vector2F(1920.0f, 1080.0f), currentDisplayInformation->LogicalDpi);
#else
	lib->UpdateWindowHandle((IUnknown*)window, Vector2F(window->Bounds.Width, window->Bounds.Height), currentDisplayInformation->LogicalDpi);
#endif
}

// Initializes scene resources, or loads a previously saved app state.
void App::Load(Platform::String^ entryPoint)
{
	//if (m_main == nullptr)
	//{
	//m_main = std::unique_ptr<UWPBootstrapMain>(new UWPBootstrapMain(m_deviceResources));
	//}
}

//ENGINE UPDATE

// This method is called after the window becomes active.
void App::Run()
{
	while (!m_windowClosed)
	{
		if (m_windowVisible)
		{
			CoreWindow::GetForCurrentThread()->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);
			unsigned int pads = Windows::Gaming::Input::Gamepad::Gamepads->Size;

			if (engine->Update())
				engine->Draw();
		}
		else
		{
			CoreWindow::GetForCurrentThread()->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessOneAndAllPending);
		}
	}
}

// Required for IFrameworkView.
// Terminate events do not cause Uninitialize to be called. It will be called if your IFrameworkView
// class is torn down while the app is in the foreground.
void App::Uninitialize()
{
}

// Application lifecycle event handlers.

void App::OnActivated(CoreApplicationView^ applicationView, IActivatedEventArgs^ args)
{
	// Run() won't start until the CoreWindow is activated.
	CoreWindow::GetForCurrentThread()->Activate();
}

void App::OnSuspending(Platform::Object^ sender, SuspendingEventArgs^ args)
{
	// Save app state asynchronously after requesting a deferral. Holding a deferral
	// indicates that the application is busy performing suspending operations. Be
	// aware that a deferral may not be held indefinitely. After about five seconds,
	// the app will be forced to exit.

	SuspendingDeferral^ deferral = args->SuspendingOperation->GetDeferral();

	create_task([this, deferral]()
	{
		//m_deviceResources->Trim();

		// Insert your code here.

		deferral->Complete();
	});
}

void App::OnResuming(Platform::Object^ sender, Platform::Object^ args)
{
	// Restore any data or state that was unloaded on suspend. By default, data
	// and state are persisted when resuming from suspend. Note that this event
	// does not occur if the app was previously terminated.

	// Insert your code here.
}

// Window event handlers.

//ENGINE WINDOW_RESIZED

void App::OnWindowSizeChanged(CoreWindow^ sender, WindowSizeChangedEventArgs^ args)
{
	//m_deviceResources->SetLogicalSize(Size(sender->Bounds.Width, sender->Bounds.Height));
	//m_main->CreateWindowSizeDependentResources();
	GraphicalLibrary* lib = engine->GetService<GraphicalLibrary>("Renderer");
	lib->GetContext()->Resize(Vector2F(sender->Bounds.Width, sender->Bounds.Height));
}

void App::OnVisibilityChanged(CoreWindow^ sender, VisibilityChangedEventArgs^ args)
{
	m_windowVisible = args->Visible;
}

void App::OnWindowClosed(CoreWindow^ sender, CoreWindowEventArgs^ args)
{
	m_windowClosed = true;
}

// DisplayInformation event handlers.

void App::OnDpiChanged(DisplayInformation^ sender, Object^ args)
{
}

void App::OnOrientationChanged(DisplayInformation^ sender, Object^ args)
{
	//m_main->CreateWindowSizeDependentResources();
}

void App::OnDisplayContentsInvalidated(DisplayInformation^ sender, Object^ args)
{
	//m_deviceResources->ValidateDevice();
}

//ENGINE FROM_HERE_INPUT_FUNCTIONS

void	App::OnPointerMoved(Windows::UI::Core::CoreWindow^ window, Windows::UI::Core::PointerEventArgs^ args)
{
	auto val = args->CurrentPoint->Position;
	InputManager* mgr = engine->GetService<InputManager>("Input Manager");
	mgr->OnPointerMoved(val.X, val.Y);
}

void	App::OnPointerDown(Windows::UI::Core::CoreWindow^ window, Windows::UI::Core::PointerEventArgs^ args)
{
	InputManager* mgr = engine->GetService<InputManager>("Input Manager");
	if (args->CurrentPoint->Properties->IsLeftButtonPressed)
		mgr->OnLeftClick(true);
	else if (args->CurrentPoint->Properties->IsRightButtonPressed)
		mgr->OnRightClick(true);
	else if (args->CurrentPoint->Properties->IsMiddleButtonPressed)
		mgr->OnMiddleClick(true);
}

void	App::OnPointerUp(Windows::UI::Core::CoreWindow^ window, Windows::UI::Core::PointerEventArgs^ args)
{
	InputManager* mgr = engine->GetService<InputManager>("Input Manager");
	if (!args->CurrentPoint->Properties->IsLeftButtonPressed)
		mgr->OnLeftClick(false);
	else if (!args->CurrentPoint->Properties->IsRightButtonPressed)
		mgr->OnRightClick(false);
	else if (!args->CurrentPoint->Properties->IsMiddleButtonPressed)
		mgr->OnMiddleClick(false);
}

void	App::OnKeyDown(CoreWindow^ window, Windows::UI::Core::KeyEventArgs^ args)
{
	InputManager* mgr = (InputManager*)(engine->GetService<InputManager>("Input Manager"));
	mgr->OnKeyDown((unsigned int)args->VirtualKey);
}
		
void	App::OnGamepadAdded(Platform::Object^, Windows::Gaming::Input::Gamepad^ pad)
{
	if (engine && engine->IsInitialized())
		createGamepad(pad);
}

/*
void	App::OnArcadeStickAdded(Platform::Object^, Windows::Gaming::Input::ArcadeStick^ pad)
{
	if (controllerCount == 4)
		return;
	controllers[controllerCount] = pad;
	controllerCount++;
}
*/

void	App::OnGamepadRemoved(Platform::Object^, Windows::Gaming::Input::Gamepad^ pad)
{
	for (auto it = gamepads.begin(); it != gamepads.end(); it++)
	{
		if ((*it)->CompareGamepadValue(pad))
		{
			InputManager* mgr = engine->GetService<InputManager>("Input Manager");
			mgr->RemoveGamepad((*it));
			gamepads.erase(it);
			return;
		}
	}
}

/*
void	App::OnArcadeStickRemoved(Platform::Object^, Windows::Gaming::Input::ArcadeStick^ pad)
{
	controllerCount--;
}
*/

//ENGINE CREATE_GAMEPAD

auto	App::createGamepad(Windows::Gaming::Input::Gamepad^ pad) -> void
{
	InputManager* mgr = engine->GetService<InputManager>("Input Manager");
	if (mgr->GetGamepadsCount() == 4)
		return;
	UWPGamepad* temp = new UWPGamepad(pad);
	mgr->AddGamepad((Gamepad*)temp);
	gamepads.push_back(temp);
}