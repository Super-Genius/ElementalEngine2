# Microsoft Developer Studio Project File - Name="sml" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=sml - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "sml.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "sml.mak" CFG="sml - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "sml - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "sml - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "sml - Win32 Release with debug RTL" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "sml - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /G6 /MD /W3 /GR /GX /O2 /Ob2 /I "." /D "NDEBUG" /D "_SMLDLL_" /D "WIN32" /D "_WINDOWS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386 /implib:"..\lib\sml.lib"
# SUBTRACT LINK32 /profile /map /debug
# Begin Custom Build - Copy sml.dll to SDK folder.
TargetPath=.\Release\sml.dll
TargetName=sml
InputPath=.\Release\sml.dll
SOURCE="$(InputPath)"

"$(TargetName)" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(TargetPath) ..\lib

# End Custom Build

!ELSEIF  "$(CFG)" == "sml - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /G6 /Zp16 /MDd /W3 /Gm /Gi /GR /GX /ZI /Od /I "." /D "_DEBUG" /D "_SMLDLL_" /D "WIN32" /D "_WINDOWS" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /map /debug /machine:I386 /implib:"..\lib\sml.lib"
# SUBTRACT LINK32 /profile
# Begin Custom Build - Copy sml.dll to SDK folder.
TargetPath=.\Debug\sml.dll
TargetName=sml
InputPath=.\Debug\sml.dll
SOURCE="$(InputPath)"

"$(TargetName)" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(TargetPath) ..\lib

# End Custom Build
# Begin Special Build Tool
TargetName=sml
SOURCE="$(InputPath)"
PreLink_Cmds=del  ..\lib\$(TargetName).*
# End Special Build Tool

!ELSEIF  "$(CFG)" == "sml - Win32 Release with debug RTL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "sml___Wi"
# PROP BASE Intermediate_Dir "sml___Wi"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseD"
# PROP Intermediate_Dir "ReleaseD"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MD /W3 /GR /GX /Ox /Ot /Og /Oi /Ob2 /I "." /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_SMLDLL_" /YX /FD /c
# ADD CPP /nologo /G6 /MDd /W3 /Gi /GR /GX /Ox /Ot /Og /Oi /Ob2 /Gy /I "." /D "NDEBUG" /D "_SMLDLL_" /D "WIN32" /D "_WINDOWS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386 /implib:"..\lib\sml.lib"
# ADD LINK32 /nologo /subsystem:windows /dll /map /debug /machine:I386 /implib:"..\lib\sml.lib"
# SUBTRACT LINK32 /profile /incremental:yes
# Begin Custom Build - Copy sml.dll to SDK folder.
TargetPath=.\ReleaseD\sml.dll
TargetName=sml
InputPath=.\ReleaseD\sml.dll
SOURCE="$(InputPath)"

"$(TargetName)" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(TargetPath) ..\lib

# End Custom Build

!ENDIF 

# Begin Target

# Name "sml - Win32 Release"
# Name "sml - Win32 Debug"
# Name "sml - Win32 Release with debug RTL"
# Begin Group "Source Files"

# PROP Default_Filter "*.cpp;*.inl"
# Begin Source File

SOURCE=.\smlbound.cpp
# End Source File
# Begin Source File

SOURCE=.\smlbound.inl
# End Source File
# Begin Source File

SOURCE=.\smlextra.cpp
# End Source File
# Begin Source File

SOURCE=.\smlmath.cpp
# End Source File
# Begin Source File

SOURCE=.\smlmath.inl
# End Source File
# Begin Source File

SOURCE=.\smlmath_sse.inl
# End Source File
# Begin Source File

SOURCE=.\smlxmatrix.cpp

!IF  "$(CFG)" == "sml - Win32 Release"

# ADD CPP /O2 /Ob1 /FAs

!ELSEIF  "$(CFG)" == "sml - Win32 Debug"

# ADD CPP /FAs

!ELSEIF  "$(CFG)" == "sml - Win32 Release with debug RTL"

# ADD BASE CPP /O2 /Ob1
# ADD CPP /O2 /Ob1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\smlxmatrix.inl
# End Source File
# Begin Source File

SOURCE=.\smlxmmm.inl
# End Source File
# Begin Source File

SOURCE=.\smlxopt.inl
# End Source File
# Begin Source File

SOURCE=.\smlxunroll.inl
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=.\ismbound.h
# End Source File
# Begin Source File

SOURCE=.\ismmath.h
# End Source File
# Begin Source File

SOURCE=.\smlbound.h
# End Source File
# Begin Source File

SOURCE=.\smlextra.h
# End Source File
# Begin Source File

SOURCE=.\smlmath.h
# End Source File
# Begin Source File

SOURCE=.\smlxmatrix.h
# End Source File
# Begin Source File

SOURCE=.\smlxopt.h
# End Source File
# End Group
# Begin Group "Optimized Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\smlxfun.inl
# End Source File
# Begin Source File

SOURCE=.\smlxGENops.inl

!IF  "$(CFG)" == "sml - Win32 Release"

# Begin Custom Build - Generating optimized functions decls (smlxGENops.h) and def instances (smlxGENops.i)
InputPath=.\smlxGENops.inl

BuildCmds= \
	cl  smlxGENops.cpp /c /Zs /P /EP /I.    /nologo \
	copy smlxGENops.i smlxGENops.h \
	cl  smlxGENops.cpp /c /Zs /P /EP /I. /D_MMBODY   /nologo \
	

"smlxGENops.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"smlxGENops.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "sml - Win32 Debug"

# Begin Custom Build - Generating optimized functions decls (smlxGENops.h) and def instances (smlxGENops.i)
InputPath=.\smlxGENops.inl

BuildCmds= \
	cl  smlxGENops.cpp /c /Zs /P /EP /I.  /nologo \
	copy smlxGENops.i smlxGENops.h \
	cl  smlxGENops.cpp /c /Zs /P /EP /I. /D_MMBODY   /nologo \
	

"smlxGENops.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"smlxGENops.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "sml - Win32 Release with debug RTL"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\smlxmult.inl
# End Source File
# End Group
# End Target
# End Project
