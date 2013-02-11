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

#include "stdafx.h"
#include <math.h>


int	 LightMapGenerator::NUMPHOTONS					=		3000;
int LightMapGenerator::MAX_BOUNCE_TEST			=		300;
float LightMapGenerator::PHOTON_DIRECT_RANGE_MAX	=		30.f;
float LightMapGenerator::PHOTON_INDIRECT_RANGE_MAX	=		50.f;
float LightMapGenerator::PHOTON_WEIGHT_RANGE		=		30.f;
float LightMapGenerator::PHOTON_RANGE_EXTENSION		=		20.f;

int LightMapGenerator::PHOTON_SAMPLES				=		10;
float LightMapGenerator::PHOTON_WEIGHT_RANGE_MAX	=		PHOTON_INDIRECT_RANGE_MAX;
float LightMapGenerator::SHADOW_EPSILON				=		40.1f;
int LightMapGenerator::SPECULAR_CHANCE			=		40;
float LightMapGenerator::PHOTON_MAX_DISK_ITERATION	=		3;
float LightMapGenerator::TRIANGLE_NORMAL_EPSILON	=		-.1f;	//epsilon for averaged normals, making sure we're still lighting if a vertex is somehow
																	//different from the averaged normal in a way that makes it lit when the average normal isn't lit
float LightMapGenerator::ENERGY_CUTOFF				=		.1f;
int LightMapGenerator::NUMBOUNCES					=		6;
float LightMapGenerator::NORMAL_CUTOFF				=		.86f; //anything with dissimilar normals does not get affected, should be > 90 degrees
float LightMapGenerator::DOT_SHADOW_ANGLE_CUTOFF	=		0.4f;

float LightMapGenerator::energy_loss				=		1.0f;

#define NUM_GAUSSIAN_WEIGHTS 8
float Gaussian[ NUM_GAUSSIAN_WEIGHTS ] = { 0.5074f, 0.2185f, 0.0821f, 0.0461f, 0.0262f, 0.0162f, 0.0102f, 0.0052f };


REGISTER_COMPONENT(LightMapGenerator);

REGISTER_MESSAGE_HANDLER(AddMesh, OnAddMesh, LightMapGenerator);
REGISTER_MESSAGE_HANDLER(AddLight, OnAddLight, LightMapGenerator);
REGISTER_MESSAGE_HANDLER(CalculateLightmaps, OnCalculateLightmaps, LightMapGenerator);

///
/// Constructor / Destructor
///

LightMapGenerator::LightMapGenerator( IHashString *parentName, IHashString *name ) : 
	OBJECTTEMPLATE(LightMapManager, LightMapGenerator, IObject, parentName, name)
{
	m_KDCounter = 0;
	m_FogColor.r = .9f;
	m_FogColor.g = .9f;
	m_FogColor.b = 1;
	m_FogStart = 3000;
	m_FogEnd = 9000;
	DeInit();
	m_ToolBox = EngineGetToolBox();
}

LightMapGenerator::~LightMapGenerator()
{
	DeInit();	
}

