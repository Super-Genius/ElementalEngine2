///============================================================================
/// \file		CTerrainDecalRenderObject.h
/// \brief		Header file for a Terrain Sector Render Object
/// \date		04-24-2005
/// \author		Brian Bazyk
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

#ifndef	CTERRAINDECALRENDEROBJECT_H
#define	CTERRAINDECALRENDEROBJECT_H

#include "CRenderObject.h"

class CTerrainManager;

class CTerrainDecalRenderObject  : public CRenderObject<>
{
protected:
	IArchive* ConvertTerrainArchive( IArchive &ar );

	/// \brief Default Constructor
	/// \param parentName = name of parent in hierarchy
	/// \param name = IHashString * name of this object
	CTerrainDecalRenderObject(IHashString *parentName, IHashString *name, bool bAddToHier);

public:
	/// \briefDefault Destructor
	~CTerrainDecalRenderObject();

	/// \brief Create an instance
	/// \return	Pointer to this
	static IComponent *Create(int nArgs, va_list argptr);

	/// \brief DeInit Function
	virtual void DeInit();

	/// \brief Init Function
	virtual void Init();

	/// \brief serialize
	/// \param ar - The Archive
	virtual void Serialize(IArchive &ar);

	virtual bool Update();

	virtual OBJECTSORTTYPES GetSceneSortID() const { return RENDEROBJECT3D; }

	/// \brief Render the renderobject
	/// \return true if object was rendered, false otherwise
	virtual bool Render( UINT pass, IEffect * override );

	/// \brief Compare for sorting
	/// \param obj = object to compare to
	/// \param sortMethod = a sort method specifying how to compare(set in RENDEROBJECTCOMPARE)
	/// \param curRenderPass = the current render pass to sort to
	/// \return bool = must return true if (this pointer) < (obj)
	virtual bool IsLessThan( const IRenderObject * obj, DWORD sortMethod, int curRenderPass ) { return true; }

	/// \brief This is for render priorities between classes
	/// will change as soon as a new architecture can be designed to better incorporate
	/// different renderobject types being pushed on the same sorted list
	/// \return DWORD = the priority key this returns, the higher the number,
	/// the higher its priority
	virtual DWORD GetRenderPriority() { return 0; }

	/// \brief Number of passes this render object has to go through
	/// \return UINT = returns one pass for this implementation
	virtual UINT GetNumPass() { return 1; };

	/// \brief Sets the position of the render object
	/// \param position Vec3 class reference to fill in
	virtual void SetPosition( const Vec3 &vec );

	/// \brief Gets the position of the render object
	/// \return Vec3 = position f the object
	virtual Vec3 &GetPosition();

	/// \brief Gets bounding sphere of object. This is a 3D interface call that should be implemented for only
	///			3D objects. Might want to seperate more of this later
	/// \param position = vector that will be filled the center of the sphere
	/// \param radius = float that will be filled with the radius of the sphere
	virtual void GetBoundingSphere( Vec3 &position, float &radius );

	// ** MESSAGES ** //
	DWORD OnSetPosition( DWORD size, void* params );
	DWORD OnSetDecalTexture( DWORD size, void* params );
	DWORD OnSetDecalSize( DWORD size, void* params );
	DWORD OnSetDecalColor( DWORD size, void* params );

	// TODO: implement if necessary
	virtual IHashString * GetBaseSortClass(){ return GetComponentType(); }

	/// \brief returns if this renderobject needs to be sorted in the transparency list
	/// \return bool = true if this object uses transparency
	virtual bool IsAlphaObject(){ return true; }

	IMeshObject * GetMeshObject() { return NULL; }

	/// Gets the number of Geometry elements this renderobject possesses or if it uses geometry at all	
	virtual UINT GetNumGeometry(){ return 0; }

	/// Grabs the geometry pieces of the render object if there is any uses it for render instead
	/// of the render object render(might need to be changed)
	virtual IGeometry* GetGeometry( int index ){ return NULL; }

	virtual bool IsKindOf( IHashString * compType );

	virtual void AttachLight( ILightObject * light ) { }
	virtual void DetachAllLights() { }

	/// \brief GetBoundingBox default implementation
	/// child should extend off this
	virtual bool GetBoundingBox( Vec3 &pos, Vec3 &dim, EulerAngle &rotation ) {	return false; }
	virtual IMaterial * GetMaterial( int index ){ return NULL; }
	virtual UINT GetNumMaterial() { return 0; }
	virtual ISceneSettings * GetSceneSettings(){ return NULL; }

private:
	// Toolbox
	IRenderer *m_pRenderer;
	CTerrainManager *m_pTerrainMgr;

	// Vars
	bool m_bAddToHierarchy;
	float m_fBoundingSphereRadius;
	Matrix3x3 m_TexTransform;

	IEffect *m_pDecalEffect;
	CHashString m_szDecalTexture;
	ITextureObject *m_pDecalTexture;

	float m_fSize;
	Vec4 m_Color;

	float m_fLingerTime;
	float m_fFadeTime;	
	float m_fDecalET;

/*	bool AllocateVertices( int iVertexCount );
	bool AllocateIndices( int iIndexCount );
	BUFFERALLOCATIONSTRUCT m_VertexBufferAllocation;
	IVertexBufferObject *m_VertexBufferInterface;
	BUFFERALLOCATIONSTRUCT m_IndexBufferAllocation;
	IIndexBuffer *m_IndexBufferInterface;
*/
};

#endif
