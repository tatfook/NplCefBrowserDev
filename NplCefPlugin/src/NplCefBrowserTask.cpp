#include "NplCefBrowserTask.h"
#include "NplCefBrowser.h"

NplCefBrowserTask::NplCefBrowserTask(NplCefBrowser::TaskTypes type, NplCefBrowser::BrowserParams& params)
	:mType(type)
	,mParams(params)
{
}
void NplCefBrowserTask::Execute()
{
	NplCefBrowser& browser = NplCefBrowser::CreateGetSingleton();
	browser.DoTask(mType, mParams);
}
