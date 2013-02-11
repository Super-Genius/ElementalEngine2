///============================================================================
/// \file	CGeometry.h
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

#ifndef CGEOMETRY_H
#define CGEOMETRY_H

using namespace std;

class CGeometry : public IGeometry
{
public:

	CGeometry();
	virtual ~CGeometry();

	virtual IMaterial * GetMaterial();
	virtual UINT GetIndexOffset()
	{
		assert( m_IndexBufferAllocation.m_Size != 0 );
		return m_IndexBufferAllocation.m_Offset;
	}

	/// Sets the texture of the surface to a stage name. Stage names are things like "BaseTexture", "NormalMap",
	/// and that are used by the shader
	virtual void SetMaterial( IMaterial * mat );


	/// responsible for loading the mesh into internal buffers and then allocating vertex buffer space for the mesh
	virtual bool SetMesh( IMeshObject * mesh )
	{
		return false;
	}

	virtual void Serialize( IArchive &ar )
	{	
	}

	/// --vertex buffer associations
	/// allows you to lock the vertices (delegation) of the vertex buffer directly
	virtual bool LockVertices( UINT offset, UINT size, void ** pData );

	/// get number of vertices
	virtual UINT GetNumVertices()
	{
		return m_SizeVerts;
	}

	/// allows you to resize the buffer
	virtual bool ResizeVertices( UINT size, UINT stridebytes, CHANNELDESCRIPTORLIST * VertexFormat );
	/// Index buffer associations
	virtual bool ResizeIndices( UINT size, UINT stridebytes );

	/// locks the index buffer
	virtual bool LockIndices( UINT offset, UINT size, void ** pData );
	virtual bool UnlockVertices();

	virtual bool UnlockIndices();

	/// Index buffer associations
	virtual UINT GetNumIndices()
	{
		return m_SizeIndices;
	}
	/// number of primitives
	virtual UINT GetNumPrimitives()
	{
		return m_NumPrimitives;
	}

	//Renders this surface with a shader(1 pass )
	virtual bool Render( IEffect * override );

	virtual bool DrawGeometry();

	virtual IController * GetController()
	{
		return m_Controller;
	}	

	virtual void SetController( IController * controller )
	{
		m_Controller = controller;
	}
	/// for pre allocated buffers, for example, we want to share a vertex pool
	virtual bool SetVertexBufferAllocation( BUFFERALLOCATIONSTRUCT &alloc );
	/// for pre allocated buffers, for example, we want to share a vertex pool
	virtual bool SetIndexBufferAllocation( BUFFERALLOCATIONSTRUCT &alloc );
	
	/// for pre allocated buffers, for example, we want to share a vertex pool
	virtual bool GetIndexBufferAllocation( BUFFERALLOCATIONSTRUCT &alloc )
	{
		alloc = m_IndexBufferAllocation;
		return true;		
	}
	/// for pre allocated buffers, for example, we want to share a vertex pool
	virtual bool GetVertexBufferAllocation( BUFFERALLOCATIONSTRUCT &alloc )
	{
		alloc = m_VertexBufferAllocation;
		return true;		
	}
	/// access to buffer interface
	virtual IVertexBufferObject* GetVertexBufferInterface()
	{
		return m_VertexBufferInterface;
	}
	/// access to buffer interface
	virtual IIndexBuffer* GetIndexBufferInterface()
	{
		return m_IndexBufferInterface;
	}

	virtual void SetPrimitiveType( DRAWPRIMITIVETYPE primtype )
	{
		m_PrimType = primtype;
	}
	virtual void AttachLight( ILightObject * light, UINT index )
	{
		if (light->GetLightType() == AMBIENT_LIGHT)
		{
			float fColorIntensity[4];
			light->GetColorIntensity(fColorIntensity);
			m_AmbientLightTerm[0] += fColorIntensity[0];
			m_AmbientLightTerm[1] += fColorIntensity[1];
			m_AmbientLightTerm[2] += fColorIntensity[2];
			m_AmbientLightTerm[3] += fColorIntensity[3];
		}
		else
		{
			if( light->IsDetailLight() || index >= MAX_COMPLEX_LIGHTS )				
			{
				AddLight( -1, light, m_DetailLights, m_NumDetailLights, MAX_SIMPLE_LIGHTS );
			}
			else 
			{
				AddLight( -1, light, m_Lights, m_NumLights, MAX_COMPLEX_LIGHTS );
			}
		}
	}

