#pragma once
#include "afxwin.h"


// CGUIProperties dialog

class CGUIProperties : public CDialog
{
	DECLARE_DYNAMIC(CGUIProperties)

public:
	CGUIProperties(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGUIProperties();

// Dialog Data
	enum { IDD = IDD_GUIPROPERTIES };

	virtual BOOL OnInitDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnUpdateProperties(NMHDR* pNotifyStruct, LRESULT* plResult);
	CStatic m_PropertiesSpot;
	ObjectDockWin *m_DockWin;
	afx_msg void OnBnClickedButton1();
};
