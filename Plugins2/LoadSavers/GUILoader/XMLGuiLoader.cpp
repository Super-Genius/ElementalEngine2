///============================================================================
/// \file		XMLGuiLoader.cpp
/// \brief		Implementation file for XML based Gui Loader
/// \date		04-04-2005
/// \author		Halbert Nakagawa
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

REGISTER_COMPONENT_SINGLETON(CXMLGuiLoader);
REGISTER_MESSAGE_HANDLER(LoadFile, OnLoadGuiFile, CXMLGuiLoader);
REGISTER_MESSAGE_HANDLER(SaveFile, OnSaveGuiFile, CXMLGuiLoader);
REGISTER_MESSAGE_HANDLER(GetGUIVersion, OnGetGUIVersion, CXMLGuiLoader);
REGISTER_MESSAGE_HANDLER(SetGUIVersion, OnSetGUIVersion, CXMLGuiLoader);
REGISTER_MESSAGE_HANDLER(GetFileResolution, OnGetFileResolution, CXMLGuiLoader);
REGISTER_MESSAGE_HANDLER(GetPageList, OnGetPageList, CXMLGuiLoader);

CHashString CXMLGuiLoader::m_HashName(_T("CXMLGuiLoader"));

LSRC_MESSAGE CXMLGuiLoader::m_LoadSaveRegisterData = {
		&CXMLGuiLoader::m_HashName,
		_T("XML Gui Files (*.gxl)\0" "*.gxl\0"
		"\0\0"),
		_T("XML Gui Files (*.gxl)\0" "*.gxl\0"
		"\0\0"),
		false, NULL,
};

CXMLGuiLoader::CXMLGuiLoader() 
{
	// save for convenience
	m_ToolBox = EngineGetToolBox();
	static DWORD msgHash_RegisterLoadSaveComponent = CHashString(_T("RegisterLoadSaveComponent")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_RegisterLoadSaveComponent, sizeof(LSRC_MESSAGE),&m_LoadSaveRegisterData);

	// setup function ptrs
	StdString index = _T("1.1");
	m_VersToLoadGuiFuncMap[index] = &CXMLGuiLoader::LoadGuiFileV11a;
	index = _T("1.1b");
	m_VersToLoadGuiFuncMap[index] = &CXMLGuiLoader::LoadGuiFileV11b;
	index = _T("1.1c");
	m_VersToLoadGuiFuncMap[index] = &CXMLGuiLoader::LoadGuiFile;

	// august 2006 serialization
	index = _T("1.1d");
	m_VersToLoadGuiFuncMap[index] = &CXMLGuiLoader::LoadGuiFile;

	// october 2006 serialization
	index = _T("2.0a");
	m_VersToLoadGuiFuncMap[index] = &CXMLGuiLoader::LoadGuiFile;

	// november 2007 serialization
	index = _T("2.0b");
	m_VersToLoadGuiFuncMap[index] = &CXMLGuiLoader::LoadGuiFile;
	
	// february 2008 serialization
	index = _T("2.0c");
	m_VersToLoadGuiFuncMap[index] = &CXMLGuiLoader::LoadGuiFile;

	m_szVersionName = "";
	m_iXRes = 0;
	m_iYRes = 0;

	m_CurrentFile = _T("");
}

// create function for this Gui loader
IComponent *CXMLGuiLoader::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE( CXMLGuiLoader );
}

IHashString *CXMLGuiLoader::GetComponentType()
{
	return &m_HashName;
}

bool CXMLGuiLoader::IsKindOf(IHashString *compType)
{
	return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
}

// load a file if we recognize the type.
DWORD CXMLGuiLoader::OnLoadGuiFile(DWORD size, void *params)
{
	bool checkGUISys = true;

	CHashString oldVersion = _T("");

	static DWORD msgHash_SetFileVersion = CHashString(_T("SetFileVersion")).GetUniqueID();

	static DWORD msgHash_CheckGUISystemEmpty = CHashString(_T("CheckGUISystemEmpty")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_CheckGUISystemEmpty, sizeof(bool), &checkGUISys);

	if (!checkGUISys)
	{
		StdString error;
		error = _T("Cannot load a gui file while gui manager has objects.\n");

		// log error
		EngineGetToolBox()->SetErrorValue(WARN_INVALID_OPERATION);
		EngineGetToolBox()->Log(LOGWARNING, error);
#ifdef WIN32
		MessageBox(NULL, error, _T("Warning"), MB_OK);
#endif
		return MSG_WARNING;
	}

	IXMLArchive *Archive;
	TCHAR *pFileName;
	CHashString streamType(_T("File"));
	CREATEARCHIVE ca;
	StdString szNodeName;
	StdString versNum;
	DWORD retVal;
	
	VERIFY_MESSAGE_SIZE(size, sizeof(TCHAR *));
	pFileName = (TCHAR *)params;
    
	StdString szFileName(pFileName);
	szFileName.MakeSafeFileName();
	m_CurrentFile.Init(szFileName);

	ca.streamData = (void *)pFileName;
	ca.mode = STREAM_MODE_READ;
	ca.streamType = &streamType;
	// call the Archive factory to create an XML archive
	static DWORD msgHash_CreateXMLArchive = CHashString(_T("CreateXMLArchive")).GetUniqueID();
	if ((retVal = m_ToolBox->SendMessage(msgHash_CreateXMLArchive, sizeof(CREATEARCHIVE), &ca)) != MSG_HANDLED)
	{
		return retVal;
	}

	Archive = dynamic_cast <IXMLArchive *>(ca.archive);

	Archive->GetNode(szNodeName);
	
	// check if using GUI Editor v .95
	if ( _tcscmp(szNodeName, _T("GUIOBJECTS")) == 0 )
	{
		LoadGuiFileV95(Archive);
		Archive->Close();
		return MSG_HANDLED_STOP;

	}
	else if ( _tcscmp(szNodeName, _T("CGUI")) == 0 )
	{
		LoadGuiFileV1(Archive);
		Archive->Close();
		return MSG_HANDLED_STOP;
	}
	else if ( _tcscmp(szNodeName, _T("CGUI_")) != 0 )
	{
		// incorrect file
		Archive->Close();
		return MSG_ERROR;
	}
	else // szNodeName == _T("CGUI_")
	{
		Archive->Read(versNum);
		m_szVersionName = versNum;		
        
		static DWORD msgHash_GetFileVersion = CHashString(_T("GetFileVersion")).GetUniqueID();
		if (m_ToolBox->SendMessage(msgHash_GetFileVersion, sizeof(IHashString), &oldVersion) != MSG_HANDLED)
		{
			m_ToolBox->Log(LOGERROR, _T("Could not get file version!"));
			return MSG_ERROR;
		}
		
		if (m_ToolBox->SendMessage(msgHash_SetFileVersion, sizeof(IHashString), &m_szVersionName) != MSG_HANDLED)
		{
			m_ToolBox->Log(LOGERROR, _T("Could not set file version!"));
			return MSG_ERROR;
		}

		if ((_tcscmp(versNum, _T("1.1d"))== 0) || 
			(_tcscmp(versNum, _T("2.0a"))== 0) || 
			(_tcscmp(versNum, _T("2.0b"))== 0) ||
			(_tcscmp(versNum, _T("2.0c"))== 0))
		{
			Archive->Read(m_iXRes);
			Archive->Read(m_iYRes);			

			GUIWORKINGRES gwr;
			gwr.iSizeX = m_iXRes;
			gwr.iSizeY = m_iYRes;
			static DWORD msgHash_SetGUIWorkingRes = CHashString(_T("SetGUIWorkingRes")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_SetGUIWorkingRes, sizeof(GUIWORKINGRES), &gwr);

			if (_tcscmp(versNum, _T("2.0c"))== 0)
			{
				StdString szFontGlyphDBFile;
				Archive->Read(szFontGlyphDBFile);
				CHashString hsFontGlyphDB(szFontGlyphDBFile);

				static DWORD msgSetGUIFontGlyphDB = CHashString(_T("SetGUIFontGlyphDB")).GetUniqueID();
				m_ToolBox->SendMessage(msgSetGUIFontGlyphDB, sizeof(IHashString*), &hsFontGlyphDB);
			}
		}
		else
		{
			m_iXRes = 1024; // default to 1024 for older gui stuffs
			m_iYRes = 768;  // default to 768 for older gui stuffs
		}

		VERSTOLOADGUIFUNCMAP::iterator iter = m_VersToLoadGuiFuncMap.find(versNum);
		if(iter == m_VersToLoadGuiFuncMap.end())
		{
			// version not recognized
			//m_ToolBox->Log(
			Archive->Close();
			m_ToolBox->SendMessage(msgHash_SetFileVersion, sizeof(IHashString), &oldVersion);
			return MSG_ERROR;
		}
		LOADGUIFUNC funcPtr = iter->second;

		(this->*funcPtr)(Archive, 0);
		Archive->Close();
		m_ToolBox->SendMessage(msgHash_SetFileVersion, sizeof(IHashString), &oldVersion);
		return MSG_HANDLED_STOP;
	}
}