	/// Signals a detach of all light objects
	virtual void DetachAllLights()
	{
		ResetLightCount();
		memset( m_AmbientLightTerm, 0, sizeof(m_AmbientLightTerm) );
	}
	virtual void ResetLightCount()
	{
		m_NumLights = 0;
		m_NumDetailLights = 0;		
	}

	virtual void Release()
	{
		//is this ok..? For releasing in the correct DLL space since these are allocated
		//through the renderer, should check for ref counts
		delete this;
	}
	virtual bool Render( ISceneSettings * settings, IEffect * override );

	
	virtual void SetSceneSettings( ISceneSettings * settings )
	{ 
		m_SceneSettings = settings;
	}

	virtual ISceneSettings * GetSceneSettings()
	{
		return m_SceneSettings;
	}
	virtual void SetLightmapTexture( IBaseTextureObject* texture );
	virtual void SetLightmapUVStream( float * uvstream, int numelements );

	/// Sets a vec4 override for the material(typically for vertex / pixel shader constants)
	/// \param hsType = type of var to set (pix const, vert const, etc)
	/// \param hsName = name of var to set
	/// \param vecValue = value to set
	virtual void SetMaterialOverride(DWORD hsType, DWORD hsName, Vec4 vecValue)
	{
		if (m_MatOverride)
		{
			m_MatOverride->SetOverride(hsType, hsName, vecValue);
		}
	}

	/// Sets a string override for the material(textures, renderstates)
	/// \param hsType = type of var to set (tex, render state, etc)
	/// \param hsName = name of var to set
	/// \param szValue = text value to set
	virtual void SetMaterialOverride(DWORD hsType, DWORD hsName, StdString szValue)
	{
		if (m_MatOverride)
		{
			m_MatOverride->SetOverride(hsType, hsName, szValue);
		}
	}

	/// Resets the material override, so the geometry renders with its original material
	virtual void ClearMaterialOverride()
	{
		if (m_MatOverride)
		{
			m_MatOverride->ResetOverrides();
		}
	}

	/// Checks to see if the geometry has alpha or not
	/// \return true if alphablend enabled
	virtual bool HasAlpha();
private:

	void AddLight( int index, ILightObject * light, ILightObject ** array, int &numLights, int maxlights )
	{
		if( index == -1 )
		{
			if( numLights < maxlights )
			{
				array[ numLights ] = light;
				numLights++;
			}
		}else
		{
			if( index < maxlights )
			{
				m_Lights[ index ] = light;
				//expand if we're bigger
				if( (index+1) > numLights )
				{
					numLights = (numLights+1);
				}
			}
		}
	}
	//TODO: CHeck usage of this class, should be materials
	vector< IMaterial * > m_Materials;	
	UINT	m_NumPrimitives;

	UINT	m_SizeVerts;
	UINT	m_SizeIndices;
	/// the maximum # textures used per pass
	UINT	m_MaxTextures;
	DRAWPRIMITIVETYPE m_PrimType;

	BUFFERALLOCATIONSTRUCT m_IndexBufferAllocation;
	BUFFERALLOCATIONSTRUCT m_VertexBufferAllocation;

	IToolBox * m_ToolBox;

	IRenderer * m_Renderer;
	IMaterial*  m_Material;
	IVertexBufferObject * m_VertexBufferInterface;
	IIndexBuffer * m_IndexBufferInterface;

	IController * m_Controller;
	ISceneSettings * m_SceneSettings;

	//lights attached to this geometry object
	ILightObject * m_Lights[ MAX_COMPLEX_LIGHTS ];
	ILightObject * m_DetailLights[ MAX_SIMPLE_LIGHTS ];
	int				m_NumLights;
	float			m_AmbientLightTerm[4];
	IBaseTextureObject * m_Lightmap;
	BUFFERALLOCATIONSTRUCT m_LightmapUVs;
	IVertexBufferObject * m_LightmapVB;

	IMaterialOverride *m_MatOverride;
	int				m_NumDetailLights;
};

#endif