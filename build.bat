@echo off 
rem Setp 1: Build libcef_dll_wrapper.lib
if not exist "cef3/bin/Win32" ( mkdir "cef3/bin/Win32" )

pushd "cef3/bin/Win32"
    cmake -G "Visual Studio 14 2015" ../../
    msbuild cef.sln /p:Configuration=Debug
    msbuild cef.sln /p:Configuration=Release
popd
copy .\cef3\bin\Win32\libcef_dll_wrapper\Debug\libcef_dll_wrapper.lib .\cef3\Debug\libcef_dll_wrapper.lib 
copy .\cef3\bin\Win32\libcef_dll_wrapper\Release\libcef_dll_wrapper.lib .\cef3\Release\libcef_dll_wrapper.lib

rem Setp 2: Build NplCefBrowser.dll
if not exist "bin/Win32" ( mkdir "bin/Win32" )

pushd "bin/Win32"
    cmake -G "Visual Studio 14 2015" -DParaEngineClient_SOURCE_DIR:STRING=.\npl_packages\NPLRuntime\Client\trunk\ParaEngineClient  ../../
    msbuild NplCefBrowser.sln /p:Configuration=Debug
    msbuild NplCefBrowser.sln /p:Configuration=Release
popd
 