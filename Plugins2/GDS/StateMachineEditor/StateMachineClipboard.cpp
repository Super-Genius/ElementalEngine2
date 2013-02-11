///=====================================================================
/// \file	StateMachineClipboard.cpp
/// \brief	Implementation of State Machine editor clipboard
/// \date	12/16/2007
/// \author	Andrey Ivanov
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
///=====================================================================
#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

class CStateMachineClipboardPreprocessor
{
public:
	CStateMachineClipboardPreprocessor()
		: m_pToolBox( EngineGetToolBox() )
	{

	}

	///	\brief	prepare clipboard data and replace dependent names
	///	\param	pParentName - name of the state machine or single state to paste
	///	\param	pArchive - archive with clipboard data
	///	\return	false if archive preprocessing was failed
	bool Prepare( IHashString *pParentName, IXMLArchive *pArchive );

	bool operator()( const StdString &nodeName, StdString &value, const StdString &tag) const;

	///	\brief	determine if top level events are in clipboard data.
	///	\pre	Should be called after Prepare() call only
	///	\return	true if such events exist
	bool HasTopLevelEvents() const
	{
		return !m_clipboardEventLabels.empty();
	}

private:
	///	\brief	rename duplicate labels from clipboard
	///	\param	existingLabels - list with existing state machine object labels
	///	\param	clipboardLabels - clipboard object labels
	///	\param	labelMappings - contains mapping for clipboard labels. map does
	///						    contain non-renamed labels
	void RenameDuplicates( const list<IHashString *> &existingLabels,
						   const vector<StdString> &clipboardLabels,
						   map<StdString, StdString> &labelMappings );

	///	\brief	compare passed list with pointer to IHashString with passed name
	///	\param	names - list with names to compare
	///	\param	name - compared value
	///	\return	true if list contains such name
	static bool HasValue( const list<IHashString *> &names, const StdString &name );

	///	\brief	extract first attribute with name "name" from archive
	///	\param	ar - pointer to XML archive with values
	///	\return name of object or empty string
	StdString GetName( IXMLArchive *pArchive ) const;

private:
	IToolBox *m_pToolBox;
	vector<StdString> m_clipboardStateLabels;
	vector<StdString> m_clipboardEventLabels;
	map<StdString, StdString> m_stateLabelMappings;
	map<StdString, StdString> m_eventLabelMappings;
};

///	\brief	prepare clipboard data and replace dependent names
///	\param	pParentName - name of the state machine or single state to paste
///	\param	pArchive - archive with clipboard data
///	\return	false if archive preprocessing was failed
bool CStateMachineClipboardPreprocessor::Prepare( IHashString *pParentName, IXMLArchive *pArchive )
{
	ASSERT( pParentName != NULL );

	// this container contains state name for each node
	m_clipboardStateLabels.clear();
	m_clipboardEventLabels.clear();
	m_stateLabelMappings.clear();
	m_eventLabelMappings.clear();

	// 1 pass
	StdString sNode;
	pArchive->SeekTo( 1 );
	pArchive->GetNode( sNode );
	int depth = pArchive->GetDepth();
	do
	{
		if( sNode == _T("CQHState") )
		{
			StdString sName = GetName( pArchive );
			// Collect state names
			m_clipboardStateLabels.push_back( sName );
		}
		else if( sNode == _T("CQHStateMachineEvent") )
		{
			if( depth == pArchive->GetDepth() )
			{
				StdString sName = GetName( pArchive );
				// Collect top level event names
				m_clipboardEventLabels.push_back( sName );
			}
		}

		pArchive->GetNode( sNode );
	}
	while( depth <= pArchive->GetDepth() );

	// determine mapping for state labels
	GETSETSTATEMACHINENAMESPARAMS gssmn;
	static DWORD msgHash_GetStateMachineStateLabels = CHashString( _T("GetStateMachineStateLabels") ).GetUniqueID();
	DWORD result = m_pToolBox->SendMessage( msgHash_GetStateMachineStateLabels, sizeof( gssmn ), &gssmn, pParentName );
	if( MSG_HANDLED != result )
	{
		return false;
	}
	RenameDuplicates( gssmn.Names, m_clipboardStateLabels, m_stateLabelMappings );

	// determine mapping for top level event labels
	static DWORD msgHash_GetStateEventLabels = CHashString( _T("GetStateEventLabels") ).GetUniqueID();
	result = m_pToolBox->SendMessage( msgHash_GetStateEventLabels, sizeof( gssmn ), &gssmn, pParentName);
	RenameDuplicates( gssmn.Names, m_clipboardEventLabels, m_eventLabelMappings );

	// 1.2 Rename states where needed
	// 1.3 Collect top level objects placements?
	// 2 pass
	// 2.1 Collect transition target states
	// 2.2 Determine transitions which refer to states outside of clipboard group
	return true;
}

