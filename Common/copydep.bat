@echo off
rem %1 = dep filename
rem %2 = source dir
rem %3 = dest dir
rem %A = filenameset of dependancy files to process
rem %B = line within dependancy file

for %%A in (%1) do for /F %%B in (%%A) do (
	rem add stuff here if you want
	xcopy /YEDIFCR "%~f2%%B" "%3%%B"
)

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
