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

// CNavMeshCmdTarget

IMPLEMENT_DYNAMIC(CNavMeshCmdTarget, CCmdTarget)
CNavMeshCmdTarget::CNavMeshCmdTarget()
{
	m_bNavMeshMode = false;
	m_ToolBox = EngineGetToolBox();
}

CNavMeshCmdTarget::~CNavMeshCmdTarget()
{
}


BEGIN_MESSAGE_MAP(CNavMeshCmdTarget, CCmdTarget)
	ON_COMMAND(ID_RENDER_NAVMESH, OnRenderNavmesh)
	ON_UPDATE_COMMAND_UI(ID_RENDER_NAVMESH, OnUpdateRenderNavmesh)
END_MESSAGE_MAP()



void CNavMeshCmdTarget::OnRenderNavmesh()
{
	m_bNavMeshMode = !m_bNavMeshMode;
	static DWORD msgHash_VisualizeNavMesh = CHashString(_T("VisualizeNavMesh")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_VisualizeNavMesh, sizeof(m_bNavMeshMode), &m_bNavMeshMode, NULL, &CHashString(_T("CNavMeshObject")) );
}

void CNavMeshCmdTarget::OnUpdateRenderNavmesh(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bNavMeshMode);
}

