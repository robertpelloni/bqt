@echo off
REM ============================================================================
REM BQt (BobUI) Build Script
REM Compiles the Go-first BQt runtime kernel into bqt.exe without purging
REM any existing native or Go build outputs.
REM ============================================================================
setlocal
title BQt Build
cd /d "%~dp0"

echo [BQt] Verifying Go toolchain...
where go >nul 2>nul
if errorlevel 1 (
    echo [BQt] ERROR: Go toolchain not found in PATH.
    exit /b 1
)

echo [BQt] Building BQt Go kernel (bqt.exe)...
go build -buildvcs=false -o bqt.exe .
if errorlevel 1 (
    echo [BQt] Build failed with exit code %errorlevel%.
    exit /b %errorlevel%
)

echo success> .build_success
echo [BQt] Build succeeded: %CD%\bqt.exe
endlocal
