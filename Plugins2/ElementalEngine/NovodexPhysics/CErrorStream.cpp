///============================================================================
/// \file		CErrorStream.cpp
/// \brief		Implementations of Novodex's NxUserOutputStream interface
/// \date		12-08-2006
/// \author		Kyle Swaim
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

#include "StdAfx.h"

#include "NxPhysics.h"
#include "CErrorStream.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CErrorStream::CErrorStream()
{
	m_ToolBox = EngineGetToolBox();
}

CErrorStream::~CErrorStream()
{
}

void CErrorStream::reportError( NxErrorCode code, const char* message, const char* file, int line )
{
	if( code == NXE_INTERNAL_ERROR )
	{
		m_ToolBox->Log( LOGERROR, _T("Internal PhysX Error in %s on line %d!\n"), file, line );
		m_ToolBox->Log( LOGERROR, _T("%s\n"), message );
	}
	else if( code == NXE_DB_WARNING )
	{
		m_ToolBox->Log( LOGINFORMATION, _T("Internal PhysX Warning in %s on line %d!\n"), file, line );
		m_ToolBox->Log( LOGINFORMATION, _T("%s\n"), message );
	}
	else if( code == NXE_DB_INFO )
	{
		m_ToolBox->Log( LOGINFORMATION, _T("Internal PhysX Information in %s on line %d!\n"), file, line );
		m_ToolBox->Log( LOGINFORMATION, _T("%s\n"), message );
	}
	else
	{
		m_ToolBox->Log( LOGINFORMATION, _T("Internal PhysX Information in %s on line %d!\n"), file, line );
		m_ToolBox->Log( LOGINFORMATION, _T("%s\n"), message );
	}
}

NxAssertResponse CErrorStream::reportAssertViolation( const char* message, const char* file, int line )
{
	return NX_AR_IGNORE;
}