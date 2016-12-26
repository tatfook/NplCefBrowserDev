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
	static NplCefBrowser& CreateGetSingleton();
	bool IsStart();
	void DoStart(std::string subProcessName, int parentHandle, std::string winID, std::string url, bool showTitleBar, bool withControl, int x, int y, int width, int height);
	void DoEnd();

	void Open(std::string winID, std::string url, bool resize, int x, int y, int width, int height);
	void SetWindowPos(std::string winID, int x, int y, int width, int height);
	void SetVisible(std::string winID, bool visible);
	void Delete(std::string winID);
	void Quit();


	void PostTask(NplCefBrowserTask* task);
private:
	RootWindowPtr GetRootWindow(std::string& id);
	void AddRootWindow(std::string& id, RootWindowPtr pWindow);
	void DeleteRootWindow(std::string& id);

private:
	bool mStart;
	HWND mParentHandle;
	client::MainMessageLoopMultithreadedWin* mMessageLoop;
	bool mWithControl;
	std::map<std::string, RootWindowPtr> mRootWindows;
};