@echo off
echo Building Vietnamese Text Normalization Test Suite...
echo.

REM Set library path
set PATH=..\lib;%PATH%

REM Define source files
set DEPS=ICUReadFile.cpp ICUMapping.cpp ICUNumberConverting.cpp ICUDictionary.cpp SpecialCase.cpp Address.cpp Math.cpp DateTime.cpp
set TEST=test.cpp

REM Build test executable
echo Compiling test executable...
g++ -std=c++17 -o test.exe %TEST% %DEPS% -lpthread -lm -L../lib -licudt -licuin -licuio -licuuc

if %ERRORLEVEL% equ 0 (
    echo [SUCCESS] Test build successful!
    echo.
    echo Running tests...
    test.exe
) else (
    echo [ERROR] Test build failed!
    echo.
    echo Troubleshooting:
    echo 1. Check that ICU libraries are in ../lib directory
    echo 2. Verify all source files exist
    echo 3. Ensure g++ is properly installed
)

pause 