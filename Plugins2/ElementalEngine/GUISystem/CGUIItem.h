///============================================================================
/// \file		CGUIItem.h
/// \brief		Header file for CGUIItem
///				An adapter into Visions' VMenuItem_cl
///				Used for static text, buttons, textures, etc...
/// \date		03-10-2005
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

#ifndef CGUIITEM_H
#define CGUIITEM_H

/// \class CGUIItem
/// \brief An adapter into Vision's VMenuItem_cl class
class CGUIItem : public CObjectTemplate<IGUIReference>
{
private:
	/// \brief	Default constructor for CGUIItem
	/// \param parentName = name of parent in hierarchy
	/// \param name = IHashString * name of this object
	CGUIItem(IHashString *parentName, IHashString *name);

public:
	/// \brief	Default deconstructor for CGUIItem
	virtual ~CGUIItem();

	/// \brief	Create an instance of CGUIItem
	/// \return	Pointer to this
	static IComponent *Create(int nArgs, va_list argptr);

	/// \brief	The Update Function for the Object
	/// \return	True on sucess, False on faulure
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
	virtual void CheckSize(float *width, float *height){}

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
	virtual DWORD GetHashText(DWORD inState) { return 0; }

	/// Sets a hashval representing the current 'text' of the element, if applicable
	/// \param inState = current state of element
	/// \param hashText = text to set
	/// \return new state
	virtual DWORD SetHashText(DWORD inState, DWORD hashText) { return inState; }

	/// Flags the ref to update its text if using a language entry
	virtual void FlagLanguageEntryChanged();

private:
	/// \brief Create the GUI item
	/// \param none
	/// \return none
	void CreateItem();

	GUIButton * m_pVisItem;

	// base tex
	StdString m_szNorm;
	StdString m_szHigh;
	StdString m_szSelect;
	StdString m_szGrayed;

	// decal tex
	StdString m_szDecalNorm;
	StdString m_szDecalHigh;
	StdString m_szDecalSelect;
	StdString m_szDecalGrayed;

	// text
	StdString m_szText;
	StdString m_szTextHigh;
	StdString m_szTextSelect;
	StdString m_szTextGrayed;

	// fonts
	StdString m_szFont;	
	StdString m_szFontBold;	
    StdString m_szFontItalic;		// only using two fonts now, not 4
	StdString m_szFontBoldItalic;	// only using two fonts now, not 4
	int m_iFontsize;

	// outdated; text color is now set with tags (text-specific)
	int m_iNormFontColor;
	int m_iHighFontColor;
	int m_iSelFontColor;
	int m_iGrayFontColor;

	// base texture is always smart scaled; outdated
	bool m_bSmartScale;

	int m_iTextOffSetX;
	int m_iTextOffSetY;

	float m_fDecalOffsetX;
	float m_fDecalOffsetY;
	bool m_bCenterText;

	float m_fCursorPosX;
	float m_fCursorPosY;

	float m_fRectX1;
	float m_fRectX2;
	float m_fRectY1;
	float m_fRectY2;

	float m_fSaveVersion;
};

#endif//#ifndef _CGUIItem__H__