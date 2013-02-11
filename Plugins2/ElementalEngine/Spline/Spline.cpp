///============================================================================
/// \file		Spline.cpp
/// \brief		Implmentation of CSplineObject Class
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
#include "float.h"
#define _USE_MATH_DEFINES
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// TODO <rnguyen>:	Most of these messages need parameters structs.
//					Right now, they are receiving their parameters
//					through memory space intended for output. The
//					hack works, but is confusing to script coders.

REGISTER_COMPONENT(CSplineObject);
REGISTER_MESSAGE_HANDLER(UpdateSpline, OnUpdateSpline, CSplineObject);
REGISTER_MESSAGE_HANDLER(GetSplineInfo, OnGetSplineInfo, CSplineObject );
REGISTER_MESSAGE_HANDLER(SetSplineInfo, OnSetSplineInfo, CSplineObject );
REGISTER_MESSAGE_HANDLER(GetSplineRenderPoints, OnGetSplineRenderPoints, CSplineObject );
REGISTER_MESSAGE_HANDLER(GetSplineFirstDerivativePoints, OnGetSplineFirstDerivativePoints, CSplineObject );
REGISTER_MESSAGE_HANDLER(GetSplineSecondDerivativePoints, OnGetSplineSecondDerivativePoints, CSplineObject );
REGISTER_MESSAGE_HANDLER(EvaluateSpline, OnEvaluateSpline, CSplineObject );
REGISTER_MESSAGE_HANDLER(EvaluateSplineDistance, OnEvaluateSplineDistance, CSplineObject );
//REGISTER_MESSAGE_HANDLER(EvaluateSplineDistanceToPoint, OnEvaluateSplineDistanceToPoint, CSplineObject );
REGISTER_MESSAGE_HANDLER(EvaluateSpline1stDerivative, OnEvaluateSpline1stDerivative, CSplineObject );
REGISTER_MESSAGE_HANDLER(EvaluateSpline2ndDerivative, OnEvaluateSpline2ndDerivative, CSplineObject );
REGISTER_MESSAGE_HANDLER(EvaluateSplineLookAhead, OnEvaluateSplineLookAhead, CSplineObject );
REGISTER_MESSAGE_HANDLER(AddPoint, OnAddPoint, CSplineObject);
REGISTER_MESSAGE_HANDLER(AddPointObject, OnAddPointObject, CSplineObject);
REGISTER_MESSAGE_HANDLER(InsertPoint, OnInsertPoint, CSplineObject);
REGISTER_MESSAGE_HANDLER(InsertPointObject, OnInsertPointObject, CSplineObject);
REGISTER_MESSAGE_HANDLER(RemovePoint, OnRemovePoint, CSplineObject);

// Position marker messages
REGISTER_MESSAGE_HANDLER(SetMarker, OnSetMarker, CSplineObject);
REGISTER_MESSAGE_HANDLER(GetNearestDistance, OnGetNearestDistance, CSplineObject);
REGISTER_MESSAGE_HANDLER(AdvanceMarker, OnAdvanceMarker, CSplineObject);
REGISTER_MESSAGE_HANDLER(GetMarkerPosition, OnGetMarkerPosition, CSplineObject);
REGISTER_MESSAGE_HANDLER(ClearLeadingPoints, OnClearLeadingPoints, CSplineObject);
REGISTER_MESSAGE_HANDLER(ClearTrailingPoints, OnClearTrailingPoints, CSplineObject);
REGISTER_MESSAGE_HANDLER(ClearPoints, OnClearPoints, CSplineObject);

REGISTER_MESSAGE_HANDLER(SetSplineRenderOn, OnSetSplineRenderOn, CSplineObject);
REGISTER_MESSAGE_HANDLER(SetSplineRenderOff, OnSetSplineRenderOff, CSplineObject);

CSplineObject::CSplineObject(IHashString *parentName, IHashString *name, bool bAddToHier) :
	m_hszNameType( _T("CSplineObject") ),
	OBJECTTEMPLATE(CSplineManager, CSplineObject, IObject, parentName, name)
{
	m_ToolBox = EngineGetToolBox();
	
	IHashString *managerName = GetManagerName();
	if (managerName)
	{
		m_pManager = dynamic_cast<CSplineManager*>(m_ToolBox->GetComponent(managerName));
	}	

	m_bAddToHierarchy = bAddToHier;
	if (bAddToHier)
	{
		AddToHierarchy();
	}

	m_szSplineType = _T("Point");
	m_bSplineLoop = false; //Open Loop type
	m_iResolution = 2;

	m_iPositionMarker = 0;
	m_fDistanceMarker = 0.0f;
	m_iMaxSplinePoints = 0;

	m_vPointVec.clear();
	m_vRenderPointsVec.clear();
	m_vFirstDerivativeVec.clear();
	m_vSecondDerivativeVec.clear();
	m_vDistanceVec.clear();

	m_pRenderObject = NULL;
	m_bToRender = true;
}

CSplineObject::~CSplineObject( void )
{
	DeInit();
	if (m_bAddToHierarchy)
	{
		RemoveFromHierarchy();
	}
}

