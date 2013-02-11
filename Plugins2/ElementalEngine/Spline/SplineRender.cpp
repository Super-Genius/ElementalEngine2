///============================================================================
/// \file		SplinePointRender.cpp
/// \brief		Implmentation of CSplineRenderObject Class
/// \date		09-10-2005
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
#define _USE_MATH_DEFINES
#include <math.h>
#include "SplineRender.h"

#include "PerformanceProfiler.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT(CSplineRenderObject);
REGISTER_MESSAGE_HANDLER(SetGlobalPosition, OnSetGlobalPosition, CSplineRenderObject);

CSplineRenderObject::CSplineRenderObject(IHashString *parentName, IHashString *name) :
	m_hszNameType( _T("CSplineRenderObject") ),
	SCENEOBJECTTEMPLATE(CRenderManager, CSplineRenderObject, IRenderObject, parentName, name)
{
	m_ToolBox = EngineGetToolBox();

	m_vPosition.Set( 0.0f, 0.0f, 0.0f );
	m_Ends.Set( 5.0f, 5.0f, 5.0f );

	m_bRender = true;
}

CSplineRenderObject::~CSplineRenderObject( void )
{
	DeInit();
}

IComponent *CSplineRenderObject::Create(int nArgs, va_list argptr)
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
		self = new CSplineRenderObject(parentName, name);
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

void CSplineRenderObject::Init()
{
	GETRENDERERINTERFACEPARAMS renderInterfaceParam;
	static DWORD msgHash_GetRendererInterface = CHashString(_T("GetRendererInterface")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetRendererInterface, sizeof(GETRENDERERINTERFACEPARAMS), &renderInterfaceParam, NULL, NULL );
	m_IRenderer = renderInterfaceParam.m_RendererInterface;

	UpdateGlobalObject();
}

void CSplineRenderObject::DeInit()
{
	RemoveBoundingObject();

	static DWORD msgHash_ParentRemoveBoundingBox = CHashString(_T("ParentRemoveBoundingBox")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_ParentRemoveBoundingBox, 0, NULL, &m_ParentName);
}

void CSplineRenderObject::Serialize( IArchive &ar )
{
	
}

IHashString* CSplineRenderObject::GetComponentType( void )
{
	return &m_hszNameType;
}

bool CSplineRenderObject::IsKindOf( IHashString* compType )
{
	return (compType->GetUniqueID() == m_hszNameType.GetUniqueID() );
}

