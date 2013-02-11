///============================================================================
/// \file		CGUIGroup.h
/// \brief		Header file for CGUIGroup
/// \date		03-24-2005
/// \author		Kenneth Hurley
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

#ifndef _CGUIGROP_H
#define _CGUIGROP_H

#ifndef _ZORDERCOMP_DEF_
#define _ZORDERCOMP_DEF_
struct ZORDERCOMPARE
{
	inline bool operator()(const int Left, const int Right) const
	{
		return (Left > Right);
	}
};
#endif

// map a Z Order to the gui element's render object
typedef multimap<int, IObject *, ZORDERCOMPARE> GUIELEMENTMAP;
// associate a z order to a gui element's render object
typedef pair<int, IObject *> GEMPAIR;

typedef list<IHashString*> NAMELIST;

class CGUIGroup : public CGUIElement
{
private:
	/// \brief	Default constructor for CGUIGroup
	/// \param parentName = name of parent in hierarchy
	/// \param name = IHashString * name of this object
	CGUIGroup(IHashString *parentName, IHashString *name);

public:
	/// \brief	Default deconstructor for CGUIGroup
	~CGUIGroup();

	/// \brief	Create an instance of GUIElement
	/// \return	Pointer to this
	static IComponent *Create(int nArgs, va_list argptr);

	/// \brief	The Update Function for the Object
	/// \return	True on sucess, False on faulure
	virtual bool Update();

	/// \brief  Override object template Serialize()
	virtual void Serialize(IArchive &ar);

	/// \brief Render the renderobject
	/// \return true if object was rendered, false otherwise
	virtual bool Render( UINT pass, IEffect * override );

	/// Renders this object with the data that it has from a separate location and size
	/// \param xPos - x position
	/// \param yPos - y position
	/// \param width - width position
	/// \param height - height position
	/// \param color - color to render element with
	virtual void RenderExternal(float xPos, float yPos, float width, float height, DWORD state, DWORD color, float angle);

	/// \brief Returns component type
	/// \return IHashString
	virtual IHashString *GetComponentType();

	/// Checks if this is similar to it's base classes component
	/// \return true if it is simlar, else false
	virtual bool IsKindOf(IHashString *compType);
	
	/// Determine if the cursor is over a gui element
	/// \param fCursorPosX = the x pos of the cursor
	/// \param fCursorPosY = the y pos of the cursor
	/// \return bool = is cursor over this element?
	bool  IsCursorOver(float fCursorPosX, float fCursorPosY);

	/// \brief Gets the position of this gui group
	/// Will return the coordinates to the upper left corner
	/// of the box which bounds this gui group.
	/// \param msgSize = size of POSITIONDATA
	/// \param param = pointer to POSITIONDATA * to set values from
	/// \return DWORD = message return code, MSG_HANDLED_STOP,  
	///					MSG_HANDLED_PROCEED, error
	virtual DWORD OnGetPosition(DWORD msgSize, void *param);

	/// \brief Sets the position of this gui element
	/// Given a new position, this function will calculate the offset
	/// for each of the gui elements in its sublist and update their
	/// positions as well.
	/// \param msgSize = size of POSITIONDATA
	/// \param param = pointer to POSITIONDATA * to set values from
	/// \return DWORD = message return code, MSG_HANDLED_STOP,  
	///					MSG_HANDLED_PROCEED, error
	virtual DWORD OnSetPosition(DWORD msgSize, void *param);

	/// Sets render offset for rendering away from 0,0 when attached
	/// \param msgSize = size of POSITIONDATA
	/// \param param = pointer to POSITIONDATA * to set values from
	/// \return DWORD = message return code, MSG_HANDLED_STOP,  
	///					MSG_HANDLED_PROCEED, error
	virtual DWORD OnSetAttachOffset(DWORD msgSize, void *param);

	/// Sets the width/height this gui element
	/// \param msgSize = size of SIZEDATA
	/// \param param = pointer to SIZEDATA * to set values from
	/// \return DWORD = message return code, MSG_HANDLED_STOP,  
	///					MSG_HANDLED_PROCEED, error
	virtual DWORD OnSetSize(DWORD msgSize, void *param);

