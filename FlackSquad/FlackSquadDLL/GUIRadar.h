///==========================================================================
/// \file	GUIRadar.h
/// \brief	Radar for FlackSquad
/// \date	01/10/07
/// \author Nick Tourte
///==========================================================================

#ifndef	_CGUIRADAR_HPP
#define	_CGUIRADAR_HPP

#include "IGUIElement.h"

typedef map<DWORD, CHashString> ETYPEMAP;

struct ENEMYPOS
{
	float x, y;
};

typedef vector<ENEMYPOS> ALLENEMYPOS;

///==========================================================================
/// \class	CGUIRadar
/// \brief	GUIElement implementation of IGUIElement interface
///==========================================================================
class CGUIRadar : public CObjectTemplate<IGUIReference>
{
protected:
	
	/// CTor
	CGUIRadar(IHashString *parentName, IHashString *name);

public:
	/// DTor
	~CGUIRadar();

	/// \brief	Create an instance of GUIElement
	/// \return	Pointer to this
	static IComponent *Create(int nArgs, va_list argptr);

	/// \brief	The Update Function for the GUIElement
	/// \return	True on sucess, False on faulure
	virtual bool Update();

	/// Get the textual component type 
	/// \return textual IHashString * of component type
	virtual IHashString *GetComponentType()
	{
		return &radarCT;
	}

	/// Checks if this component type is similar to a base class's componet type
	/// \return true if similar, else false
	virtual bool IsKindOf(IHashString *compType)
	{
		if (compType)
		{
			if (compType->GetUniqueID() == radarCT.GetUniqueID())
			{
				return true;
			}
		}
		return false;
	}
	
	/// \brief  Override object template Serialize()
	virtual void Serialize(IArchive &ar);

	/// \brief Compare for sorting
	/// \param obj = object to compare to
	/// \param sortMethod = a sort method specifying how to compare(set in RENDEROBJECTCOMPARE)
	/// \param curRenderPass = the current render pass to sort to
	/// \return bool = must return true if (this pointer) < (obj)
	virtual bool IsLessThan( const IRenderObject * obj, DWORD sortMethod, int curRenderPass ) { return false; }

	/// \brief This is for render priorities between classes
	/// will change as soon as a new architecture can be designed to better incorporate
	/// different renderobject types being pushed on the same sorted list
	/// \return DWORD = the priority key this returns, the higher the number,
	/// the higher its priority
	virtual DWORD GetRenderPriority(){ return 0; }

	/// \brief Number of passes this render object has to go through
	/// \return UINT = returns one pass for this implementation
	virtual UINT GetNumPass() { return 1; }
	
	/// \brief Sets the position of the render object
	/// \param position Vec3 class reference to fill in
	virtual void SetPosition( const Vec3 &vec ) { }

	/// \brief Gets the position of the render object
	/// \return Vec3 = position f the object
	virtual Vec3 &GetPosition()
	{
		static Vec3 empty;
		return empty;
	}

	virtual DWORD OnKeyPress(DWORD msgSize, void *param){ return MSG_NOT_HANDLED; }

	/// \brief Gets bounding sphere of object. This is a 3D interface call that should be implemented for only
	///			3D objects. Might want to seperate more of this later
	/// \param position = vector that will be filled the center of the sphere
	/// \param radius = float that will be filled with the radius of the sphere
	virtual void GetBoundingSphere( Vec3 &position, float &radius ){}

	/// \brief GetBoundingBox default implementation
	/// \param pos = position(center of box)
	/// \param dim = dimensions of box
	/// \param rotation = rotation euler angles of box
	virtual bool	GetBoundingBox( Vec3 &pos, Vec3 &dim, Vec3 &rotation )
	{	
		return false;
	}

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

	DWORD OnSetTurretPosition(DWORD size, void *param);
	DWORD OnSetTurretBearing(DWORD size, void *param);
	DWORD OnSetRadarRadius(DWORD size, void *param);
	DWORD OnCheckNameInRadarRange(DWORD size, void *param);

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
	virtual void GetFontParams(vector<GUIFONTINFO> &fontData, float x, float y, float width, float height, DWORD state, float angle=0.0f, DWORD color=-1){}

	/// Checks to see if the size provided is ok
	/// This func is used for sliders, combo boxes, and spinners, where there is a set width (or height)
	/// that the object shouldn't render beyond
	/// \param width = width "request", adjusted value returned
	/// \param height = height "request", adjusted value returned
	virtual void CheckSize(float *width, float *height){};

