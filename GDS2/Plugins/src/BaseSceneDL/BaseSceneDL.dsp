# Microsoft Developer Studio Project File - Name="BaseSceneDL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 60000
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=BaseSceneDL - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "BaseSceneDL.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "BaseSceneDL.mak" CFG="BaseSceneDL - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "BaseSceneDL - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "BaseSceneDL - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "BaseSceneDL"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "BaseSceneDL - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "BaseSceneDL___Win32_Debug"
# PROP BASE Intermediate_Dir "BaseSceneDL___Win32_Debug"
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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\..\\" /I "..\include" /I "..\..\..\3rd party\sizecbar-v2.44\src" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\Bin\Debug\Plugins\BaseSceneDL.dlb" /pdbtype:sept /libpath:"..\..\..\..\mrcext\Lib\Debug" /libpath:"..\..\..\..\..\nvsdk\Common\Lib\Debug"
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\Bin\Debug\Plugins\BaseSceneDL.dlb" /pdbtype:sept

!ELSEIF  "$(CFG)" == "BaseSceneDL - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "BaseSceneDL___Win32_Release"
# PROP BASE Intermediate_Dir "BaseSceneDL___Win32_Release"
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
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "..\..\..\\" /I "..\include" /I "..\..\..\3rd party\sizecbar-v2.44\src" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\Bin\Release\Plugins\BaseSceneDL.dlb" /libpath:"..\..\..\..\mrcext\Lib\Release" /libpath:"..\..\..\..\..\nvsdk\Common\Lib\Release"
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\Bin\Plugins\BaseSceneDL.dlb"

!ENDIF 

# Begin Target

# Name "BaseSceneDL - Win32 Debug"
# Name "BaseSceneDL - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BaseSceneDL.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseSceneDL.def
# End Source File
# Begin Source File

SOURCE=.\BaseSceneDL.rc
# End Source File
# Begin Source File

SOURCE=.\Hierarchy.cpp
# End Source File
# Begin Source File

SOURCE=.\SceneDLDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\SceneDLDockWin.cpp
# End Source File
# Begin Source File

SOURCE=.\SceneDLToolPal.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD BASE CPP /Yc"stdafx.h"
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TreeCtrlEx.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BaseSceneDL.h
# End Source File
# Begin Source File

SOURCE=.\Hierarchy.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SceneDLDoc.h
# End Source File
# Begin Source File

SOURCE=.\SceneDLDockWin.h
# End Source File
# Begin Source File

SOURCE=.\SceneDLToolPal.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TreeCtrlEx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\BaseSceneDL.rc2
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