///	\brief	rename duplicate labels from clipboard
///	\param	existingLabels - list with existing state machine object labels
///	\param	clipboardLabels - clipboard object labels
///	\param	labelMappings - contains mapping for clipboard labels. map does
///						    contain non-renamed labels
void CStateMachineClipboardPreprocessor::RenameDuplicates( const list<IHashString *> &existingLabels,
														   const vector<StdString> &clipboardLabels,
														   map<StdString, StdString> &labelMappings )
{
	TCHAR chBuf[4];
	vector<StdString>::const_iterator itClipboard = clipboardLabels.begin();
	for( ; itClipboard != clipboardLabels.end(); ++itClipboard )
	{
		StdString mappedLabel = *itClipboard;
		int counter = 0;
		while( HasValue( existingLabels, mappedLabel ) ||
			   labelMappings.count( mappedLabel ) > 0 ||
			   count( clipboardLabels.begin(), clipboardLabels.end(), mappedLabel ) > 0 )
		{
			_itot( ++counter, chBuf, 10 );
			mappedLabel = *itClipboard + chBuf;
		}
		if( counter > 0 )
		{
			labelMappings.insert( make_pair( *itClipboard, mappedLabel ) );
		}
	}
}

bool CStateMachineClipboardPreprocessor::operator()( const StdString &nodeName,
													 StdString &value,
													 const StdString &tag) const
{
	map<StdString, StdString>::const_iterator it;
	if( nodeName == _T("CQHState") )
	{
		if( tag == _T("name") )
		{
			it = m_stateLabelMappings.find( value );
			if( it != m_stateLabelMappings.end() )
			{
				value = it->second;
				return true;
			}
		}
	}
	else if( nodeName == _T("CQHStateMachineEvent") )
	{
		if( tag == _T("name") )
		{
			it = m_eventLabelMappings.find( value );
			if( it != m_eventLabelMappings.end() )
			{
				value = it->second;
				return true;
			}
		}
	}
	else if( nodeName == _T("CQHStateMachineTransition") )
	{
		if( tag == _T("targetState") )
		{
			it = m_stateLabelMappings.find( value );
			if( it != m_stateLabelMappings.end() )
			{
				value = it->second;
				return true;
			}
			// remove transitions without corresponding state in clipboard
			return m_clipboardStateLabels.end() != find( m_clipboardStateLabels.begin(),
														 m_clipboardStateLabels.end(),
														 value );
		}
	}
	return true;
}

///	\brief	compare passed list with pointer to IHashString with passed name
///	\param	names - list with names to compare
///	\param	name - compared value
///	\return	true if list contains such name
bool CStateMachineClipboardPreprocessor::HasValue( const list<IHashString *> &names,
												   const StdString &name )
{
	list<IHashString *>::const_iterator it = names.begin();
	for( ; it != names.end(); ++it )
	{
		if( name == (*it)->GetString() )
		{
			return true;
		}
	}
	return false;
}

