///============================================================================
/// \file		Point.cpp
/// \brief		Implmentation of CPointRenderObject Object Class
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
#include "Point.h"
#include "ShapeManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT(CPointRenderObject);

CPointRenderObject::CPointRenderObject(IHashString *parentName, IHashString *name) :
	m_hszNameType( _T("CPointRenderObject") ),
	SCENEOBJECTTEMPLATE(CShapeManager , CPointRenderObject, IRenderObject, parentName, name)
{
	m_vPosition.Set( 0, 0, 0 );

	m_IRenderer = NULL;
}

CPointRenderObject::~CPointRenderObject()
{
	DeInit();
}

IComponent *CPointRenderObject::Create(int nArgs, va_list argptr)
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
		self = new CPointRenderObject(parentName, name);
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

void CPointRenderObject::Serialize( IArchive &ar )
{
	if(ar.IsReading())
	{
	}
	else
	{
	}
}

void CPointRenderObject::Init()
{
	GETRENDERERINTERFACEPARAMS renderInterfaceParam;
	static DWORD msgHash_GetRendererInterface = CHashString(_T("GetRendererInterface")).GetUniqueID();
	if( m_ToolBox->SendMessage(msgHash_GetRendererInterface, sizeof(GETRENDERERINTERFACEPARAMS), &renderInterfaceParam ) == MSG_HANDLED)
		m_IRenderer = renderInterfaceParam.m_RendererInterface;

	CShapeManager *pManager = SINGLETONINSTANCE(CShapeManager);
	pManager->AddShapeRenderObject( this );
}

bool CPointRenderObject::Update()
{
	return true;
}

void CPointRenderObject::DeInit()
{
}

IHashString* CPointRenderObject::GetComponentType()
{
	return &m_hszNameType;
}

bool CPointRenderObject::IsKindOf( IHashString* compType )
{
	return (compType->GetUniqueID() == m_hszNameType.GetUniqueID() );
}

bool CPointRenderObject::Render( UINT pass, IEffect * override )
{
	CShapeManager *pManager = SINGLETONINSTANCE(CShapeManager);

	if( (pass == 0) && (m_IRenderer) )
	{
		std::vector<CPoint>::iterator itrPoint = pManager->GetPoints().begin();
		while (itrPoint != pManager->GetPoints().end())
		{
			CPoint *pPoint = &(*itrPoint);

			Vec3 objPos;
			static DWORD msgHash_GetGlobalPosition = CHashString(_T("GetGlobalPosition")).GetUniqueID();
			static CHashString hsEntityType(_T("CEntity"));
			if (m_ToolBox->SendMessage(msgHash_GetGlobalPosition, sizeof(Vec3), &objPos, &pPoint->m_hszObjectName, &hsEntityType ) != MSG_HANDLED)
			{
				m_ToolBox->Log( LOGERROR, _T("Could not get global position to draw point shape!\n" ) );
			}

			m_IRenderer->DrawPoint( objPos.x, objPos.y, objPos.z, 
				pPoint->red, pPoint->green, pPoint->blue, 
				pPoint->size );
			itrPoint++;
		}
	}
	return true;
}

bool CPointRenderObject::IsLessThan( const IRenderObject * obj, DWORD sortMethod, int curRenderPass )
{
	return true;
}

DWORD CPointRenderObject::GetRenderPriority()
{
    return 0;
}

UINT CPointRenderObject::GetNumPass()
{
	return 1;
}

IHashString * CPointRenderObject::GetBaseSortClass()
{
	return GetComponentType();
}

bool CPointRenderObject::IsAlphaObject()
{
	return false;
}

IMeshObject* CPointRenderObject::GetMeshObject()
{
	return NULL;
}
