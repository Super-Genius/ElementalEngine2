///============================================================================
/// \file		BoundingBox.cpp
/// \brief		Implmentation of CObjectOrientedBoxRenderObject Object Class
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
#include "ObjectOrientedBox.h"
#include "ShapeManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT(CObjectOrientedBoxRenderObject);

CObjectOrientedBoxRenderObject::CObjectOrientedBoxRenderObject(IHashString *parentName, IHashString *name) :
	m_hszNameType( _T("CObjectOrientedBoxRenderObject") ),
	SCENEOBJECTTEMPLATE(CShapeManager , CObjectOrientedBoxRenderObject, IRenderObject, parentName, name)
{
	m_vPosition.Set( 0, 0, 0 );

	m_IRenderer = NULL;
}

CObjectOrientedBoxRenderObject::~CObjectOrientedBoxRenderObject()
{
	DeInit();
}

IComponent *CObjectOrientedBoxRenderObject::Create(int nArgs, va_list argptr)
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
		self = new CObjectOrientedBoxRenderObject(parentName, name);
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

void CObjectOrientedBoxRenderObject::Serialize( IArchive &ar )
{
	if(ar.IsReading())
	{
	}
	else
	{
	}
}

void CObjectOrientedBoxRenderObject::Init()
{
	GETRENDERERINTERFACEPARAMS renderInterfaceParam;
	static DWORD msgHash_GetRendererInterface = CHashString(_T("GetRendererInterface")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_GetRendererInterface, sizeof(GETRENDERERINTERFACEPARAMS), &renderInterfaceParam ) == MSG_HANDLED)
		m_IRenderer = renderInterfaceParam.m_RendererInterface;

	CShapeManager *pManager = SINGLETONINSTANCE(CShapeManager);
	pManager->AddShapeRenderObject( this );
}

bool CObjectOrientedBoxRenderObject::Update()
{
	return true;
}

void CObjectOrientedBoxRenderObject::DeInit()
{
}

IHashString* CObjectOrientedBoxRenderObject::GetComponentType()
{
	return &m_hszNameType;
}

bool CObjectOrientedBoxRenderObject::IsKindOf( IHashString* compType )
{
	return (compType->GetUniqueID() == m_hszNameType.GetUniqueID() );
}

bool CObjectOrientedBoxRenderObject::Render( UINT pass, IEffect * override )
{
	CShapeManager *pManager = SINGLETONINSTANCE(CShapeManager);

	if( (pass == 0) && (m_IRenderer) )
	{
		std::vector<CObjectOrientedBoundingBox>::iterator itrBBox = pManager->GetObjectOrientedBoundingBoxes().begin();
		while( itrBBox != pManager->GetObjectOrientedBoundingBoxes().end() )
		{
			CObjectOrientedBoundingBox *pBBox = &(*itrBBox);

  			Matrix4x4 parentTransform;
			static DWORD msgHash_GetGlobalTransform = CHashString(_T("GetGlobalTransform")).GetUniqueID();
			static CHashString hsEntityType(_T("CEntity"));
			if( m_ToolBox->SendMessage(msgHash_GetGlobalTransform, sizeof(Matrix4x4), &parentTransform, &pBBox->m_hszObjectName, &hsEntityType ) != MSG_HANDLED )
			{
				// quietly fail and leave parentTransform as identity matrix.
//				m_ToolBox->Log( LOGERROR, _T("Could not get global position to draw OOBB shape for %s!\n"), pBBox->m_hszObjectName.GetString() );
			}

			Matrix4x4 bboxTransform = parentTransform * pBBox->localTransform;

			Matrix3x3 bboxRot;
			bboxRot.SetFrom4x4( bboxTransform.m );
			Vec3 bboxPos = bboxTransform.GetTranslation();
			m_IRenderer->DrawOrientedBoundingBox( pBBox->min, pBBox->max, bboxRot, bboxPos,
				pBBox->red, pBBox->green, pBBox->blue );
			itrBBox++;
		}
	}
	return true;
}

bool CObjectOrientedBoxRenderObject::IsLessThan( const IRenderObject * obj, DWORD sortMethod, int curRenderPass )
{
	return true;
}

DWORD CObjectOrientedBoxRenderObject::GetRenderPriority()
{
    return 0;
}

UINT CObjectOrientedBoxRenderObject::GetNumPass()
{
	return 1;
}

IHashString * CObjectOrientedBoxRenderObject::GetBaseSortClass()
{
	return GetComponentType();
}

bool CObjectOrientedBoxRenderObject::IsAlphaObject()
{
	return false;
}

IMeshObject* CObjectOrientedBoxRenderObject::GetMeshObject()
{
	return NULL;
}
