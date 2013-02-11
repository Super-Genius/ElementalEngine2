///============================================================================
/// \file		GUIListBox.hpp
/// \brief		Refactored Vision GUI ListBox
/// \date		07-15-2005
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

#ifndef _GUILISTBOX_HPP_
#define _GUILISTBOX_HPP_

class GUIListBox;

class ListBoxElement
{
public:
	inline ListBoxElement( const TCHAR *szDisplayName, const TCHAR *szFullName, void *pCustomData=NULL )
	{
		m_szDisplayName = szDisplayName;
		m_szFullName = szFullName;
		m_pCutomData = pCustomData;
		SetColor( RGBA_WHITE );
		m_iSortingKey = 0;
		m_iBBox[0] = m_iBBox[1] = m_iBBox[2] = m_iBBox[3] = 0;
	}

	inline void SetColor(CColorRef iColor) {m_iColor=iColor;}
	inline void SetSortingKey(int iKey) {m_iSortingKey=iKey;};
	StdString m_szDisplayName;
	StdString m_szFullName;
	void *m_pCutomData;
	CColorRef m_iColor;
	int m_iSortingKey;

private:
	friend class GUIListBox;
	inline bool IsInsideBB(int sx,int sy, int xOffSet, int yOffSet) const
	{
        return (sx>=(m_iBBox[0]+xOffSet) && sx<=(m_iBBox[2]+xOffSet) && sy>=(m_iBBox[1]+yOffSet) && sy<=(m_iBBox[3]+yOffSet));
	}
	int m_iBBox[4]; // text bounding box of item
};


class GUIListBox
{

public:

	/// \brief	constructor
	GUIListBox();

	///	\brief	deconstructor
	~GUIListBox();

	///	\brief	Deinitialize the object
	void DeInit();
	
	///	\brief	Set the font
	/// \param	*szFontname - the fontname
	/// \param fontsize = size of font
	void SetFont(const TCHAR *szFontname, int fontsize);

	///	\brief	Set the font colors
	///	\param	iFontColor - the color of the listbox text
	///	\param	iSelectFontColor - the color of the selected text
	void SetColor(int iFontColor, int iSelectFontColor);
	
	///	\brief	Set the background listbox texture
	///	\param	*szBackMask - the background texture
	void SetBackMask(const TCHAR *szBackMask);
	
	///	\brief	Set the highlight select texture
	///	Not working
	///	\param	*szHighLightMask - the highlight texture
	void SetHighLightMask(const TCHAR *szHighLightMask);

	/// \brief	Set the test position offset
	/// \param	iTextOffSetX, iTextOffSetY = offset position of text
	void SetTextPos(int iTextOffSetX, int iTextOffSetY);

	/// \brief	Get the selected text
	///	\return	the selected text
	const TCHAR* GetSelectedText();

	///	\brief	Mouse is over the listbox's textlist
	///	\param	fCursorPosX - the cursor's x position
	///	\param	fCursorPosY - the cursor's y positoin
	/// \param bButtonPresed = t/f left mb pressed
	/// \param bButtonRelease = t/f left mb released
	/// \param xPos, yPos = position of list box
	/// \param width, height = size of list box
	/// \param inState = in/out of state (state gets adjusted)
	///	\return true if mouse is over the listbox
	bool MouseOverListBox(float fCursorPosX, float fCursorPosY, bool bButtonPresed, bool bButtonRelease, 
		float xPos, float yPos, float width, float height, DWORD *inState);

	/// \brief	Set the visible status
	/// \param bVisible = t/f visible (render)
	void SetVisible(bool bVisible) { m_bVisible = bVisible; }

	/// \brief	Select the next item in the listbox
	DWORD SelectNext();

	///	\brief	Select the previous item in the listbox
	DWORD SelectPrev();

	///	\brief	Reset the listbox entries
	void ResetList();

