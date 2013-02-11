#if !defined(AFX_BASESCENEDL_H__80A18743_A0D3_4593_AADE_3EB5BA220CC5__INCLUDED_)
#define AFX_BASESCENEDL_H__80A18743_A0D3_4593_AADE_3EB5BA220CC5__INCLUDED_

// BaseSceneDL.cpp : implementation file
//

#include "stdafx.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BaseSceneDL.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBaseSceneDL document

class CBaseSceneDL : public CBaseDLLInterface
{
// attributes
public:
	CProgramInfo *m_ProgramInfo;

	HMENU m_hMenu;		// our local menu
	HACCEL m_hAccel;	// our local accelerator table
	CSceneDLDockWin m_SceneDLDockWin;

// implementation
public:
	CBaseSceneDL(CProgramInfo *pi);
	~CBaseSceneDL();
	void OnCreate(CMDIFrameWnd *mainWnd);
	BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual DWORD Save(LPTSTR filename);
	virtual DWORD Load(LPTSTR filename);
    virtual CString *GetName();
    virtual BOOL Initialize();


// attributes
protected:
	CMultiDocTemplate *m_DocTemplate;
};

extern CBaseSceneDL *gBaseSceneDL;
extern AFX_EXTENSION_MODULE BaseSceneDLDLL;
extern CLogger *gLogger;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BASESCENEDL_H__80A18743_A0D3_4593_AADE_3EB5BA220CC5__INCLUDED_)
