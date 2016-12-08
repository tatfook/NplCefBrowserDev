#include "NplCefBrowserApp.h"

#include <string>
#include <thread> 
#include "include/cef_browser.h"
#include "include/cef_command_line.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_helpers.h"
#include "Core/NPLInterface.hpp"

#include "cefclient/common/client_app.h"
#include "cefclient/common/client_app_other.h"
#include "cefclient/common/client_switches.h"
#include "cefclient/browser/client_app_browser.h"
#include "cefclient/renderer/client_app_renderer.h"
#include "cefclient/browser/main_context_impl.h"
#include "cefclient/browser/main_message_loop_multithreaded_win.h"
#include "cefclient/browser/main_message_loop_std.h"
#include "cefclient/browser/test_runner.h"
#include "include/cef_sandbox_win.h"

#include "cefsimple/simple_app.h"
using namespace client;

/** @def custom user messages that are posted from the main thread to the window thread. */
#define PE_WM_NPLCEFBROWSER_FIRST					WM_USER+2350
#define PE_WM_NPLCEFBROWSER_LOADURL				WM_USER+2351
#define PE_WM_NPLCEFBROWSER_ONSIZE_CHANGED			WM_USER+2352
#define PE_WM_NPLCEFBROWSER_SETCAPTURE				WM_USER+2354
#define PE_WM_NPLCEFBROWSER_RELEASECAPTURE			WM_USER+2355
#define PE_WM_NPLCEFBROWSER_SETFOCUS				WM_USER+2356
#define PE_WM_NPLCEFBROWSER_QUIT					WM_USER+2357
#define PE_WM_NPLCEFBROWSER_CREATE_WINDOW			WM_USER+2359
#define PE_WM_NPLCEFBROWSER_REFRESH				WM_USER+2360
#define PE_WM_NPLCEFBROWSER_STOP					WM_USER+2361
#define PE_WM_NPLCEFBROWSER_GOBACK					WM_USER+2362
#define PE_WM_NPLCEFBROWSER_GOFORWARD				WM_USER+2363
#define PE_WM_NPLCEFBROWSER_SETCONTENT				WM_USER+2364
#define PE_WM_NPLCEFBROWSER_GETCONTENT				WM_USER+2365
#define PE_WM_NPLCEFBROWSER_FINDTEXT				WM_USER+2366
#define PE_WM_NPLCEFBROWSER_LAST					WM_USER+2367

NplCefBrowserApp::NplCefBrowserApp() {
}

void NplCefBrowserApp::OnContextInitialized() {
	
}


NplCefBrowserApp& NplCefBrowserApp::CreateGetSingleton()
{
	static NplCefBrowserApp g_singleton;
	return g_singleton;
}

