///============================================================================
/// \file	CParticleManager.h
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

#ifndef PARTICLEMANAGER_H
#define PARTICLEMANAGER_H

namespace ElementalEngine 
{

typedef map< DWORD, IParticleType * > PARTICLETYPES;
typedef pair< DWORD, IParticleType * > PARTICLETYPESPAIR;
typedef map< DWORD, IParticleEmitter * > EMITTERTYPES;
typedef pair< DWORD, IParticleEmitter * > EMITTERTYPESPAIR;

class CParticleLineRenderObject;
class CParticleLineType;
class CParticleRibbonRenderObject;
class CParticleRibbonType;
class CParticleEmitterObject;
class CParticleEmitterType;
class CParticleBatchRenderObject;

// Batch Rendering of Particle Lines/Ribbons
typedef map< DWORD, CParticleLineRenderObject* > PARTICLELINESMAP;
typedef map< const CParticleLineType*, PARTICLELINESMAP> PARTICLETYPETOPARTICLELINE;
typedef map< DWORD, CParticleRibbonRenderObject* > PARTICLERIBBONSMAP;
typedef map< const CParticleRibbonType*, PARTICLERIBBONSMAP> PARTICLETYPETOPARTICLERIBBON;
typedef map< DWORD, CParticleEmitterObject* > PARTICLEEMITTERSMAP;
typedef map< const CParticleEmitterType*, PARTICLEEMITTERSMAP> PARTICLETYPETOPARTICLEEMITTER;

#define MAX_PARTICLE_BUFFER 65536
#define MAX_RANDOM_NORMAL_VECTORS 1024
#define MAX_RANDOM_NORMAL_XZDISK 512
#define MAX_RANDOM_XZDISK 512
#define MAX_RANDOM_FLOATS 4096

class CParticleManager : public CManager
{

public:

	/// \brief The Default Destructor for CParticleManager
	/// \return void
	~CParticleManager( );

	/// \brief	Function to get this component.  This is a singleton
	/// \param	void
	/// \return	IComponent*: This Manager
	static IComponent* Create( int nArgs, va_list argptr );

	/// \brief	Initializes the Particle Manager
	/// \param	void
	/// \return	bool: Success
	virtual bool Init( void );

	/// \brief	Update function for the Particle Manager
	/// \return	void
	virtual void Update( DWORD tickCount );
	
	/// \brief	DeInitialization
	/// \return	bool: Success
	virtual bool DeInit( void );
	
	DWORD OnGetParticleType(DWORD size, void *params);	
	DWORD OnGetParticleTypeNoLoad( DWORD size, void *params);
	DWORD OnInitParticles(DWORD size, void *param);
	DWORD OnGetEmitterType( DWORD size, void *params);
	DWORD OnReloadParticle( DWORD size, void *params);
	DWORD OnReloadAllParticles( DWORD size, void *params);
	DWORD OnRenameParticleEmitter(DWORD size, void *param);
	DWORD OnRenameParticleBrush(DWORD size, void *param);
	DWORD OnAddLoadedEmitter(DWORD size, void *params);
	DWORD OnAddLoadedBrush(DWORD size, void *params);
		
	// Nothing but the Editor should use this message.
	DWORD OnEditorDisplay( DWORD size, void *params);

	/// \brief Adds A Particle Line for Batch Rendering
	/// \param Particle Line To Be Added
	/// \return VOID
	void AddParticleLine( CParticleLineRenderObject* particleObject );
	
	/// \brief Removes A Particle Line for Batch Rendering
	/// \param Particle Line To Be Removed
	/// \return VOID
	void RemoveParticleLine( CParticleLineRenderObject* particleObject );

	/// 
	PARTICLETYPETOPARTICLELINE* GetParticleLineBatches( void );

	/// \brief Adds A Particle Ribbon for Batch Rendering
	/// \param Particle Ribbon To Be Added
	/// \return VOID
	void AddParticleRibbon( CParticleRibbonRenderObject* particleObject );
	
	/// \brief Removes A Particle Ribbon for Batch Rendering
	/// \param Particle Ribbon To Be Removed
	/// \return VOID
	void RemoveParticleRibbon( CParticleRibbonRenderObject* particleObject );

	/// 
	PARTICLETYPETOPARTICLERIBBON* GetParticleRibbonBatches( void );

	/// \brief Adds A Particle Emitter for Batch Rendering
	/// \param Particle Emitter To Be Added
	/// \return VOID
	void AddParticleEmitter( CParticleEmitterObject* particleObject );
	
	/// \brief Removes A Particle Emitter for Batch Rendering
	/// \param Particle Emitter To Be Removed
	/// \return VOID
	void RemoveParticleEmitter( CParticleEmitterObject* particleObject );

	/// 
	PARTICLETYPETOPARTICLEEMITTER* GetParticleEmitterBatches( void );

