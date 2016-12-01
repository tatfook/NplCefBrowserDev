#pragma once
#include "stdafx.h"
namespace NplCefBroser {
	class StringHelper {
	public:
		static const WCHAR* MultiByteToWideChar(const char* name, unsigned int nCodePage = 0);
		static const char* WideCharToMultiByte(const WCHAR* name, unsigned int nCodePage = 0);
	};
}
