#include "NplCefBrowserTask.h"
#include "NplCefBrowser.h"

NplCefBrowserTask::NplCefBrowserTask(NplCefBrowser::TaskTypes type, NplCefBrowser::BrowserParams& params)
	:mType(type)
	,mParams(params)
{

}

NplCefBrowserTask::~NplCefBrowserTask()
{

}

void NplCefBrowserTask::Execute()
{
	NplCefBrowser& browser = NplCefBrowser::CreateGetSingleton();
	browser.DoTask(mType, mParams);
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
