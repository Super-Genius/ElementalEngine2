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

#if !defined(AFX_STDAFX_H__058DD8BD_CA21_47C7_9068_04B4C8B438FD__INCLUDED_)
#define AFX_STDAFX_H__058DD8BD_CA21_47C7_9068_04B4C8B438FD__INCLUDED_

#if _MSC_VER >= 1400
#define _CRT_SECURE_NO_DEPRECATE 1
#endif

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxcview.h>
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#include <afxadv.h>
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxsock.h>		// MFC socket extensions

#if _MSC_VER < 1300
// so we don't have to include afximpl.h which is the source directory
class CPushRoutingFrame
{
protected:
	CFrameWnd* pOldRoutingFrame;
	_AFX_THREAD_STATE* pThreadState;

public:
	CPushRoutingFrame(CFrameWnd* pNewRoutingFrame)
	{
		pThreadState = AfxGetThreadState();
		pOldRoutingFrame = pThreadState->m_pRoutingFrame;
		pThreadState->m_pRoutingFrame = pNewRoutingFrame;
	}
	~CPushRoutingFrame()
	{ pThreadState->m_pRoutingFrame = pOldRoutingFrame; }
};
#endif

// bug in visual studio with 255 characters in template functions.
// for some reason <map> ignores this warning
#pragma warning(disable : 4786)

#include "GuiLib.h"				// Gui Lib
#include "ElementalEngine.hpp"	// Elemental Engine SDK
#include "EEDefines.h"
#include "Singleton.h"
#include "CHashString.hpp"
#include "Register.h"
#include "LoadSaveMessages.hpp"	// for certain structures from LoadSaveManager
#include "BaseMessages.h"
#include "EE-SDK.h"
#include "Physics/PhysicsDefines.h"
#include "HelpSystem/HelpSystemDefines.hpp"
#include "CObjectTemplate.hpp"

#include "logger.h"
#include "resource.h"       // main symbols
#include "IDLL.h"
#include "Messages.h"
#include <list>
#include <vector>
#include <string>
#include <map>
#include "ResourceComponent.h"
#include "ResourceOptions.h"
#include "ResourcePage.h"
#include "ResourceDoc.h"
#include "ResourceView.h"
#include "ResourceFolderView.h"
#include "ResourceBar.h"
#include "NewObjectDialog.h"
#include "OptionsDialog.h"
#include "GDSComponent.hpp"
#include "SplashDialog.h"
#include "gds.h"
#include "MainFrm.h"

#include "EditorHelpers.h"
using namespace GDS_SDK_Helpers;

#include "EditingPropertiesHolder.h"
#include "EditingProperties.h"

#include <afxdlgs.h>


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__058DD8BD_CA21_47C7_9068_04B4C8B438FD__INCLUDED_)
