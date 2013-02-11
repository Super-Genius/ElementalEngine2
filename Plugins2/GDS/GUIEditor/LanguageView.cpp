// LanguageView.cpp : implementation file
//

#include "stdafx.h"
#include "GUIEditor.h"
#include "LanguageView.h"


// CLanguageView dialog

IMPLEMENT_DYNAMIC(CLanguageView, CDialog)

CLanguageView::CLanguageView(CWnd* pParent /*=NULL*/)
	: CDialog(CLanguageView::IDD, pParent)
{
	m_bLanguagesLoaded = false;
	m_szSelectedLanguage = _T("");
}

CLanguageView::~CLanguageView()
{
}

void CLanguageView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LANGUAGELIST, m_LanguageList);
	DDX_Control(pDX, IDC_ACTIVELANGUAGE, m_szActiveLanguage);
}


BEGIN_MESSAGE_MAP(CLanguageView, CDialog)
	ON_BN_CLICKED(IDOK, &CLanguageView::OnBnClickedOk)
	ON_LBN_SELCHANGE(IDC_LANGUAGELIST, &CLanguageView::OnLbnSelchangeLanguagelist)
	ON_BN_CLICKED(IDC_SETACTIVELANG, &CLanguageView::OnBnClickedSetactivelang)
	ON_BN_CLICKED(IDC_REMOVELANG, &CLanguageView::OnBnClickedRemovelang)
	ON_BN_CLICKED(IDC_ADDLANG, &CLanguageView::OnBnClickedAddlang)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_EDITLANG, &CLanguageView::OnBnClickedEditlang)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CLanguageView message handlers

void CLanguageView::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//OnOK();
}

void CLanguageView::OnLbnSelchangeLanguagelist()
{
	int sel = m_LanguageList.GetCurSel();
	TCHAR buf[256];
	m_LanguageList.GetText(sel, buf);
	m_szSelectedLanguage = buf;
}

void CLanguageView::OnBnClickedSetactivelang()
{
	if (_tcscmp(m_szSelectedLanguage, _T("")) != 0)
	{
		CHashString hszLang(m_szSelectedLanguage);
		static DWORD msgSetActiveGUILanguage = CHashString(_T("SetActiveGUILanguage")).GetUniqueID();
		DWORD retval = EngineGetToolBox()->SendMessage(msgSetActiveGUILanguage, sizeof(IHashString*), &hszLang);
		if (retval == MSG_HANDLED)
		{
			m_szActiveLanguage.SetWindowTextA(m_szSelectedLanguage);
		}
	}
}

void CLanguageView::OnBnClickedRemovelang()
{
	if (_tcscmp(m_szSelectedLanguage, _T("")) != 0)
	{
		CHashString hszLang(m_szSelectedLanguage);
		static DWORD msgRemoveGUILanguage = CHashString(_T("RemoveGUILanguage")).GetUniqueID();
		DWORD retval = EngineGetToolBox()->SendMessage(msgRemoveGUILanguage, sizeof(IHashString*), &hszLang);
		if (retval == MSG_HANDLED)
		{
			TCHAR buf[256];
			m_szActiveLanguage.GetWindowTextA(buf, 256);
			if (_tcscmp(buf, m_szSelectedLanguage) == 0)
			{
				static DWORD msgSetActiveGUILanguage = CHashString(_T("SetActiveGUILanguage")).GetUniqueID();
				DWORD retval = EngineGetToolBox()->SendMessage(msgSetActiveGUILanguage, sizeof(IHashString*), NULL);

				m_szActiveLanguage.SetWindowTextA(_T("- not set -"));
			}

			m_LanguageList.DeleteString(m_LanguageList.GetCurSel());

			LANGUAGEFILEMAP::iterator mapIter = m_mLanguages.find(hszLang.GetUniqueID());
			if (mapIter != m_mLanguages.end())
			{
				m_mLanguages.erase(mapIter);
			}
		}
	}
}

void CLanguageView::OnBnClickedAddlang()
{
	m_AddEditLangBox.SetLanguageData(_T(""), _T(""));
	m_AddEditLangBox.SetAddMode(true);
	m_AddEditLangBox.ShowWindow(SW_SHOWNORMAL);
	m_AddEditLangBox.SetActiveWindow();
	m_AddEditLangBox.BringWindowToTop();
}

