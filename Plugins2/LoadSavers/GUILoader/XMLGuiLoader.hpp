///============================================================================
/// \file		XMLGuiLoader.hpp
/// \brief		Header file for XML based Gui Loader
/// \date		04-04-2005
/// \author		Kenneth Hurley
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

#ifndef _XMGUILOADER_H_
#define _XMLGHILOADER_H_

#include "ILoadSave.hpp"
#include "IXMLArchive.h"

class CXMLGuiLoader;

typedef void (CXMLGuiLoader::*LOADGUIFUNC)(IXMLArchive *Archive, UINT mapIndex);
typedef std::map<StdString, LOADGUIFUNC> VERSTOLOADGUIFUNCMAP;

typedef struct GROUPDATA
{
	StdString groupName;
	StdString pageName;

} GROUPDATA;

typedef std::list<GROUPDATA> GROUPNAMELIST;

typedef map<DWORD, GUIPAGELIST> GPLMAP;

class CXMLGuiLoader : public IComponent
{
private:
	SINGLETONCONSTRUCTOROVERRIDE(CXMLGuiLoader);
	CXMLGuiLoader();
public:

	/// We are the serializer, so we don't do anything with this.
	virtual void Serialize(IArchive &ar) {};

	/// return a string of the component type
	/// \return IHashString *reference to textual classname
	virtual IHashString *GetComponentType();

	/// Checks if this is similar to it's base classes component
	/// \return true if it is simlar, else false
	virtual bool IsKindOf(IHashString *compType);

	/// standard static component create function
	static IComponent *Create(int nArgs, va_list argptr);

	// ------------------ Messages ------------------ //
	/// \brief	message to load a Gui file
	/// Load a file if we recognize the ext type (.xgl).
	/// Currently implemented with two passes:
	/// 1st pass. Creates and adds all gui elements, groups, and pages.
	///			  Will skip pages w/ nodes "PAGEBUFFER"
	/// 2nd pass. Goes through only pages w/ node "PAGEBUFFER"
	///			  Creates the "PAGEBUFFER" pages and adds the guielements to it
	///
	/// note: The 2nd pass is only for the gui editor so that the user can
	///		  edit groups.
	/// \param	size - size of TCHAR *
	/// \param	*params - pointer to the .xgl file to load
	/// \returns MSG_HANDLED_STOP on sucess, else MSG_ERROR
	DWORD OnLoadGuiFile(DWORD size, void *params);

	/// \brief message to save a Gui file
	/// Function only used int GUI Editor v. 95
	/// \param	size - size of TCHAR *
	/// \param	*params - pointer to the .xgl file to save to.
	/// \returns MSG_HANDLED_STOP on sucess, else MSG_ERROR
	DWORD OnSaveGuiFile(DWORD size, void *params);

	/// gets the loaded gui version
	/// \param size - size of IHashString
	/// \param params - pointer to IHashString to return version
	/// \return MSG_HANDLED_STOP on success, MSG_ERROR on failure
	DWORD OnGetGUIVersion(DWORD size, void *params);

	/// sets the gui version for saveout
	/// \param size - size of CHashString
	/// \param params - pointer to CHashString to set version
	/// \return MSG_HANDLED_STOP on success, MSG_ERROR on failure
	DWORD OnSetGUIVersion(DWORD size, void *params);

	/// gets the loaded file resolution (1.1d+)
	/// \param size - sizeof(GUIWORKINGRES)
	/// \param param - GUIWORKINGRES of loaded resolution
	/// \return MSG_HANDLED_STOP on success
	DWORD OnGetFileResolution(DWORD size, void *param);

	/// Gets the page list given a filename
	/// \param size = sizeof(GETPAGELIST)
	/// \param param = GETPAGELIST with filename; returns gpl data
	/// \return MSG_HANDLED_STOP
	DWORD OnGetPageList(DWORD size, void *param);

	/// \brief Loads gui saved .gxl files for the latest GUI Editor vers.
	/// Currently, this is v 1.1c
	/// \param IXMLArchive* - ptr to the archive
	/// \param mapIndex = index of gui system that will be loaded into
	void LoadGuiFile(IXMLArchive *Archive, UINT mapIndex);

	/// \brief Loads gui saved .gxl files for the GUI Editor v1.1b
	/// \param IXMLArchive* - ptr to the archive
	void LoadGuiFileV11b(IXMLArchive *Archive, UINT mapIndex);
	

	/// \brief Loads gui saved .gxl files for the GUI Editor v1.1a
	/// \param IXMLArchive* - ptr to the archive
	void LoadGuiFileV11a(IXMLArchive *Archive, UINT mapIndex);

	/// \brief Loads gui saved .gxl files for GUI Editor v 1.0
	/// Converts all GUI elements from v1.0 to v1.1 so that they
	///	can be properly serialized.
	/// \param IXMLArchive* - ptr to the archive
	void LoadGuiFileV1(IXMLArchive *Archive);

	/// \brief Loads gui saved .gxl files for GUI Editor v .95 
	/// \param IXMLArchive* - ptr to the archive
	void LoadGuiFileV95(IXMLArchive *Archive);

  	/// \brief Creates the page 
	/// This is a function written for compatibility. 
	///	Function only used to load saved .gxl files from 
	///	GUI Editor v .95
  	/// \param ar - the archive to read from
  	void CreatePage(StdString strPageName, StdString strPageType);
  
  	/// brief Creates gui elements from an archive
	/// This is a function written for compatibility. 
	///	Function only used to load saved .gxl files from 
	///	GUI Editor v .95
  	/// \param ar - the archive to read from
  	void CreateGuiElement(StdString strGuiName, StdString strGuiType, IXMLArchive &Archive);

  	/// brief Creates gui groups from an archive
	/// This is a function written for compatibility. 
	///	Function only used to load saved .gxl files from 
	///	GUI Editor v .95
  	/// \param ar - the archive to read from
	void CreateGuiGroup(StdString strGroupName, StdString strGuiType, IXMLArchive &Archive);

private:
	static CHashString m_HashName;
	IToolBox *m_ToolBox;
	static LSRC_MESSAGE m_LoadSaveRegisterData;

	GROUPNAMELIST m_GroupDataList;

	VERSTOLOADGUIFUNCMAP m_VersToLoadGuiFuncMap;

	CHashString m_szVersionName;
	UINT m_iXRes;
	UINT m_iYRes;	

	GPLMAP m_mGplMap;

	CHashString m_CurrentFile;
};

#endif	// #ifndef _XMLGUILOADER_H_