IComponent *CSplineObject::Create(int nArgs, va_list argptr)
{
	IHashString *name;
	IHashString *parentName;
	IObject *self;
	bool bAddToHier;

	name = va_arg(argptr, IHashString *);
	parentName = va_arg(argptr, IHashString *);
	// create the object however you must create it
	try
	{
		// try and create an instance
		if (nArgs == 2)
		{
			self = new CSplineObject(parentName, name, true);
		}
		else if (nArgs == 3)
		{
			bAddToHier = va_arg(argptr, bool);
			self = new CSplineObject(parentName, name, bAddToHier);
		}
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

void CSplineObject::Init( void )
{
	CHashString hszSplineName = GetName();
	StdString wszSplineName = hszSplineName.GetString();

	StdString wszSplineROName = wszSplineName + _T("_RenderObject");

    IComponent *tmpComponent;
	static CHashString SROTypeName("CSplineRenderObject");
	CHashString SROName(wszSplineROName);
	tmpComponent = m_ToolBox->CreateComponent(&SROTypeName, 2, &SROName, &hszSplineName );
	m_pRenderObject = dynamic_cast<CSplineRenderObject*>(tmpComponent);
	m_pRenderObject->Init();
	m_pRenderObject->SetPosition( m_vPosition );

	if (m_pManager)
	{
		m_bToRender = m_pManager->GetGlobalRenderFlag();
		SetToRender(m_bToRender);
	}
}

void CSplineObject::DeInit( void )
{
	if( m_pRenderObject )
	{
		m_pRenderObject->DeInit();
		delete m_pRenderObject;
		m_pRenderObject = NULL;
	}

	for( unsigned int i = 0; i < m_vPointVec.size(); i++ )
	{
		CSplinePointObject* tmpPtr = m_vPointVec[i];
		m_vPointVec[i] = NULL;
		tmpPtr->DeInit();
		delete tmpPtr;
	}
	m_vPointVec.clear();

	m_vRenderPointsVec.clear();
	m_vFirstDerivativeVec.clear();
	m_vSecondDerivativeVec.clear();
}

bool CSplineObject::Update( void )
{
	return true;
}

void CSplineObject::Serialize( IArchive &ar )
{
	if(ar.IsReading())
	{
		ar.Read( m_szSplineType, _T("Type") );
		ar.Read( m_bSplineLoop, _T("Loop") );
		ar.Read( m_iResolution, _T("Resolution") );
		ar.Read( m_wszEventCollide, _T("EventCollide") );
		ar.Read( m_wszEventEnter, _T("EventEnter") );
		ar.Read( m_wszEventExit, _T("EventExit") );
	}

	else
	{
		ar.Write( m_szSplineType, _T("Type") );
		ar.Write( m_bSplineLoop, _T("Loop") );
		ar.Write( m_iResolution, _T("Resolution") );
		ar.Write( m_wszEventCollide, _T("EventCollide") );
		ar.Write( m_wszEventEnter, _T("EventEnter") );
		ar.Write( m_wszEventExit, _T("EventExit") );
	}
}

IHashString* CSplineObject::GetComponentType( void )
{
	return &m_hszNameType;
}

bool CSplineObject::IsKindOf( IHashString* compType )
{
	return (compType->GetUniqueID() == m_hszNameType.GetUniqueID() );
}

void CSplineObject::BuildSpline( void )
{
	m_vRenderPointsVec.clear();
	m_vFirstDerivativeVec.clear();
	m_vSecondDerivativeVec.clear();

	if( m_szSplineType == _T("Point") )
	{
		BuildPointSpline();
	}
	else if( m_szSplineType == _T("Line") )
	{
		BuildLineSpline();
	}
	else if( m_szSplineType == _T("Bezier") )
	{
		BuildBezierSpline();
	}
}

void CSplineObject::BuildSpline( int pointVecIndex )
{
	if( m_szSplineType == _T("Point") )
	{
		BuildPointSpline();
	}
	else if( m_szSplineType == _T("Line") )
	{
		BuildLineSpline();
	}
	else if( m_szSplineType == _T("Bezier") )
	{
		BuildBezierSpline( pointVecIndex );
	}
}

void CSplineObject::BuildPointSpline()
{
	// Do Nothing
}

void CSplineObject::BuildLineSpline()
{
	if( (m_bSplineLoop == true) && (m_iMaxSplinePoints < MIN_POINTS_LINE_CLOSE) )
	{
		return;
	}
	else if( m_iMaxSplinePoints < MIN_POINTS_LINE_OPEN )
	{
		return;
	}

	for( unsigned int i = 0; i < m_iMaxSplinePoints; i++ )
	{
		Vec3 tmp;
		m_vPointVec[i]->GetPosition( (Vec3*)(&tmp) );
		m_vRenderPointsVec.push_back( tmp );
	}
}

void CSplineObject::BuildBezierSpline()
{
	Vec3 Pt0;
	Vec3 Pt1;
	Vec3 Pt2;
	Vec3 Pt3;
	Vec3 PtPre0;
	double detailbias;

	if( (m_bSplineLoop == true) && (m_iMaxSplinePoints < MIN_POINTS_BEZIER_CLOSED) )
	{
		return;
	}
	if( m_iMaxSplinePoints < MIN_POINTS_BEZIER_OPEN )
	{
		return;
	}
	if( m_iResolution < 2 )
	{
		return;
	}
	detailbias = 1.0f / (float)( m_iResolution - 1 );
	if( detailbias <= 0.0f )
	{
		return;
	}

	// Middle Section
	for( unsigned int i = 0; i < m_iMaxSplinePoints - 2; i++ )
	{
		BuildBezierSplineSegment( i, detailbias );
	}
}

void CSplineObject::BuildBezierSpline( int pointVecIndex )
{
	double detailbias;

	if( m_iMaxSplinePoints < 2 )
	{
		return;
	}
	if( m_iResolution < 2 )
	{
		return;
	}
	detailbias = 1.0f / (float)( m_iResolution - 1 );
	if( detailbias <= 0.0f )
	{
		return;
	}

	if( pointVecIndex > 0 )
	{
		BuildBezierSplineSegment( pointVecIndex - 1, detailbias );
	}

	BuildBezierSplineSegment( pointVecIndex, detailbias );

	if( pointVecIndex + 2 < (int)m_vDistanceVec.size() )
	{
		BuildBezierSplineSegment( pointVecIndex + 1, detailbias );
	}
}

void CSplineObject::BuildBezierSplineSegment( unsigned int pointVecIndex, double detailbias )
{
	assert( m_iMaxSplinePoints >= 2 );	// Should never reach here if it's less than 5,
									// but let's make sure we get at least 2
	if( ( pointVecIndex + 1 ) < m_iMaxSplinePoints && pointVecIndex >= 0 )
	{
		Vec3 Pt0;
		Vec3 Pt1;
		Vec3 Pt2;
		Vec3 Pt3;

		m_vPointVec[pointVecIndex]->GetPosition( (Vec3*)(&Pt1) );
		m_vPointVec[pointVecIndex + 1]->GetPosition( (Vec3*)(&Pt2) );

		if( pointVecIndex == 0 )
		{
			Pt0.x = Pt1.x - ( Pt2.x - Pt1.x );
			Pt0.y = Pt1.y - ( Pt2.y - Pt1.y );
			Pt0.z = Pt1.z - ( Pt2.z - Pt1.z );
		}
		else
		{
			m_vPointVec[pointVecIndex - 1]->GetPosition( (Vec3*)(&Pt0) );
		}
		if( ( pointVecIndex + 2 ) >= m_iMaxSplinePoints )
		{
			Pt3.x = Pt2.x + (Pt2.x - Pt1.x );
			Pt3.y = Pt2.y + (Pt2.y - Pt1.y );
			Pt3.z = Pt2.z + (Pt2.z - Pt1.z );
		}
		else
		{
			m_vPointVec[pointVecIndex + 2]->GetPosition( (Vec3*)(&Pt3) );
		}

		BuildCatmullRomSplineSegment( &Pt0, &Pt1, &Pt2, &Pt3, detailbias, pointVecIndex );
	}
}

void CSplineObject::BuildCatmullRomSplineSegment( Vec3 *Pt0, Vec3 *Pt1, Vec3 *Pt2, Vec3 *Pt3, double detailbias, int pointVecIndex )
{
	double count = 0.0f;
	int renderIndex = pointVecIndex * m_iResolution;
	int tempSize = 0;
	float calculatedDistance = 0.0f;

	do
	{
		Vec3 PtResult;
		CalculateCatmullRomPoint( Pt0, Pt1, Pt2, Pt3, (float)count, PtResult );
		tempSize = m_vRenderPointsVec.size();
		assert( ( renderIndex - 1 ) < tempSize );
		if( renderIndex == tempSize )
		{
			m_vRenderPointsVec.push_back( PtResult );
		}
		else
		{
			m_vRenderPointsVec[renderIndex] = PtResult;
		}

		CalculateCatmullRom1stDerivative( Pt0, Pt1, Pt2, Pt3, (float)count, PtResult );
		tempSize = m_vFirstDerivativeVec.size();
		assert( ( renderIndex - 1 ) < tempSize );
		if( renderIndex == tempSize )
		{
			m_vFirstDerivativeVec.push_back( PtResult );
		}
		else
		{
			m_vFirstDerivativeVec[renderIndex] = PtResult;
		}

		CalculateCatmullRom2ndDerivative( Pt0, Pt1, Pt2, Pt3, (float)count, PtResult );
		tempSize = m_vSecondDerivativeVec.size();
		assert( ( renderIndex - 1 ) < tempSize );
		float lastDistance = 0.0f;
		if( renderIndex == tempSize )
		{
			m_vSecondDerivativeVec.push_back( PtResult );
		}
		else
		{
			m_vSecondDerivativeVec[renderIndex] = PtResult;
		}

		if( renderIndex > 0 )
		{
			lastDistance = m_vDistanceVec[renderIndex - 1];
		}

		if( renderIndex == pointVecIndex )
		{
			calculatedDistance = 0.0f;
		}
		else
		{
			calculatedDistance = CalculateRenderPointDistance( renderIndex - 1 );
		}
		calculatedDistance += lastDistance;

		tempSize = m_vDistanceVec.size();
		assert( ( renderIndex - 1 ) < tempSize );
		if( renderIndex == tempSize )
		{
			m_vDistanceVec.push_back( calculatedDistance );
		}
		else
		{
			m_vDistanceVec[renderIndex] = calculatedDistance;
		}

		count = count + detailbias;
		renderIndex++;
	} while( count <= 1 );
}

void CSplineObject::CalculateCatmullRomPoint( Vec3 *Pt0, Vec3 *Pt1, Vec3 *Pt2, Vec3 *Pt3, float fFactor, Vec3 &PtResult )
{
	float t = fFactor;
	float tsquare = fFactor * fFactor;
	float tcube = fFactor * fFactor * fFactor;

	PtResult.x = 0.5f * ( ( 2.0f * Pt1->x )
							+ ( -Pt0->x + Pt2->x ) * t
							+ ( 2.0f * Pt0->x - 5.0f * Pt1->x + 4.0f * Pt2->x - Pt3->x ) * tsquare
							+ ( -Pt0->x + 3.0f * Pt1->x - 3.0f * Pt2->x + Pt3->x ) * tcube);

	PtResult.y = 0.5f * ( ( 2.0f * Pt1->y )
							+ ( -Pt0->y + Pt2->y ) * t
							+ ( 2.0f * Pt0->y - 5.0f * Pt1->y + 4.0f * Pt2->y - Pt3->y ) * tsquare
							+ ( -Pt0->y + 3.0f * Pt1->y - 3.0f * Pt2->y + Pt3->y ) * tcube);

	PtResult.z = 0.5f * ( ( 2.0f * Pt1->z )
							+ ( -Pt0->z + Pt2->z ) * t
							+ ( 2.0f * Pt0->z - 5.0f * Pt1->z + 4.0f * Pt2->z - Pt3->z ) * tsquare
							+ ( -Pt0->z + 3.0f * Pt1->z - 3.0f * Pt2->z + Pt3->z ) * tcube);
}

// First Derivative
void CSplineObject::CalculateCatmullRom1stDerivative( Vec3 *Pt0, Vec3 *Pt1, Vec3 *Pt2, Vec3 *Pt3, float fFactor, Vec3 &PtResult )
{
	float t = fFactor;
	float tsquare = fFactor * fFactor;
	
	PtResult.x = 0.5f * ( ( -Pt0->x + Pt2->x )
						    + 2 * ( 2.0f * Pt0->x - 5.0f * Pt1->x + 4.0f * Pt2->x - Pt3->x ) * t
						    + 3 * ( -Pt0->x + 3.0f * Pt1->x - 3.0f * Pt2->x + Pt3->x ) * tsquare );

	PtResult.y = 0.5f * ( ( -Pt0->y + Pt2->y )
						    + 2 * ( 2.0f * Pt0->y - 5.0f * Pt1->y + 4.0f * Pt2->y - Pt3->y ) * t
						    + 3 * ( -Pt0->y + 3.0f * Pt1->y - 3.0f*Pt2->y + Pt3->y ) * tsquare);

	PtResult.z = 0.5f * ( (-Pt0->z + Pt2->z )
						    + 2 * ( 2.0f * Pt0->z - 5.0f * Pt1->z + 4.0f * Pt2->z - Pt3->z ) * t
						    + 3 * ( -Pt0->z + 3.0f * Pt1->z - 3.0f * Pt2->z + Pt3->z ) * tsquare);
}

// Second Derivative
void CSplineObject::CalculateCatmullRom2ndDerivative( Vec3 *Pt0, Vec3 *Pt1, Vec3 *Pt2, Vec3 *Pt3, float fFactor, Vec3 &PtResult )
{
	float t = fFactor;

	PtResult.x = 0.5f * ( 2 * ( 2.0f * Pt0->x - 5.0f * Pt1->x + 4.0f * Pt2->x - Pt3->x )
						    + 6 * ( -Pt0->x + 3.0f * Pt1->x - 3.0f * Pt2->x + Pt3->x ) * t);

	PtResult.y = 0.5f * ( 2 * ( 2.0f * Pt0->y - 5.0f * Pt1->y + 4.0f * Pt2->y - Pt3->y )
						    + 6 * ( -Pt0->y + 3.0f * Pt1->y - 3.0f * Pt2->y + Pt3->y ) * t);

	PtResult.z = 0.5f * ( 2 * ( 2.0f * Pt0->z - 5.0f * Pt1->z + 4.0f * Pt2->z - Pt3->z )
						    + 6 * ( -Pt0->z + 3.0f * Pt1->z - 3.0f * Pt2->z + Pt3->z ) * t);
}

void CSplineObject::CalculateQuadradic( Vec3 Pt0, Vec3 Pt1, Vec3 Pt2, float fFactor, Vec3 &PtResult )
{
	PtResult.x = Pt0.x*B1(fFactor) + Pt1.x*B2(fFactor) + Pt2.x*B3(fFactor);
	PtResult.y = Pt0.y*B1(fFactor) + Pt1.y*B2(fFactor) + Pt2.y*B3(fFactor);
	PtResult.z = Pt0.z*B1(fFactor) + Pt1.z*B2(fFactor) + Pt2.z*B3(fFactor);
}

float CSplineObject::CalculateRenderPointDistance( unsigned int renderIndex )
{
	assert( ( renderIndex + 1 ) < (int)m_vRenderPointsVec.size() && renderIndex >= 0 );
	Vec3 *renderPoint1 = &m_vRenderPointsVec[renderIndex];
	Vec3 *renderPoint2 = &m_vRenderPointsVec[renderIndex+1];

	float distanceX = renderPoint2->x - renderPoint1->x;
	float distanceY = renderPoint2->y - renderPoint1->y;
	float distanceZ = renderPoint2->z - renderPoint1->z;

	float calculatedDistance = sqrt( pow( distanceX, 2 ) + pow( distanceY, 2 ) + pow( distanceZ, 2 ) );

	return calculatedDistance;
}

DWORD CSplineObject::OnUpdateSpline(DWORD size, void *params)
{
	BuildSpline();

	return MSG_HANDLED_STOP;
}

DWORD CSplineObject::OnGetSplineInfo( DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE( size, sizeof(SPLINEINFOPARAMS) );
    
	SPLINEINFOPARAMS* tmpSplineInfo = (SPLINEINFOPARAMS*)params;

	tmpSplineInfo->m_bSplineLoop = m_bSplineLoop;
	tmpSplineInfo->m_szSplineType = m_szSplineType;
	tmpSplineInfo->m_iResolution = m_iResolution;
	tmpSplineInfo->SplinePointsCount = m_iMaxSplinePoints;
	tmpSplineInfo->MarkerNearEdge = false;

	unsigned int asplinePointIndex = m_iPositionMarker / m_iResolution;
	if( asplinePointIndex + 4 >= m_iMaxSplinePoints )
	{
		tmpSplineInfo->MarkerNearEdge = true;
	}
	if( m_iMaxSplinePoints > 0 && tmpSplineInfo->PreviousSplinePoint != NULL )
	{
		m_vPointVec[ asplinePointIndex ]->GetPosition( tmpSplineInfo->PreviousSplinePoint );
	}
	if( m_iMaxSplinePoints > 1 && tmpSplineInfo->NextSplinePoint != NULL )
	{
		m_vPointVec[ asplinePointIndex + 1 ]->GetPosition( tmpSplineInfo->NextSplinePoint );
	}
	if( m_iMaxSplinePoints > 0 && tmpSplineInfo->EndSplinePoint != NULL )
	{
		 m_vPointVec[ m_iMaxSplinePoints - 1 ]->GetPosition( tmpSplineInfo->EndSplinePoint );
	}

	return MSG_HANDLED_STOP;
}

DWORD CSplineObject::OnSetSplineInfo( DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE( size, sizeof(SPLINEINFOPARAMS) );
    
	SPLINEINFOPARAMS* tmpSplineInfo = (SPLINEINFOPARAMS*)params;

	m_bSplineLoop = tmpSplineInfo->m_bSplineLoop;
	if( (tmpSplineInfo->m_szSplineType == _T("Point")) ||
		(tmpSplineInfo->m_szSplineType == _T("Line")) ||
		(tmpSplineInfo->m_szSplineType == _T("Bezier")) )
	{
		m_szSplineType = tmpSplineInfo->m_szSplineType;
	}
	m_iResolution = tmpSplineInfo->m_iResolution;

	BuildSpline();

	return MSG_HANDLED_STOP;
}



DWORD CSplineObject::OnGetSplineRenderPoints( DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE( size, sizeof(RENDERPOINTVEC**) );
	RENDERPOINTVEC** tmpptr = (RENDERPOINTVEC**)params;
	*tmpptr	= &m_vRenderPointsVec;
	
	return MSG_HANDLED_STOP;
}

DWORD CSplineObject::OnGetSplineFirstDerivativePoints( DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE( size, sizeof(RENDERPOINTVEC**) );
	RENDERPOINTVEC** tmpptr = (RENDERPOINTVEC**)params;
	*tmpptr	= &m_vFirstDerivativeVec;
	
	return MSG_HANDLED_STOP;
}

DWORD CSplineObject::OnGetSplineSecondDerivativePoints( DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE( size, sizeof(RENDERPOINTVEC**) );
	RENDERPOINTVEC** tmpptr = (RENDERPOINTVEC**)params;
	*tmpptr	= &m_vSecondDerivativeVec;
	
	return MSG_HANDLED_STOP;
}

DWORD CSplineObject::OnEvaluateSpline( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(Vec3) );
	Vec3* point = (Vec3*)params;

	int index = (int)(point->x * m_vRenderPointsVec.size());

	point->Set( m_vRenderPointsVec[index] );

	return MSG_HANDLED_STOP;
}

DWORD CSplineObject::OnEvaluateSplineDistance( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(Vec3) );
	
	float splineDistance = m_vDistanceVec[(int)m_vDistanceVec.size() - 1];

	Vec3 *vec = (Vec3*)params;

	vec->Set( splineDistance, splineDistance, splineDistance );

	return MSG_HANDLED_STOP;
}
/*
DWORD CSplineObject::OnEvaluateSplineDistanceToPoint( DWORD size, void *params )
{
	return MSG_HANDLED_STOP;
}
*/

