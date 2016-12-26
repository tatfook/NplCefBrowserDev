#include "NplCefBrowserTask.h"
#include "NplCefBrowser.h"

NplCefBrowserTask::NplCefBrowserTask(TaskTypes type, const std::string& window_id, const std::string& url, bool resize, int x, int y, int width, int height)
	:mID(window_id)
	,mType(type)
	, mUrl(url)
	, mResize(resize)
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
	if (mType == TaskTypes::Open)
	{
		if (!mUrl.empty())
		{
			browser.Open(mID, mUrl, mResize, mX, mY, mWidth, mHeight);
		}
	}
	else if (mType == TaskTypes::Quit)
	{
		browser.Quit();
	}
	else if (mType == TaskTypes::ChangePosSize)
	{
		browser.SetWindowPos(mID, mX, mY, mWidth, mHeight);
	}
	else if (mType == TaskTypes::Show)
	{
		browser.SetVisible(mID,true);
	}
	else if (mType == TaskTypes::Hide)
	{
		browser.SetVisible(mID, false);
	}
	else if (mType == TaskTypes::Delete)
	{
		browser.Delete(mID);
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
