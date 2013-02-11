///============================================================================
/// \file	Cal3DCoreMeshCache.h
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

#ifndef CAL3DCOREMESHCACHE_H
#define CAL3DCOREMESHCACHE_H

class Cal3DModelManager;

class CCal3DCoreMeshCache : public CObjectTemplate<IObject>
{
public:
	static IComponent *Create(int nArgs, va_list argptr);

	CCal3DCoreMeshCache(IHashString *parentName, IHashString *name);
	~CCal3DCoreMeshCache();

	virtual void Init();
	virtual void DeInit();
	virtual void Serialize(IArchive &ar);

	Cal3DModelManager *m_pModelManager;
	CalCoreMesh *m_pCoreMesh;
};

#endif
