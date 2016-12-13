#include "NplCefBrowserTask.h"
#include "NplCefBrowser.h"

NplCefBrowserTask::NplCefBrowserTask(TaskTypes type, const std::string& url, int x, int y, int width, int height)
	:mType(type)
	, mUrl(url)
	, mX(x)
	, mY(y)
	, mWidth(width)
	, mHeight(height)
{

}

NplCefBrowserTask::~NplCefBrowserTask()
{

}

void NplCefBrowserTask::Execute()
{
	NplCefBrowser& browser = NplCefBrowser::CreateGetSingleton();
	if (!mUrl.empty())
	{
		browser.Open(mUrl, mX, mY, mWidth, mHeight);
	}
}

void NplCefBrowserTask::AddRef() const
{

}

bool NplCefBrowserTask::Release() const
{
	return true;
}

bool NplCefBrowserTask::HasOneRef() const
{
	return false;
}
