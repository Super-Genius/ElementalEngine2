///============================================================================
/// \file		StateMachineEditorDoc.cpp
/// \brief		Class implementation for CStateMachineEditorDoc
/// \date		03-06-2007
/// \author		Richard Nguyen
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
// CStateMachineEditorDoc

IMPLEMENT_DYNCREATE(CStateMachineEditorDoc, CDocument)

CStateMachineEditorDoc::CStateMachineEditorDoc() :
	m_hszStateMachineType( _T("CQHStateMachine") )
{
	m_ToolBox = EngineGetToolBox();
	m_hszStateMachineName.Init( _T("") );
}

CStateMachineEditorDoc::~CStateMachineEditorDoc()
{
}

BEGIN_MESSAGE_MAP(CStateMachineEditorDoc, CDocument)
	//{{AFX_MSG_MAP(CStateMachineEditorDoc)
	// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CStateMachineEditorDoc::OnNewDocument()
{
	BOOL retVal = CDocument::OnNewDocument();
	if (retVal)
	{
		m_hszStateMachineName = CreateStateMachine();
		CreateUndoStack( &m_hszStateMachineName );
	}

	return retVal;
}

BOOL CStateMachineEditorDoc::OnOpenDocument( LPCTSTR lpszPathName )
{
	BOOL retVal = CDocument::OnOpenDocument( lpszPathName );
	if( retVal )
	{
		// Getting some flakey behavior with passing CHashString around.
		CHashString hszStateMachineName( _T("") );
		CHashString hszStateMachineFilePath( lpszPathName );

		GETSTATEMACHINEPARAMS getStateMachineParams;
		getStateMachineParams.StateMachineName = &hszStateMachineName;
		getStateMachineParams.StateMachineFilePath = &hszStateMachineFilePath;

		static DWORD msgHash_GetStateMachineName = CHashString(_T("GetStateMachineName")).GetUniqueID();
		m_ToolBox->SendMessage( msgHash_GetStateMachineName, sizeof(GETSTATEMACHINEPARAMS), &getStateMachineParams );
		m_hszStateMachineName = getStateMachineParams.StateMachineName;

		CreateUndoStack( &m_hszStateMachineName );
	}

	return retVal;
}

BOOL CStateMachineEditorDoc::OnSaveDocument( LPCTSTR lpszPathName )
{
	BOOL retVal = TRUE;
	POSITION pos = GetFirstViewPosition();
	if( pos != NULL )
	{
		CStateMachineEditorView *pView;
		pView = dynamic_cast<CStateMachineEditorView *>( GetNextView( pos ) );
		ASSERT( pView != NULL );
		if( !pView->IsValid() )
		{
			CString msg = FormatString( IDS_CONFIRM_SAVE_INVALID_MACHINE );
			CString title = FormatString( IDS_CONFIRM );
			if( MB_YESNO == pView->MessageBox( msg, title, MB_YESNO | MB_ICONINFORMATION ) )
			{
				retVal = FALSE;
			}
		}
	}

	if( retVal )
	{
		retVal = CDocument::OnSaveDocument( lpszPathName );
	}

	if( retVal )
	{
		CHashString hszFileName( lpszPathName );
		static DWORD msgHash_MapToFileName = CHashString( _T("MapToFileName") ).GetUniqueID();
		m_ToolBox->SendMessage( msgHash_MapToFileName, sizeof(IHashString), &hszFileName, &m_hszStateMachineName, &m_hszStateMachineType );

		static DWORD msgHash_SaveFileByExtension = CHashString( _T("SaveFileByExtension") ).GetUniqueID();
		m_ToolBox->SendMessage( msgHash_SaveFileByExtension, sizeof(LPTSTR), (LPTSTR)lpszPathName );
	}

	return retVal;
}

