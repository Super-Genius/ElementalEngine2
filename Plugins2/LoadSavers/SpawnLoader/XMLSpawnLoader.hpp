///============================================================================
/// \file		XMLSpawnLoader.hpp
/// \brief		Header file for XML based Spawn Loader
/// \date		08-12-2005
/// \author		Josh Lulewicz
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

#ifndef XMLSPAWNLOADER_H
#define XMLSPAWNLOADER_H

#include "ILoadSave.hpp"

// filenames to archives
typedef std::map<DWORD, IXMLArchive*> SPAWNMAP;
// actual name in file to generated name
typedef std::map<DWORD, CHashString> PARENTNAMEMAP;

class CXMLSpawnLoader : public IComponent
{
private:
	SINGLETONCONSTRUCTOROVERRIDE(CXMLSpawnLoader);
	CXMLSpawnLoader();
public:
	~CXMLSpawnLoader();

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
	/// message to load a world file
	DWORD OnLoadSpawnFile(DWORD size, void *params);
	/// message to save a world file
	DWORD OnSaveSpawnFile(DWORD size, void *params);
	/// message to remove spawn file that's loaded
	DWORD OnRemoveSpawnFile(DWORD size, void *params);
	/// message to get the name of the last loaded item
	DWORD OnGetSpawnedName(DWORD size, void *params);
	/// message to set the parent name of the next loaded item
	DWORD OnSetSpawnParentName(DWORD size, void *params);
	/// message to set the position of the next spawned item
	DWORD OnSetSpawnPosition(DWORD size, void *params);
	/// message to set the euler of the next spawned item
	DWORD OnSetSpawnEuler(DWORD size, void *params);
	/// message to load spawner from build in from memory archive
	DWORD OnSpawnFromXMLArchive(DWORD size, void *params);

	DWORD OnCacheSpawnFile(DWORD size, void *param);

private:
	static CHashString m_HashName;
	IToolBox *m_ToolBox;
	static LSRC_MESSAGE m_LoadSaveRegisterData;

	/// map of spawn file names to archive pointers
	SPAWNMAP m_SpawnFilesLoadedMap;
	/// name of the last spawned entity
	CHashString m_LastSpawnedName;
	/// name of the object currently being inited
	CHashString m_InitName;
	/// bool if we are in process of initializing the object
	bool m_bIntializingObject;

	/// name of the parent for the next spawned entity
	CHashString m_NextSpawnParentName;
	/// is the next spawn parent name set
	bool m_bParentNameSet;	
	
	Vec3 m_vSpawnedPosition;
	EulerAngle m_eSpawnedEuler;
};

#endif	// #ifndef _XMLSPAWNLOADER_H_
