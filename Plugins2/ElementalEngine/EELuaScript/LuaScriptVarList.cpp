///============================================================================
/// \file		LuaScriptVarList.cpp
/// \brief		Header file for Lua Script Variable List
/// \date		01-29-2007
/// \author		D. Patrick Ghiocel
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

REGISTER_COMPONENT(CLuaScriptVarList);
REGISTER_MESSAGE_HANDLER(GetSchema, OnGetSchema, CLuaScriptVarList);


IComponent *CLuaScriptVarList::Create(int nArgs, va_list argptr)
{
	IHashString *name;
	IHashString *parentName;
	IObject *self;

	name = va_arg(argptr, IHashString *);
	parentName = va_arg(argptr, IHashString *);
	// create the object however you must create it
	try
	{
		// try and create an instance
		self = new CLuaScriptVarList(parentName, name);
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

CLuaScriptVarList::CLuaScriptVarList(IHashString *parentName, IHashString *name) :
	OBJECTTEMPLATE( CLuaManager, CLuaScriptVarList, IObject, parentName, name)
{
	m_ToolBox = EngineGetToolBox();
	m_Variables.clear();
	m_SchemaFileName = _T("");

	m_pSchemaDB = NULL;
	
	AddToHierarchy();
}

CLuaScriptVarList::~CLuaScriptVarList( )
{
	DeInit();
	RemoveFromHierarchy();
}

void CLuaScriptVarList::Init()
{
	LoadSchema();
	
	CHashString PName = GetParentName();
	SETTABLEVALUEPARAMS stv;
	for( unsigned int i = 0; i < m_Variables.size(); i++ )
	{
		CHashString hsKey(m_Variables[i].GetName());
		CHashString hsValue(m_Variables[i].GetValueAsString());
		CHashString hsType(m_Variables[i].GetType());
		stv.TableKey = &hsKey;
		stv.TableValue = &hsValue;
		stv.TableType = &hsType;
		static DWORD msgHash_SetPropertyTableValue = CHashString("SetPropertyTableValue" ).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_SetPropertyTableValue, sizeof(stv), &stv, &PName );
	}
}

void CLuaScriptVarList::DeInit()
{
	UnloadSchema();
}

void CLuaScriptVarList::Serialize(IArchive &ar )
{
	if(ar.IsReading())
	{
		// Read the Schema File Name
		StdString tmpFileName;
		ar.Read( tmpFileName, _T("Schema") );
		m_SchemaFileName = tmpFileName;

		// Read the VarCount for Parsing Purposes
		unsigned int VarCount = 0;
		ar.Read( VarCount, _T("VarCount") );

		for( unsigned int i = 0; i < VarCount; i++ )
		{
			CLuaScriptVar tmpVar;
			StdString tmpStringName;
			StdString tmpStringValue;
			bool tmpBoolIsDefault;
			ar.Read( tmpStringName, _T("Name")  );
			ar.Read( tmpStringValue, _T("Value") );
			ar.Read( tmpBoolIsDefault, _T("Default") );
			tmpVar.SetName( tmpStringName );
			tmpVar.SetValue( tmpStringValue );
			tmpVar.SetIsDefault( tmpBoolIsDefault );

			bool bFound = false;

			for( unsigned int j = 0; j < m_Variables.size(); j++ )
			{
				if( tmpVar.GetName() == m_Variables[j].GetName() )
				{
					bFound = true;
					if( tmpVar.GetValueAsString() != m_Variables[j].GetValueAsString() )
					{
						m_Variables[j].SetValue( tmpVar.GetValueAsString() );
						m_Variables[j].SetIsDefault( false );
					}
					break;
				}
			}

			if( bFound == false )
			{
				m_Variables.push_back( tmpVar );
			}
		}
	}

	else
	{
		ar.Write( m_SchemaFileName.GetString(), _T("Schema") );

		// Read the VarCount for Parsing Purposes
		ar.Write( (unsigned int)m_Variables.size(), _T("VarCount") );

		for( unsigned int i = 0; i < m_Variables.size(); i++ )
		{
			ar.StartClass(_T("CLuaScriptVar") );
			ar.Write( m_Variables[i].GetName(), _T("Name") );
			ar.Write( m_Variables[i].GetValueAsString(), _T("Value") );
			ar.Write( m_Variables[i].GetIsDefault(), _T("Default") );
			ar.EndClass();
		}
	}

}

void CLuaScriptVarList::UnloadSchema()
{
	if (m_pSchemaDB)
	{
		DELETEOBJECTPARAMS dop;
		dop.name = m_pSchemaDB->GetName();
		static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
		EngineGetToolBox()->SendMessage(msgHash_DeleteObject, sizeof(dop), &dop);

		m_pSchemaDB = NULL;
	}
}

void CLuaScriptVarList::LoadSchema( void )
{
	UnloadSchema();

	DWORD retval;
	
	DBCREATEPARAMS dbparam;
	dbparam.fileName = (TCHAR*)m_SchemaFileName.GetString();
	dbparam.dbObj = &m_pSchemaDB;

	static DWORD msgHash_CreateGlobalDatabase = CHashString(_T("CreateGlobalDatabase")).GetUniqueID();
	retval = EngineGetToolBox()->SendMessage(msgHash_CreateGlobalDatabase, sizeof(DBCREATEPARAMS), &dbparam);
	if( retval != MSG_HANDLED )
	{
		m_ToolBox->SetErrorValue(WARN_INVALID_FILE);
		m_ToolBox->Log(LOGWARNING, _T("LuaScriptVarList: Could Not Create Global Database From File %s\n"), m_SchemaFileName.GetString());
	}
	if( m_pSchemaDB == NULL )
	{
		m_ToolBox->SetErrorValue(WARN_INVALID_FILE);
		m_ToolBox->Log(LOGWARNING, _T("LuaScriptVarList: Could Not Create Global Database From File %s\n"), m_SchemaFileName.GetString());
		return;
	}
	CheckVarList();
}

void CLuaScriptVarList::CheckVarList( void )
{
	// Schema: <Name> <Type> <Value> <Min> <Max>
	DATABASEDATA* dbItem;
	LUAVARVEC tmpVarVec;

	for( int i = 0; i < m_pSchemaDB->GetNumRows(); i++ )
	{
		dbItem = m_pSchemaDB->GetRow(i);
		StdString dbItemName( dbItem[0].m_TCharPtr );
		bool bFound = false;

		// look for it in the Varlist
		for( unsigned int j = 0; j < m_Variables.size(); j++ )
		{
			if( m_Variables[j].GetName() == dbItemName )
			{
				bFound = true;

				if( m_Variables[j].GetIsDefault() )
				{
					CLuaScriptVar tmpVar;
					StdString dbItemType = dbItem[1].m_TCharPtr;
					StdString dbItemValue = dbItem[2].m_TCharPtr;

					tmpVar.SetName( dbItemName );
					tmpVar.SetType( dbItemType );
					tmpVar.SetValue( dbItemValue );
					tmpVar.SetIsDefault( true );
					tmpVarVec.push_back( tmpVar );
				}

				else
				{
					CLuaScriptVar tmpVar;
					StdString dbItemType = dbItem[1].m_TCharPtr;
					StdString dbItemValue = dbItem[2].m_TCharPtr;

					tmpVar.SetName( dbItemName );
					tmpVar.SetType( dbItemType );
					tmpVar.SetValue( m_Variables[j].GetValueAsString() );
					tmpVar.SetIsDefault( false );
					tmpVarVec.push_back( tmpVar );
				}

				break;
			}
		}

		// if not in the varlist, create it with the default value
		if( !bFound )
		{
			CLuaScriptVar tmpVar;
			StdString dbItemType = dbItem[1].m_TCharPtr;
			StdString dbItemValue = dbItem[2].m_TCharPtr;

			tmpVar.SetName( dbItemName );
			tmpVar.SetType( dbItemType );
			tmpVar.SetValue( dbItemValue );
			tmpVar.SetIsDefault( true );
			tmpVarVec.push_back( tmpVar );
		}
	}

	m_Variables = tmpVarVec;
}

DWORD CLuaScriptVarList::OnGetSchema(DWORD size, void *in_params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(GETSCHEMAPARAMS));
	GETSCHEMAPARAMS *pParams = (GETSCHEMAPARAMS*)in_params;
	IXMLArchive *pArchive = pParams->ar;
	if (!pArchive)
		return MSG_ERROR;

	int iSchemaItemCount = 2 + m_Variables.size() * 3;
	pArchive->StartClass( StdString(GetComponentType()->GetString()) );
	pArchive->Write( GetName()->GetString(), "Name" );
	pArchive->Write( iSchemaItemCount, "Num" );

	// hidden Schema Name
	pArchive->StartClass(_T("hidden") );
	pArchive->Write( _T("string"), _T("Type") );
	pArchive->EndClass();
	// hidden Variable count
	pArchive->StartClass(_T("hidden") );
	pArchive->Write( _T("int"), _T("Type") );
	pArchive->EndClass();
	// Variables
	for( unsigned int i = 0; i < m_Variables.size(); i++ )
	{
		// hidden Variable Name string
		pArchive->StartClass(_T("hidden") );
		pArchive->Write( _T("string"), _T("Type") );
		pArchive->EndClass();
		// shown Variable Value string
		pArchive->StartClass(_T("edit") );
		pArchive->Write( _T("string"), _T("Type") );
		pArchive->Write( m_Variables[i].GetName(), _T("Name") );
		pArchive->Write( false, _T("Key") );
		pArchive->EndClass();
		// hidden Variable Default bool
		pArchive->StartClass(_T("hidden") );
		pArchive->Write( _T("bool"), _T("Type") );
		pArchive->EndClass();
	}
	pArchive->EndClass();

	return MSG_HANDLED_STOP;
}

DWORD CLuaScriptVarList::OnReloadSchemas(DWORD size, void *in_params)
{
	return MSG_HANDLED_STOP;
}

