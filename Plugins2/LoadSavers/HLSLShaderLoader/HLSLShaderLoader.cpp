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
		"HLSL VertexShader (*.vsh)\0" "*.vsh\0"
		"HLSL PixelShader (*.psh)\0" "*.psh\0"
		"\0\0",
		"HLSL VertexShader (*.vsh)\0" "*.vsh\0"
		"HLSL PixelShader (*.psh)\0" "*.psh\0"
		"\0\0",
		false, NULL,
};

CDX9ShaderLoader::CDX9ShaderLoader()
{
	m_ToolBox = EngineGetToolBox();
	static DWORD msgHash_RegisterLoadSaveComponent = CHashString(_T("RegisterLoadSaveComponent")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_RegisterLoadSaveComponent, sizeof(LSRC_MESSAGE),&m_LoadSaveRegisterData, NULL, NULL);
}

// create function for this Particle loader
IComponent *CDX9ShaderLoader::Create(int nArgs, va_list argptr)
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

	StdString szFileName(pFileName);
	szFileName.MakeSafeFileName();	

	CHashString hashFile(szFileName);
	map<DWORD, bool>::iterator mapIter = m_mFileLoaded.find(hashFile.GetUniqueID());

	// if the file is not loaded yet, load it; otherwise do nothing
	if (mapIter == m_mFileLoaded.end())
	{
		CHashString hszTypeName;
		//check filetype
		if( _tcsstr( szFileName, _T(".psh") ) )
		{
			hszTypeName.Init( _T("CPixelShader") );
		}
		else
		{
			hszTypeName.Init( _T("CVertexShader") );
		}		
		
		CREATEOBJECTPARAMS cop;
		cop.parentName = NULL;
		cop.typeName = &hszTypeName;
		cop.name = &hashFile;
		static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
		DWORD retval = m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop, NULL, NULL);

		INITOBJECTPARAMS iop;
		iop.name = &hashFile;
		if( retval == MSG_HANDLED )
		{
			static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
			retval = m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop, NULL, NULL);
		}

		ADDSHADERTOMANAGER astom;
		astom.m_Name = &hashFile;
		astom.m_CompType = &hszTypeName;
		if( retval == MSG_HANDLED )
		{
			static DWORD msgHash_AddShader = CHashString(_T("AddShader")).GetUniqueID();
			retval = m_ToolBox->SendMessage(msgHash_AddShader, sizeof(ADDSHADERTOMANAGER), &astom, NULL, NULL);
		}
		m_mFileLoaded.insert(pair<DWORD, bool>(hashFile.GetUniqueID(), true));
	}
	return MSG_HANDLED_STOP;
}

DWORD CDX9ShaderLoader::OnSaveFile(DWORD size, void *params)
{
	return MSG_HANDLED_STOP;
}