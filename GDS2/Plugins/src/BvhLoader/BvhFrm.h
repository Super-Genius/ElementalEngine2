#pragma once
#include "BvhTreeCtrl.h"
#include "BvhTreeDlg.h"

// CBvhFrm frame

class CBvhFrm : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CBvhFrm)
	void DrawHeirarchy(  CBvhDoc * pDoc );
	void BvhRecurseForwardPrint(BvhNode * bNode, CRect * pRect, HTREEITEM hParentItem);

protected:
	CBvhFrm();           // protected constructor used by dynamic creation
	virtual ~CBvhFrm();
	CTreeCtrl * BvhTreeControl;
	CBvhTreeDlg m_BvhTreeDlg;
	HINSTANCE m_AppInst;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	CBvhTreeDlg * GetBvhTreeDlg();
};


