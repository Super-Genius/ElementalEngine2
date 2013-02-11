///=====================================================================
/// \file	EditorHelpers.cpp
/// \brief	Implementation of helper functions for editors
/// \date	11/27/2007
/// \author	Andrey Ivanov
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
///=====================================================================

#include "StdAfx.h"

#define INI_FILEPATH _T("\\basedata\\gds.ini")

namespace GDS_SDK_Helpers
{

///	\brief	set active scene name
///	\param	sceneName - pointer to IHashString with name of scene.
///						 empty name is set if NULL is passed
///	\return	EE return code
DWORD SetActiveScene(IHashString *sceneName)
{
	if (sceneName == NULL)
	{
		static CHashString hszEmpty(_T(""));
		sceneName = &hszEmpty;
	}
	GETSETACTIVESCENE gsas;
	gsas.pSceneName = sceneName;
	static DWORD msgHash_SetActiveScene = CHashString(_T("SetActiveScene")).GetUniqueID();
	return EngineGetToolBox()->SendMessage(msgHash_SetActiveScene, sizeof(gsas), &gsas);
}

///	\brief	get active scene name
///	\return	active scene name
CHashString GetActiveScene()
{
	GETSETACTIVESCENE gsas;
	gsas.pSceneName = NULL;
	static DWORD msgHash_GetActiveScene = CHashString(_T("GetActiveScene")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_GetActiveScene, sizeof(gsas), &gsas);
	return gsas.pSceneName;
}


///	\brief	find opened document in doc template document list
///			and activate first view for this document
///	\param	pDocTemplate - pointer to CDocTemplate for iterating document
///	\param	szFilename - filename to find
///	\return	true if document was found
bool ActivateOpenedDocument(CDocTemplate *pDocTemplate, LPCTSTR szFilename)
{
	POSITION posDoc = pDocTemplate->GetFirstDocPosition();
	while (posDoc != NULL)
	{
		CDocument *pDoc = pDocTemplate->GetNextDoc(posDoc);
		if (0 == pDoc->GetPathName().CompareNoCase(szFilename))
		{
			POSITION posView = pDoc->GetFirstViewPosition();
			if (posView != NULL)
			{
				CView *pView = pDoc->GetNextView(posView);
				ASSERT(pView != NULL);
				static DWORD msgHash_SetActiveView = CHashString(_T("SetActiveView")).GetUniqueID();
				EngineGetToolBox()->SendMessage(msgHash_SetActiveView, sizeof(pView), pView);
				return true;
			}
			return false;
		}
	}
	return false;
}

///	\brief	load resource string
///	\param	nResourceId - id of resource string
///	\return	string with loaded resource
CString FormatString(UINT nResourceId, ...)
{
	CString fmt;
	VERIFY(fmt.LoadString(nResourceId));

	CString res;
	va_list args;
	va_start(args, nResourceId);
	res.FormatV(fmt, args);
	va_end(args);

	return res;
}

///	\brief	get last error code and format message
///	\return	string with error message
CString GetLastErrorMessage()
{
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | 
				  FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), 
				  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
				  (LPTSTR) &lpMsgBuf, 0, NULL);
	CString res = (LPCTSTR)lpMsgBuf;
	// Free the buffer.
	LocalFree(lpMsgBuf);
	return res;
}

void ReadINIKey(StdString key, StdString &value, StdString subCategory, StdString defaultVal)
{
	StdString fullPath, resourceDir;
	EngineGetToolBox()->GetDirectories(&fullPath, &resourceDir);

	if (_tcscmp(fullPath, _T("")) == 0)
	{
		TCHAR buf[_MAX_PATH];
		GetCurrentDirectory(_MAX_PATH, buf);
		fullPath = buf;
	}
	
	fullPath += INI_FILEPATH;
	
	TCHAR buf[1024];
	DWORD result = GetPrivateProfileString(subCategory, key, _T(""), buf, 1024, fullPath);
	if (result == 0)
	{
		// read failed, missing key? write it
		WriteINIKey(key, defaultVal, subCategory);
		value = defaultVal;
	}
	else
	{
		value = buf;
	}
}

