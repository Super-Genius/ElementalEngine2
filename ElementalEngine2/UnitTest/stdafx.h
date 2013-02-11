// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#endif

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows 95 and Windows NT 4 or later.
#define WINVER 0x0400		// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows NT 4 or later.
#define _WIN32_WINNT 0x0400		// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 4.0 or later.
#define _WIN32_IE 0x0400	// Change this to the appropriate value to target IE 5.0 or later.
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes

#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <windowsx.h>

#include <vector>
#include <algorithm>

#include "Platform.h"
#include "ElementalEngine.hpp"
#include "CHashString.hpp"
#include "LoadSaveMessages.hpp"
#include "BaseMessages.h"
#include "Register.h"
//#include "Yasper\Yasper.h"
//typedef ptr<IHashString> IHashStringPtr;
#include "DatabaseDefines.hpp"
#include "IVariableList.hpp"
#include "Vec3.h"
#include "EulerAngle.h"

#include "TestBase.h"
#include "StateMachineTest.h"
#include "EntityQueryTest.h"

#include "..\plugins2\include\openal\openaldefines.h" // needed for test...
#include "SoundTest.h"
#include "DatabaseTest.h"
#include "NodeSystemTest.h"
#include "MathLibraryTest.h"
#include "LanguageTest.h"
#include "Win32NetworkTest.h"