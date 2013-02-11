///============================================================================
/// \file		VariableList.cpp
/// \brief		Implmentation of CVariableList Object Class
/// \date		08-02-2005
/// \author		Kenneth Hurley
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

#include "stdafx.h"
#include "VariableList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define VARDBCOLUMNS 5

REGISTER_COMPONENT(CVariableList);

CVariableList::CVariableList() :
	m_hszNameType( _T("CVariableList") )
{
	m_ToolBox = EngineGetToolBox();
}

CVariableList::~CVariableList( void )
{

}

IComponent *CVariableList::Create(int nArgs, va_list argptr)
{
	IComponent *self;

	// create the object however you must create it
	try
	{
		// try and create an instance
		self = new CVariableList();
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

void CVariableList::Serialize( IArchive &ar )
{
}

void CVariableList::Init()
{
}

IHashString* CVariableList::GetComponentType( void )
{
	return &m_hszNameType;
}

bool CVariableList::IsKindOf( IHashString* compType )
{
	return (compType->GetUniqueID() == m_hszNameType.GetUniqueID() );
}

// add an database with VarDatabase schema to variable access list
void CVariableList::AddDatabase(DATABASEINFO *dbInfo)
{
	DATABASEDATA *dbItem;
	StdString varName;
	CHashString newVarName;
	DWORD i;

	dbItem = dbInfo->m_DatabaseData;
	// loop though 5 dbItems and build string/value
	for (i=0; i< dbInfo->m_nItems / VARDBCOLUMNS; i++)
	{
		// parent name
		varName = (TCHAR *)dbItem[0].m_VoidPtr;
		if (_tcslen((TCHAR *)dbItem[1].m_VoidPtr) != 0)
		{
			if (!varName.empty())
				varName += _T("::");
			// name of item
			varName += (TCHAR *)dbItem[1].m_VoidPtr;
		}

		if (!varName.empty())
		{
			varName += _T("::");
		}

		// variable name, this cannot be empty
		varName += (TCHAR *)dbItem[2].m_VoidPtr;
		// skip dbItem[3] as it is the type for combo box in editor

		// set just to get uniqueID, keeps in string table in toolbox
		newVarName = varName.c_str();
		m_Variables[newVarName.GetUniqueID()] = &dbItem[4];
		dbItem += VARDBCOLUMNS;
	}
}

// add an objects database with VarDatabase schema to 
// variable access list
void CVariableList::AddObjectDatabases(IHashString *name)
{
	GETDATABASEBYSCHEMA gdbbs;
	CHashString hszSchema(_T("VarDatabase"));
	CHashString hszDBType(_T("CDatabase"));
	DATABASEINFOLIST dbInfoList;
	DATABASEINFOLIST::iterator dbilIter;

	dbInfoList.clear();
	gdbbs.m_SchemaName = &hszSchema;
	gdbbs.m_dbInfoList = &dbInfoList;
	static DWORD msgHash_GetDatabaseBySchema = CHashString(_T("GetDatabaseBySchema")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetDatabaseBySchema, sizeof(GETDATABASEBYSCHEMA),
		&gdbbs, name, &hszDBType );

	// ok, now we should have a list of VarDatabase schema type databases,
	// so let's build our variable list from this.
	for (dbilIter = dbInfoList.begin(); dbilIter != dbInfoList.end(); 
			++dbilIter)
	{
		AddDatabase(*dbilIter);
	}
}

// Retrieve a variable from the map
bool CVariableList::GetValue(IHashString *varName, DATABASEDATA **outValue)
{
	VARIABLEMAP::iterator vmIter;
	CHashString checkName(varName);
	StdString nextName;

	if (outValue == NULL)
		return false;

	// initialize to zero
	*outValue = NULL;

	while (*outValue == NULL)
	{
		// first look it up the name in the map.
		vmIter = m_Variables.find(checkName.GetUniqueID());
		if (vmIter != m_Variables.end())
		{
			*outValue = vmIter->second;
		}
		else
		{
			StdString::size_type indexStart;
			StdString::size_type indexEnd;

			nextName = checkName.GetString();
			// now strip off one level of subclass
			// first :: precedes variable name
			indexEnd = nextName.rfind(_T("::"));
			if ((indexEnd == StdString::npos) || (indexEnd == 0))
			{
				// no variable found, bail
				return false;
			}
			// this one should precede the sub-class name
			indexStart = nextName.rfind(_T("::"), indexEnd-1);
			if (indexStart == StdString::npos)
			{
				return false;
			}

			// remove the sub-class name
			nextName.erase(indexStart+2, indexEnd-indexStart);
			checkName = nextName.c_str();
		}

	}

	return true;
}

// Set the variable in a map
bool CVariableList::SetValue(IHashString *varName, DATABASEDATA *inValue)
{
	DATABASEDATA *oldData;
	
	if (!GetValue(varName, &oldData))
	{
		return false;
	}

	// copy the data to the old area
	*oldData = *inValue;

	return true;
}
