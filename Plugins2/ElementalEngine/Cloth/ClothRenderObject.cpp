///============================================================================
/// \file	ClothRenderObject.cpp
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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


REGISTER_COMPONENT(CClothRenderObject);

REGISTER_MESSAGE_HANDLER( GetGeometryAllocations, OnGetGeometryAllocations, CClothRenderObject );

IComponent *CClothRenderObject::Create(int nArgs, va_list argptr)
{
	IObject *self;

	IHashString *name = va_arg(argptr, IHashString *);
	IHashString *parentName = va_arg(argptr, IHashString *);
	bool bAddToHier;
	// create the object however you must create it
	try
	{
		// try and create an instance
		if (nArgs == 2)
		{
			self = new CClothRenderObject(parentName, name, true);
		}
		else if (nArgs == 3)
		{
			bAddToHier = va_arg(argptr, bool);
			self = new CClothRenderObject(parentName, name, bAddToHier);
		}
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

CClothRenderObject::CClothRenderObject(IHashString *parentName, IHashString *name, bool bAddToHier) : 
	CRenderObject<>( _T("CClothManager"), _T("CClothRenderObject"), parentName, name ) 
{
	m_IRenderer = NULL;
	m_bAddToHierarchy = bAddToHier;
	if (bAddToHier)
	{
		AddToHierarchy();
	}
	m_bTransformIsDirty = true;

	m_VertexBufferInterface = NULL;
	m_IndexBufferInterface = NULL;
}

CClothRenderObject::~CClothRenderObject()
{
	DeInit();
	if (m_bAddToHierarchy)
	{
		RemoveFromHierarchy();
	}
}

void CClothRenderObject::Init()
{
	GETRENDERERINTERFACEPARAMS renderInterfaceParam;
	static DWORD msgHash_GetRendererInterface = CHashString(_T("GetRendererInterface")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_GetRendererInterface, sizeof(GETRENDERERINTERFACEPARAMS), &renderInterfaceParam ) == MSG_HANDLED)
		m_IRenderer = renderInterfaceParam.m_RendererInterface;

	m_bTransformIsDirty = true;

	//Create Geometry
	/*
	static DWORD msgHash_CreateGeometry = CHashString("CreateGeometry").GetUniqueID();
	m_ToolBox->SendMessage(msgHash_CreateGeometry, sizeof(m_pGeometry), &m_pGeometry );
	*/

	//Create Temp Render Object
	/*
	static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
	static CHashString hszTmpRenderObjectName = CHashString(_T("TempClothObject"));
	static CHashString hszTmpRenderObjectType = CHashString(_T("Cal3DRenderObject"));
	CREATEOBJECTPARAMS cop;
	cop.name = &hszTmpRenderObjectName;
	cop.parentName = NULL;
	cop.typeName = &hszTmpRenderObjectType;
	m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop );

	//Serialize Temp Render Object
	CREATEARCHIVE ca;
	CHashString memType(_T("Memory"));

	ca.mode = STREAM_MODE_WRITE | STREAM_MODE_READ;
	ca.streamData = NULL;
	ca.streamSize = 0;
	ca.streamType = &memType;
	static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca);
    
	Vec3 vOrigin(0.0, 0.0, 0.0);
	Vec3 vIdentity(1.0, 1.0, 1.0);
	StdString szEffect(_T("Shaders/Cal3DEffect_noskin.efx"));
	ca.archive->SetIsWriting( true );
	ca.archive->Write( m_szFilename );
	ca.archive->Write( vOrigin );
	ca.archive->Write( vOrigin );
	ca.archive->Write( vIdentity );
	ca.archive->Write( szEffect );
	ca.archive->SetIsWriting( false );

	SERIALIZEOBJECTPARAMS sop;
	sop.name = &hszTmpRenderObjectName;
	sop.archive = ca.archive;
	static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop, NULL, NULL);

	//Init Temp Render Object
	INITOBJECTPARAMS iop;
	iop.name = &hszTmpRenderObjectName;
	static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop );
	*/

	//Get the IRender Interface From CFG Model from m_szFilename
	static DWORD msgHash_GetRenderObjectInterface = CHashString(_T("GetRenderObjectInterface")).GetUniqueID();
	IRenderObject* pTmpRenderObject = NULL;
	CHashString hszParentName( m_szParentRenderObjectName );
	m_ToolBox->SendMessage(msgHash_GetRenderObjectInterface, sizeof(pTmpRenderObject), &pTmpRenderObject, &hszParentName);
	
	//Get the IGeometry Interface
	if( pTmpRenderObject == NULL )
		return;
	IGeometry* pTmpGeometry = pTmpRenderObject->GetGeometry( 0 );
	
	//Copy Geometry Info from Model into m_pGeometry
	/*
	m_pGeometry->SetMaterial( pTmpGeometry->GetMaterial() );
	m_pGeometry->SetSceneSettings( pTmpGeometry->GetSceneSettings() );
	m_pGeometry->SetController( pTmpGeometry->GetController() );
	*/
    
	BUFFERALLOCATIONSTRUCT tmpAllocation;
	/*
	pTmpGeometry->GetIndexBufferAllocation( tmpAllocation );
	m_pGeometry->SetIndexBufferAllocation( tmpAllocation );
	*/
	
	pTmpGeometry->GetVertexBufferAllocation( tmpAllocation );
	void* pSrcData;
	void* pDestData;
	pTmpGeometry->LockVertices( 0, tmpAllocation.m_Size, &pSrcData );
	//Create Dynamic Vertex Buffer
	AllocateVertices( tmpAllocation.m_Size, tmpAllocation.VertexFormat, tmpAllocation.m_Stride );
	//m_pGeometry->SetVertexBufferAllocation( m_VertexBufferAllocation );
	//m_pGeometry->LockVertices( m_VertexBufferAllocation.m_Offset, m_VertexBufferAllocation.m_Size, &pDestData );
	m_VertexBufferInterface->Lock( m_VertexBufferAllocation.m_Offset, m_VertexBufferAllocation.m_Size, &pDestData );
	//Copy Vertex Data from TmpGeometry into Dynamic Vertex Buffer
	memcpy( pDestData, pSrcData, m_VertexBufferAllocation.m_Size * m_VertexBufferAllocation.m_Stride );
	pTmpGeometry->UnlockVertices();
	//m_pGeometry->UnlockVertices();
	m_VertexBufferInterface->Unlock();
	pTmpGeometry->SetVertexBufferAllocation( m_VertexBufferAllocation );

	//Delete The Temp Render Object
	/*
	DELETEOBJECTPARAMS dop;
	dop.name = &hszTmpRenderObjectName;
	static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof(DELETEOBJECTPARAMS), &dop );
	*/
}

bool CClothRenderObject::Update()
{
	if( m_bTransformIsDirty == true )
	{
		static DWORD msgHash_GetGlobalTransform = CHashString(_T("GetGlobalTransform")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GetGlobalTransform, sizeof(Matrix4x4), &m_bParentTransform, GetParentName() );
	}

	// TODO: only do this if visible in scene
	static DWORD msgHash_GetRenderObjectInterface = CHashString(_T("GetRenderObjectInterface")).GetUniqueID();
	IRenderObject* pTmpRenderObject = NULL;
	CHashString hszParentName( m_szParentRenderObjectName );
	m_ToolBox->SendMessage(msgHash_GetRenderObjectInterface, sizeof(pTmpRenderObject), &pTmpRenderObject, &hszParentName);
	
	//Get the IGeometry Interface
	if( pTmpRenderObject == NULL )
		return false;
	IGeometry* pTmpGeometry = pTmpRenderObject->GetGeometry( 0 );

	// get grandparent entity name
	IHashString *pGrandparentName;
	static DWORD msgHash_GetParentName = CHashString(_T("GetParentName")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_GetParentName, sizeof(IHashString*), &pGrandparentName, GetParentName() ) != MSG_HANDLED)
		return false;

	CHashString hszNodeName(m_szNodeName);
	GETCLOTHPHYSICSGEOMETRYPARAMS gcpgp;
	gcpgp.pGeometry = pTmpGeometry;
	gcpgp.pNodeName = &hszNodeName;
	static DWORD msgHash_GetClothPhysicsGeometry = CHashString(_T("GetClothPhysicsGeometry")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_GetClothPhysicsGeometry, sizeof(gcpgp), &gcpgp, pGrandparentName ) != MSG_HANDLED)
		return false;

	return true;
}

