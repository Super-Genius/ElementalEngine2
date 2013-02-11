#if !defined(AFX_ObjLoader_H__80A18743_A0D3_4593_AADE_3EB5BA220CC5__INCLUDED_)
#define AFX_ObjLoader_H__80A18743_A0D3_4593_AADE_3EB5BA220CC5__INCLUDED_

// ObjLoader.cpp : implementation file
//

#include "stdafx.h"
#include "ObjPart.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#undef AFX_DATA
#define AFX_DATA AFX_DATA_IMPORT

// {84C022D9-93B4-41bc-B670-5B2054959ECC}
static const GUID GUID_TEXTURE = 
{ 0x84c022d9, 0x93b4, 0x41bc, { 0xb6, 0x70, 0x5b, 0x20, 0x54, 0x95, 0x9e, 0xcc } };

using namespace std;

typedef list<struct BITMAPIMAGES *> BITIMAGELIST;

typedef struct BITMAPIMAGES
{
	CBitmap *bitmap;
	BITIMAGELIST mipmaps;
} BITMAPIMAGES;


// when we add animation, this should work, because it has access
// to the CObjDoc object?

typedef list<CWavefrontObj *> WAVFOBJLIST;

// ObjLoader.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CObjDoc document

class AFX_CLASS_EXPORT CObjLoader : public CObjectDLLInterface
{
	// attributes
public:
	UINT m_PageIndex;
	CProgramInfo *m_ProgramInfo;
	WAVFOBJLIST m_WavfObjects;

	// implementation
public:
	CObjLoader(CProgramInfo *ProgramInfo);
	~CObjLoader();

	LPTSTR GetLoadStrings();
	LPTSTR GetSaveStrings();

	DWORD Load(LPTSTR filename);
	DWORD Save(LPTSTR filename);
    virtual CString *GetName();
    virtual BOOL Initialize();

// attributes
protected:
	CMultiDocTemplate *m_DocTemplate;

};

enum TEXTURETYPES
{
	TEXTURE_2D = 0,
	TEXTURE_3D,
	TEXTURE_CUBEMAP,
	TEXTURE_ANIMATED
};

extern AFX_EXTENSION_MODULE ObjLoaderDLL;
extern CObjLoader *gObjLoader;

#undef AFX_DATA
#define AFX_DATA

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ObjLoader_H__80A18743_A0D3_4593_AADE_3EB5BA220CC5__INCLUDED_)
