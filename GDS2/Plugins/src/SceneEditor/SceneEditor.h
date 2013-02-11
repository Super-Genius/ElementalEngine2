///============================================================================
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

#if !defined(AFX_SceneEditor_H__80A18743_A0D3_4593_AADE_3EB5BA220CC5__INCLUDED_)
#define AFX_SceneEditor_H__80A18743_A0D3_4593_AADE_3EB5BA220CC5__INCLUDED_

// SceneEditor.cpp : implementation file
//

#include "stdafx.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SceneEditor.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSceneEditor document

class CSceneEditor : public IDLL
{
	SINGLETONCONSTRUCTOROVERRIDE(CSceneEditor);
	CSceneEditor();

// implementation
public:
	~CSceneEditor();
    CString *GetName();
    BOOL Initialize();
	DWORD GetPriority();
	static void CreateScene(void *userData = NULL);

	void OnCreate(CMDIFrameWnd *mainWnd);
	BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra,
		AFX_CMDHANDLERINFO* pHandlerInfo);

	///	\brief	Set file name for opened scene document
	///	\param	pFilename - string with new filename
	void SetWorldFileName(LPCTSTR pFilename)
	{
		m_SceneDLDockWin.SetWorldFileName(pFilename);
	}

	///	\brief	updated object label if it has been changed
	///	\param	name - name of initialized object
	void OnInitObject(IHashString *name)
	{
		m_SceneDLDockWin.OnInitObject(name);
	}

	void OnUpdateTick( DWORD tickCount )
	{
		if ( m_pCameraDockWin != NULL && m_pCameraDockWin->m_CameraDialog != NULL )
		{
			m_pCameraDockWin->m_CameraDialog->UpdateTick();
		}
	}

// attributes
private:
	//CMultiDocTemplate *m_DocTemplate;
	IToolBox *m_ToolBox;

	HMENU m_hMenu;		// our local menu
	HACCEL m_hAccel;	// our local accelerator table
	CSceneDLDockWin m_SceneDLDockWin;
	CCameraDockWin *m_pCameraDockWin;
};

extern AFX_EXTENSION_MODULE SceneEditorDLL;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SceneEditor_H__80A18743_A0D3_4593_AADE_3EB5BA220CC5__INCLUDED_)
