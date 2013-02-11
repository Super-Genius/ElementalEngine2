#pragma once
#include "afxwin.h"


// CGUITestDialog dialog

class CGUITestDialog : public CDialog
{
	DECLARE_DYNAMIC(CGUITestDialog)

public:
	CGUITestDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGUITestDialog();

// Dialog Data
	enum { IDD = IDD_TEST_DIALOG };

	virtual BOOL OnInitDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_TestProperties;
	COptionTree m_otTree;
	COptionTreeItem *m_pRoot;
	ObjectDockWin *m_DockWin;
};
