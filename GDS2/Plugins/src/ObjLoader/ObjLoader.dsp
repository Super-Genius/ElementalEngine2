# Microsoft Developer Studio Project File - Name="ObjLoader" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 60000
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=ObjLoader - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ObjLoader.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ObjLoader.mak" CFG="ObjLoader - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ObjLoader - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ObjLoader - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "ObjLoader"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ObjLoader - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ObjLoader___Win32_Release"
# PROP BASE Intermediate_Dir "ObjLoader___Win32_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MD /W3 /GX /Zi /O2 /I "..\..\..\\" /I "..\include" /I "..\..\..\3rd party\sizecbar-v2.44\src" /I "..\..\..\3rd Party\DevIL\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "..\..\..\\" /I "..\include" /I "..\..\..\3rd party\sizecbar-v2.44\src" /I "..\..\..\3rd Party\DevIL\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 DevIL.lib ilu.lib ilut.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\ObjLoader.dlr" /libpath:"..\..\..\..\DevIL\Lib\Release" /libpath:"..\..\..\..\mrcext\Lib\Release" /libpath:"..\..\..\..\..\NVSDK\Direct3D\DX8\mssdk_8\lib"
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\Bin\Plugins\ObjLoader.dlo"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy "..\..\..\3rd Party\DevIL\Lib\DevIL.dll" ..\..\..\Bin\Plugins	copy "..\..\..\3rd Party\DevIL\Lib\ilu.dll" ..\..\..\Bin\Plugins	copy "..\..\..\3rd Party\DevIL\Lib\ilut.dll" ..\..\..\Bin\Plugins
# End Special Build Tool

!ELSEIF  "$(CFG)" == "ObjLoader - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ObjLoader___Win32_Debug"
# PROP BASE Intermediate_Dir "ObjLoader___Win32_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\..\\" /I "..\include" /I "..\..\..\3rd party\sizecbar-v2.44\src" /I "..\..\..\3rd Party\DevIL\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\..\\" /I "..\include" /I "..\..\..\3rd party\sizecbar-v2.44\src" /I "..\..\..\3rd Party\DevIL\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 DevIL.lib ilu.lib ilut.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\Debug\ObjLoader.dlr" /pdbtype:sept /libpath:"..\..\..\..\DevIL\Lib\Debug" /libpath:"..\..\..\..\mrcext\Lib\Debug" /libpath:"..\..\..\..\..\NVSDK\Direct3D\DX8\mssdk_8\lib"
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\Bin\Debug\Plugins\ObjLoader.dlo" /pdbtype:sept
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy "..\..\..\3rd Party\DevIL\Lib\Debug\DevIL.dll" ..\..\..\Bin\Debug\Plugins	copy "..\..\..\3rd Party\DevIL\Lib\Debug\ilu.dll" ..\..\..\Bin\Debug\Plugins	copy "..\..\..\3rd Party\DevIL\Lib\Debug\ilut.dll" ..\..\..\Bin\Debug\Plugins
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "ObjLoader - Win32 Release"
# Name "ObjLoader - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ObjLoader.cpp
# End Source File
# Begin Source File

SOURCE=.\ObjLoader.def
# End Source File
# Begin Source File

SOURCE=.\ObjLoader.rc
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD BASE CPP /Yc"stdafx.h"
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TextureDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\TextureView.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ObjLoader.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TextureDoc.h
# End Source File
# Begin Source File

SOURCE=.\TextureView.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\idr_text.ico
# End Source File
# Begin Source File

SOURCE=.\res\ObjLoader.rc2
# End Source File
# Begin Source File

SOURCE=.\res\textureIcon.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
