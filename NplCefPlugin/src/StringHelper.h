#pragma once
#include "stdafx.h"
namespace NplCefBrowser {
	class StringHelper {
	public:
		static const WCHAR* MultiByteToWideChar(const char* name, unsigned int nCodePage = 0);
		static const char* WideCharToMultiByte(const WCHAR* name, unsigned int nCodePage = 0);
	};
}
