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
- Setp 2: Build NplCefBrowser.dll
- With debug    mode these libs will be built at NCefWorkSpace/NPLRuntime/ParaWorld: NplCefBrowser_d.dll NplCefBrowser_d.pdb libcef.dll
- With release  mode these libs will be built at NCefWorkSpace/NPLRuntime/ParaWorld: NplCefBrowser.dll NplCefBrowser.pdb libcef.dll

### Main Folder's structure
```lua
NCefWorkSpace
        NPLRuntime
                ParaWorld
                        NplCefBrowser.dll
                        NplCefBrowser.pdb
                        libcef.dll
        NplCefBrowser
                deps/cef3
```
```lua
--test code
local engine_attr = ParaEngine.GetAttributeObject();
local parentHandle = engine_attr:GetField("AppHWND", 0);
local dll_name = "NplCefBrowser_d.dll";--debug mode
NPL.activate(dll_name,{cmd = "create", parentHandle = parentHandle, url = "http://www.wikicraft.cn/"}); 
```
