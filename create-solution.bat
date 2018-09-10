@echo off 
if not exist "deps/cef3/sln/Win32" ( mkdir "deps/cef3/sln/Win32" )
pushd "deps/cef3/sln/Win32"
    "C:\Program Files\CMake\bin\cmake.exe" -G "Visual Studio 14 2015" ../../
popd

if not exist "sln/Win32" ( mkdir "sln/Win32" )
pushd "sln/Win32"
    "C:\Program Files\CMake\bin\cmake.exe" -G "Visual Studio 14 2015"  -DNPLRuntime_DIR:STRING=../NPLRuntime -DCEF3_WRAPPER_LIB_DIR:STRING=sln/Win32/libcef_dll_wrapper   ../../
popd


 