DWORD CSplineObject::OnEvaluateSpline1stDerivative( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(Vec3) );
	Vec3* point = (Vec3*)params;

	int index = (int)(point->x * m_vFirstDerivativeVec.size());

	point->Set( m_vFirstDerivativeVec[index] );

	return MSG_HANDLED_STOP;
}

DWORD CSplineObject::OnEvaluateSpline2ndDerivative( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(Vec3) );
	Vec3* point = (Vec3*)params;

	int index = (int)(point->x * m_vSecondDerivativeVec.size());

	point->Set( m_vSecondDerivativeVec[index] );

	return MSG_HANDLED_STOP;
}

DWORD CSplineObject::OnEvaluateSplineLookAhead( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(SPLINELOOKAHEADPARAMS) );
	SPLINELOOKAHEADPARAMS *lookAheadParams = (SPLINELOOKAHEADPARAMS*)params;

	lookAheadParams->FirstDerivativeMaxLength = m_firstDerivativeMaxLength;
	lookAheadParams->SecondDerivativeMaxLength = m_secondDerivativeMaxLength;

	lookAheadParams->FirstDerivativeXYMaxLength = m_firstDerivativeMaxXYLength;
	lookAheadParams->SecondDerivativeXYMaxLength = m_secondDerivativeMaxXYLength;

	lookAheadParams->FirstDerivativeXZMaxLength = m_firstDerivativeMaxXZLength;
	lookAheadParams->SecondDerivativeXZMaxLength = m_secondDerivativeMaxXZLength;

	if( lookAheadParams->FirstDerivativeMax != NULL )
	{
		lookAheadParams->FirstDerivativeMax->Set( m_firstDerivativeLookAheadMax );
	}

	if( lookAheadParams->SecondDerivativeMax != NULL )
	{
		lookAheadParams->SecondDerivativeMax->Set( m_secondDerivativeLookAheadMax );
	}

	if( lookAheadParams->FirstDerivativeXYMax != NULL )
	{
		lookAheadParams->FirstDerivativeXYMax->Set( m_firstDerivativeXYLookAheadMax );
	}

	if( lookAheadParams->SecondDerivativeXYMax != NULL )
	{
		lookAheadParams->SecondDerivativeXYMax->Set( m_secondDerivativeXYLookAheadMax );
	}

	if( lookAheadParams->FirstDerivativeXZMax != NULL )
	{
		lookAheadParams->FirstDerivativeXZMax->Set( m_firstDerivativeXZLookAheadMax );
	}

	if( lookAheadParams->SecondDerivativeXZMax != NULL )
	{
		lookAheadParams->SecondDerivativeXZMax->Set( m_secondDerivativeXZLookAheadMax );
	}

	return MSG_HANDLED_STOP;
}

