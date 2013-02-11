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

#ifndef _C_COLORREF_H_
#define _C_COLORREF_H_

#include "stdafx.h"

#define RGBA(r,g,b,a)  (unsigned int) ((unsigned int)((unsigned char)r) + (((unsigned int)((unsigned char)g)) << 8) + (((unsigned int)((unsigned char)b))<<16) + (((unsigned int)((unsigned char)a))<<24))

class CColorRef
{
public:
	CColorRef();
	CColorRef(unsigned int iColVal);
	CColorRef(unsigned char red,unsigned char green,unsigned char blue,unsigned char alpha=255);

	void Clear();
	void Set(unsigned char red,unsigned char green,unsigned char blue,unsigned char alpha=255);
	void Set(const unsigned char *pColor);
	void SetNoAlpha(CColorRef iColor);
	CColorRef& operator = (const CColorRef& other);
	CColorRef& operator = (unsigned int iColor);
	CColorRef& operator = (const unsigned char *pColor);
	operator unsigned int& ();
	operator unsigned char* ();
	bool operator == (const CColorRef& other) const;
	bool operator == (unsigned int iColor) const;
	bool operator != (const CColorRef& other) const;
	bool operator != (unsigned int iColor) const;
	unsigned char& operator [] (int iIndex);
	void operator += (const CColorRef& other);
	void operator -= (const CColorRef& other);
	void operator *= (const CColorRef& other);
	void operator *= (float factor);
	CColorRef operator+ (const CColorRef& other) const;
	CColorRef operator- (const CColorRef& other) const;
	CColorRef operator* (const CColorRef& other) const;
	CColorRef operator* (float factor) const;
	bool IsZero() const;
	float GetIntensity() const;
	void SetRandomColor();
	void Lerp(const CColorRef &iCol1, const CColorRef &iCol2, float fFactor);
	TCHAR* ToString()
	{ 
		_stprintf(m_StringFormat, "%d,%d,%d,%d", r, g, b, a);
		return m_StringFormat;
	}

	union {
		struct { unsigned char r,g,b,a; };
		struct { unsigned char color[4]; };
		struct { unsigned int iColorVal; };
	};

private:
	TCHAR m_StringFormat[20];
};

const CColorRef  RGBA_WHITE  = RGBA(255,255,255,255);
const CColorRef  RGBA_GREY   = RGBA(127,127,127,255);
const CColorRef  RGBA_BLACK  = RGBA(  0,  0,  0,255);

const CColorRef  RGBA_RED    = RGBA(255,  0,  0,255);
const CColorRef  RGBA_YELLOW = RGBA(255,255,  0,255);
const CColorRef  RGBA_GREEN  = RGBA(  0,255,  0,255);
const CColorRef  RGBA_CYAN   = RGBA(  0,255,255,255);
const CColorRef  RGBA_BLUE   = RGBA(  0,  0,255,255);
const CColorRef  RGBA_PURPLE = RGBA(255,  0,255,255);


#endif