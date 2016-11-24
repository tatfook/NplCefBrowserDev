#include "NplCefBrowserHandler.h"

#include <string>
#include <windows.h>

#include "include/cef_browser.h"

void NplCefBrowserHandler::PlatformTitleChange(CefRefPtr<CefBrowser> browser,
                                        const CefString& title) {
  CefWindowHandle hwnd = browser->GetHost()->GetWindowHandle();
  //SetWindowText(hwnd, std::wstring(title).c_str());
}
