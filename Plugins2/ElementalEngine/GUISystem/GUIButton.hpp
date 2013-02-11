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

#ifndef _GUIBUTTON_HPP_
#define _GUIBUTTON_HPP_

class GUIButton
{

public:

	/// \brief	constructor
	GUIButton(); 

	/// \brief	deconstructor
	~GUIButton();
	
	/// \brief	Deinitializes the object
	void DeInit();

	/// \brief	Offset the display's text position from the upper left corner
	/// \param	iOffSetX - the X offset
	/// \param	iOffSetY - the Y offset
	void SetTextPos(int iOffSetX, int iOffSetY);

	/// \brief	Centers the text in respect to the gui button
	/// \param	bCenterText - if true, center the text
	void CenterText(bool bCenterText) { m_bCenterText = bCenterText; }

	/// \brief	Set the text colors of the gui button
	/// \param	iColorNorm - the normal color
	/// \param	iColorHigh - the highlighted color
	/// \param	iColorSelect - the selected color
	/// \param	iColorGray	- the gray color
	void SetTextColors(int iColorNorm, int iColorHigh, int iColorSelect, int iColorGray);
	
	/// \brief	Set the texture screenmasks for the gui button
	/// \param	szNorm - the normal state texture
	/// \param	szHigh - the highlighted state texture
	/// \param	szSelect - the selected state texture
	/// \param	szGray	- the grayed state texture
	void SetScreenMasks(const TCHAR* szNorm, const TCHAR* szHigh, const TCHAR* szSelect, const TCHAR* szGray);
	
	/// \brief	Set the texture screenmasks for a decal to be placed on top of the button
	/// \param	szNorm - the normal state texture
	/// \param	szHigh - the highlighted state texture
	/// \param	szSelect - the selected state texture
	/// \param	szGray	- the grayed state texture
	/// \param offsetX, offsetY = offset position from upper left corner to display decal
	void SetDecalMasks(const TCHAR* szNorm, const TCHAR* szHigh, const TCHAR* szSelect, 
		const TCHAR* szGray, float offsetX, float offsetY);
	
	/// Sets the text properties of the button
	/// \param szFontname = font file to load for normal font
	/// \param szBFontname = font file to load for bold font
	/// \param szNormText = text to display for normal state
	/// \param szHighText = text to display for high state
	/// \param szSelectText = text to display for select state
	/// \param szGrayedText = text to display for grayed state
	/// \param fontsize = size of font
	void SetText(const TCHAR *szFontname, const TCHAR *szBFontname, const TCHAR *szNormText, 
		const TCHAR *szHighText, const TCHAR *szSelectText, const TCHAR *szGrayedText,
		int fontsize);
	
	/// \brief	Set SmartScale on or off
	/// \param	bSmartScale - if true, smart scale is on
	void SetSmartScale(bool bSmartScale) { m_bSmartScale = bSmartScale; }
	
	/// \brief	Show or hide the texture screen masks
	/// \param bShowTexture = t/f show the texture
	void ShowTexture(bool bShowTexture) { m_bShowTexture = bShowTexture; }
	
	/// Checks to see if a mouse button press/release was over the button
	/// \param fCursorPosX, fCursorPosY = mouse cursor position
	/// \param bButtonPressed = true if button was pressed
	/// \param bButtonReleased = true if button was released
	/// \param xPos, yPos = position of button
	/// \param width, height = size of button
	/// \param state = output state given inputs
	/// \return true if pressed/released over
	bool MouseButton(float fCursorPosX, float fCursorPosY, bool bButtonPressed, bool bButtonRelease, 
		float xPos, float yPos, float width, float height, DWORD *state);	

	/// \brief Mouseover the gui button
	/// \param fCursorPosX - the x position of the mouse cursor
	/// \param fCursorPosY - the y position of the mouse cursor
	/// \param xPos, yPos = position of button
	/// \param width, height = size of button	
	/// \param state = output state of button given inputs
	/// \return true if mouseover else false
	bool MouseOver(float fCursorPosX, float fCursorPosY, float xPos, float yPos, float width, 
		float height, DWORD *state);
	
