#include "StringHelper.h"
namespace NplCefBroser
{
	const WCHAR* StringHelper::MultiByteToWideChar(const char* name, unsigned int nCodePage /*= 0*/)
	{
		const std::string str = name;
		if (str.empty()) return std::wstring().c_str();
		int size_needed = ::MultiByteToWideChar(nCodePage, 0, &str[0], (int)str.size(), NULL, 0);
		std::wstring wstrTo(size_needed, 0);
		::MultiByteToWideChar(nCodePage, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
		return wstrTo.c_str();
	}

	const char* StringHelper::WideCharToMultiByte(const WCHAR* name, unsigned int nCodePage /*= 0*/)
	{
		const std::wstring wstr(name);
		if (wstr.empty()) return std::string().c_str();
		int size_needed = ::WideCharToMultiByte(nCodePage, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
		std::string strTo(size_needed, 0);
		::WideCharToMultiByte(nCodePage, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
		return strTo.c_str();
	}
}
