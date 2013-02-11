///==========================================================================
/// \file	CParticleEmitterType.h
/// \brief	Implementation of CParticleEmitterType Data Class for CParticleEmitterObject 
/// \date	12/05/2007
/// \author	Brendan Geiger
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
///==========================================================================

#ifndef PARTICLEEMITTERTYPE_H
#define PARTICLEEMITTERTYPE_H

extern UINT MAX_PARTICLE_COUNT;

namespace ElementalEngine
{
class CParticleManager;

enum PARTICLE_TYPES 
{
PT_POINT = 0,
FAST_POINT,
SPHERE,
BOX,
PLANE,
DISK,
RING,
HALO,
SHELL,
JET,
CONE,
PILLAR,
INVALID_TYPE
};

struct SParticleKeyframe
{
	float time;
	float value;
	SParticleKeyframe()
	{
		time = 0.0f;
		value = 0.0f;
	}
};

class CParticleEmitterType : public CObjectTemplate<IObject>
{
public:
	static IComponent *Create(int nArgs, va_list argptr);

	CParticleEmitterType(IHashString *parentName, IHashString *name);
	~CParticleEmitterType();

	virtual void Init();
	virtual void DeInit();
	virtual void Serialize(IArchive &ar);

	/// Determines the point in a particle emitter's life where the particles it generates
	/// last the longest amount of time.
	/// \return - float of largest particle life.
	inline float FindGreatestParticleLife () const
	{
		float greatestlife = 0;
		for (unsigned int i = 0; i < m_nKeysLife; i++)
		{
			if (m_keysLife[i].value > greatestlife)
				greatestlife = m_keysLife[i].value;
		}
		return greatestlife;
	}

	/// Integrates over the life of a particle emitter to determine the maximum number of 
	/// particles active at any moment.
	/// \param emitterlife - max life of particular particle emitter instance
	/// \return - int max number of particles
	inline unsigned int IntegrateMaxNumParticles ( float emitterlife )
	{
		float accum = 0.0f;
		float greatestlife = FindGreatestParticleLife();

		// We've already calculated this, return cached value.
		if (m_nCachedParticleArraySize > 0)
		{
			return m_nCachedParticleArraySize;
		}
		// If only one element we cannot traverse keyframes.
		if (m_nKeysNumber == 1)
		{
			accum = m_keysNumber[0].value * greatestlife;
		}
		else
		{
			// Traverse Keyframes
			float block = 0.0f;
			for (unsigned int i = 0; i < m_nKeysNumber; i++)
			{
				// At last element
				if (i == (m_nKeysNumber - 1))
					break;
				// If Emitter Life less than keyed time, use that as an upper bound instead.
				if ( (m_keysNumber[i].time < emitterlife) && (m_keysNumber[i+1].time > emitterlife) )
				{
					block = m_keysNumber[i].value + GetEmitterNumber(emitterlife);
					block = emitterlife - m_keysNumber[i].time;
					block /= 2;
					accum += block;
					break;
				}
				// Normal Keyframe Block
				else
				{
					block = m_keysNumber[i].value + m_keysNumber[i+1].value;
					block *= m_keysNumber[i+1].time - m_keysNumber[i].time;
					block /= 2;
					accum += block;
				}
			}
			accum *= greatestlife;
		}

		accum = ceil( accum );
		if ( accum > (float)MAX_PARTICLE_COUNT )
			accum = (float)(MAX_PARTICLE_COUNT);
		m_nCachedParticleArraySize = (unsigned int)accum;

		return m_nCachedParticleArraySize;

	}

	/// LERP for a time based on current time and mid/max lives. Used by all Triplets.
	/// \param value - float * to triplet of keyframes.
	/// \param time - current time
	/// \param midlife - time of current midpoint
	/// \param maxlife - max life of particle
	/// \return - float value LERP from triplet.
	inline float GetMidValue ( const float * value, float time, float maxlife, float midlife ) const
	{
		if (time < midlife)
			return value[0] + (value[1] - value[0])*(time / midlife); 
			//(value[1] - value[0]) / ((1.0f - (midlife - time))/(midlife));
		else
			return (value[1]*(maxlife - time) + value[2]*(time - midlife))/(maxlife - midlife);
			//(value[2] - value[1]) / ((1.0f - (midlife - time))/(maxlife - midlife));	
	}