void CXMLGuiLoader::LoadGuiFile(IXMLArchive *Archive, UINT mapIndex)
{
	CREATEOBJECTPARAMS cop;
	INITOBJECTPARAMS iop;
	SERIALIZEOBJECTPARAMS sop;
	FINDGUIOBJECT fgo;

	StdString szNodeName;
	StdString szName;
	CHashString hszName(_T(""));
	CHashString hszTypeName(_T(""));

	GUIPAGELIST gpl;

	gpl.m_iMapIndex = mapIndex;

	static DWORD dwCGUIPage = CHashString(_T("CGUIPage")).GetUniqueID();
	static DWORD dwCGUICursor = CHashString(_T("CGUICursor")).GetUniqueID();
	static DWORD dwCGUIAnimation = CHashString(_T("CGUIAnimation")).GetUniqueID();
	static DWORD dwLanguage = CHashString(_T("Language")).GetUniqueID();

	while (Archive->GetNode(szNodeName))
	{
		hszTypeName = szNodeName.c_str();

		if (hszTypeName.GetUniqueID() == dwLanguage)
		{
			// parse language here
			float langVersion;
			StdString langName;
			StdString langFile;
			
			Archive->Read(langVersion);
			Archive->Read(langName);
			Archive->Read(langFile);

			CHashString hszLangName(langName);
			CHashString hszLangFile(langFile);
			ADDGUILANGUAGEPARAMS adlp;
			adlp.dwHashLangName = hszLangName.GetUniqueID();
			adlp.dwHashLangFile = hszLangFile.GetUniqueID();

			static DWORD msgAddGUILanguage = CHashString(_T("AddGUILanguage")).GetUniqueID();
			m_ToolBox->SendMessage(msgAddGUILanguage, sizeof(ADDGUILANGUAGEPARAMS), &adlp);

			// don't go on to try to create a component
			continue;
		}

		Archive->Read( szName, _T("Name") );
		hszName = szName.c_str();

		if (hszTypeName.GetUniqueID() == dwCGUIPage)
		{
			gpl.m_vPages.push_back(hszName.GetString());
		}
		else if (hszTypeName.GetUniqueID() == dwCGUICursor)
		{
			gpl.m_szCursorName = szName;
		}		
		else if (hszTypeName.GetUniqueID() == dwCGUIAnimation)
		{
			gpl.m_vAnimations.push_back(szName);
		}

		fgo.name = &hszName;
		fgo.bFound = false;
		
		// first check if object already exists in EE

		static DWORD msgHash_FindGUIObject = CHashString(_T("FindGUIObject")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_FindGUIObject, sizeof(FINDGUIOBJECT), &fgo);
		if( !fgo.bFound )
		{

			cop.parentName = NULL;
			cop.typeName = &hszTypeName;
			cop.name = &hszName;
			static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop);

			iop.name = &hszName;
			static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop);
		}
		sop.name = &hszName;
		sop.archive = Archive;
		static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop);
	}

	m_mGplMap.insert(pair<DWORD, GUIPAGELIST>(m_CurrentFile.GetUniqueID(), gpl));
}


// Save a file if we recognize the type.
DWORD CXMLGuiLoader::OnSaveGuiFile(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(TCHAR *));
	TCHAR *pFileName = (TCHAR *)params;

	CHashString guiManager(_T("CGUIManager"));
	IComponent *manager;

	CREATEARCHIVE ca;
	ca.streamData = pFileName;
	ca.mode = STREAM_MODE_WRITE;
	static DWORD msgHash_CreateXMLArchive = CHashString(_T("CreateXMLArchive")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_CreateXMLArchive, sizeof(CREATEARCHIVE), &ca) == MSG_HANDLED)
	{
		manager = m_ToolBox->CreateComponent(&guiManager, 0);
		manager->Serialize(*(ca.archive));
		ca.archive->Close();
	}

	return MSG_HANDLED_STOP;
}

DWORD CXMLGuiLoader::OnGetGUIVersion(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(IHashString));
	IHashString *version = (IHashString*) params;
	if (_tcscmp(m_szVersionName.GetString(), "") == 0)
	{
		m_szVersionName = "2.0c"; // february 2008 serialization
	}
	*version = m_szVersionName.GetString();
	return MSG_HANDLED_STOP;
}

DWORD CXMLGuiLoader::OnSetGUIVersion(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(CHashString));
	CHashString version = *(CHashString*) params;
	m_szVersionName = version;
	return MSG_HANDLED_STOP;
}

void CXMLGuiLoader::CreatePage(StdString strPageName, StdString strPageType)
{
	CREATEOBJECTPARAMS cop;
  	INITOBJECTPARAMS iop;
  
  	CHashString hszPageName(strPageName.c_str());
  	CHashString hszPageType(strPageType.c_str());
  	
  	cop.name = &hszPageName;
  	cop.typeName = &hszPageType;
  	cop.parentName = NULL;
  	iop.name = &hszPageName;
  
  	// send message to ee to create a new page
static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
  	m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop);
static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
  	m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop);
}

void CXMLGuiLoader::CreateGuiElement(StdString strGuiName, StdString strGuiType, IXMLArchive &Archive)
{
  	CHashString hszGuiName(strGuiName.c_str());
  	CHashString hszGuiType(strGuiType.c_str());

	CREATEOBJECTPARAMS cop;
  	INITOBJECTPARAMS iop;
	SERIALIZEOBJECTPARAMS sop;
	FINDOBJNAME fon;

  	cop.name = &hszGuiName;
  	cop.typeName = &hszGuiType;
  	cop.parentName = NULL;
  	iop.name = &hszGuiName;
  	sop.name = &hszGuiName;
  	sop.archive = &Archive;
  	
  	fon.searchName = &hszGuiName;
  	fon.compType = &hszGuiType;
  	fon.bFound = false;
  
  	// first check if object already exists in object factor
static DWORD msgHash_FindGUIObject = CHashString(_T("FindGUIObject")).GetUniqueID();
  	m_ToolBox->SendMessage(msgHash_FindGUIObject, sizeof(FINDOBJNAME), &fon);
  	if( !fon.bFound )
  	{
  		// not found...
  		// send message to ee to create new gui element
static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
  		m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop);
static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
  		m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop);
  	}
  	else 
  	{
  		printf("haf");
  	}
static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
  	m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop);
}

