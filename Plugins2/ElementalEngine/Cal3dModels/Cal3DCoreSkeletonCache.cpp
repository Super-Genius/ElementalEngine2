///============================================================================
/// \file	Cal3DCoreSkeletonCache.cpp
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

REGISTER_COMPONENT( CCal3DCoreSkeletonCache );
IComponent *CCal3DCoreSkeletonCache::Create(int nArgs, va_list argptr)
{
	IObject *self;
	IHashString *name = va_arg(argptr, IHashString *);
	IHashString *parentName = va_arg(argptr, IHashString *);
	try
	{
		// try and create an instance
		self = new CCal3DCoreSkeletonCache(parentName, name);
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

CCal3DCoreSkeletonCache::CCal3DCoreSkeletonCache(IHashString *parentName, IHashString *name) : 
	OBJECTTEMPLATE(Cal3DModelManager, CCal3DCoreSkeletonCache, IObject, parentName, name)
{
	m_pModelManager = (Cal3DModelManager*)m_Manager;
	m_pCoreSkeleton = NULL;
}

CCal3DCoreSkeletonCache::~CCal3DCoreSkeletonCache()
{
	DeInit();
}

void CCal3DCoreSkeletonCache::Init()
{
	if (m_pCoreSkeleton)
	{
		m_pCoreSkeleton->incRef();
	}
}

void CCal3DCoreSkeletonCache::DeInit()
{
	if (m_pCoreSkeleton)
	{
		if (m_pCoreSkeleton->decRef())
			delete m_pCoreSkeleton;
		m_pCoreSkeleton = NULL;
	}
}

void CCal3DCoreSkeletonCache::Serialize( IArchive &ar )
{
	if (ar.IsReading())
	{
		void *pData;
		ar.Read( &pData, sizeof(void*) );

//		Cal3DDataStream dataStream( pData );
//		m_pCoreSkeleton = CalLoader::loadCoreSkeleton(dataStream);
		m_pCoreSkeleton = CalLoader::loadCoreSkeleton(pData);
	}
	else
	{
		assert( FALSE );
//		ar.Write(  );
	}
}
