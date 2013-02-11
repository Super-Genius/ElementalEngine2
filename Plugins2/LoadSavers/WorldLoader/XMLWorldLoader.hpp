///============================================================================
/// \file		XMLWorldLoader.hpp
/// \brief		Header file for XML based World Loader
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

#ifndef _XMLWORLDLOADER_H_
#define _XMLWORLDLOADER_H_

#include "ILoadSave.hpp"

const float g_fWorldVersion = 3.0f;

/* *** World Version History ***
 1.0 - Initial Version
 1.5 - Changed Position and Scale for Objects and Terrain (1000/898).
       Thus CV3ORenderObject, CPhysicsObject, and CTerrainConfig were affected
 2.0 - Added position, rotation and scale to CEntity and removing it from
	   CPhysicsObject and CRenderObject and others
 2.5 - All Positional, Rotational, and Scale data is now part of the CEntity Object
 3.0 - Changed Axis to be Left-Handed X-Right, Y-Up, Z-In
*/

class CXMLWorldLoader : public IComponent
{
private:
	SINGLETONCONSTRUCTOROVERRIDE(CXMLWorldLoader);
	CXMLWorldLoader();
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
	/// message to load a world file
	DWORD OnLoadWorldFile(DWORD size, void *params);
	/// message to save a world file
	DWORD OnSaveWorldFile(DWORD size, void *params);
	/// message to get the world version
	DWORD OnGetWorldVersion(DWORD size, void *params);
	// message to get the current latest version (ie. the version a world being saved out will be)
	DWORD OnGetLatestVersion(DWORD size, void *params);

private:
	static CHashString m_HashName;
	IToolBox *m_ToolBox;
	static LSRC_MESSAGE m_LoadSaveRegisterData;
	float m_WorldFileVersion;
	float m_fLoadingUpdateFPS;
};

#endif	// #ifndef _XMLWORLDLOADER_H_
