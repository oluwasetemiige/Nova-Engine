@echo off
REM Nova Engine - Windows build script.  Usage: build.bat [Release|Debug|RelWithDebInfo]
setlocal

where cmake >nul 2>nul
if errorlevel 1 (
    echo [Nova] CMake was not found. Install it from https://cmake.org/download/ and re-run.
    exit /b 1
)
where python >nul 2>nul
if errorlevel 1 (
    echo [Nova] Python 3 was not found. GLAD needs it to generate the OpenGL loader.
    echo        Install from https://www.python.org/downloads/ then run: pip install jinja2
    exit /b 1
)

set BUILD_TYPE=%1
if "%BUILD_TYPE%"=="" set BUILD_TYPE=Release

cmake -S . -B build
if errorlevel 1 exit /b 1

cmake --build build --config %BUILD_TYPE% --parallel
if errorlevel 1 exit /b 1

echo.
echo [Nova] Build finished. Run the editor with:
echo        build\NovaEditor\%BUILD_TYPE%\NovaEditor.exe
endlocal
