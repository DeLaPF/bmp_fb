@echo off

:: Can add `-DBUILD_EX_<example_name>=TRUE` to build example
:: Note: may not revert to building default example unless specified
cmake -S . -B builds\windows\ -DCMAKE_CXX_COMPILER="C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.42.34433\bin\Hostx64\x64\cl.exe" -DCMAKE_C_COMPILER="C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.42.34433\bin\Hostx64\x64\cl.exe"
cmake --build builds\windows