void CStateMachineEditorDoc::OnCloseDocument()
{
	DeleteUndoStack( &m_hszStateMachineName );
	CDocument::OnCloseDocument();
	SINGLETONINSTANCE(CStateMachineEditor)->ShowToolBar(false);
}

IHashString *CStateMachineEditorDoc::GetStateMachineName()
{
	return &m_hszStateMachineName;
}

/////////////////////////////////////////////////////////////////////////////
// state machine object creation functions

///	\brief	create new state machine
///	\return	name of the newly created state machine
CHashString CStateMachineEditorDoc::CreateStateMachine()
{
	static CHashString hszType = _T("CQHStateMachine");
	CHashString hszName = FindUniqueEEObjectName( _T("StateMachine") );

	IArchive *archive = CreateMemoryArchive();
	archive->Write( 1.2f, _T("version") );
	archive->Write( hszName.GetString(), _T("name") );
	archive->Write( 0, _T("numStates") );
	archive->Write( 0, _T("startState") );
	archive->Write( 0, _T("endState") );

	archive->SetIsWriting( false );
	archive->SeekTo( 0 );

	CreateEEObject( NULL, &hszType, hszName, archive );
	archive->Close();

	return hszName;
}

///	\brief	create new state machine state
///	\param	parentName - name of the parent object
///	\return	name of the newly created state
CHashString CStateMachineEditorDoc::CreateState( IHashString *parentName ) const
{
	static CHashString hszType = _T("CQHState");
	CHashString hszName = GenerateName( &hszType );

	IArchive *archive = CreateMemoryArchive();
	archive->Write( 1.2f, _T("version") );
	archive->Write( hszName.GetString(), _T("name") );
	archive->Write( _T(""), _T("entryEvent") );
	archive->Write( _T(""), _T("updateEvent") );
	archive->Write( _T(""), _T("exitEvent") );

	archive->SetIsWriting( false );
	archive->SeekTo( 0 );

	CreateEEObject( parentName, &hszType, hszName, archive );
	archive->Close();

	return hszName;
}

///	\brief	create new state machine event
///	\param	parentName - name of the parent object
///	\return	name of the newly created state
CHashString CStateMachineEditorDoc::CreateEvent( IHashString *parentName ) const
{
	static CHashString hszType = _T("CQHStateMachineEvent");
	CHashString hszName = GenerateName( &hszType );

	IArchive *archive = CreateMemoryArchive();
	archive->Write( 1.2f, _T("version") );
	archive->Write( hszName.GetString(), _T("name") );

	archive->SetIsWriting( false );
	archive->SeekTo( 0 );

	CreateEEObject( parentName, &hszType, hszName, archive );
	archive->Close();

	return hszName;
}

///	\brief	create new state machine transition
///	\param	parentName - name of the parent object
///	\param	szTargetState - name of the target state
///	\return	name of the newly created state
CHashString CStateMachineEditorDoc::CreateTransition( IHashString *parentName, LPCTSTR szTargetState ) const
{
	static CHashString hszType = _T("CQHStateMachineTransition");
	CHashString hszName = GenerateName( &hszType );

	IArchive *archive = CreateMemoryArchive();
	archive->Write( 1.0f, _T("version") );
	archive->Write( hszName.GetString(), _T("name") );
	archive->Write( 1, _T("weight") );
	archive->Write( szTargetState, _T("targetState") );

	archive->SeekTo( 0 );
	archive->SetIsWriting( false );

	CreateEEObject( parentName, &hszType, hszName, archive );
	archive->Close();

	return hszName;
}

///	\brief	create unique object name with state machine name as prefix.
///			this matches to state machine naming convention.
///	\param	type - type of object to find name
///	\return	unique name for the object
CHashString CStateMachineEditorDoc::GenerateName( IHashString *type ) const
{
	StdString sPrefix = m_hszStateMachineName.GetString();
	sPrefix += _T('.');
	sPrefix += type->GetString();
	return FindUniqueEEObjectName( sPrefix ).c_str();
}
