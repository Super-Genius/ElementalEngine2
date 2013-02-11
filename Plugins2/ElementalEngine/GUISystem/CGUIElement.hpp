///==========================================================================
/// \file	CGUIElement.hpp
/// \brief	GUI Elements in the system
///			The GUI Element is a main interface for obtaing information
///			about GUI items
/// \date	03-11-2005
/// \author Kenneth Hurley
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
///==========================================================================

#ifndef	_CGUIELEMENT_HPP
#define	_CGUIELEMENT_HPP

typedef struct GUIEDITORDATA
{
	int		m_iHit;
	float	m_fPosX;
	float	m_fPosY;
} GUIEDITORDATA;

///==========================================================================
/// \class	CGUIElement
/// \brief	GUIElement implementation of IGUIElement interface
///==========================================================================
class CGUIElement : public CSceneObject<IGUIElement>
{
protected:
	/// \brief The Default Constructor for CGUIElement
	/// \param parentName = name of parent in hierarchy
	/// \param name = IHashString * name of this object
	CGUIElement(IHashString *parentName, IHashString *name);

public:
	/// The Default Destructor for CGUIElement
	/// needs to be virtual so the real element will delete will be called
	virtual ~CGUIElement();

	/// \brief	Create an instance of GUIElement
	/// \return	Pointer to this
	static IComponent *Create(int nArgs, va_list argptr);

	/// \brief	The Update Function for the GUIElement
	/// \return	True on sucess, False on faulure
	virtual bool Update();

	/// Get the textual component type 
	/// \return textual IHashString * of component type
	virtual IHashString *GetComponentType();

	/// Checks if this component type is similar to a base class's componet type
	/// \return true if similar, else false
	virtual bool IsKindOf(IHashString *compType);
	
	/// \brief  Override object template Serialize()
	virtual void Serialize(IArchive &ar);

	/// \brief Render the renderobject
	/// \return true if object was rendered, false otherwise
	virtual bool Render( UINT pass, IEffect * override ) { return true; }

	/// \brief Compare for sorting
	/// \param obj = object to compare to
	/// \param sortMethod = a sort method specifying how to compare(set in RENDEROBJECTCOMPARE)
	/// \param curRenderPass = the current render pass to sort to
	/// \return bool = must return true if (this pointer) < (obj)
	virtual bool IsLessThan( const IRenderObject * obj, DWORD sortMethod, int curRenderPass );

	/// \brief This is for render priorities between classes
	/// will change as soon as a new architecture can be designed to better incorporate
	/// different renderobject types being pushed on the same sorted list
	/// \return DWORD = the priority key this returns, the higher the number,
	/// the higher its priority
	virtual DWORD GetRenderPriority(){ return m_iZOrder; }

	/// \brief Number of passes this render object has to go through
	/// \return UINT = returns one pass for this implementation
	virtual UINT GetNumPass() { return 1; }

	virtual IMaterial* GetMaterial( int index ) { return NULL; }
	virtual UINT GetNumMaterial() { return 0; }
	
	/// \brief Sets the position of the render object
	/// \param position Vec3 class reference to fill in
	virtual void SetPosition( const Vec3 &vec ) { }

	/// \brief Gets the position of the render object
	/// \return Vec3 = position f the object
	virtual Vec3 &GetPosition();

	/// \brief Returns the archive page
	/// \return m_Page = the page that the GUIManger is on
	virtual DWORD GetPage() {return m_Page;}

	/// \brief Returns the focus state
	/// \return m_bFocus = the focus state
	virtual bool GetFocusState() {return m_bFocus;}

	/// \brief Gets bounding sphere of object. This is a 3D interface call that should be implemented for only
	///			3D objects. Might want to seperate more of this later
	/// \param position = vector that will be filled the center of the sphere
	/// \param radius = float that will be filled with the radius of the sphere
	virtual void GetBoundingSphere( Vec3 &position, float &radius );

	/// \brief GetBoundingBox default implementation
	/// \param pos = position(center of box)
	/// \param dim = dimensions of box
	/// \param rotation = rotation euler angles of box
	virtual bool	GetBoundingBox( Vec3 &pos, Vec3 &dim, EulerAngle &rotation )
	{	
		return false;
	}
	/// Get the width of gui element
	/// \return float = width of gui element
	float GetWidth();

