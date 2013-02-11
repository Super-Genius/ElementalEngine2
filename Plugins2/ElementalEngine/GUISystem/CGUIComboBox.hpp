///============================================================================
/// \file		CGUIComboBox.hpp
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

#ifndef CGUICOMBOBOX_HPP
#define CGUICOMBOBOX_HPP

class GUIComboBox;

/// \class CGUIComboBox
///	\brief	An adapter into Vision's GUI combbox code
class CGUIComboBox : public CObjectTemplate<IGUIReference>
{
private:
	/// \brief	Default constructor for CGUIComboBox
	///	\param	parentName = name of parent in hierarchy
	///	\param	name = IHashString *name of this object
	CGUIComboBox(IHashString *parentName, IHashString *name);

public:
	///	\brief	Default deconstructor for CGUIComboBox
	~CGUIComboBox();

	///	\brief	Create an instance of CGUIComboBox
	///	\return pointer to this
	static IComponent *Create(int nArgs, va_list argptr);

	/// \brief	The update function for the object
	///	\return	True on sucess, else false
	virtual bool Update();

	virtual IHashString* GetComponentType();

	virtual bool IsKindOf(IHashString *compType);

	/// Gets rendering information for a vertex buffer given the dimensions and state of the element
	/// Returns a vector since reference element can be a composite
	/// \param vertexData = data of verts, textures, and effects
	/// \param x, y = position of element
	/// \param width, height = size of element
	/// \param state = state of object
	/// \param angle = angle element is to be rendered at
	/// \param color = color of instance (assumed to be ARGB)		
	virtual void GetRenderParams(vector<GUIVERTEXINFO> &vertexData, float x, float y, float width, float height, DWORD state, float angle=0.0f, DWORD color=-1);

	/// Gets font information given the dimensions and state of the element
	/// Returns a vector to allow for composite elements (with more than one font)
	/// \param fontData = data of font stuffs, text, etc
	/// \param x, y = position of element
	/// \param width, height = size of element
	/// \param state = state of object
	/// \param angle = angle element is to be rendered at
	/// \param color = color of instance (assumed to be ARGB)		
	virtual void GetFontParams(vector<GUIFONTINFO> &fontData, float x, float y, float width, float height, DWORD state, float angle=0.0f, DWORD color=-1);

	/// Serialize this component to the archive.
	///	\param ar = reference to the archive class
	virtual void Serialize( IArchive &ar );

	/// Gets a keyboard press
	/// \param msgSize = size of keyStruct
	/// \param keyStruct = pointer to a key structure
	/// \return DWORD = message return code, MSG_HANDLED_STOP,
	///					MSG_HANDLED_PROCEED, error
	virtual DWORD OnKeyPress(DWORD msgSize, void *keyStruct);

	/// \brief Get the selection rectangle
	///	\param mszSize = size of SELECTRECT;
	/// \param *param = pointer to the SELECTRECT
	/// \return DWORD = message return code, MSG_HANDLED_STOP,
	///					MSG_HANDLED_PROCEED, error
	DWORD OnGetSelectRect(DWORD msgSize, void *param);

	/// \brief	Enable the gui item
	DWORD OnEnable(DWORD msgSize, void *param);

	///	\brief	Disable the gui item
	DWORD OnDisable(DWORD msgSize, void *param);

	// \brief	Set a new button image
	DWORD OnSetButtonImage(DWORD msgSize, void *param);	

	/// Checks to see if the size provided is ok
	/// This func is used for sliders, combo boxes, and spinners, where there is a set width (or height)
	/// that the object shouldn't render beyond
	/// \param width = width "request", adjusted value returned
	/// \param height = height "request", adjusted value returned
	virtual void CheckSize(float *width, float *height);

	/// Checks the mouse position to see if we change state given dimensions of instance
	/// \param mouseX, mouseY = mouse position
	/// \param mouseState = state of mouse (mouse over, pressed, released)
	/// \param x, y, width, height = dimensions of instance
	/// \param dwLastState = last state of element
	/// \return state of element
	virtual DWORD CheckMouseState(float mouseX, float mouseY, MOUSESTATE mouseState, float x, float y, 
		float width, float height, DWORD dwLastState);

	/// Checks a key press given dimensions of instance
	/// \param size = sizeof(char)
	/// \param param = char of key pressed
	/// \return standard message return
	virtual DWORD CheckKeyPress(DWORD size, void *param)
	{ return MSG_NOT_HANDLED; }

	/// Adjusts state given a 'key up' command
	/// \param dwLastState = last state passed in
	/// \return new state
	virtual DWORD ToggleUp(DWORD dwLastState);

	/// Adjusts state given a 'key down' command
	/// \param dwLastState = last state passed in
	/// \return new state
	virtual DWORD ToggleDown(DWORD dwLastState);

	/// Adjusts state given a 'key select' command
	/// \param dwLastState = last state passed in
	/// \return new state
	virtual DWORD ToggleSelect(DWORD dwLastState);

	/// Adjusts state given a 'key back' command
	/// \param dwLastState = last state passed in
	/// \return new state
	virtual DWORD ToggleBack(DWORD dwLastState);

	/// Gets a hashval representing the current 'text' of the element, if applicable
	/// \param inState = current state of element
	/// \return hashstring hash val of string, or 0 if not applicable
	virtual DWORD GetHashText(DWORD inState);

	/// Sets a hashval representing the current 'text' of the element, if applicable
	/// \param inState = current state of element
	/// \param hashText = text to set
	/// \return new state
	virtual DWORD SetHashText(DWORD inState, DWORD hashText);

	/// Flags the ref to update its text if using a language entry
	virtual void FlagLanguageEntryChanged(){}


	/// MESSAGES ///

	/// Gets the combo data from the combo box
	/// \param msgSize = sizeof(COMBODATA)
	/// \param param = COMBODATA struct
	/// \return MSG_HANDLED_STOP
	DWORD OnGetComboData(DWORD msgSize, void *param);

	/// Clears the combo box
	/// \param msgSize = 0
	/// \param param = NULL
	/// \return MSG_HANDLED_STOP
	DWORD OnClearComboBox(DWORD msgSize, void *param);

	/// Adds an entry procedurally to the combo box
	/// \param msgSize = sizeof(ADDLISTBOXENTRY)
	/// \param param = ADDLISTBOXENTRY struct
	/// \return MSG_HANDLED_STOP
	DWORD OnAddEntry(DWORD msgSize, void *param);

private:

	/// \brief	Create the GUI combobox
	void CreateItem();

private:
	GUIComboBox m_ComboBox;

	StdString m_szSelectBoxNormTex;
	StdString m_szSelectBoxGrayTex;

	StdString m_szArrowNormTex;
	StdString m_szArrowSelTex;
	StdString m_szArrowGrayTex;
    
	StdString m_szDropDownTex;
	StdString m_szHighlightTex;

	StdString m_szListFile;

	StdString m_szFont;
	int m_iFontsize;

	int m_iNormFontColor;
	int m_iSelectFontColor;
	int m_iGrayFontColor;

	float m_fCursorPosX;
	float m_fCursorPosY;

	StdString m_szDefaultText;

	int m_iTextOffSetX;
	int m_iTextOffSetY;
		
	bool m_bButtonOnRight;

	int m_iListTextOffsetX;
	int m_iListTextOffsetY;
	int m_iListBoxOffsetY;
	int m_iListBoxHeightOffset;
};

#endif