///============================================================================
/// \file		NodeVolumeTessellator.cpp
/// \brief		Implmentation of NodeVolumeTessellator Helper Class
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

#include "stdafx.h"
#include "NodeVolumeTessellator.h"

///////////////////////////////////////////////////////////////
bool CNodeVolumeTessellator::polygonClockwise()
{
	float aa, bb, cc, b, c, theta;
	float convex_turn;
	float convex_sum = 0;

	for (int i = 0; i < m_nPoints - 2; i++) {
		aa = ((m_Points[i+2].x - m_Points[i].x) * (m_Points[i+2].x - m_Points[i].x)) +
			((-m_Points[i+2].y + m_Points[i].y) * (-m_Points[i+2].y + m_Points[i].y));
		
		bb = ((m_Points[i+1].x - m_Points[i].x) * (m_Points[i+1].x - m_Points[i].x)) +
			((-m_Points[i+1].y + m_Points[i].y) * (-m_Points[i+1].y + m_Points[i].y));
		
		cc = ((m_Points[i+2].x - m_Points[i+1].x) * 
		(m_Points[i+2].x - m_Points[i+1].x)) +
		((-m_Points[i+2].y + m_Points[i+1].y) *
		(-m_Points[i+2].y + m_Points[i+1].y));
	
		b = sqrt(bb);
		c = sqrt(cc);
		theta = acos((bb + cc - aa) / (2 * b * c));

		if (convex(m_Points[i].x, m_Points[i].y,
					m_Points[i+1].x, m_Points[i+1].y,
					m_Points[i+2].x, m_Points[i+2].y)) {
		convex_turn = PI - theta;
		convex_sum += convex_turn;
		}
		else {
		convex_sum -= PI - theta;
		}
	}
	aa = ((m_Points[1].x - m_Points[m_nPoints-2].x) * 
		(m_Points[1].x - m_Points[m_nPoints-2].x)) +
		((-m_Points[1].y + m_Points[m_nPoints-2].y) * 
		(-m_Points[1].y + m_Points[m_nPoints-2].y));
		
	bb = ((m_Points[0].x - m_Points[m_nPoints-2].x) * 
		(m_Points[0].x - m_Points[m_nPoints-2].x)) +
		((-m_Points[0].y + m_Points[m_nPoints-2].y) * 
		(-m_Points[0].y + m_Points[m_nPoints-2].y));
		
	cc = ((m_Points[1].x - m_Points[0].x) * (m_Points[1].x - m_Points[0].x)) +
		((-m_Points[1].y + m_Points[0].y) * (-m_Points[1].y + m_Points[0].y));
	
	b = sqrt(bb);
	c = sqrt(cc);
	theta = acos((bb + cc - aa) / (2 * b * c));
	
	if (convex(m_Points[m_nPoints-2].x, m_Points[m_nPoints-2].y,
				m_Points[0].x, m_Points[0].y,
				m_Points[1].x, m_Points[1].y)) {
		convex_turn = PI - theta;
		convex_sum += convex_turn;
	}
	else { 
		convex_sum -= PI - theta;
	}

	return (convex_sum >= ((2.0f * PI) - 0.00001f));
}

///////////////////////////////////////////////////////////////
void CNodeVolumeTessellator::classifyPoints()
{
	m_iConcaveCount = 0;

// Before cutting any ears, we must determine if the polygon was
//			* inputted in clockwise order or not, since the algorithm for
//			* cutting ears assumes that the polygon's points are in clockwise
//			* order.  If the points are in counterclockwise order, they are
//			* simply reversed in the array.
//
	if (m_iEarCount == 0) {
		if (!polygonClockwise()) {
			for (int i = 0; i < m_nPoints; i++) {
			m_PointsTemp[i].x = m_Points[m_nPoints-1 - i].x;
			m_PointsTemp[i].y = m_Points[m_nPoints-1 - i].y;
			m_PointsTemp[i].index = m_Points[m_nPoints-1 - i].index;
			}
			for (int i = 0; i < m_nPoints - 1; i++) {
			m_Points[i].x = m_PointsTemp[i].x;
			m_Points[i].y = m_PointsTemp[i].y;
			m_Points[i].index = m_PointsTemp[i].index;
		}
		}
	}

		for (int i = 0; i < m_nPoints - 1; i++) {
		if (i == 0) {
		if (convex(m_Points[m_nPoints-2].x, m_Points[m_nPoints-2].y,
							m_Points[i].x, m_Points[i].y,
							m_Points[i+1].x, m_Points[i+1].y)) {
			m_Points[i].type = 1;	// point is convex 
		}
		else {
			m_Points[i].type = -1;	// point is concave
			m_iConcaveCount++;
			}
		}
		else {    // i > 0 
		if (convex(m_Points[i-1].x, m_Points[i-1].y,
							m_Points[i].x, m_Points[i].y,
							m_Points[i+1].x, m_Points[i+1].y)) {
			m_Points[i].type = 1;	// point is convex 
				}
		else {
			m_Points[i].type = -1;	// point is concave 		    
			m_iConcaveCount++;
		}
		}
	}
}


// convex:  returns true if point (x2, y2) is convex

bool CNodeVolumeTessellator::convex(float x1, float y1, float x2, float y2, float x3, float y3)
{
	return (area(x1, y1, x2, y2, x3, y3) < 0);
}


// area:  determines area of triangle formed by three points
    
