///============================================================================
/// \file		GDSComponent.hpp
/// \brief		Header file for Component link from EE to MFC Extension app
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

#ifndef _GDSCOMPONENT_HPP_
#define _GDSCOMPONENT_HPP_

class CGDSApp;
class CMainFrame;
/// class so we can hook into messaging system of Elemental Engine
class CGDSComponent : public IComponent
{
private:
	CGDSApp *m_Parent;

	// logging file
	FILE *m_LogFile;
	IToolBox* m_ToolBox;

public:
	void SetParent(CGDSApp *parent);
	static IComponent *Create(int nArgs, va_list argptr);
	void Serialize(IArchive &ar);
	IHashString *GetComponentType();
	bool IsKindOf(IHashString *compType);

	static CMainFrame *GetMainFrame();

	CGDSComponent();

	// ----------- Messages Functions ---------------//
	DWORD OnGetMenuInfo(DWORD size, void *param);
	DWORD OnSetMenuInfo(DWORD size, void *param);
	DWORD OnRegisterFileTypeCreationInfo(DWORD size, void *param);
	/// Add a tool bar to the main window
	/// \param size = sizeof ADDTOOLBARSTRUCT
	/// \param param = pointer to ADDTOOLBARSTRUCT
	DWORD OnAddToolBar(DWORD size, void *param);
	/// Checks if a tool bar is active
	/// \param size = sizeof TOOLBARACTIVESTRUCT
	/// \param param = pointer to TOOLBARACTIVESTRUCT
	DWORD OnIsToolBarActive( DWORD size, void *param );
	/// Sets a tool bar to active
	/// \param size
	/// \param param - pointer to TOOLBARACTIVESTRUCT
	DWORD OnSetActiveBar( DWORD size, void *param );
	/// Hides a tool bar
	/// \param size
	/// \param param - pointer to CControlBar
	DWORD OnHideBar( DWORD size, void *param );

	/// Update the progress bar
	/// \param pointer to a DWORD
	DWORD OnLoadingUpdate(DWORD size, void *param);
	/// Gets the current working directory of GDS,
	/// \param param - pointer to IHashString
	DWORD OnGetResourceDirectory(DWORD size, void *param);
	/// Get currently selected view
	/// \param param - address of pointer to a CView
	DWORD CGDSComponent::OnGetActiveView(DWORD size, void *param);
	/// Set currently selected view
	/// \param param pointer to a CView
	DWORD CGDSComponent::OnSetActiveView(DWORD size, void *param);

	/// Catch log messages so we can print them to a file
	/// \param param - pointer to LOGMESSAGE struct pointer
	DWORD OnLogMessage(DWORD size, void *param);
};

#endif // #ifndef _GDSCOMPONENT_HPP_
