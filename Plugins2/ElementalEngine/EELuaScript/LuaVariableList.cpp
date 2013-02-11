///============================================================================
/// \file		LuaVariableList.cpp
/// \brief		Implementation file for CLuaVariableList
/// \date		08-17-2005
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
#include ".\luavariablelist.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CLuaVariableList::CLuaVariableList(void)
{
	// get name and type
	CHashString typeName(_T("CVariableList"));

	m_ToolBox = EngineGetToolBox();

	m_VariableList = NULL;

	// create object
	m_VariableList = dynamic_cast<IVariableList *>(
		m_ToolBox->CreateComponent(&typeName, 0));
}

CLuaVariableList::~CLuaVariableList(void)
{
    //Currently, the VariableList dlo doesn't have a manager to keep track of these
    //and delete them automagically.  So we must do it ourselves.  
    if (m_VariableList)
    {
        delete m_VariableList;
    }
}

void CLuaVariableList::AddDatabase(IHashString *name)
{
	DATABASEINFO newInfo;
	newInfo.m_FileName = name;
	static DWORD msgHash_GetDatabase = CHashString(_T("GetDatabase")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetDatabase, sizeof(DATABASEINFO), &newInfo);

	if (m_VariableList)
	{
		m_VariableList->AddDatabase(&newInfo);
	}
}


void CLuaVariableList::AddObjectDatabases(IHashString *name)
{
	if (m_VariableList)
	{
		m_VariableList->AddObjectDatabases(name);
	}
}

char CLuaVariableList::GetValueChar(IHashString *varName)
{
	DATABASEDATA *outValue;

	if (m_VariableList)
	{
		m_VariableList->GetValue(varName, &outValue);
	}

	return outValue->m_CharValue;
}

unsigned char CLuaVariableList::GetValueUChar(IHashString *varName)
{
	DATABASEDATA *outValue;

	if (m_VariableList)
	{
		m_VariableList->GetValue(varName, &outValue);
	}

	return outValue->m_UCharValue;
}

short CLuaVariableList::GetValueShort(IHashString *varName)
{
	DATABASEDATA *outValue;

	if (m_VariableList)
	{
		m_VariableList->GetValue(varName, &outValue);
	}

	return outValue->m_ShortValue;
}

unsigned short CLuaVariableList::GetValueUShort(IHashString *varName)
{
	DATABASEDATA *outValue;

	if (m_VariableList)
	{
		m_VariableList->GetValue(varName, &outValue);
	}

	return outValue->m_UShortValue;
}

long CLuaVariableList::GetValueLong(IHashString *varName)
{
	DATABASEDATA *outValue;

	if (m_VariableList)
	{
		m_VariableList->GetValue(varName, &outValue);
	}

	return outValue->m_LongValue;
}

unsigned long CLuaVariableList::GetValueULong(IHashString *varName)
{
	DATABASEDATA *outValue;

	if (m_VariableList)
	{
		m_VariableList->GetValue(varName, &outValue);
	}

	return outValue->m_ULongValue;
}

float CLuaVariableList::GetValueFloat(IHashString *varName)
{
	DATABASEDATA *outValue;

	if (m_VariableList)
	{
		m_VariableList->GetValue(varName, &outValue);
	}

	return outValue->m_FloatValue;
}

double CLuaVariableList::GetValueDouble(IHashString *varName)
{
	DATABASEDATA *outValue;

	if (m_VariableList)
	{
		m_VariableList->GetValue(varName, &outValue);
	}

	return outValue->m_DoubleValue;
}

bool CLuaVariableList::GetValueBool(IHashString *varName)
{
	DATABASEDATA *outValue;

	if (m_VariableList)
	{
		m_VariableList->GetValue(varName, &outValue);
	}

	return outValue->m_BoolValue;
}

TCHAR* CLuaVariableList::GetValueTCHAR(IHashString *varName)
{
	DATABASEDATA *outValue;

	if (m_VariableList)
	{
		m_VariableList->GetValue(varName, &outValue);
	}

	return outValue->m_TCharPtr;
}

void* CLuaVariableList::GetValueVoidPtr(IHashString *varName)
{
	DATABASEDATA *outValue;

	if (m_VariableList)
	{
		m_VariableList->GetValue(varName, &outValue);
	}

	return outValue->m_VoidPtr;
}

/// sets

void CLuaVariableList::SetValueChar(IHashString *varName, char value)
{
	DATABASEDATA dbItem;

	dbItem.type = DBTYPE_CHAR;
	dbItem.m_CharValue = value;

	if (m_VariableList)
	{
		m_VariableList->SetValue(varName, &dbItem);
	}
}

void CLuaVariableList::SetValueUChar(IHashString *varName, unsigned char value)
{
	DATABASEDATA dbItem;

	dbItem.type = DBTYPE_UCHAR;
	dbItem.m_UCharValue = value;

	if (m_VariableList)
	{
		m_VariableList->SetValue(varName, &dbItem);
	}
}

void CLuaVariableList::SetValueShort(IHashString *varName, short value)
{
	DATABASEDATA dbItem;

	dbItem.type = DBTYPE_SHORT;
	dbItem.m_ShortValue = value;

	if (m_VariableList)
	{
		m_VariableList->SetValue(varName, &dbItem);
	}
}

void CLuaVariableList::SetValueUShort(IHashString *varName, unsigned short value)
{
	DATABASEDATA dbItem;

	dbItem.type = DBTYPE_USHORT;
	dbItem.m_UShortValue = value;

	if (m_VariableList)
	{
		m_VariableList->SetValue(varName, &dbItem);
	}
}

void CLuaVariableList::SetValueLong(IHashString *varName, long value)
{
	DATABASEDATA dbItem;

	dbItem.type = DBTYPE_LONG;
	dbItem.m_LongValue = value;

	if (m_VariableList)
	{
		m_VariableList->SetValue(varName, &dbItem);
	}
}

void CLuaVariableList::SetValueULong(IHashString *varName, unsigned long value)
{
	DATABASEDATA dbItem;

	dbItem.type = DBTYPE_ULONG;
	dbItem.m_ULongValue = value;

	if (m_VariableList)
	{
		m_VariableList->SetValue(varName, &dbItem);
	}
}

void CLuaVariableList::SetValueFloat(IHashString *varName, float value)
{
	DATABASEDATA dbItem;

	dbItem.type = DBTYPE_FLOAT;
	dbItem.m_FloatValue = value;

	if (m_VariableList)
	{
		m_VariableList->SetValue(varName, &dbItem);
	}
}

void CLuaVariableList::SetValueDouble(IHashString *varName, double value)
{
	DATABASEDATA dbItem;

	dbItem.type = DBTYPE_DOUBLE;
	dbItem.m_DoubleValue = value;

	if (m_VariableList)
	{
		m_VariableList->SetValue(varName, &dbItem);
	}
}

void CLuaVariableList::SetValueTCHAR(IHashString *varName, TCHAR *value)
{
	DATABASEDATA dbItem;

	dbItem.type = DBTYPE_CHAR;
	dbItem.m_TCharPtr = value;

	if (m_VariableList)
	{
		m_VariableList->SetValue(varName, &dbItem);
	}
}

void CLuaVariableList::SetValueVoidPtr(IHashString *varName, void *value)
{
	DATABASEDATA dbItem;

	dbItem.type = DBTYPE_VOIDPTR;
	dbItem.m_VoidPtr = value;

	if (m_VariableList)
	{
		m_VariableList->SetValue(varName, &dbItem);
	}
}
