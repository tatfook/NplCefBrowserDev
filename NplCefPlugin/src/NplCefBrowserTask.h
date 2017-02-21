#pragma once
#include "stdafx.h"
#include "include/base/cef_macros.h"
#include "include/cef_task.h"
#include "NplCefBrowser.h"
class NplCefBrowserTask :public CefTask {
public:
	NplCefBrowserTask(NplCefBrowser::TaskTypes type, NplCefBrowser::BrowserParams& params);
	void Execute() OVERRIDE;

private:
	NplCefBrowser::TaskTypes mType;
	NplCefBrowser::BrowserParams mParams;


	IMPLEMENT_REFCOUNTING(NplCefBrowserTask);
	DISALLOW_COPY_AND_ASSIGN(NplCefBrowserTask);
};