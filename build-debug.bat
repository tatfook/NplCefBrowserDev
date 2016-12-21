@echo off 
if not exist "sln/Win32" ( mkdir "sln/Win32" )
pushd "sln/Win32"
    cmake -G "Visual Studio 12 2013" -DNPLRuntime_DIR:STRING=../NPLRuntime   ../../
popd



 