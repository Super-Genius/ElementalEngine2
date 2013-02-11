///============================================================================
/// \file		CTerrainWaterRenderObject.h
/// \brief		Header file for a Terrain Sector Water Render Object
/// \date		07-27-2005
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

#ifndef	CTERRAINWATERRENDEROBJECT_H
#define	CTERRAINWATERRENDEROBJECT_H

#include "CRenderObject.h"

class CTerrainManager;
class CTerrainWaterRenderObject : public CRenderObject<>
{
protected:
	/// \brief Default Constructor
	/// \param parentName = name of parent in hierarchy
	/// \param name = IHashString * name of this object
	CTerrainWaterRenderObject(IHashString *parentName, IHashString *name, bool bAddToHier);

public:
	/// \briefDefault Destructor
	~CTerrainWaterRenderObject();

	/// \brief Create an instance
	/// \return	Pointer to this
	static IComponent *Create(int nArgs, va_list argptr);

	/// \brief Update Function
	/// \return	True on sucess, False on failure
	virtual bool Update();

	/// \brief DeInit Function
	virtual void DeInit();

	/// \brief Init Function
	virtual void Init();

	/// \brief serialize
	/// \param ar - The Archive
	virtual void Serialize(IArchive &ar);

	virtual OBJECTSORTTYPES GetSceneSortID() const { return RENDEROBJECT3D; }

	/// \brief Render the renderobject
	/// \return true if object was rendered, false otherwise
	virtual bool Render( UINT pass, IEffect * override );

	/// \brief This is for render priorities between classes
	/// will change as soon as a new architecture can be designed to better incorporate
	/// different renderobject types being pushed on the same sorted list
	/// \return DWORD = the priority key this returns, the higher the number,
	/// the higher its priority
	virtual DWORD GetRenderPriority();

	/// \brief Number of passes this render object has to go through
	/// \return UINT = returns one pass for this implementation
	virtual UINT GetNumPass() { return 1; };

	/// \brief Sets the position of the render object
	/// \param vec = position
	virtual void SetPosition( const Vec3 &vec );

	/// \brief Gets the position of the render object
	/// \return Vec3 = position f the object
	virtual Vec3 &GetPosition();

	/// \brief Gets bounding sphere of object. This is a 3D interface call that should be implemented for only
	///			3D objects. Might want to seperate more of this later
	/// \param position = vector that will be filled the center of the sphere
	/// \param radius = float that will be filled with the radius of the sphere
	virtual void GetBoundingSphere( Vec3 &position, float &radius );

	virtual ISceneSettings* GetSceneSettings(){ return m_SceneSettings; }

	virtual UINT GetNumGeometry(){ return 0; }
	virtual IGeometry* GetGeometry( int index ) { return (index==0) ? m_pGeometry : NULL; }
	virtual UINT GetNumMaterial( ) { return 1; }
	virtual IMaterial* GetMaterial( int index ) { return (index==0) ? m_pTerrainWaterMaterial : NULL; }

	virtual void AttachLight( ILightObject * light );
	virtual void DetachAllLights();

	// ** MESSAGES ** //
	DWORD OnTerrainSectorResetVertexData(DWORD size, void *in_params);

private:
	/// Direct access to the renderer
	IRenderer *m_pRenderer;
	/// Direct access to the terrain manager
	CTerrainManager *m_pTerrainMgr;

	IMaterial *m_pTerrainWaterMaterial;

	Vec3 m_Position;
	Vec3 m_CenterPosition;
	float m_fWaterLevel;
	int m_iTesselation;
	float m_fTransparencyDepth;
	StdString m_szShaderName;
	float m_fSize;
	float m_fBoundingSphereRadius;

	bool m_bAddToHierarchy; 

	ISceneSettings *m_SceneSettings;
	IGeometry *m_pGeometry;

	// Lighting
	ILightObject *m_Lights[MAX_LIGHTS];
	float m_LightPriorities[MAX_LIGHTS];
	int m_NumLights;
};

#endif
