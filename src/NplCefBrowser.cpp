#include "NplCefBrowser.h"

#include <string>
#include <sstream>
#include <fstream>
#include "Core/INPLRuntimeState.h"
#include "Core/NPLInterface.hpp"
#include "NplCefBrowserHandler.h"
#include "include/cef_browser.h"
#include "include/cef_command_line.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_helpers.h"

using namespace ParaEngine;



#pragma region PE_DLL 

#ifdef WIN32
#define CORE_EXPORT_DECL    __declspec(dllexport)
#else
#define CORE_EXPORT_DECL
#endif

// forware declare of exported functions. 
#ifdef __cplusplus
extern "C" {
#endif
	CORE_EXPORT_DECL const char* LibDescription();
	CORE_EXPORT_DECL int LibNumberClasses();
	CORE_EXPORT_DECL unsigned long LibVersion();
	CORE_EXPORT_DECL ParaEngine::ClassDescriptor* LibClassDesc(int i);
	CORE_EXPORT_DECL void LibInit();
	CORE_EXPORT_DECL void LibActivate(int nType, void* pVoid);
#ifdef __cplusplus
}   /* extern "C" */
#endif

HINSTANCE Instance = NULL;



ClassDescriptor* NplCefBrowser_GetClassDesc();
typedef ClassDescriptor* (*GetClassDescMethod)();

GetClassDescMethod Plugins[] =
{
	NplCefBrowser_GetClassDesc,
};

#define MCImporter_CLASS_ID Class_ID(0x2b305a29, 0x47a409ce)

class NplCefBrowserDesc :public ClassDescriptor
{
public:
	void* Create(bool loading = FALSE)
	{
		return new NplCefBrowser();
	}

	const char* ClassName()
	{
		return "INplCefBrowser";
	}

	SClass_ID SuperClassID()
	{
		return OBJECT_MODIFIER_CLASS_ID;
	}

	Class_ID ClassID()
	{
		return MCImporter_CLASS_ID;
	}

	const char* Category()
	{
		return "NplCefBrowser Category";
	}

	const char* InternalName()
	{
		return "NplCefBrowser InternalName";
	}

	HINSTANCE HInstance()
	{
		extern HINSTANCE Instance;
		return Instance;
	}
};

ClassDescriptor* NplCefBrowser_GetClassDesc()
{
	static NplCefBrowserDesc s_desc;
	return &s_desc;
}

CORE_EXPORT_DECL const char* LibDescription()
{
	return "ParaEngine MCImporter Ver 1.0.0";
}

CORE_EXPORT_DECL unsigned long LibVersion()
{
	return 1;
}

CORE_EXPORT_DECL int LibNumberClasses()
{
	return sizeof(Plugins) / sizeof(Plugins[0]);
}

CORE_EXPORT_DECL ClassDescriptor* LibClassDesc(int i)
{
	if (i < LibNumberClasses() && Plugins[i])
	{
		return Plugins[i]();
	}
	else
	{
		return NULL;
	}
}

CORE_EXPORT_DECL void LibInit()
{
}

#ifdef WIN32
BOOL WINAPI DllMain(HINSTANCE hinstDLL, ULONG fdwReason, LPVOID lpvReserved)
#else
void __attribute__((constructor)) DllMain()
#endif
{
	// TODO: dll start up code here
#ifdef WIN32
	Instance = hinstDLL;				// Hang on to this DLL's instance handle.
	return (TRUE);
#endif
}
#pragma endregion PE_DLL 

namespace {

	// When using the Views framework this object provides the delegate
	// implementation for the CefWindow that hosts the Views-based browser.
	class SimpleWindowDelegate : public CefWindowDelegate {
	public:
		explicit SimpleWindowDelegate(CefRefPtr<CefBrowserView> browser_view)
			: browser_view_(browser_view) {
		}

		void OnWindowCreated(CefRefPtr<CefWindow> window) OVERRIDE {
			// Add the browser view and show the window.
			window->AddChildView(browser_view_);
			window->Show();

			// Give keyboard focus to the browser view.
			browser_view_->RequestFocus();
		}

		void OnWindowDestroyed(CefRefPtr<CefWindow> window) OVERRIDE {
			browser_view_ = NULL;
		}

		bool CanClose(CefRefPtr<CefWindow> window) OVERRIDE {
			// Allow the window to close if the browser says it's OK.
			CefRefPtr<CefBrowser> browser = browser_view_->GetBrowser();
			if (browser)
				return browser->GetHost()->TryCloseBrowser();
			return true;
		}

	private:
		CefRefPtr<CefBrowserView> browser_view_;

		IMPLEMENT_REFCOUNTING(SimpleWindowDelegate);
		DISALLOW_COPY_AND_ASSIGN(SimpleWindowDelegate);
	};

}  // namespace

CORE_EXPORT_DECL void LibActivate(int nType, void* pVoid)
{
	if (nType == ParaEngine::PluginActType_STATE)
	{
		NPL::INPLRuntimeState* pState = (NPL::INPLRuntimeState*)pVoid;
		const char* sMsg = pState->GetCurrentMsg();
		int nMsgLength = pState->GetCurrentMsgLength();

		NPLInterface::NPLObjectProxy tabMsg = NPLInterface::NPLHelper::MsgStringToNPLTable(sMsg);
		const std::string& sCmd = tabMsg["cmd"];

		NplCefBrowser& browser = NplCefBrowser::CreateGetSingleton();

		if (sCmd == "create")
		{
			int parentHandle = tabMsg["parentHandle"];
			const std::string& url = tabMsg["url"];
			browser.Create(parentHandle, url);
		}
	}
}

NplCefBrowser::NplCefBrowser()
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

void NplCefBrowser::Create(int parentHandle,std::string url)
{
	//CEF_REQUIRE_UI_THREAD();
	OUTPUT_LOG("NplCefBrowser create: %s\r\n", url.c_str());
	bool use_views = false;
	// NplCefBrowserHandler implements browser-level callbacks.
	CefRefPtr<NplCefBrowserHandler> handler(new NplCefBrowserHandler(use_views));

	// Specify CEF browser settings here.
	CefBrowserSettings browser_settings;

	if (url.empty())
	{
		url = "http://www.wikicraft.cn/";
	}
	if (use_views) {
		// Create the BrowserView.
		CefRefPtr<CefBrowserView> browser_view = CefBrowserView::CreateBrowserView(
			handler, url, browser_settings, NULL, NULL);

		// Create the Window. It will show itself after creation.
		CefWindow::CreateTopLevelWindow(new SimpleWindowDelegate(browser_view));
	}
	else {
		// Information used when creating the native window.
		CefWindowInfo window_info;

		//window_info.SetAsPopup((HWND)parentHandle, "cefsimple");
		RECT windowRect;
		windowRect.left = 0;
		windowRect.top = 0;
		windowRect.right = 0;
		windowRect.bottom = 0;
		window_info.SetAsChild((HWND)parentHandle, windowRect);
		OUTPUT_LOG("window_info.SetAsChild\r\n");

		// Create the first browser window.
		CefBrowserHost::CreateBrowser(window_info, handler, url, browser_settings,
			NULL);
		OUTPUT_LOG("CefBrowserHost::CreateBrowser\r\n");
	}
}

