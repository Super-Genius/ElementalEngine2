///============================================================================
/// \file		AmbientOcclusion.h
/// \brief		Declaration of AmbientOcclusion Editor Component
/// \date		04-09-2007
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

#ifndef _AMBIENTOCCLUSIONEDITOR_H_
#define _AMBIENTOCCLUSIONEDITOR_H_

namespace ElementalEngine {

class CAmbientOcclusionLight : public ILightObject
{
public:
	CAmbientOcclusionLight()
	{
		position.Set( 0,0,0 );
		m_fAtten = 10000.0f;
		m_fShadowAtten = 1000.0f;
	}
	virtual void SetColor( float r, float g, float b ) {}
	virtual void SetIntensity( float i ) {}
	virtual void SetAttenuationDistance( float distance ) {}
	virtual void SetVector( float x, float y, float z ) {}

	virtual void GetColorIntensity( float r[] ) { r[0] = 1.0f; r[1] = 1.0f; r[2] = 1.0f; r[3] = 1.0f; }
	virtual float GetIntensity() { return 1.0f; }
	virtual void GetVectorAttenuation( float v[] ) { v[0] = position.x; v[1] = position.y; v[2] = position.z; v[3] = m_fAtten; }
	virtual float GetAttenuationDistance() { return m_fAtten; }
	virtual void GetPosition( Vec3 &p ) { p = position; }
	virtual void GetDirection( Vec3 &p ) {p[0] = 0; p[1] = 0; p[2] = 0;}

	virtual void SetLightType( LIGHTOBJECTTYPE type ) {}
	virtual LIGHTOBJECTTYPE GetLightType( ) { return OMNI_POINT_LIGHT; }
	virtual void EnableShadows( bool enabled ) {}
	virtual bool CanCastShadows() { return true; }
	virtual float GetShadowAttenuation() { return m_fShadowAtten; }
	virtual bool IsDetailLight() { return false; }

	virtual bool GetLightPriority( const Vec3 &objectPos, float objectRadius, float &out_priority )
	{
		out_priority = 1.0f;
		return true;
	}

	// IRenderObject
	virtual bool Render( UINT pass, IEffect * override = NULL ) { return false; }
	virtual bool IsLessThan( const IRenderObject * obj, DWORD sortMethod, int curRenderPass ) { return false; }
	virtual DWORD GetRenderPriority() { return 0; }
	virtual UINT GetNumPass() { return 0; }
	virtual void SetPosition( const Vec3 &position ) {}
	virtual Vec3 &GetPosition() { return position; }
	virtual IHashString * GetBaseSortClass() { return NULL; }
	virtual bool IsAlphaObject() { return false; }
	virtual IMeshObject * GetMeshObject() { return NULL; }
	virtual bool GetBoundingBox( Vec3 &pos, Vec3 &dim, EulerAngle &rotation ) { return false; }
	virtual UINT GetNumGeometry() { return 0; }
	virtual IGeometry* GetGeometry( int index ) { return NULL; }
	virtual void AttachLight( ILightObject * light ) {}
	virtual void DetachAllLights() {}
	virtual ISceneSettings * GetSceneSettings() { return NULL; }
	virtual UINT GetNumMaterial() { return 0; }
	virtual IMaterial * GetMaterial( int index ) { return NULL; }
	// IObject
	virtual void Init() {}
	virtual bool Update() { return true; }
	virtual void DeInit() {}
	virtual IHashString *GetName() { return NULL; }
	virtual void SetName(IHashString *inName) {}
	virtual IHashString *GetParentName() { return NULL; }
	virtual void SetParentName(IHashString *inName) {}
	virtual void GetBoundingSphere( Vec3 &pos, float &radius ) { pos = position; radius = m_fAtten; }
	// IComponent
	virtual void Serialize(IArchive &ar) {}
	virtual IHashString *GetComponentType() { return NULL; }
	virtual bool IsKindOf(IHashString *compType) { return false; }

	Vec3 position;
	float m_fAtten;
	float m_fShadowAtten;
};

typedef list<CAmbientOcclusionLight> LIGHTSAMPLELIST;

typedef map< int, vector<ParameterizationVertex>* > CACHEDATLASMAP;


class CAmbientOcclusionEditor : public IComponent
{
	SINGLETONCONSTRUCTOROVERRIDE(CAmbientOcclusionEditor);
	CAmbientOcclusionEditor();

public:
	static IComponent *Create(int nArgs, va_list argptr);

	void Serialize(IArchive &ar) {}
	IHashString *GetComponentType()
	{
		return &m_hszComponentType;
	}
	bool IsKindOf(IHashString *compType)
	{
		return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
	}

	~CAmbientOcclusionEditor();
	/// generate list of light sample positions at certain radius about point
	void GenerateHemisphereLightSampleList( LIGHTSAMPLELIST &lightList, int sampleCount, Vec3 point, float radius );
	// generate a list of all objects that need ambient occlusion data
	void GenerateTargetObjectList( OBJECTLIST &targetList );
	/// generate a list of objects around a point within a specified radius
	void GenerateOccluderObjectList( OBJECTLIST &occluderList, Vec3 point, float occlusionRadius );

	bool GenerateAtlas( IHashString *pObjectName, IHashString *pParentName, int iTextureResolution );

	/*** Messages ***/
	DWORD OnGenerateAmbientOcclusion( DWORD size, void *params );
	DWORD OnGlobalUpdateTick(DWORD size, void *params);

private:
	IToolBox *m_ToolBox;
	CHashString m_hszComponentType;
	CHashString m_hszAmbientOcclusionEditorROName;
	CHashString m_hszAmbientOcclusionEditorROTypeName;

	CACHEDATLASMAP m_CachedAtlasMap;
};

}
#endif // _AMBIENTOCCLUSIONEDITOR_H_
