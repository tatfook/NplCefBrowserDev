# NplCefBrowserDev
### Build NplCefBrowser on Windows
- Set BOOST_ROOT environment
- Download [NPLRuntime](https://github.com/LiXizhi/NPLRuntime.git) and unzip into:[deps/NPLRuntime]
- Download cef3 binary [Branch 2704-Windows 32bit](https://cefbuilds.com/) and unzip into:[deps/cef3]
- Open Developer Command Prompt for VS2015 and run build.bat 
- Setp 1: Build libcef_dll_wrapper.lib
- Setp 2: Build NplCefBrowser.dll
- NplCefBrowser.dll will be located at folder: ../ParaWorld
### Main Folder's structure
```lua
        NplCefBrowser
                ----deps/NPLRuntime
                ----deps/cef3
ParaWorld
```
