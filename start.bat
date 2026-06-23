@echo off
setlocal
title BQt
cd /d "%~dp0"

echo [BQt] Starting...
where go >nul 2>nul
if errorlevel 1 (
    echo [BQt] go not found. Please install it.
    pause
    exit /b 1
)

go run .

if errorlevel 1 (
    echo [BQt] Exited with error code %errorlevel%.
    pause
)
endlocal
