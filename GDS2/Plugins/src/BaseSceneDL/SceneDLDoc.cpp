// SceneDLDoc.cpp : implementation file
//

#include "stdafx.h"

REGISTER_SCENE_MESSAGES();

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSceneDLDoc

IMPLEMENT_DYNCREATE(CSceneDLDoc, CDocument)

CSceneDLDoc::CSceneDLDoc()
{
	TCHAR tPath[_MAX_PATH+1];
	TCHAR oldPath[_MAX_PATH+1];
	LPTSTR fileName;

	DWORD dwReturnSize = GetModuleFileName(BaseSceneDLDLL.hModule, tPath, _MAX_PATH);
	fileName = _tcsrchr(tPath, '\\');
	if (fileName != NULL)
	{
		*fileName = '\0';
	}
	else
	{
		_tcscpy(tPath, ".");
	}

	GetCurrentDirectory(_MAX_PATH, oldPath);
	SetCurrentDirectory(tPath);
	
	SetCurrentDirectory(tPath);

}

BOOL CSceneDLDoc::OnNewDocument()
{
//	if (!CDocument::OnNewDocument())
//		return FALSE;
	return TRUE;
}

CSceneDLDoc::~CSceneDLDoc()
{
}


BEGIN_MESSAGE_MAP(CSceneDLDoc, CDocument)
	//{{AFX_MSG_MAP(CSceneDLDoc)
	ON_COMMAND(ID_FILE_CLOSE_SCENE, OnFileCloseScene)
	ON_UPDATE_COMMAND_UI(ID_FILE_CLOSE_SCENE, OnUpdateFileCloseScene)
	ON_COMMAND(ID_FILE_OPEN_SCENE, OnFileOpenScene)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPEN_SCENE, OnUpdateFileOpenScene)
	ON_COMMAND(ID_FILE_SAVE_AS_SCENE, OnFileSaveAsScene)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS_SCENE, OnUpdateFileSaveAsScene)
	ON_COMMAND(ID_FILE_SAVE_SCENE, OnFileSaveScene)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_SCENE, OnUpdateFileSaveScene)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSceneDLDoc diagnostics

#ifdef _DEBUG
void CSceneDLDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSceneDLDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSceneDLDoc serialization

void CSceneDLDoc::Serialize(CArchive& ar)
{
	CString errMess;
	CString mungeName;			// depot can't have /, space or ()

	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
	}
}

/////////////////////////////////////////////////////////////////////////////
// CSceneDLDoc commands


void CSceneDLDoc::OnFileCloseScene() 
{
	OnCloseDocument();
}

void CSceneDLDoc::OnUpdateFileCloseScene(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_SFXFile != NULL);
}

void CSceneDLDoc::OnFileOpenScene() 
{
}

void CSceneDLDoc::OnUpdateFileOpenScene(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();
}

void CSceneDLDoc::OnFileSaveAsScene() 
{
}

void CSceneDLDoc::OnUpdateFileSaveAsScene(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_SFXFile != NULL);		
}

void CSceneDLDoc::OnFileSaveScene() 
{
	// TODO: Add your command handler code here
	
}

void CSceneDLDoc::OnUpdateFileSaveScene(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_SFXFile != NULL);	
}

void CSceneDLDoc::OnCloseDocument() 
{
	int hr;
	if (IsModified())
	{
		hr = MessageBox(AfxGetApp()->GetMainWnd()->m_hWnd, _T("Unsaved changes to scene!\n"
			"Do you want to save your changes?\n"), _T("Unsaved Scene Warning!"),
			MB_YESNOCANCEL);
		switch (hr)
		{
		case IDCANCEL:
			return;
			break;
		case IDYES:
			OnFileSaveScene();
			delete m_SFXFile;
			m_SFXFile = NULL;
			m_SFXLen = 0;
			break;
		case IDNO:
			// Call clear scene from tree view here!
			break;
		}
	}

	CDocument::OnCloseDocument();
}

