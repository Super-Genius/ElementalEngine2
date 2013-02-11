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

#include "stdafx.h"
#include "CColorRef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CColorRef::CColorRef()
{
	iColorVal=0;
}

CColorRef::CColorRef(unsigned int iColVal)
{
	iColorVal=iColVal;
}

CColorRef::CColorRef(unsigned char red,unsigned char green,unsigned char blue,unsigned char alpha)
{
	r=red;
	g=green;
	b=blue;
	a=alpha;
}
	
void CColorRef::Clear()
{
	iColorVal=0;
}

void CColorRef::Set(unsigned char red,unsigned char green,unsigned char blue,unsigned char alpha)
{
	r = red;
	g = green;
	b = blue;
	a = alpha;
}

void CColorRef::Set(const unsigned char *pColor)
{
	Set(pColor[0],pColor[1],pColor[2],pColor[4]);
}

void CColorRef::SetNoAlpha(CColorRef iColor)
{
	r=iColor.r;
	g=iColor.g;
	b=iColor.b;
}

CColorRef& CColorRef::operator = (const CColorRef& other)
{
	iColorVal=other.iColorVal;
	return *this;
}

CColorRef& CColorRef::operator = (unsigned int iColor)
{
	iColorVal=iColor; 
	return *this;
}

CColorRef& CColorRef::operator = (const unsigned char *pColor)
{
	Set(pColor[0],pColor[1],pColor[2],pColor[4]); 
	return *this;
}
  
CColorRef::operator unsigned int& ()
{
	return iColorVal;
}

CColorRef::operator unsigned char* ()
{
	return color;
}

bool CColorRef::operator == (const CColorRef& other) const
{
	return iColorVal==other.iColorVal;
}

bool CColorRef::operator == (unsigned int iColor) const
{
	return iColorVal==iColor;
}

bool CColorRef::operator != (const CColorRef& other) const
{
	return iColorVal!=other.iColorVal;
}

bool CColorRef::operator != (unsigned int iColor) const
{
	return iColorVal!=iColor;
}

unsigned char& CColorRef::operator [] (int iIndex)
{
	return color[iIndex];
}

void CColorRef::operator += (const CColorRef& other)
{
	int m;
	
	m = (int)r+(int)other.r; 
	if (m>255) m=255; 
	r=(unsigned char)m;
	
	m = (int)g+(int)other.g;
	if (m>255) m=255;
	g=(unsigned char)m;

	m = (int)b+(int)other.b; 
	if (m>255) m=255; 
	b=(unsigned char)m;
	
	m = (int)a+(int)other.a; 
	if (m>255) m=255; 
	a=(unsigned char)m;
}

void CColorRef::operator -= (const CColorRef& other)
{
	int m;

	m = (int)r-(int)other.r; 
	if (m<0) m=0; 
	r=(unsigned char)m;

	m = (int)g-(int)other.g;
	if (m<0) m=0;
	g=(unsigned char)m;

	m = (int)b-(int)other.b;
	if (m<0) m=0;
	b=(unsigned char)m;

	m = (int)a-(int)other.a;
	if (m<0) m=0;
	a=(unsigned char)m;
}

void CColorRef::operator *= (const CColorRef& other)
{
	int m;

	m = (int)r*(int)other.r; 
	r=(unsigned char)(m/255);

	m = (int)g*(int)other.g;
	g=(unsigned char)(m/255);

	m = (int)b*(int)other.b;
	b=(unsigned char)(m/255);

	m = (int)a*(int)other.a;
	a=(unsigned char)(m/255);
}

void CColorRef::operator *= (float factor)
{
	int m;

	if (factor<0.f) factor=0.f;

	m = (int)((float)r*factor);
	if (m>255) m=255;
	r=(unsigned char)m;

	m = (int)((float)g*factor);
	if (m>255) m=255;
	g=(unsigned char)m;

	m = (int)((float)b*factor);
	if (m>255) m=255;
	b=(unsigned char)m;

	m = (int)((float)a*factor);
	if (m>255) m=255;
	a=(unsigned char)m;
}

CColorRef CColorRef::operator+ (const CColorRef& other) const
{
	CColorRef col=*this;
	col+=other;
	return col;
}

CColorRef CColorRef::operator- (const CColorRef& other) const 
{
	CColorRef col=*this;
	col-=other;
	return col;
}

CColorRef CColorRef::operator* (const CColorRef& other) const 
{
	CColorRef col=*this;
	col*=other;
	return col;
}

CColorRef CColorRef::operator* (float factor) const
{
	CColorRef col=*this;
	col*=factor;
	return col;
}

bool CColorRef::IsZero() const
{
	return iColorVal==0;
}

float CColorRef::GetIntensity() const
{
	return ((float)r+(float)g+(float)b)*(1.f/3.f);
}

void CColorRef::SetRandomColor()
{
	Set(rand()%255,rand()%255,rand()%255,rand()%255);
}

void CColorRef::Lerp(const CColorRef &iCol1, const CColorRef &iCol2, float fFactor)
{
	if (fFactor<0.f)
		fFactor = 0.f; 
	else
		if (fFactor>1.f) fFactor=1.f;
	const float f2 = 1.f-fFactor;
	r = (unsigned char)((float)iCol1.r*f2+(float)iCol2.r*fFactor);
	g = (unsigned char)((float)iCol1.g*f2+(float)iCol2.g*fFactor);
	b = (unsigned char)((float)iCol1.b*f2+(float)iCol2.b*fFactor);
	a = (unsigned char)((float)iCol1.a*f2+(float)iCol2.a*fFactor);	
}
