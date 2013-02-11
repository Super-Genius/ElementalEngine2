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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSceneDLDoc

IMPLEMENT_DYNCREATE(CSceneDLDoc, CDocument)

CSceneDLDoc::CSceneDLDoc()
	: m_hszWorldName(_T("World"))
{
}

CSceneDLDoc::~CSceneDLDoc()
{
}


BEGIN_MESSAGE_MAP(CSceneDLDoc, CDocument)
	//{{AFX_MSG_MAP(CSceneDLDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSceneDLDoc serialization

void CSceneDLDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
	}
	else
	{
	}
}

/////////////////////////////////////////////////////////////////////////////
// CSceneDLDoc commands

void CSceneDLDoc::OnCloseDocument() 
{
	ClearUndoStack(&m_hszWorldName);
	DeleteEEObject(&m_hszWorldName);

	CDocument::OnCloseDocument();
}

BOOL CSceneDLDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
	{
		return FALSE;
	}

	// create new world root
	static CHashString hszWorldType(_T("CWorld"));
	if (!CreateEEObject(NULL, &hszWorldType, m_hszWorldName, NULL))
	{
		EngineGetToolBox()->Log(LOGERROR, _T("Failed to create World Root Object\n"));
		return FALSE;
	}

	ClearUndoStack(&m_hszWorldName);

	return TRUE;
}

BOOL CSceneDLDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
	{
		return FALSE;
	}
	SetPathName(lpszPathName);
	return TRUE;
}
