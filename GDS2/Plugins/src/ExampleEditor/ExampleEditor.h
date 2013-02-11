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

#if !defined(AFX_ExampleEditor_H__80A18743_A0D3_4593_AADE_3EB5BA220CC5__INCLUDED_)
#define AFX_ExampleEditor_H__80A18743_A0D3_4593_AADE_3EB5BA220CC5__INCLUDED_

// ExampleEditor.cpp : implementation file
//

#include "stdafx.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExampleEditor.h : header file
//
#undef AFX_DATA
#define AFX_DATA AFX_DATA_IMPORT

/////////////////////////////////////////////////////////////////////////////
// CExampleEditor document


class CExampleEditor : public IDLL
{
	SINGLETONCONSTRUCTOROVERRIDE(CExampleEditor);
	CExampleEditor();
// attributes
private:
	UINT m_PageIndex;
	RESOURCECOLLECTION m_ObjCollection;
	
// implementation
public:   
	~CExampleEditor();
    virtual CString *GetName();
	virtual DWORD GetPriority();
    virtual BOOL Initialize();

// message handlers
	BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra,
		AFX_CMDHANDLERINFO* pHandlerInfo);
	void OnCreate(CMDIFrameWnd *mainWnd);
	DWORD EditDocument(/*IArchive &ar */);

// attributes
private:
	CMultiDocTemplate *m_DocTemplate;
	IToolBox *m_ToolBox;
};

extern AFX_EXTENSION_MODULE ExampleEditorDLL;

#undef AFX_DATA
#define AFX_DATA

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ExampleEditor_H__80A18743_A0D3_4593_AADE_3EB5BA220CC5__INCLUDED_)
