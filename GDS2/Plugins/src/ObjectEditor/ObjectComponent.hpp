///============================================================================
/// \file		ObjectComponent.hpp
/// \brief		Header file for <insert brief description here>
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

#ifndef _OBJECTCOMPONENT_HPP_
#define _OBJECTCOMPONENT_HPP_

class CObjectEditor;

/// class so we can hook into messaging system of Elemental Engine
class CObjectComponent : public IComponent
{
private:
	SINGLETONCONSTRUCTOROVERRIDE(CObjectComponent);
	CObjectComponent();
public:
	virtual ~CObjectComponent();

	void SetParent(CObjectEditor *parent);
	static IComponent *Create(int nArgs, va_list argptr);
	void Serialize(IArchive &ar);
	IHashString *GetComponentType();
	bool IsKindOf(IHashString *compType);

	// ----------- Messages Functions ---------------//
	DWORD OnCmdMsg(DWORD size, void *param);
	DWORD OnCreate(DWORD size, void *param);
	DWORD OnEditObject(DWORD size, void *param);
	DWORD OnLeftClickEditorObject(DWORD size, void *param);
	DWORD OnLeftClickObject(DWORD size, void *param);
	DWORD OnChangeObjectName(DWORD size, void *param);
	DWORD OnMouseMove(DWORD size, void *param);
	DWORD OnItemDropped(DWORD size, void *param);
	/// message handler when global update tick called
	/// \param size = size of GLOBALTICKPARAMS struct
	/// \param param = pointer to GLOBALTICKPARAMS
	DWORD OnGlobalUpdateTick(DWORD size, void *param);
	DWORD OnGetObjectGridToolData(DWORD size, void *param);
	DWORD OnEditorMoveObject(DWORD size, void *param);
	DWORD OnSelectObject(DWORD size, void *param);
	DWORD OnEditorGetObjectToolConfig(DWORD size, void *param);
	DWORD OnEditorSetObjectToolConfig(DWORD size, void *param);

private:
	CObjectEditor *m_Parent;
	DWORD m_LastObjectID;
	DWORD m_LastCompTypeID;
	EDITABLELIST m_EditableObjects;
	IToolBox *m_ToolBox;
};

#endif // #ifndef _OBJECTCOMPONENT_HPP_
