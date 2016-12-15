#include "NplCefBrowser.h"

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

#include "NplCefBrowserTask.h"
using namespace client;

NplCefBrowser::NplCefBrowser()
	:mStart(false)
{

}

NplCefBrowser::~NplCefBrowser()
{

}

NplCefBrowser& NplCefBrowser::CreateGetSingleton()
{
	static NplCefBrowser g_singleton;
	return g_singleton;
}

bool NplCefBrowser::IsStart()
{
	return mStart;
}

void NplCefBrowser::DoStart(std::string subProcessName, int parentHandle, std::string url, bool showTitleBar, int x, int y, int width, int height)
{
	if (mStart)
	{
		return;
	}
	HWND hWnd = (HWND)parentHandle;

	mParentHandle = hWnd;

	CefMainArgs main_args(NULL);

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
	CefString(&settings.browser_subprocess_path).FromASCII(subProcessName.c_str());

#if !defined(CEF_USE_SANDBOX)
	settings.no_sandbox = true;
#endif

	// Populate the settings based on command line arguments.
	context->PopulateSettings(&settings);
	settings.multi_threaded_message_loop = true;
	// Create the main message loop object.
	mMessageLoop = new MainMessageLoopMultithreadedWin;
	scoped_ptr<MainMessageLoop> message_loop(mMessageLoop);

	// Initialize CEF.
	context->Initialize(main_args, settings, app, sandbox_info);

	// Register scheme handlers.
	test_runner::RegisterSchemeHandlers();
	context->GetRootWindowManager()->SetParentHandle(hWnd);
	//context->GetRootWindowManager()->SetShowTitleBar(showTitleBar);
	// Create the first window.
	if (!url.empty())
	{
		CefRect rect(x, y, width, height);
		context->GetRootWindowManager()->CreateRootWindow(
			!command_line->HasSwitch(switches::kHideControls),  // Show controls.
			settings.windowless_rendering_enabled ? true : false,
			rect,   // windows size.
			url);   // Use default URL.
	}
	mStart = true;
	// Run the message loop. This will block until Quit() is called by the
	// RootWindowManager after all windows have been destroyed.
	int result = message_loop->Run();

	// Shut down CEF.
	context->Shutdown();

	// Release objects in reverse order of creation.
	message_loop.reset();
	context.reset();
}

void NplCefBrowser::DoEnd()
{
	mStart = false;
}

void NplCefBrowser::Open(std::string url, int x, int y, int width, int height)
{
	if (!mStart)
	{
		return;
	}
	if (!url.empty())
	{
		CefRect rect(x, y, width, height);
		RootWindowManager* m = MainContext::Get()->GetRootWindowManager();
		m->CreateRootWindow(
			true,
			false,
			rect,   // windows size.
			url);   // Use default URL.
	}
}

void NplCefBrowser::PostTask(NplCefBrowserTask* task)
{
	CefRefPtr<NplCefBrowserTask> t(task);
	mMessageLoop->PostTask(t);
}
