///============================================================================
/// \file	CParticleType.h
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

#ifndef PARTICLETYPE_H
#define PARTICLETYPE_H

namespace ElementalEngine
{
#define SAFEDELETE( arg ) if( arg ){ delete arg; arg = NULL; }
#define SAFEARRAYDELETE( arg ) if ( arg ){ delete[] arg; arg = NULL; }

class CParticleRenderObject;


class CParticleType : public CObjectTemplate<IParticleType>
{
public:
	CParticleType(IHashString *parentName, IHashString *name, bool bAddToHier);
	~CParticleType();
	//From IParticleEmitter
	virtual bool SetKeyData( PARTICLE_VARIABLE channel, KEYFRAME_DATA_SET &data );
	virtual bool GetKeyData( PARTICLE_VARIABLE channel, KEYFRAME_DATA_SET &data );

	virtual bool SetAdditiveBlend( bool enabled );
	virtual bool SetTexture( ITextureObject * texture );
	virtual bool SetAnimationTextureSubdivisions( const UINT xdiv = 1, const UINT ydiv = 1,
					const UINT numAnimations = 1 );
	virtual bool SetMaterial( IMaterial * mat );
	
	virtual bool GetAdditiveBlend();//should be additive, or normal
	virtual const TCHAR * GetTextureFilename();
	virtual const TCHAR * GetShaderFilename();

	virtual void SetFrameRate( float frames );
	virtual void SetTextureRate( float fps );
	virtual float GetFrameRate();
	virtual float GetTextureRate();

	static IComponent *Create(int nArgs, va_list argptr);

	/// \brief	Initialize the object
	virtual void Init();
	/// \brief	The Update Function for the Object
	/// \return	True on sucess, False on faulure
	virtual bool Update();
	virtual void DeInit();

	/// \brief	Serialize this component to the archive.
	///	\param ar = reference to the archive class
	virtual void Serialize(IArchive &ar);

	/// set the m_bAddToHierarchy flag
	/// \param flag = false to prevent object from being added to hierarchy
//	void SetAddToHierarchy(bool flag) { m_bAddToHierarchy = flag; }

	virtual const TCHAR * GetFilename();
	virtual void SetFilename( StdString * filename );
	virtual const TCHAR * GetFilepath();
	virtual void SetFilepath( StdString * filepath );

	DWORD   OnPumpBrushProperty(DWORD size, void *params);
	DWORD	OnGetParticleBrushPointer( DWORD size, void *params);

	/// Can only be accessed in EE and should only be accessed directly 
	/// by the particle render object for fast particle manipulation
	friend class ElementalEngine::CParticleRenderObject;
private:
	void LoadTexturesAndShaders();
	void FixEmptyKeys();//any keys that have no values, add one value set to 0
	void CheckAndFixEmptyKeyframe( KEYFRAME_DATA_SET &arg, int property );
	void AddKeyFrame(int property, float time, float value);
	int GetKeyframeCount();

	void Serialize_EditableProperties( IArchive *ar );
	/// hierarchy addition flag
	bool m_bAddToHierarchy;

	bool					m_bAdditiveBlend;
	bool					m_bTextureLoop;
	KEYFRAME_DATA_SET		m_KeyData[ PARTICLE_VARIABLE_MAX ];
	IBaseTextureObject *	m_Texture;
	IMaterial *				m_Material;
	StdString				m_Name;
	StdString				m_Filename;
	StdString				m_Filepath;
	StdString				m_TextureFilename;
	StdString				m_ShaderFilename;
	float					m_FrameRate;
	float					m_TexFrameRate;
	float					m_fAttachToEmitter;
};



} //namespace EE

#endif
