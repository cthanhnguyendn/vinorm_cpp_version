@echo off
echo Building Vietnamese Text Normalization Main Program...
echo.

REM Set library path
set PATH=..\lib;%PATH%

REM Define source files
set DEPS=ICUReadFile.cpp ICUMapping.cpp ICUNumberConverting.cpp ICUDictionary.cpp SpecialCase.cpp Address.cpp Math.cpp DateTime.cpp
set MAIN=main.cpp

REM Build main executable
echo Compiling main executable...
g++ -std=c++17 -o main.exe %MAIN% %DEPS% -lpthread -lm -L../lib -licudt -licuin -licuio -licuuc

if %ERRORLEVEL% equ 0 (
    echo ✅ Main build successful!
    echo.
    echo Usage:
    echo   main.exe              - Normal mode
    echo   main.exe -lower       - Lowercase output
    echo   main.exe -unknown     - Handle unknown words
    echo   main.exe -punc        - Preserve punctuation
    echo   main.exe -rule        - Regex-only mode
    echo.
    echo Create input.txt file with your text to test.
) else (
    echo ❌ Main build failed!
    echo.
    echo Troubleshooting:
    echo 1. Check that ICU libraries are in ../lib directory
    echo 2. Verify all source files exist
    echo 3. Ensure g++ is properly installed
)

pause 