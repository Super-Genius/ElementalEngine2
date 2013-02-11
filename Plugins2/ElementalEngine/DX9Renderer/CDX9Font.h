///============================================================================
/// \file	CDX9Font.h
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

#ifndef EE_CDX9FONT_H
#define EE_CDX9FONT_H
#include "ITextureObject.h"
#include "IFont.h"
#include "CDX9VertexBufferObject.h"
#include "CDX9IndexBuffer.h"
#include "ft2build.h"
#include FT_FREETYPE_H
#include <string>
#include <vector>

#define MAX_FONT_CHAR 128
#define MAX_CHAR_COUNT 4096


using namespace std;
struct fRect
{
	float x1;
	float y1;
	float x2;
	float y2;

	void operator*=(Matrix4x4 &mat)
	{
		// TODO: is there a better way to do this?
		Vec3 topLeft(x1, y1, 0.0f);
		Vec3 bottomRight(x2, y2, 0.0f);
		Matrix3x3 tempMat;
		tempMat.SetFrom4x4(mat.GetMatrix());
		topLeft *= tempMat; // PS why oh why don't we have Vec3 * and *= Matrix4x4???
		bottomRight *= tempMat;
		x1 = topLeft.x;
		y1 = topLeft.y;
		x2 = bottomRight.x;
		y2 = bottomRight.y;
	}
};

struct CDX9FontFace
{
	FT_Face m_FTFace;
	//texture
	ITextureObject * m_Texture;
	FT_Glyph_Metrics	m_Metrics[ MAX_FONT_CHAR ];
	short				m_Kerning[ MAX_FONT_CHAR ][ MAX_FONT_CHAR ];
	bool				m_HasKerning;
	fRect				m_CharCoords[ MAX_FONT_CHAR ];//texcoords
	int m_RefCount;
};

struct FONT_VERTEX
{
	float pos[3];
	float u,v;
	DWORD color;
};

typedef map< DWORD, CDX9FontFace > FACEMAP;
typedef map<DWORD, IBaseTextureObject*> IMAGEMAP;

enum FONTTYPE
{
	NORMAL = 0,
	BOLD = 1,
	ITALIC = 2,
	BOLDITALIC = 3,
	IMAGE = 4
};

struct taggableChar
{
	TCHAR data;
	DWORD color;
	bool bold;
	bool italic;
	bool underline;
	bool strikethru;
	DWORD imageID;
	taggableChar():data(0), color(0), bold(false), italic(false), 
		underline(false), strikethru(false), imageID(0) {}
};

typedef vector< taggableChar > tagString;

struct charColor
{
	UINT startA;
	UINT startR;
	UINT startG;
	UINT startB;
	UINT endA;
	UINT endR;
	UINT endG;
	UINT endB;
	unsigned int startIndex;
	unsigned int endIndex;
	int rFactor;
	int gFactor;
	int bFactor;
	int aFactor;
};

class EE_CDX9Font : public IFontObject
{
public:	
	EE_CDX9Font();
	virtual bool SetFont( const TCHAR * filename, int height );
	virtual bool SetFontBold( const TCHAR * filename, int height );
	virtual bool SetFontItalic( const TCHAR * filename, int height );
	virtual bool SetFontBoldItalic( const TCHAR * filename, int height );
	virtual void SetFontPixHeight( int height );
	virtual int GetFontPixWidth( const TCHAR * str );
	virtual void SetText( const TCHAR * str , char delimators[], int numtokendelim);
	virtual void SetRenderRect( int left, int top, int right, int bottom )
	{
		int oldwidth = (int)(m_RRect.x2 - m_RRect.x1);
		int newwidth = right - left;
		//change in rectangle
		m_RRect.x1 = (float)left;
		m_RRect.y1 = (float)top;
		m_RRect.x2 = (float)right;
		m_RRect.y2 = (float)bottom;
		if (oldwidth != newwidth)
		{
			SetText( (char*)m_TextWithTags.c_str(), (char*)m_delimators.c_str(), m_numdelimators );            
		}
	}
	virtual void SetClippingRect( int left, int top, int right, int bottom )
	{
		m_CRect.x1 = (float)left;
		m_CRect.y1 = (float)top;
		m_CRect.x2 = (float)right;
		m_CRect.y2 = (float)bottom;
	}
	virtual void SetOffset(float x, float y)
	{
		xOff = floor(x);
		yOff = floor(y);
	}
	virtual int GetFontPixHeight()
	{
		if( m_Faces[NORMAL] )
		{
			return m_PixHeight;
		}
		return 0;
	}
	virtual void Release()
	{
		m_Delete = true;
	}
	virtual void Render();
	virtual void SetVisible( bool visible )
	{
		m_bVisible = visible;
	}
	/// Space between lines in a paragraph
	virtual void UseKerning( bool on )
	{
		m_bUseKerning = on;
	}
	virtual void SetLeadingScale( float scale )
	{
		m_LeadScale = scale;
	} 
	/// Kerning is space between two characters
	virtual void SetKerningScale( float scale )
	{
		m_KernScale = scale;
	}
	virtual void SetColor( float r, float g, float b, float a )
	{
		m_Color = D3DCOLOR_ARGB( (int)(a*255.f),
								(int)(r*255.f),
								(int)(g*255.f),
								(int)(b*255.f) );
	}
	/// returns offset
	int UpdateBuffer( char ** pDest, int offset,IVertexBufferObject * vb );
	int UpdateIndexBuffer( char ** pDest, int offset,IIndexBuffer * ib );
	bool Update();
	~EE_CDX9Font();

