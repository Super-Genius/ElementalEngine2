///============================================================================
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
//#include <LibXML++/libxml++.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT_SINGLETON(CMaterialLoader);
REGISTER_MESSAGE_HANDLER(LoadFile, OnLoadFile, CMaterialLoader);
REGISTER_MESSAGE_HANDLER(SaveFile, OnSaveFile, CMaterialLoader);
//REGISTER_MESSAGE_HANDLER(UnloadFile, OnUnloadFile, CMaterialLoader);

CHashString CMaterialLoader::m_HashName(_T("CMaterialLoader"));

LSRC_MESSAGE CMaterialLoader::m_LoadSaveRegisterData = {
		&CMaterialLoader::m_HashName,
		_T("Material Files (*.xrf)\0" "*.xrf\0"
		"\0\0"),
		_T("Cal3D Material Files (*.xrf)\0" "*.xrf\0"
		"\0\0"),
		false, NULL,
};

CMaterialLoader::CMaterialLoader()
{
	m_ToolBox = EngineGetToolBox();
	static DWORD msgHash_RegisterLoadSaveComponent = CHashString(_T("RegisterLoadSaveComponent")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_RegisterLoadSaveComponent, sizeof(LSRC_MESSAGE),&m_LoadSaveRegisterData);
}

// create function for this Particle loader
IComponent *CMaterialLoader::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE( CMaterialLoader );
}

IHashString *CMaterialLoader::GetComponentType()
{
	return &m_HashName;
}

bool CMaterialLoader::IsKindOf(IHashString *compType)
{
	return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
}

DWORD CMaterialLoader::OnLoadFile(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(TCHAR *));
	TCHAR *pFileName = (TCHAR *)params;
	CHashString hszFileName( pFileName );

	if( _tcslen( pFileName ) == 0 )
	{
		m_ToolBox->Log( LOGWARNING, _T("Material File is empty string") );
		return MSG_ERROR;
	}

	StdString szFileName(pFileName);
	szFileName.MakeSafeFileName();
	CHashString matname( szFileName );

	// only load an material file once
	static DWORD msgHash_FindObject = CHashString(_T("FindObject")).GetUniqueID();
	FINDOBJECTPARAMS param;
	param.hszName = &matname;
	m_ToolBox->SendMessage(msgHash_FindObject, sizeof(FINDOBJECTPARAMS), &param );
	if( param.bFound == true )
	{
		// material already loaded
		return MSG_HANDLED_STOP;
	}

	IArchive *MemArchive;
	// hash the file name
	CHashString streamType(_T("File"));
	// create a new archive and load the file
	CREATEARCHIVE ca;
	DWORD retVal;
	ca.streamData = pFileName;
	ca.mode = STREAM_MODE_READ;
	ca.streamType = &streamType;
	// call the Archive factory to create an XML archive
	static DWORD msgHash_CreateXMLArchive = CHashString(_T("CreateXMLArchive")).GetUniqueID();
	if (retVal = m_ToolBox->SendMessage(msgHash_CreateXMLArchive, sizeof(CREATEARCHIVE), &ca) != MSG_HANDLED)
	{
		m_ToolBox->Log( LOGWARNING, _T("MATERIALLOADER: %s\n\tCould not create XML Archive\n"), pFileName );
		return MSG_ERROR;
	}

	MemArchive = dynamic_cast<IXMLArchive *>(ca.archive);

	///create the cal model
	CREATEOBJECTPARAMS cop;
	INITOBJECTPARAMS iop;
	SERIALIZEOBJECTPARAMS sop;
	
	CHashString hszParentName( _T("World") );
	CHashString hszTypeName(_T("CMaterial"));	

	cop.parentName = &hszParentName;
	cop.typeName = &hszTypeName;
	cop.name = &matname;
	static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
	DWORD retval = m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop, NULL, NULL);

	sop.name = &matname;
	sop.archive = MemArchive;
	static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop, NULL, NULL);

	iop.name = &matname;
	if( retval == MSG_HANDLED )
	{
		static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
		retval = m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop, NULL, NULL);
	}

	MemArchive->Close();

	m_UnloadMap[hszFileName.GetUniqueID()] = matname.GetUniqueID();

	return MSG_HANDLED_STOP;
}

DWORD CMaterialLoader::OnSaveFile(DWORD size, void *params)
{
	return MSG_HANDLED_STOP;
}

DWORD CMaterialLoader::OnUnloadFile(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(TCHAR *));
	CHashString fileName((const TCHAR *)params);
	DWORD result;

	UNLOADOBJECTMAP::iterator itr = m_UnloadMap.find( fileName.GetUniqueID() );

	if( itr != m_UnloadMap.end() )
	{
		CHashString tmpObjectName( m_ToolBox->GetHashString( itr->second ) );
		DELETEOBJECTPARAMS dop;
		dop.name = &tmpObjectName;
		static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
		result = m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof(dop), &dop );
		m_UnloadMap.erase( itr );
		return result;
	}
	else
	{
		return MSG_ERROR;
	}
}