void CXMLGuiLoader::CreateGuiGroup(StdString strGroupName, StdString strGuiType, IXMLArchive &Archive)
{
  	CHashString hszGuiName(strGroupName.c_str());
  	CHashString hszGuiType(strGuiType.c_str());

	CREATEOBJECTPARAMS cop;
  	INITOBJECTPARAMS iop;
	SERIALIZEOBJECTPARAMS sop;
	FINDOBJNAME fon;

  	cop.name = &hszGuiName;
  	cop.typeName = &hszGuiType;
  	cop.parentName = NULL;
  	iop.name = &hszGuiName;
  	sop.name = &hszGuiName;
  	sop.archive = &Archive;
  	
  	fon.searchName = &hszGuiName;
  	fon.compType = &hszGuiType;
  	fon.bFound = false;
  
  	// first check if object already exists in object factor
static DWORD msgHash_FindGUIObject = CHashString(_T("FindGUIObject")).GetUniqueID();
  	m_ToolBox->SendMessage(msgHash_FindGUIObject, sizeof(FINDOBJNAME), &fon);
  	if( !fon.bFound )
  	{
  		// not found...
  		// send message to ee to create new gui element
static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
  		m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop);
static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
  		m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop);
  	}
}

void CXMLGuiLoader::LoadGuiFileV95(IXMLArchive *Archive)
{   
  	StdString szPageNode;
  	StdString szGuiNode;
  	StdString szNum;
  	StdString szGuiName;
  	StdString szGuiType;
  	StdString szGroupName;
  	StdString szGroupType;
  	StdString szPageName;
  	StdString szPageType;
	int iNumGuiElements;
  	int iNumSubGuiElements;
  	int iNumPages;
	
  	CHashString hszGuiName(_T(""));
  	CHashString hszGuiType(_T(""));
  	CHashString hszGroupName(_T(""));
  	CHashString hszGroupType(_T(""));
  	CHashString hszPageName(_T(""));
  	CHashString hszPageType(_T(""));
  
  	GUIPAGEMESSAGE gmsgPage;
  	ADDGUIELEMENT gmsgGroup;
  
	Archive->Read(szNum);
	iNumPages = _tstoi(szNum);

  	while( iNumPages != 0 )
  	{
  		Archive->GetNode(szPageNode);	
  		if( _tcscmp(szPageNode, _T("PAGE")) == 0 ||
  			_tcscmp(szPageNode, _T("PAGEBUFFER")) == 0 )
  		{
  			Archive->Read(szNum);
  			iNumGuiElements = _tstoi(szNum);

  			Archive->Read( szPageName );
  			Archive->Read( szPageType );
  			// create page and add to ee
  			CreatePage( szPageName, szPageType);
  			// setup page so elements can add themseleves
  			hszPageName = szPageName.c_str();
  			hszPageType = szPageType.c_str();

			while( iNumGuiElements != 0 )
  			{
  				Archive->GetNode( szGuiNode );

  				if( _tcscmp(szGuiNode, _T("GUIELEMENT")) == 0 )
  				{
  					Archive->Read( szGuiName );
  					Archive->Read( szGuiType );

  					// create gui element
  					CreateGuiElement(szGuiName, szGuiType, *Archive);

  					// send message to ee to add gui element to this page
  					hszGuiName = szGuiName.c_str();
  					hszGuiType = szGuiType.c_str();
  					gmsgPage.name = &hszGuiName;
  					gmsgPage.compType = &hszGuiType;
static DWORD msgHash_AddGUIElementToPage = CHashString(_T("AddGUIElementToPage")).GetUniqueID();
  					m_ToolBox->SendMessage(msgHash_AddGUIElementToPage, sizeof(GUIPAGEMESSAGE), &gmsgPage, &hszPageName);			
  				}
  				else if( _tcscmp(szGuiNode, _T("GUIGROUP")) == 0 )
  				{
  					Archive->Read(szNum);
  					iNumSubGuiElements = _tstoi(szNum);

  					Archive->Read( szGroupName );
  					Archive->Read( szGroupType );

  					// create gui group
  					CreateGuiGroup(szGroupName, szGroupType, *Archive);

  					// send message to add this group to this page
  					hszGroupName = szGroupName.c_str();
  					hszGroupType = szGroupType.c_str();
  					gmsgPage.name = &hszGroupName;
  					gmsgPage.compType = &hszGroupType;
static DWORD msgHash_AddGUIElementToPage = CHashString(_T("AddGUIElementToPage")).GetUniqueID();
  					m_ToolBox->SendMessage(msgHash_AddGUIElementToPage, sizeof(GUIPAGEMESSAGE), &gmsgPage, &hszPageName);
		  			
  					while( iNumSubGuiElements != 0 )
  					{
  						Archive->Read( szGuiName );
  						Archive->Read( szGuiType );

  						// create gui element
  						CreateGuiElement(szGuiName, szGuiType, *Archive);

  						hszGuiName = szGuiName.c_str();
  						hszGuiType = szGuiType.c_str();

  						// send message to ee to add gui element to this page
  						gmsgGroup.m_GuiName = &hszGuiName;
  						gmsgGroup.m_CompType = &hszGuiType;
  						gmsgGroup.m_ZOrder = 0;
static DWORD msgHash_AddGUIElement = CHashString(_T("AddGUIElement")).GetUniqueID();
  						m_ToolBox->SendMessage(msgHash_AddGUIElement, sizeof(ADDGUIELEMENT), &gmsgGroup, &hszGroupName);

  						if( _tcscmp(szPageNode, _T("PAGEBUFFER")) == 0)
  						{
  							gmsgPage.name = &hszGuiName;
  							gmsgPage.compType = &hszGuiType;
static DWORD msgHash_AddGUIElementToPage = CHashString(_T("AddGUIElementToPage")).GetUniqueID();
  							m_ToolBox->SendMessage(msgHash_AddGUIElementToPage, sizeof(GUIPAGEMESSAGE), &gmsgPage, &hszPageName);		
						}
  						iNumSubGuiElements--;
					}
				}
  				iNumGuiElements--;
			}
		}
		iNumPages--;
	}
}

