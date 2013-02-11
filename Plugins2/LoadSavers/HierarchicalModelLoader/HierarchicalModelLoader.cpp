#include "stdafx.h"
//#include <LibXML++/libxml++.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT_SINGLETON(CHierarchicalModelLoader);
REGISTER_MESSAGE_HANDLER(LoadFile, OnLoadFile, CHierarchicalModelLoader);
REGISTER_MESSAGE_HANDLER(SaveFile, OnSaveFile, CHierarchicalModelLoader);

CHashString CHierarchicalModelLoader::m_HashName(_T("CHierarchicalModelLoader"));

LSRC_MESSAGE CHierarchicalModelLoader::m_LoadSaveRegisterData = {
		&CHierarchicalModelLoader::m_HashName,
		_T("HierarhicalModel Files (*.hrc)\0" "*.hrc\0"
		"\0\0"),
		_T("HierarhicalModel Files (*.hrc)\0" "*.hrc\0"
		"\0\0"),
		true, NULL,
};

CHierarchicalModelLoader::CHierarchicalModelLoader()
{
	m_ToolBox = EngineGetToolBox();
	static DWORD msgHash_RegisterLoadSaveComponent = CHashString(_T("RegisterLoadSaveComponent")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_RegisterLoadSaveComponent, sizeof(LSRC_MESSAGE), &m_LoadSaveRegisterData);
}

// create function for this Particle loader
IComponent *CHierarchicalModelLoader::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE( CHierarchicalModelLoader );
}

IHashString *CHierarchicalModelLoader::GetComponentType()
{
	return &m_HashName;
}

bool CHierarchicalModelLoader::IsKindOf(IHashString *compType)
{
	return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
}

DWORD CHierarchicalModelLoader::OnLoadFile(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(TCHAR *));
	TCHAR *pFileName = (TCHAR *)params;

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
		return MSG_ERROR;
	}

	MemArchive = dynamic_cast<IXMLArchive *>(ca.archive);

	///create the cal model
	CREATEOBJECTPARAMS cop;
	INITOBJECTPARAMS iop;
	SERIALIZEOBJECTPARAMS sop;
	
	CHashString hszParentName( _T("World") );
	CHashString hszTypeName(_T("HierarchicalModelType"));	

	StdString szFileName(pFileName);
	szFileName.MakeSafeFileName();
	CHashString modelname( szFileName );

	// only load an effect file once
	static DWORD msgHash_FindObject = CHashString(_T("FindObject")).GetUniqueID();
	FINDOBJECTPARAMS param;
	param.hszName = &modelname;
	m_ToolBox->SendMessage(msgHash_FindObject, sizeof(FINDOBJECTPARAMS), &param);
	if( param.bFound == true )
	{
		MemArchive->Close();
		// effect already loaded
		return MSG_HANDLED_STOP;
	}
	
	cop.parentName = &hszParentName;
	cop.typeName = &hszTypeName;
	cop.name = &modelname;
	static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
	DWORD retval = m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop, NULL, NULL);

	sop.name = &modelname;
	sop.archive = MemArchive;
	static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop, NULL, NULL);
	

	iop.name = &modelname;
	if( retval == MSG_HANDLED )
	{
		static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
		retval = m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop, NULL, NULL);
	}
	MemArchive->Close();
	return MSG_HANDLED_STOP;
}

DWORD CHierarchicalModelLoader::OnSaveFile(DWORD size, void *params)
{
	return MSG_HANDLED_STOP;
}