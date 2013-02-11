///============================================================================
/// \file	IFont.h
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

#ifndef IFONT_H
#define IFONT_H

namespace ElementalEngine
{
class IFontObject 
{
public:
	/// Virtual Destructor
	virtual ~IFontObject() {}
	
	/// Sets the Normal Font with a given pixel height
	/// \param filename - pointer to tchar of the name
	/// \param height - int of the font height
	/// \return - true on success
	virtual bool SetFont( const TCHAR* filename, int height ) = 0;
	
	/// Sets the Bold Font with a given pixel height
	/// \param filename - pointer to tchar of the name
	/// \param height - int of the font height
	/// \return - true on success
	virtual bool SetFontBold( const TCHAR* filename, int height ) = 0;

	/// Sets the Italic Font with a given pixel height
	/// \param filename - pointer to tchar of the name
	/// \param height - int of the font height
	/// \return - true on success
	virtual bool SetFontItalic( const TCHAR* filename, int height ) = 0;

	/// Sets the Bold-Italic combo Font with a given pixel height
	/// \param filename - pointer to tchar of the name
	/// \param height - int of the font height
	/// \return - true on success
	virtual bool SetFontBoldItalic( const TCHAR* filename, int height ) = 0;

	/// Sets the pixel height of the font(rebuilds textures if no other font object is using this height
	/// \param height - int of the height
	virtual void SetFontPixHeight( int height ) = 0;

	/// Gets the current font pixel height
	/// \return - int of the height
	virtual int GetFontPixHeight( void ) = 0;	

	/// Gets the width (in pixels) of a string in this font
	/// \param str - pointer to tchar of a string
	/// \return - int indicating the number of pixels
	virtual int GetFontPixWidth( const TCHAR* str ) = 0;
	
	/// Sets the render rectangle of the font, must be done before using SetText
	/// \param left - left coordinate
	/// \param top - top coordinate
	/// \param right - right coordinate
	/// \param bottom - bottom coordinate
	virtual void SetRenderRect( int left, int top, int right, int bottom ) = 0;

	/// Sets the cliping rectangle of the font, must be done before using SetText
	/// \param left - left coordinate
	/// \param top - top coordinate
	/// \param right - right coordinate
	/// \param bottom - bottom coordinate
	virtual void SetClippingRect( int left, int top, int right, int bottom ) = 0;

	/// Sets the offset
	/// \param x - float x coordinate offset
	/// \param y - float y coordinate offset
	virtual void SetOffset( float x, float y ) = 0;

	/// Sets the current text of this font object, must not be done before rendering
	/// \param str - tchar pointer to string of text
	/// \param delimators - array of char delimators
	/// \param numtokendelim - int of number of tokens to be seperated (default = 1)
	virtual void SetText( const TCHAR* str, char delimators[] = NULL, int numtokendelim  = 1 ) = 0;
	
	/// Renders the font
	virtual void Render( void )=0;

	/// Releases the font, don't have to delete yourself, the Interface is automatically invalidated
	/// just make sure you set your pointer to NULL afterward.
	virtual void Release( void )=0;

	/// make the fontobject visible or invisible
	/// \param visible - bool to set visibility (true = visible)
	virtual void SetVisible( bool visible ) =0;

	/// Space between lines in a paragraph
	/// \param scale - float space between lines
	virtual void SetLeadingScale( float scale ) = 0; 

	/// Kerning is space between two characters
	/// \param scale - float space between characters
	virtual void SetKerningScale( float scale ) = 0;
	
	/// Either can use kerning if kerning is there or 
	/// can use character based advances. Default on if kerning is present
	/// \param on - bool to turn on kerning (true = kerning on)
	virtual void UseKerning( bool on ) = 0;
	
	/// Sets the color of the font
	/// \param r = red amount
	/// \param g = blue amount
	/// \param b = green amount
	/// \param a = alpha amount
	virtual void SetColor( float r, float g, float b, float a ) = 0;

