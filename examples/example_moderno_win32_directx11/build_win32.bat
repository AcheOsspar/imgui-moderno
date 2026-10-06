@REM Build for Visual Studio compiler. Run your copy of vcvars32.bat or vcvarsall.bat to setup command-line compiler.
@set OUT_DIR=Debug
@set OUT_EXE=example_moderno_win32_directx11
@set INCLUDES=/I..\.. /I..\..\backends /I..\..\misc\moderno /I "%WindowsSdkDir%Include\um" /I "%WindowsSdkDir%Include\shared" /I "%DXSDK_DIR%Include"
@set SOURCES=main.cpp ..\..\backends\imgui_impl_dx11.cpp ..\..\backends\imgui_impl_win32.cpp ..\..\imgui*.cpp ..\..\misc\moderno\imgui_moderno_demo.cpp
@set LIBS=/LIBPATH:"%DXSDK_DIR%/Lib/x86" d3d11.lib d3dcompiler.lib
mkdir %OUT_DIR%
cl /nologo /Zi /MD /utf-8 /D _CRT_SECURE_NO_WARNINGS %INCLUDES% /D UNICODE /D _UNICODE %SOURCES% /Fe%OUT_DIR%/%OUT_EXE%.exe /Fo%OUT_DIR%/ /link %LIBS%
