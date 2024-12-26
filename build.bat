@echo off

cmake -S . -B builds\windows\ -DCMAKE_CXX_COMPILER="C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.42.34433\bin\Hostx64\x64\cl.exe" -DCMAKE_C_COMPILER="C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.42.34433\bin\Hostx64\x64\cl.exe"
cmake --build builds\windows
