#pragma once

#ifdef WIN32
#	ifndef _CRT_SECURE_NO_WARNINGS
#		define _CRT_SECURE_NO_WARNINGS
#	endif
#endif


// ParaEngine includes
#include "PluginAPI.h"
#include <memory>


class NplCefBrowser
{
public:
	NplCefBrowser();
	~NplCefBrowser();


	static NplCefBrowser& CreateGetSingleton();
    void Create(int parentHandle,std::string url);
};