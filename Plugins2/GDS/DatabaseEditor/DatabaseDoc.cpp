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
// CDatabaseDoc

IMPLEMENT_DYNCREATE(CDatabaseDoc, CDocument)

CDatabaseDoc::CDatabaseDoc()
{
	m_ToolBox = EngineGetToolBox();
}

CDatabaseDoc::~CDatabaseDoc()
{
}

BEGIN_MESSAGE_MAP(CDatabaseDoc, CDocument)
	//{{AFX_MSG_MAP(CDatabaseDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdateFileSaveAs)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDatabaseDoc commands

void CDatabaseDoc::OnCloseDocument()
{
	DeleteUndoStack(GetUndoStackName());
	// removes doc from in the doc template
	CDocument::OnCloseDocument();

	// get the main instance
	CDatabaseEditor *theEditor = SINGLETONINSTANCE(CDatabaseEditor);
	ASSERT(theEditor != NULL);

	// update toolbar visibility
	theEditor->HideToolbar();
}

BOOL CDatabaseDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	CDatabaseEditor *pEditor = SINGLETONINSTANCE(CDatabaseEditor);
	StdString schemaName = *(pEditor->GetCurrentSchema());
	VIEWOBJECTLIST *pList = pEditor->GetViewObjectList(schemaName);
	if (pList == NULL)
	{
		ASSERT("!Invalid scheme name was passed");
		return FALSE;
	}

	m_SchemaName = schemaName;
	m_DatabaseInfo.m_FileName = NULL;
	m_DatabaseInfo.m_nItems = 0;
	m_DatabaseInfo.m_DatabaseData = NULL;
	m_DatabaseInfo.m_SchemaName = &m_SchemaName;
	m_DatabaseInfo.m_nRowSize = pList->size();
	// generate name for undo stack
	TCHAR nameBuffer[] = _T("DatabaseXXXXXXXX");
	_ltot(reinterpret_cast<long>(this), nameBuffer + _countof(_T("Database")-1), 16);
	m_UndoStackName = nameBuffer;

	CreateUndoStack(GetUndoStackName());

	return TRUE;
}

BOOL CDatabaseDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	m_hszFileName = lpszPathName;

	DeleteContents();
	SetModifiedFlag(FALSE);			// make clean

	m_DatabaseInfo.m_FileName = &m_hszFileName;
	// now grab the data from the loader
	static DWORD msgHash_GetDatabase = CHashString(_T("GetDatabase")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_GetDatabase, sizeof(DATABASEINFO), &m_DatabaseInfo)
		!= MSG_HANDLED)
	{
		CString sMessage;
		sMessage.Format(IDS_ERR_CANT_FIND_DB, lpszPathName);
		MessageBox(NULL, sMessage, _T("Error!"), MB_ICONERROR);
		return FALSE;
	}

	CDatabaseEditor *pEditor = SINGLETONINSTANCE(CDatabaseEditor);
	StdString sSchema;
	if (m_DatabaseInfo.m_SchemaName != NULL)
	{
		sSchema = m_DatabaseInfo.m_SchemaName->GetString();
	}
	if (pEditor->GetViewObjectList(sSchema) == NULL)
	{
		// Try loading the schema from the engine via DBX before we conclude it does not exist
		if( m_DatabaseInfo.m_SchemaName == NULL || !pEditor->AddSchema( m_DatabaseInfo.m_SchemaName ) )
		{
			CString sMessage;
			sMessage.Format(IDS_ERR_UNKNOWN_SCHEMA, sSchema.c_str());
			MessageBox(NULL, sMessage, _T("Error!"), MB_ICONERROR);
			return FALSE;
		}
	}

	SetPathName(lpszPathName);

	m_UndoStackName = lpszPathName;
	CreateUndoStack(GetUndoStackName());

	return TRUE;
}

