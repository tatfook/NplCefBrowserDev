//#pragma warning( disable : 4201)
#include "PluginAPI.h"
#include "Core/INPLRuntimeState.h"
#include "Core/NPLInterface.hpp"

using namespace ParaEngine;

#include "include/cef_app.h"

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

#define MCImporter_CLASS_ID Class_ID(0x2b305a29, 0x47a409ce)

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
		return MCImporter_CLASS_ID;
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
	return "ParaEngine MCImporter Ver 1.0.0";
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
		OUTPUT_LOG("================dll activated\n");
		CefMainArgs main_args;
		CefExecuteProcess(main_args, NULL, NULL);
	}
}



