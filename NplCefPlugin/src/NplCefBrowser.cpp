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

NplCefBrowser* NplCefBrowser::m_pInstance = nullptr;
NplCefBrowser::NplCefBrowser()
	:mStart(false)
{

}

NplCefBrowser::~NplCefBrowser()
{
}

NplCefBrowser* NplCefBrowser::CreateGetSingleton()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new NplCefBrowser();
	}
	return m_pInstance;
}

bool NplCefBrowser::IsStart()
{
	return mStart;
}

void NplCefBrowser::Start(BrowserParams& params)
{
	if (mStart)
	{
		return;
	}
	std::string subProcessName = params.subProcessName;
	HWND hWnd = (HWND)params.parentHandle;
	bool showTitleBar = params.showTitleBar;

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
	m_context.reset(new MainContextImpl(command_line, true));
	CefSettings settings;
	// Specify the path for the sub-process executable.
	CefString(&settings.browser_subprocess_path).FromASCII(subProcessName.c_str());

#if !defined(CEF_USE_SANDBOX)
	//settings.no_sandbox = true;
#endif

	// Populate the settings based on command line arguments.
	m_context->PopulateSettings(&settings);
	settings.multi_threaded_message_loop = true;
	// Create the main message loop object.
	m_message_loop.reset(new MainMessageLoopMultithreadedWin);
	// Initialize CEF.
	m_context->Initialize(main_args, settings, app, sandbox_info);

	// Register scheme handlers.
	test_runner::RegisterSchemeHandlers();
	m_context->GetRootWindowManager()->SetParentHandle(hWnd);
	m_context->GetRootWindowManager()->SetShowTitleBar(showTitleBar);
	mStart = true;
	// We need a internal browser to block the termination of m_message_loop,when others browsers had been destroyed.
	BrowserParams first_browser_params;
	first_browser_params.id = "first_browser";
	first_browser_params.withControl = false;
	first_browser_params.url = "";
	first_browser_params.width = 0;
	first_browser_params.height = 0;
	Open(first_browser_params);
	// Run the message loop. This will block until Quit() is called by the
	// RootWindowManager after all windows have been destroyed.
	int result = m_message_loop->Run();

	// Shut down CEF.
	m_context->Shutdown();

	// Release objects in reverse order of creation.
	m_message_loop.reset();
	m_context.reset();

	End();
}

void NplCefBrowser::End()
{
	mStart = false;
}

void NplCefBrowser::Open(BrowserParams& params)
{
	if (!mStart)
	{
		return;
	}
	std::string id = params.id;
	std::string url = params.url;
	bool resize = params.resize;
	bool visible = params.visible;
	int x = params.x;
	int y = params.y;
	int width = params.width;
	int height = params.height;
	int withControl = params.withControl;
	// we need a id for create root window
	if (id.empty())
	{
		return;
	}
	RootWindowPtr p = GetRootWindow(id);

	if (p.get() == NULL)
	{
		CefRect rect(x, y, width, height);
		RootWindowManager* m = MainContext::Get()->GetRootWindowManager();
		p = m->CreateRootWindow(
			withControl,
			false,
			rect,   // windows size.
			url);   // Use default URL.
		AddRootWindow(id, p);
	}else 
	{
		CefBrowser* b = p->GetBrowser().get();
		if (b)
		{
			b->GetMainFrame().get()->LoadURL(url);
			if (resize)
			{
				p->SetBounds(x, y, width, height);
			}
			if (visible)
			{
				p->Show(RootWindow::ShowMode::ShowNormal);
			}
		}
		
	}
	
}


void NplCefBrowser::ChangePosSize(BrowserParams& params)
{
	RootWindowPtr p = GetRootWindow(params.id);
	if (p.get())
	{
		CefBrowser* b = p->GetBrowser().get();
		if (b)
		{
			p->SetBounds(params.x, params.y, params.width, params.height);
		}
	}
	
}

void NplCefBrowser::Show(BrowserParams& params)
{
	RootWindowPtr p = GetRootWindow(params.id);
	if (p.get())
	{
		CefBrowser* b = p->GetBrowser().get();
		if (b)
		{
			if (params.visible)
			{
				p->Show(RootWindow::ShowMode::ShowNormal);
			}
			else
			{
				p->Hide();
			}

			//HWND hWnd = p->GetWindowHandle();
			////Using LayeredWindowAttributes https://msdn.microsoft.com/en-us/magazine/ee819134.aspx
			////https://msdn.microsoft.com/en-us/library/windows/desktop/ms632598(v=vs.85).aspx#layered_win
			//SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
			//bool result = SetLayeredWindowAttributes(hWnd, RGB(255, 255, 255), 0, LWA_COLORKEY);
			//if (!result)
			//{

			//	OUTPUT_LOG("error: failed to SetLayeredWindowAttributes on windows. error code is %d\n", GetLastError());
			//}
		}
		
	}
}

void NplCefBrowser::Delete(BrowserParams& params)
{
	RootWindowPtr p = GetRootWindow(params.id);
	if (p.get())
	{
		CefBrowser* b = p->GetBrowser().get();
		if (b)
		{
			p->Close(true);
			DeleteRootWindow(params.id);
		}
		
	}
}

void NplCefBrowser::Quit()
{

	RootWindowManager* m = MainContext::Get()->GetRootWindowManager();
	if (m)
	{
		m->CloseAllWindows(true);
	}
	mRootWindows.clear();
}

void NplCefBrowser::EnableWindow(BrowserParams& params)
{
	RootWindowPtr p = GetRootWindow(params.id);
	if (p.get())
	{
		CefBrowser* b = p->GetBrowser().get();
		if (b)
		{
			::EnableWindow(p->GetWindowHandle(), params.enabled);
		}

	}
}

void NplCefBrowser::DoTask(TaskTypes type, BrowserParams& params)
{
	if (type == TaskTypes::Start)
	{
		Start(params);
	}
	else if (type == TaskTypes::Open)
	{
		Open(params);
	}
	else if (type == TaskTypes::ChangePosSize)
	{
		ChangePosSize(params);
	}
	else if (type == TaskTypes::Delete)
	{
		Delete(params);
	}
	else if (type == TaskTypes::Show)
	{
		Show(params);
	}
	else if (type == TaskTypes::EnableWindow)
	{
		EnableWindow(params);
	}
	else if (type == TaskTypes::Quit)
	{
		Quit();
	}
}

void NplCefBrowser::PostTask(NplCefBrowserTask* task)
{
	CefRefPtr<NplCefBrowserTask> t(task);
	m_message_loop->PostTask(t);
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
	if (pWindow.get() == NULL || p.get() != NULL)
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