/// Set the database data and size to new data
/// \param nItems - new size of the database in items
/// \param data - Global alloced new data
void CDatabaseDoc::SetDatabaseData(DWORD nItems, DATABASEDATA *data)
{ 
	m_DatabaseInfo.m_nItems = nItems;
	m_DatabaseInfo.m_DatabaseData = data;

	if (!m_strPathName.IsEmpty())
	{
		// now update the database inside the manager
		static DWORD msgHash_SetDatabase = CHashString(_T("SetDatabase")).GetUniqueID();
		if (m_ToolBox->SendMessage(msgHash_SetDatabase, sizeof(DATABASEINFO), &m_DatabaseInfo)
			!= MSG_HANDLED)
		{
			StdString errMess;
			errMess = _T("Unable to Set the data base for file ");
			errMess += m_DatabaseInfo.m_FileName->GetString(); 
			errMess += _T("in manager.\n");
			MessageBox(NULL, errMess.c_str(), _T("Error!"), MB_OK);
			return;
		}
	}
}

BOOL CDatabaseDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	bool filenameIsChanged = 0 != GetPathName().CompareNoCase(lpszPathName);
	if (filenameIsChanged && !GetPathName().IsEmpty())
	{
		// TODO: We need to do rename in the Database plugin of allocate
		// another data copy for another database object, etc.
		MessageBox(NULL, _T("Database renaming is not supported"), _T("Error"), MB_ICONERROR);
		return FALSE;
	}

	DWORD retVal;
	CDatabaseView *dbView = GetView();

	// make sure view saves off the data, because view has mapping
	// of header control names to datasize, tags
	dbView->SaveData();

	m_hszFileName = lpszPathName;
	m_DatabaseInfo.m_FileName = &m_hszFileName;

	// first time through or filename change
	if (filenameIsChanged)
	{
		// now add the database inside the manager
		static DWORD msgHash_AddDatabase = CHashString(_T("AddDatabase")).GetUniqueID();
		if (m_ToolBox->SendMessage(msgHash_AddDatabase, sizeof(DATABASEINFO), &m_DatabaseInfo)
			!= MSG_HANDLED)
		{
			StdString errMess;
			errMess = _T("Unable to Set the data base for file ");
			errMess += m_DatabaseInfo.m_FileName->GetString(); 
			errMess += _T(" in manager.\n");
			MessageBox(NULL, errMess.c_str(), _T("Error!"), MB_OK);
			return FALSE;
		}
	}

	static DWORD msgHash_SaveFileByExtension = CHashString(_T("SaveFileByExtension")).GetUniqueID();
	retVal =  m_ToolBox->SendMessage(msgHash_SaveFileByExtension, sizeof(LPTSTR), (void *)lpszPathName );
	if (retVal != MSG_HANDLED)
	{
		return FALSE;
	}

	// db was saved successfully. We may update db name related items now
	if (filenameIsChanged)
	{
		CHashString hszNewName = lpszPathName;
		RenameUndoStack(&m_UndoStackName, &hszNewName);
		// update document path
		SetPathName(lpszPathName);
		// and update db stack name
		m_UndoStackName = lpszPathName;
	}

	return TRUE;
}

CDatabaseView* CDatabaseDoc::GetView()
{
	POSITION pos = GetFirstViewPosition();
	if (pos != NULL)
	{
		return (CDatabaseView*)GetNextView(pos); // get first one
	}

	return NULL;
}

void CDatabaseDoc::OnUpdateFileSave(CCmdUI *pCmdUI)
{
	if (m_DatabaseInfo.m_nItems > 0)
	{
		pCmdUI->Enable();
	}
	else
	{
		pCmdUI->Enable(false);
	}
}

void CDatabaseDoc::OnUpdateFileSaveAs(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

void CDatabaseDoc::OnFileSave()
{
	if (m_strPathName.IsEmpty())
	{
		OnFileSaveAs();
	}
	else
	{
		OnSaveDocument(m_strPathName);
	}

}

void CDatabaseDoc::OnFileSaveAs()
{
	TCHAR *saveName;
	// here we need to select only our file types to save as.
	saveName = SaveFileAsByExtension(_T("Database XML Files (*.dxl)\0" "*.dxl\0"
		"Defined Database files (*.dbx)\0" "*.dbx\0"
		"\0\0"));

	if (saveName != NULL)
	{
		OnSaveDocument(saveName);
	}
}