void NplCefBrowserApp::Create(int moduleHandle, int parentHandle, std::string url)
{
	std::thread t(&NplCefBrowserApp::CreateCefSimple, this, moduleHandle, parentHandle,url);
	t.detach();
}
bool NplCefBrowserApp::MsgProcCustom(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == PE_WM_NPLCEFBROWSER_CREATE_WINDOW)
	{
		int i = 0;
		return true;
	}
	return false;
}
void NplCefBrowserApp::CreateWnd(int moduleHandle, int parentHandle, std::string url)
{
	if (url.empty())
	{
		url = "http://www.wikicraft.cn";
	}
	HWND hWnd = (HWND)parentHandle;
	HINSTANCE instance = (HINSTANCE)moduleHandle;
	HINSTANCE hInstance = GetModuleHandle(NULL);
	HWND m_hBrowserHostWnd = CreateWindowExW(WS_EX_NOPARENTNOTIFY, L"static", NULL, WS_CHILD,
		100, 100, 400, 300,
		hWnd, NULL, hInstance, NULL);
	DWORD m_dwWinThreadID;
	if (m_hBrowserHostWnd != 0)
	{
		m_dwWinThreadID = ::GetWindowThreadProcessId(m_hBrowserHostWnd, NULL);
		PostThreadMessage(m_dwWinThreadID, PE_WM_NPLCEFBROWSER_CREATE_WINDOW, 1, 0);
	}
	else
	{
		return;
	}
	//
	// Dispatching window messages in this window thread. 
	//
	SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) | WS_CLIPCHILDREN);
	::ShowWindow(m_hBrowserHostWnd, SW_SHOW);
	// Now we're ready to receive and process Windows messages.
	MSG  msg;
	msg.message = WM_NULL;
	PeekMessageW(&msg, NULL, 0U, 0U, PM_NOREMOVE);

	while (WM_QUIT != msg.message)
	{
		if (GetMessageW(&msg, NULL, 0U, 0U) != 0)
		{
			if (MsgProcCustom(msg.message, msg.wParam, msg.lParam) == 0)
			{
				// Translate and dispatch the message
				TranslateMessage(&msg);
				DispatchMessageW(&msg);
			}
		}
	}
}
void NplCefBrowserApp::CreateCefClient(int moduleHandle, int parentHandle, std::string url)
{
	if (url.empty())
	{
		url = "http://www.wikicraft.cn";
	}
	HWND hWnd = (HWND)parentHandle;
	HINSTANCE instance = (HINSTANCE)moduleHandle;


	CefMainArgs main_args(instance);

	void* sandbox_info = NULL;

#if defined(CEF_USE_SANDBOX)
	// Manage the life span of the sandbox information object. This is necessary
	// for sandbox support on Windows. See cef_sandbox_win.h for complete details.
	CefScopedSandboxInfo scoped_sandbox;
	sandbox_info = scoped_sandbox.sandbox_info();
#endif

	// Parse command-line arguments.
	CefRefPtr<CefCommandLine> command_line = CefCommandLine::CreateCommandLine();
	command_line->InitFromString(::GetCommandLineW());

	// Create a ClientApp of the correct type.
	CefRefPtr<CefApp> app;
	ClientApp::ProcessType process_type = ClientApp::GetProcessType(command_line);
	process_type = ClientApp::OtherProcess;
	if (process_type == ClientApp::BrowserProcess)
		app = new ClientAppBrowser();
	else if (process_type == ClientApp::RendererProcess)
		app = new ClientAppRenderer();
	else if (process_type == ClientApp::OtherProcess)
		app = new ClientAppOther();

	// Create the main context object.
	scoped_ptr<MainContextImpl> context(new MainContextImpl(command_line, true));

	CefSettings settings;
	// Specify the path for the sub-process executable.
	CefString(&settings.browser_subprocess_path).FromASCII("cef3/NplCefProcess_d.exe");

#if !defined(CEF_USE_SANDBOX)
	settings.no_sandbox = true;
#endif

	// Populate the settings based on command line arguments.
	context->PopulateSettings(&settings);
	// Create the main message loop object.
	scoped_ptr<MainMessageLoop> message_loop;
	if (settings.multi_threaded_message_loop)
		message_loop.reset(new MainMessageLoopMultithreadedWin);
	else
		message_loop.reset(new MainMessageLoopStd);

	// Initialize CEF.
	context->Initialize(main_args, settings, app, sandbox_info);

	// Register scheme handlers.
	test_runner::RegisterSchemeHandlers();
	context->GetRootWindowManager()->setParentHandle(hWnd);
	// Create the first window.
	context->GetRootWindowManager()->CreateRootWindow(
		!command_line->HasSwitch(switches::kHideControls),  // Show controls.
		settings.windowless_rendering_enabled ? true : false,
		CefRect(),        // Use default system size.
		url);   // Use default URL.

				// Run the message loop. This will block until Quit() is called by the
				// RootWindowManager after all windows have been destroyed.
	int result = message_loop->Run();

	// Shut down CEF.
	context->Shutdown();

	// Release objects in reverse order of creation.
	message_loop.reset();
	context.reset();
}
void NplCefBrowserApp::CreateCefSimple(int moduleHandle, int parentHandle, std::string url)
{
	if (url.empty())
	{
		url = "http://www.wikicraft.cn";
	}
	HWND hWnd = (HWND)parentHandle;
	HINSTANCE instance = (HINSTANCE)moduleHandle;

	void* sandbox_info = NULL;

#if defined(CEF_USE_SANDBOX)
	// Manage the life span of the sandbox information object. This is necessary
	// for sandbox support on Windows. See cef_sandbox_win.h for complete details.
	CefScopedSandboxInfo scoped_sandbox;
	sandbox_info = scoped_sandbox.sandbox_info();
#endif

	// Provide CEF with command-line arguments.
	CefMainArgs main_args(instance);
	SimpleApp* s_app = new SimpleApp(hWnd, url, true);
	// SimpleApp implements application-level callbacks for the browser process.
	// It will create the first browser instance in OnContextInitialized() after
	// CEF has initialized.
	CefRefPtr<SimpleApp> app(s_app);

	// Specify CEF global settings here.
	CefSettings settings;
	//settings.multi_threaded_message_loop = true;
	// Specify the path for the sub-process executable.
	CefString(&settings.browser_subprocess_path).FromASCII("cef3/NplCefProcess_d.exe");

#if !defined(CEF_USE_SANDBOX)
	settings.no_sandbox = true;
#endif

	// Initialize CEF.
	bool v = CefInitialize(main_args, settings, app.get(), sandbox_info);
	SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) | WS_CLIPCHILDREN);
	// Run the CEF message loop. This will block until CefQuitMessageLoop() is
	// called.
	CefRunMessageLoop();

	//// Shut down CEF.
	CefShutdown();
}