void CXMLGuiLoader::LoadGuiFileV1(IXMLArchive *Archive)
{
	IArchive *MemArchive;
	CHashString memType(_T("Memory"));
	CREATEARCHIVE ca;

	CREATEOBJECTPARAMS cop;
	INITOBJECTPARAMS iop;
	SERIALIZEOBJECTPARAMS sop;
	FINDOBJNAME fon;

	StdString szNodeName;
	StdString szName;
	
  	CHashString hszName(_T(""));
  	CHashString hszTypeName(_T(""));
    
	// create archive
	ca.mode = STREAM_MODE_READ | STREAM_MODE_WRITE;
	ca.streamType = &memType;
	ca.streamData = NULL;
	ca.streamSize = 0;
	static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE),&ca) != MSG_HANDLED)
	{
		return;
	}
	MemArchive = ca.archive;

  	while (Archive->GetNode(szNodeName))
	{
  		if( _tcscmp(szNodeName, _T("CGUIPage")) == 0 )
  		{
			hszTypeName = szNodeName.c_str();

			Archive->Read( szName );
			hszName = szName.c_str();

			cop.name = &hszName;
			cop.typeName = &hszTypeName;
			cop.parentName = NULL;

			static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop);

			iop.name = &hszName;
			static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop);

			sop.name = &hszName;
			sop.archive = Archive;
			static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop);
		}

		else if( _tcscmp(szNodeName, _T("CGUIGroup")) == 0 )
		{
			hszTypeName = szNodeName.c_str();

			Archive->Read( szName, _T("Name") );
			hszName = szName.c_str();

			fon.searchName = &hszName;
			fon.compType = &hszTypeName;
			fon.bFound = false;

			// first check if object already exists in EE
			static DWORD msgHash_FindGUIObject = CHashString(_T("FindGUIObject")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_FindGUIObject, sizeof(FINDOBJNAME), &fon);
			if( !fon.bFound )
			{

				cop.parentName = NULL;
				cop.typeName = &hszTypeName;
				cop.name = &hszName;
				static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
				m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop);

				iop.name = &hszName;
				static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
				m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop);
			}
			sop.name = &hszName;
			sop.archive = Archive;
			static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop);
		}

		else
		{
			hszTypeName = szNodeName.c_str();

			Archive->Read( szName, _T("Name") );
			hszName = szName.c_str();

			fon.searchName = &hszName;
			fon.compType = &hszTypeName;
			fon.bFound = false;

			// first check if object already exists in EE
			static DWORD msgHash_FindGUIObject = CHashString(_T("FindGUIObject")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_FindGUIObject, sizeof(FINDOBJNAME), &fon);
			if( !fon.bFound )
			{

				cop.parentName = NULL;
				cop.typeName = &hszTypeName;
				cop.name = &hszName;
				static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
				m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop);

				iop.name = &hszName;
				static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
				m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop);
			}


			// serialize differently for every gui type:
			if( _tcscmp(szNodeName, _T("CGUIItem")) == 0 )
			{
				// old data
				float fPosX;
				float fPosY;
				float fSizeX;
				float fSizeY;
				int	iZOrder;
				StdString normTex;
				StdString highTex;
				StdString selTex;
				StdString grayTex;
				StdString staticText;
				int iID;
				StdString fontName;

				// new data
				int iNormFontColor = 0;
				int iHighFontColor = 0;
				int iSelFontColor = 0;
				int iGrayFontColor = 0;
				int iTextOffSetX = 0;
				int iTextOffSetY = 0;
				bool bCenterText = false;
				bool bUseSmartScale = false;
				int iRectX1 = 0;
				int iRectX2 = 0;
				int iRectY1 = 0;
				int iRectY2 = 0;

				// set data
				Archive->Read(fPosX);
				Archive->Read(fPosY);
				Archive->Read(fSizeX);
				Archive->Read(fSizeY);
				Archive->Read(iZOrder);
				Archive->Read(normTex);
				Archive->Read(highTex);
				Archive->Read(selTex);
				Archive->Read(grayTex);
				Archive->Read(staticText);
				Archive->Read(iID);
				Archive->Read(fontName);
				
				// start at the begining
				MemArchive->SetIsWriting(true);
				MemArchive->SeekTo(0, SEEK_SET);
				MemArchive->Write(fPosX);
				MemArchive->Write(fPosY);
				MemArchive->Write(fSizeX);
				MemArchive->Write(fSizeY);
				MemArchive->Write(iZOrder);
				MemArchive->Write(iRectX1);
				MemArchive->Write(iRectX2);
				MemArchive->Write(iRectY1);
				MemArchive->Write(iRectY2);
				MemArchive->Write(normTex);
				MemArchive->Write(highTex);
				MemArchive->Write(selTex);
				MemArchive->Write(grayTex);
				MemArchive->Write(staticText);
				MemArchive->Write(fontName);
				MemArchive->Write(iNormFontColor);
				MemArchive->Write(iHighFontColor);
				MemArchive->Write(iSelFontColor);
				MemArchive->Write(iGrayFontColor);
				MemArchive->Write(iTextOffSetX);
				MemArchive->Write(iTextOffSetY);
				MemArchive->Write(bCenterText);
				MemArchive->Write(bUseSmartScale);
			}
			else if ( _tcscmp(szNodeName, _T("CGUICheckBox")) == 0 )
			{
				// old data
				float fPosX;
				float fPosY;
				float fSizeX;
				float fSizeY;
				int	iZOrder;
				StdString normTex_unselect;
				StdString highTex_select;
				StdString normTex_select;
				StdString grayTex_unselect;
				StdString staticText;
				int iID;
				bool bIsChecked;
				StdString fontName;

				// new data
				StdString highTex_unselect = _T("Data\\GUITextures\\checkbutton_Unselected_high.tex");
				StdString depressedTex = _T("Data\\GUITextures\\checkbutton_depressed.tex");
				StdString grayTex_select = _T("Data\\GUITextures\\checkbutton_selected_gray.tex");
				int iRectX1 = 0;
				int iRectX2 = 0;
				int iRectY1 = 0;
				int iRectY2 = 0;

				Archive->Read(fPosX);
				Archive->Read(fPosY);
				Archive->Read(fSizeX);
				Archive->Read(fSizeY);
				Archive->Read(iZOrder);
				Archive->Read(normTex_unselect);
				Archive->Read(highTex_select);
				Archive->Read(normTex_select);
				Archive->Read(grayTex_unselect);
				Archive->Read(staticText);
				Archive->Read(iID);
				Archive->Read(bIsChecked);
				Archive->Read(fontName);

				// start at the begining
				MemArchive->SeekTo(0, SEEK_SET);
				MemArchive->SetIsWriting(true);
				MemArchive->Write(fPosX);
				MemArchive->Write(fPosY);
				MemArchive->Write(fSizeX);
				MemArchive->Write(fSizeY);
				MemArchive->Write(iZOrder);
				MemArchive->Write(iRectX1);
				MemArchive->Write(iRectX2);
				MemArchive->Write(iRectY1);
				MemArchive->Write(iRectY2);
				MemArchive->Write(normTex_unselect);
				MemArchive->Write(highTex_unselect);
				MemArchive->Write(grayTex_unselect);
				MemArchive->Write(depressedTex);
				MemArchive->Write(normTex_select);
				MemArchive->Write(highTex_select);
				MemArchive->Write(grayTex_select);
				MemArchive->Write(bIsChecked);
			}
			else if ( _tcscmp(szNodeName, _T("CGUIRadioButton")) == 0 )
			{
				// old data
				float fPosX;
				float fPosY;
				float fSizeX;
				float fSizeY;
				int	iZOrder;
				StdString normTex_unselect;
				StdString highTex_select;
				StdString normTex_select;
				StdString grayTex_unselect;
				StdString staticText;
				int iID;
				bool bIsChecked;
				StdString fontName;

				// new data
				StdString highTex_unselect = _T("Data\\GUITextures\\radiobutton_unselected_high.tex");
				StdString depressedTex = _T("Data\\GUITextures\\radiobutton_depressed.tex");
				StdString grayTex_select = _T("Data\\GUITextures\\radiobutton_selected_gray.tex");
				int iRectX1 = 0;
				int iRectX2 = 0;
				int iRectY1 = 0;
				int iRectY2 = 0;

				Archive->Read(fPosX);
				Archive->Read(fPosY);
				Archive->Read(fSizeX);
				Archive->Read(fSizeY);
				Archive->Read(iZOrder);
				Archive->Read(normTex_unselect);
				Archive->Read(highTex_select);
				Archive->Read(normTex_select);
				Archive->Read(grayTex_unselect);
				Archive->Read(staticText);
				Archive->Read(iID);
				Archive->Read(bIsChecked);
				Archive->Read(fontName);

				// start at the begining
				MemArchive->SeekTo(0, SEEK_SET);
				MemArchive->SetIsWriting(true);
				MemArchive->Write(fPosX);
				MemArchive->Write(fPosY);
				MemArchive->Write(fSizeX);
				MemArchive->Write(fSizeY);
				MemArchive->Write(iZOrder);
				MemArchive->Write(iRectX1);
				MemArchive->Write(iRectX2);
				MemArchive->Write(iRectY1);
				MemArchive->Write(iRectY2);
				MemArchive->Write(normTex_unselect);
				MemArchive->Write(highTex_unselect);
				MemArchive->Write(grayTex_unselect);
				MemArchive->Write(depressedTex);
				MemArchive->Write(normTex_select);
				MemArchive->Write(highTex_select);
				MemArchive->Write(grayTex_select);
				MemArchive->Write(bIsChecked);
			}
			else if (_tcscmp(szNodeName, _T("CGUISlider")) == 0 )
			{
				// old data
				float fPosX;
				float fPosY;
				float fSizeX;
				float fSizeY;
				int	iZOrder;
				StdString barNormTex;
				StdString barHighTex;
				StdString barSelTex;
				StdString barGrayTex;
				StdString buttonNormTex;
				StdString buttonHighTex;
				StdString buttonSelTex;
				StdString buttonGrayTex;				
				int iID;
				StdString SliderType;

				// new data
				
				Archive->Read(fPosX);
				Archive->Read(fPosY);
				Archive->Read(fSizeX);
				Archive->Read(fSizeY);
				Archive->Read(iZOrder);
				Archive->Read(barNormTex);
				Archive->Read(barHighTex);
				Archive->Read(barSelTex);
				Archive->Read(barGrayTex);
				Archive->Read(buttonNormTex);
				Archive->Read(buttonHighTex);
				Archive->Read(buttonSelTex);
				Archive->Read(buttonGrayTex);
				Archive->Read(iID);
				Archive->Read(SliderType);

				MemArchive->SeekTo(0, SEEK_SET);
				MemArchive->SetIsWriting(true);
				MemArchive->Write(fPosX);
				MemArchive->Write(fPosY);
				MemArchive->Write(fSizeX);
				MemArchive->Write(fSizeY);
				MemArchive->Write(iZOrder);
				MemArchive->Write(barNormTex);
				MemArchive->Write(barGrayTex);
				MemArchive->Write(buttonNormTex);
				MemArchive->Write(buttonSelTex);
				MemArchive->Write(buttonGrayTex);
				MemArchive->Write(SliderType);

			}
			else if (_tcscmp(szNodeName, _T("CGUIListBox")) == 0)
			{
				// old data
				float fPosX;
				float fPosY;
				float fSizeX;
				float fSizeY;
				int	iZOrder;
				StdString barNormTex;
				StdString barHighTex;
				StdString barSelTex;
				StdString barGrayTex;
				StdString buttonNormTex;
				StdString buttonHighTex;
				StdString buttonSelTex;
				StdString buttonGrayTex;
				StdString backgroundTex;
				StdString sourceText;
				int iID;
				StdString fontName;
				
				// new data
				StdString horizBarTex = _T("Data\\GUITextures\\slider_back.tex");
				StdString horizButtonNormTex = _T("Data\\GUITextures\\slider_button.tex");
				StdString horizButtonHighTex = _T("Data\\GUITextures\\slider_button_high.tex");
				StdString horizButtonSelTex = _T("Data\\GUITextures\\slider_button_high.tex");
				int iSelFontColor = 65535;
				int iNormFontColor = 16777215;
				bool bWrapText = false;
				bool bDisableSelect = false;
				float fVertSliderWidth = 32;
				float fHorizSliderWidth = 32;
				int iTextOffSetX = 8;
				int iTextOffSetY = 10;

				// set data
				Archive->Read(fPosX);
				Archive->Read(fPosY);
				Archive->Read(fSizeX);
				Archive->Read(fSizeY);
				Archive->Read(iZOrder);
				Archive->Read(barNormTex);
				Archive->Read(barHighTex);
				Archive->Read(barSelTex);
				Archive->Read(barGrayTex);
				Archive->Read(buttonNormTex);
				Archive->Read(buttonHighTex);
				Archive->Read(buttonSelTex);
				Archive->Read(buttonGrayTex);
				Archive->Read(backgroundTex);
				Archive->Read(sourceText);
				Archive->Read(iID);
				Archive->Read(fontName);

				MemArchive->SeekTo(0, SEEK_SET);
				MemArchive->SetIsWriting(true);
				MemArchive->Write(fPosX);
				MemArchive->Write(fPosY);
				MemArchive->Write(fSizeX);
				MemArchive->Write(fSizeY);
				MemArchive->Write(iZOrder);
				MemArchive->Write(fVertSliderWidth);
				MemArchive->Write(fHorizSliderWidth);
				MemArchive->Write(barNormTex);
				MemArchive->Write(buttonNormTex);
				MemArchive->Write(buttonHighTex);
				MemArchive->Write(buttonSelTex);
				MemArchive->Write(horizBarTex);
				MemArchive->Write(horizButtonNormTex);
				MemArchive->Write(horizButtonHighTex);
				MemArchive->Write(horizButtonSelTex);
				MemArchive->Write(backgroundTex);
				MemArchive->Write(sourceText);
				MemArchive->Write(fontName);
				MemArchive->Write(iNormFontColor);
				MemArchive->Write(iSelFontColor);
				MemArchive->Write(iTextOffSetX);
				MemArchive->Write(iTextOffSetY);
				MemArchive->Write(bWrapText);
				MemArchive->Write(bDisableSelect);

			}
			else if (_tcscmp(szNodeName, _T("CGUIInsetMap")) == 0)
			{
				// old data
				float fPosX;
				float fPosY;
				float fSizeX;
				float fSizeY;
				int	iZOrder;
				StdString NormTex;
				int iID;

				// set data
				Archive->Read(fPosX);
				Archive->Read(fPosY);
				Archive->Read(fSizeX);
				Archive->Read(fSizeY);
				Archive->Read(iZOrder);
				Archive->Read(NormTex);
				Archive->Read(iID);

				MemArchive->SeekTo(0, SEEK_SET);
				MemArchive->SetIsWriting(true);
				MemArchive->Write(fPosX);
				MemArchive->Write(fPosY);
				MemArchive->Write(fSizeX);
				MemArchive->Write(fSizeY);
				MemArchive->Write(iZOrder);
				MemArchive->Write(NormTex);

			}
			else if (_tcscmp(szNodeName, _T("CGUICompass")) == 0)
			{
				// old data
				float fPosX;
				float fPosY;
				float fSizeX;
				float fSizeY;
				int	iZOrder;
				StdString normTex;
				int iID;

				// set data
				Archive->Read(fPosX);
				Archive->Read(fPosY);
				Archive->Read(fSizeX);
				Archive->Read(fSizeY);
				Archive->Read(iZOrder);
				Archive->Read(normTex);
				Archive->Read(iID);

				MemArchive->SeekTo(0, SEEK_SET);
				MemArchive->SetIsWriting(true);
				MemArchive->Write(fPosX);
				MemArchive->Write(fPosY);
				MemArchive->Write(fSizeX);
				MemArchive->Write(fSizeY);
				MemArchive->Write(iZOrder);
				MemArchive->Write(normTex);

			}
			else if (_tcscmp(szNodeName, _T("CGUIEditText")) == 0)
			{
				// old data
				float fPosX;
				float fPosY;
				float fSizeX;
				float fSizeY;
				int	iZOrder;
				StdString normTex;
				StdString highTex;
				StdString selTex;
				StdString grayTex;
				StdString editText;
				int iID;
				int iMaxChar;
				StdString fontName;

				// new data
				int iFontColor = 16777215;
				int iCursorColor = 0;
				int iTextOffSetX = 0;
				int iTextOffSetY = 0;
				bool bCenterText = false;

				// set data
				Archive->Read(fPosX);
				Archive->Read(fPosY);
				Archive->Read(fSizeX);
				Archive->Read(fSizeY);
				Archive->Read(iZOrder);
				Archive->Read(normTex);
				Archive->Read(highTex);
				Archive->Read(selTex);
				Archive->Read(grayTex);
				Archive->Read(editText);
				Archive->Read(iID);
				Archive->Read(iMaxChar);
				Archive->Read(fontName);

				MemArchive->SeekTo(0, SEEK_SET);
				MemArchive->SetIsWriting(true);
				MemArchive->Write(fPosX);
				MemArchive->Write(fPosY);
				MemArchive->Write(fSizeX);
				MemArchive->Write(fSizeY);
				MemArchive->Write(iZOrder);
				MemArchive->Write(normTex);
				MemArchive->Write(editText);
				MemArchive->Write(iMaxChar);
				MemArchive->Write(fontName);
				MemArchive->Write(iFontColor);
				MemArchive->Write(iCursorColor);
				MemArchive->Write(iTextOffSetX);
				MemArchive->Write(iTextOffSetY);
				MemArchive->Write(bCenterText);
			}
			else if (_tcscmp(szNodeName, _T("CGUIStaticText")) == 0)
			{
				// old data
				float fPosX;
				float fPosY;
				float fSizeX;
				float fSizeY;
				int	iZOrder;
				StdString normTex;
				StdString highTex;
				StdString selTex;
				StdString grayTex;
				StdString staticText;
				int iID;
				StdString fontName;

				// new data
				int iNormFontColor = 16777215;
				int iHighFontColor = 16777215;
				int iSelFontColor = 16777215;
				int iGrayFontColor = 16777215;
				bool bHideTex = false;
				int iTextOffSetX = 0;
				int iTextOffSetY = 0;
				bool bCenterText = false;

				// set data
				Archive->Read(fPosX);
				Archive->Read(fPosY);
				Archive->Read(fSizeX);
				Archive->Read(fSizeY);
				Archive->Read(iZOrder);
				Archive->Read(normTex);
				Archive->Read(highTex);
				Archive->Read(selTex);
				Archive->Read(grayTex);
				Archive->Read(staticText);
				Archive->Read(iID);
				Archive->Read(fontName);

				MemArchive->SeekTo(0, SEEK_SET);
				MemArchive->SetIsWriting(true);
				MemArchive->Write(fPosX);
				MemArchive->Write(fPosY);
				MemArchive->Write(fSizeX);
				MemArchive->Write(fSizeY);
				MemArchive->Write(iZOrder);
				MemArchive->Write(normTex);
				MemArchive->Write(staticText);
				MemArchive->Write(fontName);
				MemArchive->Write(iNormFontColor);
				MemArchive->Write(iHighFontColor);
				MemArchive->Write(iSelFontColor);
				MemArchive->Write(iGrayFontColor);
				MemArchive->Write(iTextOffSetX);
				MemArchive->Write(iTextOffSetY);
				MemArchive->Write(bCenterText);
				MemArchive->Write(bHideTex);		
			}

			// reset archive to begining
			MemArchive->SeekTo(0, SEEK_SET);
			MemArchive->SetIsWriting(false);
			sop.name = &hszName;
			sop.archive = MemArchive;
			static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop);
		}
	}
	MemArchive->Close();
}

