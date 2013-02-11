///============================================================================
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

#ifndef LIGHTMAPGENERATOR_H
#define LIGHTMAPGENERATOR_H

struct PhotonValue
{
	//point of intersection is already stored as the key to the kd tree
	floatColor	intensity;
	int		bounce;
	Vec3 direction;
};

struct PotentialIntersection
{
	int faceIndex;
	CHashString mesh;
	double t, u, v;
	Ray  transformedRay;//modelspace
};

typedef multimap< float, PotentialIntersection > POTENTIAL_INTERSECTION_SORT;
typedef pair< float, PotentialIntersection > POTENTIAL_INTERSECTION_SORT_PAIR;

namespace ElementalEngine
{

class LightMapGenerator : public CObjectTemplate< IObject>
{
private:
	/// \brief The Default Constructor for LightMapGenerator
	LightMapGenerator(IHashString *parentName, IHashString *name );

public:
	/// \brief	The Default Destructor for LightMapObject
	~LightMapGenerator( );

	/// \brief	Create the a instance of IObject
	/// \return	IComponent pointer
	static IComponent *Create(int nArgs, va_list argptr);

	/// \brief	The Initialization for the Object
	/// \param	IManager* pManager: Parent Manager
	virtual void Init();
	/// \brief	The Update Function for the Object
	/// \return	True on sucess, False on faulure
	virtual bool Update();
	/// \brief	The DeInit Function for the Object
	virtual void DeInit();
	
	/// \brief either reads or writes the entity information to a file
	/// Currently Loads up the entity completely if reading
	virtual void Serialize(IArchive &ar);

	virtual bool IsKindOf( IHashString * compType );
public:
    DWORD OnAddMesh( DWORD size, void * params )
	{
		MESHTEXTUREPAIRMSG * msg;
		VERIFY_MESSAGE_SIZE( size, sizeof( MESHTEXTUREPAIRMSG ) );
		msg = (MESHTEXTUREPAIRMSG*)params;
		if( !msg ||
			msg->meshname == NULL )
		{
			return MSG_ERROR;
		}
		CHashString meshHash( msg->meshname->GetString() );
		ITextureObject * texture = msg->texture;
		m_MeshObjects.push_back( meshHash );
		m_TextureObjects.push_back( texture );
		return MSG_HANDLED;
	}
	DWORD OnAddLight( DWORD size, void * params )
	{
		ILightObject * light;
		VERIFY_MESSAGE_SIZE( size, sizeof( ILightObject ) );
		light = (ILightObject*)params;
		if( !light )
		{
			return MSG_ERROR;
		}
		if( light->GetLightType() != AMBIENT_LIGHT )
		{
			m_Lights.push_back( light );
		}
		return MSG_HANDLED;
	}
	DWORD OnCalculateLightmaps( DWORD size, void * params );
private:
	void WriteTextureDataFromPhotonMap();
	void CalculateSingleLight( ILightObject * l );
	bool ComputeRay( Ray &vRay, ILightObject * l, int bounces, float energy, int curbounce, floatColor &color );	
	bool ComputeIrradianceAtPoint2( Vec3 &tcoord,  Vec3 &normal, float &maxLightIntensity, floatColor &outIndirect );
	bool ComputeDirectLightAtPoint( Vec3 &tcoord, Vec3 &normal, floatColor &outDirect,
													ILightObject * light, floatColor &color  );
	void IntersectWithWorld( Ray &vRay, POTENTIAL_INTERSECTION_SORT &sortedOut );
	bool CastIndirect( Ray &vRayOut, Vec3 &worldSpaceNormals );
	bool CastDiffuseRay( Ray &vRayOut, Vec3 &worldSpaceNormals );
	bool CastSpecularRay( Ray &vRayOut, Vec3 &worldSpaceNormals );
	void SetFogColor( float r, float g, float b, float DistanceStart, float DistanceEnd )
	{
		m_FogColor.r = r;
		m_FogColor.g = g;
		m_FogColor.b = b;
		m_FogStart = DistanceStart;
		m_FogEnd = DistanceEnd;
	}
	//Optimize with oct tree
	vector< CHashString > m_MeshObjects;
	vector< ITextureObject * > m_TextureObjects;
	vector< ILightObject * > m_Lights;
	int m_KDCounter;
	floatColor m_FogColor;
	float m_FogStart;
	float m_FogEnd;
	CPhotonMap	* m_PhotonMap;
	IToolBox * m_ToolBox;
public:
	//Generation Parameters:
	static	int NUMPHOTONS;
	static	int MAX_BOUNCE_TEST;
	static	float PHOTON_DIRECT_RANGE_MAX;
	static	float PHOTON_INDIRECT_RANGE_MAX;
	static	float PHOTON_WEIGHT_RANGE;
	static	float PHOTON_RANGE_EXTENSION;

	static	int PHOTON_SAMPLES;
	static	float PHOTON_WEIGHT_RANGE_MAX;
	static	float SHADOW_EPSILON;
	static	int   SPECULAR_CHANCE;
	static	float PHOTON_MAX_DISK_ITERATION;
	static	float TRIANGLE_NORMAL_EPSILON;	//epsilon for averaged normals, making sure we're still lighting if a vertex is somehow
																		//different from the averaged normal in a way that makes it lit when the average normal isn't lit
	static	float ENERGY_CUTOFF	;
	static	int NUMBOUNCES;
	static	float NORMAL_CUTOFF; //anything with dissimilar normals does not get affected, should be > 90 degrees
	static	float DOT_SHADOW_ANGLE_CUTOFF;

	static	float energy_loss;

};

} //namespace EE
#endif