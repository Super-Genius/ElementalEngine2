///============================================================================
/// \file	Cal3DCoreAnimationCache.cpp
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

#include "Stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT( CCal3DCoreAnimationCache );
IComponent *CCal3DCoreAnimationCache::Create(int nArgs, va_list argptr)
{
	IObject *self;
	IHashString *name = va_arg(argptr, IHashString *);
	IHashString *parentName = va_arg(argptr, IHashString *);
	try
	{
		// try and create an instance
		self = new CCal3DCoreAnimationCache(parentName, name);
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

CCal3DCoreAnimationCache::CCal3DCoreAnimationCache(IHashString *parentName, IHashString *name) : 
	OBJECTTEMPLATE(Cal3DModelManager, CCal3DCoreAnimationCache, IObject, parentName, name)
{
	m_pModelManager = (Cal3DModelManager*)m_Manager;
	m_pCoreAnimation = NULL;
}

CCal3DCoreAnimationCache::~CCal3DCoreAnimationCache()
{
	DeInit();
}

void CCal3DCoreAnimationCache::Init()
{
	if (m_pCoreAnimation)
	{
		m_pCoreAnimation->incRef();
	}
}

void CCal3DCoreAnimationCache::DeInit()
{
	if (m_pCoreAnimation)
	{
		if (m_pCoreAnimation->decRef())
			delete m_pCoreAnimation;
		m_pCoreAnimation = NULL;
	}
}

void CCal3DCoreAnimationCache::Serialize( IArchive &ar )
{
	if (ar.IsReading())
	{
		void *pData;
		ar.Read( &pData, sizeof(void*) );

//		Cal3DDataStream dataStream( pData );
//		m_pCoreAnimation = CalLoader::loadCoreAnimation(dataStream);
		m_pCoreAnimation = CalLoader::loadCoreAnimation(pData);
	}
	else
	{
		assert( FALSE );
//		ar.Write(  );
	}
}
