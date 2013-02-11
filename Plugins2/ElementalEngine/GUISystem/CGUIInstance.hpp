///============================================================================
/// \file		CGUIInstance.hpp
/// \brief		Header file for element instances
/// \date		10/05/2006
/// \author		Nick Tourte
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

#ifndef _C_GUIINSTANCE_HPP_
#define _C_GUIINSTANCE_HPP_

/// \class CGUIInstance
///	\brief	An adapter into Vision's GUI combbox code
class CGUIInstance : public CGUIElement
{
private:
	/// \brief	Default constructor for CGUIInstance
	///	\param	parentName = name of parent in hierarchy
	///	\param	name = IHashString *name of this object
	CGUIInstance(IHashString *parentName, IHashString *name);

public:
	///	\brief	Default deconstructor for CGUIInstance
	~CGUIInstance();

	///	\brief	Create an instance of CGUIInstance
	///	\return pointer to this
	static IComponent *Create(int nArgs, va_list argptr);

	/// \brief	The update function for the object
	///	\return	True on sucess, else false
	virtual bool Update();

	virtual IHashString* GetComponentType();

	virtual bool IsKindOf(IHashString *compType);

		/// \brief Render the renderobject
	/// \return true if object was rendered, false otherwise
	virtual bool Render( UINT pass, IEffect * override );

	/// Serialize this component to the archive.
	///	\param ar = reference to the archive class
	virtual void Serialize( IArchive &ar );

	/// Gets a left mouse button press message
	/// \param msgSize = size of keyStruct
	/// \param keyStruct = pointer to a key structure
	/// \return DWORD = message return code, MSG_HANDLED_STOP,  
	///					MSG_HANDLED_PROCEED, error
	virtual DWORD OnLeftMousePress(DWORD msgSize, void *keyStruct);

	/// Gets a left mouse button release message
	/// \param msgSize = size of keyStruct
	/// \param keyStruct = pointer to a key structure
	/// \return DWORD = message return code, MSG_HANDLED_STOP,  
	///					MSG_HANDLED_PROCEED, error
	virtual DWORD OnLeftMouseRelease(DWORD msgSize, void *keyStruct);

	/// Gets a mouse position change message
	/// \param msgSize = size of keyStruct
	/// \param keyStruct = pointer to a key structure
	/// \return DWORD = message return code, MSG_HANDLED_STOP,  
	///					MSG_HANDLED_PROCEED, error
	virtual DWORD OnMousePosition(DWORD msgSize, void *keyStruct);

	/// Gets a keyboard press
	/// \param msgSize = size of keyStruct
	/// \param keyStruct = pointer to a key structure
	/// \return DWORD = message return code, MSG_HANDLED_STOP,
	///					MSG_HANDLED_PROCEED, error
	virtual DWORD OnKeyPress(DWORD msgSize, void *keyStruct);

	/// Sets the position of this gui element
	/// \param msgSize = size of POSITIONDATA
	/// \param param = pointer to POSITIONDATA * to set values from
	/// \return DWORD = message return code, MSG_HANDLED_STOP,  
	///					MSG_HANDLED_PROCEED, error
	virtual DWORD OnSetPosition(DWORD msgSize, void *param);

	/// Sets the position of the element relative to parent position
	/// \param size = sizeof(POSITIONDATA)
	/// \param param = POSITIONDATA with relative coords
	/// \return MSG_HANDLED_STOP
	DWORD OnSetPositionRelative(DWORD size, void *param);

	/// Gets the position of the element relative to parent position
	/// \param size = sizeof(POSITIONDATA)
	/// \param param = POSITIONDATA with relative coords
	/// \return MSG_HANDLED_STOP
	DWORD OnGetPositionRelative(DWORD size, void *param);

	/// Sets the width/height this gui element
	/// \param msgSize = size of SIZEDATA
	/// \param param = pointer to SIZEDATA * to set values from
	/// \return DWORD = message return code, MSG_HANDLED_STOP,  
	///					MSG_HANDLED_PROCEED, error
	virtual DWORD OnSetSize(DWORD msgSize, void *param);	

	/// Sets the object reference for data rendering etc.
	/// \param size = sizeof(CHashString)
	/// \param param = CHashString with the name of the object to reference
	/// \return MSG_HANDLED_STOP
	virtual DWORD OnSetItemRef(DWORD size, void *param);	

