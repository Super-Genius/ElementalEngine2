///============================================================================
/// \file		WorldEventRenderObject.h
/// \brief		Header file for a World Event Trigger Render Object
/// \date		01-16-2007
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

#ifndef	WORLDEVENTRENDEROBJECT_H
#define	WORLDEVENTRENDEROBJECT_H

class CWorldEventRenderObject : public CSceneObject<IRenderObject>
{
protected:
	/// \brief Default Constructor
	/// \param parentName = name of parent in hierarchy
	/// \param name = IHashString * name of this object
	CWorldEventRenderObject(IHashString *parentName, IHashString *name);

public:
	/// \briefDefault Destructor
	virtual ~CWorldEventRenderObject();

	/// \brief Create an instance
	/// \return	Pointer to this
	static IComponent *Create(int nArgs, va_list argptr);

	virtual void Init();

	virtual bool Update();

	virtual void DeInit();

	/// \brief Render the renderobject
	/// \return true if object was rendered, false otherwise
	virtual bool Render( UINT pass , IEffect * override);

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
	virtual DWORD GetRenderPriority();

	/// \brief Number of passes this render object has to go through
	/// \return UINT = the number of passes that this object has to go through
	virtual UINT GetNumPass();

	/// \brief Gets bounding sphere of object. This is a 3D interface call that should be implemented for only
	///			3D objects. Might want to seperate more of this later
	/// \param position = vector that will be filled the center of the sphere
	/// \param radius = float that will be filled with the radius of the sphere
	virtual void GetBoundingSphere( Vec3 &position, float &radius );

	virtual bool GetBoundingBox( Vec3 &pos, Vec3 &dim, EulerAngle &rotation )
	{	
		return false;
	}

	/// \brief Gets the base sort class which implements IRenderObject::IsLessThan()
	/// child classes must not re-implement IsLessThan() if they return the BaseSortClass hash!!
	/// typically, each render object has a IsLessThan() and returns a unique BaseSortClass
	virtual IHashString * GetBaseSortClass();

	/// \brief returns if this renderobject needs to be sorted in the transparency list
	/// \return bool = true if this object uses transparency
	virtual bool IsAlphaObject();

	/// \brief Gets the internal mesh
	virtual IMeshObject * GetMeshObject();

	/// Gets the number of Geometry elements this renderobject possesses or if it uses geometry at all	
	virtual UINT GetNumGeometry(){ return 0; }

	/// Grabs the geometry pieces of the render object if there is any uses it for render instead
	/// of the render object render(might need to be changed)
	virtual IGeometry* GetGeometry( int index ){ return NULL; }

	virtual void AttachLight( ILightObject * light ) { }
	virtual void DetachAllLights() { }
	virtual IMaterial * GetMaterial( int index ){ return NULL; }
	virtual UINT GetNumMaterial() { return 0; }
	virtual ISceneSettings * GetSceneSettings(){ return NULL; }

private:
	/// Pointer to the IRenderer
	IRenderer * m_IRenderer;
	/// Pointer to the editor
	CWorldEventEditor* m_pEditor;
};

#endif //WORLDEVENTRENDEROBJECT_H