void CClothRenderObject::DeInit()
{
	RemoveBoundingObject();

	if(m_VertexBufferInterface)
	{
		static DWORD msgHash_RemoveVertexAllocation = CHashString(_T("RemoveVertexAllocation")).GetUniqueID();
		m_ToolBox->SendMessage( msgHash_RemoveVertexAllocation, sizeof(m_VertexBufferAllocation), &m_VertexBufferAllocation );
		m_VertexBufferAllocation.m_Size = 0;
		m_VertexBufferInterface = NULL;
	}

	if(m_IndexBufferInterface)
	{
        static DWORD msgHash_RemoveIndexAllocation = CHashString(_T("RemoveIndexAllocation")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_RemoveIndexAllocation, sizeof(m_IndexBufferAllocation), &m_IndexBufferAllocation );
		m_IndexBufferAllocation.m_Size = 0;
		m_IndexBufferInterface = NULL;
	}
}

void CClothRenderObject::Serialize(IArchive &ar)
{
	if(ar.IsReading())
	{
		ar.Read( m_szFilename );
		ar.Read( m_szParentRenderObjectName );
		ar.Read( m_szNodeName );
	}
	else
	{
		ar.Write( m_szFilename );
		ar.Write( m_szParentRenderObjectName );
		ar.Write( m_szNodeName );
	}
}

bool CClothRenderObject::Render( UINT pass, IEffect * override )
{
	if (!m_IRenderer || pass != 0)
		return false;

	//Send Message:"GetClothPhysicsGeometry" To Parent with data m_pGeometry
	//static DWORD msgHash_GetClothPhysicsGeometry = CHashString(_T("GetClothPhysicsGeometry")).GetUniqueID();
	//m_ToolBox->SendMessage(msgHash_GetClothPhysicsGeometry, sizeof(m_pGeometry), &m_pGeometry );

	//Render It HOW?!?!?!?
	
	return true;
}

