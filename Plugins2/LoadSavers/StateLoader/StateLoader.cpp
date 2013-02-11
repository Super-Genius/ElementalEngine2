///============================================================================
/// \file		StateLoader.cpp
/// \brief		Implementation file for State Loader
/// \date		05-17-2007
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

#include "StdAfx.h"

REGISTER_COMPONENT_SINGLETON(CStateLoader);
REGISTER_MESSAGE_HANDLER(LoadFile, OnLoadFile, CStateLoader);
REGISTER_MESSAGE_HANDLER(SaveFile, OnSaveFile, CStateLoader);

CStateLoader::CStateLoader() :
	LoadSaverBase( _T("CStateLoader"), _T("CQHStateMachineManager"), _T("CQHStateMachine") )
{
	RegisterLoadSaveComponent();
}

const TCHAR *CStateLoader::GetLoadDescription()
{
	return _T("State Machine XML Files (*.xms)\0" "*.xms\0" "\0\0");
}

const TCHAR *CStateLoader::GetSaveDescription()
{
	return _T("State Machine XML Files (*.xms)\0" "*.xms\0" "\0\0");
}

bool CStateLoader::GetPrecachableBool( )
{
	return true;
}

CACHABLEQUERYCALLBACK CStateLoader::GetPrecachableFunc()
{
	return NULL;
}

// create function for this loader
IComponent *CStateLoader::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE( CStateLoader );
}

DWORD CStateLoader::SaveFile( IXMLArchive *ar, IHashString *fileName )
{
	CHashString hszStateMachineName( _T("") );
	static CHashString hszStateMachineType( _T("CQHStateMachine") );
	static CHashString hszStateMachineManagerType( _T("CQHStateMachineManager") );
	static DWORD msgHash_GetStateMachineName = CHashString( "GetStateMachineName" ).GetUniqueID();

	GETSTATEMACHINEPARAMS getStateMachineParams;
	getStateMachineParams.StateMachineName = &hszStateMachineName;
	getStateMachineParams.StateMachineFilePath = fileName;

	m_ToolBox->SendMessage( msgHash_GetStateMachineName, sizeof(GETSTATEMACHINEPARAMS), &getStateMachineParams );

	DWORD stateMachineHash = getStateMachineParams.StateMachineName->GetUniqueID();

	IComponent *managerComponent = m_ToolBox->CreateComponent( &hszStateMachineManagerType, 0 );
	CManager *manager = (CManager*)managerComponent;
	IDTOOBJECTMAP *objectMap = manager->GetObjectMap( &hszStateMachineType );

	IObject *stateMachineObject = NULL;
	IDTOOBJECTMAP::iterator objectsIterator = objectMap->find( stateMachineHash );
	if( objectsIterator != objectMap->end() )
	{
		stateMachineObject = objectsIterator->second;
	}

	ar->SetIsWriting( true );
	CSaveVisitor saveVisitor( ar );

	VISITHIERARCHYDATA vhd;
	vhd.visitor = &saveVisitor;

	static DWORD msgHash_VisitHierarchy = CHashString(_T("VisitHierarchy")).GetUniqueID();
	m_ToolBox->SendMessage( msgHash_VisitHierarchy, sizeof(vhd), &vhd, getStateMachineParams.StateMachineName, &hszStateMachineType );

	return MSG_HANDLED_STOP;
}

void CStateLoader::GenerateObjectName( IHashString *objectName, IHashString *nodeType, IXMLArchive *archive )
{
	static const DWORD hashCQHStateMachine = CHashString( _T("CQHStateMachine") ).GetUniqueID();
	static const DWORD hashCQHDerivedState = CHashString( _T("CQHDerivedState") ).GetUniqueID();
	static const DWORD hashCQHDerivedEvent = CHashString( _T("CQHDerivedEvent") ).GetUniqueID();
	const DWORD hashType = nodeType->GetUniqueID();
	if( hashCQHStateMachine != hashType && 
		hashCQHDerivedState != hashType &&
		hashCQHDerivedEvent != hashType )
	{
		StdString sPrefix = m_hszStateMachineName.GetString();
		sPrefix += _T('.');
		sPrefix += nodeType->GetString();
		CHashString hszPrefix(sPrefix.c_str());
		LoadSaverBase::GenerateObjectName( objectName, &hszPrefix, archive );
	}
	else
	{
		LoadSaverBase::GenerateObjectName( objectName, nodeType, archive );
	}
}