	/// Sets the position of this gui element
	/// \param msgSize = size of SIZEDATA
	/// \param param = pointer to SIZEDATA * to set values from
	/// \return DWORD = message return code, MSG_HANDLED_STOP,  
	///					MSG_HANDLED_PROCEED, error
	virtual DWORD OnGetSize(DWORD msgSize, void *param);

	/// Gets a left mouse button press message
	/// \param msgSize = size of params
	/// \param params = pointer to a key structure
	/// \return DWORD = message return code, MSG_HANDLED_STOP,  
	///					MSG_HANDLED_PROCEED, error
	virtual DWORD OnLeftMousePress(DWORD msgSize, void *params);

	/// Gets a left mouse button release message
	/// \param msgSize = size of params
	/// \param params = pointer to a key structure
	/// \return DWORD = message return code, MSG_HANDLED_STOP,  
	///					MSG_HANDLED_PROCEED, error
	virtual DWORD OnLeftMouseRelease(DWORD msgSize, void *params);

	/// Gets a mouse position change message
	/// \param msgSize = size of params
	/// \param params = pointer to a key structure
	/// \return DWORD = message return code, MSG_HANDLED_STOP,  
	///					MSG_HANDLED_PROCEED, error
	virtual DWORD OnMousePosition(DWORD msgSize, void *params);

	/// Checks left mouse press based on external position and size (instance)
	/// \param xPos, yPos = external position
	/// \param width, height = external size
	virtual DWORD LeftMousePressExt(float xPos, float yPos, float width, float height)
	{
		return 0;
	}

	/// Checks left mouse release based on external position and size (instance)
	/// \param xPos, yPos = external position
	/// \param width, height = external size
	virtual DWORD LeftMouseReleaseExt(float xPos, float yPos, float width, float height)
	{
		return 0;
	}

	/// Checks mouse over based on external position and size (instance)
	/// \param size = sizeof(MOUSECOORDS)
	/// \param param = MOUSECOORDS of position
	/// \param xPos, yPos = external position
	/// \param width, height = external size
	virtual DWORD MousePositionExt(DWORD size, void* param, float xPos, float yPos, float width, float height)
	{
		return 0;
	}

	/// Gets a keyboard press
	/// \param msgSize = size of params
	/// \param params = pointer to a key structure
	/// \return DWORD = message return code, MSG_HANDLED_STOP,
	///					MSG_HANDLED_PROCEED, error
	virtual DWORD OnKeyPress(DWORD msgSize, void *params);

	/// Message to add a GUI Element to our list
	/// \param msgSize = size of params
	/// \param param = pointer to an ADDGUIELEMENT structure
	/// \return DWORD = message return code, MSG_HANDLED_STOP,
	///					MSG_HANDLED_PROCEED, error
	DWORD OnAddGUIElement(DWORD msgSize, void *param);

	/// Message to remove a GUI Element to our list
	/// \param msgSize = size of params
	/// \param param - pointer to a IHashString *
	/// \return DWORD = message return code, MSG_HANDLED_STOP,
	///					MSG_HANDLED_PROCEED, error
	DWORD OnRemoveGUIElement(DWORD msgSize, void *param);

	/// Message to get sub gui element under the cursor
	/// \param msgSize = size of OBJUNDERCURSOR struct
	/// \param param = pointer to the OBJUNDERCUSOR struct
	/// \return DWORD = message return code, MSG_HANDLED_STOP,
	///					MSG_HANDLED_PROCEED, error
	DWORD OnGetObjectUnderCursor(DWORD size, void *params);

	/// \brief Gets the Object inside the rect
	/// \param size - the size of OBJINRECT struct
	/// \param *params - pointer to the OBJINRECT struct
	/// \return DWORD = message return code, MSG_HANDLED_STOP,
	///					MSG_HANDLED_PROCEED, error	
	DWORD OnGetObjectInsideRect(DWORD size, void *params);

	/// \brief	Checks if checkX, checkY are inside the rect x1,y1,x2,y2
	/// \param  CheckX - the x position to checl
	/// \param	CheckY - the y position to check
	///	\param	x1 - the x1 position of the rect
	///	\param	y1 - the y1 position of the rect
	///	\param	x2 - the x2 position of the rect
	///	\param	y2 - the y2 position of the rect
	bool InsideRect(int checkX, int checkY, int x1, int y1, int x2, int y2);

