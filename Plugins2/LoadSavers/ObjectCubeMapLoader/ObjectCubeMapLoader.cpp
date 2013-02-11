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

REGISTER_COMPONENT_SINGLETON(ObjectCubeMapLoader);
REGISTER_MESSAGE_HANDLER(LoadFile, OnLoadFile, ObjectCubeMapLoader);
REGISTER_MESSAGE_HANDLER(SaveFile, OnSaveFile, ObjectCubeMapLoader);

CHashString ObjectCubeMapLoader::m_HashName(_T("ObjectCubeMapLoader"));

LSRC_MESSAGE ObjectCubeMapLoader::m_LoadSaveRegisterData = {
		&ObjectCubeMapLoader::m_HashName,
		"Object Cube Map (*.ocm)\0" "*.ocm\0"
		"\0\0",
		"Object Cube Map (*.ocm)\0" "*.ocm\0"
		"\0\0",
		false, NULL,
};

ObjectCubeMapLoader::ObjectCubeMapLoader()
{
	m_ToolBox = EngineGetToolBox();
	static DWORD msgHash_RegisterLoadSaveComponent = CHashString(_T("RegisterLoadSaveComponent")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_RegisterLoadSaveComponent, sizeof(LSRC_MESSAGE), &m_LoadSaveRegisterData);

}

// create function for this Particle loader
IComponent *ObjectCubeMapLoader::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE( ObjectCubeMapLoader );
}

IHashString *ObjectCubeMapLoader::GetComponentType()
{
	return &m_HashName;
}

bool ObjectCubeMapLoader::IsKindOf(IHashString *compType)
{
	return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
}

DWORD ObjectCubeMapLoader::OnLoadFile(DWORD size, void *params)
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
			return MSG_ERROR;
		}

		MemArchive = dynamic_cast<IXMLArchive *>(ca.archive);

		///create the cal model
		CREATEOBJECTPARAMS cop;
		INITOBJECTPARAMS iop;
		SERIALIZEOBJECTPARAMS sop;
		
		CHashString hszParentName( _T("World") );
		CHashString hszTypeName;

		//check filetype
		hszTypeName.Init( _T("ObjectCubeMap") );		
		
		cop.parentName = &hszParentName;
		cop.typeName = &hszTypeName;
		cop.name = &hashFile;
		static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
		DWORD retval = m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop);

		sop.name = &hashFile;
		sop.archive = MemArchive;
		static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop);
		

		iop.name = &hashFile;
		if( retval == MSG_HANDLED )
		{
			static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
			retval = m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop);
		}

		MemArchive->Close();
		m_mFileLoaded.insert(pair<DWORD, bool>(hashFile.GetUniqueID(), true));
	}
	return MSG_HANDLED_STOP;
}

DWORD ObjectCubeMapLoader::OnSaveFile(DWORD size, void *params)
{
	return MSG_HANDLED_STOP;
}