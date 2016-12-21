@echo off 
if not exist "sln/Win32" ( mkdir "sln/Win32" )
pushd "sln/Win32"
    cmake -G "Visual Studio 14 2015" -DCMAKE_BUILD_TYPE=Release -DNPLRuntime_DIR:STRING=../NPLRuntime   ../../
    rem msbuild NplCefBrowser.sln /p:Configuration=Release
popd


 