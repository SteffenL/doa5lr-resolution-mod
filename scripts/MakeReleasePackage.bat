@echo off

set mainExeName=d3d9.dll
set solutionDir=%~dp0..
set endUserDocDir=%solutionDir%\doc\end-users
set artifactsDir=%solutionDir%\build\Release

::for /f "delims=" %%a in ('git describe --long --tags --dirty --always') do set "version=%%a"
:: Ref.: http://stackoverflow.com/questions/6697878/get-the-file-version-of-a-dll-or-exe
set version=x.x.x
FOR /F "tokens=1-3" %%i IN ('%~dp0bin\sigcheck\sigcheck.exe /accepteula "%artifactsDir%\%mainExeName%"') DO (
    IF "%%i %%j" == "File version:" SET version=%%k
)

:: Strip build/patch number
SET version=%version:~0,-2%

set artifactsArchiveFilePath=%~dp0\Doa5lrResolutionMod-%version%-artifacts.zip
set releaseArchiveFilePath=%~dp0\Doa5lrResolutionMod-%version%-bin.zip


:: Package artifacts
powershell "Compress-Archive \"%artifactsDir%\*\" \"%artifactsArchiveFilePath%\""


:: Create release-package

set tempFilesDir=%~dp0tempPackageFiles
if exist "%tempFilesDir%" rmdir /S /Q "%tempFilesDir%"
mkdir "%tempFilesDir%"

:: Copy build artifacts
robocopy "%artifactsDir%" "%tempFilesDir%" *.exe *.dll /XJ /XF *Tests.*

:: Sign files
::for %%f in ("%tempFilesDir%\*") do (
::    call :sign "%%f"
::)

:: Copy documentation
robocopy "%endUserDocDir%" "%tempFilesDir%" /E /XJ

:: Zip everything
powershell "Compress-Archive \"%tempFilesDir%\*\" \"%releaseArchiveFilePath%\""


:: Clean up
rmdir /S /Q "%tempFilesDir%"

goto :eof

:sign
    "%SIGNTOOL%" sign /d "EDI Tools" /du "https://www.steffenl.com/" /f "%USERPROFILE%\Documents\Certificates\StartSSL\Class 2\Code\codesign.pfx" /t "http://timestamp.verisign.com/scripts/timestamp.dll" /v "%~1"
    goto :eof