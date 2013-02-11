///============================================================================
/// \note   Elemental Engine
///         Copyright (c)  2005-2008 Signature Devices, Inc.
///
///         This code is redistributable under the terms of the EE License.
///
///         This code is distributed without warranty or implied warranty of
///			merchantability or fitness for a particular purpose. See the 
///			EE License for more details.
///
///         You should have received a copy of the EE License along with this
///			code; If not, write to Signature Devices, Inc.,
///			3200 Bridge Parkway Suite 102, Redwood City, CA 94086 USA.
///============================================================================
#pragma once

#define _AFX_NO_DEBUG_CRT
#define _AFX_PORTABLE

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxcmn.h>

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
// Windows Header Files:
//#include <windows.h>
#include <assert.h>
#include <tchar.h>
#include <math.h>

#ifdef _DEBUG
	// This define must occur before any headers are included.
	//#define _CRTDBG_MAP_ALLOC
	#include <crtdbg.h>	
	#ifdef DEBUG_NEW
		#undef DEBUG_NEW
		#define DEBUG_NEW new(_NORMAL_BLOCK, THIS_FILE, __LINE__)
	#endif
#endif

#include "resource.h"
#include "ProfilerApp.h"

#include "ElementalEngine.hpp"
#include "eedefines.h"
#include "CHashString.hpp"
#include "Register.h"
#include "BaseMessages.h"
#include "CObjectTemplate.hpp"
#include "mathutils.h"
#include "Vec3.h"
#include "Matrix4x4.h"
#include "Matrix3x3.h"

#include "performanceprofiler.h"

#include "ReturnEdit.h"
#include "ReturnComboBox.h"

#include "IProfileDisplayType.h"

typedef vector<StdString> ITEMSZVEC;
typedef multimap<StdString, ITEMSZVEC> STRINGSORTMAP;
typedef multimap<float, ITEMSZVEC> FLOATSORTMAP;
typedef multimap<int, ITEMSZVEC> INTSORTMAP;
typedef vector<float> AVGTIMEVEC;
typedef map<DWORD, float> PEAKTIMEMAP;
typedef map<DWORD, AVGTIMEVEC> AVGTIMEMAP;

#include "CNameTimeDisplay.h"
#include "CNameNumberDisplay.h"

#include "ProfilerManager.h"
#include "ProfilerWindow.h"
#include "ProfilerTabCtrl.h"
#include "ProfileBox.h"
//#include <afxwin.h>

