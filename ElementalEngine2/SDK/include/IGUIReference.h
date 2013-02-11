///============================================================================
/// \file		IGUIReference.h
/// \brief		Interface for reference element in gui system
/// \date		04/23/2007
/// \author		Nick Tourte
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

#ifndef _I_GUIREFERENCE_H_
#define _I_GUIREFERENCE_H_

namespace ElementalEngine
{

#define MAXVERTEXCOUNT 54 // 6 verts x 18 triangles

typedef struct GUIVERTEX
{
	float pos[3];
	float u,v;
	DWORD color;
}GUIVERTEX;

typedef struct GUIVERTEXINFO
{
	GUIVERTEX VertexData[MAXVERTEXCOUNT];			// vertex buffer data, to be loaded into manager buffer
	UINT iNumPrims;									// num prims for this texture
	IBaseTextureObject *pTexture;					// texture to render with this data
	IEffect *pEffect;								// effect to set with this data
	GUIVERTEXINFO()
	{
		iNumPrims = 0;
		pTexture = NULL;
		pEffect = NULL;
	}
}GUIVERTEXINFO;

typedef struct GUIFONTINFO
{
	DWORD dwPrimaryFontName;			// filename of primary font face
	DWORD dwSecondaryFontName;			// filename of secondary font face
	DWORD dwText;						// text to print
	UINT iFontSize;						// fontsize for both fonts
	bool bCentered;						// centered?
	int iXOffset;						// x offset from position
	int iYOffset;						// y offset from position
	RECT renderRect;					// render rect to set
	RECT clippingRect;					// clipping rect to set
	bool bUseClippingRect;				// bool to use clipping rect
	bool bRender;						// enable/disable font rendering
	bool bUseShadow;					// use drop shadow?
	DWORD iShadowColor;					// drop shadow color (argb)
	int iShadowXScale;					// additional pixel width of shadow
	int iShadowYScale;					// additional pixel height of shadow
	int iShadowXOffset;					// shadow x offset
	int iShadowYOffset;					// shadow y offset
	GUIFONTINFO()
	{
		dwPrimaryFontName = 0;
		dwSecondaryFontName = 0;
		dwText = 0;
		iFontSize = 0;
		bCentered = false;
		iXOffset = 0;
		iYOffset = 0;
		bRender = true;
		bUseClippingRect = false;
		bUseShadow = false;
		iShadowColor = 0xff000000;
		iShadowXOffset = 0;
		iShadowYOffset = 0;
		iShadowXScale = 0;
		iShadowYScale = 0;
	}
}GUIFONTINFO;

enum MOUSESTATE { MOUSE_OPEN=0, MOUSE_LEFTPRESSED, MOUSE_LEFTRELEASED };

class IGUIReference : public IObject
{
public:
	/// Virtual dtor
	virtual ~IGUIReference() {};

	/// Gets rendering information for a vertex buffer given the dimensions and state of the element
	/// Returns a vector since reference element can be a composite
	/// \param vertexData = data of verts, textures, and effects
	/// \param x, y = position of element
	/// \param width, height = size of element
	/// \param state = state of object
	/// \param angle = angle element is to be rendered at
	/// \param color = color of instance (assumed to be ARGB)		
	virtual void GetRenderParams(vector<GUIVERTEXINFO> &vertexData, float x, float y, float width, float height, DWORD state, float angle=0.0f, DWORD color=-1) = 0;

	/// Gets font information given the dimensions and state of the element
	/// Returns a vector to allow for composite elements (with more than one font)
	/// \param fontData = data of font stuffs, text, etc
	/// \param x, y = position of element
	/// \param width, height = size of element
	/// \param state = state of object
	/// \param angle = angle element is to be rendered at
	/// \param color = color of instance (assumed to be ARGB)		
	virtual void GetFontParams(vector<GUIFONTINFO> &fontData, float x, float y, float width, float height, DWORD state, float angle=0.0f, DWORD color=-1) = 0;

	/// Checks to see if the size provided is ok
	/// This func is used for sliders, combo boxes, and spinners, where there is a set width (or height)
	/// that the object shouldn't render beyond
	/// \param width = width "request", adjusted value returned
	/// \param height = height "request", adjusted value returned
	virtual void CheckSize(float *width, float *height) = 0;

	/// Checks the mouse position to see if we change state given dimensions of instance
	/// \param mouseX, mouseY = mouse position
	/// \param mouseState = state of mouse (mouse over, pressed, released)
	/// \param x, y, width, height = dimensions of instance
	/// \param dwLastState = last state of element
	/// \return state of element
	virtual DWORD CheckMouseState(float mouseX, float mouseY, MOUSESTATE mouseState, float x, float y, 
		float width, float height, DWORD dwLastState) = 0;

	/// Checks a key press given dimensions of instance
	/// \param size = sizeof(char)
	/// \param param = char of key pressed
	/// \return standard message return
	virtual DWORD CheckKeyPress(DWORD size, void *param) = 0;

	/// Adjusts state given a 'key up' command
	/// \param dwLastState = last state passed in
	/// \return new state
	virtual DWORD ToggleUp(DWORD dwLastState) = 0;

	/// Adjusts state given a 'key down' command
	/// \param dwLastState = last state passed in
	/// \return new state
	virtual DWORD ToggleDown(DWORD dwLastState) = 0;

	/// Adjusts state given a 'key select' command
	/// \param dwLastState = last state passed in
	/// \return new state
	virtual DWORD ToggleSelect(DWORD dwLastState) = 0;

	/// Adjusts state given a 'key back' command
	/// \param dwLastState = last state passed in
	/// \return new state
	virtual DWORD ToggleBack(DWORD dwLastState) = 0;

	/// Gets a hashval representing the current 'text' of the element, if applicable
	/// \param inState = current state of element
	/// \return hashstring hash val of string, or 0 if not applicable
	virtual DWORD GetHashText(DWORD inState) = 0;

	/// Sets a hashval representing the current 'text' of the element, if applicable
	/// \param inState = current state of element
	/// \param hashText = text to set
	/// \return new state
	virtual DWORD SetHashText(DWORD inState, DWORD hashText) = 0;

	/// Flags the ref to update its text if using a language entry
	virtual void FlagLanguageEntryChanged() = 0;
};

}		//namespace ElementalEngine

#endif // _I_GUIREFERENCE_H_