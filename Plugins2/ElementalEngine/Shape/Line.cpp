///============================================================================
/// \file		Line.cpp
/// \brief		Implmentation of CLineRenderObject Object Class
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
#include "Line.h"
#include "ShapeManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT( CLineRenderObject );

CLineRenderObject::CLineRenderObject(IHashString *parentName, IHashString *name) :
	m_hszNameType( _T("CLineRenderObject") ),
	SCENEOBJECTTEMPLATE(CShapeManager , CLineRenderObject, IRenderObject, parentName, name)
{
	m_vPosition.Set( 0, 0, 0 );

	m_IRenderer = NULL;
}

CLineRenderObject::~CLineRenderObject()
{
	DeInit();
}

IComponent *CLineRenderObject::Create( int nArgs, va_list argptr )
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
		self = new CLineRenderObject( parentName, name );
	}
	catch( ... )
	{
		return NULL;
	}

	return self;
}

void CLineRenderObject::Serialize( IArchive &ar )
{
	if( ar.IsReading() )
	{
	}
	else
	{
	}
}

void CLineRenderObject::Init()
{
	GETRENDERERINTERFACEPARAMS renderInterfaceParam;
	static DWORD msgHash_GetRendererInterface = CHashString(_T("GetRendererInterface")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_GetRendererInterface, sizeof(GETRENDERERINTERFACEPARAMS), &renderInterfaceParam ) == MSG_HANDLED )
		m_IRenderer = renderInterfaceParam.m_RendererInterface;

	CShapeManager *pManager = SINGLETONINSTANCE(CShapeManager);
	pManager->AddShapeRenderObject( this );
}

bool CLineRenderObject::Update()
{
	return true;
}

void CLineRenderObject::DeInit()
{
}

IHashString* CLineRenderObject::GetComponentType()
{
	return &m_hszNameType;
}

bool CLineRenderObject::IsKindOf( IHashString* compType )
{
	return ( compType->GetUniqueID() == m_hszNameType.GetUniqueID() );
}

bool CLineRenderObject::Render( UINT pass, IEffect * override )
{
	CShapeManager *pManager = SINGLETONINSTANCE(CShapeManager);

	if( (pass == 0) && (m_IRenderer) )
	{
		std::vector<CLine>::iterator itrLine = pManager->GetLines().begin();
		while( itrLine != pManager->GetLines().end() )
		{
			CLine *pLine = &(*itrLine);

			Vec3 objPos;
			static DWORD msgHash_GetGlobalPosition = CHashString(_T("GetGlobalPosition")).GetUniqueID();
			static CHashString hsEntityType(_T("CEntity"));
			if( m_ToolBox->SendMessage(msgHash_GetGlobalPosition, sizeof(Vec3), &objPos, &pLine->m_hszObjectName, &hsEntityType ) == MSG_HANDLED )
			{
				Vec3 start = ( pLine->start + objPos );
				Vec3 end = ( pLine->end + objPos );
				m_IRenderer->Draw3DLine( start.x, start.y, start.z, 
					end.x, end.y, end.z,
					pLine->red, pLine->green, pLine->green,
					pLine->width );
			}
			else
			{
				m_ToolBox->Log( LOGERROR, _T("Could not get global position to draw line shape!\n" ) );
			}

			itrLine++;
		}
	}

	return true;
}

bool CLineRenderObject::IsLessThan( const IRenderObject * obj, DWORD sortMethod, int curRenderPass )
{
	return true;
}

DWORD CLineRenderObject::GetRenderPriority()
{
    return 0;
}

UINT CLineRenderObject::GetNumPass()
{
	return 1;
}

IHashString * CLineRenderObject::GetBaseSortClass()
{
	return GetComponentType();
}

bool CLineRenderObject::IsAlphaObject()
{
	return false;
}

IMeshObject* CLineRenderObject::GetMeshObject()
{
	return NULL;
}
