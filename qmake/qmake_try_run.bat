@echo off

C:/Users/hyper/workspace/npp/bobui/bin/qmake.exe -v > nul 2>&1

if "%errorlevel%" == "-1073741515" (
echo
echo     'qmake' is built successfully, but some of the libraries
echo     necessary for running it are missing. If you are building Qt with
echo     3rdparty libraries, make sure that you add their directory to the
echo     PATH environment variable.
echo
exit /b %errorlevel%
)
echo. > C:/Users/hyper/workspace/npp/bobui/qmake/qmake_try_run_passed