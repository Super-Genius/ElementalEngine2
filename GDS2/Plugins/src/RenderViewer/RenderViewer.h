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

#include "stdafx.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RenderViewer.h : header file
//
#undef AFX_DATA
#define AFX_DATA AFX_DATA_IMPORT

/////////////////////////////////////////////////////////////////////////////
// CRenderViewer document
#include "FullScreenFrame.h"


class CRenderViewer : public IDLL
{
	SINGLETONCONSTRUCTOROVERRIDE(CRenderViewer);
	CRenderViewer();
	
// implementation
public:   
	~CRenderViewer();
    virtual CString *GetName();
	virtual DWORD GetPriority();
    virtual BOOL Initialize();	
	void OnUpdateTick(DWORD deltaTicks);

	///	\brief	Returns the active RenderView
	CView *GetActiveView();
	///	\brief	Returns the active RenderView
	CRenderView* GetActiveRenderView();
	///	\brief	Returns the first created RenderView
	CRenderView *GetFirstView();

// message handlers
	void OnCreate(CMDIFrameWnd *mainWnd);
	BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra,
		AFX_CMDHANDLERINFO* pHandlerInfo);
	DWORD ViewDocument();

	/// \brief	View in fullscreen 
	///	\param	nXRes - the x resolution
	///	\param	nYRes - the y resolution
	void ViewFullScreen(UINT nXRes, UINT nYRes);

	void SetCameraMoveSensitivity( float f );
	float GetCameraMoveSensitivity() { return m_fCameraMoveSensitivity; }

private:
	void DisplayRendererProfiling();

	CMultiDocTemplate *m_DocTemplate;
	IToolBox *m_ToolBox;
	DWORD m_TickCount;

	// for fullscreen support
	UINT m_uOldWidth;
	UINT m_uOldHeight;
	bool m_bFullScreen;
	WINDOWPLACEMENT m_wpPrev;
	CPoint m_ptMaxSize;
	DWORD m_oldStyles;
	DWORD m_oldExStyles;
	HMENU m_OldMenu;
	CMDIChildWnd *m_CurChild;
	CMDIChildWnd *m_TempChild;
	HWND m_OldParent;
	HWND m_OldOwner;
	CFullScreenFrame m_NewFullScreenWnd;
	CView *m_pActiveView;
	float m_fCameraMoveSensitivity;
};

extern AFX_EXTENSION_MODULE RenderViewerDLL;

#undef AFX_DATA
#define AFX_DATA