void CLanguageView::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if (!m_bLanguagesLoaded)
	{
		static DWORD msgGetGUILanguages = CHashString(_T("GetGUILanguages")).GetUniqueID();
		EngineGetToolBox()->SendMessage(msgGetGUILanguages, sizeof(LANGUAGEFILEMAP), &m_mLanguages);

		LANGUAGEFILEMAP::iterator mapIter = m_mLanguages.begin();
		for (; mapIter != m_mLanguages.end(); mapIter++)
		{
			StdString szName = EngineGetToolBox()->GetHashString(mapIter->first);
			m_LanguageList.AddString(szName);
		}

		m_bLanguagesLoaded = true;
		m_szActiveLanguage.SetWindowTextA(_T("- not set -"));
	}
}

void CLanguageView::OnBnClickedEditlang()
{
	StdString szName = m_szSelectedLanguage;
	CHashString hszName(szName);
	LANGUAGEFILEMAP::iterator mapIter = m_mLanguages.find(hszName.GetUniqueID());
	if (mapIter != m_mLanguages.end())
	{
		StdString szFile = mapIter->second;
		m_AddEditLangBox.SetLanguageData(szName, szFile);
		m_AddEditLangBox.SetAddMode(false);
		m_AddEditLangBox.ShowWindow(SW_SHOWNORMAL);
		m_AddEditLangBox.SetActiveWindow();
		m_AddEditLangBox.BringWindowToTop();
	}
}

int CLanguageView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_AddEditLangBox.Create(IDD_ADDEDITLANG, this);
	m_AddEditLangBox.SetLangViewParent(this);

	return 0;
}

void CLanguageView::AddLanguageEntry(StdString szLangName, StdString szLangFile)
{
	static DWORD msgAddGUILanguage = CHashString(_T("AddGUILanguage")).GetUniqueID();
	CHashString hszName(szLangName), hszFile(szLangFile);
	ADDGUILANGUAGEPARAMS aglp;
	aglp.dwHashLangName = hszName.GetUniqueID();
	aglp.dwHashLangFile = hszFile.GetUniqueID();
	
	DWORD retval = EngineGetToolBox()->SendMessage(msgAddGUILanguage, sizeof(ADDGUILANGUAGEPARAMS), &aglp);
	if (retval == MSG_HANDLED)
	{
		m_mLanguages[hszName.GetUniqueID()] = szLangFile;
		m_LanguageList.AddString(szLangName);
	}
}

void CLanguageView::EditLanguageEntry(StdString szLangName, StdString szLangFile)
{
	int curSel = m_LanguageList.GetCurSel();
	TCHAR buf[256];
	m_LanguageList.GetText(curSel, buf);
	CHashString hszOldName(buf);
	// easiest way to edit is remove, then add
	CHashString hszName(szLangName), hszFile(szLangFile);
	static DWORD msgRemoveGUILanguage = CHashString(_T("RemoveGUILanguage")).GetUniqueID();
	DWORD retval = EngineGetToolBox()->SendMessage(msgRemoveGUILanguage, sizeof(IHashString*), &hszOldName);
	if (retval == MSG_HANDLED)
	{
		m_LanguageList.DeleteString(curSel);		
		LANGUAGEFILEMAP::iterator mapIter = m_mLanguages.find(hszOldName.GetUniqueID());
		if (mapIter != m_mLanguages.end())
		{
			m_mLanguages.erase(mapIter);
		}
	
		static DWORD msgAddGUILanguage = CHashString(_T("AddGUILanguage")).GetUniqueID();
		ADDGUILANGUAGEPARAMS aglp;
		aglp.dwHashLangName = hszName.GetUniqueID();
		aglp.dwHashLangFile = hszFile.GetUniqueID();
		
		retval = EngineGetToolBox()->SendMessage(msgAddGUILanguage, sizeof(ADDGUILANGUAGEPARAMS), &aglp);
		if (retval == MSG_HANDLED)
		{
			m_mLanguages[hszName.GetUniqueID()] = szLangFile;

			m_LanguageList.InsertString(curSel, szLangName);
		}

		TCHAR buf[256];
		m_szActiveLanguage.GetWindowTextA(buf, 256);
		if (_tcscmp(buf, hszOldName.GetString()) == 0)
		{
			static DWORD msgSetActiveGUILanguage = CHashString(_T("SetActiveGUILanguage")).GetUniqueID();
			DWORD retval = EngineGetToolBox()->SendMessage(msgSetActiveGUILanguage, sizeof(IHashString*), &hszName);

			m_szActiveLanguage.SetWindowTextA(szLangName);
		}
	}
}

bool CLanguageView::CheckNameInUse(StdString szName)
{
	CHashString hszName(szName);
	
	LANGUAGEFILEMAP::iterator mapIter = m_mLanguages.find(hszName.GetUniqueID());
	
	return (mapIter != m_mLanguages.end());
}