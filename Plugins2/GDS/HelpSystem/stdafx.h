///============================================================================
/// \file		stdafx.h
/// \brief		Precompiled header.
/// \date		11-11-2007
/// \author		Dmitriy S. Sergeyev
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

#ifndef HELPSYSTEM_STDAFX_H
#define HEPLSYSTEM_STDAFX_H

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#endif

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

#include <algorithm>
#include <list>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <stack>

#include "chmxx.h"

#include "Resource.h"
#include "ElementalEngine.hpp"
#include "Singleton.h"
#include "CHashString.hpp"
#include "Register.h"

#include "ResizablePropertySheet.h"
#include "SizeBar.h"
#include "browser.h"

#include "HelpSystem.h"
#include "HelpSystemCore.hpp"

#include "PropPageContents.h"
#include "PropPageIndex.h"
#include "PropPageSearch.h"
#include "BrowserPane.h"
#include "HelpDialogWindow.h"

#include "HelpSystem/HelpSystemDefines.hpp"
#include "HelpSystemComponent.hpp"
#include <afxdlgs.h>

#endif // #ifdef HELPSYSTEM_STDAFX_H
