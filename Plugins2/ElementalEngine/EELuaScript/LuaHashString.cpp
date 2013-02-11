///============================================================================
/// \file		LuaHashString.cpp
/// \brief		Implementation file for CLuaHashString
/// \date		05-27-2005
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

CLuaHashString::CLuaHashString()
{
	m_szPtr = NULL;
	m_UniqueID = 0;
}

CLuaHashString::CLuaHashString(const TCHAR *szName)
{
	Init(szName);
}

CLuaHashString::CLuaHashString(StdString *szName)
{
	Init(szName->c_str());
}

CLuaHashString::CLuaHashString(IHashString *hashString)
{
	if (hashString)
		Init(hashString->GetString());
	else
		SINGLETONINSTANCE(CLuaManager)->PrintLuaError(_T("NULL Assignment in CLuaHashString:new_local()\n"));
}

CLuaHashString::~CLuaHashString(void)
{
}

void CLuaHashString::Init(const TCHAR *szName)
{
	HASHSTRINGMAP *hsm;
	HASHSTRINGMAP::iterator hsmIter;

	m_ToolBox = EngineGetToolBox();
	
	hsm = m_ToolBox->GetHashStringMap();
	hsmIter = hsm->find(szName);
	if (hsmIter != hsm->end())
	{
		// copy the old data
		m_UniqueID = hsmIter->second;
		m_szPtr = hsmIter->first;
	}
	else
	{
		// fill in new data
		m_UniqueID = m_ToolBox->GetNextUniqueID();
		// duplicate string. HashMap will free it in destructor
		//m_szPtr = _tcsdup(szName);
		// we need constant data here
		//(*hsm)[m_szPtr] = m_UniqueID;

		m_szPtr = m_ToolBox->AddHashString(szName, m_UniqueID);
	}

}

///	operator to compare CHashString to a const TCHAR * 
///	\param szName zero terminated array of chars
bool CLuaHashString::operator==(const TCHAR *szName)
{
	HASHSTRINGMAP *hsm;
	HASHSTRINGMAP::iterator hsmIter;

	hsm = m_ToolBox->GetHashStringMap();
	hsmIter = hsm->find(szName);
	return (hsmIter != hsm->end());
}

bool CLuaHashString::operator==(IHashString &hs)
{
	return (m_UniqueID == hs.GetUniqueID());
}

bool CLuaHashString::operator!=(IHashString &hs)
{
	return (m_UniqueID != hs.GetUniqueID());
}

IHashString& CLuaHashString::operator=(const TCHAR *inStr)
{
	Init(inStr);
	return *this;
}

IHashString& CLuaHashString::operator=(const IHashString &hs)
{
	m_UniqueID = hs.GetUniqueID();
	m_szPtr = hs.GetString();
	return *this;
}

IHashString& CLuaHashString::operator=(const CHashString &hs)
{
	m_UniqueID = hs.GetUniqueID();
	m_szPtr = hs.GetString();
	return *this;
}

/// operator< check if one IHashString is less than other
/// \param hs = pointer to right IHashString
/// \return bool if Left < Right
bool CLuaHashString::operator<(const IHashString *hs) const
{
	return (m_UniqueID < hs->GetUniqueID());
}

/// Get the unique ID associated with this HashString
/// \return DWORD which is unique ID for this HashString
DWORD CLuaHashString::GetUniqueID() const
{
	return m_UniqueID;
}

/// Get the string pointer for this HashString
/// \return const TCHAR * which is pointer to string
const TCHAR* CLuaHashString::GetString() const
{
	return m_szPtr;
}

/// check if string is empty
///	\return true if hash string holds empty value
bool CLuaHashString::IsEmpty() const
{
	static const DWORD hashEmpty = CLuaHashString(_T("")).GetUniqueID();
	return hashEmpty == m_UniqueID;
}

/// Get the string pointer for this HashString
/// \returns const TCHAR * which is pointer to string
StdString CLuaHashString::GetStdString()
{
	StdString ret(m_szPtr);
	return ret;
}

