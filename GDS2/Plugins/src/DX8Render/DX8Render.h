#if !defined(AFX_DX8Render_H__80A18743_A0D3_4593_AADE_3EB5BA220CC5__INCLUDED_)
#define AFX_DX8Render_H__80A18743_A0D3_4593_AADE_3EB5BA220CC5__INCLUDED_

// DX8Render.cpp : implementation file
//

#include "stdafx.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DX8Render.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDX8Render document

class CDX8Render : public IDLL
{
// attributes
public:
	CProgramInfo *m_ProgramInfo;

// implementation
public:
	CDX8Render(CProgramInfo *pi);
	~CDX8Render();
	BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	BOOL OnIdle(LONG lCount);
    virtual CString *GetName();
    virtual BOOL Initialize();
    virtual CString *GetAPIName();
    virtual CRender *GetRenderer();


// attributes
protected:
	CMultiDocTemplate *m_DocTemplate;
    CDX8RenderCore *m_Renderer;
};

extern CDX8Render *gDX8Render;
extern AFX_EXTENSION_MODULE DX8RenderDLL;
extern CLogger *gLogger;


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DX8Render_H__80A18743_A0D3_4593_AADE_3EB5BA220CC5__INCLUDED_)
