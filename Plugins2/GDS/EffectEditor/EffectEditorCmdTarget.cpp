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

#include "stdafx.h"
#include "EffectEditor.h"
#include "EffectEditorCmdTarget.h"
#include ".\effecteditorcmdtarget.h"


// CEffectEditorCmdTarget

IMPLEMENT_DYNAMIC(CEffectEditorCmdTarget, CCmdTarget)
CEffectEditorCmdTarget::CEffectEditorCmdTarget()
{
	m_pEditor = SINGLETONINSTANCE(CEffectEditor);
	m_bEditorStarted = false;
}

CEffectEditorCmdTarget::~CEffectEditorCmdTarget()
{
}


BEGIN_MESSAGE_MAP(CEffectEditorCmdTarget, CCmdTarget)
	ON_COMMAND(ID_TOOLS_EFFECTEDITOR, OnToolsEffecteditor)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_EFFECTEDITOR, OnUpdateToolsEffecteditor)
END_MESSAGE_MAP()


void CEffectEditorCmdTarget::OnToolsEffecteditor()
{
	m_bEditorStarted = !m_bEditorStarted;
	if (m_bEditorStarted)
		m_pEditor->StartEditor();
	else
		m_pEditor->StopEditor();
}

void CEffectEditorCmdTarget::OnUpdateToolsEffecteditor(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck( m_bEditorStarted );
}
