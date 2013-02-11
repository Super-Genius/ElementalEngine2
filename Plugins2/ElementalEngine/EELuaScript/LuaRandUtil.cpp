///============================================================================
/// \file		LuaRandUtil.cpp
/// \brief		Implementation for CLuaRandUtil
/// \date		09-21-2005
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


#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CLuaRandUtil::CLuaRandUtil() {}

CLuaRandUtil::CLuaRandUtil(unsigned long seed) : m_Random(seed) {}

CLuaRandUtil::~CLuaRandUtil(void) {}

void CLuaRandUtil::SetSeed(unsigned long seed)
{
	m_Random.SetSeed(seed);
}

unsigned long CLuaRandUtil::GetRandomNumber()
{
	return m_Random.GetRandomNumber();
}

double CLuaRandUtil::GetRandom()
{
	return m_Random.GetRandom();
}
