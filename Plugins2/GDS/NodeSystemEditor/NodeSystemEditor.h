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

#if !defined(AFX_NODESYSTEMEDITOR_H__F85F2623_8051_44B9_A931_6B01454941B3__INCLUDED_)
#define AFX_NODESYSTEMEDITOR_H__F85F2623_8051_44B9_A931_6B01454941B3__INCLUDED_

// NodeSystemEditor.cpp : implementation file
//

#include "stdafx.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NodeSystemEditor.h : header file
//
#undef AFX_DATA
#define AFX_DATA AFX_DATA_IMPORT

/////////////////////////////////////////////////////////////////////////////
// CNodeSystemEditor document


class CNodeSystemEditor : public IDLL
{
	SINGLETONCONSTRUCTOROVERRIDE(CNodeSystemEditor);
	CNodeSystemEditor();

// implementation
public:
	~CNodeSystemEditor();
	virtual CString *GetName();
	virtual DWORD GetPriority();
	virtual BOOL Initialize();

	void OnCreate(CMDIFrameWnd *mainWnd);
	BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

	void ObjectSelected( IHashString *hszObjectName, IHashString *hszComponentType );
	void ActivateNodePlot();
	void DeactivateNodePlot();
	void ActivateConnectionPlot();
	void DeactivateConnectionPlot();
	void PlotNode();
	void PlotNode( Vec3 *plotPosition );
	void PlotConnection( IHashString *hszParentName, IHashString *hszConnectedNode );
	DWORD HandleEditorLeftClick( Vec3 *hitPosition );

// attributes
private:
	void MoveObjectInFrontOfCamera(IHashString *objName);
	void GetParentObjectName( IHashString *hszParentObjectName, IHashString *hszObjectName, IHashString *hszComponentType );

	RESOURCECOLLECTION m_ObjCollection;
	IToolBox *m_ToolBox;
	CNodeToolPal m_NodeToolPal;
	CMDIFrameWnd *m_pMainWnd;
	bool m_bNodePlot;
	bool m_bConnectionPlot;
	CHashString m_hszLastNodeName;
	CHashString m_hszSelectedObjectName;
	CHashString m_hszSelectedObjectType;
};

extern AFX_EXTENSION_MODULE NodeSystemEditorDLL;

#undef AFX_DATA
#define AFX_DATA

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NODESYSTEMEDITOR_H__F85F2623_8051_44B9_A931_6B01454941B3__INCLUDED_)
