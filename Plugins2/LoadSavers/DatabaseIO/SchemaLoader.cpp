///============================================================================
/// \file		SchemaLoader.cpp
/// \brief		Implementation file for Schema Loader
/// \date		04-17-2007
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

REGISTER_COMPONENT_SINGLETON(CSchemaLoader);
REGISTER_MESSAGE_HANDLER(LoadFile, OnLoadFile, CSchemaLoader);
REGISTER_MESSAGE_HANDLER(SaveFile, OnSaveFile, CSchemaLoader);

CSchemaLoader::CSchemaLoader() :
	LoadSaverBase( _T("CSchemaLoader"), _T("CQHDatabaseManager"), _T("CDatabaseSchema") )
{
	RegisterLoadSaveComponent();
}

const TCHAR *CSchemaLoader::GetLoadDescription()
{
	return _T("Schema XML Files (*.fxl)\0" "*.fxl\0" "\0\0");
}

const TCHAR *CSchemaLoader::GetSaveDescription()
{
	return _T("Schema XML Files (*.fxl)\0" "*.fxl\0" "\0\0");
}

bool CSchemaLoader::GetPrecachableBool( )
{
	return false;
}

CACHABLEQUERYCALLBACK CSchemaLoader::GetPrecachableFunc()
{
	return NULL;
}

// create function for this world loader
IComponent *CSchemaLoader::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE( CSchemaLoader );
}