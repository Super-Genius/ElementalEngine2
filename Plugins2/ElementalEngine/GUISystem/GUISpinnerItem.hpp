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

#ifndef _GUISPINNERITEM_HPP_
#define _GUISPINNERITEM_HPP_

class GUIButton;

enum eNumType
{
	SPINNER_INT = 0,
	SPINNER_FLOAT
};

class GUISpinnerItem
{

public:

	/// \brief	constructor
	GUISpinnerItem(); 

	/// \brief	deconstructor
	~GUISpinnerItem();
	
	/// \brief	Deinitializes the object
	void DeInit();
	
	///	\brief	Set the font
	///	\param	*szFontName - the name of the font
	/// \param fontsize = size of font
	void SetFont(const TCHAR *szFontName, int fontsize);

	/// Set the edit text box properties
	/// \param szNorm = texture for normal state
	/// \param szGray = texture for gray state
	/// \param iCursorColor = color of blinking cursor (unused)
	void SetEditBox(const TCHAR* szNorm, const TCHAR* szGray, int iCursorColor);

	/// Set the up arrow box properties
	/// \param szNorm = texture for normal state
	/// \param szSel = texture for selected state
	/// \param szGray = texture for gray state
	void SetUpArrowBox(const TCHAR *szNorm, const TCHAR *szSel, const TCHAR *szGray);

	/// Set the down arrow box properties
	/// \param szNorm = texture for normal state
	/// \param szSel = texture for selected state
	/// \param szGray = texture for gray state
	void SetDownArrowBox(const TCHAR *szNorm, const TCHAR *szSel, const TCHAR *szGray);

	/// \brief	Offset the display's text position from the upper left corner
	/// \param	iOffSetX - the X offset
	/// \param	iOffSetY - the Y offset
	void SetTextPos(int iOffSetX, int iOffSetY);

	/// \brief	Set the text colors of the gui button
	/// \param	iColorNorm - the normal color
	/// \param	iColorGray	- the gray color
	void SetTextColors(int iColorNorm, int iColorGray);
	
	/// \brief	Set the increment value
	///	\param	fInc - the value to increment by
	void SetIncrementValue(float fInc);

	///	\brief	Set the range
	///	\param	fMin - the minimum value
	///	\param	fMax - the maximum value
	void SetRange(float fMin, float fMax);

	/// Sets whether you want the buttons to render on the right side or not (defaults to left)
	/// \param bRight = t/f buttons on right
	void SetButtonsOnRight(bool bRight) { m_bButtonsOnRight = bRight; }

	/// Updates font information
	/// \param fontData = vector of font info to update (more than one in this case!)
	/// \param xPos, yPos = position of list box
	/// \param width, height = size of list box
	/// \param state = state of instance
	void UpdateFontExt(vector<GUIFONTINFO> &fontData, float xPos, float yPos, float width, float height, DWORD state);

	/// Fills vertex buffer data
	/// \param vertexData = vector of triangle data to fill
	/// \param x, y = position of spinner
	/// \param width, height = size of spinner
	/// \param state = current instance state
	/// \param angle = angle of instance about center, in degrees
	/// \param color = color of instance, multiplied
	void LoadVertexBuffer(vector<GUIVERTEXINFO> &vertexData, float x, float y, float width, float height, DWORD state, 
												float angle, DWORD color);

	/// Checks to see if the mouse is over the spinner bits, updates state accordingly
	/// \param fCursorPosX, fCursorPosY = position of mouse cursor
	/// \param bLeftMouseDown = t/f left mb pressed
	/// \param bLeftMouseUp = t/f left mb released
	/// \param xPos, yPos = position of spinner
	/// \param width, height = size of spinner
	/// \param state = in/out of last state, update of current state
	/// \return true if mouse is over spinner
	bool MouseOver(float fCursorPosX, float fCursorPosY, bool bLeftMouseDown, bool bLeftMouseUp, float xPos, 
		float yPos, float width, float height, DWORD *state);

	/// Sets the default value of the spinner (float)
	/// \param val = default value
	void SetDefaultValue(float val);

	/// Sets the default value of the spinner (int)
	/// \param val = default value
	void SetDefaultValue(int val);

	/// Updates state given "up" button input
	/// \param lastState = last state of instance
	/// \return new state
	DWORD ToggleUp(DWORD lastState);
	
	/// Updates state given "down" button input
	/// \param lastState = last state of instance
	/// \return new state
	DWORD ToggleDown(DWORD lastState);

	/// Gets the text from the spinner in hash value given state
	/// \param inState = current state of instance
	/// \return hash of text
	DWORD GetHashText(DWORD inState);

	/// Sets the text of the spinner, if valid; returns updated state
	/// \param inState = current state of instance
	/// \param hashText = text to set
	/// \return updated state of spinner
	DWORD SetHashText(DWORD inState, DWORD hashText);

private:

	/// \brief	Set the Edit Text field
	void SetEditText();

	/// \brief	Set the editable integer value
	/// \param	iNum - the integer number
	void SetEditInt(int iNum);
	
	/// \brief	Set the editable float value
	/// \param	fNum - the float number
	void SetEditFloat(float fNum);

	/// Gets positional and size data for sub elements given overall item size
	/// \param x, y = position of spinner item
	/// \param width, height = size of spinner item
	/// \param arrowX = x position of arrow buttons
	/// \param editX = x position of edit box
	/// \param arrowWidth = width of arrow buttons
	/// \param editWidth = width of edit box
	void GetSubElemSpots(float x, float y, float width, float height, float &arrowX, float &editX, 
		float &arrowWidth, float &editWidth);

	/// Packs the substates into a complete spinner state
	/// \param editState = state of edit box
	/// \param upArrowState = state of up arrow
	/// \param downArrowState = state of down arrow
	/// \return packed dword state of spinner
	DWORD PackStates(DWORD editState, DWORD upArrowState, DWORD downArrowState);

	/// Unpacks composite state into sub states
	/// \param inState = state of spinner
	/// \param editState = state of edit box
	/// \param upArrowState = state of up arrow
	/// \param downArrowState = state of down arrow	
	void UnpackState(DWORD inState, DWORD &editState, DWORD &upArrowState, DWORD &downArrowState);

	/// Translates float or int value of spinner to hashstring hash val
	/// \return hashstring hash of value (minus bias)
	DWORD GetHashVal();
	
	/// Translates a hashstring hash val into float (or int)
	/// \param val = hashstring hash val to translate
	void LoadHashVal(DWORD val);

	/// \brief	Increment the edit number by one increment
	///	\param	bUp - if true, increment up, else increment down
	void IncrementValue(bool bUp);

	///	\brief	Restricts the value between the min and max range
	void ClampValue();	

	GUIButton * m_pEditBox;
	GUIButton * m_pUpArrow;
	GUIButton * m_pDownArrow;

	StdString m_szFontName;

	bool m_bFocus;

	bool m_bIsEnabled;
	bool m_bVisible;

	IRenderer * m_Renderer;

	// display text position offsetted from gui button's pos
	int m_iTextOffSetX;
	int m_iTextOffSetY;
	int m_iFontsize;

	eNumType m_NumType;

	float m_fEditValue;
	int m_iEditValue;

	float m_fIncrement;
	float m_fMinRange;
	float m_fMaxRange;

	bool m_bButtonsOnRight;

	DWORD m_HashBias;

	int m_iDefaultVal;
	float m_fDefaultVal;

};
	
#endif