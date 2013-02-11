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

#define MAX_MEM_SIZE 4000

#include "Resource.h"
#include "GuiLib.h"			// GuiLib includes
#include "ElementalEngine.hpp"
#include "Singleton.h"
#include "CHashString.hpp"
#include "Register.h"
#include "IDLL.h"
#include "Messages.h"
#include "BaseMessages.h"
#include "GUIDefines.h"
#include "EEDefines.h"
#include "Vec3.h"
#include "GuiLib.h"			// GuiLib includes
#include "Singleton.h"
#include "CHashString.hpp"
#include "Register.h"
#include "IDLL.h"
#include "Messages.h"
#include "OptionTree.h"
#include "logger.h"
#include "CObjectTemplate.hpp"
#include "CSceneObject.h"

#include "EditorHelpers.h"

#include "IGUIElement.h"
#include "IGUIReference.h"

#include "ViewObject.h"
#include "ControlSchema.hpp"
#include "IEditViewObject.h"
#include "IStaticViewObject.h"
#include "ISpinnerViewObject.h"
#include "ICheckBoxViewObject.h"
#include "IRadioViewObject.h"
#include "IFileViewObject.h"
#include "IComboViewObject.h"
#include "IColorViewObject.h"
#include "COptionTreeWrapper.h"
#include "COptionTreeDock.h"

#include "GUIFullScreen.h"
#include "GUIPageName.h"
#include "GUIGroupNameBox.h"
#include "GUIUseGroupBox.h"
#include "GUINewButtonBox.h"
#include "GUINewTextBox.h"
#include "GUINewStaticBox.h"
#include "GUIUseItemBox.h"
#include "GUISetBGColorBox.h"
#include "NewAnimationBox.h"
#include "GUIKeyDataView.h"
#include "GUITabBar.h"
#include "GUIResBox.h"
#include "CGUIRect.h"
#include "GUITools.h"
#include "GUIAnimationView.h"
#include "AddEditLanguageBox.h"
#include "LanguageView.h"
#include "GUIData.h"
#include "GenericDockBar.h"
#include "GUIEditorComponent.hpp"
#include "GUIEditorView.h"
#include "GUIEditorFrm.h"
#include "GUIEditor.h"
#include "GUIEditorDoc.h"
#include "MenuUtil.h"

using namespace GDS_SDK_Helpers;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__55FDC6E7_E4EA_4848_979C_97D5F9C9E70E__INCLUDED_)