	/// Returns value of data at time base on fully dynamic keyframe data.
	/// \params keys - pointer to keyframe group.
	/// \params numkeys - number of keys in keyframe group.
	/// \params time - time to get the data from.
	inline float GetKeyValue( SParticleKeyframe *keys, unsigned int numkeys, float time) const
	{
		// Invalid Case: This shouldn't happen, basically Emitter loaded with broken data
		if ( (0 == numkeys) || (keys == NULL) )
		{
			assert(0); 
			return 0.0f;
		}
		// Special Case: Algorithm doesn't need to run for 1 key frame.
		if (1 == numkeys)
			return keys[0].value;
		// Special Case: if time is past last keyframe, run with last value.
		if (time > keys[numkeys-1].time)
			return keys[numkeys-1].value;
		// Common Case: Point falls between two values;
		unsigned int i = 0;
		while ( i < numkeys-1 )
		{
			if (	(time < keys[i+1].time) &&
					(time > keys[i].time))
			{
				float t1 = time - keys[i].time;
				float t2 = keys[i+1].time - keys[i].time;
				float R = t1 / t2;
				float R1 = 1.0f - R;
				return (keys[i].value * R1 + keys[i+1].value * R);
			}
			i++;
		}
		// Shouldn't arrive here.
		assert(0);
		return 0.0f;
	}
	/// Create Keys From an Incomming Archive
	/// \param ar - Reference to Archive.
	/// \param keys - Reference to an Array of Keys.
	/// \param numkeys - number of keys.
	void CreateKeys(IArchive &ar, SParticleKeyframe *&keys, unsigned int numkeys  );
	/// File Parsing Helper Function
	/// \param type - Pointer to String to get Enumerated Type PARTICLE_TYPES
	/// \return - PARTICLE_TYPES enumerated type.
	PARTICLE_TYPES GetTypeFromString (const char *type) const;
	/// Get Type Enumerated Particle Emitter Type
	/// \return - PARTICLE_TYPES enumerated type.
	PARTICLE_TYPES GetType()					const { return m_Type; }
	/// Get Screen Aligned (UNUSED)
	/// \return - bool flag for Screen Aligned
	inline bool GetScreenAligned()						const { return m_bScreenAligned; }
	/// Get Additive Blending Flag
	/// \return - bool flag for Additive Blending
	inline bool GetAdditiveBlending()					const { return m_bAdditiveBlending; }
	/// Get Stretch To Origin
	/// \return - bool flag for Stretching Particle from current position to origin.
	inline bool GetStretchToOrigin()					const { return m_bStretchToOrigin; }
	/// Get Looping Flag (UNUSED)
	/// \return - bool flag for Looping
	inline bool GetLooping()							const { return m_bLooping; }
	/// Get Attach To Emitter Scale
	/// \return - float (0.0-1.0) for how much particles mimic the movement their parent emitter.
	inline float GetAttachToEmitter()					const { return m_fAttachToEmitter; }
	/// Point at which Particle Emitter loops
	/// \return - float time in seconds when particle loops around 
	inline float GetLoopLife()							const { return m_fLoopLife; }
	/// Get Texture
	/// \return - ITextureObject Interface
	inline ITextureObject* GetTexture()					{ return m_pUseTexture; }
	/// Get Animated Texture
	/// \return - bool, true if Texture is Animated
	inline bool GetAnimatedTexture()					const { return m_bAnimatedTexture; }
	/// Get Texture FPS
	/// \return - float, texture frame rate
	inline float GetTextureFPS()						const { return m_fTextureFPS; }
	/// Get Texture Scale
	/// \return - float, texture scale
	inline float GetTextureScale()						const { return m_fTextureScale; }
	/// Get Texture UV's
	/// \param u1 - Reference to u1 coord
	/// \param u2 - Reference to u2 coord
	/// \param v1 - Reference to v1 coord
	/// \param v2 - Reference to v2 coord
	/// \param life - Time in Particle Life at which to sample coords
	inline void GetAnimatedUVs( float &u1, float &u2, float &v1, float &v2, const float life)
	{
		IAnimatedTextureObject * at = (IAnimatedTextureObject * )m_pTexture;
		int iTextureFrame = (int)(life * m_fTextureFPS);
		iTextureFrame %= at->GetNumFrames();
		at->GetFrame( iTextureFrame, v1, u1, v2, u2, NULL );
	}
	/// Get Random Rotation Flag
	/// \return bool - Random Start Rotation Flag
	inline bool GetRandomRotation()						const { return m_bRandomRotation; }
	/// Get Emitter Number of Particles at Time
	/// \param time - Emitter Time
	/// \return - float Number of Particles at Time 
	inline float GetEmitterNumber(float time)			const { return GetKeyValue(m_keysNumber, m_nKeysNumber, time); }
	/// Get Emitter L Volume Param
	/// \param time - Emitter Time
	/// \return - float L Volume Param
	inline float GetEmitterL(float time)				const { return GetKeyValue(m_keysL, m_nKeysL, time); }
	/// Get Emitter W Volume Param
	/// \param time - Emitter Time
	/// \return - float, W Volume Param
	inline float GetEmitterW(float time)				const { return GetKeyValue(m_keysW, m_nKeysW, time); }
	/// Get Emitter H Volume Param
	/// \param time - Emitter Time
	/// \return - float, H Volume Param
	inline float GetEmitterH(float time)				const { return GetKeyValue(m_keysH, m_nKeysH, time); }
	/// Get Emitter Out Velocity
	/// \param time - Emitter Time
	/// \return - float, Out Velocity at Time
	inline float GetEmitterOutVelocity(float time)		const { return GetKeyValue(m_keysOutVelocity, m_nKeysOutVelocity, time); }
	/// Get Emitter Alt Velocity
	/// \param time - Emitter Time
	/// \return - float, Alt Velocity at Time
	inline float GetEmitterAltVelocity(float time)		const { return GetKeyValue(m_keysAltVelocity, m_nKeysAltVelocity, time); }
	/// Get Particle Life at Emission Time
	/// \param time - Emitter Time
	/// \return - float, Particle Life in Seconds
	inline float GetEmitterLife(float time)			const { return GetKeyValue(m_keysLife, m_nKeysLife, time); }
	/// Get Particle MidLife at Emission Time
	/// \param time - Emitter Time
	/// \return - float, Particle MidLife in Seconds
	inline float GetEmitterMidLife(float time)			const { return GetKeyValue(m_keysMidlife, m_nKeysMidlife, time); }
	/// Get Particle Weight at Time
	/// \param time - Particle Time
	/// \param maxlife - Particle Max Life
	/// \param midlife - Particle Mid Life
	/// \return - float, Particle Weight at Time
	inline float GetParticleWeight(float time, float maxlife, float midlife)	const { return GetMidValue(m_fWeight, time, maxlife, midlife); }
	/// Get Particle Speed at Time
	/// \param time - Particle Time
	/// \param maxlife - Particle Max Life
	/// \param midlife - Particle Mid Life
	/// \return - float, Particle Speed at Time
	inline float GetParticleSpeed(float time, float maxlife, float midlife)		const { return GetMidValue(m_fSpeed, time, maxlife, midlife); }
	/// Get Particle Size at Time
	/// \param time - Particle Time
	/// \param maxlife - Particle Max Life
	/// \param midlife - Particle Mid Life
	/// \return - float, Particle Size at Time
	inline float GetParticleSize(float time, float maxlife, float midlife)		const { return GetMidValue(m_fSize, time, maxlife, midlife); }
	/// Get Particle Spin at Time
	/// \param time - Particle Time
	/// \param maxlife - Particle Max Life
	/// \param midlife - Particle Mid Life
	/// \return - float, Particle Spin at Time
	inline float GetParticleSpin(float time, float maxlife, float midlife)		const { return GetMidValue(m_fSpin, time, maxlife, midlife); }
	/// Get Particle Color at Time
	/// \param time - Particle Time
	/// \param maxlife - Particle Max Life
	/// \param midlife - Particle Mid Life
	/// \return - DWORD, Particle Color at Time
	inline DWORD GetParticleColor(float time, float maxlife, float midlife)	const 
	{
		float color[4];
		memset(color, 0, sizeof(float)*4);

		float t1;
		float t2;
		float R;
		float R1;

		if (time < midlife)
		{
			t1 = time;
			t2 = midlife;
			R = t1 / t2;
			R1 = 1 - R;
			for (int i = 0; i < 4; i++)
				color[i] = R1 * m_fColor[0][i] + R * m_fColor[1][i];
			return m_IRenderer->GetColorARGB( color[ 3 ], color[ 0 ], color[ 1 ], color[ 2 ] );
		}
		else
		{
			t1 = time - midlife;
			t2 = maxlife - midlife;
			R = t1 / t2;
			R1 = 1 - R;
			for (int i = 0; i < 4; i++)
				color[i] = R1 * m_fColor[1][i] + R * m_fColor[2][i];
			return m_IRenderer->GetColorARGB( color[ 3 ], color[ 0 ], color[ 1 ], color[ 2 ] );
		}	
		return 0xFFFFFFFF; // Impossible return, but make it white anyhow.
	}
	/// Get Keyframes Color
	/// Shader Packing Function
	inline void GetKeyframesColor( Vec4 &key0, Vec4 &key1, Vec4 &key2 )
	{
		key0.Set( m_fColor[0][0], m_fColor[0][1], m_fColor[0][2], m_fColor[0][3] );
		key1.Set( m_fColor[1][0], m_fColor[1][1], m_fColor[1][2], m_fColor[1][3] );
		key2.Set( m_fColor[2][0], m_fColor[2][1], m_fColor[2][2], m_fColor[2][3] );
	}
	/// Get Keyframes Weight Speed Size Spin
	/// Shader Packing Function
	inline void GetKeyframesWeightSpeedSizeSpin( Vec4 &key0, Vec4 &key1, Vec4 &key2 )
	{
		key0.Set( m_fWeight[0], m_fSpeed[0], m_fSize[0], m_fSpin[0] );
		key1.Set( m_fWeight[1], m_fSpeed[1], m_fSize[1], m_fSpin[1] );
		key2.Set( m_fWeight[2], m_fSpeed[2], m_fSize[2], m_fSpin[2] );
	}

private:
	/// quick manager access
	CParticleManager *m_pParticleManager;

