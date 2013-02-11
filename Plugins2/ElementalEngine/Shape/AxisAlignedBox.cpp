///============================================================================
/// \file		AxisAlignedBox.cpp
/// \brief		Implmentation of CAxisAlignedBoxRenderObject Object Class
/// \date		07-25-2005
/// \author		D. Patrick Ghiocel
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
#include "AxisAlignedBox.h"
#include "ShapeManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT( CAxisAlignedBoxRenderObject );

CAxisAlignedBoxRenderObject::CAxisAlignedBoxRenderObject(IHashString *parentName, IHashString *name) :
	m_hszNameType( _T("CAxisAlignedBoxRenderObject") ),
	SCENEOBJECTTEMPLATE(CShapeManager , CAxisAlignedBoxRenderObject, IRenderObject, parentName, name)
{
	m_vPosition.Set( 0, 0, 0 );

	m_IRenderer = NULL;
}

CAxisAlignedBoxRenderObject::~CAxisAlignedBoxRenderObject()
{
	DeInit();
}

IComponent *CAxisAlignedBoxRenderObject::Create( int nArgs, va_list argptr )
{
	IHashString *name;
	IHashString *parentName;
	IObject *self;

	name = va_arg( argptr, IHashString* );
	parentName = va_arg( argptr, IHashString* );
	// create the object however you must create it
	try
	{
		// try and create an instance
		self = new CAxisAlignedBoxRenderObject( parentName, name );
	}
	catch( ... )
	{
		return NULL;
	}

	return self;
}

void CAxisAlignedBoxRenderObject::Serialize( IArchive &ar )
{
	if( ar.IsReading() )
	{
	}
	else
	{
	}
}

void CAxisAlignedBoxRenderObject::Init()
{
	GETRENDERERINTERFACEPARAMS renderInterfaceParam;
	static DWORD msgHash_GetRendererInterface = CHashString(_T("GetRendererInterface")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_GetRendererInterface, sizeof(GETRENDERERINTERFACEPARAMS), &renderInterfaceParam ) == MSG_HANDLED )
		m_IRenderer = renderInterfaceParam.m_RendererInterface;
	
	CShapeManager *pManager = SINGLETONINSTANCE(CShapeManager);
	pManager->AddShapeRenderObject( this );
}

bool CAxisAlignedBoxRenderObject::Update()
{
	return true;
}

void CAxisAlignedBoxRenderObject::DeInit()
{
}

IHashString* CAxisAlignedBoxRenderObject::GetComponentType()
{
	return &m_hszNameType;
}

bool CAxisAlignedBoxRenderObject::IsKindOf( IHashString* compType )
{
	return (compType->GetUniqueID() == m_hszNameType.GetUniqueID() );
}

bool CAxisAlignedBoxRenderObject::Render( UINT pass, IEffect * override )
{
	CShapeManager *pManager = SINGLETONINSTANCE(CShapeManager);

	if( (pass == 0) && (m_IRenderer) )
	{
		std::vector<CAxisAlignedBoundingBox>::iterator itrBBox = pManager->GetAxisAlignedBoundingBoxes().begin();
		while( itrBBox != pManager->GetAxisAlignedBoundingBoxes().end() )
		{
			CAxisAlignedBoundingBox *pBBox = &(*itrBBox);

			Vec3 objPos;
			static DWORD msgHash_GetGlobalPosition = CHashString(_T("GetGlobalPosition")).GetUniqueID();
			static CHashString hsEntityType(_T("CEntity"));
			if( m_ToolBox->SendMessage(msgHash_GetGlobalPosition, sizeof(Vec3), &objPos, &pBBox->m_hszObjectName, &hsEntityType ) == MSG_HANDLED )
			{
				Vec3 min = ( pBBox->min + objPos );
				Vec3 max = ( pBBox->max + objPos );
				m_IRenderer->DrawBoundingBox( min, max, pBBox->red, pBBox->green, pBBox->blue, pBBox->alpha );
			}
			else
			{
				m_ToolBox->Log( LOGERROR, _T("Could not get global position to draw AABB shape!\n" ) );
			}

			itrBBox++;
		}
	}
	return true;
}

bool CAxisAlignedBoxRenderObject::IsLessThan( const IRenderObject * obj, DWORD sortMethod, int curRenderPass )
{
	return true;
}

DWORD CAxisAlignedBoxRenderObject::GetRenderPriority()
{
    return 0;
}

UINT CAxisAlignedBoxRenderObject::GetNumPass()
{
	return 1;
}

IHashString * CAxisAlignedBoxRenderObject::GetBaseSortClass()
{
	return GetComponentType();
}

bool CAxisAlignedBoxRenderObject::IsAlphaObject()
{
	return false;
}

IMeshObject* CAxisAlignedBoxRenderObject::GetMeshObject()
{
	return NULL;
}
