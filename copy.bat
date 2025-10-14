@echo off
rem ================================================================
rem  PostgreSQL Extension Installer for Windows
rem  Copies DLL, control, and SQL files to PostgreSQL directories
rem ================================================================

rem --- CONFIGURATION ---
set EXTNAME=myfunc
set PGVER=16
set PGBIN="C:\Program Files\PostgreSQL\%PGVER%\bin"
set PGLIB="C:\Program Files\PostgreSQL\%PGVER%\lib"
set PGEXT="C:\Program Files\PostgreSQL\%PGVER%\share\extension"

rem --- SOURCE FILES (in current folder) ---
set DLL=%EXTNAME%.dll
set CONTROL=%EXTNAME%.control
set SQL=%EXTNAME%--1.0.sql

echo.
echo Installing PostgreSQL extension: %EXTNAME%
echo PostgreSQL version: %PGVER%
echo.

rem --- Check for files ---
if not exist "%DLL%" (
    echo [ERROR] %DLL% not found!
    exit /b 1
)
if not exist "%CONTROL%" (
    echo [ERROR] %CONTROL% not found!
    exit /b 1
)
if not exist "%SQL%" (
    echo [ERROR] %SQL% not found!
    exit /b 1
)

rem --- Copy DLL to lib directory ---
echo Copying %DLL% to %PGLIB% ...
copy /Y "%DLL%" "%PGLIB%\"

rem --- Copy control file to extension directory ---
echo Copying %CONTROL% to %PGEXT% ...
copy /Y "%CONTROL%" "%PGEXT%\"

rem --- Copy SQL file to extension directory ---
echo Copying %SQL% to %PGEXT% ...
copy /Y "%SQL%" "%PGEXT%\"

echo.
echo ✅ Installation complete!
echo Files installed to:
echo   DLL:   %PGLIB%
echo   SQL:   %PGEXT%
echo   CTRL:  %PGEXT%
echo.

pause