DWORD CSplineObject::OnAddPoint(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE( size, sizeof(Vec3) );
	Vec3* pPoint = (Vec3*)params;
	
	CSplinePointObject* pSplinePoint;

	if( (int)m_vPointVec.size() > m_iMaxSplinePoints )
	{
		pSplinePoint = m_vPointVec[m_iMaxSplinePoints];
		pSplinePoint->SetPosition( pPoint );
	}
	else
	{
		CHashString hszSplineName = GetName();
		StdString wszSplineName = hszSplineName.GetString();
		StdString wszSplinePointName = wszSplineName + _T("_SplinePoint");
		char temp[64];
		_itot( m_vPointVec.size(), temp, 10);
		wszSplinePointName += temp;
			
		IComponent *tmpComponent;
		static CHashString SPOTypeName("CSplinePointObject");
		CHashString SPName(wszSplinePointName);
		tmpComponent = m_ToolBox->CreateComponent(&SPOTypeName, 2, &SPName, &hszSplineName );
		pSplinePoint = dynamic_cast<CSplinePointObject*>(tmpComponent);
		//pSplinePoint->OnSetPosition( sizeof(pPoint), pPoint );
		pSplinePoint->OnSetGlobalPosition( sizeof(*pPoint), pPoint );
		pSplinePoint->Init();
	}

	m_iMaxSplinePoints++;
	if( m_iMaxSplinePoints > 1 )
	{
		BuildSpline( m_iMaxSplinePoints - 2 );
	}

	return MSG_HANDLED_STOP;
}

