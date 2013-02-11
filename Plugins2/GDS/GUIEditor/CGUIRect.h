///============================================================================
/// \file		CGUIRect.h
/// \brief		Header file for a 2D rectangle gui element
///				Used to draw a rectangle around the selectable area of a gui element
/// \date		07-18-2005
/// \author		Halbert Nakagawa
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

#ifndef CGUIRECT_H
#define CGUIRECT_H

/// \class CGUIRect
/// \brief 2d rectangle
///
/// Editor Specific GUI Object - Box
class CGUIRect : public CSceneObject<IGUIElement>
{
private:
	/// Constructor
	/// \param parentName = name of parent in hierarchy
	/// \param name = IHashString * name of this object
	CGUIRect(IHashString *parentName, IHashString *name);

public:
	~CGUIRect() {};

	/// Serialize this component to the archive.
	///	\param ar = reference to the archive class
	virtual void Serialize(IArchive &ar);

	/// standard static component create function
	static IComponent *Create(int nArgs, va_list argptr);

	/// \brief	The Update Function for the CGUIRect
	/// \return	True on sucess, False on faulure
	virtual bool Update();

	/// Get the textual component type 
	/// \return textual IHashString * of component type
	virtual IHashString *GetComponentType();

	/// Determine if the cursor is over a gui element
	/// \param iCursorPosX = the x pos of the cursor
	/// \param iCursorPosY = the y pos of the cursor
	/// \return bool = is cursor over this element?
	virtual bool IsCursorOver(int iCursorPosX, int iCursorPosY);

	/// Get the Z order value
	/// \return int = z order vaule
	int GetZOrder() {return -100;}

	/// \brief Render the renderobject
	/// \return true if object was rendered, false otherwise
	virtual bool Render( UINT pass, IEffect * override );

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
	virtual DWORD GetRenderPriority(){ return 0; }

	/// \brief Number of passes this render object has to go through
	/// \return UINT = returns one pass for this implementation
	virtual UINT GetNumPass() { return 1; };
	
	/// \brief Sets the position of the render object
	/// \param position IVec3 class reference to fill in
	virtual void SetPosition( const Vec3 &vec ) { };

	/// \brief Gets the position of the render object
	/// \return IVec3 = position f the object
	virtual Vec3 &GetPosition();

	/// \brief Gets bounding sphere of object. This is a 3D interface call that should be implemented for only
	///			3D objects. Might want to seperate more of this later
	/// \param position = vector that will be filled the center of the sphere
	/// \param radius = float that will be filled with the radius of the sphere
	virtual void GetBoundingSphere( Vec3 &position, float &radius );

	/// \brief GetBoundingBox default implementation
	/// \param pos = position(center of box)
	/// \param dim = dimensions of box
	/// \param rotation = rotation euler angles of box
	virtual bool GetBoundingBox( Vec3 &pos, Vec3 &dim, EulerAngle &rotation )
	{	
		return false;
	}

	/// \brief Gets the base sort class which implements IRenderObject::IsLessThan()
	/// child classes must not re-implement IsLessThan() if they return the BaseSortClass hash!!
	/// typically, each render object has a IsLessThan() and returns a unique BaseSortClass
	virtual IHashString * GetBaseSortClass();

	// ------------------ Messages ------------------ //
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

	virtual DWORD OnSetRect(DWORD msgSIze, void *param);

	/// \brief returns if this renderobject needs to be sorted in the transparency list
	/// \return bool = true if this object uses transparency
	virtual bool IsAlphaObject(){ return false; }

	/// returns the mesh associated with this renderobject(should be a quad, but
	/// not implemented right now
	virtual IMeshObject * GetMeshObject(){ return NULL; };
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

	virtual void UpdateProgressBar(DWORD size, void *param){}

	DWORD OnGetHandleIndex(DWORD msgSize, void *param);
	DWORD OnFollowObject(DWORD size, void *param);

	virtual void SetWidthRatio(float value){ m_fWidthRatio = value; }
	virtual void SetHeightRatio(float value){ m_fHeightRatio = value; }
	virtual void SetZoomFactor(float value){ m_fZoomFactor = value; }
	virtual void SetPageOffset(int xOff, int yOff)
	{
		m_iPageOffsetX = xOff;
		m_iPageOffsetY = yOff;
	}
	virtual bool ToSerializeOut() { return false; }

	/// Loads the gui manager buffer up with vertex data
	/// \param pDest = buffer to copy into
	/// \param offset = initial offset
	/// \param vb = vertex buffer object where this data is stored
	/// \return number of verts written
	virtual UINT UpdateBuffer( void **pDest, UINT offset, IVertexBufferObject *vb);

	// stubs
	UINT GetNumGeometry(){ return 0; }
	IGeometry* GetGeometry(int index){ return NULL; }
	void AttachLight(ILightObject *light) {}
	void DetachAllLights() {}
	virtual IMaterial * GetMaterial( int index ){ return NULL; }
	virtual UINT GetNumMaterial() { return 0; }
	virtual ISceneSettings * GetSceneSettings(){ return NULL; }
	bool IsCursorOver(float fPosX, float fPosY){ return false; }

private:

	void GetQuad(GUIVERTEX data[], float x, float y, float width, float height, DWORD color, 
		float uStart=0.0f, float vStart=0.0f, float uEnd=1.0f, float vEnd=1.0f);

	IRenderer *m_Renderer;

	// Width of gui element
	int m_iSizeX;
	// Height of gui element
	int m_iSizeY;
	// upper left corner X coord pos of gui element 
	int m_iPosX;
	// upper left corner Y coord pos of gui element
	int m_iPosY;
	// z draw order
	int m_iZOrder;

	int m_iRectX1;
	int m_iRectX2;
	int m_iRectY1;
	int m_iRectY2;

	float m_fWidthRatio;
	float m_fHeightRatio;
	float m_fZoomFactor;
	int m_iPageOffsetX, m_iPageOffsetY;

	ITextureObject *m_pRectOutline;

	IVertexBufferObject *m_VertexBuffer;
	UINT m_VBOffset;
};

#endif	// #ifndef CGUIBOX_H
