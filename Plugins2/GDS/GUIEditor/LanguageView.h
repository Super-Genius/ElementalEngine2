#pragma once
#include "afxwin.h"


// CLanguageView dialog

class CLanguageView : public CDialog
{
	DECLARE_DYNAMIC(CLanguageView)

public:
	CLanguageView(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLanguageView();

// Dialog Data
	enum { IDD = IDD_LANGUAGEVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	bool m_bLanguagesLoaded;
	StdString m_szSelectedLanguage;
	CAddEditLanguageBox m_AddEditLangBox;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnLbnSelchangeLanguagelist();
	CListBox m_LanguageList;
	afx_msg void OnBnClickedSetactivelang();
	afx_msg void OnBnClickedRemovelang();
	afx_msg void OnBnClickedAddlang();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	CStatic m_szActiveLanguage;
	afx_msg void OnBnClickedEditlang();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	void AddLanguageEntry(StdString szLangName, StdString szLangFile);
	void EditLanguageEntry(StdString szLangName, StdString szLangFile);
	bool CheckNameInUse(StdString szName);

	LANGUAGEFILEMAP m_mLanguages;
};
