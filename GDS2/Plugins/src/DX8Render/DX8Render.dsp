# Microsoft Developer Studio Project File - Name="DX8Render" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 60000
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=DX8Render - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "DX8Render.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "DX8Render.mak" CFG="DX8Render - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DX8Render - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "DX8Render - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "DX8Render"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "DX8Render - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DX8Render___Win32_Debug"
# PROP BASE Intermediate_Dir "DX8Render___Win32_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\..\\" /I "..\include" /I "..\..\..\3rd party\sizecbar-v2.44\src" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\..\\" /I "..\include" /I "..\..\..\3rd party\sizecbar-v2.44\src" /I "..\..\..\3rd party\mLogger" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\Bin\Debug\Plugins\DX8Render.dlb" /pdbtype:sept /libpath:"..\..\..\..\mrcext\Lib\Debug" /libpath:"..\..\..\..\..\nvsdk\Common\Lib\Debug"
# ADD LINK32 d3d8.lib dxerr8.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\Bin\Debug\Plugins\DX8Render.dlr" /pdbtype:sept

!ELSEIF  "$(CFG)" == "DX8Render - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "DX8Render___Win32_Release"
# PROP BASE Intermediate_Dir "DX8Render___Win32_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MD /W3 /GX /Zi /O2 /I "..\..\..\\" /I "..\include" /I "..\..\..\3rd party\sizecbar-v2.44\src" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "..\..\..\\" /I "..\include" /I "..\..\..\3rd party\sizecbar-v2.44\src" /I "..\..\..\3rd party\mLogger" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\Bin\Release\Plugins\DX8Render.dlb" /libpath:"..\..\..\..\mrcext\Lib\Release" /libpath:"..\..\..\..\..\nvsdk\Common\Lib\Release"
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\Bin\Plugins\DX8Render.dlr"

!ENDIF 

# Begin Target

# Name "DX8Render - Win32 Debug"
# Name "DX8Render - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\DX8Render.cpp
# End Source File
# Begin Source File

SOURCE=.\DX8Render.def
# End Source File
# Begin Source File

SOURCE=.\DX8Render.rc
# End Source File
# Begin Source File

SOURCE=.\DX8RenderContext.cpp
# End Source File
# Begin Source File

SOURCE=.\DX8RenderCore.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD BASE CPP /Yc"stdafx.h"
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\DX8Render.h
# End Source File
# Begin Source File

SOURCE=.\DX8RenderContext.h
# End Source File
# Begin Source File

SOURCE=.\DX8RenderCore.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\DX8Render.rc2
# End Source File
# Begin Source File

SOURCE=.\res\idr_DX8Render.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_SceneDL.ico
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tree_ico.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
