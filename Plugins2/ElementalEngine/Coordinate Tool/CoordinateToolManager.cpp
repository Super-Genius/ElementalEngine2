///============================================================================
/// \file		CoordinateToolManager.cpp
/// \brief		Implmentation of CCoordinateToolManager Class
/// \date		08-03-2007
/// \author		Richard Nguyen
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

#include "stdafx.h"
#include "PerformanceProfiler.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT_SINGLETON(CCoordinateToolManager);

// Default Constructor
CCoordinateToolManager::CCoordinateToolManager( void ) :
	CManager(_T("CCoordinateToolManager"), COORDINATETOOLPRIORITY, COORDINATETOOLPRIORITY)
{
	m_ToolBox = EngineGetToolBox();
}

// Default Destructor
CCoordinateToolManager::~CCoordinateToolManager( void )
{
	CManager::DeInit();
}

IComponent *CCoordinateToolManager::Create(int nArgs, va_list argptr)
{
	// Return the singleton instance of this class
  	return SINGLETONINSTANCE( CCoordinateToolManager );
}

bool CCoordinateToolManager::Init()
{
	return true;
}

void CCoordinateToolManager::Update(DWORD tickCount)
{
}

void CCoordinateToolManager::Serialize( IArchive &ar )
{
}

IHashString* CCoordinateToolManager::GetComponentType()
{
	static CHashString hszComponentType( "CCoordinateToolManager" );
	return &hszComponentType;
}