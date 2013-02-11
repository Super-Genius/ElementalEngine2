@echo off
if not exist "..\..\FlackSquad\Shaders\Compiled" mkdir "..\..\FlackSquad\Shaders\Compiled"

cd ..\EEShaderCompiler\
call EEShaderCompiler.bat ..\..\FlackSquad\ 2>CompileShaders.log
