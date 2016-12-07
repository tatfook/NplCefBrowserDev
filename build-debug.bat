@echo off 
rem Setp 1: Build libcef_dll_wrapper.lib
if not exist "deps/cef3/sln/Win32" ( mkdir "deps/cef3/sln/Win32" )

pushd "deps/cef3/sln/Win32"
    cmake -G "Visual Studio 14 2015" ../../
    rem msbuild cef.sln /p:Configuration=Debug
popd
rem copy .\deps\cef3\sln\Win32\libcef_dll_wrapper\Debug\libcef_dll_wrapper.lib .\deps\cef3\Debug\libcef_dll_wrapper.lib 

rem Setp 2: Build NplCefBrowser.dll
if not exist "sln/Win32" ( mkdir "sln/Win32" )

pushd "sln/Win32"
    cmake -G "Visual Studio 14 2015" -DCMAKE_BUILD_TYPE=Debug -DNPLRuntime_DIR:STRING=../NPLRuntime   ../../
    rem msbuild NplCefBrowser.sln /p:Configuration=Debug
popd

rem xcopy /f deps\cef3\Debug\*.dll ..\NPLRuntime\ParaWorld\cef3\
rem xcopy /f deps\cef3\Debug\*.exe ..\NPLRuntime\ParaWorld\cef3\
rem xcopy /f deps\cef3\Debug\*.bin ..\NPLRuntime\ParaWorld\cef3\

 