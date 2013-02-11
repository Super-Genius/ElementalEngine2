///============================================================================
/// \file		TextureLoader.hpp
/// \brief		Declaration file for texture loader
/// \date		06-28-2005
/// \author		D. Patrick Ghiocel
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

#ifndef _TEXTURELOADER_H_
#define _TEXTURELOADER_H_

#include "ILoadSave.hpp"

/// Map of DWORD Texture Names to DWORD IObjects
typedef map<DWORD, DWORD> TEXTURETOOBJECTMAP;

class CTextureLoader : public IComponent
{
private:
	SINGLETONCONSTRUCTOROVERRIDE(CTextureLoader);
	CTextureLoader();
public:
	~CTextureLoader();

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
	/// message to load a texture
	DWORD OnLoadTextureFile(DWORD size, void *params);
	/// message to save a texture
	DWORD OnSaveTextureFile(DWORD size, void *params);
	/// message to unload a texture
	DWORD OnUnloadTextureFile(DWORD size, void *params);
	/// message to set the save as format
	DWORD OnSetSaveAsFormat( DWORD size, void * params );
private:
	ILenum GetILFormat( IHashString * hash );
	static CHashString m_HashName;
	IToolBox *m_ToolBox;
	static LSRC_MESSAGE m_LoadSaveRegisterData;
	CHashString m_SaveAsFormat;
	TEXTURETOOBJECTMAP m_TextureMap;
};

#endif	// #ifndef _TEXTURELOADER_H_
