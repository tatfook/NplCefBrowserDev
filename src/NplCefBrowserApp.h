#pragma once
#include "PluginAPI.h"
#include "include/cef_app.h"

// Implement application-level callbacks for the browser process.
class NplCefBrowserApp: public CefApp,
	public CefBrowserProcessHandler {
public:
	NplCefBrowserApp();

	// CefApp methods:
	virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler()
		OVERRIDE {
		return this;
	}

	// CefBrowserProcessHandler methods:
	virtual void OnContextInitialized() OVERRIDE;

	static NplCefBrowserApp& CreateGetSingleton();
	void Create(int parentHandle, std::string url);

private:
	// Include the default reference counting implementation.
	IMPLEMENT_REFCOUNTING(NplCefBrowserApp);
};
