// AddEditLanguageBox.cpp : implementation file
//

#include "stdafx.h"
#include "GUIEditor.h"
#include "AddEditLanguageBox.h"


// CAddEditLanguageBox dialog

IMPLEMENT_DYNAMIC(CAddEditLanguageBox, CDialog)

CAddEditLanguageBox::CAddEditLanguageBox(CWnd* pParent /*=NULL*/)
	: CDialog(CAddEditLanguageBox::IDD, pParent)
{
	m_bAddMode = false;
	m_Parent = NULL;
}

CAddEditLanguageBox::~CAddEditLanguageBox()
{
}

void CAddEditLanguageBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LANGNAMEEDIT, m_LanguageNameEdit);
	DDX_Control(pDX, IDC_LANGFILEEDIT, m_LanguageFileEdit);
}


BEGIN_MESSAGE_MAP(CAddEditLanguageBox, CDialog)
	ON_BN_CLICKED(IDOK, &CAddEditLanguageBox::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CAddEditLanguageBox::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_LANGFILEOPENBT, &CAddEditLanguageBox::OnBnClickedLangfileopenbt)
END_MESSAGE_MAP()


// CAddEditLanguageBox message handlers

void CAddEditLanguageBox::OnBnClickedOk()
{
	if (m_Parent)
	{
		StdString szName, szFile;
		TCHAR buf[2048];
		m_LanguageNameEdit.GetWindowTextA(buf, 2048);
		szName = buf;

		m_LanguageFileEdit.GetWindowTextA(buf, 2048);
		szFile = buf;

		if (_tcscmp(m_szLangNameIn, szName) != 0)
		{
			if (m_Parent->CheckNameInUse(szName) == true)
			{
				MessageBox(_T("Language name is already in use!\n"), _T("Nope"), MB_OK);
				return;
			}
		}

		if (m_bAddMode)
		{
			m_Parent->AddLanguageEntry(szName, szFile);
		}
		else
		{
			m_Parent->EditLanguageEntry(szName, szFile);
		}

		OnOK();
	}
	else
	{
		OnOK();
	}
}

void CAddEditLanguageBox::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

void CAddEditLanguageBox::OnBnClickedLangfileopenbt()
{
	TCHAR filePathName[MAX_PATH] = {'\0'};
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = GetSafeHwnd();
	ofn.lpstrTitle = "Get Language File";
	ofn.lpstrFilter = "Language Files (*.lxl)\0*.lxl\0All Files (*.*)\0*.*\0";
	ofn.lpstrFile = filePathName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
	ofn.lpstrDefExt = "lxl";
	if(!GetOpenFileName(&ofn))
	{
		return;
	}

	StdString szResource;
	EngineGetToolBox()->GetDirectories(NULL, &szResource);

	StdString szFile = filePathName;
	szResource.MakeSafeFileName();
	szFile.MakeSafeFileName();

	// make sure we add a trailing '\' to ensure the relative path does not have a leading '\'
	UINT resourceLen = _tcslen(szResource);
	TCHAR lastChar = szResource[resourceLen-1];
	if (lastChar != _T('\\'))
	{
		szResource += _T("\\");
	}

	TCHAR *loc = (TCHAR*)_tcsstr(szFile, szResource);
	if (loc)
	{
		TCHAR *relativePath = &szFile[resourceLen+1];
		szFile = relativePath;

		m_LanguageFileEdit.SetWindowTextA(szFile);
	}
	else
	{
		EngineGetToolBox()->Log(LOGWARNING, _T("File is outside of resource path; not setting\n"));
	}
}

void CAddEditLanguageBox::SetLanguageData(StdString szLangName, StdString szFileName)
{
	m_LanguageNameEdit.SetWindowTextA(szLangName);
	m_LanguageFileEdit.SetWindowTextA(szFileName);

	m_szLangNameIn = szLangName;
}