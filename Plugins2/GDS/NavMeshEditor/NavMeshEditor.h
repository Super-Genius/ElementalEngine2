///============================================================================
/// \file		NavMeshEditor.h
/// \brief		Declaration of NavMesh Editor
/// \date		09-07-2005
/// \author		Brian Bazyk
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

#include "stdafx.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#undef AFX_DATA
#define AFX_DATA AFX_DATA_IMPORT

//class CNavMeshToolPal;
class CNavMeshEditor : public IDLL
{
	SINGLETONCONSTRUCTOROVERRIDE(CNavMeshEditor);
	CNavMeshEditor();
	
// implementation
public:   
	~CNavMeshEditor();
    virtual CString *GetName();
	virtual DWORD GetPriority();
    virtual BOOL Initialize();

	void OnCreate(CMDIFrameWnd *mainWnd);

	BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra,
		AFX_CMDHANDLERINFO* pHandlerInfo);

	void OnGlobalUpdateTick( void );
//	void ShowNavMeshDialog( bool bShow );

	
// attributes
private:
	IToolBox *m_ToolBox;
	HMENU m_hMenu;
	HACCEL m_hAccel;

	// just needs to be be here to be instianted so it registers
	// it's menu handlers.
	CNavMeshCmdTarget m_NavMeshCmdTarget;
};

extern AFX_EXTENSION_MODULE NavMeshEditorDLL;

#undef AFX_DATA
#define AFX_DATA