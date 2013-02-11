///============================================================================
/// \file		StateMachineEditor.h
/// \brief		Header file for State Machine Editor
/// \date		03-05-2007
/// \author		Richard Nguyen
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

#if !defined(AFX_STATEMACHINEEDITOR_H__80A18743_A0D3_4593_AADE_3EB5BA220CC5__INCLUDED_)
#define AFX_STATEMACHINEEDITOR_H__80A18743_A0D3_4593_AADE_3EB5BA220CC5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StateMachineEditor.h : header file
//
#undef AFX_DATA
#define AFX_DATA AFX_DATA_IMPORT

/////////////////////////////////////////////////////////////////////////////
// CStateMachineEditor document

class CStateMachineEditor : public IDLL
{
	SINGLETONCONSTRUCTOROVERRIDE(CStateMachineEditor);
	CStateMachineEditor();
// attributes
private:
	UINT m_PageIndex;
	RESOURCECOLLECTION m_ObjCollection;
	
// implementation
public:   
	~CStateMachineEditor();
	void	LoadXMS( IHashString *hszFilePath );
    virtual CString *GetName();
	virtual DWORD GetPriority();
    virtual BOOL Initialize();

// message handlers
	BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	void OnCreate(CMDIFrameWnd *mainWnd);
	DWORD CreateStateMachine();
	static void StateMachineEditorCreationCallback(void *userData);	

	///	\brief	provide access to state machine editor toolbar
	///	\return	pointer to CStateMachineToolBar
	CStateMachineToolBar *GetToolBar()
	{
		return &m_ToolBar;
	}

	///	\brief	this function shows or hides state machine editor toolbar
	///	\param	show - true if toolbar should be shown or false otherwise
	void ShowToolBar(bool show);

	///	\brief	this function updates all state machine editor views with passed
	///			object name hash
	///	\param	name - hash of the object name
	void OnInitObject(IHashString *name);

	///	\brief	select object in the editor 
	///	\param	name - object name
	void SelectObject(IHashString *name);

	///	\brief	validate passed data in the editor
	///	\param	sop - pointer to serialization parameters
	///	\return EE message code
	DWORD ValidateObject(SERIALIZEOBJECTPARAMS *sop);

private:
	///	\brief	select object in the editor 
	///	\param	name - hash of the object name
	void UpdateViews(IHashString *name, void (CStateMachineEditorView:: *pViewFun)(IHashString *));
// attributes
private:
	CStateMachineToolBar m_ToolBar;
	BOOL m_ToolBarShown;
	CMultiDocTemplate *m_DocTemplate;
	IToolBox *m_ToolBox;
};

extern AFX_EXTENSION_MODULE StateMachineEditorDLL;

#undef AFX_DATA
#define AFX_DATA

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATEMACHINEEDITOR_H__80A18743_A0D3_4593_AADE_3EB5BA220CC5__INCLUDED_)
