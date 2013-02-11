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

#if !defined(AFX_SCENEDLDOC_H__80A18743_A0D3_4593_AADE_3EB5BA220CC5__INCLUDED_)
#define AFX_SCENEDLDOC_H__80A18743_A0D3_4593_AADE_3EB5BA220CC5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SceneDLDoc.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CSceneDLDoc document

class CSceneDLDoc : public CDocument
{
protected:
	CSceneDLDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CSceneDLDoc)

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSceneDLDoc)
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void OnCloseDocument();
protected:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSceneDLDoc();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSceneDLDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CHashString m_hszWorldName;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCENEDLDOC_H__80A18743_A0D3_4593_AADE_3EB5BA220CC5__INCLUDED_)
