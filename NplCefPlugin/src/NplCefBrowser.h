//#pragma once
//#include "stdafx.h"
//namespace client {
//	class MainMessageLoopMultithreadedWin;
//}
//class NplCefBrowserTask;
//class NplCefBrowser {
//public:
//	NplCefBrowser();
//	~NplCefBrowser();
//	static NplCefBrowser& CreateGetSingleton();
//	bool IsStart();
//	void DoStart(std::string subProcessName, int parentHandle, std::string url, bool showTitleBar, int x, int y, int width, int height);
//	void DoEnd();
//
//	void Open(std::string url, int x, int y, int width, int height);
//
//	void PostTask(NplCefBrowserTask* task);
//private:
//	bool mStart;
//	HWND mParentHandle;
//	client::MainMessageLoopMultithreadedWin* mMessageLoop;
//};