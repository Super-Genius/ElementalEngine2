///============================================================================
/// \file		WorldEventEditor.h
/// \brief		Declaration of World Event Editor
/// \date		01-30-2007
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

#ifndef _WORLDEVENTEDITOR_H__
#define _WORLDEVENTEDITOR_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#undef AFX_DATA
#define AFX_DATA AFX_DATA_IMPORT

class CWorldEventEditor : public IDLL
{
	SINGLETONCONSTRUCTOROVERRIDE(CWorldEventEditor);
	CWorldEventEditor();

public:
	~CWorldEventEditor();
    virtual CString *GetName();
	virtual DWORD GetPriority();
    virtual BOOL Initialize();

	void OnCreate(CMDIFrameWnd *mainWnd);

	BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra,
		AFX_CMDHANDLERINFO* pHandlerInfo);

	void OnGlobalUpdateTick( void );

	IHashString* GetSelectedObject() { return &m_SelectedObjectName; }
	void SetSelectedObject( IHashString *pObjectName );
	void WaitForObjectSelection( bool bWait ) { m_bWaitingForObjectSelection = bWait; }
	bool IsWaitingForObjectSelection() { return m_bWaitingForObjectSelection; }

	bool AddWorldEventToEntity( IHashString *pSrcEntityName, IHashString *pSrcStateEventName, IHashString *pDstEntityName, IHashString *pDstStateEventName );
	void ShowAddGameEventPopupMenu( CPoint& point );
	void SetVisualization( bool bShow );

private:
	IToolBox *m_ToolBox;
	HMENU m_hMenu;
	HACCEL m_hAccel;

	CHashString m_hsWorldEventRenderObjectClass;
	CHashString m_hsWorldEventRenderObjectName;
	bool m_bRenderObjectShown;

	// just needs to be be here to be instianted so it registers
	// it's menu handlers.
	CWorldEventCmdTarget m_WorldEventCmdTarget;

	CWorldEventToolPal m_WorldEventToolPal;

	CWorldEventDockWin m_WorldEventDockWin;	

	CHashString m_SelectedObjectName;
	bool m_bSelectedObject;

	bool m_bWaitingForObjectSelection;
};

extern AFX_EXTENSION_MODULE WorldEventEditorDLL;

#undef AFX_DATA
#define AFX_DATA

#endif _WORLDEVENTEDITOR_H__