void CStateLoader::CreateObject( IXMLArchive * ar, IHashString *parentName, IHashString *type, IHashString *objectName )
{
	static const DWORD hashCQHStateMachine = CHashString( _T("CQHStateMachine") ).GetUniqueID();
	static const DWORD hashCQHDerivedState = CHashString( _T("CQHDerivedState") ).GetUniqueID();
	static const DWORD hashCQHDerivedEvent = CHashString( _T("CQHDerivedEvent") ).GetUniqueID();
	const DWORD hashType = type->GetUniqueID();
	if( hashCQHStateMachine == hashType )
	{
		LoadSaverBase::CreateObject( ar, parentName, type, objectName );

		m_Label2State.clear();
		m_hszStateMachineName = objectName;
		// fill label to state map for abstract states
		CStateVisitor stateVisitor( m_Label2State );
		VISITHIERARCHYDATA vhd;
		vhd.visitor = &stateVisitor;
		static DWORD msgHash_VisitHierarchy = CHashString(_T("VisitHierarchy")).GetUniqueID();
		m_ToolBox->SendMessage( msgHash_VisitHierarchy, sizeof(vhd), &vhd, objectName, type );
	}
	else if( hashCQHDerivedState == hashType )
	{
		float fVersion;
		ar->Read( fVersion, _T("version") );
		StdString parentState;
		ar->Read( parentState, _T("parentState") );
		if( 1.0 == fVersion)
		{
			int tmp;
			ar->Read( tmp, _T("x") );
			ar->Read( tmp, _T("y") );
			ar->Read( tmp, _T("width") );
			ar->Read( tmp, _T("height") );
			ar->Read( tmp, _T("numSubStates") );
			ar->Read( tmp, _T("numEvents") );
		}
		CHashString label(parentState.c_str());
		Hash2Hash::const_iterator it = m_Label2State.find( label.GetUniqueID() );
		if( it != m_Label2State.end() )
		{
			*objectName = m_ToolBox->GetHashString( it->second );
		}
		else
		{
			m_ToolBox->Log( LOGERROR, _T("Failed to find derived state parent (%s)"), parentState.c_str() );
			*objectName = m_hszStateMachineName.GetString();
		}
	}
	else if( hashCQHDerivedEvent == hashType )
	{
		float fVersion;
		ar->Read( fVersion, _T("version") );
		assert( fVersion == 1.0 );
		StdString parentEvent;
		ar->Read( parentEvent, _T("parentEvent") );
		CHashString hszName(parentEvent.c_str());
		static CHashString hszCQHState = _T("CQHState");
		static DWORD msgHash_GetEventNameFromLabel = CHashString(_T("GetEventNameFromLabel")).GetUniqueID();
		DWORD res = m_ToolBox->SendMessage( msgHash_GetEventNameFromLabel, 
											sizeof(IHashString), &hszName,
											parentName, &hszCQHState );
		if( MSG_HANDLED == res )
		{
			*objectName = hszName.GetString();
		}
		else
		{
			*objectName = _T("");
		}
	}
	else
	{
		LoadSaverBase::CreateObject( ar, parentName, type, objectName );
	}
}

//Check if the file is UNICODE
//for debugging purposes only.
#if 0
int IsUnicodeFile(char* szFileName)
{
    FILE *fpUnicode;
    char l_szCharBuffer[80];

    //Open the file
    if((fpUnicode= fopen(szFileName,"r")) == NULL)
    return 0; //Unable to open file

    if(!feof(fpUnicode))
    {
        fread(l_szCharBuffer,80,1,fpUnicode);
        fclose(fpUnicode);
        if(IsTextUnicode(l_szCharBuffer,80,NULL))
        {
            return 2; //Text is Unicode
        }
       else
       {
           return 1; //Text is ASCII
       }
    }
    return 0; // Some error happened
}
#endif