    /// Gets the object reference for data rendering etc.
	/// \param size = sizeof(IHashString)
	/// \param param = IHashString with the name of the object to reference
	/// \return MSG_HANDLED_STOP
	virtual DWORD OnGetItemRef(DWORD size, void *param);	

	/// Sets the color of the instance (with alpha)
	/// \param size = sizeof(DWORD)
	/// \param param = DWORD color of object to set
	/// \return MSG_HANDLED_STOP
	virtual DWORD OnSetColor(DWORD size, void *param);

	/// Gets the color of the instance (with alpha)
	/// \param size = sizeof(DWORD)
	/// \param param = DWORD to return color of object
	/// \return MSG_HANDLED_STOP
	virtual DWORD OnGetColor(DWORD size, void *param);

	/// Sets JUST alpha, recalcs color given alpha
	/// \param size = sizeof(DWORD)
	/// \param param = DWORD alpha of object to set
	/// \return MSG_HANDLED_STOP
	virtual DWORD OnSetAlpha(DWORD size, void *param);

	/// Gets the current angle of this instance
	/// \param size = sizeof(float)
	/// \param param = float to get angle data
	/// \return MSG_HANDLED_STOP
	virtual DWORD OnGetAngle(DWORD size, void *param);

	/// Sets the angle of this instance
	/// \param size = sizeof(float)
	/// \param param = float to set angle
	/// \return MSG_HANDLED_STOP
	virtual DWORD OnSetAngle(DWORD size, void *param);

	/// Sets the angle from lua with a lua bound struct
	/// \param size = sizeof(GUIFLOAT)
	/// \param param = GUIFLOAT with float of angle to set
	/// \return MSG_HANDLED_STOP
	DWORD OnSetAngleFromLua(DWORD size, void *param);

	/// Sets width factor for odd window aspect ratio adjustment
	/// \param value - value to set ratio to
	virtual void SetWidthRatio(float value)
	{
		for (UINT i=0; i<m_pFont.size(); i++)
		{
			m_pFont[i]->SetWidthRatio(value);
		}
		m_fWidthRatio = value;
	}

	/// Sets height factor for odd window aspect ratio adjustment
	/// \param value - value to set ratio to
	virtual void SetHeightRatio(float value)
	{
		for (UINT i=0; i<m_pFont.size(); i++)
		{
			m_pFont[i]->SetHeightRatio(value);
		}
		m_fHeightRatio = value;
	}

	/// Sets zoom factor for zooming in (used in editor)
	/// \param value - value to set zoom to
	virtual void SetZoomFactor(float value)
	{
		for (UINT i=0; i<m_pFont.size(); i++)
		{
			m_pFont[i]->SetZoomFactor(value);
		}
		m_fZoomFactor = value;
	}

	/// Sets page offset for rendering away from 0,0 (used in editor)
	/// \param xOff - x offset
	/// \param yOff - y offset
	virtual void SetPageOffset(int xOff, int yOff)
	{
		for (UINT i=0; i<m_pFont.size(); i++)
		{
			m_pFont[i]->SetPageOffset(xOff, yOff);
		}
		m_iPageOffsetX = xOff;
		m_iPageOffsetY = yOff;
	}

	/// Loads the gui manager buffer up with vertex data
	/// \param pDest = buffer to copy into
	/// \param offset = initial offset
	/// \param vb = vertex buffer object where this data is stored
	/// \return number of verts written
	virtual UINT UpdateBuffer( void **pDest, UINT offset, IVertexBufferObject *vb);

	////// Animation messages //////

	/// Pos X animation
	/// param size = sizeof(float)
	/// \param param = float to change value to
	/// \return MSG_HANDLED_STOP
	DWORD OnGUIAnimPosX(DWORD size, void *param);

	/// Pos Y animation
	/// param size = sizeof(float)
	/// \param param = float to change value to
	/// \return MSG_HANDLED_STOP
	DWORD OnGUIAnimPosY(DWORD size, void *param);

	/// Size X animation
	/// param size = sizeof(float)
	/// \param param = float to change value to
	/// \return MSG_HANDLED_STOP
	DWORD OnGUIAnimSizeX(DWORD size, void *param);

	/// Size Y animation
	/// param size = sizeof(float)
	/// \param param = float to change value to
	/// \return MSG_HANDLED_STOP
	DWORD OnGUIAnimSizeY(DWORD size, void *param);

