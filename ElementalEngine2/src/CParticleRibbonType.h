///============================================================================
/// \file	CParticleRibbonType.h
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

#ifndef PARTICLERIBBONTYPE_H
#define PARTICLERIBBONTYPE_H

namespace ElementalEngine
{

class CParticleManager;

class CParticleRibbonType : public CObjectTemplate<IObject>
{
public:
	static IComponent *Create(int nArgs, va_list argptr);

	CParticleRibbonType(IHashString *parentName, IHashString *name);
	~CParticleRibbonType();

	virtual void Init();
	virtual void DeInit();
	virtual void Serialize(IArchive &ar);

	inline int GetSegmentCount()				const { return m_iSegmentCount; }
	inline float GetSegmentLength()				const { return m_fSegmentLength; }
	inline float GetSegmentWidth()				const { return m_fSegmentWidth; }
	inline float GetSegmentLife()				const { return m_fSegmentLife; }
	inline float GetRibbonLength()				const { return m_fRibbonLength; }
	inline bool GetAdditive()					const { return m_bAdditive; }

	inline IMaterial* GetMaterial()			const { return m_pMaterial; }
	inline IBaseTextureObject* GetTexture()	const { return m_pTexture; }
	inline float GetTextureFPS()			const { return m_fTextureFPS; }
	inline float GetTextureScale()			const { return m_fTextureScale; }

private:
	/// quick manager access
	CParticleManager *m_pParticleManager;
	/// segment count
	int m_iSegmentCount;
	/// segment length
	float m_fSegmentLength;
	/// total ribbon length
	float m_fRibbonLength;
	/// blend mode
	bool m_bAdditive;
	/// fps texture plays at if animated
	float m_fTextureFPS;
	/// texture scale along line
	float m_fTextureScale;
	/// segment life
	float m_fSegmentLife;
	/// segment width
	float m_fSegmentWidth;

	CHashString m_szMaterialName;
	IMaterial *m_pMaterial;
	IBaseTextureObject *m_pTexture;
};

} //namespace EE

#endif //PARTICLERIBBONTYPE_H
