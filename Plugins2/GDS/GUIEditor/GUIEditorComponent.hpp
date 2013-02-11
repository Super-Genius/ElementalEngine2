///============================================================================
/// \file		GUIEditorComponent.hpp
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

#ifndef _GUIEDITORCOMPONENT_HPP_
#define _GUIEDITORCOMPONENT_HPP_

class CGUIEditor;
class CGUIEditorView;
class CGUIEditorFrm;

/// class so we can hook into messaging system of Elemental Engine
class CGUIEditorComponent : public IComponent
{
	SINGLETON(CGUIEditorComponent);
private:
	CGUIEditor *m_Parent;
	CGUIEditorView *m_View;
public:
	void SetParent(CGUIEditor *parent);
	static IComponent *Create(int nArgs, va_list argptr);
	void Serialize(IArchive &ar);
	IHashString *GetComponentType();
	bool IsKindOf(IHashString *compType);

	// ----------- Messages Functions ---------------//
	DWORD OnCmdMsg(DWORD size, void *param);
	DWORD OnCreate(DWORD size, void *param);
	DWORD OnEditGUIEditor(DWORD size, void *param);			
	DWORD OnFileLoadedGXL(DWORD size, void *param);
	DWORD OnGUIEditorDocOpen(DWORD size, void *param);
	DWORD OnGUIEditorDocClose(DWORD size, void *param);
	DWORD OnUpdateGUIAnimBarPos(DWORD size, void *param);

};

#endif // #ifndef _GUIEditorCOMPONENT_HPP_
