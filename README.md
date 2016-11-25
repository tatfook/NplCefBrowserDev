# NplCefBrowserDev
### Build NplCefBrowser on Windows
- Set BOOST_ROOT environment
- If you haven't NPLRuntime please download it from [NPLRuntime](https://github.com/LiXizhi/NPLRuntime.git)
- Download cef3 binary [Branch 2704-Windows 32bit](https://cefbuilds.com/) and unzip into:[deps/cef3]
- Open Developer Command Prompt for VS2015 and run build-debug.bat or build-release.bat  
- Setp 1: Build libcef_dll_wrapper.lib
- Setp 2: Build NplCefBrowser.dll
- NplCefBrowser.dll will be located at folder: ../NPLRuntime/ParaWorld

### Main Folder's structure
```lua
NPLRuntime
        ----ParaWorld
NplCefBrowser
        ----deps/NPLRuntime
        ----deps/cef3
```
