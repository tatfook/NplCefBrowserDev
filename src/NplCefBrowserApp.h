#pragma once
#include "stdafx.h"
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
	void Create(int moduleHandle, int parentHandle, std::string url);

private:
	bool MsgProcCustom(UINT message, WPARAM wParam, LPARAM lParam);
	void CreateThread(int moduleHandle, int parentHandle, std::string url);
	void CreateCefClient(int moduleHandle, int parentHandle, std::string url);
	// Include the default reference counting implementation.
	IMPLEMENT_REFCOUNTING(NplCefBrowserApp);
};
