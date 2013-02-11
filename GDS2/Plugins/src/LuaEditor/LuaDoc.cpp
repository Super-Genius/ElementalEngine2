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
#include "stdafx.h"
#include ".\luadoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLuaDoc

IMPLEMENT_DYNCREATE(CLuaDoc, CDocument)

BEGIN_MESSAGE_MAP(CLuaDoc, CDocument)
	//{{AFX_MSG_MAP(CLuaDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLuaDoc construction/destruction

CLuaDoc::CLuaDoc()
{
	// TODO: add one-time construction code here

}

CLuaDoc::~CLuaDoc()
{
}

BOOL CLuaDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CLuaDoc serialization

void CLuaDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
		CLuaEditCtrl* pEditor = GetView()->GetEditor();
		//pEditor->Save(ar.GetFile());
	}
	else
	{
		// TODO: add loading code here
		CLuaEditCtrl* pEditor = GetView()->GetEditor();
		pEditor->Load(ar.GetFile());
	}
}

/////////////////////////////////////////////////////////////////////////////
// CLuaDoc diagnostics

#ifdef _DEBUG
void CLuaDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CLuaDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLuaDoc commands

CLuaView* CLuaDoc::GetView()
{
	POSITION pos = GetFirstViewPosition();
	if (pos != NULL)
		return (CLuaView*)GetNextView(pos); // get first one

	return NULL;
}

BOOL CLuaDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{	
	// since the script is already loaded, don't load it again!
//	if (!CDocument::OnOpenDocument(lpszPathName))
//		return FALSE;
	SCRIPTPARAMS sp;
	CLuaEditCtrl* pEditor;

	CHashString name(lpszPathName);

	// set the file name
	sp.fileName = &name;
	// make sure to get the source script and not the compiled script
	sp.getScriptSource = true;

	// get script based on the file name
	static DWORD msgHash_GetScript = CHashString(_T("GetScript")).GetUniqueID();
	if (EngineGetToolBox()->SendMessage(msgHash_GetScript, sizeof(SCRIPTPARAMS), &sp) == MSG_HANDLED)
	{
		// grab the editor
		pEditor = GetView()->GetEditor();
		// load the data
		pEditor->Load(sp.scriptData, sp.dataSize);
		// set the path name
		SetPathName(lpszPathName);
		// SUCCESS!
		return TRUE;
	}
	else
	{
		// file didn't load for some reason
		return FALSE;
	}
}

// get full path name
CString CLuaDoc::GetName()
{
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];

	_splitpath( m_strPathName, drive, dir, fname, ext );
	return CString(fname);
}

// get the pathname without the extension
CString CLuaDoc::GetPathNameNoExt()
{
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];

	_splitpath( m_strPathName, drive, dir, fname, ext );
	CString d(drive);
	CString dr(dir);
	CString f(fname);
	return CString(d+dr+f);
}

void CLuaDoc::AddBreakPoint(int nLine)
{
	m_breakPoints[nLine] = 1;
	if ( nLine<m_nMinBreakPoint)
		m_nMinBreakPoint = nLine;
	if ( nLine>m_nMaxBreakPoint )
		m_nMaxBreakPoint = nLine;
}

void CLuaDoc::RemoveBreakPoint(int nLine)
{
	m_breakPoints.RemoveKey(nLine);

	m_nMinBreakPoint = 2147483647;
	m_nMaxBreakPoint = 0;

	POSITION pos = m_breakPoints.GetStartPosition();
	int nTemp;
	while (pos != NULL)
	{
		m_breakPoints.GetNextAssoc( pos, nLine, nTemp );
		if ( nLine<m_nMinBreakPoint)
			m_nMinBreakPoint = nLine;
		if ( nLine>m_nMaxBreakPoint )
			m_nMaxBreakPoint = nLine;
	}
}

void CLuaDoc::RemoveAllBreakPoints()
{
	m_nMinBreakPoint = 2147483647;
	m_nMaxBreakPoint = 0;

	m_breakPoints.RemoveAll();
}

BOOL CLuaDoc::PositionBreakPoints(CString &fileName)
{
	if ( !CLuaHelper::LoadDebugLines(this,fileName) )
		return FALSE;

	BOOL bModified = FALSE;
	POSITION pos = m_breakPoints.GetStartPosition();
	int nLine, nTemp, nNearest;
	while (pos != NULL)
	{
		m_breakPoints.GetNextAssoc( pos, nLine, nTemp );
		nNearest = GetNearestDebugLine(nLine);
		if ( nNearest == 0 )
		{
			m_breakPoints.RemoveKey(nLine);
			bModified = TRUE;
		}
		else if ( nLine != nNearest )
		{
			m_breakPoints.RemoveKey(nLine);
			m_breakPoints.SetAt(nNearest, 1);
			bModified = TRUE;
		}
	}

	return bModified;
}

int CLuaDoc::GetNearestDebugLine(int nLine)
{
	int nTemp, nNearest;
	if ( m_debugLines.Lookup(nLine, nTemp) )
		return nLine;

	if ( (nNearest=GetNextDebugLine(nLine)) > 0 )
		return nNearest;

	if ( (nNearest=GetPreviousDebugLine(nLine)) > 0 )
		return nNearest;

	return 0;
}

