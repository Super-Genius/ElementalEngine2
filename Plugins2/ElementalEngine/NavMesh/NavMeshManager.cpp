///==========================================================================
/// \file	CNavMeshManager.cpp
/// \brief	Implementation of NavMesh
/// \date	03-24-2005
/// \author	Brian Bazyk
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
///==========================================================================

#include "StdAfx.h"
#include "NavMeshManager.h"
#include "NavMeshRenderObject.h"
#include <math.h>
#include <list>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define NAVMESHPRIORITY     0x0587

#define DEFAULT_HEIGHTMAP_SIZE 64

REGISTER_COMPONENT_SINGLETON( CNavMeshManager );
REGISTER_MESSAGE_HANDLER(Start, OnStart, CNavMeshManager);
REGISTER_MESSAGE_HANDLER(Stop, OnStop, CNavMeshManager);
//REGISTER_MESSAGE_HANDLER(MouseMove, OnMouseMove, CNavMeshManager);

IComponent *CNavMeshManager::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE( CNavMeshManager );
}

CNavMeshManager::CNavMeshManager() : CManager( _T("CNavMeshManager"), 0, NAVMESHPRIORITY )
{
}

CNavMeshManager::~CNavMeshManager()
{
}

DWORD CNavMeshManager::OnStart(DWORD size, void *data)
{
	m_bIsRunning = true;
	
	return MSG_HANDLED_PROCEED;
}

DWORD CNavMeshManager::OnStop(DWORD size, void *data)
{
	m_bIsRunning = false;
	
	return MSG_HANDLED_PROCEED;
}

DWORD CNavMeshManager::OnMouseMove(DWORD size, void *data)
{
	// UNDONE: rewrite with new map of all groups of objects in CManager
	// for each type of object that we manage
	const TCHAR *vObjectTypes[] = {
//		_T("CNavMeshObject"),
		_T("CNavMeshRenderObject"),
		NULL
	};
	IDTOOBJECTMAP *pObjectMap = NULL;
	for (int i=0; vObjectTypes[i] != NULL; i++)
	{
		CHashString vObjTypeName(vObjectTypes[i]);
		pObjectMap = GetObjectMap( &vObjTypeName );
		if (!pObjectMap)
			continue;

		IDTOOBJECTMAP::iterator objIter;
		for( objIter = pObjectMap->begin(); objIter != pObjectMap->end(); objIter++ )
		{
			CNavMeshRenderObject *destObject = (CNavMeshRenderObject*)objIter->second;
			destObject->OnMouseMove(size, data);
		}
	}
	return MSG_HANDLED_PROCEED;
}
