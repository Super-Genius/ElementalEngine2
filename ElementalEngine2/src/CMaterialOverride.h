///=====================================================================
/// \file	CMaterialOverride.h
/// \brief	Implementation for material overrides
/// \date	08/08/2007
/// \author	Nick Tourte
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
///=====================================================================

#ifndef _C_MATERIALOVERRIDE_H_
#define _C_MATERIALOVERRIDE_H_

typedef map<DWORD, Vec4> VEC4OVERRIDEMAP;
typedef map<UINT, IBaseTextureObject*> TEXTUREMAP;

class CMaterialOverride : public IMaterialOverride
{
public:
	/// ctor, dtor
	CMaterialOverride();
	virtual ~CMaterialOverride();
	
	/// Applies overrides before rendering
	virtual void Apply(IRenderer *pRenderer, IEffect *pEffect);

	/// UnApplies overrides after rendering
	/// \param pRenderer = renderer
	/// \param pEffect = effect to apply settings to
	virtual void UnApply(IRenderer *pRenderer, IEffect *pEffect);

	/// Sets a vec4 override (typically for vertex / pixel shader constants)
	/// \param hsType = type of var to set (pix const, vert const, etc)
	/// \param hsName = name of var to set
	/// \param vecValue = value to set
	virtual void SetOverride(DWORD hsType, DWORD hsName, Vec4 vecValue);

	/// Sets a string override (textures, renderstates)
	/// \param hsType = type of var to set (tex, render state, etc)
	/// \param hsName = name of var to set
	/// \param szValue = text value to set
	virtual void SetOverride(DWORD hsType, DWORD hsName, StdString szValue);

	/// Checks to see if the alpha flag is set in this override
	/// \return true if alphablend enabled
	virtual bool HasAlpha() { return m_bAlphaBlend; }

	/// Clears out the material maps
	virtual void ResetOverrides() 
	{ 
		m_PixelConstantOverrides.clear();
		m_VertexConstantOverrides.clear();
		m_TextureOverrides.clear();
		m_RenderStateOverrides.clear();
		m_bAlphaBlend = false;
	}

private:
	void AddTextureToMap(DWORD hsNamedSpot, StdString szTexture);

	/// types used when adding overrides
	DWORD m_dwPixelConstantType;
	DWORD m_dwVertexConstantType;
	DWORD m_dwTextureType;
	DWORD m_dwRenderStateType;

	/// maps
	VEC4OVERRIDEMAP m_PixelConstantOverrides;
	VEC4OVERRIDEMAP m_VertexConstantOverrides;
	TEXTUREMAP m_TextureOverrides;
	map< ENUMRENDERSTATE, UINT > m_RenderStateOverrides;

	/// toolbox
	IToolBox *m_ToolBox;
	IRenderer *m_Renderer;

	bool m_bAlphaBlend;

};

#endif //_C_MATERIALOVERRIDE_H_
