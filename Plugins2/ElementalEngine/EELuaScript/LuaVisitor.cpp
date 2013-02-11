///============================================================================
/// \file		LuaVisitor.cpp
/// \brief		Implementation file for the Lua Visitor class
/// \date		10-09-2005
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

CLuaVisitor::CLuaVisitor() : m_VisitorNodeName(&m_LuaHashStringList), m_VisitorNodeDWORD(&m_DWORDSet)
{

}

CLuaVisitor::~CLuaVisitor()
{

}

void* CLuaVisitor::GetVisitFuncName()
{
	return (void *)&m_VisitorNodeName;
}

void* CLuaVisitor::GetVisitFuncDWORD()
{
	return (void *)&m_VisitorNodeDWORD;
}

CLuaHashStringList* CLuaVisitor::GetHashStringList()
{
	return &m_LuaHashStringList;
}

CLuaDWORDSet* CLuaVisitor::GetDWORDSet()
{
	return &m_DWORDSet;
}

void CLuaVisitor::ClearDWORDSet()
{
	/// clear the set
	m_DWORDSet.Clear();
}

void CLuaVisitor::ClearHashStringList()
{
	// clear the list
	m_LuaHashStringList.clear();
}