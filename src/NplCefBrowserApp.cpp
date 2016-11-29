#include "NplCefBrowserApp.h"

#include <string>

#include "NplCefBrowserHandler.h"
#include "include/cef_browser.h"
#include "include/cef_command_line.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_helpers.h"
#include "Core/NPLInterface.hpp"
namespace {

	class WXRequestContextHandler :public CefRequestContextHandler
	{
	public:
		WXRequestContextHandler() {};
		~WXRequestContextHandler() {};
		CefRefPtr<CefCookieManager> GetCookieManager() OVERRIDE {

			return CefCookieManager::CreateManager("F:\\CefCookie", FALSE, NULL);
		}
	private:
		// Include the default reference counting implementation.
		IMPLEMENT_REFCOUNTING(WXRequestContextHandler);
	};

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

NplCefBrowserApp::NplCefBrowserApp() {
}

void NplCefBrowserApp::OnContextInitialized() {
	
}


NplCefBrowserApp& NplCefBrowserApp::CreateGetSingleton()
{
	static NplCefBrowserApp g_singleton;
	return g_singleton;
}

void NplCefBrowserApp::Create(int parentHandle, std::string url)
{
	//CEF_REQUIRE_UI_THREAD();
	OUTPUT_LOG("NplCefBrowser create:parentHandle %d,url %s\r\n", parentHandle, url.c_str());
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

		CefWindowHandle wnd = (CefWindowHandle)parentHandle;
		//window_info.SetAsPopup(wnd, "cefsimple");

		CefRequestContextSettings settings;
		CefRefPtr<CefRequestContext> conctex = CefRequestContext::CreateContext(settings, new WXRequestContextHandler);
		RECT windowRect = { 0, 0, 800, 600 };
		window_info.SetAsChild(wnd, windowRect);
		OUTPUT_LOG("window_info.SetAsChild\r\n");
		// Create the first browser window.
		CefBrowserHost::CreateBrowser(window_info, handler, url, browser_settings, conctex);
		OUTPUT_LOG("CefBrowserHost::CreateBrowser\r\n");
	}
}
