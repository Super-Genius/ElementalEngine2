///============================================================================
/// \file		SplineLoader.cpp
/// \brief		Loader for spline data files
/// \date		11-14-2006
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

REGISTER_COMPONENT_SINGLETON( CSplineLoader );
REGISTER_MESSAGE_HANDLER( LoadFile, OnLoadFile, CSplineLoader );
REGISTER_MESSAGE_HANDLER( SaveFile, OnSaveFile, CSplineLoader );

CSplineLoader::CSplineLoader() :
	LoadSaverBase( "CSplineLoader", _T("CQHSplineManager"), _T("CSpline") )
{
	IHashString *componentHashString = GetComponentType();
	RegisterLoadSaveComponent();
}

/// standard static component create function
IComponent *CSplineLoader::Create( int nArgs, va_list argptr )
{
	return SINGLETONINSTANCE( CSplineLoader );
}

DWORD CSplineLoader::LoadFile( const TCHAR *fileName, IArchive *archive )
{
	IXMLArchive *xmlArchive = static_cast<IXMLArchive*>(archive);
	DWORD areturn = LoadXMLFile( fileName, xmlArchive );

	return areturn;
}

DWORD CSplineLoader::LoadXMLFile( const TCHAR *fileName, IXMLArchive *archive )
{
	DWORD areturn = MSG_HANDLED_STOP;
	StdString szNodeName;
	while( archive->GetNode(szNodeName) )
	{
		if( szNodeName != _T("Spline") )
		{
			m_ToolBox->SetErrorValue(WARN_INVALID_FILE);
			m_ToolBox->Log( LOGWARNING, _T("Invalid spline file %s\n"), fileName );
			areturn = MSG_ERROR;
		}
		else
		{
			areturn = ReadSplineNode( fileName, archive );
		}
	}
	return areturn;
}

DWORD CSplineLoader::ReadSplineNode( const TCHAR *fileName, IXMLArchive *archive )
{
	DWORD areturn = MSG_HANDLED_STOP;
	StdString szName;
	StdString szType;
	bool bLooping;
	StdString szLoopAnchor;
	int iNumPoints;

	archive->Read( szName, _T("Name") );
	archive->Read( szType, _T("Type") );
	archive->Read( bLooping, _T("Looping") );
	archive->Read( szLoopAnchor, _T("LoopAnchor") );
	archive->Read( iNumPoints, _T("NumPoints") );

	TCHAR builtName[256];
	BuildName( builtName, szName );

	// Need to create the spline before we can add points to it.
	bool splineCreated = CreateSpline( builtName );

	if( splineCreated )
	{
		// TODO <rnguyen>:	Eventually, we will want to gather these points from an external
		//					source such as the database for re-useability.
		StdString szNodeName;
		bool isReading = true;
		for( int i = 0; i < iNumPoints; i++ )
		{
			szNodeName = "";
			while( szNodeName != _T("SplinePoint") && isReading )
			{
				isReading = archive->GetNode(szNodeName);
			}
			if( isReading )
			{
				CHashString zName( builtName );
				ReadSplinePointNode( fileName, archive, dynamic_cast<IHashString*>( &zName ) );
			}
			else
			{
				m_ToolBox->SetErrorValue(WARN_INVALID_FILE);
				m_ToolBox->Log( LOGWARNING, _T("Invalid number of spline points specified in file %s\n"), fileName );
				areturn = MSG_ERROR;
			}
		}
	}
	else
	{
		areturn = MSG_ERROR;
	}

	return areturn;
}

