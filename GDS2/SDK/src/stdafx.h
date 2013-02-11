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

#if _MSC_VER >= 1400
#define _CRT_SECURE_NO_DEPRECATE 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#endif

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows 95 and Windows NT 4 or later.
#define WINVER 0x0501		// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows NT 4 or later.
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target Windows 2000 or later.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0501 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 4.0 or later.
#define _WIN32_IE 0x0501	// Change this to the appropriate value to target IE 5.0 or later.
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <assert.h>

// specific to library includes
#include "MenuUtil.h"
#include "ElementalEngine.hpp"
#include "EE-SDK.h"
#include "CHashString.hpp"
#include "BaseMessages.h"
#include "ViewObject.h"
#include "ControlSchema.hpp"
#include "OptionTree.h"
#include "GuiLib.h"		
// interfaces for objects
#include "IEditViewObject.h"
#include "IStaticViewObject.h"
#include "ISpinnerViewObject.h"
#include "IRadioViewObject.h"
#include "ICheckBoxViewObject.h"
#include "IFileViewObject.h"
#include "IComboViewObject.h"
#include "IColorViewObject.h"
#include "IHiddenViewObject.h"
#include "Messages.h"

#include "EditViewObject.h"
#include "StaticViewObject.h"
#include "SpinnerViewObject.h"
#include "RadioViewObject.h"
#include "CheckBoxViewObject.h"
#include "FileViewObject.h"
#include "ComboViewObject.h"
#include "ColorViewObject.h"
#include "HiddenViewObject.h"
#include "COptionTreeItemCheckBoxEx.h"
#include "COptionTreeWrapper.h"
#include "ViewCamera.h"
#include "RenderContext.h"

#include "ObjectHelpers.h"
#include "ArchiveHelpers.h"
#include "EditorHelpers.h"
#include "FileHelpers.h"
#include "IUndoRedoAction.h"
#include "UndoHelpers.h"
#include "UndoActions.h"

#include "CChildFindVisitor.h"

using namespace GDS_SDK_Helpers;