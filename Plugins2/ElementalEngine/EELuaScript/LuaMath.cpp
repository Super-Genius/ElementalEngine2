///============================================================================
/// \file		LuaMath.cpp
/// \brief		Wraps and exposes MathUtil class to Lua
/// \date		08-25-2006
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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMathUtilImplementation* CLuaMath::m_MathUtilImplementation = NULL;

void CLuaMath::InitImplementation()
{
	ACTIVEMATHIMPLEMENTATION msg;
	msg.dwImplementationName = CHashString( _T("MathUtilDefaultImplementation") ).GetUniqueID();
	static DWORD msgHash_GetActiveMathImplementation = CHashString(_T("GetActiveMathImplementation")).GetUniqueID();
	if( EngineGetToolBox()->SendMessage(msgHash_GetActiveMathImplementation, sizeof(msg), &msg ) != MSG_HANDLED )
	{
		assert(0);
		m_MathUtilImplementation = NULL;
	}
	else
	{
		m_MathUtilImplementation = (IMathUtilImplementation*)msg.pImplementation;
	}
}