void WriteINIKey(StdString key, StdString value, StdString subCategory)
{
	StdString fullPath, resourceDir;
	EngineGetToolBox()->GetDirectories(&fullPath, &resourceDir);
	
	if (_tcscmp(fullPath, _T("")) == 0)
	{
		TCHAR buf[_MAX_PATH];
		GetCurrentDirectory(_MAX_PATH, buf);
		fullPath = buf;
	}
	
	fullPath += INI_FILEPATH;
	WritePrivateProfileString(subCategory, key, value, fullPath);	
}

/////////////////////////// CFileVersionSetter members /////////////////////////
/// \brief	This constructor sets latest file version
CFileVersionSetter::CFileVersionSetter()
{
	IToolBox *pToolbox = EngineGetToolBox();
	// get latest version
	float fLatestVersion = 0;
	static DWORD msgHash_GetLatestVersion = CHashString(_T("GetLatestVersion")).GetUniqueID();
	DWORD retval = pToolbox->SendMessage(msgHash_GetLatestVersion, sizeof(&fLatestVersion), &fLatestVersion);
	if (retval != MSG_HANDLED)
	{
		pToolbox->Log(LOGERROR, _T("%s(%i): Could not get latest version."), __FILE__, __LINE__);
		throw std::exception("Could not get latest version");
	}
	TCHAR szLatestVersion[64];
	sprintf(szLatestVersion, _T("%f"), fLatestVersion);
	CHashString latestVersion(szLatestVersion);
	Init(&latestVersion);
}

/// \brief	This constructor sets specific file version
/// \param	szFileVersion - file version string
CFileVersionSetter::CFileVersionSetter(LPCTSTR szFileVersion)
{
	CHashString hszVersion = szFileVersion;
	Init(&hszVersion);
}

/// \brief	This constructor sets specific file version
/// \param	pVersion - ptr to the IHashString version
CFileVersionSetter::CFileVersionSetter(IHashString *pFileVersion)
{
	Init(pFileVersion);
}

CFileVersionSetter::~CFileVersionSetter()
{
	Apply(&m_hszPreviousFileVersion);
}

/// \brief	Set passed value as file version
/// \param	pVersion - ptr to the IHashString version
void CFileVersionSetter::Apply(IHashString *pVersion)
{
	IToolBox *pToolbox = EngineGetToolBox();
	static DWORD msgHash_GetFileVersion = CHashString(_T("SetFileVersion")).GetUniqueID();
	if (MSG_HANDLED != pToolbox->SendMessage(msgHash_GetFileVersion, sizeof(pVersion), pVersion))
	{
		pToolbox->Log(LOGERROR, _T("%s(%i): Could not set file version."), __FILE__, __LINE__);
		throw std::exception("Could not set file version");
	}
}

/// \brief	does initial file version setting
///			it saves current file version in m_hszPreviousFileVersion
/// \param	pVersion - ptr to the IHashString version
void CFileVersionSetter::Init(IHashString *pVersion)
{
	IToolBox *pToolbox = EngineGetToolBox();
	static DWORD msgHash_GetFileVersion = CHashString(_T("GetFileVersion")).GetUniqueID();
	if (MSG_HANDLED != pToolbox->SendMessage(msgHash_GetFileVersion, sizeof(&m_hszPreviousFileVersion), &m_hszPreviousFileVersion))
	{
		pToolbox->Log(LOGERROR, _T("%s(%i): Could not get file version."), __FILE__, __LINE__);
	}
	// set desired version
	Apply(pVersion);
}

// \brief	private member for protecting from copying
CFileVersionSetter::CFileVersionSetter(const CFileVersionSetter&)
{
}

// \brief	private member for protecting from copying
void CFileVersionSetter::operator=(const CFileVersionSetter&)
{
}

}; // namespace GDS_SDK_Helpers
