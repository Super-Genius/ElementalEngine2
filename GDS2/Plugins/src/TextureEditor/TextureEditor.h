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

#if !defined(AFX_TextureEditor_H__80A18743_A0D3_4593_AADE_3EB5BA220CC5__INCLUDED_)
#define AFX_TextureEditor_H__80A18743_A0D3_4593_AADE_3EB5BA220CC5__INCLUDED_

// TextureEditorr.cpp : implementation file
//

#include "stdafx.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#undef AFX_DATA
#define AFX_DATA AFX_DATA_IMPORT

// {84C022D9-93B4-41bc-B670-5B2054959ECC}
static const GUID GUID_TEXTURE = 
{ 0x84c022d9, 0x93b4, 0x41bc, { 0xb6, 0x70, 0x5b, 0x20, 0x54, 0x95, 0x9e, 0xcc } };

//typedef list<CTextureData *> TEXDATALIST;

// TextureEditor.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CTextureDoc document

class CTextureEditor : public IDLL
{
	SINGLETONCONSTRUCTOROVERRIDE(CTextureEditor);
	CTextureEditor();
	// attributes
private:
	UINT m_PageIndex;
	RESOURCECOLLECTION m_ObjCollection;

	HMENU m_hMenu;
	HACCEL m_hAccel;


	// implementation
public:
	~CTextureEditor();

    virtual CString *GetName();
    virtual BOOL Initialize();
	DWORD GetPriority();

	DWORD OpenTexture( LPTSTR filename );

	void OnCreate(CMDIFrameWnd *mainWnd);
	BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

	CTextureEditorCmdTarget m_TextureEditorCmdTarget;

// attributes
protected:
	CMultiDocTemplate *m_DocTemplate;
	IToolBox *m_ToolBox;
	CTextureComponent *m_TextureComponent;	// hooks into EE
};

extern AFX_EXTENSION_MODULE TextureEditorDLL;

#undef AFX_DATA
#define AFX_DATA

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TextureEditorr_H__80A18743_A0D3_4593_AADE_3EB5BA220CC5__INCLUDED_)
