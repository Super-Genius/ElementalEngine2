#include "stdafx.h"
//#include <LibXML++/libxml++.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT_SINGLETON(CDX9ShaderLoader);
REGISTER_MESSAGE_HANDLER(LoadFile, OnLoadFile, CDX9ShaderLoader);
REGISTER_MESSAGE_HANDLER(SaveFile, OnSaveFile, CDX9ShaderLoader);

CHashString CDX9ShaderLoader::m_HashName(_T("CDX9ShaderLoader"));

LSRC_MESSAGE CDX9ShaderLoader::m_LoadSaveRegisterData = {
		&CDX9ShaderLoader::m_HashName,
		_T("HLSL VertexShader (*.vsh)\0" "*.vsh\0"
		"\0\0"),
		_T("HLSL PixelShader (*.psh)\0" "*.psh\0"
		"\0\0"),
};

CDX9ShaderLoader::CDX9ShaderLoader()
{
	m_ToolBox = EngineGetToolBox();
	m_ToolBox->SendMessage(_T("RegisterLoadSaveComponent"), sizeof(LSRC_MESSAGE),
		&m_LoadSaveRegisterData, NULL, NULL);

}

// create function for this Particle loader
IComponent *CDX9ShaderLoader::Create(va_list argptr)
{
	return SINGLETONINSTANCE( CDX9ShaderLoader );
}

IHashString *CDX9ShaderLoader::GetComponentType()
{
	return &m_HashName;
}

bool CDX9ShaderLoader::IsKindOf(IHashString *compType)
{
	return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
}

DWORD CDX9ShaderLoader::OnLoadFile(DWORD size, void *params)
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
	if (retVal = m_ToolBox->SendMessage(_T("CreateXMLArchive"), 
			sizeof(CREATEARCHIVE), &ca) != MSG_HANDLED)
	{
		return MSG_ERROR;
	}

	MemArchive = dynamic_cast<IXMLArchive *>(ca.archive);

	///create the cal model
	CREATEOBJECTPARAMS cop;
	INITOBJECTPARAMS iop;
	SERIALIZEOBJECTPARAMS sop;
	
	CHashString hszParentName( _T("World") );
	CHashString hszTypeName;
	

	TCHAR * lwr = _tcslwr( (TCHAR*)pFileName );
	//check filetype
	if( _tcsstr( lwr, _T(".psh") ) )
	{
		hszTypeName.Init( _T("CDX9PixelShader") );
	}else
	{
		hszTypeName.Init( _T("CDX9VertexShader") );
	}
	CHashString modelname( lwr );
	
	cop.parentName = &hszParentName;
	cop.typeName = &hszTypeName;
	cop.name = &modelname;
	DWORD retval = m_ToolBox->SendMessage(_T("CreateObject"), sizeof(CREATEOBJECTPARAMS), &cop, NULL, NULL);

	sop.name = &modelname;
	sop.archive = MemArchive;
	m_ToolBox->SendMessage(_T("SerializeObject"), sizeof(SERIALIZEOBJECTPARAMS), &sop, NULL, NULL);
	

	iop.name = &modelname;
	if( retval == MSG_HANDLED )
	{
		retval = m_ToolBox->SendMessage(_T("InitObject"), sizeof(INITOBJECTPARAMS), &iop, NULL, NULL);
	}
	return MSG_HANDLED_STOP;
}

DWORD CDX9ShaderLoader::OnSaveFile(DWORD size, void *params)
{
	return MSG_HANDLED_STOP;
}