///============================================================================
/// \file		CoordinateTool.h
/// \brief		Declaration of CCoordinateTool Object Class
/// \date		10-05-2007
/// \author		Richard Nguyen, Dmitriy S. Sergeyev
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
#ifndef __CCOORDINATETOOLRENDEROBJECT_H__
#define __CCOORDINATETOOLRENDEROBJECT_H__

class CCoordinateTool : public CSceneObject<IRenderObject>
{
private:
	/// Default Constructor
	CCoordinateTool(IHashString *parentName, IHashString *name);
	/// Default Destructor
	~CCoordinateTool();

public:
	static IComponent *Create(int nArgs, va_list argptr);

	/// Serialize this component to the archive.
	///	\param ar = reference to the archive class
	virtual void Serialize(IArchive &ar);

	/// Initializes the CCoordinateTool
	virtual void Init();

	/// \brief	The Update Function for the Object
	virtual bool Update();

	/// \brief	The DeInit Function for the Object
	virtual void DeInit();

	/// return a string of the component type
	/// \return IHashString *reference to textual classname
	virtual IHashString *GetComponentType();

	/// Checks if this is similar to it's base classes component
	/// \return true if it is simlar, else false
	virtual bool IsKindOf(IHashString *compType);

	virtual OBJECTSORTTYPES GetSceneSortID() const { return RENDEROBJECT3D; }

	/// \brief Render the renderobject
	/// \return true if object was rendered, false otherwise
	virtual bool Render(UINT pass, IEffect * override);

	/// \brief Compare for sorting
	/// \param obj = object to compare to
	/// \param sortMethod = a sort method specifying how to compare(set in RENDEROBJECTCOMPARE)
	/// \param curRenderPass = the current render pass to sort to
	/// \return bool = must return true if (this pointer) < (obj)
	virtual bool IsLessThan(const IRenderObject * obj, DWORD sortMethod, int curRenderPass);

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
	virtual void GetBoundingSphere(Vec3 &position, float &radius);

	virtual bool GetBoundingBox(Vec3 &pos, Vec3 &dim, EulerAngle &rotation);

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
	virtual UINT GetNumGeometry();

	/// Grabs the geometry pieces of the render object if there is any uses it for render instead
	/// of the render object render(might need to be changed)
	virtual IGeometry* GetGeometry(int index);

	virtual void AttachLight(ILightObject * light);
	virtual void DetachAllLights();
	virtual IMaterial * GetMaterial(int index);
	virtual UINT GetNumMaterial();
	virtual ISceneSettings * GetSceneSettings();

	// **** MESSAGES **** //
	DWORD OnSetParent(DWORD size, void *params);

private:
	/// Update tranform according to parent global transform update.
	bool UpdateTransformCustom();
	/// Create picking physics objects for all axes.
	void CreateAxesPickingBoxes();
	/// Delete picking physics objects for all axes.
	void DeleteAxesPickingBoxes();
	/// Create picking physics object.
	IObject * CreatePickingBox(IHashString *boxName, Vec3 &position, Vec3 &extents);
	
	// NOT IN USE ANY MORE (?)
	/// Get the distance from the gizmo to the camera
	//float GetCameraDistance();

	/// Class Type
	static CHashString m_hszNameType;

	/// Length of axes.
	static float m_axisLength;

	/// Axes picking boxes extent.
	static float m_axisBoxExtent;

	/// Pointer to the IRenderer
	IRenderer * m_IRenderer;

	/// CoordinateToolRenderer global transform
	Matrix4x4 m_globalTransform;

	/// True when physics object initialized once.
	bool m_physInit;

	/// X-axis box picking physics object instance.
	IHashString* m_xAxisPhysObjectName;
	IObject* m_xAxisPhysObject;
	/// Y-axis box picking physics object instance.
	IHashString* m_yAxisPhysObjectName;
	IObject* m_yAxisPhysObject;
	/// Z-axis box picking physics object instance.
	IHashString* m_zAxisPhysObjectName;
	IObject* m_zAxisPhysObject;

	/// true, when the object should be rendered. There are elements 
	/// in the hierarchy that can't be rotated, so they should not have 
	/// coordinatetool either
	BOOL m_bRenderCoordinateTool;
};

#endif // __CCOORDINATETOOLRENDEROBJECT_H__