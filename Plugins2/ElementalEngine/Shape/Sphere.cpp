///============================================================================
/// \file		Sphere.cpp
/// \brief		Implmentation of CSphereRenderObject Object Class
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
#include "Sphere.h"
#include "ShapeManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT(CSphereRenderObject);

CSphereRenderObject::CSphereRenderObject(IHashString *parentName, IHashString *name) :
	m_hszNameType( _T("CSphereRenderObject") ),
	SCENEOBJECTTEMPLATE(CShapeManager , CSphereRenderObject, IRenderObject, parentName, name)
{
	m_vPosition.Set( 0, 0, 0 );

	m_IRenderer = NULL;
}

CSphereRenderObject::~CSphereRenderObject()
{
	DeInit();
}

IComponent *CSphereRenderObject::Create(int nArgs, va_list argptr)
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
		self = new CSphereRenderObject(parentName, name);
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

void CSphereRenderObject::Serialize( IArchive &ar )
{
	if(ar.IsReading())
	{
	}
	else
	{
	}
}

void CSphereRenderObject::Init()
{
	GETRENDERERINTERFACEPARAMS renderInterfaceParam;
	static DWORD msgHash_GetRendererInterface = CHashString(_T("GetRendererInterface")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_GetRendererInterface, sizeof(GETRENDERERINTERFACEPARAMS), &renderInterfaceParam ) == MSG_HANDLED)
		m_IRenderer = renderInterfaceParam.m_RendererInterface;

	CShapeManager *pManager = SINGLETONINSTANCE(CShapeManager);
	pManager->AddShapeRenderObject( this );
}

bool CSphereRenderObject::Update()
{
	return true;
}

void CSphereRenderObject::DeInit()
{
}

IHashString* CSphereRenderObject::GetComponentType()
{
	return &m_hszNameType;
}

bool CSphereRenderObject::IsKindOf( IHashString* compType )
{
	return (compType->GetUniqueID() == m_hszNameType.GetUniqueID() );
}

bool CSphereRenderObject::Render( UINT pass, IEffect * override )
{
	CShapeManager *pManager = SINGLETONINSTANCE(CShapeManager);

	if( (pass == 0) && (m_IRenderer) )
	{
		std::vector<CSphere>::iterator itrSphere = pManager->GetSpheres().begin();
		while( itrSphere != pManager->GetSpheres().end() )
		{
			CSphere *pSphere = &(*itrSphere);

			Vec3 objPos;
			static DWORD msgHash_GetGlobalPosition = CHashString(_T("GetGlobalPosition")).GetUniqueID();
			static CHashString hsEntityType(_T("CEntity"));
			if( m_ToolBox->SendMessage(msgHash_GetGlobalPosition, sizeof(Vec3), &objPos, &pSphere->m_hszObjectName, &hsEntityType ) != MSG_HANDLED )
			{
				m_ToolBox->Log( LOGERROR, _T("Could not get global position to draw sphere shape!\n" ) );
			}

			m_IRenderer->DrawSphere( objPos, pSphere->radius, 
				pSphere->red, pSphere->green, pSphere->blue, 
				pSphere->resolution );

			itrSphere++;
		}
	}
	return true;
}

bool CSphereRenderObject::IsLessThan( const IRenderObject * obj, DWORD sortMethod, int curRenderPass )
{
	return true;
}

DWORD CSphereRenderObject::GetRenderPriority()
{
    return 0;
}

UINT CSphereRenderObject::GetNumPass()
{
	return 1;
}

IHashString * CSphereRenderObject::GetBaseSortClass()
{
	return GetComponentType();
}

bool CSphereRenderObject::IsAlphaObject()
{
	return false;
}

IMeshObject* CSphereRenderObject::GetMeshObject()
{
	return NULL;
}
