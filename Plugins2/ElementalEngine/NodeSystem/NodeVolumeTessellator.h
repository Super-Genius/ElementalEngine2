///============================================================================
/// \file		NodeVolumeTessellator.h
/// \brief		Declaration of NodeVolumeTessellator Helper Class
/// \date		11-20-2007
/// \author		Brendan Geiger
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

#ifndef __NODVOLUMETESSELLATOR_H__
#define __NODVOLUMETESSELLATOR_H__

class NVTPoint
{
public:
	float x;
	float y;
	int type;
	int index;
	NVTPoint()
	{
		x = 0.0f;
		y = 0.0f;
		type = 0;
		index = 0;
	}
	NVTPoint(float X, float Y)
	{
		x = X;
		y = Y;
		type = 0;
		index = 0;
	}
	NVTPoint(float X, float Y, int pointIndex)
	{
		x = X;
		y = Y;
		type = 0;
		index = pointIndex;
	}
};

class NVTPolygon
{
public:
	NVTPolygon()
	{
	}
	~NVTPolygon()
	{
	}
	void addPoint(float x, float y, int pointIndex)
	{
		points.push_back( NVTPoint(x,y,pointIndex) );
//		print();
	}

	void print()
	{
//		EngineGetToolBox()->Log(LOGWARNING, _T("Polygon: \n"));
//		for (int i = 0; i < nPoints; i++)
//			EngineGetToolBox()->Log(LOGWARNING, _T("  p%d: (%f, %f)\n"), i, points[i].x, points[i].y);
	}

	vector<NVTPoint> points;
};

class CNodeVolumeTessellator
{
public:
	CNodeVolumeTessellator()
	{
		m_nPoints = 0;
		m_iConcaveCount = 0;
		earPoly = NULL;
	}
	void pushPoint( float x, float y )
	{
		m_Points.push_back( NVTPoint(x,y,m_nPoints) );
		m_PointsTemp.push_back( NVTPoint(x,y,m_nPoints) );

		m_nPoints = m_Points.size();
	}
	void tessellatePoints( vector<int> &faces )
	{
		if (m_Points.size() < 2)
			return;

		// close the polygon 'loop', the algorithm can be rewritten to not require this step
		NVTPoint t = m_Points[0];
		m_Points.push_back( t );
		m_PointsTemp.push_back( t );
		m_nPoints = m_Points.size();

		// create array of 3 point ear polygons
		earPoly = new NVTPolygon[m_Points.size() - 2];
		m_iEarCount = 0;
		doCutEar();

		// store the polies in the given faces vector
		int j = 0;
		for (int i=0; i < m_iEarCount; i++)
		{
			assert( earPoly[i].points.size() == 3 );
			j = earPoly[i].points[0].index;
			j = earPoly[i].points[1].index;
			j = earPoly[i].points[2].index;
			faces.push_back( earPoly[i].points[0].index );
			faces.push_back( earPoly[i].points[1].index );
			faces.push_back( earPoly[i].points[2].index );
		}

		delete[] earPoly;
		earPoly = NULL;
	}

private:
	vector<NVTPoint> m_Points;
	int m_nPoints;
	vector<NVTPoint> m_PointsTemp;
	int m_iConcaveCount;
	int m_iEarCount;
	NVTPolygon *earPoly;


	/// classifyPoints:  Classifies points as "convex" or "concave". Convex points are represented as a "1" in the
    ///                  ptType array; concave points are represented as a "-1" in the array.
	void classifyPoints();
    /// polygonClockwise:  Returns true if user inputted polygon in clockwise order, false if counterclockwise.
	///                    The Law of Cosines is used to determine the angle.
	bool polygonClockwise();
	/// convex:  returns true if point (x2, y2) is convex
    bool convex(float x1, float y1, float x2, float y2, float x3, float y3);
	/// area:  determines area of triangle formed by three points
    float area(float x1, float y1, float x2, float y2, float x3, float y3);
	/// triangleContainsPoints:  returns true if the triangle formed by three points contains another point
    bool triangleContainsPoint(float x1, float y1, float x2, float y2, float x3, float y3);
	/// ear:  returns true if the point (x2, y2) is an ear, false otherwise
    bool ear(float x1, float y1, float x2, float y2, float x3, float y3);
    /// cutEar:  creates triangle that represents ear for graphics purposes
    void cutEar(int index);
    /// updatePolygon:  creates new polygon without the ear that was cut
    void updatePolygon(int index);
	/// doCutEar:  Performs all the functions needed to find and cut an ear.
    void doCutEar();
};

#endif //#ifndef __NODVOLUMETESSELLATOR_H__