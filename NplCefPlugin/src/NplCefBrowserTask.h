#pragma once
#include "stdafx.h"
#include "include/cef_task.h"
class NplCefBrowserTask :public CefTask {
public:
	enum class TaskTypes {
		Open
	};
public:
	NplCefBrowserTask(TaskTypes type = TaskTypes::Open, const std::string& url = "", int x = 0, int y = 0, int width = 0, int height = 0);
	~NplCefBrowserTask();
	void Execute() OVERRIDE;

	void AddRef() const OVERRIDE;
	bool Release() const OVERRIDE;
	bool HasOneRef() const OVERRIDE;

private:
	TaskTypes mType;
	std::string mUrl;
	int mX;
	int mY;
	int mWidth;
	int mHeight;
};