void CXMLGuiLoader::LoadGuiFileV11a(IXMLArchive *Archive, UINT mapIndex)
{
	IArchive *MemArchive;
	CHashString memType(_T("Memory"));
	CREATEARCHIVE ca;

	CREATEOBJECTPARAMS cop;
	INITOBJECTPARAMS iop;
	SERIALIZEOBJECTPARAMS sop;
	FINDOBJNAME fon;

	StdString szNodeName;
	StdString szName;
	
  	CHashString hszName(_T(""));
  	CHashString hszTypeName(_T(""));
    
	// create archive
	ca.mode = STREAM_MODE_READ | STREAM_MODE_WRITE;
	ca.streamType = &memType;
	ca.streamData = NULL;
	ca.streamSize = 0;
	static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE),&ca) != MSG_HANDLED)
	{
		return;
	}
	MemArchive = ca.archive;

  	while (Archive->GetNode(szNodeName))
	{
		// serialize differently for only CGUIItems, CGUICheckBoxes, CGUIRadios and CGUIListBoxes
		if( _tcscmp(szNodeName, _T("CGUIItem")) == 0 )
		{
			hszTypeName = szNodeName.c_str();

			Archive->Read( szName, _T("Name") );
			hszName = szName.c_str();

			fon.searchName = &hszName;
			fon.compType = &hszTypeName;
			fon.bFound = false;

			// first check if object already exists in EE
			static DWORD msgHash_FindGUIObject = CHashString(_T("FindGUIObject")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_FindGUIObject, sizeof(FINDOBJNAME), &fon);
			if( !fon.bFound )
			{

				cop.parentName = NULL;
				cop.typeName = &hszTypeName;
				cop.name = &hszName;
				static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
				m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop);

				iop.name = &hszName;
				static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
				m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop);
			}

			// old data
			float fPosX;
			float fPosY;
			float fSizeX;
			float fSizeY;
			int	iZOrder;
			StdString normTex;
			StdString highTex;
			StdString selTex;
			StdString grayTex;
			StdString staticText;
			StdString fontName;
			int iNormFontColor;
			int iHighFontColor;
			int iSelFontColor;
			int iGrayFontColor;
			int iTextOffSetX;
			int iTextOffSetY;
			bool bCenterText;
			bool bUseSmartScale;
			// new data

			int iRectX1 = 0;
			int iRectX2 = 0;
			int iRectY1 = 0;
			int iRectY2 = 0;

			// set data
			Archive->Read(fPosX);
			Archive->Read(fPosY);
			Archive->Read(fSizeX);
			Archive->Read(fSizeY);
			Archive->Read(iZOrder);
			Archive->Read(normTex);
			Archive->Read(highTex);
			Archive->Read(selTex);
			Archive->Read(grayTex);
			Archive->Read(staticText);
			Archive->Read(fontName);
			Archive->Read(iNormFontColor);
			Archive->Read(iHighFontColor);
			Archive->Read(iSelFontColor);
			Archive->Read(iGrayFontColor);
			Archive->Read(iTextOffSetX);
			Archive->Read(iTextOffSetY);
			Archive->Read(bCenterText);
			Archive->Read(bUseSmartScale);
			
			// start at the begining
			MemArchive->SeekTo(0, SEEK_SET);
			MemArchive->SetIsWriting(true);
			MemArchive->Write(fPosX);
			MemArchive->Write(fPosY);
			MemArchive->Write(fSizeX);
			MemArchive->Write(fSizeY);
			MemArchive->Write(iZOrder);
			MemArchive->Write(iRectX1);
			MemArchive->Write(iRectX2);
			MemArchive->Write(iRectY1);
			MemArchive->Write(iRectY2);
			MemArchive->Write(normTex);
			MemArchive->Write(highTex);
			MemArchive->Write(selTex);
			MemArchive->Write(grayTex);
			MemArchive->Write(staticText);
			MemArchive->Write(fontName);
			MemArchive->Write(iNormFontColor);
			MemArchive->Write(iHighFontColor);
			MemArchive->Write(iSelFontColor);
			MemArchive->Write(iGrayFontColor);
			MemArchive->Write(iTextOffSetX);
			MemArchive->Write(iTextOffSetY);
			MemArchive->Write(bCenterText);
			MemArchive->Write(bUseSmartScale);

			MemArchive->SeekTo(0, SEEK_SET);
			MemArchive->SetIsWriting(false);
			sop.name = &hszName;
			sop.archive = MemArchive;
			static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop);
		}
		else if ( _tcscmp(szNodeName, _T("CGUICheckBox")) == 0 )
		{
			hszTypeName = szNodeName.c_str();

			Archive->Read( szName, _T("Name") );
			hszName = szName.c_str();

			fon.searchName = &hszName;
			fon.compType = &hszTypeName;
			fon.bFound = false;

			// first check if object already exists in EE
			static DWORD msgHash_FindGUIObject = CHashString(_T("FindGUIObject")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_FindGUIObject, sizeof(FINDOBJNAME), &fon);
			if( !fon.bFound )
			{

				cop.parentName = NULL;
				cop.typeName = &hszTypeName;
				cop.name = &hszName;
				static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
				m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop);

				iop.name = &hszName;
				static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
				m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop);
			}
			// old data
			float fPosX;
			float fPosY;
			float fSizeX;
			float fSizeY;
			int	iZOrder;
			StdString normTex_unselect;
			StdString highTex_select;
			StdString normTex_select;
			StdString grayTex_unselect;
			StdString highTex_unselect;
			StdString depressedTex;
			StdString grayTex_select;
			StdString staticText;
			bool bIsChecked;
			StdString fontName;

			// new data
			int iRectX1 = 0;
			int iRectX2 = 0;
			int iRectY1 = 0;
			int iRectY2 = 0;

			Archive->Read(fPosX);
			Archive->Read(fPosY);
			Archive->Read(fSizeX);
			Archive->Read(fSizeY);
			Archive->Read(iZOrder);
			Archive->Read(normTex_unselect);
			Archive->Read(highTex_unselect);
			Archive->Read(grayTex_unselect);
			Archive->Read(depressedTex);
			Archive->Read(normTex_select);
			Archive->Read(highTex_select);
			Archive->Read(grayTex_select);
			Archive->Read(bIsChecked);

			// start at the begining
			MemArchive->SeekTo(0, SEEK_SET);
			MemArchive->SetIsWriting(true);
			MemArchive->Write(fPosX);
			MemArchive->Write(fPosY);
			MemArchive->Write(fSizeX);
			MemArchive->Write(fSizeY);
			MemArchive->Write(iZOrder);
			MemArchive->Write(iRectX1);
			MemArchive->Write(iRectX2);
			MemArchive->Write(iRectY1);
			MemArchive->Write(iRectY2);
			MemArchive->Write(normTex_unselect);
			MemArchive->Write(highTex_unselect);
			MemArchive->Write(grayTex_unselect);
			MemArchive->Write(depressedTex);
			MemArchive->Write(normTex_select);
			MemArchive->Write(highTex_select);
			MemArchive->Write(grayTex_select);
			MemArchive->Write(bIsChecked);

			MemArchive->SeekTo(0, SEEK_SET);
			MemArchive->SetIsWriting(false);
			sop.name = &hszName;
			sop.archive = MemArchive;
			static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop);
		}
		else if ( _tcscmp(szNodeName, _T("CGUIRadioButton")) == 0 )
		{
			hszTypeName = szNodeName.c_str();

			Archive->Read( szName, _T("Name") );
			hszName = szName.c_str();

			fon.searchName = &hszName;
			fon.compType = &hszTypeName;
			fon.bFound = false;

			// first check if object already exists in EE
			static DWORD msgHash_FindGUIObject = CHashString(_T("FindGUIObject")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_FindGUIObject, sizeof(FINDOBJNAME), &fon);
			if( !fon.bFound )
			{

				cop.parentName = NULL;
				cop.typeName = &hszTypeName;
				cop.name = &hszName;
				static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
				m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop);

				iop.name = &hszName;
				static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
				m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop);
			}
			// old data
			float fPosX;
			float fPosY;
			float fSizeX;
			float fSizeY;
			int	iZOrder;
			StdString normTex_unselect;
			StdString highTex_select;
			StdString normTex_select;
			StdString grayTex_unselect;
			StdString highTex_unselect;
			StdString depressedTex;
			StdString grayTex_select;
			StdString staticText;
			bool bIsChecked;
			StdString fontName;

			// new data
			int iRectX1 = 0;
			int iRectX2 = 0;
			int iRectY1 = 0;
			int iRectY2 = 0;

			Archive->Read(fPosX);
			Archive->Read(fPosY);
			Archive->Read(fSizeX);
			Archive->Read(fSizeY);
			Archive->Read(iZOrder);
			Archive->Read(normTex_unselect);
			Archive->Read(highTex_unselect);
			Archive->Read(grayTex_unselect);
			Archive->Read(depressedTex);
			Archive->Read(normTex_select);
			Archive->Read(highTex_select);
			Archive->Read(grayTex_select);
			Archive->Read(bIsChecked);

			// start at the begining
			MemArchive->SeekTo(0, SEEK_SET);
			MemArchive->SetIsWriting(true);
			MemArchive->Write(fPosX);
			MemArchive->Write(fPosY);
			MemArchive->Write(fSizeX);
			MemArchive->Write(fSizeY);
			MemArchive->Write(iZOrder);
			MemArchive->Write(iRectX1);
			MemArchive->Write(iRectX2);
			MemArchive->Write(iRectY1);
			MemArchive->Write(iRectY2);
			MemArchive->Write(normTex_unselect);
			MemArchive->Write(highTex_unselect);
			MemArchive->Write(grayTex_unselect);
			MemArchive->Write(depressedTex);
			MemArchive->Write(normTex_select);
			MemArchive->Write(highTex_select);
			MemArchive->Write(grayTex_select);
			MemArchive->Write(bIsChecked);

			MemArchive->SeekTo(0, SEEK_SET);
			MemArchive->SetIsWriting(false);
			sop.name = &hszName;
			sop.archive = MemArchive;
			static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop);
		}
		else if (_tcscmp(szNodeName, _T("CGUIListBox")) == 0)
		{
			hszTypeName = szNodeName.c_str();

			Archive->Read( szName, _T("Name") );
			hszName = szName.c_str();

			fon.searchName = &hszName;
			fon.compType = &hszTypeName;
			fon.bFound = false;

			// first check if object already exists in EE
			static DWORD msgHash_FindGUIObject = CHashString(_T("FindGUIObject")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_FindGUIObject, sizeof(FINDOBJNAME), &fon);
			if( !fon.bFound )
			{

				cop.parentName = NULL;
				cop.typeName = &hszTypeName;
				cop.name = &hszName;
				static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
				m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop);

				iop.name = &hszName;
				static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
				m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop);
			}

			// old data
			float fPosX;
			float fPosY;
			float fSizeX;
			float fSizeY;
			int	iZOrder;
			StdString barNormTex;
			StdString barHighTex;
			StdString barSelTex;
			StdString barGrayTex;
			StdString buttonNormTex;
			StdString buttonHighTex;
			StdString buttonSelTex;
			StdString buttonGrayTex;
			StdString backgroundTex;
			StdString sourceText;
			StdString fontName;
			StdString horizBarTex;
			StdString horizButtonNormTex;
			StdString horizButtonHighTex;
			StdString horizButtonSelTex;	
			int iSelFontColor;
			int iNormFontColor;
			bool bWrapText;
			bool bDisableSelect;			
			int iTextOffSetX;
			int iTextOffSetY;

			// new data
			float fVertSliderWidth = 32;
			float fHorizSliderWidth = 32;

			// set data
			Archive->Read(fPosX);
			Archive->Read(fPosY);
			Archive->Read(fSizeX);
			Archive->Read(fSizeY);
			Archive->Read(iZOrder);
			Archive->Read(barNormTex);
			Archive->Read(buttonNormTex);
			Archive->Read(buttonHighTex);
			Archive->Read(buttonSelTex);
			Archive->Read(horizBarTex);
			Archive->Read(horizButtonNormTex);
			Archive->Read(horizButtonHighTex);
			Archive->Read(horizButtonSelTex);
			Archive->Read(backgroundTex);
			Archive->Read(sourceText);
			Archive->Read(fontName);
			Archive->Read(iNormFontColor);
			Archive->Read(iSelFontColor);
			Archive->Read(iTextOffSetX);
			Archive->Read(iTextOffSetY);
			Archive->Read(bWrapText);
			Archive->Read(bDisableSelect);

			MemArchive->SeekTo(0, SEEK_SET);
			MemArchive->SetIsWriting(true);
			MemArchive->Write(fPosX);
			MemArchive->Write(fPosY);
			MemArchive->Write(fSizeX);
			MemArchive->Write(fSizeY);
			MemArchive->Write(iZOrder);
			MemArchive->Write(fVertSliderWidth);
			MemArchive->Write(fHorizSliderWidth);
			MemArchive->Write(barNormTex);
			MemArchive->Write(buttonNormTex);
			MemArchive->Write(buttonHighTex);
			MemArchive->Write(buttonSelTex);
			MemArchive->Write(horizBarTex);
			MemArchive->Write(horizButtonNormTex);
			MemArchive->Write(horizButtonHighTex);
			MemArchive->Write(horizButtonSelTex);
			MemArchive->Write(backgroundTex);
			MemArchive->Write(sourceText);
			MemArchive->Write(fontName);
			MemArchive->Write(iNormFontColor);
			MemArchive->Write(iSelFontColor);
			MemArchive->Write(iTextOffSetX);
			MemArchive->Write(iTextOffSetY);
			MemArchive->Write(bWrapText);
			MemArchive->Write(bDisableSelect);

			MemArchive->SeekTo(0, SEEK_SET);
			MemArchive->SetIsWriting(false);
			sop.name = &hszName;
			sop.archive = MemArchive;
			static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop);
		}
		else
		{
			// serialize everything else the same as before
			hszTypeName = szNodeName.c_str();

			Archive->Read( szName, _T("Name") );
			hszName = szName.c_str();

			fon.searchName = &hszName;
			fon.compType = &hszTypeName;
			fon.bFound = false;

			// first check if object already exists in EE
			static DWORD msgHash_FindGUIObject = CHashString(_T("FindGUIObject")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_FindGUIObject, sizeof(FINDOBJNAME), &fon);
			if( !fon.bFound )
			{

				cop.parentName = NULL;
				cop.typeName = &hszTypeName;
				cop.name = &hszName;
				static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
				m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop);

				iop.name = &hszName;
				static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
				m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop);
			}
			sop.name = &hszName;
			sop.archive = Archive;
			static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop);
		}
		

	}
	MemArchive->Close();
}

