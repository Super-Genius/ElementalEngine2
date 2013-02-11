///============================================================================
/// \file		ProjectLoadSave.cpp
/// \brief		Implementation file for Sound Loading
/// \date		12-16-2008
/// \author		Zsolt Matyas
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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT_SINGLETON(CProjectLoadSave);
REGISTER_MESSAGE_HANDLER(LoadFile, OnLoadFile, CProjectLoadSave);
REGISTER_MESSAGE_HANDLER(SaveFile, OnSaveFile, CProjectLoadSave);

CHashString CProjectLoadSave::m_HashName(_T("CProjectLoadSave"));

LSRC_MESSAGE CProjectLoadSave::m_LoadSaveRegisterData = {
		&CProjectLoadSave::m_HashName,
		_T("GDS Project Files (*.gdsprj)\0" "*.gdsprj\0")		
		_T("\0\0"),
		_T("GDS Project Files (*.gdsprj)\0" "*.gdsprj\0")
		_T("\0\0"),
		false, NULL,
};

CProjectLoadSave::CProjectLoadSave() 
{
	// save for convenience
	m_ToolBox = EngineGetToolBox();
	DWORD check;

	static DWORD msgHash_RegisterLoadSaveComponent = CHashString(_T("RegisterLoadSaveComponent")).GetUniqueID();
	check = m_ToolBox->SendMessage(msgHash_RegisterLoadSaveComponent, sizeof(LSRC_MESSAGE),&m_LoadSaveRegisterData);

	if (!check)
	{
		// log error RegisterLoadSaveComponent failed!
	}
}

// create function for this world loader
IComponent *CProjectLoadSave::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE( CProjectLoadSave );
}

IHashString *CProjectLoadSave::GetComponentType()
{
	return &m_HashName;
}

bool CProjectLoadSave::IsKindOf(IHashString *compType)
{
	return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
}

DWORD CProjectLoadSave::OnLoadFile( DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(TCHAR *));
	TCHAR *pFileName = (TCHAR *)params;

	DWORD retVal;
	
	static CHashString streamType(_T("File"));
	CREATEARCHIVE ca;
	ca.streamData = (void *)pFileName;
	ca.mode = STREAM_MODE_READ;
	ca.streamType = &streamType;
	// call the Archive factory to create an XML archive
	static DWORD msgHash_CreateXMLArchive = CHashString(_T("CreateXMLArchive")).GetUniqueID();
	if (retVal = m_ToolBox->SendMessage(msgHash_CreateXMLArchive, sizeof(CREATEARCHIVE), &ca) != MSG_HANDLED)
	{
		return MSG_ERROR;
	}
	IXMLArchive *FileArchive = dynamic_cast<IXMLArchive *>(ca.archive);

	// get the first node
	StdString szNodeName;
	FileArchive->GetNode(szNodeName);

	// must start with <GDSProject>
	if (szNodeName != _T("GDSProject"))
	{
		m_ToolBox->SetErrorValue(WARN_INVALID_FILE);
		m_ToolBox->Log(LOGWARNING, _T("Invalid precache file %s\n"), pFileName);
		FileArchive->Close();
		return MSG_ERROR;
	}

	static CHashString hszType( _T("CProjectSettings") );
	CHashString hszName( pFileName );
	
	IComponent *pComponent = dynamic_cast<IComponent *>(
		m_ToolBox->CreateComponent(&hszType, 2, &hszName, NULL));
	if (pComponent == NULL)
	{
		m_ToolBox->Log( LOGFATALERROR, _T("Cannot create project setting for: %s\n"), pFileName );
		return MSG_ERROR;
	}
	
	// call the serialize function of the default project settings.
	pComponent->Serialize(*FileArchive);

	// serialize everything else
	if( FileArchive->IsReading() )
	{
		// stores the name of the plugin - is read from the archive
  		CHashString hszPluginName(_T(""));

		// we read the nodes - false is returned when there are no more 
		while( FileArchive->GetNode(szNodeName) ) 
		{
			// check if we are still on the same level
			if( FileArchive->GetDepth() > 1 )
			{
				// if not the same level, we skip the iteration
				m_ToolBox->Log( LOGWARNING, _T("Processing the project settings file (%s) will skip some tag.\nPerhaps a plugin could did not handle its part of the file correctly.\n"), pFileName );
				continue;
			}
			
			// set the plugin name
			hszPluginName = szNodeName.c_str();

			// try to create (get) the instance of the plugin
			pComponent = dynamic_cast<IComponent *>(
			m_ToolBox->CreateComponent(&hszPluginName, 0));
			if (pComponent == NULL)
			{
				m_ToolBox->Log( LOGERROR, _T("Cannot create project setting for: %s\n"), pFileName );
				return MSG_ERROR;
			} 
	
			// call the serialize function of the instance
			pComponent->Serialize(*FileArchive);
		}

	}
	else
	{
		// we should never get here... archive is not under writing in the 'onloadfile' message handler.
		m_ToolBox->Log( LOGERROR, _T("%s(%d):Wrong archive mode while reading the project settigns file (%s).\n"), __FILE__, __LINE__, pFileName );
		return MSG_ERROR;
	}

	FileArchive->Close();

	return MSG_HANDLED_PROCEED;
}

DWORD CProjectLoadSave::OnSaveFile( DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(TCHAR *));
	TCHAR *pFileName = (TCHAR *)params;
	
	DWORD retVal;

	static CHashString streamType(_T("File"));
	CREATEARCHIVE ca;
	ca.streamData = (void *)pFileName;
	ca.mode = STREAM_MODE_WRITE;
	ca.streamType = &streamType;
	// call the Archive factory to create an XML archive
	static DWORD msgHash_CreateXMLArchive = CHashString(_T("CreateXMLArchive")).GetUniqueID();
	if (retVal = m_ToolBox->SendMessage(msgHash_CreateXMLArchive, sizeof(CREATEARCHIVE), &ca) != MSG_HANDLED)
	{
		return MSG_ERROR;
	}
	IXMLArchive *FileArchive = dynamic_cast<IXMLArchive *>(ca.archive);
	
	FileArchive->StartClass( _T("GDSProject") );
		FileArchive->StartClass( _T("CProjectSettings") );

			static CHashString hszType( _T("CProjectSettings") );
			CHashString hszName( pFileName );
			IComponent *pComponent = dynamic_cast<IComponent *>(
			m_ToolBox->CreateComponent(&hszType, 2, &hszName, NULL));
			if (pComponent == NULL)
			{
				m_ToolBox->Log( LOGFATALERROR, _T("Cannot create project setting for: %s\n"), pFileName );
				return MSG_ERROR;
			}

			pComponent->Serialize(*FileArchive);

		FileArchive->EndClass(); // CProjectSettings

		
	// write out every other settings

	// get the project editor component
	static CHashString hszProjectEditorComponent( _T("CProjectEditorComponent") );
	IComponent *pProjectEditorComponent = dynamic_cast<IComponent *>(m_ToolBox->CreateComponent(&hszProjectEditorComponent, 0));
	
	// Error handling...
	if (pProjectEditorComponent == NULL)
	{
		// log our problem
		m_ToolBox->Log( LOGFATALERROR, _T("Cannot project editor component to serialize the registered pages.%s\n"), pFileName );

		// Close and return 
		FileArchive->EndClass(); // GDSProject
		FileArchive->Close();
		return MSG_ERROR;
	}
	else 
	{
		// call the manager component's serilaize function.
		pProjectEditorComponent->Serialize( *FileArchive );
	}

	FileArchive->EndClass(); // GDSProject
	FileArchive->Close();
	return MSG_HANDLED_PROCEED;
}

