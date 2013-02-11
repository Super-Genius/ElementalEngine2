///============================================================================
/// \file		PhysicsLoader.cpp
/// \brief		Loader for physics data files
/// \date		10-06-2006
/// \author		Kyle Swaim
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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT_SINGLETON( CPhysicsLoader );
REGISTER_MESSAGE_HANDLER( LoadFile, OnLoadFile, CPhysicsLoader );
REGISTER_MESSAGE_HANDLER( SaveFile, OnSaveFile, CPhysicsLoader );

CHashString CPhysicsLoader::m_HashName( _T("CPhysicsLoader") );

LSRC_MESSAGE CPhysicsLoader::m_LoadSaveRegisterData = {
		&CPhysicsLoader::m_HashName,
		// Loaders
		"Physics Collada Files (*.dae)\0" "*.dae\0"
		"Physics Binary Files (*.nxb)\0" "*.nxb\0"
		"\0\0",
		// Savers
		"Physics Collada Files (*.dae)\0" "*.dae\0"
		"Physics Binary Files (*.nxb)\0" "*.nxb\0"
		"\0\0",
		true, NULL,
};

CPhysicsLoader::CPhysicsLoader()
{
	m_ToolBox = EngineGetToolBox();
	static DWORD msgHash_RegisterLoadSaveComponent = CHashString(_T("RegisterLoadSaveComponent")).GetUniqueID();
	DWORD result = m_ToolBox->SendMessage(msgHash_RegisterLoadSaveComponent, sizeof(LSRC_MESSAGE), &m_LoadSaveRegisterData, NULL, NULL );

	if( !result )
	{
		m_ToolBox->Log( LOGERROR, _T("PhysicsLoader: Failed to register Physics load/saver!\n") ); 
	}
}

// create function for this Particle loader
IComponent *CPhysicsLoader::Create( int nArgs, va_list argptr )
{
	return SINGLETONINSTANCE( CPhysicsLoader );
}

IHashString *CPhysicsLoader::GetComponentType()
{
	return &m_HashName;
}

bool CPhysicsLoader::IsKindOf( IHashString *compType )
{
	return ( compType->GetUniqueID() == GetComponentType()->GetUniqueID() );
}

DWORD CPhysicsLoader::OnLoadFile( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(TCHAR *) );
	TCHAR *pFileName = (TCHAR*)params;

	if( pFileName == NULL )
	{
		m_ToolBox->Log( LOGERROR, _T("CPhysicsLoader: Bad physics file name. %s\n"), pFileName ); 
		return MSG_ERROR;
	}

	StdString extension, currentToken, fileName;
	fileName = pFileName;
	fileName.GetToken( _T("."), currentToken );
	while( _tcscmp( currentToken, _T("") ) != 0 )
	{ 
		extension = currentToken;
		fileName.GetToken( _T("."), currentToken );
	}

	bool result = false;

	// collada file
	if( _tcscmp( extension, _T("dae") ) == 0 )
	{
		result = LoadFile( pFileName, NXU::FT_COLLADA );
	}

	// binary file
	if( _tcscmp( extension, _T("nxb") ) == 0 )
	{
		result = LoadFile( pFileName, NXU::FT_BINARY );
	}

	// XML file
	if( _tcscmp( extension, _T("xml") ) == 0 )
	{
		result = LoadFile( pFileName, NXU::FT_XML );
	}

	// error while loading
	if( !result )
	{
		m_ToolBox->Log( LOGERROR, _T("CPhysicsLoader: Failed to open Physics File %s\n"), pFileName ); 
		return MSG_ERROR;
	}

	return MSG_HANDLED_STOP;
}

bool CPhysicsLoader::LoadFile( TCHAR* fileName, NXU::NXU_FileType type )
{
	StdString szFilePath(fileName);
	szFilePath.MakeSafeFileName();
#ifdef XBOX
	szFilePath = SetPathDrive( szFilePath, EngineGetToolBox()->GetDrive() );
#endif

	CREATESTREAM cs;
	cs.mode = STREAM_MODE_READ;
	cs.streamData = (void*)szFilePath.c_str();
	cs.streamSize = 0;
	static DWORD msgHash_CreateStream_File = CHashString(_T("CreateStream_File")).GetUniqueID();
	DWORD retval = m_ToolBox->SendMessage(msgHash_CreateStream_File, sizeof(CREATESTREAM), &cs);
	if ((retval != MSG_HANDLED) || (cs.openStream == NULL))
	{
		m_ToolBox->Log( LOGERROR, _T("Error Loading Physics File %s\n"), szFilePath.c_str() );
		return false;
	}
	cs.openStream->SeekTo(0, SEEK_END);
	long iFileSize = cs.openStream->Tell();
	void* pFileData = new BYTE[iFileSize];
	cs.openStream->SeekTo(0,SEEK_SET);
	cs.openStream->Read( pFileData, iFileSize );
	cs.openStream->Close();

	NXU::NxuPhysicsCollection* collection = NXU::loadCollection( szFilePath, type, pFileData, iFileSize);

	delete [] pFileData;


	if( collection == NULL )
	{
		return false;
	}

	CHashString hashFile( fileName );

	ADDNOVODEXIMPORTER addCollectionParams;
	addCollectionParams.szFileName = hashFile;
	addCollectionParams.nxCollection = collection;

	static DWORD msgHash_AddNovodexImporter = CHashString(_T("AddNovodexImporter")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_AddNovodexImporter, sizeof(ADDNOVODEXIMPORTER), &addCollectionParams );

	return true;
}

DWORD CPhysicsLoader::OnSaveFile( DWORD size, void *params )
{
	// will we ever be editing and saving physics files?
	return MSG_HANDLED_STOP;
}