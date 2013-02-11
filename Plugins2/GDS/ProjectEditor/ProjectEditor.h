///============================================================================
// ProjectEditor.h : header file
//
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

#if !defined(AFX_PROJECTEDITOR_H_INCLUDED_)
#define AFX_PROJECTEDITOR_H_INCLUDED_

#include "stdafx.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#undef AFX_DATA
#define AFX_DATA AFX_DATA_IMPORT

/////////////////////////////////////////////////////////////////////////////
// CProjectEditor DLL singleton class

class CProjectEditor : public IDLL
{
	SINGLETONCONSTRUCTOROVERRIDE(CProjectEditor);
	CProjectEditor();
	// attributes
private:
	HMENU m_hMenuTools;
	HMENU m_hMenuFile;
	HMENU m_hMenuFileCloseProject;
	HACCEL m_hAccel;

public:
	~CProjectEditor();

    virtual CString *GetName();

    virtual BOOL Initialize();
	DWORD GetPriority();

	// Simply relays this call to projectEditorComponent
	DWORD DisplayProjectSettingsDialog(bool allowNew);

	void OnCreate(CMDIFrameWnd *mainWnd);
	BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

	/// Adds a new property page to the sheet
	DWORD CProjectEditor::AddProjectSettingsPropertyPage( IProjectSettingsPropertyPage* p_newPage );

	void Serialize(IArchive &ar);

	///Reads the project settings stored in the ini file
	BOOL AutoLoadProjectSettings();

//	CTextureEditorCmdTarget m_TextureEditorCmdTarget;

// attributes
protected:
	IToolBox *m_ToolBox;

	/// the property sheet of the project settings
	CProjectEditorPropertySheet* m_projectEditorPropertySheet;

	/// the property page of the project settings
	CProjectEditorDefaultPropertyPage* m_projectEditorBasePropertyPage;

	/// pointer to the singleton instance of the project settings
	CProjectSettings* m_projectSettings; 

	/// the list of components, which added propety pages to our sheet.
	/// it is used for serializing their date.
	vector<IComponent*> * registeredComponents;

};

extern AFX_EXTENSION_MODULE ProjectEditorDLL;

#undef AFX_DATA
#define AFX_DATA

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROJECTEDITOR_H_INCLUDED_)
