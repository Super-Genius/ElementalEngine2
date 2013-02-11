///============================================================================
/// \file		TextureComponent.hpp
/// \brief		Header file for Scene Component hooks into EE
/// \date		04-22-2005
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

#ifndef _TEXTURECOMPONENT_HPP_
#define _TEXTURECOMPONENT_HPP_

class CTextureEditor;

/// class so we can hook into messaging system of Elemental Engine
class CTextureComponent : public IComponent
{
	SINGLETONCONSTRUCTOROVERRIDE( CTextureComponent );

	/// \brief	Constructor
	///	Private since this class is using the singleton template
	CTextureComponent();

private:
	IToolBox *m_ToolBox;
	CTextureEditor *m_Parent;
	
public:
	/// destructor for component
	~CTextureComponent();

	void SetParent( CTextureEditor *parent );
	static IComponent *Create( int nArgs, va_list argptr );
	void Serialize( IArchive &ar );
	IHashString *GetComponentType();
	bool IsKindOf(IHashString *compType);

	// ----------- Messages Functions ---------------//
	/// Fast load texture for thumbnail generation.
	DWORD OnCreateTextureThumbnail(DWORD size, void *param);
	DWORD OnCmdMsg(DWORD size, void *param);
	DWORD OnCreate(DWORD size, void *param);
	DWORD OnLoadedTexture(DWORD size, void *param);
	DWORD OnGlobalUpdateTick(DWORD size, void *param);
};

#endif // #ifndef _TEXTURECOMPONENT_HPP_