	/// Gets the number of pixels based on actual visibile characters
	/// \param txt - tchar pointer to the string of text
	/// \param delimators - char array of delimators
	/// \param numtokendelim - int number of tokens to delimit in the string
	/// \param width - float value of the width
	/// \return - int number of characters
	virtual int GetCharNum( const TCHAR* txt, char delimators[], int numtokendelim, float width ) = 0;

	/// Gets the number of lines
	/// \return - int of the number of lines
	virtual int GetNumLines( void ) = 0;

	/// Sets width factor for odd window aspect ratio adjustment
	/// \param value - value to set ratio to
	virtual void SetWidthRatio( float value ) = 0;

	/// Sets height factor for odd window aspect ratio adjustment
	/// \param value - value to set ratio to
	virtual void SetHeightRatio( float value ) = 0;

	/// Sets zoom factor for zooming in (used in editor)
	/// \param value - value to set zoom to
	virtual void SetZoomFactor( float value ) = 0;

	/// Sets page offset for rendering away from 0,0 (used in editor)
	/// \param xOff - x offset
	/// \param yOff - y offset
	virtual void SetPageOffset( int xOff, int yOff ) = 0;
	
	/// Update buffer is called by the Font manager. The manager will lock a 
	/// vertex buffer for you, pass it on with the current valid offset. Your 
	/// job is to fill pDest with data, and return the amount written in bytes
	/// \param pDest - Address you write your vertex data to
	/// \param offset - the current offset of the vertex buffer
	/// \param vb - the vertex buffer in question
	/// \return int - the amount of bytes written
	virtual int UpdateBuffer( char** pDest, int offset, IVertexBufferObject* vb ) = 0;

	/// Update index buffer is called by the Font manager. The manager will lock a 
	/// index buffer for you, pass it on with the current valid offset. Your 
	/// job is to fill pDest with data, and return the amount written in bytes
	/// \param pDest - Address you write your vertex data to
	/// \param offset - the current offset of the vertex buffer
	/// \param vb - the IIndexBuffer in question
	/// \return int - the amount of bytes written	
	virtual int UpdateIndexBuffer( char** pDest, int offset,IIndexBuffer* ib ) = 0;

	/// update is when you update the Font internal buffers
	virtual bool Update( void ) = 0;

	/// Sets whether or not we want the text to wrap (cuts down on time)
	/// \param value = bool t/f to wrap or not
	virtual void SetWrapping( bool value ) = 0;

	/// Sets the shadow values for the drop shadow
	/// \param color = color of shadow (argb)
	/// \param xScale, yScale = additional pixel width in x and y axes
	/// \param xOffset, yOffset = offset from text pos
	virtual void SetShadowVals( DWORD color, int xScale, int yScale, int xOffset, int yOffset )=0;

	/// Enables / disables the drop shadow
	/// \param bEnable = enable / disable
	virtual void EnableDropShadow( bool bEnable )=0;
	
};

/// IFontImplementation
class IFontImplementation
{
public:
	/// Destructor
	virtual ~IFontImplementation() {};

	/// Creates a font object and returns it
	/// \return - pointer to IFontObject interface
	virtual IFontObject* CreateFont( void ) = 0;

	/// Destroys a given font object
	/// \param pFontObject - pointer to IFontObject interface
	virtual void DestroyFont( IFontObject * pFontObject ) = 0;

	/// Gets the number of pixels based on actual visibile characters
	/// \param fontFace - pointer to IHashString of the name of the font
	/// \param fontsize - unsigned int of the size
	/// \param text - pointer to TCHAR of the text to measure
	/// \returns - int of number of pixels
	virtual int GetFontPixWidth(IHashString *fontFace, UINT fontsize, const TCHAR* text) = 0;
};

//example usage:
//m_FontObject->SetFont( "Fonts/Off.TTF", 40 );
//m_FontObject->SetRect( 10,600,300,600 );
//m_FontObject->SetText( "Hello My Name is Font Man" );
//m_FontObject->SetVisible( true );
//m_FontObject->SetKerningScale( 1.5f );
//m_FontObject->SetLeadingScale( 2.5f );
}
#endif