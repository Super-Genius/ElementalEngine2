//
// BaseRender.h : header file
//

#if !defined(AFX_BASERENDER_H__80A18743_A0D3_4593_AADE_3EB5BA220CC5__INCLUDED_)
#define AFX_BASERENDER_H__80A18743_A0D3_4593_AADE_3EB5BA220CC5__INCLUDED_


#include "stdafx.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CBaseRender document

class CBaseRender : public CBaseDLLInterface
{
private:
    CRenderView *m_ActiveRenderer;

// attributes
public:
	CProgramInfo *m_ProgramInfo;

// implementation
public:
	CBaseRender(CProgramInfo *ProgramInfo);
	~CBaseRender();
	void OnCreate(CMDIFrameWnd *mainWnd);
	BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
    virtual CString *GetName();
    virtual BOOL Initialize();
	virtual BOOL OnIdle(LONG lCount);

// attributes
protected:
	CMultiDocTemplate *m_DocTemplate;
    STDDLLLIST m_RenderDLLS;
    HANDLELIST m_RenderDLLHandles;
	void LoadRenderDLLS();
    void InitRenderDLL(CString *name);
    void SearchDLLS(const CString& pathName, DLLTYPE type);
    void GetALLInterfaces(const CString& pathName, DLLTYPE type);

};

extern CBaseRender *gBaseRender;
extern AFX_EXTENSION_MODULE BaseRenderDLL;
extern CLogger *gLogger;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BASERENDER_H__80A18743_A0D3_4593_AADE_3EB5BA220CC5__INCLUDED_)
