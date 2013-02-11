///============================================================================
/// \file		DetailObject.h
/// \brief		Declaration of Detail Object Class
/// \date		05-27-2007
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

#ifndef __DETAILOBJECT_H__
#define __DETAILOBJECT_H__

/// This structure allows us to draw geometry with only 4 bytes
///	per vertex passed into the vertex shader.
typedef struct COMPRESSED3DETAILVERTEX
{
	unsigned char pos[3];		/// position in 8 bit format
	unsigned char vertIndex:4;	/// vertex index in constant memory
	unsigned char yaw:4;		/// yaw around center
} SUPERCOMPRESSEDDETAILVERTEX;

/// This structure allows us to draw geometry with only 8 bytes
///	per vertex passed into the vertex shader.
typedef struct COMPRESSED2DETAILVERTEX
{
	unsigned short pos[3];		/// position in 16 bit format
	unsigned char vertIndex;	/// vertex index in constant memory
	unsigned char yaw;			/// yaw around center
} COMPRESSEDDETAILVERTEX;

/// This structure allows us to draw geometry with only 16 bytes
///	per vertex passed into the vertex shader.
typedef struct COMPRESSED1DETAILVERTEX
{
	float pos[3];				// unquantized position
	DWORD vertIndex:16;			// 16 bits for vertex index
	DWORD yaw:16;				// yaw quanitized
} COMPRESSED1DETAILVERTEX;

/// This structure allows us to draw geometry with 24 bytes
///	per vertex passed into the vertex shader.
typedef struct DETAILVERTEX
{
	float pos[3];				// unquantized position
	float UV[2];				// uv coordinates
	float yaw;					// unquantized yaw
} DETAILVERTEX;

class CDetailObject : public CSceneObject<IRenderObject>
{
private:
	/// Default Constructor
	CDetailObject(IHashString *parentName, IHashString *name, bool bAddToHier);
	/// Default Destructor
	~CDetailObject();

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
	Vec3& GetRotation( void );
	/// Sets the Rotation Vector (Euler Angles)
	/// \param - Rotation Vector( Yaw, Pitch, Roll )
	void SetRotation( Vec3& vRotation );

	/// MESSAGES
	DWORD OnGetGlobalPosition( DWORD size, void *params );
	DWORD OnSetGlobalPosition( DWORD size, void *params );

private:
	void GenerateVertices( void );
	void InitializeTextures( void );

	IToolBox		*m_ToolBox;
	IRenderer		*m_IRenderer;
	IController		*m_GeometryController;
	IGeometry		*m_Geometry;
	IBaseTextureObject *m_Texture;
	/// name of layer that this detail uses as a mask
	CHashString		m_LayerLink;
	/// the model (.cfg) file to use for this detail object
	CHashString		m_ModelName;
	/// coverage in x direction, (1.0f / # steps across mask in X)
	float			m_XCoverage;
	/// coverage in y direction, (1.0f / # steps across mask in Y)
	float			m_YCoverage;
	/// percentage of randomly visible in X Direction of mask.
	/// 1.0 = all randomly visible,
	/// .5 = 50% are randomly visible/placed
	float			m_XRandomness;
	/// percentage of randomly visible in X Direction of mask.
	/// 1.0 = all randomly visible,
	/// .5 = 50% are randomly visible/placed
	float			m_YRandomness;
	/// low random scale value
	/// setting this and m_MaxScale will randomly choose scale values
	///	between the two settings
	float			m_MinScale;
	/// high random scale value
	float			m_MaxScale;
	/// minimum yaw value for object
	/// setting this and m_MaxYaw will randomly choose yaw values
	// between the two settings -1.0 to 1.0 is -360.0 to 360.0 degrees
	float			m_MinYaw;
	/// high random yaw value
	float			m_MaxYaw;

	CDetailObjectShaderCallback	*m_DetailObjectShaderCallback;
	StdString					m_ShaderFile;

	/// Global allocation for vertex and index buffers
	BUFFERALLOCATIONSTRUCT	m_IndexBufferAllocation;
	BUFFERALLOCATIONSTRUCT	m_VertexBufferAllocation;
	IIndexBuffer			*m_IndexBufferInterface;
	IVertexBufferObject		*m_VertexBufferInterface;

	/// Class Type
	CHashString				m_hszNameType;

	bool m_bAddToHierarchy;
};

#endif //__DETAILOBJECT_H__

