///============================================================================
/// \file		LuaDoc.h
/// \brief		Header file for the Lua Document class
/// \date		06-17-2005
/// \author		Josh Lulewicz
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

#if !defined(AFX_LUADOC_H__60B9BAB0_26C5_4E6D_AB07_404295AEF7E6__INCLUDED_)
#define AFX_LUADOC_H__60B9BAB0_26C5_4E6D_AB07_404295AEF7E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLuaView;
class CProjectFile;

class CLuaDoc : public CDocument
{
protected: // create from serialization only
	CLuaDoc();
	DECLARE_DYNCREATE(CLuaDoc)

// Attributes
public:

// Operations
public:
	CLuaView* GetView();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLuaDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL DoFileSave();
	BOOL DoSave(LPCTSTR lpszPathName, BOOL bReplace = 1);
	BOOL OnFileSaveAsLua();
	CString GetName();
	CString GetPathNameNoExt();
	void AddBreakPoint(int nLine);
	void RemoveBreakPoint(int nLine);
	void RemoveAllBreakPoints();
	void AddDebugLine(int nLine);
	void RemoveAllDebugLines();
	BOOL PositionBreakPoints(CString &fileName);
	int GetNearestDebugLine(int nLine);
	int GetPreviousDebugLine(int nLine);
	int GetNextDebugLine(int nLine);
	BOOL HasBreakPoint(int nLine);
	virtual ~CLuaDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CMap<int, int, BOOL, BOOL> m_breakPoints;
	int m_nMinBreakPoint, m_nMaxBreakPoint;
	CMap<int, int, BOOL, BOOL> m_debugLines;
	int m_nMinDebugLine, m_nMaxDebugLine;

// Generated message map functions
protected:
	//{{AFX_MSG(CLuaDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnCloseDocument();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LUADOC_H__60B9BAB0_26C5_4E6D_AB07_404295AEF7E6__INCLUDED_)
