@echo off
if not exist "%CommonProgramFiles%\Elemental Engine II" mkdir "%CommonProgramFiles%\Elemental Engine II"
echo Dependency Copy started at %DATE% %TIME% >"%CommonProgramFiles%\Elemental Engine II\commonfiles.log"
echo Copy everything (subdirectories as well) without confirmation
echo Work in progress. Please, wait! 
xcopy * "%CommonProgramFiles%\Elemental Engine II\" /y /s /e >>"%CommonProgramFiles%\Elemental Engine II\commonfiles.log"

if errorlevel 0 goto exit
if errorlevel 1 goto nofiles
if errorlevel 2 goto abort 
if errorlevel 4 goto lowmemory 
if errorlevel 5 goto diskwrite

:diskwrite
echo Disk write error occurred.
goto exit 

:lowmemory 
echo Insufficient memory to copy files or 
echo invalid drive or command-line syntax. 
goto exit 

:abort 
echo You pressed CTRL+C to end the copy operation. 
goto exit 

:nofiles
echo No files were found to copy.
goto exit 

:exit
