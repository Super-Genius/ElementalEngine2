///============================================================================
/// \file	CParticleLineType.h
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

#ifndef PARTICLELINETYPE_H
#define PARTICLELINETYPE_H

namespace ElementalEngine
{
class CParticleManager;

class CParticleLineType : public CObjectTemplate<IObject>
{
public:
	static IComponent *Create(int nArgs, va_list argptr);

	CParticleLineType(IHashString *parentName, IHashString *name);
	~CParticleLineType();

	virtual void Init();
	virtual void DeInit();
	virtual void Serialize(IArchive &ar);
	
	/// Get Head Color
	/// \return - Vec3 of Head Color
	inline Vec3 GetHeadColor()				const { return m_HeadColor; }
	/// Get Tail Color
	/// \return - Vec3 of Tail Color
	inline Vec3 GetTailColor()				const { return m_TailColor; }
	/// Get Head Color Red
	/// \return - float of Head Color Red
	inline float GetColorHeadColorR()		const { return m_HeadColor.x; }
	/// Get Head Color Green
	/// \return - float of Head Color Green
	inline float GetColorHeadColorG()		const { return m_HeadColor.y; }
	/// Get Head Color Blue
	/// \return - float of Head Color Blue
	inline float GetColorHeadColorB()		const { return m_HeadColor.z; }
	/// Get Tail Color Red
	/// \return - float of Tail Color Red
	inline float GetColorTailColorR()		const { return m_TailColor.x; }
	/// Get Tail Color Green
	/// \return - float of Tail Color Green
	inline float GetColorTailColorG()		const { return m_TailColor.y; }
	/// Get Tail Color Blue
	/// \return - float of Tail Color Blue
	inline float GetColorTailColorB()		const { return m_TailColor.z; }
	/// Get Particle Alpha Value
	/// \return - float of Alpha
	inline float GetAlpha()					const { return m_fAlpha; }
	/// Get Additive Flag
	/// \return - bool Additive Flag
	inline bool  GetAdditive()				const { return m_bAdditive; }
	/// Get Material
	/// \return - IMaterial interface.
	inline IMaterial* GetMaterial()			const { return m_pMaterial; }
	/// Get Texture
	/// \return - IBaseTextureObject interface.
	inline IBaseTextureObject* GetTexture()	const { return m_pTexture; }
	/// Get Texture FPS
	/// \return - float Texture FPS
	inline float GetTextureFPS()			const { return m_fTextureFPS; }
	/// Get Texture Scale
	/// \return - float Texture Scale
	inline float GetTextureScale()			const { return m_fTextureScale; }

	/// Get Max duration of particle
	/// \return - float in seconds
	inline float GetMaxLife()				const { return m_fMaxLife; }

	/// Get Unit Particle Direction
	/// \return - Vec3 unit vector of particle direction of travel
	inline Vec3  GetDirection()				const { return m_Direction; }
	/// Get Particle Travel Speed
	/// \return - float of travel speed
	inline float GetSpeed()					const { return m_fSpeed; }

	/// Get Length of Tail
	/// \return - float of Tail Length
	inline float GetLength()				const { return m_fLength; }
	/// Get Length of Head
	/// \return - float of Head Length
	inline float GetHeadLength()			const { return m_fHeadLength; }
	/// Get Particle Head Width
	/// \return - float of Head width
	inline float GetHeadWidth()				const { return m_fHeadWidth; }
	/// Get Particle Mid Width
	/// \return - float of Mid width
	inline float GetMidWidth()					const { return m_fMidWidth; }
	/// Get Particle Tail Widge
	/// \return - float of Tail width
	inline float GetTailWidth()				const { return m_fTailWidth; }

private:
	// CParticleManager pointer so we aren't sending a message to query Manager every frame.
	CParticleManager *m_pParticleManager;
	// Head Color
	Vec3 m_HeadColor;
	// Tail Color
	Vec3 m_TailColor;
	// Max Particle Life in Seconds
	float m_fMaxLife;
	// Alpha Value
	float m_fAlpha;
	// Additive Blend Flag
	bool m_bAdditive;
	// length of line
	float m_fLength;
	// length from head to midsection
	float m_fHeadLength;
	// orientation vector
	Vec3 m_Direction;
	// speed line moves along forward dir
	float m_fSpeed;
	// Head Width
	float m_fHeadWidth;
	// Mid Width
	float m_fMidWidth;
	// Tail Width
	float m_fTailWidth;
	// Texture FPS
	float m_fTextureFPS;
	// Texture Scale
	float m_fTextureScale;
	// Material Name
	CHashString m_szMaterialName;
	// Material Interface
	IMaterial *m_pMaterial;
	// Texture Interface
	IBaseTextureObject *m_pTexture;
};

} //namespace EE

#endif //PARTICLELINETYPE_H
