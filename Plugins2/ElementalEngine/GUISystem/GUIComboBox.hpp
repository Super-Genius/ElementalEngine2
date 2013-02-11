///============================================================================
/// \file		GUIComboBox.hpp
/// \brief		Header file for <insert brief description here>
/// \date		07-12-2005
/// \author		Halbert Nakagawa
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

#ifndef GUICOMBOBOX_HPP
#define GUICOMBOBOX_HPP

class GUIButton;
class GUIListBox;

#include "stdafx.h"

class GUIComboBox
{
public:

	///	\brief	constructor
	GUIComboBox();

	///	\brief	deconstructor;
	~GUIComboBox();

	///	Sets the select box textures
	/// \param szNorm = texture for normal state
	/// \param szHigh = texture for high state
	/// \param szSelect = texture for select state
	/// \param szGray = texture for gray state
	void SetSelectBox(const TCHAR* szNorm, const TCHAR* szHigh, const TCHAR* szSelect, const TCHAR* szGray);

	///	Sets the dropdown box properties
	/// \param szNorm = texture for normal state
	/// \param szHigh = texture for high state
	/// \param szSelect = texture for select state
	/// \param szGray = texture for gray state
	/// \param textXOffset, textYOffset = offset position for text
	void SetDropDownBox(const TCHAR * szNorm, const TCHAR * szFileName, int yOffset, int heightOffset, 
		int textXOffset, int textYOffset);

	///	Sets the arrow box textures
	/// \param szNorm = texture for normal state
	/// \param szSelect = texture for select state
	/// \param szGray = texture for gray state
	void SetArrowBox(const TCHAR *szNorm, const TCHAR *szSelect, const TCHAR *szGray);

	///	\brief	Set the font
	///	\param	*szFontName - the name of the font
	/// \param fontsize = size of font
	void SetFont(const TCHAR *szFontName, int fontsize);

	/// \brief	Set the text colors
	/// \param iNormColor = color of text for normal state
	/// \param iHighColor = color of text for high state
	/// \param iSelectColor = color of text for select state
	/// \param iGrayColor = color of text for gray state
	void SetTextColors(int iNormColor, int iHighColor, int iSelectColor, int iGrayColor);

	///	Set the text offset position
	/// \param iTextOffSetX, iTextOffSetY = offset position to set
	void SetTextPos(int iTextOffSetX, int iTextOffSetY);

	///	\brief	Set the default selected text
	/// \param szDisplayText = default text to set
	void SetDefaultText(const TCHAR * szDisplayText);

	/// \brief	Get the selected text
	/// \return selected text
	const TCHAR* GetSelectedText();

	/// \brief	Get the selected text's index number
	/// \return selected text index
	int GetSelectedIndex();

	/// \brief	Add new entries to the combo box
	/// \param szDisplayName = text to add to combo
	/// \param iColor = color of text to add
	void AddEntry(const TCHAR* szDisplayName, int iColor);

	/// \brief	Reset the list in the combo box
	void Reset();

	/// Sets a texture for the highlight
	/// \param tex = filename of texture to set
	void SetHighlightTex(const TCHAR* tex)
	{
		m_DropDownBox.SetHighLightMask(tex);
	}

	/// Checks the mouse status against the current state for a state update
	/// \param fMouseCursorX, fMouseCursorY = mouse cursor position
	/// \param bButtonPressed = t/f left button pressed
	/// \param bButtonRelease = t/f left button released
	/// \param xPos, yPos = position of combo box
	/// \param width, height = size of combo box
	/// \param lastState = last state of instance
	/// \return new state of instance
	DWORD MouseOver(float fMouseCursorX, float fMouseCursorY, bool bButtonPressed, bool bButtonRelease, 
		float xPos, float yPos, float width, float height, DWORD lastState);

	/// Fills vertex buffer data
	/// \param vertexData = vector of triangle data to fill
	/// \param x, y = position of combo box
	/// \param width, height = size of combo box
	/// \param state = current instance state
	/// \param angle = angle of instance about center, in degrees
	/// \param color = color of instance, multiplied
	void LoadVertexBuffer(vector<GUIVERTEXINFO> &vertexData, float x, float y, float width, float height, UINT state, 
												float angle, DWORD color);

	/// Updates font information
	/// \param fontData = vector of font info to update (more than one in this case!)
	/// \param xPos, yPos = position of combo box
	/// \param width, height = size of combo box
	/// \param state = state of instance
	void UpdateFontExt(vector<GUIFONTINFO> &fontData, float xPos, float yPos, float width, float height, DWORD state);

	/// Sets the drop down button to draw on the right rather than the left
	/// \param bOnRight = t/f render on right
	void SetButtonOnRight(bool bOnRight) { m_bButtonOnRight = bOnRight; }
	
	/// Updates state given "back" button input
	/// \param lastState = last state of instance
	/// \return new state
	DWORD ToggleBack(DWORD lastState);

	/// Updates state given "select" button input
	/// \param lastState = last state of instance
	/// \return new state
	DWORD ToggleSelect(DWORD lastState);

	/// Updates state given "up" button input
	/// \param lastState = last state of instance
	/// \return new state
	DWORD ToggleUp(DWORD lastState);

	/// Updates state given "down" button input
	/// \param lastState = last state of instance
	/// \return new state
	DWORD ToggleDown(DWORD lastState);

	/// Gets the text from the combo given the state
	/// \param inState = current state of instance
	/// \return dword hash of text
	DWORD GetHashText(DWORD inState);

	/// Sets the current selection and combo text, if it's in the list; adjusts state appropriately
	/// \param inState = current state of instance
	/// \param hashText = hash of text to set, if it's in the list
	/// \return adjusted state
	DWORD SetHashText(DWORD inState, DWORD hashText);

private:

	/// Unpacks the instance state into substates
	/// \param state = input instance state
	/// \param arrowState = state of arrow button
	/// \param selectState = state of selection box
	/// \param listState = state of dropdown list box
	/// \return combo specific state
	DWORD UnpackState(DWORD state, DWORD &arrowState, DWORD &selectState, DWORD &listState);

	/// Packs the combination of substates into a dword state
	/// \param curState = state of combo
	/// \param arrowState = state of arrow button
	/// \param selectState = state of selection box
	/// \param listState = state of dropdown list box
	/// \return packed dword state
	DWORD PackStates(DWORD curState, DWORD arrowState, DWORD selectState, DWORD listState);

	GUIButton m_SelectBox;
	GUIButton m_Arrow;
	GUIListBox m_DropDownBox;

	StdString m_szFontName;
	int m_iFontsize;

	StdString m_szListFile;

	int m_iTextOffSetX;
	int m_iTextOffSetY;	
	int m_iListTextOffSetX;
	int m_iListTextOffSetY;	

	StdString m_szDisplayText;
	int m_iIndex;

	IRenderer *m_Renderer;

	bool m_bButtonOnRight;

	int m_iBoxOffsetY;
	int m_iBoxHeightOffset;
};

#endif
