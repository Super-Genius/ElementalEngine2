///============================================================================
/// \file		StateMachineEditorDoc.h
/// \brief		Header file for State Machine Editor document class
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

#if !defined(AFX_STATEMACHINEEDITORDOC_H__80A18743_A0D3_4593_AADE_3EB5BA220CC5__INCLUDED_)
#define AFX_STATEMACHINEEDITORDOC_H__80A18743_A0D3_4593_AADE_3EB5BA220CC5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CStateMachineEditorDoc document

class CStateMachineEditorDoc : public CDocument
{
protected:
	
	CStateMachineEditorDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CStateMachineEditorDoc)

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStateMachineEditorDoc)
	public:
	
	protected:
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument( LPCTSTR lpszPathName );
	virtual BOOL OnSaveDocument( LPCTSTR lpszPathName );
	virtual void OnCloseDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CStateMachineEditorDoc();
	IHashString *GetStateMachineName();

//=== state machine object creation functions ==================================
	///	\brief	create new state machine
	///	\return	name of the newly created state machine
	static CHashString CreateStateMachine();

	///	\brief	create new state machine state
	///	\param	parentName - name of the parent object
	///	\return	name of the newly created state
	CHashString CreateState( IHashString *parentName ) const;

	///	\brief	create new state machine event
	///	\param	parentName - name of the parent object
	///	\return	name of the newly created state
	CHashString CreateEvent( IHashString *parentName ) const;

	///	\brief	create new state machine transition
	///	\param	parentName - name of the parent object
	///	\param	szTargetState - name of the target state
	///	\return	name of the newly created state
	CHashString CreateTransition( IHashString *parentName, LPCTSTR szTargetState ) const;

	// Generated message map functions
protected:
	//{{AFX_MSG(CStateMachineEditorDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	///	\brief	create unique object name with state machine name as prefix.
	///			this matches to state machine naming convention.
	///	\param	type - type of object to find name
	///	\return	unique name for the object
	CHashString GenerateName( IHashString *type ) const;

// Attributes
private:
	IToolBox* m_ToolBox;
	CHashString m_hszStateMachineName;
	CHashString m_hszStateMachineType;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATEMACHINEEDITORDOC_H__80A18743_A0D3_4593_AADE_3EB5BA220CC5__INCLUDED_)
