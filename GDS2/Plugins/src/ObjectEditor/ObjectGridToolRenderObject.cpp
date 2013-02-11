///============================================================================
/// \file		ObjectGridToolRenderObject.cpp
/// \brief		Implmentation of ObjectGridToolRenderObject Object Class
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
#include "ObjectGridToolRenderObject.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT(CObjectGridToolRenderObject);

CObjectGridToolRenderObject::CObjectGridToolRenderObject(IHashString *parentName, IHashString *name) :
	m_hszNameType( _T("CObjectGridToolRenderObject") ),
	SCENEOBJECTTEMPLATE(CRenderManager, CObjectGridToolRenderObject, IRenderObject, parentName, name)
{
	m_vPosition.Set( 0, 0, 0 );

	m_IRenderer = NULL;
}

CObjectGridToolRenderObject::~CObjectGridToolRenderObject()
{
	DeInit();
}

IComponent *CObjectGridToolRenderObject::Create(int nArgs, va_list argptr)
{
	IHashString *name;
	IHashString *parentName;
	IObject *self;

	name = va_arg(argptr, IHashString *);
	parentName = va_arg(argptr, IHashString *);
	// create the object however you must create it
	try
	{
		// try and create an instance
		self = new CObjectGridToolRenderObject(parentName, name);
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

void CObjectGridToolRenderObject::Serialize( IArchive &ar )
{
	if(ar.IsReading())
	{
	}
	else
	{
	}
}

void CObjectGridToolRenderObject::Init()
{
	GETRENDERERINTERFACEPARAMS renderInterfaceParam;
	static DWORD msgHash_GetRendererInterface = CHashString(_T("GetRendererInterface")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_GetRendererInterface, sizeof(GETRENDERERINTERFACEPARAMS), &renderInterfaceParam ) == MSG_HANDLED)
		m_IRenderer = renderInterfaceParam.m_RendererInterface;

	SCENEOBJECTPARAMS params;
	params.pObject = this;
	params.sortID = RENDEROBJECT3D;
	static DWORD msgHash_UpdateBoundingBox = CHashString("UpdateBoundingBox").GetUniqueID();
	m_ToolBox->SendMessage(msgHash_UpdateBoundingBox, sizeof(SCENEOBJECTPARAMS), &params );
}

bool CObjectGridToolRenderObject::Update()
{
	return true;
}

void CObjectGridToolRenderObject::DeInit()
{
	// remove bounding boxed object
	SCENEOBJECTPARAMS params;
	params.pObject = this;	
	params.sortID = RENDEROBJECT3D;
	static DWORD msgHash_RemoveBoundingBoxedObject = CHashString("RemoveBoundingBoxedObject").GetUniqueID();
	m_ToolBox->SendMessage(msgHash_RemoveBoundingBoxedObject, sizeof(SCENEOBJECTPARAMS), &params );
}

IHashString* CObjectGridToolRenderObject::GetComponentType()
{
	return &m_hszNameType;
}

bool CObjectGridToolRenderObject::IsKindOf( IHashString* compType )
{
	return (compType->GetUniqueID() == m_hszNameType.GetUniqueID() );
}

bool CObjectGridToolRenderObject::Render( UINT pass, IEffect * override )
{
	if (pass != 0)
		return false;
	if (m_IRenderer == NULL)
		return false;

	OBJECTGRIDTOOLDATA msgGridToolData;
	static DWORD msgHash_GetObjectGridToolData = CHashString("GetObjectGridToolData").GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_GetObjectGridToolData, sizeof(OBJECTGRIDTOOLDATA), &msgGridToolData ) != MSG_HANDLED)
		return false;

	if (!msgGridToolData.bGridActive)
		return false;

	const int VISIBLE_GRID_HALF_SIZE = 10;
	const int VISIBLE_GRID_SIZE = VISIBLE_GRID_HALF_SIZE * 2;

	float fStartX = -(float)VISIBLE_GRID_HALF_SIZE * msgGridToolData.fGridSize;
	float fStartY = -(float)VISIBLE_GRID_HALF_SIZE * msgGridToolData.fGridSize;
	float fEndX = fStartX + msgGridToolData.fGridSize * (float)VISIBLE_GRID_SIZE;
	float fEndY = fStartY + msgGridToolData.fGridSize * (float)VISIBLE_GRID_SIZE;

	float fX = fStartX, fY = fStartY;
	for (int i=0; i < VISIBLE_GRID_SIZE+1; i++)
	{
		m_IRenderer->Draw3DLine( fX, fStartY, 0, fX, fEndY, 0, 255, 255, 255, 1 );
		m_IRenderer->Draw3DLine( fStartX, fY, 0, fEndX, fY, 0, 255, 255, 255, 1 );
		fX += msgGridToolData.fGridSize;
		fY += msgGridToolData.fGridSize;
	}

	return true;
}

bool CObjectGridToolRenderObject::IsLessThan( const IRenderObject * obj, DWORD sortMethod, int curRenderPass )
{
	return true;
}

DWORD CObjectGridToolRenderObject::GetRenderPriority()
{
    return 0;
}

UINT CObjectGridToolRenderObject::GetNumPass()
{
	return 1;
}

void CObjectGridToolRenderObject::GetBoundingSphere( Vec3 &position, float &radius )
{
	position = m_vPosition;
	radius = 999999.0f;
}
