@echo off
echo Dependency Copy started at %DATE% %TIME% >syncdep.log
echo ----------------------------------------------- >>syncdep.log
call ..\Common\copydep.bat *.dep ..\Common\ ..\..\FlackSquad\ >>syncdep.log
echo ----------------------------------------------- >>syncdep.log
echo Dependency Copy ended at %DATE% %TIME% >>syncdep.log