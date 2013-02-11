///============================================================================
/// \file		StateMachineEditorView.h
/// \brief		Header file for State Machine Editor view class
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

#if !defined(AFX_STATEMACHINEEDITORVIEW_H__305F5776_A2DD_4E37_B8E4_2E140F34F290__INCLUDED_)
#define AFX_STATEMACHINEEDITORVIEW_H__305F5776_A2DD_4E37_B8E4_2E140F34F290__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CStateMachineEditorView view

class CStateMachineEditorView : public CView
{
protected:
	
	CStateMachineEditorView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CStateMachineEditorView)

// Operations
public:
	virtual ~CStateMachineEditorView();
	IHashString *GetStateMachineName();

	///	\brief	this function updates entity with passed object name hash
	///	\param	name - hash of the object name
	void OnInitObject( IHashString *name )
	{
		m_DiagramEditor.RefreshEntity( name );
	}

	///	\brief	select object in the editor 
	///	\param	name - hash of the object name
	void SelectObject( IHashString *name )
	{
		m_DiagramEditor.SelectObject( name );
	}

	///	\brief	validate passed data in the editor
	///	\param	sop - pointer to serialization parameters
	///	\return EE message code
	DWORD ValidateObject(SERIALIZEOBJECTPARAMS *sop)
	{
		return m_DiagramEditor.ValidateObject( sop );
	}

	///	\brief	check if all objects in state machine are valid
	///	\return	true if state machine is valid
	bool IsValid() const
	{
		return m_DiagramEditor.IsValid();
	}

	void EndDrawingTransitionMode();

	bool CanAddEvent();

protected:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStateMachineEditorView)	
//Overrides
	virtual void OnDraw(CDC* pDC);	// overridden to draw this view
	virtual void OnInitialUpdate();	// called first time after construct
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CStateMachineEditorView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnStateButton();
	afx_msg void OnEventButton();
	afx_msg void OnEntryEventButton();
	afx_msg void OnUpdateEventButton();
	afx_msg void OnExitEventButton();
	afx_msg void OnTransitionMode();
	afx_msg void OnValidate();
	afx_msg void OnFlatMode();
	afx_msg void OnUpdateStateButton(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEventButton(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTransitionButton(CCmdUI* pCmdUI);
	afx_msg void OnUpdateValidateButton(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	IToolBox *m_ToolBox;
	CStateMachineDiagramEditor m_DiagramEditor;
	bool	m_bActive;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATEMACHINEEDITORVIEW_H__305F5776_A2DD_4E37_B8E4_2E140F34F290__INCLUDED_)