/// \brief The Initialization for the Object
/// \return pointer to an IComponent
IComponent *LightMapGenerator::Create(int nArgs, va_list argptr)
{
	IHashString *name;
	IHashString *parentName;
	IObject *self;

	name = va_arg(argptr, IHashString *);
	parentName = va_arg(argptr, IHashString *);
	// create the object however you must create it
	try
	{
		// try and create an instance
		if (nArgs == 2)
		{
			self = new LightMapGenerator(parentName, name);
		}
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

void LightMapGenerator::Init()
{

}

void LightMapGenerator::Serialize(IArchive &ar)
{
	
}

/// \brief Update the object
/// \return true or false
bool LightMapGenerator::Update()
{	
	return true;
}

void LightMapGenerator::DeInit()
{
	
}

bool LightMapGenerator::IsKindOf( IHashString * compType )
{	
	static CHashString  CCal3dROCOMP(_T("MeshParameterization") );
	
	return (compType->GetUniqueID() == CCal3dROCOMP.GetUniqueID());
}


DWORD LightMapGenerator::OnCalculateLightmaps( DWORD size, void * params )
{
	srand( 100 );
	//first 2 factor is for specular and diffuse channels per bounce
	int npoints = 2*(NUMBOUNCES + 1 )*NUMPHOTONS*(int)m_Lights.size();
	int ndim = 3;
	//m_KDBuffer = new Vec3[npoints];
	//m_KDValue = new PhotonValue[npoints];
	//m_KDTree = new KDTree;
	m_KDCounter = 0;
	//Build photon map:
	m_PhotonMap = new CPhotonMap();
    //find light sources, for each light, calculate
	for( int i = 0; i< (int)m_Lights.size(); i++ )
	{
		CalculateSingleLight( m_Lights[ i ] );		
	}
	OutputDebugString( "Creating KD Tree for final Gather\n");
	if( m_KDCounter > 0 )
	{
		m_PhotonMap->balance();
		//m_KDTree->create( (float*)m_KDBuffer, m_KDCounter, ndim );
		OutputDebugString( "Gathering Final Data\n");
		//go through triangle textures and write data out
		WriteTextureDataFromPhotonMap();
	}
	
	if( m_PhotonMap )
	{ 
		delete m_PhotonMap; 
		m_PhotonMap = NULL;
	}
	//if( m_KDTree ){ delete m_KDTree; m_KDTree = NULL;} //temporarily disable since STL deletion is taking up a load of time
	return MSG_HANDLED;
}

void LightMapGenerator::WriteTextureDataFromPhotonMap()
{	
	// Set pointers for the array of ranges
	static Matrix3x3 matRot;
	
	CHashString meshType(_T("MeshParameterization") );
	char buf[1024];
	static int num = 0;
	static CHashString h(_T("none"));
	static Vec3 last(0,0,0);
	int imin = 33;
	int imax = 36;//m_MeshObjects.size();
	for( int i = imin; i < imax; i++ )
	{
		CHashString &meshName = m_MeshObjects[ i ];		
		GETPARAMETERIZEDTRIANGLESMSG meshFaces;
		GETPARAMETERIZEDVERTICESMSG meshVertices;
		Matrix4x4 meshTransform;
		Matrix4x4 meshInverseTransform;
		static DWORD msgHash_GetMeshTransform = CHashString(_T("GetMeshTransform")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GetMeshTransform, sizeof( Matrix4x4 ), &meshTransform, &meshName, &meshType );
		static DWORD msgHash_GetMeshInverseTransform = CHashString(_T("GetMeshInverseTransform")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GetMeshInverseTransform, sizeof( Matrix4x4 ), &meshInverseTransform, &meshName, &meshType );
		matRot.SetFrom4x4( meshTransform.GetMatrix() );
		DWORD meshTextureSize;
		static DWORD msgHash_OnGetTextureSize = CHashString(_T("OnGetTextureSize")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_OnGetTextureSize, sizeof( meshTextureSize ), &meshTextureSize, &meshName, &meshType );
		static DWORD msgHash_GetTriangleFaces = CHashString(_T("GetTriangleFaces")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GetTriangleFaces, sizeof( GETPARAMETERIZEDTRIANGLESMSG), &meshFaces, &meshName, &meshType );
		if( meshFaces.outList == NULL ||
			meshVertices.outList == NULL )
		{
			continue;
		}
		int sizeTexture = meshTextureSize*meshTextureSize;
		for( int j = 0; j < (int)meshFaces.outList->size(); j++ )
		{
			if( j%100 == 0 )
			{					
				sprintf( buf, "Gathering data for mesh#%d triangles %d-%d\n", i, j/100, j/100+100 );
				OutputDebugString( buf );
			}
			//TODO:
			/*triMapping = &((*meshFaces.outList)[ j ].m_Pixels);
			floatColor * buffer = m->GetTextureBuffer();
			floatColor * indirectLightBuffer = m->GetIndirectLightBuffer();
			for( int k = 0; k < (int)triMapping->size(); k++ )
			{
				TriangleTextureMapping &triMap = (*triMapping)[ k ];
				int index = triMap.v*m->GetTextureSize() + triMap.u;
				if( index >= 0 && 
					index < sizeTexture  )
				{
					int indexOut = -1;
					float usedRange = PHOTON_WEIGHT_RANGE;
					//check if a closest point is within range, if not, we have to extend range
					Vec3 transformed = (*m->m_Transform)*triMap.localSpaceCoord;
					//Vec3 transformed = triMap.localSpaceCoord;
					num++;
					Vec3 transformedNormal = matRot*triMap.localNormal;
					transformedNormal.Normalize();

					float accumMaxLightIntensity = 0;
					static int skiptest = 0;
					skiptest++;
					//Direct pass
					if( buffer[ index ].a < 0 
					//	&& skiptest%100==0
						)
					{
						for( int a = 0; a< (int)m_Lights.size(); a++ )
						{
							static floatColor lightColor;
							static float fcolor[4];
							ILightObject * light = m_Lights[ a ];
							if( light )
							{
								light->GetColorIntensity(fcolor);
								lightColor.a = fcolor[3];
								lightColor.r = 1.f;//fcolor[0];
								lightColor.g = 1.f;//fcolor[1];
								lightColor.b = 1.f;//fcolor[2];
								if( ComputeDirectLightAtPoint( transformed,  transformedNormal, 
											buffer[ index ], light , lightColor ) )
								{
									accumMaxLightIntensity += lightColor.a;
								}
							}
						}
					}
					if( accumMaxLightIntensity <= 0 )
					{
						accumMaxLightIntensity = 1;
					}

					ComputeIrradianceAtPoint2( transformed, transformedNormal, accumMaxLightIntensity,
						indirectLightBuffer[ index ] );
#if 0
					
					if( (num % 40) == 0 )
					{
						ADDLINEPARAMS LineParam;
						LineParam.name = &h;
						//LineParam.start = last;
						//LineParam.end = transformed;
						LineParam.start = transformed + transformedNormal*40;
						LineParam.end = transformed;
						LineParam.blue = 0;
						LineParam.green = 0;
						last = transformed;
						static DWORD msgHash_AddLine = CHashString(_T("AddLine")).GetUniqueID();
						m_ToolBox->SendMessage(msgHash_AddLine,sizeof(LineParam), &LineParam );
					}
#endif
				}						
			}*/
		}
	}	
}

bool LightMapGenerator::ComputeDirectLightAtPoint( Vec3 &tcoord, Vec3 &normal, floatColor &outDirect,
													ILightObject * light, floatColor &color)
{
	POTENTIAL_INTERSECTION_SORT sortedIntersections;
	Vec3 RayOrigin;
	Vec3 Normals[ 3 ];
	float attenuationDistance = light->GetAttenuationDistance();
	light->GetPosition( RayOrigin );
	Ray castRay;
	double u, v, t;
	static CHashString meshType(_T("MeshParameterization") );
	castRay.m_Origin = Vec3( (float)RayOrigin[0], (float)RayOrigin[1], (float)RayOrigin[2] );
	castRay.m_Direction = tcoord - castRay.m_Origin;
	float distanceFromLight = castRay.m_Direction.Length();
	castRay.m_Direction.Normalize();
	if( distanceFromLight < attenuationDistance )
	{
		IntersectWithWorld( castRay, sortedIntersections);
	}
	if( sortedIntersections.size() > 0 )
	{
		POTENTIAL_INTERSECTION_SORT::iterator iter = sortedIntersections.begin();
		PotentialIntersection &firstIntersection = iter->second;
		if( firstIntersection.t < (distanceFromLight - SHADOW_EPSILON ) 
			|| firstIntersection.t > (distanceFromLight + SHADOW_EPSILON ) )//shadow
		{
			outDirect.a = 1;
			//outDirect.r = 0;
			//outDirect.g = 0;
			//outDirect.b = 1;		
			return false;
		}
        u = firstIntersection.u;
		v = firstIntersection.v;
		t = firstIntersection.t;
		int face = firstIntersection.faceIndex;
		CHashString &meshName = firstIntersection.mesh;
		GETPARAMETERIZEDTRIANGLESMSG meshFaces;
		GETPARAMETERIZEDVERTICESMSG meshVertices;
		static DWORD msgHash_GetTriangleFaces = CHashString(_T("GetTriangleFaces")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GetTriangleFaces, sizeof( GETPARAMETERIZEDTRIANGLESMSG), &meshFaces, &meshName, &meshType );
		static DWORD msgHash_GetCollapsedMesh = CHashString(_T("GetCollapsedMesh")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GetCollapsedMesh, sizeof( GETPARAMETERIZEDVERTICESMSG), &meshVertices, &meshName, &meshType );
		if( meshFaces.outList == NULL ||
			meshVertices.outList == NULL )
		{
			return false;
		}
		Ray rRay = castRay;
		TriangleFace &tri = (*meshFaces.outList)[ face ];
		//we intersected, find point of intersection, find texel it maps to, color
		for( int a = 0; a < 3; a++ )
		{	
			Normals[ a ] = (*meshVertices.outList)[ tri.index [ a ] ].normal;
		}

		Normals[ 0 ] = normal;
		Normals[ 0 ].Normalize();
		//now take the dot with light ray
		float value = Normals[ 0 ].Dot( -rRay.m_Direction );

		//write to KD Tree
		//attenuate value based on distance
		float intensity = 1.f - ((float)t / attenuationDistance);
		if( intensity > 1.f )
		{
			intensity = 1.f;
		}
		else
			if( intensity < 0 )
		{
			intensity = 0;
		}
		value *= intensity;
		if( value <= 0.f )
		{
			outDirect.a = 1;
			//outDirect.r = 1;
			//outDirect.g = 0;
			//outDirect.b = 0;	
			return false;
		}
		else if( value > 1.f )
		{
			value = 1.f;
		}
		//linearly interpolate the color based on distance
		floatColor OutColor;
		float fogFactor = ( (float)t - m_FogStart ) / ( m_FogEnd - m_FogStart );
		if( fogFactor < 0 ) 
		{
			fogFactor = 0.f;
		}
		else
			if( fogFactor > 1.f )
		{
			fogFactor = 1.f;
		}
		OutColor.r = m_FogColor.r*fogFactor + color.r*(1.f - fogFactor );
		OutColor.g = m_FogColor.g*fogFactor + color.g*(1.f - fogFactor );
		OutColor.b = m_FogColor.b*fogFactor + color.b*(1.f - fogFactor );
			
		value *= color.a; //light intensity

		if( outDirect.a < 0 )
		{
			outDirect.a = 1;
			outDirect.r = value*OutColor.r;
			outDirect.g = value*OutColor.g;
			outDirect.b = value*OutColor.b;
		}else
		{
			outDirect.r += value*OutColor.r;
			outDirect.g += value*OutColor.g;
			outDirect.b += value*OutColor.b;
		}
		return true;
	}
	else
	{
		//DEBUG green for missed intersections
		//outDirect.a = 1.f;
		//outDirect.r = 0.f;
		//outDirect.g = 1.f;
		//outDirect.b = 0.f;
	}
	return false;
}


bool LightMapGenerator::ComputeIrradianceAtPoint2( Vec3 &tcoord, Vec3 &normal, float &maxLightIntensity, 
													floatColor &outIndirect )
{
	outIndirect.a = 1.f;
	Vec3 colorOut;
	m_PhotonMap->lookup( colorOut, tcoord, normal, PHOTON_SAMPLES );
	outIndirect.r = colorOut.x;
	outIndirect.g = colorOut.y;
	outIndirect.b = colorOut.z;
#if 0
	//DEBUG CODE for drawing lines where the normals are and the reflection rays
	static int num = 0;
	num++;
	if( (num % 10) == 0 )
	{
		ADDLINEPARAMS LineParam;
		LineParam.name = &h;
		LineParam.start = tcoord + normal*50.f;
		LineParam.end = tcoord;
		LineParam.red = 0;
		LineParam.blue = 0;
		LineParam.green = 255;
		static DWORD msgHash_AddLine = CHashString(_T("AddLine")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_AddLine,sizeof(LineParam), &LineParam );
	}
#endif
	return true;
}

void RandomRay( Vec3 &ray )
{
	ray.x = ((float)rand()/(float)RAND_MAX) - .5f;
	ray.y = ((float)rand()/(float)RAND_MAX) - .5f;
	/*ray.z = sqrtf( 1 - ray.x*ray.x - ray.y*ray.y );
	if( rand()%2 == 0 )
	{
		ray.z = -ray.z;
	}*/
	ray.z = ((float)rand()/(float)RAND_MAX) - .5f;
	//ray.x = sin( ((float)rand())*.0001f );
	//ray.y = sin( ((float)rand())*.0001f );
	//ray.z = sin( ((float)rand())*.0001f );
	ray.Normalize();
}

float g_Gaussian[ 5 ][ 5 ] =
{
	{ 0.1f, 0.20f, 0.40f, 0.20f, 0.1f },
	{ 0.2f, 0.50f, 0.75f, 0.50f, 0.2f },
	{ 0.40f, 0.75f, 1.0f, 0.75f, 0.40f },
	{ 0.30f, 0.50f, 0.75f, 0.50f, 0.2f },
	{ 0.1f, 0.20f, 0.40f, 0.20f, 0.1f },
};

void LightMapGenerator::CalculateSingleLight( ILightObject * l )
{
	int rayCasts = NUMPHOTONS;
	Vec3 RayOrigin;
	l->GetPosition( RayOrigin );
	Ray castRay;
	castRay.m_Origin = RayOrigin;
	int bounces = NUMBOUNCES;
	char buf[1024];
	floatColor lightColor;
	float fcolor[4];
	l->GetColorIntensity(fcolor);
	lightColor.a = fcolor[3];
	lightColor.r = fcolor[0];
	lightColor.g = fcolor[1];
	lightColor.b = fcolor[2];
	for( int i = 0; i < rayCasts; )
	{
		//random raycast
		RandomRay( castRay.m_Direction );
		if( ComputeRay( castRay, l, bounces, l->GetIntensity(), bounces, lightColor ) )
		{
			i++;
			if( i%1000 == 0 )
			{
				sprintf( buf, "Calculating Photon %dx1000\n", i/1000 );
				OutputDebugString( buf );
			}
		}
	}
	OutputDebugString( "DAMN IT\n" );
}

extern void OutputVector( Vec3 &vec, char * name );
extern void OutputVector(float array[3], char * name );

void LightMapGenerator::IntersectWithWorld( Ray &vRay, POTENTIAL_INTERSECTION_SORT &sortedIntersections )
{
	float rayTmin, rayTMax;
	Vec3 LightOrigin;	
	double t, u, v;
	Ray rRay;
	static CHashString meshType(_T("MeshParameterization") );
	for( int j = 0; j < (int)m_MeshObjects.size(); j++ )
	{
		//now check each mesh's triangles
		CHashString &meshName= m_MeshObjects[ j ];
		Matrix4x4 meshTransform;
		Matrix4x4 meshInverseTransform;
		static DWORD msgHash_GetMeshTransform = CHashString(_T("GetMeshTransform")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GetMeshTransform, sizeof( Matrix4x4 ), &meshTransform, &meshName, &meshType );
		static DWORD msgHash_GetMeshInverseTransform = CHashString(_T("GetMeshInverseTransform")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GetMeshInverseTransform, sizeof( Matrix4x4 ), &meshInverseTransform, &meshName, &meshType );
		
		AABB meshBounds;
		static DWORD msgHash_GetAABB = CHashString(_T("GetAABB")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GetAABB, sizeof( AABB ), &meshBounds, &meshName, &meshType );
		
		//transform this by inverse matrix
		LightOrigin = meshInverseTransform*vRay.m_Origin;
		Matrix3x3 matRotate;
		matRotate.SetFrom4x4( meshInverseTransform.GetMatrix() );
		Vec3 transformedDir = matRotate*vRay.m_Direction;
		transformedDir.Normalize();
		rRay = Ray( LightOrigin, transformedDir );
		
		if( meshBounds.IntersectRay( rRay, rayTmin, rayTMax ) )
		{
			//cull mesh away that need not be tested
			int face = 0;
			//test intersection
			MESHPARAMINTERSECTRAYTRIANGLEMSG intersectMsg;
			intersectMsg.inRay = &rRay;
			static DWORD msgHash_IntersectRayTriangle = CHashString(_T("IntersectRayTriangle")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_IntersectRayTriangle, sizeof( MESHPARAMINTERSECTRAYTRIANGLEMSG ), &intersectMsg, &meshName, &meshType );
		
			if( intersectMsg.outCollided == true)
			{
				face = intersectMsg.outFaceIndex;
				t = intersectMsg.outIntersectionDistance;
				u = intersectMsg.outULength;
				v = intersectMsg.outVLength;
				if( t < 0 )
				{
					continue;
				}
				PotentialIntersection pIntersection;
				pIntersection.faceIndex = face;
				pIntersection.t = t;
				pIntersection.u = u;
				pIntersection.v = v;
				pIntersection.mesh = meshName;
				pIntersection.transformedRay = rRay;
				sortedIntersections.insert( POTENTIAL_INTERSECTION_SORT_PAIR( (float)t, pIntersection ) );
			}
		}
		
	}
}

bool LightMapGenerator::ComputeRay( Ray &vRay, ILightObject * l, int bounces, float energy, int curbounce,
								   floatColor &color)
{
	POTENTIAL_INTERSECTION_SORT sortedIntersections;
	if( curbounce < 0 )return false;
	if( energy <= ENERGY_CUTOFF )return false;

	Vec3 triVerts[3];
	Vec3 Normals[ 3 ];
	float meshU[ 3 ];
	float meshV[ 3 ];
	double t, u, v;
	Ray rRay;
	Vec3 LightOrigin;
	float attenuationDistance = l->GetAttenuationDistance();
	//add to lightposition
	//check meshes
	IntersectWithWorld( vRay, sortedIntersections );

	if( sortedIntersections.size() > 0 )
	{
		POTENTIAL_INTERSECTION_SORT::iterator iter = sortedIntersections.begin();
		PotentialIntersection &firstIntersection = iter->second;
		u = firstIntersection.u;
		v = firstIntersection.v;
		t = firstIntersection.t;
		int face = firstIntersection.faceIndex;
		rRay = firstIntersection.transformedRay;

		static CHashString meshType(_T("MeshParameterization") );
		CHashString &meshName = firstIntersection.mesh;
		GETPARAMETERIZEDTRIANGLESMSG meshFaces;
		GETPARAMETERIZEDVERTICESMSG meshVertices;
		static DWORD msgHash_GetTriangleFaces = CHashString(_T("GetTriangleFaces")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GetTriangleFaces, sizeof( GETPARAMETERIZEDTRIANGLESMSG), &meshFaces, &meshName, &meshType );
		static DWORD msgHash_GetCollapsedMesh = CHashString(_T("GetCollapsedMesh")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GetCollapsedMesh, sizeof( GETPARAMETERIZEDVERTICESMSG), &meshVertices, &meshName, &meshType );
		if( meshFaces.outList == NULL ||
			meshVertices.outList == NULL )
		{
			return false;
		}

		TriangleFace &tri = (*meshFaces.outList)[ face ];
		for( int a = 0; a < 3; a++ )
		{
			Vec3 &v = (* meshVertices.outList)[ tri.index [ a ] ].originalPosition;
			triVerts[ a ].x = v.x;
   			triVerts[ a ].y = v.y;
   			triVerts[ a ].z = v.z;
		}
		//we intersected, find point of intersection, find texel it maps to, color
		for( int a = 0; a < 3; a++ )
		{	
			meshU[ a ] = (* meshVertices.outList)[ tri.index [ a ] ].generatedU;
			meshV[ a ] = (* meshVertices.outList)[ tri.index [ a ] ].generatedV;
			Normals[ a ] = (* meshVertices.outList)[ tri.index [ a ] ].normal;
		}
		Vec3 intersect;
		intersect.x = (float)(rRay.m_Origin.x + rRay.m_Direction.x*t);
		intersect.y = (float)(rRay.m_Origin.y + rRay.m_Direction.y*t);
		intersect.z = (float)(rRay.m_Origin.z + rRay.m_Direction.z*t);
		//calculate average normal
		Normals[ 0 ] = 
			Normals[ 0 ]*(1 -  (float)u)
			+ Normals[ 0 ]*(1 -  (float)v) 
			+ Normals[1]* (float)u 
			+ Normals[2]* (float)v;
		Normals[ 0 ].Normalize();
		//now take the dot with light ray
		float value = Normals[ 0 ].Dot( -rRay.m_Direction );

		//write to KD Tree
		//attenuate value based on distance
		if( curbounce < bounces  )
		{
			attenuationDistance = 4000; //hack attenuate to prevent artifacts for now
		}
		float intensity = 1.f - ((float)t / attenuationDistance);
		if( intensity > 1.f )
		{
			intensity = 1.f;
		}
		else
			if( intensity < 0 )
		{
			intensity = 0;
		}
		value *= intensity*energy;
		if( value <= 0.f )
		{
			value = 0.f;
			return false;
		}

		//linearly interpolate the color based on distance
		floatColor OutColor;
		float fogFactor = ( (float)t - m_FogStart ) / ( m_FogEnd - m_FogStart );
		if( fogFactor < 0 ) 
		{
			fogFactor = 0.f;
		}
		else
			if( fogFactor > 1.f )
		{
			fogFactor = 1.f;
		}
		OutColor.r = m_FogColor.r*fogFactor + color.r*(1.f - fogFactor );
		OutColor.g = m_FogColor.g*fogFactor + color.g*(1.f - fogFactor );
		OutColor.b = m_FogColor.b*fogFactor + color.b*(1.f - fogFactor );
		
		Matrix4x4 meshTransform;
		static DWORD msgHash_GetMeshTransform = CHashString(_T("GetMeshTransform")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GetMeshTransform, sizeof( Matrix4x4 ), &meshTransform, &meshName, &meshType );
		
		intersect = meshTransform*intersect;		
		//Only need rotation component for normal
		Matrix3x3 matRotate;
		matRotate.SetFrom4x4( meshTransform.GetMatrix() );
			
		Vec3 vWorldSpaceNormals = matRotate*Normals[ 0 ];
		vWorldSpaceNormals.Normalize();

		if( curbounce < bounces  )
		{
			Vec3 color( value*OutColor.r, value*OutColor.g, value*OutColor.b );
			m_PhotonMap->store( intersect, vWorldSpaceNormals, vRay.m_Direction, color ); 
			//incoming direction must be stored as well
			m_KDCounter++;
		}
#if 0
		//DEBUG code to output bounce rays
		static int num2 = 0;
		num2++;
		if( curbounce < bounces  )
		{
			static CHashString h(_T("linename"));
			ADDLINEPARAMS LineParam;
			LineParam.name = &h;
			LineParam.start = vRay.m_Origin;
			LineParam.end = vRay.m_Origin + vRay.m_Direction*50;//intersect;
			LineParam.green = 255;
			LineParam.blue = 0;
			LineParam.red = 0;
			static DWORD msgHash_AddLine = CHashString(_T("AddLine")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_AddLine,sizeof(LineParam), &LineParam );
			LineParam.start = intersect;
			LineParam.end = intersect - vRay.m_Direction*50;//intersect;
			LineParam.blue = 0;
			LineParam.green = 0;
			LineParam.red = 255;
			static DWORD msgHash_AddLine = CHashString(_T("AddLine")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_AddLine,sizeof(LineParam), &LineParam );
		}
#endif
		//Get bounce color		
		float finalU = (float)( meshU[0]*( 1 - u - v ) + meshU[1]*u + meshU[2]*v);
 		float finalV = (float)( meshV[0]*( 1 - u - v ) + meshV[1]*u + meshV[2]*v);
		floatColor bounceColor;
		MESHPARAMGETBASECOLORATTRIANGLEINTERSECTIONMSG getColorMsg;
		getColorMsg.inFace = face;
		getColorMsg.inUBaryCentric = finalU;
		getColorMsg.inVBaryCentric = finalV;
		static DWORD msgHash_GetBaseColorAtTriangleIntersection = CHashString(_T("GetBaseColorAtTriangleIntersection")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GetBaseColorAtTriangleIntersection, sizeof( MESHPARAMGETBASECOLORATTRIANGLEINTERSECTIONMSG), &getColorMsg, &meshName, &meshType );
		bounceColor = getColorMsg.outColor;
		bounceColor.a *= OutColor.a;
		bounceColor.r *= OutColor.r;
		bounceColor.g *= OutColor.g;
		bounceColor.b *= OutColor.b;		
		//bounceColor = OutColor;
		//if( bounceColor.a >= 0)x
		{
			//Calculate bounces
			Ray rNewRay;
			rNewRay.m_Origin = intersect;
			float currentIntensity = value*energy_loss;
			if(	value > ENERGY_CUTOFF &&
				curbounce > 0 &&
				rand()%SPECULAR_CHANCE == 0)
			{
				CastSpecularRay( rNewRay, vWorldSpaceNormals );
				ComputeRay( rNewRay, l, bounces, currentIntensity, curbounce - 1, bounceColor);
			}
			bool diffuseFound = false;
			int bounceTests = 0;
			while(	currentIntensity > ENERGY_CUTOFF &&
				curbounce > 0 &&
				!diffuseFound &&
				bounceTests < MAX_BOUNCE_TEST )
			{
				bounceTests++;
				CastDiffuseRay( rNewRay, vWorldSpaceNormals );		
				diffuseFound = ComputeRay( rNewRay, l, bounces, currentIntensity, curbounce - 1, bounceColor);				
			}
		}
		return true;
	}
	return false;
}

bool LightMapGenerator::CastIndirect( Ray &vRayInOut, Vec3 &worldSpaceNormals )
{
	if( rand()%2 == 0 )
	{
		CastDiffuseRay( vRayInOut, worldSpaceNormals );
		return false;
	}
	else
	{
		CastSpecularRay( vRayInOut, worldSpaceNormals );
		return true;	
	}	
}

bool LightMapGenerator::CastDiffuseRay( Ray &vRayInOut, Vec3 &worldSpaceNormals )
{
	RandomRay( vRayInOut.m_Direction );
	if( vRayInOut.m_Direction.Dot( worldSpaceNormals ) < 0 )
	{
		vRayInOut.m_Direction = -vRayInOut.m_Direction;
	}
	return true;
	/*Vec3 RP = vRayInOut.m_Direction - worldSpaceNormals * ( 2.0f * vRayInOut.m_Direction.Dot( worldSpaceNormals ) );
	Vec3 RN1 = Vec3( RP.z, RP.y, -RP.x );
	Vec3 RN2 = RP.Cross( RN1 );
	float specularReflection = 1.f;
	float xoffs, yoffs;
	do
	{
		xoffs = (((float)rand()/(float)RAND_MAX) - 0.5f);
		yoffs = (((float)rand()/(float)RAND_MAX) - 0.5f);
	}
	while ((xoffs * xoffs + yoffs * yoffs) > 1 );
	Vec3 newDirection = RP + (RN1 * xoffs) + (RN2 * yoffs);
	newDirection.Normalize();
	vRayInOut.m_Direction = newDirection;*/
	return true;
}

bool LightMapGenerator::CastSpecularRay( Ray &vRayInOut, Vec3 &worldSpaceNormals )
{
	//calculate reflection ray	
	Vec3 Reflection = (worldSpaceNormals*2) - vRayInOut.m_Direction;
	Reflection.Normalize();
	//simple weight distribution
	int weight = rand()%100;
	//goes to 10,000
	float multiplier = 1.f - ((float)(weight*weight)/10000.f);//most of the time it will be near 1k so near (1-1) = 0

	//add fuzzy
	Reflection.x = Reflection.x*100 + multiplier*(float)(rand()%20) + multiplier*10.f;
	Reflection.y = Reflection.y*100 + multiplier*(float)(rand()%20) + multiplier*10.f;
	Reflection.z = Reflection.z*100 + multiplier*(float)(rand()%20) + multiplier*10.f;
	Reflection.Normalize();
	vRayInOut.m_Direction = Reflection;
	return true;
}
