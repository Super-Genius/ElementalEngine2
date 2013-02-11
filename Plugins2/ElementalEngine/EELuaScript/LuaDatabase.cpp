///============================================================================
/// \file		LuaDatabase.cpp
/// \brief		Implementation for CLuaDatabase
/// \date		09-13-2005
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

CLuaDatabase::CLuaDatabase(TCHAR *name)
{
	DBCREATEPARAMS dbcp;

	dbcp.dbObj = &m_Database;
	dbcp.fileName = name;

	// grab the toolbox
	m_ToolBox = EngineGetToolBox();

	m_Database = NULL;
	m_DatabaseData = NULL;

	static DWORD msgHash_CreateGlobalDatabase = CHashString(_T("CreateGlobalDatabase")).GetUniqueID();
	if (!m_ToolBox->SendMessage(msgHash_CreateGlobalDatabase, sizeof(dbcp), &dbcp) == MSG_HANDLED)
	{
		// couldn't create global database!
		// log error
		StdString error;
		error = _T("Couldn't Create Global Database.\n");
		EngineGetToolBox()->SetErrorValue(WARN_INVALID_OPERATION);
		EngineGetToolBox()->Log(LOGWARNING, error);
	}
	m_DatabaseName.Init( m_Database->GetName()->GetString() );
}

CLuaDatabase::~CLuaDatabase()
{
	if (m_Database)
	{
		DELETEOBJECTPARAMS dop;
		dop.name = &m_DatabaseName;
		static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof(dop), &dop);
		m_Database = NULL;
	}
}

void CLuaDatabase::FindRow(IHashString *key)
{
	if (m_Database)
	{
		// find the row in the database
		m_DatabaseData = (LUADATABASEDATA*)m_Database->FindRow(key);
	}
}

void CLuaDatabase::GetRow(int index)
{
	if (m_Database)
	{
		// get the row from the database
		m_DatabaseData = (LUADATABASEDATA*)m_Database->GetRow(index);
	}
}

char CLuaDatabase::GetCharValue(int index)
{
	char a = 'a';

	if (m_DatabaseData)
	{
		return m_DatabaseData[index].GetCharValue();
	}

	return a;
}

unsigned char CLuaDatabase::GetUCharValue(int index)
{
	unsigned char a = 'a';

	if (m_DatabaseData)
	{
		return m_DatabaseData[index].GetUCharValue();
	}

	return a;
}

short CLuaDatabase::GetShortValue(int index)
{
	short a = 0;

	if (m_DatabaseData)
	{
		return m_DatabaseData[index].GetShortValue();
	}

	return a;
}

unsigned short CLuaDatabase::GetUShortValue(int index)
{
	unsigned short a = 0;

	if (m_DatabaseData)
	{
		return m_DatabaseData[index].GetUShortValue();
	}

	return a;
}

long CLuaDatabase::GetLongValue(int index)
{
	long a = 0;

	if (m_DatabaseData)
	{
		return m_DatabaseData[index].GetLongValue();
	}

	return a;
}

unsigned long CLuaDatabase::GetULongValue(int index)
{
	unsigned long a = 0;

	if (m_DatabaseData)
	{
		return m_DatabaseData[index].GetULongValue();
	}

	return a;
}

float CLuaDatabase::GetFloatValue(int index)
{
	float a = 0.0f;

	if (m_DatabaseData)
	{
		return m_DatabaseData[index].GetFloatValue();
	}

	return a;
}

double CLuaDatabase::GetDoubleValue(int index)
{
	double a = 0.0;

	if (m_DatabaseData)
	{
		return m_DatabaseData[index].GetDoubleValue();
	}

	return a;
}

bool CLuaDatabase::GetBoolValue(int index)
{
	bool a = false;

	if (m_DatabaseData)
	{
		return m_DatabaseData[index].GetBoolValue();
	}

	return a;
}

char* CLuaDatabase::GetStringValue(int index)
{
	if (m_DatabaseData)
	{
		return m_DatabaseData[index].GetStringValue();
	}

	return NULL;
}

int CLuaDatabase::GetNumRows()
{
	if (m_Database)
	{
		return m_Database->GetNumRows();
	}

	return 0;
}

int CLuaDatabase::GetNumItems(int index)
{
	if (m_Database)
	{
		return m_Database->GetNumItems();
	}

	return 0;
}

