///============================================================================
/// \file	Cal3DCoreAnimationCache.h
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

#ifndef CAL3DCOREANIMATIONCACHE_H
#define CAL3DCOREANIMATIONCACHE_H

class Cal3DModelManager;

class CCal3DCoreAnimationCache : public CObjectTemplate<IObject>
{
public:
	static IComponent *Create(int nArgs, va_list argptr);

	CCal3DCoreAnimationCache(IHashString *parentName, IHashString *name);
	~CCal3DCoreAnimationCache();

	virtual void Init();
	virtual void DeInit();
	virtual void Serialize(IArchive &ar);

	Cal3DModelManager *m_pModelManager;
	CalCoreAnimation *m_pCoreAnimation;
};

#endif
