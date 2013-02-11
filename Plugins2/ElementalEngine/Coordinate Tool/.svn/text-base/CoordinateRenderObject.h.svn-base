///============================================================================
/// \file		CoordinateRenderObject.h
/// \brief		Declaration of CBoundingBox Object Class
/// \date		08-03-2007
/// \author		Richard Nguyen
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

#ifndef __COORDINATERENDEROBJECT_H__
#define __COORDINATERENDEROBJECT_H__

typedef struct COORDINATETOOLVERTEX
{
	float pos[3];
	float ucoord, vcoord;
} COORDINATETOOLVERTEX;

class CCoordinateRenderObject : public CSceneObject<IRenderObject>
{
private:
	/// Default Constructor
	CCoordinateRenderObject( IHashString *parentName, IHashString *name );
	/// Default Destructor
	~CCoordinateRenderObject();

public:
	static IComponent *Create(int nArgs, va_list argptr);

	/// Serialize this component to the archive.
	///	\param ar = reference to the archive class
	virtual void Serialize( IArchive &ar );

	/// Initializes the CBoundingBox
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
	virtual DWORD GetRenderPriority();

	/// \brief Number of passes this render object has to go through
	/// \return UINT = the number of passes that this object has to go through
	virtual UINT GetNumPass();

	/// \brief Sets the position of the render object
	/// \param x = x position
	/// \param y = y position
	/// \param z = z position
	virtual void SetPosition( const Vec3 &vPosition );

	/// \brief Gets the position of the render object
	/// \return Vec3 = position f the object
	virtual Vec3 &GetPosition();

	/// \brief Gets bounding sphere of object. This is a 3D interface call that should be implemented for only
	///			3D objects. Might want to seperate more of this later
	/// \param position = vector that will be filled the center of the sphere
	/// \param radius = float that will be filled with the radius of the sphere
	virtual void GetBoundingSphere( Vec3 &position, float &radius );
	virtual bool GetBoundingBox( Vec3 &pos, Vec3 &dim, EulerAngle &rotation );

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

	/// Calculates the Min and Max from Postion +- m_vEnds*0.5f
	/// \param vMin - Min Position
	/// \param vMax - Max Position
	void GetMinMax( Vec3& vMin, Vec3& vMax );

	/// Gets the Ends Vector (determines Bound Box min and max);
	/// \return - Ends Vector
	Vec3& GetEnds( void );
	/// Sets the Ends Vector (determines Bound Box min and max);
	/// \param vEnds - Ends Vector
	void SetEnds( Vec3& vEnds );

	/// Gets the Rotation Vector (Euler Angles)
	/// \return - Rotation Vector( Yaw, Pitch, Roll )
	EulerAngle& GetRotation( void );
	/// Sets the Rotation Vector (Euler Angles)
	/// \param - Rotation Vector( Yaw, Pitch, Roll )
	void SetRotation( EulerAngle& vRotation );

private:
	void GenerateVertices( void );
	void InitializeTextures( void );

	IToolBox				*m_ToolBox;
	IRenderer				*m_IRenderer;
	IController				*m_GeometryController;
	IGeometry				*m_Geometry;
	IBaseTextureObject		*m_Texture;
	StdString				m_TexturePath;

	CCoordinateToolShaderCallback	*m_CoordinateToolShaderCallback;
	StdString					m_ShaderFile;

	/// Global allocation for vertex and index buffers
	BUFFERALLOCATIONSTRUCT	m_IndexBufferAllocation;
	BUFFERALLOCATIONSTRUCT	m_VertexBufferAllocation;
	IIndexBuffer			*m_IndexBufferInterface;
	IVertexBufferObject		*m_VertexBufferInterface;

	/// Class Type
	CHashString				m_hszNameType;

	/// Transformation vectors
	Vec3					m_vPosition;
	EulerAngle				m_vRotation;
	Vec3					m_vEnds;

	static const float		m_AxisLength;
	static const float		m_AxisWidthInSpace;
	static const UINT		m_AxisWidthInPixels;
};

#endif //__COORDINATERENDEROBJECT_H__

