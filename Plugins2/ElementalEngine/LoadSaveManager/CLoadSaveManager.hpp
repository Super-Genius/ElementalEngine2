///============================================================================
/// \file		CLoadSaveManager.hpp
/// \brief		Header file for Load/Save Manager
/// \date		4-20-2005
/// \author		Kenneth Hurley
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

#ifndef _CLOADSAVEMANAGER_H_
#define _CLOADSAVEMANAGER_H_

#ifndef	_ICOMPONENT_H
#include "IComponent.h"
#endif

class CLoadSaveManager : public IComponent
{
private:
	SINGLETONCONSTRUCTOROVERRIDE(CLoadSaveManager);
	CLoadSaveManager();

	/// Searches through registered loaders/savers strings
	///	to find a match for extension
	/// \param ext = extension to search for
	/// \param isLoad = boolean TRUE for import FALSE for export
	/// \return pointer to the IComponent name, if found
	IHashString *FindExtensionMatch(LPCTSTR ext, BOOL isLoad);
	/// Load or Save a file by extension
	/// \param filePathName = full path/filename of file to load/save
	/// \param isLoad = boolean whether loading or not
	/// \param isInternal = is the load/save internal?
	/// \return MSG_XXX return value
	DWORD LoadSaveFile(LPCTSTR filePathName, bool isLoad, bool isInternal = true);
	/// UnLoads a file by extension
	/// \param filePathName = full path/filename of file to load/save
	/// \return MSG_XXX return value
	DWORD UnloadFile(LPCTSTR filePathName);

public:
	~CLoadSaveManager();

	/// standard static component create function
	static IComponent *Create(int nArgs, va_list argptr);

	void Serialize(IArchive &ar) {};

	IHashString *GetComponentType() { return &m_ComponentType; }

	bool IsKindOf(IHashString *compType)
	{
		return ( compType->GetUniqueID() == GetComponentType()->GetUniqueID() );	
	}

	// ------------------ Messages ------------------ //
	/// message to load a file
	DWORD OnLoadFile(DWORD size, void *params);
	/// message to save a file
	DWORD OnSaveFile(DWORD size, void *params);
	/// message to unload a file
	DWORD OnUnloadFile(DWORD size, void *params);
	/// message to register a loader/saver
	DWORD OnRegisterLoadSave(DWORD size, void *params);
	/// messaget to get all the filter strings
	DWORD OnGetFilterStrings(DWORD size, void *params);

	/// Gets the version of the currently loading file
	/// \param size = sizeof(IHashString)
	/// \param param = IHashString to return version
	/// \return MSG_HANDLED_STOP
	DWORD OnGetFileVersion(DWORD size, void *param);

	/// Sets the version of the currently loading file
	/// \param size = sizeof(IHashString)
	/// \param param = IHashString to set version
	/// \return MSG_HANDLED_STOP
	DWORD OnSetFileVersion(DWORD size, void *param);

private:
	CHashString m_ComponentType;
	IToolBox *m_ToolBox;
	LOADSAVEMAP m_LoadSaveMap;
	FILTERSTRINGSLIST m_FilterStrings;

	/// file version to get/set
	CHashString m_hszFileVersion;

	/// static component type used in FindExtensionMatch
	CHashString m_hsExtComponentType;

	/// default resource path - recieved from the environment
	StdString m_strDefaultResourcePath;
};

#endif	// #ifndef _CLOADSAVEMANAGER_H_
