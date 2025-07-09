@echo off
echo === Vietnamese Text Normalization Test Suite ===
echo.

REM Set library path for Windows
set PATH=..\lib;%PATH%

REM Clean previous builds
echo Cleaning previous builds...
if exist main.exe del main.exe
if exist test.exe del test.exe
if exist output.txt del output.txt
if exist input.txt del input.txt

REM Build the test executable
echo Building test executable...
make test

if %ERRORLEVEL% neq 0 (
    echo ❌ Build failed!
    pause
    exit /b 1
)

echo ✅ Build successful!
echo.

REM Run the unit tests
echo Running unit tests...
test.exe

if %ERRORLEVEL% neq 0 (
    echo ❌ Unit tests failed!
    pause
    exit /b 1
)

echo.
echo === Integration Tests ===

REM Test the main program with different options
echo Testing main program with default options...
copy test_input.txt input.txt
main.exe
if %ERRORLEVEL% equ 0 (
    echo ✅ Default normalization test passed
    echo Output:
    type output.txt
) else (
    echo ❌ Default normalization test failed
)

echo.
echo Testing main program with lowercase option...
main.exe -lower
if %ERRORLEVEL% equ 0 (
    echo ✅ Lowercase normalization test passed
    echo Output:
    type output.txt
) else (
    echo ❌ Lowercase normalization test failed
)

echo.
echo Testing main program with unknown word handling...
main.exe -unknown
if %ERRORLEVEL% equ 0 (
    echo ✅ Unknown word handling test passed
    echo Output:
    type output.txt
) else (
    echo ❌ Unknown word handling test failed
)

echo.
echo Testing main program with punctuation preservation...
main.exe -punc
if %ERRORLEVEL% equ 0 (
    echo ✅ Punctuation preservation test passed
    echo Output:
    type output.txt
) else (
    echo ❌ Punctuation preservation test failed
)

echo.
echo Testing main program with regex-only mode...
main.exe -rule
if %ERRORLEVEL% equ 0 (
    echo ✅ Regex-only mode test passed
    echo Output:
    type output.txt
) else (
    echo ❌ Regex-only mode test failed
)

echo.
echo === Test Summary ===
echo All tests completed!
echo Check the output files for detailed results.
echo.
echo Generated files:
if exist output.txt (
    echo output.txt - exists
) else (
    echo output.txt - not found
)
if exist input.txt (
    echo input.txt - exists
) else (
    echo input.txt - not found
)

REM Clean up
echo.
echo Cleaning up...
if exist main.exe del main.exe
if exist test.exe del test.exe
if exist output.txt del output.txt
if exist input.txt del input.txt

echo ✅ Test suite completed!
pause 