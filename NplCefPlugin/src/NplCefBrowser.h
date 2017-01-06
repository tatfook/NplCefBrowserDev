#pragma once
#include "stdafx.h"
#include "include/base/cef_scoped_ptr.h"
#include "cefclient/browser/root_window_win.h"

namespace client {
	class MainMessageLoopMultithreadedWin;
}
class NplCefBrowserTask;
typedef scoped_refptr<client::RootWindow> RootWindowPtr;

class NplCefBrowser {
public:
	NplCefBrowser();
	~NplCefBrowser();
	enum class TaskTypes {
		None, Start, Open, ChangePosSize, Delete, Show, EnableWindow, Quit
	};
	struct BrowserParams
	{
		std::string subProcessName;
		int parentHandle;
		std::string id;
		std::string url;
		bool showTitleBar = false;
		bool withControl = false;
		int x = 0;
		int y = 0;
		int width = 800;
		int height = 600;
		bool visible = true;
		bool resize = true;
		bool enabled = true;
	};
	void DoTask(TaskTypes type, BrowserParams& params);
	static NplCefBrowser& CreateGetSingleton();
	bool IsStart();
	void PostTask(NplCefBrowserTask* task);
private:
	void Start(BrowserParams& params);
	void End();
	void Open(BrowserParams& params);
	void ChangePosSize(BrowserParams& params);
	void Show(BrowserParams& params);
	void Delete(BrowserParams& params);
	void Quit();
	void EnableWindow(BrowserParams& params);

	RootWindowPtr GetRootWindow(std::string& id);
	void AddRootWindow(std::string& id, RootWindowPtr pWindow);
	void DeleteRootWindow(std::string& id);

private:
	bool mStart;
	client::MainMessageLoopMultithreadedWin* mMessageLoop;
	std::map<std::string, RootWindowPtr> mRootWindows;
};