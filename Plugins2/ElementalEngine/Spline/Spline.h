///============================================================================
/// \file		Spline.h
/// \brief		Declaration of CSplineObject Class
/// \date		09-10-2005
/// \author		D. Patrick Ghiocel
///============================================================================
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
///============================================================================

#ifndef __SPLINEOBJECT_H__
#define __SPLINEOBJECT_H__

#include "CObjectTemplate.hpp"
#include "Vec3.h"
#include "IObject.h"

// quadratic bezier spline stuff
#define	B1(t)		(t*t)
#define	B2(t)		(2*t*(1-t))
#define	B3(t)		((1-t)*(1-t))

// Min Point Counts
const int MIN_POINTS_LINE_OPEN = 2;
const int MIN_POINTS_LINE_CLOSE = 3;
const int MIN_POINTS_BEZIER_OPEN = 4;
const int MIN_POINTS_BEZIER_CLOSED = 5;

class CSplineManager;


class CSplineObject : public CObjectTemplate<IObject>
{
private:
	/// Default Constructor
	CSplineObject(IHashString *parentName, IHashString *name, bool bAddToHier);
	
public:
	/// Default Destructor
	virtual ~CSplineObject();

	static IComponent *Create(int nArgs, va_list argptr);

	/// Init
	virtual void Init();
	/// DeInit
	virtual void DeInit();
	/// Update
	virtual bool Update();

	/// Serialize this component to the archive.
	///	\param ar = reference to the archive class
	virtual void Serialize( IArchive &ar );

	/// return a string of the component type
	/// \return IHashString *reference to textual classname
	virtual IHashString *GetComponentType();

	/// Checks if this is similar to it's base classes component
	/// \return true if it is simlar, else false
	virtual bool IsKindOf(IHashString *compType);

    // MESSAGES
	DWORD OnUpdateSpline(DWORD size, void *params);
	DWORD OnGetSplineInfo( DWORD size, void *params);
	DWORD OnSetSplineInfo( DWORD size, void *params);
	DWORD OnGetSplineRenderPoints( DWORD size, void *params);
	DWORD OnGetSplineFirstDerivativePoints( DWORD size, void *params);
	DWORD OnGetSplineSecondDerivativePoints( DWORD size, void *params);
	DWORD OnEvaluateSpline( DWORD size, void *params );
	DWORD OnEvaluateSplineDistance( DWORD size, void *params );
	//DWORD OnEvaluateSplineDistanceToPoint( DWORD size, void *params );
	DWORD OnEvaluateSpline1stDerivative( DWORD size, void *params );
	DWORD OnEvaluateSpline2ndDerivative( DWORD size, void *params );
	DWORD OnEvaluateSplineLookAhead( DWORD size, void *params );
	DWORD OnAddPoint(DWORD size, void *params);
	DWORD OnAddPointObject(DWORD size, void *params);
	DWORD OnInsertPoint(DWORD size, void *params);
	DWORD OnInsertPointObject(DWORD size, void *params);
	DWORD OnRemovePoint(DWORD size, void *params);

	/// Sets the marker at the specified distance from the beginning.
	/// Populates the parameter with the markers current position
	/// Accepts SPLINEMARKERPARAMS as the parameter
	DWORD OnSetMarker(DWORD size, void *params);

	/// Finds the distance point closest to the specified point
	/// Populates the parameter with the distance value found.
	/// Accepts SPLINEMARKERPARAMS as the parameter
	DWORD OnGetNearestDistance(DWORD size, void *params);

	/// Sets the marker at the specified distance from its current distance.
	/// Populates the parameter with the markers current position.
	/// Accepts SPLINEMARKERPARAMS as the parameter.
	DWORD OnAdvanceMarker(DWORD size, void *params);

	/// Populates the parameter with the markers current position.
	/// Accepts SPLINEMARKERPARAMS as the parameter.
	DWORD OnGetMarkerPosition(DWORD size, void *params);

	/// Clears points ahead of marker that do not influence the marker's position.
	DWORD OnClearLeadingPoints(DWORD size, void *params);

	/// Clears points behind marker that do not influence the marker's position.
	DWORD OnClearTrailingPoints(DWORD size, void *params);

	/// Clears all points.
	DWORD OnClearPoints(DWORD size, void *params);

	/// Turns on spline rendering for this particular spline object
	/// \param size = 0
	/// \param param = NULL
	/// \return MSG_HANDLED_STOP
	DWORD OnSetSplineRenderOn(DWORD size, void *param);

	/// Turns off spline rendering for this particular spline object
	/// \param size = 0
	/// \param param = NULL
	/// \return MSG_HANDLED_STOP
	DWORD OnSetSplineRenderOff(DWORD size, void *param);

