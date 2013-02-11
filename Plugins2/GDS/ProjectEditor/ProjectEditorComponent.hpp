///============================================================================
/// \file		ProjectEditorComponent.hpp
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

#ifndef _PROJECTEDITORCOMPONENT_HPP_
#define _PROJECTEDITORCOMPONENT_HPP_

class CProjectEditorEditor;

/// class so we can hook into messaging system of Elemental Engine
class CProjectEditorComponent : public IComponent
{
	SINGLETON(CProjectEditorComponent);
private:
	CProjectEditor *m_Parent;

public:

	void SetParent(CProjectEditor *parent);

	static IComponent *Create(int nArgs, va_list argptr);

	void Serialize(IArchive &ar);

	IHashString *GetComponentType();
	bool IsKindOf(IHashString *compType);

	// ----------- Messages Functions ---------------//
	DWORD OnCmdMsg(DWORD size, void *param);
	DWORD OnCreate(DWORD size, void *param);
	// message that a .gdproj file was loaded
	DWORD OnProjectFileLoaded(DWORD size, void *param);

	// Message handler. Adds new property page to the sheet
	afx_msg DWORD OnAddProjectSettingsPropertyPageMessage(DWORD size, void *param);
	
	DWORD DisplayProjectSettingsDialog(bool allowNew);


	/// gets the confirmation of project close and returns TRUE
	/// if confirmed
	afx_msg DWORD OnGdsClose(DWORD size, void *param);
};

#endif // #ifndef _PROJECTEDITORCOMPONENT_HPP_