	/// \brief	Show the gui group
	///	\param	msgSize - size of msg is 0
	///	\param	*param	- NULL
	/// \return DWORD - message return code, MSG_HANDLED_STOP,
	///					MSG_HANDLED_PROCEED, error
	virtual DWORD OnShow(DWORD msgSize, void *param);

	///	\brief	Hide the grui group
	///	\param	msgSize - size of msg is 0
	///	\param	*param	- NULL
	/// \return DWORD - message return code, MSG_HANDLED_STOP,
	///					MSG_HANDLED_PROCEED, error
	virtual DWORD OnHide(DWORD msgSize, void *param);

	/// \brief	Disable the gui element
	///	Will not allow the mouse input to affect any gui elements in this group
	virtual DWORD OnDisable(DWORD msgSize, void *param);

	///	\brief	Enable the gui element
	/// Will allow the mouse input to affect any gui elements in this group
	virtual DWORD OnEnable(DWORD msgSize, void *param);

	/// \brief	Checks if a groupName is already in the m_SubElements map
	///	\param	*groupName - the IHashString ptr to the group's name
	///	\return	true if *groupName is found in m_SubElements map, else false
	bool GroupExists(IHashString *groupName);

	/// Sets width factor for odd window aspect ratio adjustment
	/// \param value - value to set ratio to
	virtual void SetWidthRatio(float value);

	/// Sets height factor for odd window aspect ratio adjustment
	/// \param value - value to set ratio to
	virtual void SetHeightRatio(float value);

	/// Sets zoom factor for zooming in (used in editor)
	/// \param value - value to set zoom to
	virtual void SetZoomFactor(float value);

	/// Sets page offset for rendering away from 0,0 (used in editor)
	/// \param xOff - x offset
	/// \param yOff - y offset
	virtual void SetPageOffset(int xOff, int yOff);	

	/// Gets a list of the items in the group
	/// \param size = sizeof(GUIELEMLIST)
	/// \param param = GUIELEMLIST struct
	/// \return MSG_HANDLED_STOP
	DWORD OnGetGroupItemList(DWORD size, void *param);

	/// Checks to see if the size provided is ok
	/// This func is used for sliders, combo boxes, and spinners, where there is a set width (or height)
	/// that the object shouldn't render beyond
	/// \param width = width "request", adjusted value returned
	/// \param height = height "request", adjusted value returned
	virtual void CheckExtSize(float *width, float *height){}

	/// Updates the font stuffs per element, given size, position, and state
	/// \param xPos, yPos = position of instance
	/// \param width, height = size of instance
	/// \state = state of button (mouse over, mouse click, etc.)
	/// \return updated font info
	virtual DWORD UpdateFontExt(float xPos, float yPos, float width, float height, DWORD state)
	{
		return 0;
	}

	/// Checks to see if the group is hidden or not
	/// \param size = sizeof(bool)
	/// \param param = bool to store t/f
	/// \return MSG_HANDLED_STOP
	DWORD OnGetIsGroupVisible(DWORD size, void* param);
	
	/// Loads the gui manager buffer up with vertex data
	/// \param pDest = buffer to copy into
	/// \param offset = initial offset
	/// \param vb = vertex buffer object where this data is stored
	/// \return number of verts written
	virtual UINT UpdateBuffer( void **pDest, UINT offset, IVertexBufferObject *vb);

	/// Sorts the elements in the group
	void SortElements();

private:
	/// \brief Calculates and sets the position of this gui group
	/// This is different from SetPosition() in that it calculates 
	/// and sets the absolute position of the gui group based on
	/// the gui elements that are contained in its sublist. 
	void SetInitialPosition();
	
	/// \brief Calculates and sets the size of this gui group
	/// This is different from SetSize() in that it calculates 
	/// and sets the absolute position of the gui group based on
	/// the gui elements that are contained in its sublist. 
	void SetInitialSize();

private:
	// the sub elements of my group
	GUIELEMENTMAP m_SubElements;

	bool m_bEnableGroup;

	bool m_bShowGroup;
	
	float m_fWidthRatio;
	float m_fHeightRatio;
	float m_fZoomFactor;
	int m_iPageOffsetX;
	int m_iPageOffsetY;
	float m_fAttachOffsetX;
	float m_fAttachOffsetY;
	float m_fSaveVersion;
};

#endif //#ifndef _CGUIGROP_H