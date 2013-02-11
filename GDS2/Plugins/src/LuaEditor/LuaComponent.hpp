///============================================================================
/// \file		LuaComponent.h
/// \brief		Header file for the Lua Component 
/// \date		06-17-2005
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

#ifndef _LUACOMPONENT_HPP_
#define _LUACOMPONENT_HPP_

class CLuaEditor;

typedef struct 
{
	TCHAR scriptFileName[_MAX_PATH];
	BOOL bAutoStart;
} SCRIPTOBJECTDATA;

/// class so we can hook into messaging system of Elemental Engine
class CLuaComponent : public IComponent
{
private:
	SINGLETONCONSTRUCTOROVERRIDE(CLuaComponent);
	CLuaComponent();
public:
	~CLuaComponent();
	/// Set the parent of the component
	/// \param parent - parent you want to set
	/// \return void
	void SetParent(CLuaEditor *parent);

	/// Create a Lua Component
	/// \param argptr - argument list
	/// \return the created component
	static IComponent *Create(int nArgs, va_list argptr);

	/// Serialize the lua component
	/// \param ar - archive
	/// \return void
	void Serialize(IArchive &ar);

	/// Get the component type
	/// \param void
	/// \return IHashString which is the component type
	IHashString *GetComponentType();

	/// Find out if component is a kind of another component
	/// \param compType - component type to compare
	/// \return true if it is, false if not
	bool IsKindOf(IHashString *compType);

	// ----------- Messages Functions ---------------//
	DWORD OnCmdMsg(DWORD size, void *param);
	DWORD OnCreate(DWORD size, void *param);
	DWORD OnScriptLoad(DWORD size, void *param);
	DWORD OnGetDataSize(DWORD size, void *param);
	DWORD OnGetEditorData(DWORD size, void *param);
private:
	CLuaEditor *m_Parent;
	EDITABLELIST m_EditableObjects;
	IToolBox *m_ToolBox;

};

#endif // #ifndef _LUACOMPONENT_HPP_