DWORD CSplineObject::OnAddPointObject(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE( size, sizeof(CSplinePointObject*) );
	CSplinePointObject* pSplinePoint = (CSplinePointObject*)params;

	m_vPointVec.push_back( pSplinePoint );

	return MSG_HANDLED_STOP;
}

DWORD CSplineObject::OnInsertPoint(DWORD size, void *params)
{
	return MSG_HANDLED_STOP;
}

DWORD CSplineObject::OnInsertPointObject(DWORD size, void *params)
{
	return MSG_HANDLED_STOP;
}

DWORD CSplineObject::OnRemovePoint(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE( size, sizeof(IHashString*) );
	IHashString* phszName = (IHashString*)params;

	POINTVEC::iterator itr = m_vPointVec.begin();
	for( unsigned int i = 0; i < m_vPointVec.size(); i++, itr++ )
	{
		if( m_vPointVec[i]->GetName()->GetUniqueID() == phszName->GetUniqueID() )
		{
			CSplinePointObject* tmpPtr = *itr;
			tmpPtr->DeInit();
			delete tmpPtr;
			m_vPointVec.erase( itr );
			break;
        }
	}

	BuildSpline();

	return MSG_HANDLED_STOP;
}

DWORD CSplineObject::OnSetMarker(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE( size, sizeof(SPLINEMARKERPARAMS) );
	SPLINEMARKERPARAMS *asetMarkerParams = (SPLINEMARKERPARAMS*)params;

	SetMarker( asetMarkerParams->Distance );
	if( asetMarkerParams->Position != NULL )
	{
		unsigned int vecSize = m_vRenderPointsVec.size();
		if( vecSize <= 0 )
		{
			asetMarkerParams->Position->Set( 0.0f, 0.0f, 0.0f );
		}
		else
		{
			if( vecSize <= m_iPositionMarker )
			{
				unsigned int lastPosition = vecSize - 1;
				m_iPositionMarker = lastPosition;
			}
			asetMarkerParams->Position->Set( m_vRenderPointsVec[m_iPositionMarker] );
		}
	}

	return MSG_HANDLED_STOP;
}

