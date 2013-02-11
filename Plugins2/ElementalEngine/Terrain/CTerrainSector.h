///============================================================================
/// \file		CTerrainSector.h
/// \brief		Header file for a Terrain Sector
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

#ifndef	CTERRAINSECTOR_H
#define	CTERRAINSECTOR_H

class CTerrainManager;

class CTerrainSector : public CSceneObject<ISceneObject>
{
protected:
	/// \brief Default Constructor
	/// \param parentName = name of parent in hierarchy
	/// \param name = IHashString * name of this object
	CTerrainSector(IHashString *parentName, IHashString *name, bool bAddToHier);

public:
	/// \briefDefault Destructor
	~CTerrainSector();

	/// \brief Create an instance
	/// \return	Pointer to this
	static IComponent *Create(int nArgs, va_list argptr);

	virtual void Serialize(IArchive &ar);
	virtual void Init();
	virtual void DeInit();
	virtual bool Update();

	virtual OBJECTSORTTYPES GetSceneSortID() const { return RENDEROBJECT3D; }

	inline int GetSectorIndexX() { return m_iSectorX; }
	inline int GetSectorIndexY() { return m_iSectorY; }

	bool Deform( BRUSHPARAMS brush, float fAvgHeight );

	DWORD OnTerrainSectorDeform(DWORD size, void *in_params);
	DWORD OnTerrainSectorSetData(DWORD size, void *in_params);
	DWORD OnTerrainSectorGetData(DWORD size, void *in_params);
	DWORD OnTerrainSectorGetPosition(DWORD size, void *in_params);
	DWORD OnTerrainSectorGetBrushAreaInfo(DWORD size, void *in_params);
	DWORD OnGetGlobalPosition(DWORD size, void *param);
	DWORD OnGetGlobalEuler(DWORD size, void *param);
	DWORD OnGetGlobalScale(DWORD size, void *param);
	DWORD OnGetGlobalTransform(DWORD size, void *param);
	DWORD OnTerrainSetPath( DWORD size, void *param );

	// Since CTerrainSectors have children they need to implement some important
	// bits of CEntity's functionality.
	DWORD OnGetEntityType(DWORD size, void *param);
	DWORD OnGetParentName(DWORD size, void *param);
	DWORD OnIsEntityTypeOf(DWORD size, void *param);

private:
	void UpdatePositionFromParent();
	///	\brief	send TerrainSectorSetData to EE
	void NotifyHeightMapChange();

	/// Easy access to the terrain manager
	CTerrainManager *m_pTerrainMgr;

	/// Heightmap filename
	StdString m_szHeightMap;
	/// Heightmap data ptr
	float *m_pHeightMap;
	/// Number of samples across a dimension of the heightmap
	long m_iHeightMapSize;
	/// Sector indices
	int m_iSectorX, m_iSectorY;

	bool m_bAddToHierarchy;

	/// Position and orientation of entity above
	Vec3 m_Position;
	EulerAngle m_Orientation;
	Vec3 m_Scale;	
	Matrix4x4 m_Transform;
};

#endif