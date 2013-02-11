///============================================================================
/// \file		ExampleLoader.cpp
/// \brief		Implementation file for Example Loader
/// \date		04-04-2005
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

#include "StdAfx.h"

REGISTER_COMPONENT_SINGLETON(CExampleLoader);
REGISTER_MESSAGE_HANDLER(LoadFile, OnLoadExampleFile, CExampleLoader);
REGISTER_MESSAGE_HANDLER(SaveFile, OnSaveExampleFile, CExampleLoader);

CHashString CExampleLoader::m_HashName(_T("CExampleLoader"));

LSRC_MESSAGE CExampleLoader::m_LoadSaveRegisterData = {
		&CExampleLoader::m_HashName,
		_T("Example Dummy Files (*.xmp)\0" "*.xmp\0"
		"\0\0"),
		_T("Example Dummy Files (*.xmp)\0" "*.xmp\0"
		"\0\0"),
};

CExampleLoader::CExampleLoader() 
{
	// save for convenience
	m_ToolBox = EngineGetToolBox();
	static DWORD msgHash_RegisterLoadSaveComponent = CHashString(_T("RegisterLoadSaveComponent")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_RegisterLoadSaveComponent, sizeof(LSRC_MESSAGE), &m_LoadSaveRegisterData);
}

// create function for this Example loader
IComponent *CExampleLoader::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE( CExampleLoader );
}

IHashString *CExampleLoader::GetComponentType()
{
	return &m_HashName;
}

bool CExampleLoader::IsKindOf(IHashString *compType)
{
	return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
}

// load a file if we recognize the type.
DWORD CExampleLoader::OnLoadExampleFile(DWORD size, void *params)
{
	// Load the file

	// Add to manager

	// Send Message that it was loaded, we could send data, or IArchive
	static DWORD msgHash_EditExample = CHashString(_T("EditExample")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_EditExample, 0, NULL);

	return MSG_HANDLED_STOP;
}

// Save a file if we recognize the type.
DWORD CExampleLoader::OnSaveExampleFile(DWORD size, void *params)
{
	return MSG_HANDLED_STOP;
}
