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
	, mWithControl(true)
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

void NplCefBrowser::DoStart(std::string subProcessName, int parentHandle, std::string winID, std::string url, bool showTitleBar, bool withControl, int x, int y, int width, int height)
{
	if (mStart)
	{
		return;
	}
	HWND hWnd = (HWND)parentHandle;

	mParentHandle = hWnd;
	mWithControl = withControl;
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
	command_line->AppendArgument("disable-extensions");
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
	context->GetRootWindowManager()->SetShowTitleBar(showTitleBar);
	mStart = true;

	// Create the first window.
	if (!url.empty())
	{
		Open(winID, url, true, x, y, width, height);
	}
	// Run the message loop. This will block until Quit() is called by the
	// RootWindowManager after all windows have been destroyed.
	int result = message_loop->Run();

	// Shut down CEF.
	context->Shutdown();

	// Release objects in reverse order of creation.
	message_loop.reset();
	context.reset();
	DoEnd();
}

void NplCefBrowser::DoEnd()
{
	mStart = false;
}

void NplCefBrowser::Open(std::string winID, std::string url, bool resize, int x, int y, int width, int height)
{
	if (!mStart)
	{
		return;
	}
	// we need a id for create root window
	if (winID.empty())
	{
		return;
	}
	RootWindowPtr p = GetRootWindow(winID);

	if (p == NULL)
	{
		CefRect rect(x, y, width, height);
		RootWindowManager* m = MainContext::Get()->GetRootWindowManager();
		p = m->CreateRootWindow(
			mWithControl,
			false,
			rect,   // windows size.
			url);   // Use default URL.
		AddRootWindow(winID, p);
	}else 
	{
		p->GetBrowser().get()->GetMainFrame().get()->LoadURL(url);
		if (resize)
		{
			p->SetBounds(x, y, width, height);
		}
	}
	
}

void NplCefBrowser::SetWindowPos(std::string winID, int x, int y, int width, int height)
{
	RootWindowPtr p = GetRootWindow(winID);
	if (p)
	{
		p->SetBounds(x, y, width, height);
	}
	
}

void NplCefBrowser::SetVisible(std::string winID, bool visible)
{
	RootWindowPtr p = GetRootWindow(winID);
	if (p)
	{
		if (visible)
		{
			p->Show(RootWindow::ShowMode::ShowNormal);
		}
		else
		{
			p->Hide();
		}
	}
}

void NplCefBrowser::Delete(std::string winID)
{
	RootWindowPtr p = GetRootWindow(winID);
	if (p)
	{
		p->Close(true);
		DeleteRootWindow(winID);
	}
}

void NplCefBrowser::Quit()
{

	RootWindowManager* m = MainContext::Get()->GetRootWindowManager();
	if (m)
	{
		m->CloseAllWindows(true);
	}
}

void NplCefBrowser::PostTask(NplCefBrowserTask* task)
{
	CefRefPtr<NplCefBrowserTask> t(task);
	mMessageLoop->PostTask(t);
}

RootWindowPtr NplCefBrowser::GetRootWindow(std::string& id)
{
	if (id.empty())
	{
		return RootWindowPtr();
	}
	std::map<std::string, RootWindowPtr>::iterator it = mRootWindows.find(id);
	if (it != mRootWindows.end())
	{
		return it->second;
	}
	else {
		return RootWindowPtr();
	}
}

void NplCefBrowser::AddRootWindow(std::string& id, RootWindowPtr pWindow)
{
	RootWindowPtr p = GetRootWindow(id);
	if (pWindow == NULL || p != NULL)
	{
		return;
	}
	mRootWindows[id] = pWindow;
}

void NplCefBrowser::DeleteRootWindow(std::string& id)
{
	std::map<std::string, RootWindowPtr>::iterator it = mRootWindows.find(id);
	mRootWindows.erase(it);
}
