///============================================================================
/// \file		EffectEditor.h
/// \brief		Declaration of Effect Editor
/// \date		01-30-2007
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

#ifndef _EFFECTEDITOR_H__
#define _EFFECTEDITOR_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#undef AFX_DATA
#define AFX_DATA AFX_DATA_IMPORT

class CEffectEditor : public IDLL
{
	SINGLETONCONSTRUCTOROVERRIDE(CEffectEditor);
	CEffectEditor();

public:
	~CEffectEditor();
    virtual CString *GetName();
	virtual DWORD GetPriority();
    virtual BOOL Initialize();

	void OnCreate(CMDIFrameWnd *mainWnd);

	BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra,
		AFX_CMDHANDLERINFO* pHandlerInfo);

	void OnGlobalUpdateTick( void );

	void StartEditor();
	void StopEditor();
	CEffectEditorView* GetActiveView();
	void DisplayToolbar(bool bDisplay);

private:
	IToolBox *m_ToolBox;
	HMENU m_hMenu;
	HACCEL m_hAccel;
	CMDIFrameWnd *m_pMainWnd;
	CMultiDocTemplate *m_DocTemplate;

	CEffectEditorCmdTarget m_EffectEditorCmdTarget;
	CEffectEditorToolbar m_EffectEditorToolbar;
	CEffectEditorDockWin m_EffectEditorDockWin;	
};

extern AFX_EXTENSION_MODULE EffectEditorDLL;

#undef AFX_DATA
#define AFX_DATA

#endif _EFFECTEDITOR_H__