	/// Version Number (File)
	CHashString m_hszVersion;
	/// Type of Spawn/Movement (File)
	PARTICLE_TYPES m_Type;

	/// Cached Particle Array Size
	unsigned int m_nCachedParticleArraySize;

	/// Number of Particles per Second (file)
	unsigned int m_nKeysNumber;
	SParticleKeyframe *m_keysNumber;
	/// Spawn/Movement Properties (File)
	unsigned int m_nKeysL;
	SParticleKeyframe *m_keysL;
	unsigned int m_nKeysW;
	SParticleKeyframe *m_keysW;
	unsigned int m_nKeysH;
	SParticleKeyframe *m_keysH;
	unsigned int m_nKeysOutVelocity;
	SParticleKeyframe *m_keysOutVelocity;
	unsigned int m_nKeysAltVelocity;
	SParticleKeyframe *m_keysAltVelocity;
	unsigned int m_nKeysLife;
	SParticleKeyframe *m_keysLife;
	unsigned int m_nKeysMidlife;
	SParticleKeyframe *m_keysMidlife;

	/// Texture Path (File)
	CHashString m_hszTextureName;
	/// Texture FPS (File)
	float m_fTextureFPS;
	/// Texture Scale (File)
	float m_fTextureScale;

	/// Random Spawn Rotation (file)
	bool m_bRandomRotation;
	/// Additive Blending (file)
	bool m_bAdditiveBlending;
	/// Screen Aligned (true) or Local Aligned (false) (file)
	bool m_bScreenAligned;
	/// Looping of Emitter (file)
	bool m_bLooping;
	/// Stretch from particle position to Origin. (file)
	bool m_bStretchToOrigin;
	/// The degree a particle moves with the object (file1.1)
	float m_fAttachToEmitter;
	/// Life at which particle loops (file)
	float m_fLoopLife;
	/// Midlife Time (file)
	float m_fMidlife;
	/// Maxlife of Emitter (file)
	float m_fMaxlife;

	/// Speed of Particles (file)
	float m_fSpeed[3];
	/// Size of Particles (file)
	float m_fSize[3];
	/// Life of Particles (file)
	float m_fLife[3];
	/// Spin in Degrees per Second (file)
	float m_fSpin[3];
	/// Weight in units per second (file)
	float m_fWeight[3];
	/// Color (file)
	float m_fColor[3][4];

	/// Texture Pointer for internal use.(Code - Internal)
	ITextureObject * m_pTexture;
	/// Texture Pointer for external Use.(Code)
	ITextureObject * m_pUseTexture;
	/// Texture Animated (Code)
	bool m_bAnimatedTexture;
	/// Renderer (Code - Internal)
	IRenderer *			m_IRenderer;

};

} //namespace EE

#endif //PARTICLEEMITTERTYPE_H
