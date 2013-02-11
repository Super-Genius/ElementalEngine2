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

REGISTER_COMPONENT_SINGLETON(CShaderEffectLoader);
REGISTER_MESSAGE_HANDLER(LoadFile, OnLoadFile, CShaderEffectLoader);
REGISTER_MESSAGE_HANDLER(SaveFile, OnSaveFile, CShaderEffectLoader);

CHashString CShaderEffectLoader::m_HashName(_T("CShaderEffectLoader"));

LSRC_MESSAGE CShaderEffectLoader::m_LoadSaveRegisterData = {
		&CShaderEffectLoader::m_HashName,
		_T("HierarhicalModel Files (*.efx)\0" "*.efx\0"
		"\0\0"),
		_T("HierarhicalModel Files (*.efx)\0" "*.efx\0"
		"\0\0"),
		false, NULL,
};

CShaderEffectLoader::CShaderEffectLoader()
{
	m_ToolBox = EngineGetToolBox();
	static DWORD msgHash_RegisterLoadSaveComponent = CHashString(_T("RegisterLoadSaveComponent")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_RegisterLoadSaveComponent, sizeof(LSRC_MESSAGE),&m_LoadSaveRegisterData);
}

// create function for this Particle loader
IComponent *CShaderEffectLoader::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE( CShaderEffectLoader );
}

IHashString *CShaderEffectLoader::GetComponentType()
{
	return &m_HashName;
}

bool CShaderEffectLoader::IsKindOf(IHashString *compType)
{
	return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
}

DWORD CShaderEffectLoader::OnLoadFile(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(TCHAR *));
	TCHAR *pFileName = (TCHAR *)params;

	if( _tcslen( pFileName ) == 0 )
	{
		m_ToolBox->Log( LOGWARNING, _T("Shader File is empty string") );
		return MSG_ERROR;
	}

	StdString szFileName(pFileName);
	szFileName.MakeSafeFileName();
	CHashString effectname( szFileName );

	// only load an effect file once
	static DWORD msgHash_FindObject = CHashString(_T("FindObject")).GetUniqueID();
	FINDOBJECTPARAMS param;
	param.hszName = &effectname;
	m_ToolBox->SendMessage(msgHash_FindObject, sizeof(FINDOBJECTPARAMS), &param );
	if ( param.bFound == true )
	{
		// effect already loaded
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
		m_ToolBox->Log( LOGWARNING, _T("SHADEREFFECTLOADER: %s\n\tCould not create XML Archive\n"), pFileName );
		return MSG_ERROR;
	}

	MemArchive = dynamic_cast<IXMLArchive *>(ca.archive);

	///create the cal model
	CREATEOBJECTPARAMS cop;
	INITOBJECTPARAMS iop;
	SERIALIZEOBJECTPARAMS sop;
	
	CHashString hszParentName( _T("World") );
	CHashString hszTypeName(_T("CEffect"));	

	cop.parentName = &hszParentName;
	cop.typeName = &hszTypeName;
	cop.name = &effectname;
	static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
	DWORD retval = m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop, NULL, NULL);

	sop.name = &effectname;
	sop.archive = MemArchive;
	static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop, NULL, NULL);
	

	iop.name = &effectname;
	if( retval == MSG_HANDLED )
	{
		static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
		retval = m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop, NULL, NULL);
	}

	MemArchive->Close();

	CREATEEFFECTINTERFACE addmsg;
	addmsg.m_Name = &effectname;
	static DWORD msgHash_AddEffect = CHashString(_T("AddEffect")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_AddEffect, sizeof( CREATEEFFECTINTERFACE ), &addmsg );
	return MSG_HANDLED_STOP;
}

DWORD CShaderEffectLoader::OnSaveFile(DWORD size, void *params)
{
	return MSG_HANDLED_STOP;
}