	/// Turns on/off spline rendering
	/// \param value = to render (t/f)
	void SetToRender(bool value);

private:
	/// Rebuilds the entire spline
	void BuildSpline( void );

	/// Rebuilds spline segments affected by point at specified index
	void BuildSpline( int pointVecIndex );

	void CalculateCatmullRomPoint( Vec3 *Pt0, Vec3 *Pt1, Vec3 *Pt2, Vec3 *Pt3, float fFactor, Vec3 &PtResult );
	void CalculateCatmullRom1stDerivative( Vec3 *Pt0, Vec3 *Pt1, Vec3 *Pt2, Vec3 *Pt3, float fFactor, Vec3 &PtResult );
	void CalculateCatmullRom2ndDerivative( Vec3 *Pt0, Vec3 *Pt1, Vec3 *Pt2, Vec3 *Pt3, float fFactor, Vec3 &PtResult );
	void CalculateQuadradic( Vec3 Pt0, Vec3 Pt1, Vec3 Pt2, float fFactor, Vec3 &PtResult );

	/// Helper functions for BuildSpline
	void BuildPointSpline( void );
	void BuildLineSpline( void );
	void BuildBezierSpline( void );
	void BuildBezierSpline( int pointVecIndex );
	void BuildBezierSplineSegment( unsigned int pointVecIndex, double detailbias );
	void BuildCatmullRomSplineSegment( Vec3 *Pt0, Vec3 *Pt1, Vec3 *Pt2, Vec3 *Pt3, double detailbias, int pointVecIndex );
	float CalculateRenderPointDistance( unsigned int pointVecIndex );
	void SetMarker( float adistanceFromBegin );
	float GetNearestDistance( const Vec3 *anearestPosition, float aMinDistance );
	void AdvanceMarker( float aadvanceDistance );
	int GetIndexOfDistance( float adistanceFromBegin );
	void CacheLookAhead();

	/// Helper function for calculating the nearest point on the spline. This will find the
	/// two nearest consecutive control points to the specified location, and return the index
	/// to the first one found.
	/// \param testPoint = The location point which to test
	int GetClosestPointIndex( const Vec3 *testPoint );

	/// Pointed to the EE Tool Box
	IToolBox *m_ToolBox;

	/// Class Type
	CHashString m_hszNameType;

	/// Spline Type
	/*
	enum eSPLINETYPE
	{
		Point = 0,		// collection of points
		Line,			// points connected with line segments
		Bezier,			// points controlling a bezier curve 
		eSPLINETYPESize
	};
	eSPLINETYPE m_eSplineType;
	*/
	StdString m_szSplineType;

	/// Closed (true) Or Open (false) Loop
	bool m_bSplineLoop;

	/// Spline Resolution (for Bezier)
	unsigned int m_iResolution;

	/// Vector of the Points
	POINTVEC m_vPointVec;

	/// Points
	RENDERPOINTVEC m_vRenderPointsVec;
	/// First Derivative
	RENDERPOINTVEC m_vFirstDerivativeVec;
	/// Second Derivative
	RENDERPOINTVEC m_vSecondDerivativeVec;

	std::vector< float > m_vDistanceVec;

	/// Look-Ahead cache.
	float m_firstDerivativeMaxLength;
	float m_firstDerivativeMaxXYLength;
	float m_firstDerivativeMaxXZLength;

	float m_secondDerivativeMaxLength;
	float m_secondDerivativeMaxXYLength;
	float m_secondDerivativeMaxXZLength;

	Vec3 m_firstDerivativeLookAheadMax;
	Vec3 m_firstDerivativeXYLookAheadMax;
	Vec3 m_firstDerivativeXZLookAheadMax;
	
	Vec3 m_secondDerivativeLookAheadMax;
	Vec3 m_secondDerivativeXYLookAheadMax;
	Vec3 m_secondDerivativeXZLookAheadMax;


	/// Index marker for render points vector
	unsigned int m_iPositionMarker;

	/// Distance marker for entire spline
	float m_fDistanceMarker;

	/// Pointer to the Render Object
	CSplineRenderObject* m_pRenderObject;

	/// Used to restrict the spline from evaluating past a specified point
	unsigned int m_iMaxSplinePoints;

	// EVENTS
	/// On Colliding with Spline
	StdString m_wszEventCollide;
	/// On Entering a Closed Spline
	StdString m_wszEventEnter;
	/// On Exiting a Closed Spline
	StdString m_wszEventExit;

	/// The manager
	CSplineManager *m_pManager;

	/// Render t/f
	bool m_bToRender;

    bool m_bAddToHierarchy;
};

#endif //__SPLINEOBJECT_H__

