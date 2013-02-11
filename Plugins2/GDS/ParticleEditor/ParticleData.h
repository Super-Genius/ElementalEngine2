#ifndef PARTICLEDATA_H
#define PARTICLEDATA_H

struct ParticleTypeInstance
{
	IParticleType * m_ParticleType;
	int			m_ID;
	bool		m_Visible;
};

typedef map< IParticleType * , ParticleTypeInstance >	PARTICLE_INSTANCE_MAP;
typedef pair< IParticleType * , ParticleTypeInstance >	PARTICLE_INSTANCE;

class CParticleData
{
//Data
public:
	bool					m_bAdditiveBlend;
	bool					m_bTextureLoop;
	KEYFRAME_DATA_SET		m_KeyData[ PARTICLE_VARIABLE_MAX ];
	IBaseTextureObject *	m_Texture;
	IShader *				m_Shader;
	StdString				m_Name;
	StdString				m_Filename;
	StdString				m_TextureFilename;
	StdString				m_ShaderFilename;
	UINT					m_FrameRate;
	UINT					m_TexFrameRate;
	float					m_fAttachToEmitter;
//Functions
public:
//	CParticleData();
//	~CParticleData();
};


class CEmitterData
{
//Data
public:
	KEYFRAME_DATA_SET		m_KeyData[ EMITTER_VARIABLE_MAX ];
	PARTICLE_INSTANCE_MAP	m_Brushes;
	int						m_numBrushes;
//	CParticleData *			m_Brushes;
	StdString				m_Name;
	StdString				m_Filename;
	float					m_Tint[4];
//Functions
public:
//	CEmitterData();
//	~CEmitterData();
};
#endif /* PARTICLEDATA_H */