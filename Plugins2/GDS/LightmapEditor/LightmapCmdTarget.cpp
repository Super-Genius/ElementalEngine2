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

// CLightmapCmdTarget

IMPLEMENT_DYNAMIC(CLightmapCmdTarget, CCmdTarget)
CLightmapCmdTarget::CLightmapCmdTarget()
{
	m_bLightmapMode = false;
	m_ToolBox = EngineGetToolBox();
}

CLightmapCmdTarget::~CLightmapCmdTarget()
{
}


BEGIN_MESSAGE_MAP(CLightmapCmdTarget, CCmdTarget)
	ON_COMMAND(IDR_RENDERLIGHTMAPVIEWER, OnRenderLightmap)
	ON_UPDATE_COMMAND_UI(IDR_RENDERLIGHTMAPVIEWER, OnUpdateRenderLightmap)
END_MESSAGE_MAP()



void CLightmapCmdTarget::OnRenderLightmap()
{
	//m_bLightmapMode = !m_bLightmapMode;
    //Pop up the dialog
	//creation dialog
	if( !m_LightmapDialog.GetSafeHwnd() )
	{
		m_LightmapDialog.Create( IDD_LIGHTMAPEDITORDIALOG, AfxGetApp()->GetMainWnd() );
	}
	else
	{
		m_LightmapDialog.ShowWindow( SW_SHOW );
		m_LightmapDialog.BringWindowToTop();
	}
	static DWORD msgHash_VisualizeLightmap = CHashString(_T("VisualizeLightmap")).GetUniqueID();
	//m_ToolBox->SendMessage(msgHash_VisualizeLightmap, sizeof(m_bLightmapMode), &m_bLightmapMode, NULL, &CHashString(_T("CLightmapObject")) );
}

void CLightmapCmdTarget::OnUpdateRenderLightmap(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bLightmapMode);
}

