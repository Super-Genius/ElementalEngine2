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

#if !defined(AFX_STDAFX_H__55FDC6E7_E4EA_4848_979C_97D5F9C9E70E__INCLUDED_)
#define AFX_STDAFX_H__55FDC6E7_E4EA_4848_979C_97D5F9C9E70E__INCLUDED_

#if _MSC_VER >= 1400
#define _CRT_SECURE_NO_DEPRECATE 1
#endif

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

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

// bug in visual studio with 255 characters in template functions.
// for some reason <map> ignores this warning
#pragma warning(disable : 4786)

#include "IDLL.h"
#include <afxdllx.h>
#include <list>

#include "Resource.h"
#include "ElementalEngine.hpp"
#include "EE-SDK.h"
#include "EEDefines.h"
#include "CHashString.hpp"
#include "CObjectTemplate.hpp"
#include "Singleton.h"
#include "CHashString.hpp"
#include "Messages.h"
#include "Register.h"
#include "BaseMessages.h"
#include "Vec3.h"
#include "CRenderObject.h"
#include "AppertureGenRenderObject.h"
#include "TextureEditorCmdTarget.h"
#include "TextureComponent.hpp"
#include "TextureEditor.h"
#include "Messages.h"
#include "TextureDoc.h"
#include "TextureView.h"
#include "MemDC.h"
#include "MenuUtil.h"

#include "EditorHelpers.h"
using namespace GDS_SDK_Helpers;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__55FDC6E7_E4EA_4848_979C_97D5F9C9E70E__INCLUDED_)
