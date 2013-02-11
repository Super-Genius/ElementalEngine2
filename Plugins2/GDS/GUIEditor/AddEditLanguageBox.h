#pragma once
#include "afxwin.h"


// CAddEditLanguageBox dialog

class CLanguageView;

class CAddEditLanguageBox : public CDialog
{
	DECLARE_DYNAMIC(CAddEditLanguageBox)

public:
	CAddEditLanguageBox(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAddEditLanguageBox();

// Dialog Data
	enum { IDD = IDD_ADDEDITLANG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	bool m_bAddMode;
	CLanguageView *m_Parent;
	StdString m_szLangNameIn;

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_LanguageNameEdit;
	CEdit m_LanguageFileEdit;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedLangfileopenbt();

	void SetLanguageData(StdString szLangName, StdString szFileName);
	void SetAddMode(bool bAddMode) { m_bAddMode = bAddMode; }
	void SetLangViewParent(CLanguageView *parent) { m_Parent = parent; }
};