	inline Vec3 GetRandomNormalVector()
	{	
		m_iRandomNormalIndex = (m_iRandomNormalIndex + 1) % MAX_RANDOM_NORMAL_VECTORS;
		return m_v3RandomNormalVectorTable[m_iRandomNormalIndex];
	}
	inline Vec3 GetRandomNormalXZDisk()
	{	
		m_iRandomNormalXZDiskIndex = (m_iRandomNormalXZDiskIndex + 1) % MAX_RANDOM_NORMAL_XZDISK;
		return m_v3RandomNormalXZDiskTable[m_iRandomNormalXZDiskIndex];
	}
	inline Vec3 GetRandomXZDisk()
	{
		m_iRandomXZDiskIndex = (m_iRandomXZDiskIndex + 1) % MAX_RANDOM_XZDISK;
		return m_v3RandomXZDiskTable[m_iRandomXZDiskIndex];
	}
	inline float GetRandomFloat()
	{	
		m_iRandomFloatIndex = (m_iRandomFloatIndex + 1) % MAX_RANDOM_FLOATS;
		return m_fRandomFloatTable[m_iRandomFloatIndex];
	}

	const IObject* FindParticleType( IHashString *pTypeName, IHashString *pTypeTypeName );
	const IObject* LoadParticleType( IHashString *pTypeName, IHashString *pTypeTypeName );
	ITextureObject* LoadTexture( const TCHAR *str );

private:
	void RandomBuildNormalVectorTable();
	void RandomBuildNormalXZDiskTable();
	void RandomBuildXZDiskTable();
	void RandomBuildFloatTable();

	/// \brief Searches through the library to see if Brush Exists, if not, loads it from file.
	/// \return IParticleType* , particle loaded from map/file. NULL for error.
	IParticleType * LoadParticleTypeFromLibrary( IHashString * filepath );

	/// \brief Searches through the library to see if Emitter Exits, if not, loads it from file.
	/// \return IParticleEmitter* , particle loaded from map/file. NULL for error.
	IParticleEmitter * LoadEmitterType( IHashString * filepath );

	/// \brief Searches through the library to see if Brush Exists, if not, adds it. This does not load from file.
	/// \param DWORD 
	void AddBrushToLibrary( DWORD id, IParticleType * pBrush);
	
	/// \brief Searches through the library to see if Emitter Exists, if not, adds it. This does not load from file.
	/// \param DWORD
	void AddEmitterToLibrary( DWORD id, IParticleEmitter * pEmitter);

	/// \brief Searches through library for Emitter and Returns it
	/// \param DWORD Unique ID of EmitterFile
	/// \return IParticleEmitter* associated with passed Dword, if not found, NULL.
	IParticleEmitter * GetEmitterFromLibrary(DWORD id);

	/// \brief Searches through library for Brush and Returns it
	/// \param DWORD Unique ID of BrushFile
	/// \return IParticleEmitter* associated with passed Dword, if not found, NULL.
	IParticleType * GetBrushFromLibrary(DWORD id);


	SINGLETONCONSTRUCTOROVERRIDE( CParticleManager );
	/// \brief The Default Constructor for CParticleManager
	/// \return void
	CParticleManager();

	//Particle Types library
	PARTICLETYPES	m_ParticleTypeLibrary;
	EMITTERTYPES	m_EmitterTypeLibrary;
	bool			m_bInitialized;
	bool			m_bEditorDisplay;
	IRenderContext *m_pEditorContext;
	//Particle v buffer
	IVertexBufferObject *			m_Dynamic2DVertexBuffer;
	BUFFERALLOCATIONSTRUCT				m_Dynamic2DVertexBufferAllocation;
	BUFFERALLOCATIONSTRUCT				m_IndexBufferAllocation;
	IIndexBuffer *						m_IndexBuffer;

	CHashString m_szParticleLineTypeName;
	CHashString m_szParticleRibbonTypeName;
	CHashString m_szParticleEmitterTypeName;
	CHashString m_szParticleLineRenderObjectName;
	CHashString m_szParticleRibbonRenderObjectName;
	CHashString m_szParticleEmitterObjectName;
	CHashString m_szParticleRenderObjectName;
	CHashString m_szParticleSystemObjectName;

	//Batch Rendering of Particle Lines/Ribbons
	PARTICLETYPETOPARTICLELINE m_BatchDrawParticleLineMap;
	PARTICLETYPETOPARTICLERIBBON m_BatchDrawParticleRibbonMap;
	PARTICLETYPETOPARTICLEEMITTER m_BatchDrawParticleEmitterMap;

	CParticleBatchRenderObject* m_BatchRenderObject;

	// Random Number Storage
	int m_iRandomNormalIndex;
	Vec3 m_v3RandomNormalVectorTable[MAX_RANDOM_NORMAL_VECTORS];
	int m_iRandomFloatIndex;
	float m_fRandomFloatTable[MAX_RANDOM_FLOATS];
	int m_iRandomNormalXZDiskIndex;
	Vec3 m_v3RandomNormalXZDiskTable[MAX_RANDOM_NORMAL_XZDISK];
	int m_iRandomXZDiskIndex;
	Vec3 m_v3RandomXZDiskTable[MAX_RANDOM_XZDISK];
	CHashString m_ccamTypeName;
};

}	// namespace ElementalEngine
#endif