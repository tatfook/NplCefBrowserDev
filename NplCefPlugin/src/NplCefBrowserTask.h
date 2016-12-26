#pragma once
#include "stdafx.h"
#include "include/cef_task.h"
class NplCefBrowserTask :public CefTask {
public:
	enum class TaskTypes {
		None, Open, ChangePosSize, Delete, Show, Hide, Quit
	};
public:
	NplCefBrowserTask(TaskTypes type = TaskTypes::None, const std::string& window_id = "", const std::string& url = "", bool resize = false, int x = 0, int y = 0, int width = 0, int height = 0);
	~NplCefBrowserTask();
	void Execute() OVERRIDE;

	void AddRef() const OVERRIDE;
	bool Release() const OVERRIDE;
	bool HasOneRef() const OVERRIDE;

private:
	TaskTypes mType;
	std::string mID;
	std::string mUrl;
	bool mResize;
	int mX;
	int mY;
	int mWidth;
	int mHeight;
};