DWORD CSplineObject::OnGetNearestDistance(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE( size, sizeof(SPLINEGETNEARESTPARAMS) );
	SPLINEGETNEARESTPARAMS *getNearestParams = (SPLINEGETNEARESTPARAMS*)params;
	assert( getNearestParams->Position );

	getNearestParams->Distance = GetNearestDistance( getNearestParams->Position, getNearestParams->MinDistance );
	return MSG_HANDLED_STOP;
}

DWORD CSplineObject::OnAdvanceMarker(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE( size, sizeof(SPLINEMARKERPARAMS) );
	SPLINEMARKERPARAMS *asetMarkerParams = (SPLINEMARKERPARAMS*)params;

	AdvanceMarker( asetMarkerParams->Distance );

	if( asetMarkerParams->Position != NULL && (int)m_vRenderPointsVec.size() > m_iPositionMarker )
	{
		asetMarkerParams->Position->Set( m_vRenderPointsVec[m_iPositionMarker] );
	}
	if( asetMarkerParams->FirstDerivative != NULL && (int)m_vFirstDerivativeVec.size() > m_iPositionMarker )
	{
		asetMarkerParams->FirstDerivative->Set( m_vFirstDerivativeVec[m_iPositionMarker] );
	}
	if( asetMarkerParams->SecondDerivative != NULL && (int)m_vSecondDerivativeVec.size() > m_iPositionMarker )
	{
		asetMarkerParams->SecondDerivative->Set( m_vSecondDerivativeVec[m_iPositionMarker] );
	}

	// check to see if we've hit the end of the spline
	int adistanceVecCount = m_vDistanceVec.size();
	if (adistanceVecCount)
	{
		float splineDistance = m_vDistanceVec[adistanceVecCount - 1];
		if (m_fDistanceMarker == splineDistance)
		{
			asetMarkerParams->bEndOfSpline = true;
		}
		return MSG_HANDLED_STOP;
	}
	return MSG_ERROR;
}

DWORD CSplineObject::OnGetMarkerPosition(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE( size, sizeof(SPLINEMARKERPARAMS) );
	SPLINEMARKERPARAMS *asetMarkerParams = (SPLINEMARKERPARAMS*)params;

	if( asetMarkerParams->Position != NULL )
	{
		asetMarkerParams->Position->Set( m_vRenderPointsVec[m_iPositionMarker] );
	}

	return MSG_HANDLED_STOP;
}

DWORD CSplineObject::OnClearLeadingPoints(DWORD size, void *params)
{
	unsigned int amarkerSplinePointIndex = m_iPositionMarker / m_iResolution;
	if( amarkerSplinePointIndex + 3 < m_iMaxSplinePoints )
	{
		m_iMaxSplinePoints = amarkerSplinePointIndex + 3;

		int avectorResize = m_iMaxSplinePoints * m_iResolution;
		m_vDistanceVec.resize(avectorResize);
		m_vRenderPointsVec.resize(avectorResize);
		m_vFirstDerivativeVec.resize(avectorResize);
		m_vSecondDerivativeVec.resize(avectorResize);
	}

	return MSG_HANDLED_STOP;
}

DWORD CSplineObject::OnClearTrailingPoints(DWORD size, void *params)
{
	unsigned int amarkerSplinePointIndex = m_iPositionMarker / m_iResolution;
	if( amarkerSplinePointIndex > 1 )
	{
		int aindexShiftBack = amarkerSplinePointIndex - 1;
		m_iMaxSplinePoints -= aindexShiftBack;
		unsigned int avectorResize = ( m_iMaxSplinePoints - 1 ) * m_iResolution;

		CSplinePointObject *asplinePointObject = NULL;
		CSplinePointObject *asplinePointObjectShiftBack = NULL;
		for( unsigned int i = 0; i < m_iMaxSplinePoints; i++ )
		{
			asplinePointObject = m_vPointVec[i];
			asplinePointObjectShiftBack = m_vPointVec[i + aindexShiftBack];
			Vec3 tempVec;
			asplinePointObjectShiftBack->GetPosition( &tempVec );

			asplinePointObject->SetPosition( &tempVec );
		}

		unsigned int avectorsIteration = aindexShiftBack * m_iResolution;
		float adistanceShiftBack = m_vDistanceVec[avectorsIteration];
		m_iPositionMarker -= avectorsIteration;
		for( unsigned int i = 0; i < avectorResize; i++)
		{
			m_vDistanceVec[i] = m_vDistanceVec[ avectorsIteration ] - adistanceShiftBack;
			m_vRenderPointsVec[i] = m_vRenderPointsVec[ avectorsIteration ];
			m_vFirstDerivativeVec[i] = m_vFirstDerivativeVec[ avectorsIteration ];
			m_vSecondDerivativeVec[i] = m_vSecondDerivativeVec[ avectorsIteration ];

			avectorsIteration++;
		}
		m_fDistanceMarker -= adistanceShiftBack;

		m_vDistanceVec.resize(avectorResize);
		m_vRenderPointsVec.resize(avectorResize);
		m_vFirstDerivativeVec.resize(avectorResize);
		m_vSecondDerivativeVec.resize(avectorResize);
	}

	return MSG_HANDLED_STOP;
}

DWORD CSplineObject::OnClearPoints(DWORD size, void *params)
{
	m_iPositionMarker = 0;
	m_fDistanceMarker = 0;
	m_iMaxSplinePoints = 0;

	m_vDistanceVec.resize(0);
	m_vRenderPointsVec.resize(0);
	m_vFirstDerivativeVec.resize(0);
	m_vSecondDerivativeVec.resize(0);

	return MSG_HANDLED_STOP;
}

void CSplineObject::SetMarker( float adistanceFromBegin )
{
	int adistanceVecCount = m_vDistanceVec.size();
	if( adistanceVecCount > 0 )
	{
		float splineDistance = m_vDistanceVec[adistanceVecCount - 1];
		m_fDistanceMarker = adistanceFromBegin;
		if( m_fDistanceMarker < 0 )
		{
			m_fDistanceMarker = 0;
		}
		if( m_fDistanceMarker > splineDistance )
		{
			m_fDistanceMarker = splineDistance;
		}

		// The position marker has changed. Review the look-ahead second derivative.
		int oldPositionPointIndex = m_iPositionMarker / m_iResolution;

		m_iPositionMarker = GetIndexOfDistance( m_fDistanceMarker );
		int newPositionPointIndex = m_iPositionMarker / m_iResolution;

		if( newPositionPointIndex != oldPositionPointIndex )
		{
			CacheLookAhead();
		}
		assert( m_iPositionMarker >= 0 && m_iPositionMarker < (int)m_vDistanceVec.size() );
	}
}