///	\brief	extract first attribute with name "name" from archive
///	\param	pArchive - pointer to XML archive with values
///	\return name of object or empty string
StdString CStateMachineClipboardPreprocessor::GetName( IXMLArchive *pArchive ) const
{
	int count = pArchive->GetNumAttributes();
	StdString sName;
	StdString sValue;
	while( count-- )
	{
		sName = pArchive->GetCurrentNodeName();
		pArchive->Read( sValue, sName );
		if( sName == _T("name") )
		{
			return sValue;
		}
	}
	return StdString();
}

CStateMachineClipboard::CStateMachineClipboard( HWND hOwner )
	: m_pToolBox( EngineGetToolBox() )
	, m_ClipboardFormat( RegisterClipboardFormat( _T("SignatureDevices.EE.StateMachine") ) )
	, m_hOwner( hOwner )
{
}

CStateMachineClipboard::~CStateMachineClipboard()
{
}

///	\brief	copy object hierarchies to clipboard
///	\param	names - list with state machine's object names
///	\return	true if objects were copied to clipboard
bool CStateMachineClipboard::Copy( const list<CString> &names ) const
{
	// we need to create memory stream for accessing XMLArchive content
	IEEStream *pStream = CreateMemoryStream();
	if( pStream == NULL )
	{
		return false;
	}

	bool res = false;

	IXMLArchive *archive = CreateXMLArchiveStream();
	if( archive != NULL )
	{
		archive->SetIsWriting( true );
		// release all allocated resources
		archive->DeInit();
		// and use our stream
		archive->Init( pStream );

		archive->StartClass( _T("StateMachineClipboard") );
		archive->Write( 1.0f, _T("version") );
		archive->StartClass( _T("ParentedObjects") );

		CStateMachineClipboardVisitor visitor( archive );
		VISITHIERARCHYDATA vhd;
		vhd.visitor = &visitor;

		list<CString>::const_iterator itName = names.begin();
		for( ; itName != names.end(); ++itName )
		{
			CHashString	hszName = *itName;
			static DWORD msgHash_VisitHierarchy = CHashString( _T("VisitHierarchy") ).GetUniqueID();
			CHashString hszType = GetComponentType( &hszName );
			m_pToolBox->SendMessage( msgHash_VisitHierarchy, sizeof( vhd ), &vhd, &hszName, &hszType );
		}

		archive->EndClass();
		archive->EndClass();

		res = SetClipboardDataFromStream( pStream );
		archive->Close();
	}
	else
	{
		pStream->Close();
		delete pStream;
	}

	return res;
}

///	\brief	paste objects from clipboard
///	\param	pParentName - name of the state machine or single state to paste
///	\param	object - list with created objects
///	\return	true if objects were pasted from the clipboard
bool CStateMachineClipboard::Paste( IHashString *pParentName, vector<ObjectInfo> &objects ) const
{
	if( !CanPaste() )
	{
		return false;
	}

	IXMLArchive *pArchive = GetClipboardDataArchive();
	if( pArchive == NULL )
	{
		return false;
	}

	if( !IsValidArchive( pArchive ))
	{
		pArchive->Close();
		return false;
	}

	CStateMachineClipboardPreprocessor preprocessor;
	if( !preprocessor.Prepare( pParentName, pArchive ) )
	{
		pArchive->Close();
		return false;
	}

	IXMLArchive *pTransformedArchive = TransformXMLArchive( pArchive, preprocessor );
	if( pTransformedArchive == NULL )
	{
		pArchive->Close();
		return false;
	}

#ifdef _DEBUG
	DumpStream( pTransformedArchive, _T("c:\\stateMachine.transformed.xml") );
#endif

	bool res = CreateObjects( pParentName, pTransformedArchive, objects );
	pTransformedArchive->Close();
	return res;
}

///	report clipboard content availability. 
///	This function checks clipboard format only
///	\return	true if clipboard contains content for state machine
bool CStateMachineClipboard::CanPaste() const
{
	return FALSE != IsClipboardFormatAvailable( m_ClipboardFormat );
}