float CNodeVolumeTessellator::area(float x1, float y1, float x2, float y2, float x3, float y3)
{
	float areaSum = 0;

	areaSum += x1 * (y3 - y2);
	areaSum += x2 * (y1 - y3);
	areaSum += x3 * (y2 - y1);

	// for actual area, we need to multiple areaSum * 0.5, but we are only interested in the sign of the area (+/-)
	return -areaSum;
}


// triangleContainsPoints:  returns true if the triangle formed by three points contains another point   
bool CNodeVolumeTessellator::triangleContainsPoint(float x1, float y1, float x2, float y2, float x3, float y3)
{
	int i = 0;
	float area1, area2, area3;
	bool noPointInTriangle = true;

	while ((i < m_nPoints - 1) && (noPointInTriangle)) {
		if ((m_Points[i].type == -1) && // point is concave 
				(((m_Points[i].x != x1) && (m_Points[i].y != y1)) ||
			((m_Points[i].x != x2) && (m_Points[i].y != y2)) ||
			((m_Points[i].x != x3) && (m_Points[i].y != y3)))) 
		{
			area1 = area(x1, y1, x2, y2, m_Points[i].x, m_Points[i].y);
			area2 = area(x2, y2, x3, y3, m_Points[i].x, m_Points[i].y);
			area3 = area(x3, y3, x1, y1, m_Points[i].x, m_Points[i].y);

			if (area1 > 0)
				if ((area2 > 0) && (area3 > 0))
				noPointInTriangle = false;
			if (area1 < 0)
				if ((area2 < 0) && (area3 < 0))
				noPointInTriangle = false;
		}
		i++;
	}
	return !noPointInTriangle;
}


// ear:  returns true if the point (x2, y2) is an ear, false
    
bool CNodeVolumeTessellator::ear(float x1, float y1, float x2, float y2, float x3, float y3)
{
	if (m_iConcaveCount != 0)
		if (triangleContainsPoint(x1, y1, x2, y2, x3, y3))
		return false;
		else 
		return true;
	else
		return true;
}


// cutEar:  creates triangle that represents ear for graphics purposes
    
void CNodeVolumeTessellator::cutEar(int index)
{
	float ex[4], ey[4];
	int ei[4];

	if (index == 0) {
		ei[0] = m_Points[m_nPoints-2].index;
		ex[0] = m_Points[m_nPoints-2].x;
		ey[0] = m_Points[m_nPoints-2].y;
		ei[1] = m_Points[index].index;
		ex[1] = m_Points[index].x;
		ey[1] = m_Points[index].y;
		ei[2] = m_Points[index+1].index;
		ex[2] = m_Points[index+1].x;
		ey[2] = m_Points[index+1].y;
	}
	else if ((index > 0) && (index < m_nPoints-2)) {
		ei[0] = m_Points[index-1].index;
		ex[0] = m_Points[index-1].x;
		ey[0] = m_Points[index-1].y;
		ei[1] = m_Points[index].index;
		ex[1] = m_Points[index].x;
		ey[1] = m_Points[index].y;
		ei[2] = m_Points[index+1].index;
		ex[2] = m_Points[index+1].x;
		ey[2] = m_Points[index+1].y;
	}
	else if (index == m_nPoints-2) {
		ei[0] = m_Points[index-1].index;
		ex[0] = m_Points[index-1].x;
		ey[0] = m_Points[index-1].y;
		ei[1] = m_Points[index].index;
		ex[1] = m_Points[index].x;
		ey[1] = m_Points[index].y;
		ei[2] = m_Points[0].index;
		ex[2] = m_Points[0].x;
		ey[2] = m_Points[0].y;
	}
	for (int i = 0; i < 3; ++i)
	{
		earPoly[m_iEarCount].addPoint( ex[i], ey[i], ei[i] );
	}
	m_iEarCount++;
}


// updatePolygon:  creates new polygon without the ear that was cut
    
void CNodeVolumeTessellator::updatePolygon(int index)
{
	int new_i = 0;
	int i = 0;

	if (index == 0)
		i++;
	while (i < m_nPoints - 1) {
		if (i == index)
		i++;
		if (i < m_nPoints - 1) {
		m_Points[new_i].x = m_Points[i].x;
		m_Points[new_i].y = m_Points[i].y;
		m_Points[new_i].index = m_Points[i].index;
		new_i++;
		i++;
		}
	}
	m_Points[m_nPoints-2].x = m_Points[0].x;
	m_Points[m_nPoints-2].y = m_Points[0].y;
	m_Points[m_nPoints-2].index = m_Points[0].index;
	m_nPoints--;
}


// doCutEar:  Performs all the functions needed to find and cut an ear.
    
void CNodeVolumeTessellator::doCutEar()
{
	bool earHasBeenCut;

	do
	{
		earHasBeenCut = false;
		classifyPoints();
		int i = 0;
		while ((i < m_nPoints - 1) && !earHasBeenCut) {
			if (m_Points[i].type == 1) {  // point is convex 
			if (i == 0) {
				if (ear(m_Points[m_nPoints-2].x, m_Points[m_nPoints-2].y,
					m_Points[i].x, m_Points[i].y,
								m_Points[i+1].x, m_Points[i+1].y)) {
				cutEar(i);
				updatePolygon(i);
				earHasBeenCut = true;
				}
			}
			else {   // i > 0 
				if (ear(m_Points[i-1].x, m_Points[i-1].y,
					m_Points[i].x, m_Points[i].y,
								m_Points[i+1].x, m_Points[i+1].y)) {
				cutEar(i);
				updatePolygon(i);
				earHasBeenCut = true;
				}
			}
			}
			i++;
		}
	} while( earHasBeenCut );
}