int CSplineObject::GetClosestPointIndex( const Vec3 *testPoint )
{
	int retIndex = -1;
	int pointVecCount = (int)m_vPointVec.size();
	if( pointVecCount == 1 )
	{
		retIndex = 0;
	}
	else if( pointVecCount > 1 )
	{
		Vec3 pointVec1;
		Vec3 pointVec2;
		float aclosestLength = FLT_MAX;
		for( int i = 0; i < pointVecCount-1; i++ )
		{
			Vec3 testPointEditting( *testPoint );

			m_vPointVec[i]->GetPosition( &pointVec1 );
			m_vPointVec[i+1]->GetPosition( &pointVec2 );

			Vec3 directionVec;
			float angleY;
			float angleZ;
			float xPrime;
			float yPrime;
			float zPrime;

			// Rotate the directional vector so we would only have to deal with 1 dimension
			directionVec = pointVec2 - pointVec1;
			angleZ = asin( directionVec.y / sqrt( pow( directionVec.x, 2 ) + pow( directionVec.y, 2 ) ) );

			xPrime = directionVec.x - ( directionVec.x * cos( -angleZ ) - directionVec.y * sin( -angleZ ) );
			yPrime = directionVec.y - ( directionVec.x * sin( -angleZ ) + directionVec.y * cos( -angleZ ) );
			pointVec2.x -= xPrime;
			pointVec2.y -= yPrime;

			directionVec = pointVec2 - pointVec1;
			angleY = asin( directionVec.z / sqrt( pow( directionVec.x, 2 ) + pow( directionVec.z, 2 ) ) );

			xPrime = directionVec.x - ( directionVec.x * cos( -angleY ) - directionVec.y * sin( -angleY ) );
			zPrime = directionVec.z - ( directionVec.x * sin( -angleY ) + directionVec.z * cos( -angleY ) );
			pointVec2.x -= xPrime;
			pointVec2.z -= zPrime;

			// Rotate the test point with that same angle so we can compare
			directionVec = testPointEditting - pointVec1;
			xPrime = directionVec.x - ( directionVec.x * cos( -angleZ ) - directionVec.y * sin( -angleZ ) );
			yPrime = directionVec.y - ( directionVec.x * sin( -angleZ ) + directionVec.y * cos( -angleZ ) );
			testPointEditting.x -= xPrime;
			testPointEditting.y -= yPrime;

			directionVec = testPointEditting - pointVec1;
			xPrime = directionVec.x - ( directionVec.x * cos( -angleY ) - directionVec.y * sin( -angleY ) );
			zPrime = directionVec.z - ( directionVec.x * sin( -angleY ) + directionVec.z * cos( -angleY ) );
			testPointEditting.x -= xPrime;
			testPointEditting.z -= zPrime;

			// If the x value of testPointEditting is between the x values of the pointVecs
			if( ( pointVec1.x <= pointVec2.x && testPointEditting.x > pointVec1.x && testPointEditting.x <= pointVec2.x )
				|| ( testPointEditting.x <= pointVec1.x && testPointEditting.x > pointVec2.x && testPointEditting.x <= pointVec1.x )
				)
			{
				float closestDistanceFromPoints = abs( pointVec1.y - testPointEditting.y );
				if( closestDistanceFromPoints < aclosestLength )
				{
					aclosestLength = closestDistanceFromPoints;
					retIndex = i;
				}
			}
			// Test the endpoints
			else
			{
				Vec3 displacement1 = testPointEditting - pointVec1;
				float distance1 = displacement1.Length();

				Vec3 displacement2 = testPointEditting - pointVec2;
				float distance2 = displacement2.Length();

				float shortestDistance = distance1 < distance2 ? distance1 : distance2;
				if( shortestDistance < aclosestLength )
				{
					aclosestLength = shortestDistance;
					retIndex = i;
				}
			}
		}
	}
	else
	{
		assert( 0 );
	}
	return retIndex;
}

float CSplineObject::GetNearestDistance( const Vec3 *anearestPosition, float aMinDistance )
{
	float retVal = 0.0f;
	if( (int)m_vRenderPointsVec.size() > 0 )
	{
		int pointIndex = GetClosestPointIndex( anearestPosition );
		int renderPointIndex = pointIndex * m_iResolution;
		int renderPointEndIndex = renderPointIndex + m_iResolution;
		float aclosestLength = FLT_MAX;
		float aclosestDistanceFromMarker = FLT_MAX;
		Vec3 distanceVec;
		for( int i = renderPointIndex; i < renderPointEndIndex; i++ )
		{
			const Vec3 *iteratedVec = &m_vRenderPointsVec[ i ];
			distanceVec.Set( *iteratedVec );

			distanceVec -= *anearestPosition;
			float distanceLength = distanceVec.Length();
			float distanceLength2 = distanceLength;

			if( (i + 1) < renderPointEndIndex )
			{
				const Vec3 *iteratedVec2 = &m_vRenderPointsVec[ i + 1 ];
				distanceVec.Set( *iteratedVec2 );

				distanceVec -= *anearestPosition;
				distanceLength2 = distanceVec.Length();
			}
			
			// The spline segment is not intersected by the min distance
			if( distanceLength >= aMinDistance && distanceLength2 >= aMinDistance )
			{
				if( distanceLength < aclosestLength )
				{
					aclosestLength = distanceLength;
					retVal = m_vDistanceVec[i];
				}
			}
			// The spline segment is intersected with first point outside radius
			else if( distanceLength >= aMinDistance )
			{
				float distanceFromMarker = abs( distanceLength - m_fDistanceMarker );
				if( distanceFromMarker < aclosestDistanceFromMarker )
				{
					aclosestDistanceFromMarker = distanceFromMarker;
					aclosestLength = distanceLength;
					retVal = m_vDistanceVec[i];
				}
			}
			// The spline segment is intersected with second point outside radius
			else if( distanceLength2 >= aMinDistance && (i + 1) < renderPointEndIndex )
			{
				float distanceFromMarker = abs( distanceLength - m_fDistanceMarker );
				if( distanceFromMarker < aclosestDistanceFromMarker )
				{
					aclosestDistanceFromMarker = distanceFromMarker;
					aclosestLength = distanceLength2;
					retVal = m_vDistanceVec[i + 1];
				}
			}
		}
	}
	return retVal;
}

