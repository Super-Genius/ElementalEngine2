///============================================================================
/// \file		BoundingVolume.cpp
/// \brief		An object oriented bounding volume
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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT(CBoundingVolume);

CBoundingVolume::CBoundingVolume( IHashString *parentName, IHashString *name, bool bAddToHier ) :
	OBJECTTEMPLATE( CBoundingVolumeManager, CBoundingVolume, IObject, parentName, name )
{
	initBVOnce = false;
	m_PhysObject = NULL;
}

CBoundingVolume::~CBoundingVolume()
{
	DeInit();
}

IComponent *CBoundingVolume::Create( int nArgs, va_list argptr )
{
	IHashString *name;
	IHashString *parentName;
	IObject *self;

	// bAddToHier should probably always be false. We do NOT want this to get
	// serialized when loading/saving the world file.
	name = va_arg(argptr, IHashString *);
	parentName = va_arg(argptr, IHashString *);
	try
	{
		self = new CBoundingVolume(parentName, name, false);
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

void CBoundingVolume::Init()
{
	//CreatePickingBox();
}

void CBoundingVolume::Update(DWORD tickCount)
{
	if( initBVOnce == false )
	{
		initBVOnce = true;

		GETBOUNDINGBOXPARAMS bBoxParms;
		bBoxParms.position = &m_Position;
		bBoxParms.rotation = &m_Rotation;
		bBoxParms.extents = &m_Extents;
		static DWORD msgHash_GetBoundingBox = CHashString(_T("GetBoundingBox")).GetUniqueID();
		DWORD retVal = m_ToolBox->SendMessage(msgHash_GetBoundingBox, sizeof(bBoxParms), &bBoxParms, GetParentName() );

		// create the physX object for picking
		if( retVal == MSG_HANDLED )
		{
			CreatePickingBox();
		}
		else
		{
			DELETEOBJECTPARAMS dop;
			dop.name = GetName();
			static DWORD msgHash_DeleteObject = CHashString("DelayedDeleteObject").GetUniqueID();
			m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof(DELETEOBJECTPARAMS), &dop );
			return;
		}
	}

	// Set the dirty transform flag on the physics object
	//if( m_PhysObject )
	m_PhysObject->Update();
}

void CBoundingVolume::DeInit()
{
	DeletePickingBox();
}

void CBoundingVolume::Serialize(IArchive &ar)
{
	// nothing to serialize since this is never spawned in or loaded from a file.
	if (ar.IsReading())
	{
		//ar.Read();
		ar.Read( m_Position );
		ar.Read( m_Rotation );
		ar.Read( m_Extents );
	}
	else
	{
		ar.Write( m_Position, _T("position") );
		ar.Write( m_Rotation, _T("rotation") );
		ar.Write( m_Extents, _T("extents") );
	}
}

void CBoundingVolume::CreatePickingBox()
{
	CHashString typeName( _T("CPickingPhysicsObject") );

	// generate the picking object's name
	// get the parent's parent name which is a CEntity
	StdString poName = GetParentName()->GetString();
	poName += _T( "_PO" );
	CHashString hszPOName( poName );

	GENERATEUNIQUEOBJECTNAMEPARAMS guonp;
	guonp.name = &hszPOName;
	guonp.newname = &m_PhysicsObjName;
	static DWORD msgHash_GenerateUniqueObjectName = CHashString(_T("GenerateUniqueObjectName")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_GenerateUniqueObjectName, sizeof(guonp), &guonp ) != MSG_HANDLED)
		return;

	CHashString parentObjectName( GetParentName() );

	// create the object
	IComponent *physComp = m_ToolBox->CreateComponent( &typeName, 3, &m_PhysicsObjName, &parentObjectName, false );
	assert( physComp );

	OFACTORYADDPARAMS ofap;
	ofap.name = &m_PhysicsObjName;
	ofap.component = physComp;
	static DWORD msgHash_AddObjectToFactory = CHashString(_T("AddObjectToFactory")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_AddObjectToFactory, sizeof(OFACTORYADDPARAMS), &ofap );
	
	m_PhysObject = dynamic_cast<IObject*>( physComp );

	// Create Archive
	IArchive *MemArchive;	
	CHashString memType( _T("Memory") );
	CREATEARCHIVE ca;
	ca.mode = STREAM_MODE_WRITE | STREAM_MODE_READ;
	ca.streamData = NULL;
	ca.streamSize = 0;
	ca.streamType = &memType;
	static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca) != MSG_HANDLED )
	{
		return;
	}
	MemArchive = ca.archive;

	MemArchive->Write( m_Position, _T("Position") );
	MemArchive->Write( m_Extents, _T("Extents") );

	MemArchive->SetIsWriting( false );
	MemArchive->SeekTo( 0 );

	m_PhysObject->Serialize( *MemArchive );
	
	// Close the archive
	MemArchive->Close();

	m_PhysObject->Init();
}

void CBoundingVolume::DeletePickingBox()
{
	DELETEOBJECTPARAMS deleteBBox;
	deleteBBox.name = &m_PhysicsObjName;
	static DWORD msgHash_DeleteObject = CHashString("DeleteObject").GetUniqueID();
	m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof(DELETEOBJECTPARAMS), &deleteBBox );
}