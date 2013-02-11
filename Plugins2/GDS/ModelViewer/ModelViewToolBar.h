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

#ifndef MODELVIEWTOOLBAR_H
#define MODELVIEWTOOLBAR_H

class CModelViewToolBar : public CGuiDialogBar
{
	DECLARE_DYNAMIC(CModelViewToolBar);
	
	enum { IDD = IDD_MODELVIEW_TB };

public:
	CModelViewToolBar();
	virtual ~CModelViewToolBar();

protected:
	DECLARE_MESSAGE_MAP()

	virtual LRESULT OnInitDialog(WPARAM wParam, LPARAM lParam);
	virtual void DoDataExchange(CDataExchange* pDX);

	//{{AFX_MSG(CModelViewToolBar)
	//}}AFX_MSG

private:
	IToolBox*	m_ToolBox;
	CButton m_ZoomExtentsBt;
};

#endif /* MODELVIEWTOOLBAR_H */