void CSplineObject::AdvanceMarker( float aadvanceDistance )
{
	SetMarker( m_fDistanceMarker + aadvanceDistance );
}

/// Performs a binary search on the distance vectors for the closest match.
int CSplineObject::GetIndexOfDistance( float adistanceFromBegin )
{
	int aindex = -1;
	int adistanceVecCount = m_vDistanceVec.size();

	int alowerBoundIndex = 0;
	int aupperBoundIndex = adistanceVecCount - 1;

	float alowerBound = m_vDistanceVec[alowerBoundIndex];
	float aupperBound = m_vDistanceVec[aupperBoundIndex];

	float atempDistance;
	int atempIndex;

	while( aindex == -1 )
	{
		if( alowerBoundIndex == ( aupperBoundIndex - 1 ) )
		{
			aindex = alowerBoundIndex;
		}
		else
		{
			atempIndex = ( alowerBoundIndex + aupperBoundIndex ) / 2;
			atempDistance = m_vDistanceVec[atempIndex];
			if( adistanceFromBegin < atempDistance )
			{
				aupperBoundIndex = atempIndex;
			}
			else
			{
				alowerBoundIndex = atempIndex;
			}
		}
	}

	return aindex;
}

void CSplineObject::CacheLookAhead()
{
	unsigned int currentPointPosition = m_iPositionMarker / m_iResolution; 
	unsigned int lookAheadStartPosition = ( currentPointPosition + 1 ) * m_iResolution;
	unsigned int lookAheadEndPosition = lookAheadStartPosition + m_iResolution;
	unsigned int vectorFirstSize = m_vSecondDerivativeVec.size();
	unsigned int vectorSecondSize = m_vSecondDerivativeVec.size();

	Vec3 *iteratedDerivative;
	float length;
	float lengthXY;
	float lengthXZ;

	m_firstDerivativeMaxLength = 0.0;
	m_firstDerivativeMaxXYLength = 0.0;
	m_firstDerivativeMaxXZLength = 0.0;

	m_secondDerivativeMaxLength = 0.0;
	m_secondDerivativeMaxXYLength = 0.0;
	m_secondDerivativeMaxXZLength = 0.0;

	m_firstDerivativeLookAheadMax.Set( 0.0, 0.0, 0.0 );
	m_firstDerivativeXYLookAheadMax.Set( 0.0, 0.0, 0.0 );
	m_firstDerivativeXZLookAheadMax.Set( 0.0, 0.0, 0.0 );

	m_secondDerivativeLookAheadMax.Set( 0.0, 0.0, 0.0 );
	m_secondDerivativeXYLookAheadMax.Set( 0.0, 0.0, 0.0 );
	m_secondDerivativeXZLookAheadMax.Set( 0.0, 0.0, 0.0 );

	for( unsigned int i = lookAheadStartPosition; i < lookAheadEndPosition && (i < vectorFirstSize || i < vectorSecondSize); i++ )
	{
		if( i < vectorFirstSize )
		{
			iteratedDerivative = (Vec3*)(&m_vFirstDerivativeVec[i]);
			length = iteratedDerivative->Length();
			if( length > m_firstDerivativeMaxLength )
			{
				m_firstDerivativeMaxLength = length;
				m_firstDerivativeLookAheadMax.Set( iteratedDerivative->x, iteratedDerivative->y, iteratedDerivative->z );
			}

			lengthXY = sqrt( pow( iteratedDerivative->x, 2 ) + pow( iteratedDerivative->y, 2 ) );
			if( lengthXY > m_firstDerivativeMaxXYLength )
			{
				m_firstDerivativeMaxXYLength = lengthXY;
				m_firstDerivativeXYLookAheadMax.Set( iteratedDerivative->x, iteratedDerivative->y, iteratedDerivative->z );
			}

			lengthXZ = sqrt( pow( iteratedDerivative->x, 2 ) + pow( iteratedDerivative->z, 2 ) );
			if( lengthXZ > m_firstDerivativeMaxXZLength )
			{
				m_firstDerivativeMaxXZLength = lengthXZ;
				m_firstDerivativeXZLookAheadMax.Set( iteratedDerivative->x, iteratedDerivative->y, iteratedDerivative->z );
			}
		}

		if( i < vectorSecondSize )
		{
			iteratedDerivative = (Vec3*)(&m_vSecondDerivativeVec[i]);
			length = iteratedDerivative->Length();
			if( length > m_secondDerivativeMaxLength )
			{
				m_secondDerivativeMaxLength = length;
				m_secondDerivativeLookAheadMax.Set( iteratedDerivative->x, iteratedDerivative->y, iteratedDerivative->z );
			}

			lengthXY = sqrt( pow( iteratedDerivative->x, 2 ) + pow( iteratedDerivative->y, 2 ) );
			if( lengthXY > m_secondDerivativeMaxXYLength )
			{
				m_secondDerivativeMaxXYLength = lengthXY;
				m_secondDerivativeXYLookAheadMax.Set( iteratedDerivative->x, iteratedDerivative->y, iteratedDerivative->z );
			}

			lengthXZ = sqrt( pow( iteratedDerivative->x, 2 ) + pow( iteratedDerivative->z, 2 ) );
			if( lengthXZ > m_secondDerivativeMaxXZLength )
			{
				m_secondDerivativeMaxXZLength = lengthXZ;
				m_secondDerivativeXZLookAheadMax.Set( iteratedDerivative->x, iteratedDerivative->y, iteratedDerivative->z );
			}
		}
	}

//	return maxValue;
}

DWORD CSplineObject::OnSetSplineRenderOn(DWORD size, void *param)
{
	SetToRender(true);
	return MSG_HANDLED_STOP;
}

DWORD CSplineObject::OnSetSplineRenderOff(DWORD size, void *param)
{
	SetToRender(false);
	return MSG_HANDLED_STOP;
}

void CSplineObject::SetToRender( bool value )
{
	if (m_pRenderObject)
	{
		m_pRenderObject->SetToRender(value);
	}
	m_bToRender = value;
}