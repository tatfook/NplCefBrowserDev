#pragma once
#include "stdafx.h"
#include "include/cef_task.h"
#include "NplCefBrowser.h"
class NplCefBrowserTask :public CefTask {
public:
	NplCefBrowserTask(NplCefBrowser::TaskTypes type, NplCefBrowser::BrowserParams& params);
	~NplCefBrowserTask();
	void Execute() OVERRIDE;

	void AddRef() const OVERRIDE;
	bool Release() const OVERRIDE;
	bool HasOneRef() const OVERRIDE;

private:
	NplCefBrowser::TaskTypes mType;
	NplCefBrowser::BrowserParams mParams;
};