///	report clipboard content availability. This function checks whole clipboard 
///	content. It may return false if some objects cannot be pasted. i.e. events
///	cannot be pasted as children of state machine object
///	\param	pParentName - name of object to paste
///	\return	true if whole clipboard content cannot be pasted for passed parent
bool CStateMachineClipboard::CanPastePartially( IHashString *pParentName ) const
{
	ASSERT( pParentName != NULL );
	if( !CanPaste() )
	{
		return false;
	}
	static const DWORD hash_CQHState = CHashString( _T("CQHState") ).GetUniqueID();
	if( hash_CQHState != GetComponentType( pParentName ).GetUniqueID() )
	{
		IXMLArchive *pArchive = GetClipboardDataArchive();
		ASSERT( pArchive != NULL );

		CStateMachineClipboardPreprocessor preprocessor;
		VERIFY( preprocessor.Prepare( pParentName, pArchive ) );
		pArchive->Close();
		return preprocessor.HasTopLevelEvents();
	}
	return false;
}

///	\brief	check passed clipboard archive for correctness
///	\param	pArchive - clipboard data archive
///	\return	true if archive is valid
bool CStateMachineClipboard::IsValidArchive( IXMLArchive *pArchive ) const
{
	StdString sNode;
	pArchive->SeekTo( 0 );
	pArchive->GetNode( sNode );
	if( sNode != _T("StateMachineClipboard") )
	{
		// invalid clipboard content
		return false;
	}
	StdString sValue;
	float fVersion;
	pArchive->Read( fVersion, _T("version") );
	if( fVersion != 1.0f )
	{
		// unsupported clipboard data version
		return false;
	}

	pArchive->GetNode( sNode );
	if( sNode != _T("ParentedObjects") )
	{
		// invalid format
		return false;
	}
	return !pArchive->IsEndOfFile();
}

///	\brief	create EE object from passed archive
///	\param	pParentName - name of object to paste
///	\param	pArchive - archive with object data
///	\param	object - list with created objects
///	\return	true if objects were created successfully
bool CStateMachineClipboard::CreateObjects( IHashString *pParentName,
										    IXMLArchive *pArchive,
											vector<ObjectInfo> &objects ) const
{
	vector<CHashString> parents;
	parents.push_back( _T("") );
	parents.push_back( pParentName );
	StdString sType;
	pArchive->SeekTo( 1 );
	pArchive->GetNode( sType );
	while( pArchive->GetDepth() >= 2 )
	{
		parents.resize( pArchive->GetDepth() );
		ObjectInfo info;
		info.hszType = sType;
		info.hszName = info.hszType;
		info.hszParent = parents.back();
		bool fromHierarchy = false;
		static const DWORD hashState = CHashString( _T("CQHState") ).GetUniqueID();
		static const DWORD hashEvent = CHashString( _T("CQHStateMachineEvent") ).GetUniqueID();
		static const DWORD hashAction = CHashString(_T("CQHStateMachineActionHandler")).GetUniqueID();
		static const DWORD hashTransition = CHashString(_T("CQHStateMachineTransition")).GetUniqueID();
		const DWORD hashType = info.hszType.GetUniqueID();
		if( hashType == hashState || hashType == hashEvent ||
			hashType == hashAction || hashType == hashTransition )
		{
			fromHierarchy = true;
		}

		if( !CreateEEObject( &info.hszParent, &info.hszType, info.hszName, pArchive, fromHierarchy ) )
		{
			return false;
		}
		parents.push_back( info.hszName );

		if( fromHierarchy )
		{
			objects.push_back( info );
		}

		pArchive->GetNode( sType );
	}

	return true;
}

IEEStream *CStateMachineClipboard::CreateMemoryStream() const
{
	CREATESTREAM cs;
	cs.streamData = NULL;
	cs.streamSize = 0;
	cs.mode = STREAM_MODE_READ | STREAM_MODE_WRITE;
	static DWORD msgHash_CreateStream_Memory = CHashString(_T("CreateStream_Memory")).GetUniqueID();
	if ( m_pToolBox->SendMessage(msgHash_CreateStream_Memory, sizeof(cs), &cs) != MSG_HANDLED )
	{
		m_pToolBox->Log( LOGWARNING, _T("Failed to create memory stream\n") );
		return NULL;
	}
	return cs.openStream;
}