bool CClothRenderObject::IsLessThan( const IRenderObject * obj, DWORD sortMethod, int curRenderPass )
{
	return true;
}

DWORD CClothRenderObject::GetRenderPriority()
{
    return 0;
}

UINT CClothRenderObject::GetNumPass()
{
	return 1;
}

IHashString * CClothRenderObject::GetBaseSortClass()
{
	return GetComponentType();
}

bool CClothRenderObject::IsAlphaObject()
{
	return false;
}

IMeshObject* CClothRenderObject::GetMeshObject()
{
	return NULL;
}

bool CClothRenderObject::AllocateVertices( int iVertexCount, CHANNELDESCRIPTORLIST VertexFormat, int stride )
{
	if (m_VertexBufferAllocation.m_Size >= iVertexCount)
		return true;

	m_VertexBufferAllocation.VertexFormat = VertexFormat;
	// remove previous allocation
	if (m_VertexBufferAllocation.m_Size != 0)
	{
		static DWORD msgHash_RemoveVertexAllocation = CHashString(_T("RemoveVertexAllocation")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_RemoveVertexAllocation, sizeof(m_VertexBufferAllocation), &m_VertexBufferAllocation );
		m_VertexBufferAllocation.m_Size = 0;
	}

	//create new allocation
	ALLOCATERENDERBUFFERMSG msg;
	msg.ElementsToAllocate = iVertexCount;
	msg.SizeVertexFormat = stride;
	msg.VertexFormat = VertexFormat;
	msg.m_AllowSharedBuffer = false;
	msg.Dynamic = true;
	static DWORD msgHash_AllocateVertices = CHashString(_T("AllocateVertices")).GetUniqueID();
	DWORD res = m_ToolBox->SendMessage(msgHash_AllocateVertices, sizeof(msg), &msg );

	if (res == MSG_HANDLED)
	{
		m_VertexBufferAllocation.m_Offset = msg.m_ReturnOffset;
		m_VertexBufferAllocation.m_AllocationHandle = msg.m_ReturnHandle;
		m_VertexBufferAllocation.m_InterfaceHandle = msg.m_ReturnInterfaceHandle;
		m_VertexBufferAllocation.m_Size = iVertexCount;
		m_VertexBufferAllocation.m_Stride = msg.SizeVertexFormat;
		m_VertexBufferInterface = (IVertexBufferObject*)msg.m_InterfacePointer;
		return true;
	}
	else
	{	
		m_VertexBufferInterface = NULL;
		return false;
	}
}

bool CClothRenderObject::AllocateIndices( int iIndexCount )
{
	if (m_IndexBufferAllocation.m_Size >= iIndexCount)
		return true;

	// remove previous allocation
	if (m_IndexBufferAllocation.m_Size != 0)
	{
		static DWORD msgHash_RemoveIndexAllocation = CHashString(_T("RemoveIndexAllocation")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_RemoveIndexAllocation, sizeof(m_IndexBufferAllocation), &m_IndexBufferAllocation );
		m_IndexBufferAllocation.m_Size = 0;
	}

	//create new allocation
	ALLOCATERENDERBUFFERMSG msg;
	msg.m_AllowSharedBuffer = false;
	msg.ElementsToAllocate = iIndexCount;
	msg.SizeVertexFormat = sizeof(unsigned short);

	static DWORD msgHash_AllocateIndices = CHashString(_T("AllocateIndices")).GetUniqueID();
	DWORD res = m_ToolBox->SendMessage(msgHash_AllocateIndices, sizeof( msg ), &msg, NULL, NULL );
	if( res == MSG_HANDLED )
	{
		m_IndexBufferAllocation.m_Offset = msg.m_ReturnOffset;
		m_IndexBufferAllocation.m_AllocationHandle = msg.m_ReturnHandle;
		m_IndexBufferAllocation.m_InterfaceHandle = msg.m_ReturnInterfaceHandle;
		m_IndexBufferAllocation.m_Size = iIndexCount;
		m_IndexBufferAllocation.m_Stride = sizeof(unsigned short);
		m_IndexBufferInterface = (IIndexBuffer*)msg.m_InterfacePointer;
		return true;
	}
	else
	{
		m_IndexBufferInterface = NULL;
		return false;
	}
}

DWORD CClothRenderObject::OnGetGeometryAllocations( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof( GETRENDERBUFFERALLOCATIONMSG ) );
	//GETRENDERBUFFERALLOCATIONMSG * parms = (GETRENDERBUFFERALLOCATIONMSG *)params;
	CHashString tmpName( m_szParentRenderObjectName );
	static DWORD msgHash_GetGeometryAllocations = CHashString(_T("GetGeometryAllocations")).GetUniqueID();
	DWORD result = m_ToolBox->SendMessage(msgHash_GetGeometryAllocations, sizeof(GETRENDERBUFFERALLOCATIONMSG), params, &tmpName );

	return result;
}