	/// Get height of gui element
	/// \return float = height of gui element
	float GetHeight();

	/// Get x coord position of gui element
	/// \return float = x pos of gui element
	float GetPosX();

	/// Get y coord position of gui element
	/// \return float = y pos of gui element
	float GetPosY();

	/// Get the Z order value
	/// \return int = z order vaule
	int GetZOrder() {return m_iZOrder;}

	/// Set Width of the Element
	/// \param x = new width
	/// \return void
	virtual void SetWidth(float x) { m_fSizeX = x; }

	/// Set Height of the Element
	/// \param y = new height
	/// \return void
	virtual void SetHeight(float y) { m_fSizeY = y; }

	/// Determine if the cursor is over a gui element
	/// \param fCursorPosX = the x pos of the cursor
	/// \param fCursorPosY = the y pos of the cursor
	/// \return bool = is cursor over this element?
	virtual bool IsCursorOver(float fCursorPosX, float fCursorPosY);

	/// Sets the position of this gui element
	/// \param msgSize = size of POSITIONDATA
	/// \param param = pointer to POSITIONDATA * to set values from
	/// \return DWORD = message return code, MSG_HANDLED_STOP,  
	///					MSG_HANDLED_PROCEED, error
	virtual DWORD OnSetPosition(DWORD msgSize, void *param);

	/// Sets the position of this gui element
	/// \param msgSize = size of POSITIONDATA
	/// \param param = pointer to POSITIONDATA * to get values from
	/// \return DWORD = message return code, MSG_HANDLED_STOP,  
	///					MSG_HANDLED_PROCEED, error
	virtual DWORD OnGetPosition(DWORD msgSize, void *param);

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
	/// \param msgSize = size of param
	/// \param param = pointer to float *
	/// \return DWORD = message return code, MSG_HANDLED_STOP,  
	///					MSG_HANDLED_PROCEED, error
	virtual DWORD OnLeftMousePress(DWORD msgSize, void *param);

	/// Gets a left mouse button release message
	/// \param msgSize = size of param
	/// \param param = pointer to float *
	/// \return DWORD = message return code, MSG_HANDLED_STOP,  
	///					MSG_HANDLED_PROCEED, error
	virtual DWORD OnLeftMouseRelease(DWORD msgSize, void *param);

	/// Gets a mouse position change message
	/// \param msgSize = size of param
	/// \param param = pointer to float *
	/// \return DWORD = message return code, MSG_HANDLED_STOP,  
	///					MSG_HANDLED_PROCEED, error
	virtual DWORD OnMousePosition(DWORD msgSize, void *param);

	/// Gets a keyboard press
	/// \param msgSize = size of param
	/// \param param = pointer to float *
	/// \return DWORD = message return code, MSG_HANDLED_STOP,
	///					MSG_HANDLED_PROCEED, error
	virtual DWORD OnKeyPress(DWORD msgSize, void *param);

	/// Remove focus from this gui element
	/// \param msgSize = size of param
	/// \param param = IHashString * to name of object to remove focus from
	/// \return DWORD = message return code, MSG_HANDLED_STOP,
	///					MSG_HANDLED_PROCEED, error
	virtual DWORD OnRemoveFocus(DWORD msgSize, void *param);

	///	 Sets the new ZOrder
	/// \param msgSize = size of param
	/// \param param = pointer to ZORDERDATA* data
	/// \return DWORD = message return code, MSG_HANDLED_STOP,
	///					MSG_HANDLED_PROCEED, error
	virtual DWORD OnSetZOrder(DWORD msgSize, void *param);

	///  Gets the ZOrder
	/// \param msgSize = size of param
	/// \param param = pointer to ZORDERDATA* data
	/// \return DWORD = message return code, MSG_HANDLED_STOP,
	///					MSG_HANDLED_PROCEED, error
	virtual DWORD OnGetZOrder(DWORD msgSize, void *param);

	// \brief  Returns the sorting class
	virtual IHashString * GetBaseSortClass();

	/// \brief returns if this renderobject needs to be sorted in the transparency list
	/// \return bool = true if this object uses transparency
	virtual bool IsAlphaObject(){ return false; }

	/// returns the mesh associated with this renderobject(should be a quad, but
	/// not implemented right now
	virtual IMeshObject * GetMeshObject(){ return NULL; }