void CXMLGuiLoader::LoadGuiFileV11b(IXMLArchive *Archive, UINT mapIndex)
{
	IArchive *MemArchive;
	CHashString memType(_T("Memory"));
	CREATEARCHIVE ca;

	CREATEOBJECTPARAMS cop;
	INITOBJECTPARAMS iop;
	SERIALIZEOBJECTPARAMS sop;
	FINDOBJNAME fon;

	StdString szNodeName;
	StdString szName;
	
  	CHashString hszName(_T(""));
  	CHashString hszTypeName(_T(""));
    
	// create archive
	ca.mode = STREAM_MODE_READ | STREAM_MODE_WRITE;
	ca.streamType = &memType;
	ca.streamData = NULL;
	ca.streamSize = 0;
	static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE),&ca) != MSG_HANDLED)
	{
		return;
	}
	MemArchive = ca.archive;

  	while (Archive->GetNode(szNodeName))
	{
		// serialize differently for only CGUIEditText
		if( _tcscmp(szNodeName, _T("CGUIEditText")) == 0 )
		{
			hszTypeName = szNodeName.c_str();

			Archive->Read( szName, _T("Name") );
			hszName = szName.c_str();

			fon.searchName = &hszName;
			fon.compType = &hszTypeName;
			fon.bFound = false;

			// first check if object already exists in EE
			static DWORD msgHash_FindGUIObject = CHashString(_T("FindGUIObject")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_FindGUIObject, sizeof(FINDOBJNAME), &fon);
			if( !fon.bFound )
			{

				cop.parentName = NULL;
				cop.typeName = &hszTypeName;
				cop.name = &hszName;
				static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
				m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop);

				iop.name = &hszName;
				static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
				m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop);
			}

			// old data
			float fPosX;
			float fPosY;
			float fSizeX;
			float fSizeY;
			int	iZOrder;
			StdString normTex;
			StdString highTex;
			StdString selTex;
			StdString grayTex;
			StdString editText;
			int iMaxChar;
			StdString fontName;
			int iFontColor;
			int iCursorColor;
			int iTextOffSetX;
			int iTextOffSetY;
			bool bCenterText = false;

			// new data
			StdString allowChar = _T("data/AllChars.txt");

			// read data				
			Archive->Read(fPosX);
			Archive->Read(fPosY);
			Archive->Read(fSizeX);
			Archive->Read(fSizeY);
			Archive->Read(iZOrder);
			Archive->Read(normTex);
			Archive->Read(editText);
			Archive->Read(iMaxChar);
			Archive->Read(fontName);
			Archive->Read(iFontColor);
			Archive->Read(iCursorColor);
			Archive->Read(iTextOffSetX);
			Archive->Read(iTextOffSetY);
			Archive->Read(bCenterText);

			// start at the begining
			MemArchive->SetIsWriting(true);
			MemArchive->Write(fPosX);
			MemArchive->Write(fPosY);
			MemArchive->Write(fSizeX);
			MemArchive->Write(fSizeY);
			MemArchive->Write(iZOrder);
			MemArchive->Write(normTex);
			MemArchive->Write(editText);
			MemArchive->Write(iMaxChar);
			MemArchive->Write(fontName);
			MemArchive->Write(iFontColor);
			MemArchive->Write(iCursorColor);
			MemArchive->Write(iTextOffSetX);
			MemArchive->Write(iTextOffSetY);
			MemArchive->Write(bCenterText);
			MemArchive->Write(allowChar);

			MemArchive->SeekTo(0, SEEK_SET);
			MemArchive->SetIsWriting(false);
			sop.name = &hszName;
			sop.archive = MemArchive;
			static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop);
		}
		else
		{
			// serialize everything else the same as before
			hszTypeName = szNodeName.c_str();

			Archive->Read( szName, _T("Name") );
			hszName = szName.c_str();

			fon.searchName = &hszName;
			fon.compType = &hszTypeName;
			fon.bFound = false;

			// first check if object already exists in EE
			static DWORD msgHash_FindGUIObject = CHashString(_T("FindGUIObject")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_FindGUIObject, sizeof(FINDOBJNAME), &fon);
			if( !fon.bFound )
			{

				cop.parentName = NULL;
				cop.typeName = &hszTypeName;
				cop.name = &hszName;
				static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
				m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop);

				iop.name = &hszName;
				static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
				m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop);
			}
			sop.name = &hszName;
			sop.archive = Archive;
			static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop);
		}
	}
	MemArchive->Close();
}

DWORD CXMLGuiLoader::OnGetFileResolution(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(GUIWORKINGRES));
	GUIWORKINGRES *gwr = (GUIWORKINGRES*) param;
	gwr->iSizeX = m_iXRes;
	gwr->iSizeY = m_iYRes;
	return MSG_HANDLED_STOP;
}

DWORD CXMLGuiLoader::OnGetPageList(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(GETPAGELIST));
	GETPAGELIST *getPL = (GETPAGELIST*) param;
	
	IHashString *fileName = getPL->fileName;
	if (fileName)
	{
		StdString szFileName = fileName->GetString();
		szFileName.MakeSafeFileName();
		CHashString hsSafeFileName(szFileName);
		GPLMAP::iterator mapIter = m_mGplMap.find(hsSafeFileName.GetUniqueID());
		if (mapIter == m_mGplMap.end())
		{
			m_ToolBox->Log(LOGERROR, _T("GUI Loader: File has not been loaded!"));
			return MSG_ERROR;
		}
		getPL->gpl = mapIter->second;
	}
	return MSG_HANDLED_STOP;
}
