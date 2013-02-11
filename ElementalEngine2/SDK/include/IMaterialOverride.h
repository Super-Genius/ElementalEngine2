///=====================================================================
/// \file	IMaterialOverride.h
/// \brief	Interface for material overrides
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

#ifndef _I_MATERIALOVERRIDE_H_
#define _I_MATERIALOVERRIDE_H_

namespace ElementalEngine
{

class IRenderer;

class IMaterialOverride
{
public:
	/// dtor
	virtual ~IMaterialOverride(){};
	
	/// Applies overrides before rendering
	/// \param pRenderer = renderer
	/// \param pEffect = effect to apply settings to
	virtual void Apply(IRenderer *pRenderer, IEffect *pEffect) = 0;

    /// UnApplies overrides after rendering
	/// \param pRenderer = renderer
	/// \param pEffect = effect to apply settings to
	virtual void UnApply(IRenderer *pRenderer, IEffect *pEffect) = 0;

	/// Sets a vec4 override (typically for vertex / pixel shader constants)
	/// \param hsType = type of var to set (pix const, vert const, etc)
	/// \param hsName = name of var to set
	/// \param vecValue = value to set
	virtual void SetOverride(DWORD hsType, DWORD hsName, Vec4 vecValue) = 0;

	/// Sets a string override (textures, renderstates)
	/// \param hsType = type of var to set (tex, render state, etc)
	/// \param hsName = name of var to set
	/// \param szValue = text value to set
	virtual void SetOverride(DWORD hsType, DWORD hsName, StdString szValue) = 0;

	/// Checks to see if the alpha flag is set in this override
	/// \return true if alphablend enabled
	virtual bool HasAlpha() = 0;

	/// Clears out the material maps
	virtual void ResetOverrides() = 0;
};

} //namespace ElementalEngine

#endif //_I_MATERIALOVERRIDE_H_
