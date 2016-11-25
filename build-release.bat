@echo off 
rem Setp 1: Build libcef_dll_wrapper.lib
if not exist "deps/cef3/bin/Win32" ( mkdir "deps/cef3/bin/Win32" )

pushd "deps/cef3/bin/Win32"
    cmake -G "Visual Studio 14 2015" ../../
    msbuild cef.sln /p:Configuration=Release
popd
copy .\deps\cef3\bin\Win32\libcef_dll_wrapper\Release\libcef_dll_wrapper.lib .\deps\cef3\Release\libcef_dll_wrapper.lib

rem Setp 2: Build NplCefBrowser.dll
if not exist "bin/Win32" ( mkdir "bin/Win32" )

pushd "bin/Win32"
    cmake -G "Visual Studio 14 2015" -DNPLRuntime_DIR:STRING=..\NPLRuntime   ../../
    msbuild NplCefBrowser.sln /p:Configuration=Release
popd
