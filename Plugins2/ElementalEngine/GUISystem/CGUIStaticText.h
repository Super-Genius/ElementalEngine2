///============================================================================
/// \file		CGUIStaticText.h
/// \brief		Header file for CGUIStaticText
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

#ifndef CGUISTATICTEXT_H
#define CGUISTATICTEXT_H

/// \class CGUIStaticText
/// \brief An adapter into Vision's VMenuItem_cl class
class CGUIStaticText : public CObjectTemplate<IGUIReference>
{
private:
	/// \brief	Default constructor for CGUIStaticText
	/// \param parentName = name of parent in hierarchy
	/// \param name = IHashString * name of this object
	CGUIStaticText(IHashString *parentName, IHashString *name);

public:
	/// \brief	Default deconstructor for CGUIStaticText
	~CGUIStaticText();

	/// \brief	Create an instance of GUIElement
	/// \return	Pointer to this
	static IComponent *Create(int nArgs, va_list argptr);

	/// \brief	The Update Function for the Object
	/// \return	True on sucess, False on faulure
	virtual bool Update();

	virtual IHashString* GetComponentType();

	virtual bool IsKindOf(IHashString *compType);

	/// Serialize this component to the archive.
	///	\param ar = reference to the archive class
	virtual void Serialize( IArchive &ar );

	/// \brief	Sets the static text
	/// \param msgSize = size of TEXTDATA
	/// \param param = pointer to TEXTDATA struct
	/// \return DWORD = message return code, MSG_HANDLED_STOP,  
	///					MSG_HANDLED_PROCEED, error
	virtual DWORD OnSetText(DWORD msgSize, void *param);

	/// \brief	Gets the static text
	/// \param msgSize = size of TEXTDATA
	/// \param param = pointer to TEXTDATA struct
	/// \return DWORD = message return code, MSG_HANDLED_STOP,  
	///					MSG_HANDLED_PROCEED, error
	virtual DWORD OnGetText(DWORD msgSize, void *param);

	/// Sets the text file to read and display
	/// \param size = sizeof(IHashString)
	/// \param param = IHashString with text filename
	/// return MSG_HANDLED_STOP
	DWORD OnSetTextFile(DWORD size, void *param);

	/// Sets the text color for the static text
	/// \param size = sizeof(DWORD)
	/// \param param = DWORD with color
	/// \return MSG_HANDLED_STOP
	DWORD OnSetTextColor(DWORD size, void *param);

	/// Loads text from a text file into string
	/// \param szFilename = name of file to load
	/// \return t/f success
	bool LoadTextFile(const TCHAR *szFilename);	

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
		float width, float height, DWORD dwLastState)
	{ return 0; }

	/// Checks a key press given dimensions of instance
	/// \param size = sizeof(char)
	/// \param param = char of key pressed
	/// \return standard message return
	virtual DWORD CheckKeyPress(DWORD size, void *param) { return MSG_NOT_HANDLED; }

	/// Adjusts state given a 'key up' command
	/// \param dwLastState = last state passed in
	/// \return new state
	virtual DWORD ToggleUp(DWORD dwLastState) { return dwLastState; }

	/// Adjusts state given a 'key down' command
	/// \param dwLastState = last state passed in
	/// \return new state
	virtual DWORD ToggleDown(DWORD dwLastState) { return dwLastState; }

	/// Adjusts state given a 'key select' command
	/// \param dwLastState = last state passed in
	/// \return new state
	virtual DWORD ToggleSelect(DWORD dwLastState) { return dwLastState; }

	/// Adjusts state given a 'key back' command
	/// \param dwLastState = last state passed in
	/// \return new state
	virtual DWORD ToggleBack(DWORD dwLastState) { return dwLastState; }

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
	virtual void FlagLanguageEntryChanged();

	/// Updates the text in a language entry given appropriate archive parameters
	/// \param size = sizeof(IArchive*)
	/// \param param = IArchive with params filled in
	/// \return MSG_HANDLED_STOP
	DWORD OnUpdateLanguageEntryParams(DWORD size, void *param);

	/// Changes the key for the language entry
	/// \param size = sizeof(CHANGELANGUAGEKEYPARAMS)
	/// \param param = CHANGELANGUAGEKEYPARAMS struct
	/// \return MSG_HANDLED_STOP
	DWORD OnChangeLanguageEntryKey(DWORD size, void *param);

private:
	/// \brief Create the GUI item
	/// \param none
	/// \return none
	void CreateItem();

	void UpdateLanguageEntryText();

	IArchive* CreateAndFillArchive();

	GUIButton * m_pStaticText;

	StdString m_szNorm;
	//StdString m_szHigh;
	//StdString m_szSelect;
	//StdString m_szGrayed;
	StdString m_szText;
	StdString m_szFilename;
	bool m_bLoadFromFile;
	StdString m_szFont;	
	int m_iFontsize;
	int m_iNormFontColor;
	int m_iHighFontColor;
	int m_iSelectFontColor;
	int m_iGrayFontColor;
	bool m_bHideTexture;

	int m_iTextOffSetX;
	int m_iTextOffSetY;
	bool m_bCenterText;

	float m_fCursorPosX;
	float m_fCursorPosY;

	bool m_bUseFontShadow;
	int m_iFontShadowColor;
	int m_iFontShadowXScale;
	int m_iFontShadowYScale;
	int m_iFontShadowXOffset;
	int m_iFontShadowYOffset;

	bool m_bUseLanguageEntry;
	StdString m_szLangEntryKey;

	BYTE *m_LanguageTextParams;
	UINT m_iLangTextParamSize;
};

#endif//#ifndef _CGUIStaticText__H__