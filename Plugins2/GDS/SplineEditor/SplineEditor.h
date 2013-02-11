///============================================================================
/// \file		SplineComponent.cpp
/// \brief		Header file for Spline Editor
/// \date		11-14-2006
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

#include "stdafx.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SplineEditor.h : header file
//
#undef AFX_DATA
#define AFX_DATA AFX_DATA_IMPORT

/////////////////////////////////////////////////////////////////////////////
// CSplineEditor document


class CSplineEditor : public IDLL
{
	SINGLETONCONSTRUCTOROVERRIDE(CSplineEditor);
	CSplineEditor();
// attributes
private:
	UINT m_PageIndex;
	RESOURCECOLLECTION m_ObjCollection;
	
// implementation
public:   
	~CSplineEditor();
    virtual CString *GetName();
	virtual DWORD GetPriority();
    virtual BOOL Initialize();

// message handlers
	BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra,
		AFX_CMDHANDLERINFO* pHandlerInfo);
	void OnCreate(CMDIFrameWnd *mainWnd);
	DWORD EditDocument(/*IArchive &ar */);

	/// Spline in world
	/// \param lfep - load file extension parameters
	/// \return true on success, false on failure
	bool LoadSpline(LOADFILEEXTPARAMS *lfep);

	/// Spline from file drop
	/// \param doi - drop object info params
	/// \return true on success, false on failure
	DWORD LoadSplineFromFileDrop(DROPOBJECTINFO *doi);

	/// Get the extension of the file
	/// \param filepath - file to get extension for
	/// \return CString - extension of the file
	CString GetExtension(const TCHAR *filepath);

// attributes
private:
	CMultiDocTemplate *m_DocTemplate;
	IToolBox *m_ToolBox;
};

extern AFX_EXTENSION_MODULE SplineEditorDLL;

#undef AFX_DATA
#define AFX_DATA

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
