///============================================================================
/// \file		LODVisitor.cpp
/// \brief		Implementation file for CLODVisitor class
/// \date		10-10-2005
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

CLODVisitor::CLODVisitor()
{
	m_FilterSet.clear();
	m_EntityList.clear();
	m_ToolBox = EngineGetToolBox();
}

CLODVisitor::~CLODVisitor() 
{
}

bool CLODVisitor::Visit(IComponent * component, bool bVisitEnter)
{	
	IObject *theObject;
	IHashString *objName;
	DWORD objFileHash;
	CHashString tempHash;

	theObject = dynamic_cast<IObject *>(component);
	
	// if the object is invalid
	if (!theObject)
	{
		// log error
		return false;
	}

	// get the name of the object
	objName = theObject->GetName();

	// get the model file name
	static DWORD msgHash_GetModelFileId = CHashString(_T("GetModelFileId")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetModelFileId, sizeof(DWORD), &objFileHash, objName);

	// check if object name is in the filter set
	if (m_FilterSet.find( objFileHash ) != m_FilterSet.end())
	{
		// it is, so add it
		m_EntityList.push_back(theObject);
	}

	return true;
}

bool CLODVisitor::IsHierarchicalVisitor()
{
	return false;
}

void CLODVisitor::SetFilter(const DWORDSET &filter)
{
	m_FilterSet = filter;
}

OBJECTLIST* CLODVisitor::GetEntityList()
{
	return &m_EntityList;
}

void CLODVisitor::ClearEntityList()
{
	return m_EntityList.clear();
}

bool CLODVisitor::CompareNames(StdString searchString, StdString compString)
{
	StdString token(_T(":"));
	int subLen;
	// if searchString is null, positive match w/ everything
	if(_tcscmp(searchString, _T("")) == 0)
	{
		return true;
	}
	// if compString is null, no match
	if(_tcscmp(compString, _T("")) == 0)
	{
		return false;
	}
	const TCHAR* searchStringArray = searchString.c_str();
	const TCHAR* compStringArray = compString.c_str();

	while( _tcscmp(searchStringArray, _T("")) != 0 )
	{
		subLen = _tcscspn(compStringArray, token); //length of substring to ':'

		// compare substrings up to token ':'
		if( _tcsncmp(searchStringArray, compStringArray, subLen) != 0 )
		{
			return false;
		}
		// now check if compString's substring's next char is == token 
		if( compStringArray[subLen] != ':' )
		{
			if( searchStringArray[subLen] == '\0')
			{
				// both compString and searchString at the end
				return true;
			}
			else
			{
				return false;
			}
		}
		// now check searchString's substring's next char is == token or 'null'
		if( searchStringArray[subLen] != ':')
		{
			// check if its null
			if ( searchStringArray[subLen] == '\0' )
			{
				return true;
			}
			else
			{
				return false;
			}
		}	 
		// check that 2nd tokens exist
		if( searchStringArray[subLen+1] != ':')
		{
			// check if its null
			if ( searchStringArray[subLen+1] == '\0' )
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		// continue...advance ptr
		searchStringArray = _tcsninc(searchStringArray, subLen+2);
		compStringArray = _tcsninc(compStringArray, subLen+2);
	}
	// if exited while loop..then true
	return true;
}