int CLuaDoc::GetNextDebugLine(int nLine)
{
	int nTemp;
	++nLine;

	while ( nLine<=m_nMaxDebugLine )
		if ( m_debugLines.Lookup(nLine, nTemp) )
			return nLine;
		else
			++nLine;

	return 0;
}

int CLuaDoc::GetPreviousDebugLine(int nLine)
{
	int nTemp;
	--nLine;

	while ( nLine>=m_nMinDebugLine )
		if ( m_debugLines.Lookup(nLine, nTemp) )
			return nLine;
		else
			--nLine;

	return 0;
}

void CLuaDoc::AddDebugLine(int nLine)
{
	m_debugLines[nLine] = 1;
	if ( nLine<m_nMinDebugLine )
		m_nMinDebugLine = nLine;
	if ( nLine>m_nMaxDebugLine )
		m_nMaxDebugLine = nLine;
}

void CLuaDoc::RemoveAllDebugLines()
{
	m_nMinDebugLine = 2147483647;
	m_nMaxDebugLine = 0;

	m_debugLines.RemoveAll();
}

BOOL CLuaDoc::HasBreakPoint(int nLine)
{
	int nTemp;
	return m_breakPoints.Lookup(nLine, nTemp);
}


void CLuaDoc::OnCloseDocument()
{
	SCRIPTPARAMS sp;
	DWORD retVal;
	// get the main instance
	CLuaEditor *theEditor = (CLuaEditor*)GetDLLInterface();
	ASSERT(theEditor);

	StdString filePathName = GetPathName();
	
	CHashString fileName(filePathName);

	sp.fileName = &fileName;

	// attempt to remove the old script from the manager
	static DWORD msgHash_ReleaseScript = CHashString(_T("ReleaseScript")).GetUniqueID();
	retVal = EngineGetToolBox()->SendMessage(msgHash_ReleaseScript, sizeof(SCRIPTPARAMS), &sp );

	if (retVal != MSG_HANDLED)
	{
		EngineGetToolBox()->Log(LOGWARNING, _T("Couldn't release script %s!\n"),
				filePathName.c_str());
	}

	CDocument::OnCloseDocument();
}

BOOL CLuaDoc::DoFileSave()
{
	DWORD retVal;
	CString errMess;
	StdString filePathName = GetPathName();
	
	// get editor
	CLuaEditCtrl* pEditor = GetView()->GetEditor();

	CHashString fileName(filePathName);

	// attempt to remove the old script from the manager
	static DWORD msgHash_RemoveScript = CHashString(_T("RemoveScript")).GetUniqueID();
	retVal = EngineGetToolBox()->SendMessage(msgHash_RemoveScript, sizeof(IHashString), &fileName );

	if (retVal != MSG_HANDLED)
	{
		//Can't save the file using current file path, save as!
		OnFileSaveAsLua();
	}
	else
	{
		// save the file
		static DWORD msgHash_SaveFileByExtension = CHashString(_T("SaveFileByExtension")).GetUniqueID();
		retVal =  EngineGetToolBox()->SendMessage(msgHash_SaveFileByExtension, sizeof(LPTSTR), const_cast<LPTSTR>((LPCTSTR)filePathName) );
		// if save fails
		if (retVal != MSG_HANDLED)
		{
			errMess.Format(_T("Unable to save file %s\nError %d\n"), filePathName, GetLastError());
			MessageBox(AfxGetMainWnd()->GetSafeHwnd(), errMess, _T("Error Saving File"), MB_OK);
			return FALSE;
		}
	}

	// mark as saved
	pEditor->SetSavePoint();

	return TRUE;
}

BOOL CLuaDoc::DoSave(LPCTSTR lpszPathName, BOOL bReplace)
{
	return OnFileSaveAsLua();
}

// for saving a lua file as...
BOOL CLuaDoc::OnFileSaveAsLua()
{
	DWORD retVal;
	CString errMess;
	StdString oldPathName = GetPathName();
	TCHAR filePathName[_MAX_PATH];
	
	_tcscpy(filePathName, oldPathName.c_str());

	OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(ofn); // SEE NOTE BELOW
	ofn.hwndOwner = AfxGetMainWnd()->GetSafeHwnd();
    ofn.lpstrFilter = "Lua File (*.lua)\0*.lua\0All Files (*.*)\0*.*\0";
    ofn.lpstrFile = filePathName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
    ofn.lpstrDefExt = "lua";

    if(!GetSaveFileName(&ofn))
    {
        return FALSE;
    }

	// now actually save the file by extension
	static DWORD msgHash_SaveFileByExtension = CHashString(_T("SaveFileByExtension")).GetUniqueID();
	retVal =  EngineGetToolBox()->SendMessage(msgHash_SaveFileByExtension, sizeof(LPTSTR), filePathName );
	if (retVal != MSG_HANDLED)
	{
		errMess.Format(_T("Unable to save file %s\nError %d\n"), filePathName, GetLastError());
		MessageBox(ofn.hwndOwner, errMess, _T("Error Saving File"), MB_OK);
		return FALSE;
	}

	// Set the PathName to the new File Name
	SetPathName(filePathName);

	// get editor
	CLuaEditCtrl* pEditor = GetView()->GetEditor();

	// mark as saved
	pEditor->SetSavePoint();

	return TRUE;
}