	/// Alpha color anim
	/// param size = sizeof(float)
	/// \param param = float to change value to
	/// \return MSG_HANDLED_STOP
	DWORD OnGUIAnimAlphaCol(DWORD size, void *param);

	/// R color anim
	/// param size = sizeof(float)
	/// \param param = float to change value to
	/// \return MSG_HANDLED_STOP
	DWORD OnGUIAnimRCol(DWORD size, void *param);

	/// G color anim
	/// param size = sizeof(float)
	/// \param param = float to change value to
	/// \return MSG_HANDLED_STOP
	DWORD OnGUIAnimGCol(DWORD size, void *param);

	/// B color anim
	/// param size = sizeof(float)
	/// \param param = float to change value to
	/// \return MSG_HANDLED_STOP
	DWORD OnGUIAnimBCol(DWORD size, void *param);

	/// Sets the state of the instance given a "select" input
	/// \param size = sizeof(bool)
	/// \param param = bool output to test whether continuing to press "select" changes the state of the item
	/// \return MSG_HANDLED_STOP
	DWORD OnSelectBtPressed(DWORD size, void *param);

	/// Sets the state of the instance given a "back" input
	/// \param size = sizeof(bool)
	/// \param param = bool output to test whether continuing to press "back" changes the state of the item
	/// \return MSG_HANDLED_STOP
	DWORD OnBackBtPressed(DWORD size, void *param);

	/// Sets the state of the instance given an "up" input
	/// \param size = sizeof(bool)
	/// \param param = bool output to test whether continuing to press "up" changes the state of the item
	/// \return MSG_HANDLED_STOP
	DWORD OnToggleUpBtPressed(DWORD size, void *param);

	/// Sets the state of the instance given a "down" input
	/// \param size = sizeof(bool)
	/// \param param = bool output to test whether continuing to press "down" changes the state of the item
	/// \return MSG_HANDLED_STOP
	DWORD OnToggleDownBtPressed(DWORD size, void *param);

	/// Gets the item text given the current state of the instance
	/// \param size = sizeof(IHashString*)
	/// \param param = IHashString* output to receive text
	/// \return MSG_HANDLED_STOP
	DWORD OnGetItemText(DWORD size, void *param);
	
	/// Sets the item text and updates the state appropriately
	/// \param size = sizeof(IHashString*)
	/// \param param = IHashString* input to change text to
	/// \return MSG_HANDLED_STOP
	DWORD OnSetItemText(DWORD size, void *param);

	/// Returns the parent name of the element in the hierarchy
	/// \param size = sizeof(IHashString*)
	/// \param param = IHashString* output to put name into
	/// \return MSG_HANDLED_STOP
	DWORD OnGetGUIParentName(DWORD size, void *param);

private:

	/// Creates the reference item, if it doesn't exist
	void CreateItem();

	/// Removes format tags in text
	/// \param inStr = raw text
	/// \return text without tags
	StdString RemoveFormatting(StdString inStr);

	/// If the string in has text color set, this will multiply that color with the current item color
	/// \param inStr = string with color tags
	/// \return string with adjusted color
	StdString MultiplyTextColor(StdString inStr);

private:
	
	/// reference element
	IGUIReference *m_ItemRef;

	float m_fPosX, m_fPosY;
	float m_fSizeX, m_fSizeY;

	StdString m_szItemCopyName;	
	StdString m_szItemCopyType;
	
	/// font objects
	vector <IFontObject*> m_pFont;

	/// text info for font objects
	DWORD m_DWORD;

	/// state of low-level item(s), to update on render
	DWORD m_State;

	/// editor vars
	float m_fWidthRatio;
	float m_fHeightRatio;
	float m_fZoomFactor;
	int m_iPageOffsetX;
	int m_iPageOffsetY;

	/// color
	DWORD m_iColor;

	float m_fAngle;

	/// color changed flag
	bool m_bColorChanged;

	IVertexBufferObject *m_VertexBuffer;
	UINT m_VBOffset;
	vector <GUIVERTEXINFO> m_VertexInfo;
	vector<GUIFONTINFO> m_FontInfo;

	IRenderer *m_Renderer;
	CGUIManager *m_Manager;

	float m_fSaveVersion;
};

#endif