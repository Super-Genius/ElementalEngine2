///============================================================================
/// \file		LuaMatrix3x3.cpp
/// \brief		Implementation file for CLuaMatrix3x3
/// \date		08-25-2005
/// \author		Josh Lulewicz
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

IMatrix3x3Implementation *CLuaMatrix3x3::m_Matrix3x3Implementation;

void CLuaMatrix3x3::InitImplementation()
{
	ACTIVEMATHIMPLEMENTATION msg;
	msg.dwImplementationName = CHashString(_T("Matrix3x3DefaultImplementation")).GetUniqueID();
	static DWORD msgHash_GetActiveMathImplementation = CHashString(_T("GetActiveMathImplementation")).GetUniqueID();
	if (EngineGetToolBox()->SendMessage(msgHash_GetActiveMathImplementation, sizeof(msg), &msg ) != MSG_HANDLED)
	{
		assert(0);
		m_Matrix3x3Implementation = NULL;
	}
	else
	{
		m_Matrix3x3Implementation = (IMatrix3x3Implementation *)msg.pImplementation;
	}
}