bool CStateMachineClipboard::SetClipboardDataFromStream( IEEStream *pStream ) const
{
	pStream->SeekTo( 0, SEEK_END );
	long size = pStream->Tell();

	HGLOBAL hClipboardData = GlobalAlloc( GMEM_MOVEABLE, size );
	if( hClipboardData != NULL )
	{
		pStream->SeekTo( 0 );
		LPVOID pData = GlobalLock( hClipboardData );
		size_t readedSize = pStream->Read( pData, size );
#if _DEBUG
		FILE *f = fopen( "C:\\stateMachine.xml", "wt" );
		fwrite( pData, 1, size, f );
		fclose( f );
#endif
		VERIFY( GlobalUnlock( hClipboardData ) == NULL );
		if( size == readedSize )
		{
			if( OpenClipboard( m_hOwner ) )
			{
				VERIFY( SetClipboardData( m_ClipboardFormat, hClipboardData ) != NULL );
				VERIFY( CloseClipboard() );
				return true;
			}
			else
			{
				m_pToolBox->Log( LOGWARNING, _T("Failed to open clipboard\n") );
				GlobalFree( hClipboardData );
			}
		}
		else
		{
			m_pToolBox->Log( LOGERROR, _T("Failed to read clipboard data from memory stream") );
			GlobalFree( hClipboardData );
		}
	}
	else
	{
		m_pToolBox->Log( LOGERROR, _T("Failed to allocate memory for clipboard data") );
	}
	return false;
}

IXMLArchive *CStateMachineClipboard::GetClipboardDataArchive() const
{
	if( !IsClipboardFormatAvailable( m_ClipboardFormat ))
	{
		return NULL;
	}

	if( !OpenClipboard( m_hOwner ) )
	{
		m_pToolBox->Log( LOGWARNING, _T("Failed to open clipboard\n") );
		return NULL;
	}

	IXMLArchive *res = NULL;
	HGLOBAL hClipboardData = GetClipboardData( m_ClipboardFormat );
	SIZE_T size = GlobalSize( hClipboardData );
	if( size > 0)
	{
		LPVOID pData = GlobalLock( hClipboardData );
		if ( pData != NULL )
		{
			IEEStream *pStream = CreateMemoryStream();
			if( pStream != NULL )
			{
				res = CreateXMLArchiveStream();
				if( res != NULL )
				{
					pStream->Write( pData, size );
					res->SetIsWriting( false );
					res->DeInit();
					res->Init( pStream );
				}
				else
				{
					pStream->Close();
					delete pStream;
				}
			}
			else
			{
				m_pToolBox->Log( LOGERROR, _T("Failed to create memory stream\n") );
			}

			GlobalUnlock( hClipboardData );
		}
	}
	VERIFY( CloseClipboard() );
	return res;
}

void CStateMachineClipboard::DumpStream( IXMLArchive *pArchive, LPCTSTR szFilePath ) const
{
	static CHashString streamType( _T("File") );
	CREATEARCHIVE ca;
	ca.streamData = (void *)szFilePath;
	ca.streamSize = 0;
	ca.mode = STREAM_MODE_WRITE;
	ca.streamType = &streamType;

	// create archive
	static DWORD msgHash_CreateXMLArchiveStream = CHashString( _T("CreateXMLArchive") ).GetUniqueID();
	if( m_pToolBox->SendMessage( msgHash_CreateXMLArchiveStream, sizeof( ca ), &ca ) != MSG_HANDLED ) 
	{ 
		return;
	}

	IXMLArchive *pTarget = dynamic_cast<IXMLArchive *>( ca.archive );
	pTarget->SetIsWriting( true );
	CopyXMLArchive( pArchive, pTarget );
	pTarget->Close();
}
