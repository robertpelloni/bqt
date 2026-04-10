:: Copyright (C) 2016 The BobUI Company Ltd.
:: Copyright (C) 2016 Intel Corporation.
:: SPDX-License-Identifier: BSD-3-Clause

@echo off
setlocal ENABLEDELAYEDEXPANSION ENABLEEXTENSIONS
set ARGS=%*
set BOBUISRC=%~dp0
set BOBUISRC=%BOBUISRC:~0,-1%
set BOBUIDIR=%CD%

rem Parse command line

set TOPLEVEL=false
set TOPBOBUISRC=%BOBUISRC%
set TOPBOBUIDIR=%BOBUIDIR%
if /i not "%~1" == "-top-level" goto notoplevel
set ARGS=%ARGS:~10%
set TOPLEVEL=true
for %%P in ("%TOPBOBUISRC%") do set TOPBOBUISRC=%%~dpP
set TOPBOBUISRC=%TOPBOBUISRC:~0,-1%
for %%P in ("%BOBUIDIR%") do set TOPBOBUIDIR=%%~dpP
set TOPBOBUIDIR=%TOPBOBUIDIR:~0,-1%
goto wastoplevel
:notoplevel
if not exist ..\.qmake.super goto wastoplevel
echo ERROR: You cannot configure bobuibase separately within a top-level build. >&2
exit /b 1
:wastoplevel

call :doargs %ARGS%
if errorlevel 1 exit /b
goto doneargs

:doargs
    if "%~1" == "" exit /b

    if "%~1" == "/?" goto help
    if "%~1" == "-?" goto help
    if /i "%~1" == "/h" goto help
    if /i "%~1" == "-h" goto help
    if /i "%~1" == "/help" goto help
    if /i "%~1" == "-help" goto help
    if /i "%~1" == "--help" goto help

    if /i "%~1" == "-redo" goto redo
    if /i "%~1" == "--redo" goto redo

:nextarg
    shift
    goto doargs

:help
    type %BOBUISRC%\config_help.txt
    if %TOPLEVEL% == false exit /b 1
    for /d %%p in ("%TOPBOBUISRC%"\bobui*) do (
        if not "%%p" == "%BOBUISRC%" (
            if exist "%%p\config_help.txt" (
                echo.
                type "%%p\config_help.txt"
            )
        )
    )
    exit /b 1

:redo
    if not exist "%TOPBOBUIDIR%\config.opt" goto redoerr
    echo %ARGS% > %TOPBOBUIDIR%\config.redo.in
    set redoing=""
    goto nextarg
:redoerr
    echo No config.opt present - cannot redo configuration. >&2
    exit /b 1

:doneargs

cd "%TOPBOBUIDIR%"

rem Write config.opt if we're not currently -redo'ing
set OPT_FILE_PATH=%TOPBOBUIDIR%\config.opt
set OPT_TMP_FILE_PATH=%TOPBOBUIDIR%\config.opt.in
set REDO_FILE_PATH=%TOPBOBUIDIR%\config.redo.last
set REDO_TMP_FILE_PATH=%TOPBOBUIDIR%\config.redo.in
set FRESH_REQUESTED_ARG=
if not defined redoing (
    rem "The '.' in 'echo.%*' ensures we don't print "echo is off" when no arguments are passed"
    rem "https://devblogs.microsoft.com/oldnewthing/20170802-00/?p=96735"
    rem "The space before the '>' makes sure that when we have a digit at the end of the args, we"
    rem "don't accidentally concatenate it with the '>' resulting in '0>' or '2>' which redirects"
    rem "into the file from a stream different than stdout, leading to broken or empty content."
    echo.%* >"%OPT_TMP_FILE_PATH%"

    call cmake -DIN_FILE="%OPT_TMP_FILE_PATH%" -DOUT_FILE="%OPT_FILE_PATH%" -DIGNORE_ARGS=-top-level -P "%BOBUISRC%\cmake\BobUIWriteArgsFile.cmake"
) else (
    echo. 2> "%OPT_TMP_FILE_PATH%"
    for /F "usebackq tokens=*" %%A in ("%OPT_FILE_PATH%") do echo "%%A" >> "%OPT_TMP_FILE_PATH%"

    call cmake -DIN_FILE="%OPT_TMP_FILE_PATH%" -DREDO_FILE="%REDO_TMP_FILE_PATH%" -DOUT_FILE="%REDO_FILE_PATH%" -DIGNORE_ARGS="-top-level;-redo;--redo" -P "%BOBUISRC%\cmake\BobUIWriteArgsFile.cmake"

    set OPT_FILE_PATH=%REDO_FILE_PATH%
    set FRESH_REQUESTED_ARG=-DFRESH_REQUESTED=TRUE
)

rem Launch CMake-based configure
set TOP_LEVEL_ARG=
if %TOPLEVEL% == true set TOP_LEVEL_ARG=-DTOP_LEVEL=TRUE
call cmake -DOPTFILE="%OPT_FILE_PATH%" %TOP_LEVEL_ARG% %FRESH_REQUESTED_ARG% -P "%BOBUISRC%\cmake\BobUIProcessConfigureArgs.cmake"