bool CSplineLoader::CreateSpline( TCHAR *builtName )
{
	CHashString zParentName("World");
	CHashString zTypeName("CSplineObject");
	CHashString zName( builtName );

	CREATEOBJECTPARAMS createObjectParams;
	createObjectParams.parentName = &zParentName;
	createObjectParams.typeName = &zTypeName;
	createObjectParams.name = &zName;
	static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &createObjectParams);

	CHashString memType(_T("Memory"));
	CREATEARCHIVE createArchive;
	createArchive.mode = STREAM_MODE_WRITE;
	createArchive.streamData = NULL;
	createArchive.streamSize = 0;
	createArchive.streamType = &memType;
	static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &createArchive) != MSG_HANDLED)
	{
		return false;
	}

	IArchive *memArchive;
	memArchive = createArchive.archive;

	memArchive->Write( "Bezier", _T("Type") );
	memArchive->Write( true, _T("Loop") );
	memArchive->Write( 300, _T("Resolution") );
	memArchive->Write( "", _T("EventCollide") );
	memArchive->Write( "", _T("EventEnter") );
	memArchive->Write( "", _T("EventExit") );

	CREATESTREAM createStream;
	createStream.streamData = createArchive.streamData;
	createStream.streamSize = createArchive.streamSize;
	createStream.mode = STREAM_MODE_READ;
	static DWORD msgHash_CreateStream_Memory = CHashString(_T("CreateStream_Memory")).GetUniqueID();
	if ( m_ToolBox->SendMessage(msgHash_CreateStream_Memory, sizeof(CREATESTREAM), &createStream) != MSG_HANDLED )
	{
		return false;
	}

	memArchive->Init(createStream.openStream);

	SERIALIZEOBJECTPARAMS serializeObjectParams;
	serializeObjectParams.name = &zName;
	serializeObjectParams.archive = memArchive;
	static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &serializeObjectParams);

	memArchive->Close();

	INITOBJECTPARAMS initObjectParams;
	initObjectParams.name = &zName;
	static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &initObjectParams);

	// send out a message that an object needs to be added (hierarchy view)
	// may not want this happening for objects that are spawned in game...
	static DWORD msgHash_AddObject = CHashString(_T("AddObject")).GetUniqueID();
	//m_ToolBox->SendMessage(msgHash_AddObject, sizeof(CREATEOBJECTPARAMS), &cop);

	return true;
}

DWORD CSplineLoader::ReadSplinePointNode( const TCHAR *fileName, IXMLArchive *archive, IHashString *splineObjectName )
{
	DWORD areturn = MSG_HANDLED_STOP;

	StdString szName;
	float fPosition_x;
	float fPosition_y;
	float fPosition_z;

	archive->Read( szName, _T("Name") );
	archive->Read( fPosition_x, _T("Position_x") );
	archive->Read( fPosition_y, _T("Position_y") );
	archive->Read( fPosition_z, _T("Position_z") );

	Vec3 splinePoint( fPosition_x, fPosition_y, fPosition_z );

	static DWORD msgHash_AddPoint = CHashString(_T("AddPoint")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_AddPoint, sizeof(Vec3), &splinePoint, splineObjectName);

	return areturn;
}

DWORD CSplineLoader::SaveFile( const TCHAR *fileName, IArchive *archive )
{
	// TODO <rnguyen>: Save to file
	return MSG_HANDLED_STOP;
}

const TCHAR *CSplineLoader::GetLoadDescription()
{
	return _T( "Spline Files (*.xsp)\0" "*.xsp\0" );
}

const TCHAR *CSplineLoader::GetSaveDescription()
{
	return _T( "Spline Files (*.xsp)\0" "*.xsp\0" );
}

bool CSplineLoader::GetPrecachableBool( )
{
	return false;
}

CACHABLEQUERYCALLBACK  CSplineLoader::GetPrecachableFunc()
{
	return NULL;
}

void CSplineLoader::BuildName( TCHAR *buffer, StdString szName )
{
	int i = 0;
	DWORD found = MSG_HANDLED;
	FINDOBJECTPARAMS param;
	param.bFound = true;
	// find a unique name
	while( (found == MSG_HANDLED) && (param.bFound) )
	{
		// build unique name
		sprintf(buffer, _T("%s_%03d"), szName.c_str(), i);
		// convert to hash
		CHashString tempHash(buffer);
		// see if name exists
		static DWORD msgHash_FindObject = CHashString(_T("FindObject")).GetUniqueID();
		param.hszName = &tempHash;
		found = m_ToolBox->SendMessage(msgHash_FindObject, sizeof(FINDOBJECTPARAMS), &param);
		// increment i, in case it doesn't
		++i;
	}
}