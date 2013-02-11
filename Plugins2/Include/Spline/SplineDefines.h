///============================================================================
/// \file		SplineDefines.h
/// \brief		Structs for spline message parameters
/// \date		10-16-2006
/// \author		Richard Nguyen
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

#ifndef __SPLINEDEFINES_H__
#define __SPLINEDEFINES_H__

class CSplinePointObject;

typedef std::vector< Vec3 > RENDERPOINTVEC;
typedef std::vector< CSplinePointObject* > POINTVEC;

// tolua_begin
struct SPLINEINFOPARAMS
{
	SPLINEINFOPARAMS()
	{
		SplinePointsCount = 0;
		MarkerNearEdge = false;
		PreviousSplinePoint = NULL;
		NextSplinePoint = NULL;
		EndSplinePoint = NULL;
	}

	StdString m_szSplineType;
	bool m_bSplineLoop;
	unsigned int m_iResolution;

	int SplinePointsCount;
	bool MarkerNearEdge;
	Vec3 *PreviousSplinePoint;
	Vec3 *NextSplinePoint;
	Vec3 *EndSplinePoint;
};

struct SPLINELOOKAHEADPARAMS
{
	SPLINELOOKAHEADPARAMS()
	{
		FirstDerivativeMaxLength = 0.0;
		SecondDerivativeMaxLength = 0.0;

		FirstDerivativeXYMaxLength = 0.0;
		SecondDerivativeXYMaxLength = 0.0;

		FirstDerivativeXZMaxLength = 0.0;
		SecondDerivativeXZMaxLength = 0.0;

		FirstDerivativeMax = NULL;
		SecondDerivativeMax = NULL;

		FirstDerivativeXYMax = NULL;
		SecondDerivativeXYMax = NULL;

		FirstDerivativeXZMax = NULL;
		SecondDerivativeXZMax = NULL;
	}

	float FirstDerivativeMaxLength;
	float SecondDerivativeMaxLength;

	float FirstDerivativeXYMaxLength;
	float SecondDerivativeXYMaxLength;

	float FirstDerivativeXZMaxLength;
	float SecondDerivativeXZMaxLength;

	Vec3 *FirstDerivativeMax;
	Vec3 *SecondDerivativeMax;

	Vec3 *FirstDerivativeXYMax;
	Vec3 *SecondDerivativeXYMax;

	Vec3 *FirstDerivativeXZMax;
	Vec3 *SecondDerivativeXZMax;
};

struct SPLINEMARKERPARAMS
{
	SPLINEMARKERPARAMS()
	{
		Distance = 0.0f;
		Position = NULL;
		FirstDerivative = NULL;
		SecondDerivative = NULL;
		bEndOfSpline = false;
	}
	bool bEndOfSpline;
	float Distance;
	Vec3 *Position;
	Vec3 *FirstDerivative;
	Vec3 *SecondDerivative;
};

struct SPLINEGETNEARESTPARAMS
{
	SPLINEGETNEARESTPARAMS()
	{
		Distance = 0.0f;
		MinDistance = 0.0f;
		Position = NULL;
	}

	float Distance;
	float MinDistance;
	Vec3 *Position;
};

// tolua_end

#endif // #ifndef __SPLINEDEFINES_H__