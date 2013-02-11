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

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if _MSC_VER >= 1400
#define _CRT_SECURE_NO_DEPRECATE 1
#endif

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#include <math.h>

#include "Resource.h"
#include "ElementalEngine.hpp"
#include "CHashString.hpp"
#include "Register.h"
#include "IDLL.h"
#include "Messages.h"
#include "CManager.h"

#include "GuiLib.h"

#include "DiagramDefines.h"
#include "DiagramEntity.h"
#include "DiagramLine.h"
#include "DiagramEditor.h"
#include "DiagramClipboardHandler.h"
#include "DiagramEntityContainer.h"
#include "DiagramMenu.h"

#include "MFCGridCtrl.h"

#include "MenuUtil.h"
#include "ObjectHelpers.h"
#include "UndoHelpers.h"
#include "UndoActions.h"
#include "ArchiveHelpers.h"
#include "EditorHelpers.h"
using namespace GDS_SDK_Helpers;

#include "QHStateMachineDefines.h"

#include "SimpleDiagramEntity.h"
#include "StateDiagramEntity.h"
#include "StateMachineClipboard.h"
#include "StateMachineClipboardVisitor.h"

#include "StateMachineValidator.h"

#include "StateMachineToolBar.h"

#include "StateMachineDiagramEditor.h"
#include "StateMachineEditorView.h"
#include "StateMachineEditorDoc.h"
#include "StateMachineEditorFrm.h"
#include "StateMachineEditor.h"
#include "StateMachineEditorComponent.hpp"
#include "StateMachineDiagramMenu.h"

#include "EventDiagramEntity.h"
#include "TransitionDiagramEntity.h"

#include "EventActionsPage.h"
#include "EventTransitionsPage.h"

#include "StateMachineUndoActions.h"
#include "StateMachineHelpers.h"


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__55FDC6E7_E4EA_4848_979C_97D5F9C9E70E__INCLUDED_)