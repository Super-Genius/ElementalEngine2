#pragma once
#include "afxwin.h"

class CGUIEditorFrm;

// CGUIAddElementBox dialog

class CGUIAddElementBox : public CDialog
{
	DECLARE_DYNAMIC(CGUIAddElementBox)

public:
	CGUIAddElementBox(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGUIAddElementBox();

// Dialog Data
	enum { IDD = IDD_ADDELEMENTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CGUIEditorFrm *m_FrmParent;
	UINT m_iItemNumber;

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_ctElementType;
	CString m_szElementName;
	afx_msg void OnBnClickedOk();

	void SetFrameParent(CGUIEditorFrm *value) { m_FrmParent = value; }
	void SetItemNumber(UINT i) { m_iItemNumber = i; }
	CString m_szElementType;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
