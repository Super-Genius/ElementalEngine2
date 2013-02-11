///============================================================================
/// \file		CTerrainConfig.h
/// \brief		Header file for a Terrain Configuration
/// \date		03-24-2005
/// \author		Brian Bazyk
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

#ifndef	CTERRAINCONFIG_H
#define	CTERRAINCONFIG_H

struct TerrainMetrics_t
{
	float fSectorSize;
	int iSectorResolution;
	float fZScale;
	float fLODDistance;
};

class CTerrainConfig : public CObjectTemplate<IObject>
{
private:
	SINGLETONCONSTRUCTOROVERRIDE(CTerrainConfig);

	/// Default Constructor
	CTerrainConfig(IHashString *parentName, IHashString *name, bool bAddToHier);
	/// Destructor
	~CTerrainConfig();

public:
	static IComponent *Create(int nArgs, va_list argptr);

	/// Set to the default configuration
	void SetDefaults();

	/// Serialize this component to the archive.
	///	\param ar = reference to the archive class
	virtual void Serialize( IArchive &ar );

	/// return a string of the component type
	/// \return IHashString *reference to textual classname
	virtual IHashString *GetComponentType();

	/// Checks if this is similar to it's base classes component
	/// \return true if it is simlar, else false
	virtual bool IsKindOf(IHashString *compType);
	
	/// \brief	The DeInit Function for the Object
	virtual void DeInit();

	int GetSectorResolution();
	float GetSectorSize();
	float GetHeightMapZScale();
	float GetLODDistance();

private:
	CHashString m_HashName;

	// resolution of a sector (grid resolution of squares, not points!)
	int m_iSectorResolution;
	// Size of a sector in world space
	float m_fSectorSize;
	// Terrain Z scale in world space
	float m_fHeightMapZScale;
	// LOD Value to switch between Splats & Single Texture
	float m_fLODDistance;

	bool m_bAddToHierarchy;

	CHashString	m_BaseEffect;
	CHashString	m_SplatEffect;
	CHashString m_CacheEffect;
	CHashString m_SplatCacheRenderEffect;
	CHashString m_BaseCacheRenderEffect;
};

#endif