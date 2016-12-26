# NplCefBrowserDev
### Build NplCefBrowser on Windows
- Set BOOST_ROOT environment
- Create a folder which named "NCefWorkSpace"
- Download [NPLRuntime](https://github.com/LiXizhi/NPLRuntime.git) and upzip it into [NCefWorkSpace/NPLRuntime]
- Download [NplCefBrowserDev](https://github.com/tatfook/NplCefBrowserDev.git) and upzip it into [NCefWorkSpace/NplCefBrowserDev]
- Download cef3 binary [Branch 2704-Windows 32bit](https://cefbuilds.com/) and unzip into:[NCefWorkSpace/NplCefBrowserDev/deps/cef3]
- Build NPLRumtime(Optional):Please see [NCefWorkSpace/NPLRuntime/build_win32.bat]
- Open Developer Command Prompt for VS2015 and run NCefWorkSpace/NplCefBrowserDev/build-debug.bat or NCefWorkSpace/NplCefBrowserDev/build-release.bat  
- Setp 1: Build libcef_dll_wrapper.lib
- Setp 2: Build NplCefProcess.exe
- Setp 3: Build NplCefPlugin.dll
- After built successfully, all binary files will locate at: NCefWorkSpace/NPLRuntime/ParaWorld/cef3

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
local dll_name = "cef3/NplCefPlugin.dll";
NPL.activate(dll_name,{ cmd = "CreateOrOpen",
                        subProcessName = "cef3/NplCefProcess.exe",
                        parentHandle = ParaEngine.GetAttributeObject():GetField("AppHWND", 0),
                        id = "test_win",
                        url = "http://www.wikicraft.cn/",
                        showTitleBar = false,
                        withControl = false,
                        x = 0, y = 0, width = 400, height = 300, 
}); 
local dll_name = "cef3/NplCefPlugin.dll";
NPL.activate(dll_name,{ cmd = "Quit"}); 

local dll_name = "cef3/NplCefPlugin.dll";
NPL.activate(dll_name,{ cmd = "change_pos_size", id = "test_win", x = 100, y = 100, width = 400, height = 300, }); 

local dll_name = "cef3/NplCefPlugin.dll";
NPL.activate(dll_name,{ cmd = "Show", id = "test_win", visible = false,  }); 

local dll_name = "cef3/NplCefPlugin.dll";
NPL.activate(dll_name,{ cmd = "CreateOrOpen", id = "test_win", url = "http://www.baidu.com/", }); 
```
```lua
deps/cef3/include/cef_v8.h
static CefRefPtr<CefV8Value> CreateInt(int value); --int instead of int32
static CefRefPtr<CefV8Value> CreateUInt(unsigned int value); --unsigned int intead of uint32
```
