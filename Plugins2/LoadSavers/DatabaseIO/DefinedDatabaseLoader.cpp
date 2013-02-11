///============================================================================
/// \file		DefinedDatabaseLoader.cpp
/// \brief		Implementation file for Defined Database Loader
/// \date		04-18-2007
/// \author		Richard Nguyen
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

#include "StdAfx.h"

REGISTER_COMPONENT_SINGLETON(CDefinedDatabaseLoader);
REGISTER_MESSAGE_HANDLER(LoadFile, OnLoadFile, CDefinedDatabaseLoader);
REGISTER_MESSAGE_HANDLER(SaveFile, OnSaveFile, CDefinedDatabaseLoader);

CDefinedDatabaseLoader::CDefinedDatabaseLoader() :
	LoadSaverBase( _T("CDefinedDatabaseLoader"), _T("CQHDatabaseManager"), _T("CDefinedDatabase") )
{
	RegisterLoadSaveComponent();
}

const TCHAR *CDefinedDatabaseLoader::GetLoadDescription()
{
	return _T("Defined Database XML Files (*.dbx)\0" "*.dbx\0" "\0\0");
}

const TCHAR *CDefinedDatabaseLoader::GetSaveDescription()
{
	return _T("Defined Database XML Files (*.dbx)\0" "*.dbx\0" "\0\0");
}

bool CDefinedDatabaseLoader::GetPrecachableBool( )
{
	return false;
}

CACHABLEQUERYCALLBACK CDefinedDatabaseLoader::GetPrecachableFunc()
{
	return NULL;
}

// create function for this world loader
IComponent *CDefinedDatabaseLoader::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE( CDefinedDatabaseLoader );
}


DWORD CDefinedDatabaseLoader::OnLoadFile( DWORD size, void *params )
{
	DWORD retVal = MSG_HANDLED_STOP;
	VERIFY_MESSAGE_SIZE( size, sizeof(TCHAR *) );
	TCHAR *fileName = (TCHAR*)params;
	CHashString hszFileName( fileName );
	
	static DWORD msgHash_FindDatabaseByFileName = CHashString(_T("FindDatabaseByFileName")).GetUniqueID();
	DBFINDDATABASE param;
	param.m_DatabaseFileName = &hszFileName;
	// Check if the file already has been loaded
	DWORD findObjectRetVal = m_ToolBox->SendMessage(msgHash_FindDatabaseByFileName, sizeof(DBFINDDATABASE), &param);
	if( ( findObjectRetVal == MSG_HANDLED ) && (param.m_bIsFound == false) )
	{
		EngineGetToolBox()->Log(LOGWARNING, _T("Database File Loaded: %s\n"), fileName);
		IXMLArchive *ar = CreateXMLArchive( fileName );
		retVal = LoadSaverBase::LoadFile( ar, &hszFileName );
		ar->Close();
		return retVal;
	}
	
	EngineGetToolBox()->Log(LOGWARNING, _T("Database File Already Loaded!: %s\n"), fileName);
	return retVal;
}