	/// Gets the number of Geometry elements this renderobject possesses or if it uses geometry at all	
	virtual UINT GetNumGeometry(){ return 0; }

	/// Grabs the geometry pieces of the render object if there is any uses it for render instead
	/// of the render object render(might need to be changed)
	virtual IGeometry* GetGeometry( int index ){ return NULL; }

	/// \brief Set focus of this gui element to true
	virtual void SetFocus();

	/// \brief	Disable the gui element
	///	Each subclass of CGUIElement will implement this function differently
	virtual DWORD OnDisable(DWORD msgSize, void *param);

	///	\brief	Enable the gui element
	/// Each subclass of CGUIElement will implement this function differently
	virtual DWORD OnEnable(DWORD msgSize, void *param);

	/// \brief	On left key press
	virtual DWORD OnKeyLeft(DWORD msgSize, void *param);

	/// \brief	On right key press
	virtual DWORD OnKeyRight(DWORD msgSize, void *param);

	/// \brief	On home key press
	virtual DWORD OnKeyHome(DWORD msgSize, void *param);

	/// \brief	On end key press
	virtual DWORD OnKeyEnd(DWORD msgSize, void *param);

	/// \brief	On del key press
	virtual DWORD OnKeyDel(DWORD msgSize, void *param);

	/// \brief	On backspace key press
	virtual DWORD OnKeyBackSpace(DWORD msgSize, void *param);

	/// \brief	On enter key press
	virtual DWORD OnKeyEnter(DWORD msgSize, void *param);

	/// Attaches a light (unused, inherited)
	/// \param light = light to attach
	virtual void AttachLight( ILightObject * light ){}

	/// Signals a detach of all light objects (unused, inherited)
	virtual void DetachAllLights(){}
	
	/// Gets the material for the element (unused, inherited)
	/// \return NULL
	virtual IMaterial * GetMaterial(){ return NULL; }

	/// Gets the scene settings (unused, inherited)
	/// \return NULL
	virtual ISceneSettings * GetSceneSettings(){ return NULL; }

	/// Sets width factor for odd window aspect ratio adjustment
	/// \param value - value to set ratio to
	virtual void SetWidthRatio(float value){};
	
	/// Sets height factor for odd window aspect ratio adjustment
	/// \param value - value to set ratio to
	virtual void SetHeightRatio(float value){ m_fHeightRatio = value; }

	/// Sets zoom factor for zooming in (used in editor)
	/// \param value - value to set zoom to
	virtual void SetZoomFactor(float value){ m_fZoomFactor = value; }

	/// Sets page offset for rendering away from 0,0 (used in editor)
	/// \param xOff - x offset
	/// \param yOff - y offset
	virtual void SetPageOffset(int xOff, int yOff)
	{
		m_iPageOffsetX = xOff;
		m_iPageOffsetY = yOff;
	}

	/// Checks to see if we want to serialize this object or not
	/// Defaults here to true, but editor-specific objects can override and say false
	/// \return true if we want to serialize out, false otherwise
	virtual bool ToSerializeOut() { return true; }

	/// Loads the gui manager buffer up with vertex data
	/// \param pDest = buffer to copy into
	/// \param offset = initial offset
	/// \param vb = vertex buffer object where this data is stored
	/// \return number of verts written
	virtual UINT UpdateBuffer( void **pDest, UINT offset, IVertexBufferObject *vb) { return 0; }
	
private:
	///	\brief	Notfiy the gui manager that the active page needs sorting
	///	Gets called whenever the zorder of this guielement changes
	void NotifySort();

protected:
	/// Page this gui element belongs to
	DWORD	m_Page;
	/// Does this gui element have focus?
	bool m_bFocus;
	/// Width of gui element
	float m_fSizeX;
	/// Height of gui element
	float m_fSizeY;
	/// upper left corner X coord pos of gui element 
	float m_fPosX;
	/// upper left corner Y coord pos of gui element
	float m_fPosY;
	/// z draw order
	int m_iZOrder;
	/// adjustment for odd aspect ratios
	float m_fWidthRatio;
	float m_fHeightRatio;
	/// zoom factor, used in editor
	float m_fZoomFactor;
	/// page offsets
	int m_iPageOffsetX, m_iPageOffsetY;
	
};

#endif	// _CGUIELEMENT_HPP
