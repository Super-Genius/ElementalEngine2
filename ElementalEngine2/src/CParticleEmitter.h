///============================================================================
/// \file	CParticleEmitter.h
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

#ifndef PARTICLEEMITTER_H
#define PARTICLEEMITTER_H

namespace ElementalEngine
{
#define SAFEDELETE( arg ) if( arg ){ delete arg; arg = NULL; }
#define SAFEARRAYDELETE( arg ) if ( arg ){ delete[] arg; arg = NULL; }

class CParticleRenderObject;

struct ParticleTypeInstance
{
	IParticleType * m_ParticleType;
	bool		m_Visible;
};

typedef vector< ParticleTypeInstance >	PARTICLE_INSTANCE_VECTOR;

class CParticleEmitter : public CObjectTemplate<IParticleEmitter>
{
public:
	CParticleEmitter(IHashString *parentName, IHashString *name, bool bAddToHier);
	~CParticleEmitter();
	//From IParticleEmitter
	virtual bool SetKeyData( PARTICLE_EMITTER_VARIABLE channel, KEYFRAME_DATA_SET &data );
	virtual bool GetKeyData( PARTICLE_EMITTER_VARIABLE channel, KEYFRAME_DATA_SET &data );

	virtual void AddParticleBrush( IParticleType * particle );
	virtual void MoveBrush( bool bUp, UINT index );
	virtual void SetBrushVisibility( IParticleType * particle, bool visible );	
	virtual void RemoveBrush( IParticleType * particle );
	virtual UINT GetNumBrushes();
	virtual IParticleType * GetBrush( UINT index );
	virtual void SetTint( float r, float g, float b, float a );
	virtual void GetTint(float tintout[]);

	virtual const TCHAR * GetFilename();
	/// \brief	Create the a instance of IObject
	/// \return	IComponent pointer
	static IComponent *Create(int nArgs, va_list argptr);

	/// \brief	Initialize the object
	virtual void Init();
	/// \brief	The Update Function for the Object
	/// \return	True on sucess, False on failure
	virtual bool Update();

	/// \brief	Serialize this component to the archive.
	///	\param ar = reference to the archive class
	virtual void Serialize(IArchive &ar);

	DWORD   OnPumpEmitterProperty(DWORD size, void *params);
	DWORD	OnAddBrush(DWORD size, void *params);
	DWORD	OnGetParticleEmitterPointer( DWORD size, void *params);

	/// \brief Changes the internally stored filename in reponse to a save or save as
	/// \from the editor.
	/// \param filename = new filename to be assigned.
	virtual void SetFilename(StdString * filename);

	/// Can only be accessed in EE and should only be accessed directly 
	/// by the particle render object for fast particle manipulation, this is for speed
	friend class CParticleRenderObject;
private:
	void DeInit();
	void AddKeyFrame(int property, float time, float value);
	void FixEmptyKeys();//any keys that have no values, add one value set to 0
	void CheckAndFixEmptyKeyframe( KEYFRAME_DATA_SET &arg, int property );
	int GetKeyframeCount();

	/// hierarchy addition flag
	bool m_bAddToHierarchy;

	StdString				m_Filename;
	StdString				m_Name;
	KEYFRAME_DATA_SET		m_KeyData[ EMITTER_VARIABLE_MAX ];
	PARTICLE_INSTANCE_VECTOR	m_Brushes;
	IToolBox *				m_ToolBox;
	float					m_Tint[4];

	// DEBUG FUNCTIONS
	void DebugOutputLists();
};



} //namespace EE

#endif /*PARTICLEEMITTER_H*/
