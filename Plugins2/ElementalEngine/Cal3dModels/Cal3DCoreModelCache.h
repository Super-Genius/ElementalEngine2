///============================================================================
/// \file	Cal3DCoreModelCache.h
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

#ifndef CAL3DCOREMODELCACHE_H
#define CAL3DCOREMODELCACHE_H

class Cal3DModelManager;

class CCal3DCoreModelCache : public CObjectTemplate<IObject>
{
public:
	static IComponent *Create(int nArgs, va_list argptr);

	CCal3DCoreModelCache(IHashString *parentName, IHashString *name);
	~CCal3DCoreModelCache();

	virtual void Init();
	virtual void DeInit();
	virtual void Serialize(IArchive &ar);

	Cal3DModelManager *m_pModelManager;
	StdString m_szCFG;
	CalCoreModel *m_pCoreModel;
	CalHardwareModel *m_pCalHardwareModel;

	CHashString m_hszMeshName;
	CHashString m_hszSkeletonName;

	BUFFERALLOCATIONSTRUCT m_VertexBufferAllocation;
	BUFFERALLOCATIONSTRUCT m_IndexBufferAllocation;
	IVertexBufferObject *m_pVertexBufferInterface;
	IIndexBuffer *m_pIndexBufferInterface;

	vector<IMaterial*> m_Materials;
};

#endif
