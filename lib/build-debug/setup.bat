@echo off
setlocal
call "%VS140COMNTOOLS%..\..\VC\bin\vcvars32.bat"
set path=%ProgramFiles%\CMake\bin;%LOCALAPPDATA%\Programs\Python\Python36-32;%PATH%
set libroot=%~dp0..

if exist LLVM.sln goto :skip_gen
cmake -G "Visual Studio 14" -D LLVM_TARGETS_TO_BUILD=X86 -D LLVM_ENABLE_RTTI=1 -D LLVM_USE_CRT_DEBUG=MTd -D LLVM_USE_CRT_RELEASE=MT -D LLVM_ENABLE_TERMINFO=OFF -D CMAKE_INSTALL_PREFIX=%libroot%\install-debug %libroot%\llvm-6.0.0.src || exit /b 1
:skip_gen
:: cmake --build . --config Debug || exit /b 1
cmake --build . --config Debug --target install || exit /b 1
