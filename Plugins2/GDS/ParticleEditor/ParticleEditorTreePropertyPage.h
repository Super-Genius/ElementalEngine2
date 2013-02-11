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
#include "afxcmn.h"

class CParticleDoc;
// CParticleEditorTreePropertyPage dialog

class CParticleEditorTreePropertyPage : public CPropertyPage
{
	// Dialog Data
	enum { IDD = IDD_PARTICLE_TREEPROPERTYPAGE };
public:
	CParticleEditorTreePropertyPage();
	virtual ~CParticleEditorTreePropertyPage();

	void GetWorkingData(bool &bEmitter, int &iBrush, PARTICLE_EMITTER_VARIABLE &eEmitterProperty, PARTICLE_VARIABLE &eBrushProperty );
	void GetItemAndParentStrings(StdString * szItem, StdString * szItemParent);
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);

	void RefreshList(CParticleDoc *pDocument);
protected:
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTvnSelchangedParticleTreedockbarctrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);

private:
	void ShowPopupMenu( CPoint& point );
	void AddStringsToTree(HTREEITEM hParent, LPCTSTR *szFirstName, LPCTSTR *szLastName);

private:
	CTreeCtrl m_TreeCtrl;
};
