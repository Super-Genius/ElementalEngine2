///============================================================================
/// \file	Cal3DCoreMeshCache.cpp
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

REGISTER_COMPONENT( CCal3DCoreMeshCache );
IComponent *CCal3DCoreMeshCache::Create(int nArgs, va_list argptr)
{
	IObject *self;
	IHashString *name = va_arg(argptr, IHashString *);
	IHashString *parentName = va_arg(argptr, IHashString *);
	try
	{
		// try and create an instance
		self = new CCal3DCoreMeshCache(parentName, name);
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

CCal3DCoreMeshCache::CCal3DCoreMeshCache(IHashString *parentName, IHashString *name) : 
	OBJECTTEMPLATE(Cal3DModelManager, CCal3DCoreMeshCache, IObject, parentName, name)
{
	m_pModelManager = (Cal3DModelManager*)m_Manager;
	m_pCoreMesh = NULL;
}

CCal3DCoreMeshCache::~CCal3DCoreMeshCache()
{
	DeInit();
}

void CCal3DCoreMeshCache::Init()
{
	if (m_pCoreMesh)
	{
		m_pCoreMesh->incRef();
	}
}

void CCal3DCoreMeshCache::DeInit()
{
	if (m_pCoreMesh)
	{
		if (m_pCoreMesh->decRef())
			delete m_pCoreMesh;
		m_pCoreMesh = NULL;
	}
}

void CCal3DCoreMeshCache::Serialize( IArchive &ar )
{
	if (ar.IsReading())
	{
		void *pData;
		ar.Read( &pData, sizeof(void*) );

//		Cal3DDataStream dataStream( pData );
//		m_pCoreMesh = CalLoader::loadCoreMesh(dataStream);
		m_pCoreMesh = CalLoader::loadCoreMesh(pData);
	}
	else
	{
		assert( FALSE );
//		ar.Write(  );
	}
}