	/// Updates the font parameters
	/// \param fontData = vector of fonts to update
	/// \param xPos, yPos = position of button
	/// \param width, height = size of button
	/// \param state = current state of instance
	void UpdateFontExt(vector<GUIFONTINFO> &fontData, float xPos, float yPos, float width, float height, DWORD state);

	/// Loads the vertex buffer to be rendered
	/// \param vertexData = vector of vertex triangles to fill
	/// \param x, y = position of button
	/// \param width, height = size of button
	/// \param state = current state of instance
	/// \param angle = rotation of button about center, in degrees
	/// \param color = color of button (multiplied)
	void LoadVertexBuffer(vector<GUIVERTEXINFO> &vertexData, float x, float y, float width, float height, DWORD state, 
												float angle, DWORD color);

	/// Enables font shadow
	/// \param bEnable = t/f enable shadow
	void EnableFontShadow(bool bEnable) { m_bUseFontShadow = bEnable; }

	/// Sets the font shadow properties
	/// \param iColor = color of shadow
	/// \param xScale, yScale = pixel width to scale shadow in addition to fontsize
	/// \param xOffset, yOffset = offset position of shadow
	void SetFontShadowVals(DWORD iColor, int xScale, int yScale, int xOffset, int yOffset)
	{
		m_iFontShadowColor = iColor;
		m_iFontShadowXOffset = xOffset;
		m_iFontShadowYOffset = yOffset;
		m_iFontShadowXScale = xScale;
		m_iFontShadowYScale = yScale;
	}
	

private:

	/// \brief	Set to normal state
	void EnableNorm();

	/// \brief Set to high state
	void EnableHigh();
	
	/// \brief Set to select state
	void EnableSelect();
	
	/// \brief Set to gray state
	void EnableGray();
	
	/// \brief	Create a CColorRef object
	/// \param	iColor - the integer value representing the color
	/// \return CColorRef object
	CColorRef CreateColorRef(int iColor);

	/// \brief	Change the display text
	/// \param szNewText = text to set to
	void ChangeText(const TCHAR *szNewText);	

	// base tex
	StdString m_szNorm;
	StdString m_szHigh;
	StdString m_szSelect;
	StdString m_szGray;

	// decal tex
	StdString m_szDecalNorm;
	StdString m_szDecalHigh;
	StdString m_szDecalSelect;
	StdString m_szDecalGray;

	const TCHAR* m_szFontName;
	const TCHAR* m_szBoldFontName;
	const TCHAR* m_szItalicFontName; // only using two fonts
	const TCHAR* m_szBoldItalicFontName; // only using two fonts
	int m_iFontsize;

	// texts
    StdString m_szText;
	StdString m_szTextHigh;
	StdString m_szTextSelect;
	StdString m_szTextGrayed;

	StdString m_szCurrentText; // currently displayed text;

	int m_iColor;
	int m_iColorHigh;
	int m_iColorSelect;
	int m_iColorGray;
	CColorRef m_iTextcol;

	bool m_bAutoSize;
	bool m_bCenterText;
	bool m_bShowTexture;

	IRenderer * m_Renderer;

	CLayeredMask *m_CurrentBaseMask;
	CLayeredMask *m_CurrentDecalMask;

	DWORD m_Color;

	CLayeredMask *m_pNormBase;
	CLayeredMask *m_pHighBase;
	CLayeredMask *m_pSelectBase;
	CLayeredMask *m_pGrayBase;

	CLayeredMask *m_pNormDecal;
	CLayeredMask *m_pHighDecal;
	CLayeredMask *m_pSelectDecal;
	CLayeredMask *m_pGrayDecal;

	bool m_bSmartScale;

	// display text position offsetted from gui button's pos
	int m_iTextOffSetX;
	int m_iTextOffSetY;

	float m_fDecalOffsetX;
	float m_fDecalOffsetY;
	
	IEffect *m_pEffect;

	bool m_bUseFontShadow;
	DWORD m_iFontShadowColor;
	int m_iFontShadowXScale;
	int m_iFontShadowYScale;
	int m_iFontShadowXOffset;
	int m_iFontShadowYOffset;
};
	
#endif