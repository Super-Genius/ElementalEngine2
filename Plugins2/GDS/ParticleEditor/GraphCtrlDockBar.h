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


class CGraphCtrlDockBar : public CGuiControlBar
{
	DECLARE_DYNCREATE(CGraphCtrlDockBar)

	enum { IDD = ID_VIEW_GRAPH };

public:
	CGraphCtrlDockBar();
	~CGraphCtrlDockBar();
	void UpdateGraph(CParticleDoc * pDoc);
	void ApplyChanges(float lasteffectedkeytime);
	void GraphUpdateComplete(float lasteffectedkeytime){  }
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CGraph			m_Graph;
	IToolBox		*m_ToolBox;

private:

};