	///	\brief	AddEntry to the listbox
	///	\param	*szDisplayName - the display entry
	/// \param  lNum - the line number in the overall text block
	///	\param	iColor - the color to display the text
	/// \param	iSorting - the sort value	
	ListBoxElement *AddEntry( const TCHAR *szDisplayName, int lNum, CColorRef iColor=RGBA_WHITE, int iSorting=0);

	/// \brief	Wrap the text in the listbox
	/// \param bWrapText = t/f wrap text
	void WrapText(bool bWrapText) { m_bWrapText = bWrapText; m_iNumWrappedLines = 0; }
	
	///	\brief	Get the max height of the listbox w/ all the text in it
	///	\return the max height
	float GetMaxHeight();	

	/// Updates font information
	/// \param fontData = vector of font info to update (more than one in this case!)
	/// \param xPos, yPos = position of list box
	/// \param width, height = size of list box
	/// \param state = state of instance
	void UpdateFontExt(vector<GUIFONTINFO> &fontData, float xPos, float yPos, float width, float height, DWORD state);

	/// Fills vertex buffer data
	/// \param vertexData = vector of triangle data to fill
	/// \param x, y = position of list box
	/// \param width, height = size of list box
	/// \param state = current instance state
	/// \param angle = angle of instance about center, in degrees
	/// \param color = color of instance, multiplied
	void LoadVertexBuffer(vector<GUIVERTEXINFO> &vertexData, float x, float y, float width, float height, DWORD state, 
												float angle, DWORD color);

	/// Sets the file name of the listbox data
	/// \param filename = filename to set
	void SetFileName(StdString filename) { m_szFileName = filename; LoadTextFile(m_szFileName, 0); }

	/// Gets the list box item count
	/// \return count of items in list
	UINT GetItemCount() { return m_List.size(); }	

	/// Sets the selected text if in listbox, returns updated state
	/// \param text = text to find in list box
	/// \return updated listbox state
	DWORD SetSelectedText(const TCHAR* text);

	/// Sets the current selection given state
	/// \param inState = current state of instance
	void SetSelection(DWORD inState);

private:
	///	\brief	Get the picked text from the mouse coordinates
	/// \param xPos, yPos = position of list box
	///	\param sx - the mouse x coord
	///	\param sy - the mouse y coord
	/// \return the index to the picked text
	int GetPickedIndex(float xPos, float yPos, int sx, int sy);

	/// \brief	Set the text to display on multiple lines
	/// \param szText = text to reformat
	/// \param iColor = color of text
	void SetTextMultiLine(const TCHAR *szText, int iColor);

	/// \brief	Load a text file into the listbox
	/// \param szFilename = name of text file to load
	/// \param iColor = color of text
	bool LoadTextFile(const TCHAR *szFilename, int iColor);	

	//basic properties :
	const TCHAR * m_szBackGndMask;
	CLayeredMask *m_BGMask;

	// list :
	int m_iListCount;
	vector<ListBoxElement*> m_List;
	bool m_bResortList;
	bool m_bSortingEnabled;
    int m_iNumWrappedLines;
	bool m_bFileLoaded;

	// font :
	StdString m_szListFontName;
	StdString m_szFileName;
	StdString m_BlockForm;
	int m_iListFontOrder;
	int m_iLineHeight;
	int m_iMaxLineWidth;
	CColorRef m_FontColor;

	// view
	bool m_bViewChanged;
	bool m_bAllowSelection;
	int m_iCurrentSelIndex, m_iLastSelIndex;
	float m_fMaxOfs;
	float m_fMaxWidthOfs;

	//hightlight texture
	const TCHAR * m_szHighLightMask;
	CLayeredMask *m_HLMask;
	CColorRef m_HighLightColor;

	bool m_bWrapText;

	bool m_bVisible;

	StdString m_szSelectText;
	
	bool m_bEmptyText;

	int m_iTextOffSetX;
	int m_iTextOffSetY;
	int m_iFontsize;

	ListBoxElement *m_pSelElement;

	IRenderer *m_Renderer;

	bool m_bIsEnabled;

	void GenerateStringFromList();

	IEffect *m_pEffect;	
};

#endif