	/// Checks the mouse position to see if we change state given dimensions of instance
	/// \param mouseX, mouseY = mouse position
	/// \param mouseState = state of mouse (mouse over, pressed, released)
	/// \param x, y, width, height = dimensions of instance
	/// \param dwLastState = last state of element
	/// \return state of element
	virtual DWORD CheckMouseState(float mouseX, float mouseY, MOUSESTATE mouseState, float x, float y, 
		float width, float height, DWORD dwLastState){ return dwLastState; }

	/// Checks a key press given dimensions of instance
	/// \param size = sizeof(char)
	/// \param param = char of key pressed
	/// \return standard message return
	virtual DWORD CheckKeyPress(DWORD size, void *param){ return MSG_NOT_HANDLED; }

	// \brief  Returns the sorting class
	virtual IHashString * GetBaseSortClass()
	{
		static CHashString sortClass(_T("IGUIElement"));
		return &sortClass;
	}

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
	virtual void SetFocus() {}

	virtual void AttachLight( ILightObject * light ){}

	/// Signals a detach of all light objects
	virtual void DetachAllLights(){}
	
	virtual IMaterial * GetMaterial(){ return NULL; }
	virtual ISceneSettings * GetSceneSettings(){ return NULL; }

	/// Sets width factor for odd window aspect ratio adjustment
	/// \param value - value to set ratio to
	virtual void SetWidthRatio(float value){ /*m_fWidthRatio = value;*/ }

	/// Sets height factor for odd window aspect ratio adjustment
	/// \param value - value to set ratio to
	virtual void SetHeightRatio(float value){ /*m_fHeightRatio = value;*/ }

	/// Sets zoom factor for zooming in (used in editor)
	/// \param value - value to set zoom to
	virtual void SetZoomFactor(float value){ /*m_fZoomFactor = value;*/ }

	/// Sets page offset for rendering away from 0,0 (used in editor)
	/// \param xOff - x offset
	/// \param yOff - y offset
	virtual void SetPageOffset(int xOff, int yOff)
	{
		/*
		m_iPageOffsetX = xOff;
		m_iPageOffsetY = yOff;
		*/
	}

	/// Checks to see if we want to serialize this object or not
	/// Defaults here to true, but editor-specific objects can override and say false
	/// \return true if we want to serialize out, false otherwise
	virtual bool ToSerializeOut() { return true; }	

	/// Adjusts state given a 'key up' command
	/// \param dwLastState = last state passed in
	/// \return new state
	virtual DWORD ToggleUp(DWORD dwLastState){ return dwLastState; }

	/// Adjusts state given a 'key down' command
	/// \param dwLastState = last state passed in
	/// \return new state
	virtual DWORD ToggleDown(DWORD dwLastState){ return dwLastState; }

	/// Adjusts state given a 'key select' command
	/// \param dwLastState = last state passed in
	/// \return new state
	virtual DWORD ToggleSelect(DWORD dwLastState){ return dwLastState; }

	/// Adjusts state given a 'key back' command
	/// \param dwLastState = last state passed in
	/// \return new state
	virtual DWORD ToggleBack(DWORD dwLastState){ return dwLastState; }

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
	virtual void FlagLanguageEntryChanged() {}

	DWORD OnGetFrameRate(DWORD size, void *param);
	
private:
	///	\brief	Notfiy the gui manager that the active page needs sorting
	///	Gets called whenever the zorder of this guielement changes
	void NotifySort();

	void LoadEntry(GUIVERTEXINFO &entry, float x, float y, DWORD color);

protected:	

	IRenderer *m_Renderer;

	float m_fSizeX;
	float m_fSizeY;
	float m_fPosX;
	float m_fPosY;

	Vec3 m_TurretPos;
	float m_fTurretBearing;
	float m_fRadarRadius;

	static CHashString radarCT;
	ETYPEMAP m_mEntityTypes;

	ALLENEMYPOS m_vEnemyPos;
	ALLENEMYPOS m_vEnemyBossPos;

	IBaseTextureObject *m_pBlipTexture;
	float m_fPreviousBlipUpdateTime;
	float m_fPreviousBlipPosUpdateTime;
};

#endif	// _CGUIRADAR_HPP
