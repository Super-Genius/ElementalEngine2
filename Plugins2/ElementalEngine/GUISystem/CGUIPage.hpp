///============================================================================
/// \file		CGUIPage.hpp
/// \brief		Header file for <insert brief description here>
/// \date		05-05-2005
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

#ifndef CGUIPAGE_HPP
#define	CGUIPAGE_HPP

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

typedef multimap<int, IObject*, ZORDERCOMPARE> SORTEDGUIMAP;

class CGUIManager;


///==========================================================================
/// \class	CGUIPage
/// \brief	Entity implementation of IObject interface
///==========================================================================
class CGUIPage : public CObjectTemplate<IObject>
{
private:
	/// \brief The Default Constructor for CGUIPage
	/// \param parentName = name of parent in hierarchy
	/// \param name = IHashString * name of this object
	CGUIPage(IHashString *parentName, IHashString *name);

public:
	/// \brief	The Default Destructor for CGUIPage
	~CGUIPage();

	/// \brief	Create the a instance of IObject
	/// \return	Pointer to this
	static IComponent *Create(int nArgs, va_list argptr);

	/// \brief Returns component type
	/// \return IHashString
	virtual IHashString *GetComponentType();

	/// Checks if this is similar to it's base classes component
	/// \return true if it is simlar, else false
	virtual bool IsKindOf(IHashString *compType);

	/// \brief	The Update Function for the Object
	/// \return	True on sucess, False on faulure
	virtual bool Update();

	/// \brief  Checks if page is a page buffer
	/// a page buffer is a temp page used to edit group's and 
	/// their gui elements...they will never be used in the game
	/// only in the editor. They are identified by "page_buffer_"
	/// string.
	/// \param pageName - the name of the page to check
	/// \return true if page buffer, else faluse
	bool PageBuffer(const StdString &pageName);
	/// Serialize this component to the archive.
	///	\param ar = reference to the archive class
	virtual void Serialize(IArchive &ar);

	/// Returns a list of objects on page
	/// \param IDTOOBJECTMAP - mapping of objects on this page
	IDTOOBJECTMAP GetPage();

	/// \brief Gets the Object under the mouse cursor
	/// Store a mouse x and y cursor position in the struct.
	/// If an object is founde under the cursor, the name of
	/// the object will be set inside the struct.
	/// \params size - the size of OBJUNDERCURSOR struct
	/// \param *params - the pointer to the OBJUNDERCURSOR struct
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

	// message handler section
	/// Mouse movement messages
	DWORD OnMouseMove(DWORD size, void *params);
	/// left mouse button down message
	DWORD OnLeftMousePress(DWORD size, void *params);
	/// left mouse button release message
	DWORD OnLeftMouseRelease(DWORD size, void *params);
	/// key press
	DWORD OnKeyPress(DWORD size, void *params);
	// message handler to add GUI Element to page
	DWORD OnAddGUIElementToPage(DWORD size, void *params);

	/// \brief Remove GUI Element from page list	
	/// \param size - sizeof(GUIPAGEMESSAGE)
	/// \param *params - pointer to GUIPAGEMESSAGE struct
	/// \return DWORD = message return code, MSG_HANDLED_STOP,
	///					MSG_HANDLED_PROCEED, error
	DWORD OnRemoveGUIElementFromPage(DWORD size, void *params);

	/// Sets the width ratio value for this page and all the underlying gui elements
	/// \param value - value to set ratio to
	void SetWidthRatio(float value);	

	/// Sets the height ratio value for this page and all the underlying gui elements
	/// \param value - value to set ratio to
	void SetHeightRatio(float value);	

	/// Sets the zoom for this page and all underlying gui elements
	/// \param value - value to set zoom to
	void SetZoomFactor(float value);

	/// Sets the page offset to render elements at 
	/// \param xOff - x offset 
	/// \param yOff - y offset 
	void SetPageOffset(int xOff, int yOff);

	/// Gets the page offset to render elements at
	/// \param xOff - x offset
	/// \param yOff - y offset
	void GetPageOffset(int *xOff, int *yOff)
	{
		*xOff = m_iPageOffsetX;
		*yOff = m_iPageOffsetY;
	}

	/// \brief	Sort the gui elements on this page
	void SortGUIElements();

	/// Allows us to turn on or off the mouse input from the elements
	void SetActive(bool value) { m_bActive = value; }

	/// Sets the texture to render to, if we're not rendering to the backbuffer
	/// \param tex = texture to render to
	void SetTextureToRenderTo(ITextureObject *tex);

	/// Gets the render object list
	/// \return list of objects
	OBJECTLIST GetRenderObjects() { return m_ObjectList; }

	/// Disables rendering of the page
	/// \param value = t/f disable rendering
	void DisableRendering(bool value) { m_bBatchRender = !value; }

	/// Sets the render context to render to
	/// \param value = render context to render
	void SetRenderContext(IRenderContext *value) { m_RenderContext = value; }

	/// Gets a list of the items on the page
	/// \param size = sizeof(GUIELEMLIST)
	/// \param param = GUIELEMLIST struct
	/// \return MSG_HANDLED_STOP
	DWORD OnGetPageItemList(DWORD size, void *param);

	/// Iterates thru the items in the page, allowing them to update the manager's vertex buffer
	/// \param pDest = buffer to write verts to
	/// \param offset = current offset in vb
	/// \param vb = vertex buffer interface to update
	UINT UpdatePageItemVertexBuffer(void **pDest, UINT offset, IVertexBufferObject *vb);

private:
	/// list of gui elements and groups on this page
	OBJECTLIST	m_ObjectList;

	/// map of sorted gui elements and groups on this page
	SORTEDGUIMAP m_SortedMap;

	/// adjustment for odd window aspect ratios
	float m_fWidthRatio;
	float m_fHeightRatio;
	
	/// zoom-ability
	float m_fZoomFactor;

	/// page offsets
	int m_iPageOffsetX, m_iPageOffsetY;

	/// active page simulation?
	bool m_bActive;

	/// to render or not...
	bool m_bBatchRender; 	

	IRenderContext *m_RenderContext;

	ITextureObject *m_pRenderTarget;

	CGUIManager *m_pManager;

	float m_fSaveVersion;
};

#endif	// CGUIPAGE_H