bool CSplineRenderObject::Render( UINT pass, IEffect * override )
{
	PERFORMANCE_PROFILER_AUTO(CSplineRenderObject_Render);
	if( m_IRenderer )
	{
		if (m_bRender)
		{
			SPLINEINFOPARAMS SplineParams;
			static DWORD msgHash_GetSplineInfo = CHashString(_T("GetSplineInfo")).GetUniqueID();
			if( m_ToolBox->SendMessage(msgHash_GetSplineInfo, sizeof(SPLINEINFOPARAMS), &SplineParams, GetParentName()) != MSG_HANDLED )
			{
				return true;
			}
			bool bSplineLoop = SplineParams.m_bSplineLoop;
			StdString szSplineType = SplineParams.m_szSplineType;

			RENDERPOINTVEC* m_vLineSegments = NULL;
			static DWORD msgHash_GetSplineRenderPoints = CHashString(_T("GetSplineRenderPoints")).GetUniqueID();
			if( m_ToolBox->SendMessage(msgHash_GetSplineRenderPoints, sizeof(RENDERPOINTVEC**), &m_vLineSegments, GetParentName()) != MSG_HANDLED )
			{
				return true;
			}
			if( (m_vLineSegments == NULL) || (m_vLineSegments->size() < 2) )
			{
				return true;
			}
			RENDERPOINTVEC::iterator pt1 = m_vLineSegments->begin();
			RENDERPOINTVEC::iterator pt2 = m_vLineSegments->begin();
			pt2++;

	#ifdef _DEBUG
			RENDERPOINTVEC* m_vFirstDerLineSegments = NULL;
			static DWORD msgHash_GetSplineFirstDerivativePoints = CHashString(_T("GetSplineFirstDerivativePoints")).GetUniqueID();
			if( m_ToolBox->SendMessage(msgHash_GetSplineFirstDerivativePoints, sizeof(RENDERPOINTVEC**), &m_vFirstDerLineSegments, GetParentName()) != MSG_HANDLED )
			{
				return true;
			}
			RENDERPOINTVEC::iterator ptFirstDev1 = m_vFirstDerLineSegments->begin();
			RENDERPOINTVEC::iterator ptFirstDev2 = m_vFirstDerLineSegments->begin();
			ptFirstDev2++;

			RENDERPOINTVEC* m_vSecondDerLineSegments = NULL;
			static DWORD msgHash_GetSplineSecondDerivativePoints = CHashString(_T("GetSplineSecondDerivativePoints")).GetUniqueID();
			if( m_ToolBox->SendMessage(msgHash_GetSplineSecondDerivativePoints, sizeof(RENDERPOINTVEC**), &m_vSecondDerLineSegments, GetParentName()) != MSG_HANDLED )
			{
				return true;
			}
			RENDERPOINTVEC::iterator ptSecondDev1 = m_vSecondDerLineSegments->begin();
			RENDERPOINTVEC::iterator ptSecondDev2 = m_vSecondDerLineSegments->begin();
			ptSecondDev2++;
	#endif

			if( szSplineType == _T("Line") )
			{
				for( unsigned int i = 0 ; i <  m_vLineSegments->size()-1 ; i++,pt1++,pt2++ )
				{
					m_IRenderer->Draw3DLine(pt1->x,
											pt1->y,
											pt1->z,
											pt2->x,
											pt2->y,
											pt2->z,
											255, 255, 0, 1 );
				}
				if( bSplineLoop && (m_vLineSegments->size() > 2) )
				{
					pt1 = m_vLineSegments->end();
					pt2 = m_vLineSegments->begin();
					m_IRenderer->Draw3DLine(pt1->x,
											pt1->y,
											pt1->z,
											pt2->x,
											pt2->y,
											pt2->z,
											255, 255, 0, 1 );
				}
			}
			else if( szSplineType == _T("Bezier") )
			{
				for( unsigned int i = 0 ; i <  m_vLineSegments->size()-1 ; i++,pt1++,pt2++ )
				{
					m_IRenderer->Draw3DLine(pt1->x,
											pt1->y,
											pt1->z,
											pt2->x,
											pt2->y,
											pt2->z,
											255, 255, 0, 1 );
				}
			}
			else 
			{
				return true;
			}
		}
	}

	return true;
}

bool CSplineRenderObject::IsLessThan( const IRenderObject * obj, DWORD sortMethod, int curRenderPass )
{
	return true;
}

DWORD CSplineRenderObject::GetRenderPriority( void )
{
	return 0;
}

UINT CSplineRenderObject::GetNumPass( void )
{
	return 1;
}

void CSplineRenderObject::SetPosition( const Vec3 &position )
{
	m_vPosition.Set( position );
}

Vec3& CSplineRenderObject::GetPosition( void )
{
	return m_vPosition;
}

IHashString* CSplineRenderObject::GetBaseSortClass( void )
{
	return GetComponentType();
}

bool CSplineRenderObject::IsAlphaObject( void )
{
	return false;
}

IMeshObject* CSplineRenderObject::GetMeshObject( void )
{
	return NULL;
}

bool CSplineRenderObject::GetBoundingBox( Vec3 &pos, Vec3 &dim, EulerAngle &rotation )
{
	pos.Set( m_vPosition );
	dim.Set( m_Ends );

	rotation.SetRoll( 0.0f );
	rotation.SetPitch( 0.0f );
	rotation.SetYaw( 0.0f );

	return true;
}

DWORD CSplineRenderObject::OnSetGlobalPosition( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE(sizeof(Vec3), size);
	Vec3 * pos = (Vec3 *)params;
	SetPosition( *pos );

	return MSG_HANDLED_STOP;
}