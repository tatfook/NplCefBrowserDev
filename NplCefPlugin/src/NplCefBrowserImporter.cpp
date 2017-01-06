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
CORE_EXPORT_DECL void LibActivate(int nType, void* pVoid)
{
	if (nType == ParaEngine::PluginActType_STATE)
	{
		NPL::INPLRuntimeState* pState = (NPL::INPLRuntimeState*)pVoid;
		const char* sMsg = pState->GetCurrentMsg();
		int nMsgLength = pState->GetCurrentMsgLength();

		NPLInterface::NPLObjectProxy tabMsg = NPLInterface::NPLHelper::MsgStringToNPLTable(sMsg);
		const std::string& sCmd = tabMsg["cmd"];

		NplCefBrowser::BrowserParams params;
		params.subProcessName = tabMsg["subProcessName"];
		double parentHandle = tabMsg["parentHandle"];
		params.parentHandle = parentHandle;
		params.id = tabMsg["id"];
		params.url = tabMsg["url"];
		params.showTitleBar = tabMsg["showTitleBar"];
		params.withControl = tabMsg["withControl"];
		params.resize = tabMsg["resize"];
		params.visible = tabMsg["visible"];
		params.enabled = tabMsg["enabled"];
		double x = tabMsg["x"];
		double y = tabMsg["y"];
		double width = tabMsg["width"];
		double height = tabMsg["height"];
		params.x = x;
		params.y = y;
		params.width = width;
		params.height = height;

		OUTPUT_LOG("NplCefBrowser activate:%s\n", sCmd.c_str());

		NplCefBrowser& browser = NplCefBrowser::CreateGetSingleton();

		if (sCmd == "Start")
		{
			if (!browser.IsStart())
			{
				std::thread t(&NplCefBrowser::DoTask, &browser, NplCefBrowser::TaskTypes::Start, params);
				t.detach();
			}
		}
		else if (sCmd == "Open")
		{
			NplCefBrowserTask* task = new NplCefBrowserTask(NplCefBrowser::TaskTypes::Open, params);
			browser.PostTask(task);
		}
		else if (sCmd == "ChangePosSize")
		{
			NplCefBrowserTask* task = new NplCefBrowserTask(NplCefBrowser::TaskTypes::ChangePosSize, params);
			browser.PostTask(task);
		}
		else if (sCmd == "Delete")
		{
			NplCefBrowserTask* task = new NplCefBrowserTask(NplCefBrowser::TaskTypes::Delete, params);
			browser.PostTask(task);
		}
		else if (sCmd == "Show")
		{
			NplCefBrowserTask* task = new NplCefBrowserTask(NplCefBrowser::TaskTypes::Show, params);
			browser.PostTask(task);
		}
		else if (sCmd == "EnableWindow")
		{
			NplCefBrowserTask* task = new NplCefBrowserTask(NplCefBrowser::TaskTypes::EnableWindow, params);
			browser.PostTask(task);
		}
		else if (sCmd == "Quit")
		{
			NplCefBrowserTask* task = new NplCefBrowserTask(NplCefBrowser::TaskTypes::Quit, params);
			browser.PostTask(task);
		}
		
		
	}
}



