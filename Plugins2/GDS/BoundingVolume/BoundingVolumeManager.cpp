///============================================================================
/// \file		BoundingVolumeManager.h
/// \brief		Manager for bounding volume objects.
/// \date		07/27/2007
/// \author		Kyle Swaim
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

#define BOUNDINGVOLUMEPRIORITY 0x04A1

REGISTER_COMPONENT_SINGLETON(CBoundingVolumeManager);

REGISTER_MESSAGE_HANDLER_PRIORITY(CreateObject, OnCreateBVolumeObject, CBoundingVolumeManager, 0x90000000 );
REGISTER_MESSAGE_HANDLER_PRIORITY(DeleteObject, OnDeleteBVolumeObject, CBoundingVolumeManager, 0x70000000 );

CBoundingVolumeManager::CBoundingVolumeManager() : CManager( _T("CBoundingVolumeManager"), BOUNDINGVOLUMEPRIORITY )
{	
	m_BVMap.clear();
}

CBoundingVolumeManager::~CBoundingVolumeManager()
{
	m_BVMap.clear();
}

IComponent* CBoundingVolumeManager::Create( int nArgs, va_list argptr )
{
  	return SINGLETONINSTANCE( CBoundingVolumeManager );
}

void CBoundingVolumeManager::Update( DWORD tickCount )
{
	// Get the map of managed physics objects
	static CHashString bvObjType( _T("CBoundingVolume") );

	IDTOOBJECTMAP::iterator iter;
	IDTOOBJECTMAP* bvObjMap = GetObjectMap( &bvObjType );

	// iterate through all of the objects that we manage and update them
	if( bvObjMap != NULL )
	{
		CBoundingVolume* currentBvObj;
		for( iter = bvObjMap->begin(); iter != bvObjMap->end(); iter++ )
		{
			currentBvObj = dynamic_cast<CBoundingVolume*>( iter->second );
			currentBvObj->Update( tickCount );
		}
	}
}

DWORD CBoundingVolumeManager::OnCreateBVolumeObject(DWORD size, void *params)
{
	// This message is triggered when the create message is called.
	// The message priority has been lowered so that it will always
	// be called after the main create message. Otherwise it might
	// get called before the parent object has been created which
	// would cause the bounding volume generation to fail.

	VERIFY_MESSAGE_SIZE( sizeof(CREATEOBJECTPARAMS), size );
	CREATEOBJECTPARAMS* cop = (CREATEOBJECTPARAMS*)params;

	// check if the object needs a bv
	Vec3 pos;
	EulerAngle rot;
	Vec3 ext;

	GETBOUNDINGBOXPARAMS bBoxParms;
	bBoxParms.position = &pos;
	bBoxParms.rotation = &rot;
	bBoxParms.extents = &ext;
	static DWORD msgHash_GetBoundingBox = CHashString(_T("GetBoundingBox")).GetUniqueID();
	DWORD retVal = m_ToolBox->SendMessage(msgHash_GetBoundingBox, sizeof(bBoxParms), &bBoxParms, cop->name );

	if( retVal == MSG_HANDLED )
	{
		// if it does then create a new bv
		CHashString hszType( _T("CBoundingVolume") );
		StdString wszName = cop->parentName->GetString();
		wszName += _T("_BVol");
		CHashString* hszName = new CHashString( wszName );

		CREATEOBJECTPARAMS bvCop;

		bvCop.name = hszName;
		bvCop.parentName = cop->parentName;
		bvCop.typeName = &hszType;
		static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
		m_ToolBox->SendMessage( msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &bvCop );

		// Insert the name of the new object into the map
		m_BVMap[cop->parentName->GetUniqueID()] = hszName;

		// Serialize object data
		CREATEARCHIVE ca;
		CHashString memType(_T("Memory"));

		ca.mode = STREAM_MODE_WRITE | STREAM_MODE_READ;
		ca.streamData = NULL;
		ca.streamSize = 0;
		ca.streamType = &memType;
		static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
		m_ToolBox->SendMessage( msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca );

		ca.archive->SetIsWriting( true );
		ca.archive->Write( pos, _T("position") );
		ca.archive->Write( rot, _T("rotation") );
		ca.archive->Write( ext, _T("extents") );
		ca.archive->Write( cop->parentName->GetString(), _T("parentName") );

		ca.archive->SeekTo( 0 );
		ca.archive->SetIsWriting( false );

		SERIALIZEOBJECTPARAMS bvSop;
		bvSop.name = hszName;
		bvSop.archive = ca.archive;
		static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
		m_ToolBox->SendMessage( msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &bvSop );

		ca.archive->Close();

		// init the bv object
		INITOBJECTPARAMS bvIop;
		bvIop.name = hszName;
		static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
		m_ToolBox->SendMessage( msgHash_InitObject, sizeof(INITOBJECTPARAMS), &bvIop );
	}

    return MSG_HANDLED_PROCEED;
}

DWORD CBoundingVolumeManager::OnDeleteBVolumeObject(DWORD size, void *params)
{
	// This message has its priority raised so that it gets called before the
	// parent entity's delete object. Otherwise it tries to delete itself after
	// the parent is gone which fails because the parent entity name has been
	// deleted.
	
	VERIFY_MESSAGE_SIZE( sizeof(DELETEOBJECTPARAMS), size );
	DELETEOBJECTPARAMS* dop = (DELETEOBJECTPARAMS*)params;

	// Check if the name matches anything in the map
	BVNAMETOOBJECTMAP::iterator iter;
	DWORD nameDword = dop->name->GetUniqueID();
	iter = m_BVMap.find( nameDword );
	if ( iter != m_BVMap.end() )
	{
		// delete the corrosponding bv
		DELETEOBJECTPARAMS bvDop;
		bvDop.name = iter->second;
		static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
		m_ToolBox->SendMessage( msgHash_DeleteObject, sizeof(bvDop), &bvDop );

		// delete the name and remove the entry from the map
		//delete iter->first;
		delete iter->second;
		m_BVMap.erase( iter );
	}

	return MSG_HANDLED_PROCEED;
}