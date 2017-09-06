# NplCefBrowserDev
### Build NplCefBrowser on Windows
- Set BOOST_ROOT environment
- Create a folder which named "NCefWorkSpace"
- Download [NPLRuntime](https://github.com/LiXizhi/NPLRuntime.git) and upzip it into [NCefWorkSpace/NPLRuntime]
- Download [NplCefBrowserDev](https://github.com/tatfook/NplCefBrowserDev.git) and upzip it into [NCefWorkSpace/NplCefBrowserDev]
- Download cef3 binary [Branch 2704-Windows 32bit](https://cefbuilds.com/) and unzip into:[NCefWorkSpace/NplCefBrowserDev/deps/cef3]
- Build NPLRumtime(Optional):Please see [NCefWorkSpace/NPLRuntime/build_win32.bat]
- Run NCefWorkSpace/NplCefBrowserDev/create-solution.bat
- Msbuild deps/cef3/sln/Win32/cef.sln
- Msbuild sln/Win32/NplCefBrowser.sln
- After build successfully, all binary files will locate at: NCefWorkSpace/NPLRuntime/ParaWorld/cef3

### Main Folder's structure
```lua
NCefWorkSpace
        NPLRuntime
                ParaWorld
                        cef3
                                NplCefProcess.exe
                                NplCefPlugin.dll
                                libcef.dll
        NplCefBrowser
                deps/cef3
```
```lua
--test code
local id = "test_win";
local dll_name = "cef3/NplCefPlugin_d.dll";
local subProcessName = "cef3/NplCefProcess_d.exe";
local parentHandle = ParaEngine.GetAttributeObject():GetField("AppHWND", 0);
NPL.activate(dll_name,{ cmd = "Start", subProcessName = subProcessName, parentHandle = parentHandle, showTitleBar = false, }); 
NPL.activate(dll_name,{ cmd = "Open", id = id, url = "http://www.wikicraft.cn/", withControl = false, x = 0, y = 0, width = 400, height = 300, }); 
NPL.activate(dll_name,{ cmd = "ChangePosSize", id = id, x = 100, y = 100, width = 200, height = 200, }); 
NPL.activate(dll_name,{ cmd = "Show", id = id, visible = false, }); 
NPL.activate(dll_name,{ cmd = "Delete", id = id, }); 
NPL.activate(dll_name,{ cmd = "Quit"}); 
```
```lua
deps/cef3/include/cef_v8.h
static CefRefPtr<CefV8Value> CreateInt(int value); --int instead of int32
static CefRefPtr<CefV8Value> CreateUInt(unsigned int value); --unsigned int intead of uint32
```
