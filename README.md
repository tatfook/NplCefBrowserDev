# NplCefBrowserDev
### Build NplCefBrowser on Windows
- Set BOOST_ROOT environment
- Download or Git clone [NPLRuntime](https://github.com/LiXizhi/NPLRuntime.git) into npl_packages/NPLRuntime
- Download cef3 binary [Branch 2704-Windows 32bit](https://cefbuilds.com/) and unzip to cef3
- Run build.bat 
- Setp 1: Build libcef_dll_wrapper.lib
- Setp 2: Build NplCefBrowser.dll
- NplCefBrowser.dll will be located at folder:ParaWorld
### Main Folder's structure
```lua
NplCefBrowser
----npl_packages/NPLRuntime
----cef3
----ParaWorld
```