	virtual int GetCharNum(const TCHAR * txt, char delimators[], int numtokendelim, float width);
	virtual int GetNumLines();
	/// Sets width factor for odd window aspect ratio adjustment
	/// \param value - value to set ratio to
	virtual void SetWidthRatio(float value)
	{
		m_fWidthRatio = value;
	}
	/// Sets height factor for odd window aspect ratio adjustment
	/// \param value - value to set ratio to
	virtual void SetHeightRatio(float value)
	{
		m_fHeightRatio = value;
	}
	/// Sets zoom factor for zooming in (used in editor)
	/// \param value - value to set zoom to
	virtual void SetZoomFactor(float value)
	{
		m_fZoomFactor = value;
	}
	/// Sets page offset for rendering away from 0,0 (used in editor)
	/// \param xOff - x offset
	/// \param yOff - y offset
	virtual void SetPageOffset(int xOff, int yOff)
	{
		m_iPageOffsetX = xOff;
		m_iPageOffsetY = yOff;
	}

	static int GetFontPixWidth( CHashString fontFace, UINT fontsize, const TCHAR * str );

//	DWORD OnRenderFontAsVector( DWORD size, void *in_params );

	/// Sets whether or not we want the text to wrap (cuts down on time)
	/// \param value = bool t/f to wrap or not
	virtual void SetWrapping(bool value) { m_bWrap = value; }

	/// Sets the shadow values for the drop shadow
	/// \param color = color of shadow (argb)
	/// \param xScale, yScale = additional pixel width in x and y axes
	/// \param xOffset, yOffset = offset from text pos
	virtual void SetShadowVals(DWORD color, int xScale, int yScale, int xOffset, int yOffset)
	{
		m_iShadowColor = color;
		m_iShadowXScale = xScale;
		m_iShadowYScale = yScale;
		m_iShadowXOffset = xOffset;
		m_iShadowYOffset = yOffset;
	}
	/// Enables / disables the drop shadow
	/// \param bEnable = enable / disable
	virtual void EnableDropShadow(bool bEnable) { m_bUseShadow = bEnable; }

private:
	bool SetFontFace(FONTTYPE type, const TCHAR* filename, int height);
	void UpdateBuffers();

	bool			m_Delete;
	bool			m_bVisible;
	bool			m_bUseKerning;
	fRect			m_RRect;
	fRect			m_CRect;
	string			m_Text;
	string			m_TextWithTags;
	string			m_delimators;
	int				m_numdelimators;
	CDX9FontFace *	m_Face;
	CDX9FontFace *	m_Faces[4];
	CHashString		m_hsNormFontFace;
	CHashString		m_hsBoldFontFace;
	CHashString		m_hsItalicFontFace;
	CHashString		m_hsBoldItalicFontFace;
	int				m_PixHeight;
	int				m_IndicesWritten;
	int				m_VerticesWritten;
	int				m_VBOffset;
	int				m_NormIBOffset;
	int				m_BoldIBOffset;
	int				m_ItalicIBOffset;
	int				m_BoldItalicIBOffset;
	int				m_ImageIBOffset;
	float			xOff, yOff;
	IRenderer *				m_IRenderer;
	IVertexBufferObject *	m_VB;
	IIndexBuffer *		m_NormalIB;
	IIndexBuffer *		m_BoldIB;
	IIndexBuffer *		m_ItalicIB;
	IIndexBuffer *		m_BoldItalicIB;
	IIndexBuffer *		m_ImageIB;
	float				m_LeadScale;
	float				m_KernScale;
	DWORD				m_Color;

	UINT m_NormVertexCount;
	UINT m_BoldVertexCount;
	UINT m_ItalicVertexCount;
	UINT m_BoldItalicVertexCount;
	UINT m_ImageVertexCount;
	UINT m_TotalVerticesWritten;
	
	
    vector< tagString > m_Lines;

	FONT_VERTEX		m_VBBuffer[ MAX_CHAR_COUNT*6 ];//6 verts per face
	short			m_IBBuffer[5][ MAX_CHAR_COUNT*6 ]; 	

	float m_fWidthRatio;
	float m_fHeightRatio;
	float m_fZoomFactor;
	int m_iPageOffsetX;
	int m_iPageOffsetY;

	bool m_bWrap;

	vector<IBaseTextureObject*> m_ImageVec;

	bool m_bUseShadow;
	DWORD m_iShadowColor;
	int m_iShadowXScale;
	int m_iShadowYScale;
	int m_iShadowXOffset;
	int m_iShadowYOffset;

public:
	static FT_Library  m_FontLibrary;
	static FACEMAP	m_NormFaceMap;
	static FACEMAP	m_BoldFaceMap;
	static FACEMAP	m_ItalicFaceMap;
	static FACEMAP	m_BoldItalicFaceMap;
	static IMAGEMAP m_ImageMap;
};

#endif