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

REGISTER_COMPONENT_SINGLETON( CMaterialTemplateLoader );
REGISTER_MESSAGE_HANDLER(LoadFile, OnLoadFile, CMaterialTemplateLoader);
REGISTER_MESSAGE_HANDLER(SaveFile, OnSaveFile, CMaterialTemplateLoader);

CHashString CMaterialTemplateLoader::m_HashName(_T("CMaterialTemplateLoader"));

LSRC_MESSAGE CMaterialTemplateLoader::m_LoadSaveRegisterData = {
		&CMaterialTemplateLoader::m_HashName,
		_T("Material Template Files (*.mtt)\0" "*.mtt\0"
		"\0\0"),
		_T("Material Template Files (*.mtt)\0" "*.mtt\0"
		"\0\0"),
		false, NULL,
};

CMaterialTemplateLoader::CMaterialTemplateLoader()
{
	m_ToolBox = EngineGetToolBox();
	static DWORD msgHash_RegisterLoadSaveComponent = CHashString(_T("RegisterLoadSaveComponent")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_RegisterLoadSaveComponent, sizeof(LSRC_MESSAGE),&m_LoadSaveRegisterData);
}

// create function for this Particle loader
IComponent *CMaterialTemplateLoader::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE( CMaterialTemplateLoader );
}

IHashString *CMaterialTemplateLoader::GetComponentType()
{
	return &m_HashName;
}

bool CMaterialTemplateLoader::IsKindOf(IHashString *compType)
{
	return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
}

DWORD CMaterialTemplateLoader::OnLoadFile(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(TCHAR *));
	TCHAR *pFileName = (TCHAR *)params;

	if( _tcslen( pFileName ) == 0 )
	{
		m_ToolBox->Log( LOGWARNING, _T("Material File is empty string") );
		return MSG_ERROR;
	}

	TCHAR drive[MAX_PATH];
	TCHAR dir[MAX_PATH];
	TCHAR file[MAX_PATH];
	TCHAR ext[MAX_PATH];
	StdString szFileName(pFileName);
	szFileName.MakeSafeFileName();

	IArchive *MemArchive;
	// hash the file name
	CHashString streamType(_T("File"));
	// create a new archive and load the file
	CREATEARCHIVE ca;
	DWORD retVal;
	ca.streamData = (TCHAR*)szFileName.c_str();
	ca.mode = STREAM_MODE_READ;
	ca.streamType = &streamType;
	// call the Archive factory to create an XML archive
	static DWORD msgHash_CreateXMLArchive = CHashString(_T("CreateXMLArchive")).GetUniqueID();
	if (retVal = m_ToolBox->SendMessage(msgHash_CreateXMLArchive, sizeof(CREATEARCHIVE), &ca) != MSG_HANDLED)
	{
		m_ToolBox->Log( LOGWARNING, _T("MATERIALTEMPLATELOADER: %s\n\tCould not create XML Archive\n"), pFileName );
		return MSG_ERROR;
	}

	MemArchive = dynamic_cast<IXMLArchive *>(ca.archive);

	///create the cal model
	CREATEOBJECTPARAMS cop;
	INITOBJECTPARAMS iop;
	SERIALIZEOBJECTPARAMS sop;
	
	CHashString hszParentName( _T("World") );
	CHashString hszTypeName(_T("CMaterialTemplate"));	

	if( MemArchive )
	{
		StdString curName;
		_splitpath( szFileName, drive, dir, file, ext );
		curName = file;
		curName += ext;
		CHashString matname( (const TCHAR*)curName );

		//check for existance first:
		MATERIALTEMPLATEINTERFACE cef;
		cef.m_Name = &matname;
		static DWORD msgHash_GetEffectInterface = CHashString(_T("GetMaterialTemplate")).GetUniqueID();
		EngineGetToolBox()->SendMessage(msgHash_GetEffectInterface, sizeof(MATERIALTEMPLATEINTERFACE ), &cef );
		//Material already exists
		if(  cef.m_MatInterface )
		{
			return MSG_HANDLED_STOP;
		}

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
	}
	return MSG_HANDLED_STOP;
}

DWORD CMaterialTemplateLoader::OnSaveFile(DWORD size, void *params)
{
	return MSG_HANDLED_STOP;
}