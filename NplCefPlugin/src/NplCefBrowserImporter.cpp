#include "stdafx.h"

#include <string>
#include <thread>
#include <sstream>
#include <fstream>
#include "include/cef_browser.h"
#include "include/cef_command_line.h"
#include "include/cef_request_context.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_helpers.h"

#include "Core/INPLRuntimeState.h"
#include "Core/NPLInterface.hpp"
#include "NplCefBrowser.h"
#include "NplCefBrowserTask.h"
using namespace ParaEngine;



#pragma region PE_DLL 

#ifdef WIN32
#define CORE_EXPORT_DECL    __declspec(dllexport)
#else
#define CORE_EXPORT_DECL
#endif

// forware declare of exported functions. 
#ifdef __cplusplus
extern "C" {
#endif
	CORE_EXPORT_DECL const char* LibDescription();
	CORE_EXPORT_DECL int LibNumberClasses();
	CORE_EXPORT_DECL unsigned long LibVersion();
	CORE_EXPORT_DECL ParaEngine::ClassDescriptor* LibClassDesc(int i);
	CORE_EXPORT_DECL void LibInit();
	CORE_EXPORT_DECL void LibActivate(int nType, void* pVoid);
#ifdef __cplusplus
}   /* extern "C" */
#endif

HINSTANCE Instance = NULL;



ClassDescriptor* NplCefBrowser_GetClassDesc();
typedef ClassDescriptor* (*GetClassDescMethod)();

GetClassDescMethod Plugins[] =
{
	NplCefBrowser_GetClassDesc,
};

#define NplCefBrowser_CLASS_ID Class_ID(0x3b305a29, 0x47a409ce)

class NplCefBrowserDesc :public ClassDescriptor
{
public:

	void *	Create(bool loading)
	{
		return NULL;
	}
	const char* ClassName()
	{
		return "INplCefBrowser";
	}

	SClass_ID SuperClassID()
	{
		return OBJECT_MODIFIER_CLASS_ID;
	}

	Class_ID ClassID()
	{
		return NplCefBrowser_CLASS_ID;
	}

	const char* Category()
	{
		return "NplCefBrowser Category";
	}

	const char* InternalName()
	{
		return "NplCefBrowser InternalName";
	}

	HINSTANCE HInstance()
	{
		extern HINSTANCE Instance;
		return Instance;
	}
};

ClassDescriptor* NplCefBrowser_GetClassDesc()
{
	static NplCefBrowserDesc s_desc;
	return &s_desc;
}

CORE_EXPORT_DECL const char* LibDescription()
{
	return "ParaEngine NplCefBrowserImporter Ver 1.0.0";
}

CORE_EXPORT_DECL unsigned long LibVersion()
{
	return 1;
}

CORE_EXPORT_DECL int LibNumberClasses()
{
	return sizeof(Plugins) / sizeof(Plugins[0]);
}

CORE_EXPORT_DECL ClassDescriptor* LibClassDesc(int i)
{
	if (i < LibNumberClasses() && Plugins[i])
	{
		return Plugins[i]();
	}
	else
	{
		return NULL;
	}
}

CORE_EXPORT_DECL void LibInit()
{
}

#ifdef WIN32
BOOL WINAPI DllMain(HINSTANCE hinstDLL, ULONG fdwReason, LPVOID lpvReserved)
#else
void __attribute__((constructor)) DllMain()
#endif
{
	// TODO: dll start up code here
#ifdef WIN32
	Instance = hinstDLL;				// Hang on to this DLL's instance handle.
	return (TRUE);
#endif
}
#pragma endregion PE_DLL 
void DoStart(std::string subProcessName, int parentHandle, std::string window_id, std::string url, bool showTitleBar, bool withControl, int x, int y, int width, int height)
{
	NplCefBrowser& browser = NplCefBrowser::CreateGetSingleton();
	if (&browser)
	{
		browser.DoStart(subProcessName, parentHandle, window_id, url, showTitleBar, withControl, x, y, width, height);
	}
}
CORE_EXPORT_DECL void LibActivate(int nType, void* pVoid)
{
	if (nType == ParaEngine::PluginActType_STATE)
	{
		NPL::INPLRuntimeState* pState = (NPL::INPLRuntimeState*)pVoid;
		const char* sMsg = pState->GetCurrentMsg();
		int nMsgLength = pState->GetCurrentMsgLength();

		NPLInterface::NPLObjectProxy tabMsg = NPLInterface::NPLHelper::MsgStringToNPLTable(sMsg);
		const std::string& sCmd = tabMsg["cmd"];

		const std::string& subProcessName = tabMsg["subProcessName"];
		double parentHandle = tabMsg["parentHandle"];
		const std::string& id = tabMsg["id"];
		const std::string& url = tabMsg["url"];
		bool showTitleBar = tabMsg["showTitleBar"];
		bool withControl = tabMsg["withControl"];
		bool resize = tabMsg["resize"];
		bool visible = tabMsg["visible"];
		double x = tabMsg["x"];
		double y = tabMsg["y"];
		double width = tabMsg["width"];
		double height = tabMsg["height"];

		OUTPUT_LOG("NplCefBrowser activate:%s", sCmd.c_str());

		NplCefBrowser& browser = NplCefBrowser::CreateGetSingleton();

		if (sCmd == "CreateOrOpen")
		{
			if (!browser.IsStart())
			{
				std::thread t(DoStart, subProcessName, parentHandle, id, url, showTitleBar, withControl, x, y, width, height);
				t.detach();
			}
			else
			{
				NplCefBrowserTask* task = new NplCefBrowserTask(NplCefBrowserTask::TaskTypes::Open, id, url, resize, x, y, width, height);
				browser.PostTask(task);
			}
		}
		else if (sCmd == "Quit")
		{
			NplCefBrowserTask* task = new NplCefBrowserTask(NplCefBrowserTask::TaskTypes::Quit, id);
			browser.PostTask(task);
		}
		else if (sCmd == "ChangePosSize")
		{
			NplCefBrowserTask* task = new NplCefBrowserTask(NplCefBrowserTask::TaskTypes::ChangePosSize, id, url, resize, x, y, width, height);
			browser.PostTask(task);
		}
		else if (sCmd == "Show")
		{
			if (visible)
			{
				NplCefBrowserTask* task = new NplCefBrowserTask(NplCefBrowserTask::TaskTypes::Show, id);
				browser.PostTask(task);
			}
			else
			{
				NplCefBrowserTask* task = new NplCefBrowserTask(NplCefBrowserTask::TaskTypes::Hide, id);
				browser.PostTask(task);
			}
			
		}
		else if (sCmd == "Delete")
		{
			NplCefBrowserTask* task = new NplCefBrowserTask(NplCefBrowserTask::TaskTypes::Delete, id);
			browser.PostTask(task);
		}
	}
}



