#if !defined(AFX_BvhLoader_H__80A18743_A0D3_4593_AADE_3EB5BA220CC5__INCLUDED_)
#define AFX_BvhLoader_H__80A18743_A0D3_4593_AADE_3EB5BA220CC5__INCLUDED_

// BvhLoader.cpp : implementation file
//

#include "stdafx.h"
#include "BvhPart.h"
#include "Utility.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#undef AFX_DATA
#define AFX_DATA AFX_DATA_IMPORT


// {96E04B8F-292D-4bfc-81CA-C9255F0E0BFA}
static const GUID GUID_BVH = 
{ 0x96e04b8f, 0x292d, 0x4bfc, { 0x81, 0xca, 0xc9, 0x25, 0x5f, 0xe, 0xb, 0xfa } };

using namespace std;

typedef list<struct BITMAPIMAGES *> BITIMAGELIST;

typedef struct BITMAPIMAGES
{
	CBitmap *bitmap;
	BITIMAGELIST mipmaps;
} BITMAPIMAGES;


typedef list<CBvhObject *> BVHOBJLIST;

// BvhLoader.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CBvhDoc document

class AFX_CLASS_EXPORT CBvhLoader : public CObjectDLLInterface
{
	// attributes
public:
	UINT m_PageIndex;
	CProgramInfo *m_ProgramInfo;
	BVHOBJLIST m_BvhObjects;

	// implementation
public:
	CBvhLoader(CProgramInfo *pi);
	~CBvhLoader();

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

extern AFX_EXTENSION_MODULE BvhLoaderDLL;
extern CBvhLoader *gBvhLoader;

#undef AFX_DATA
#define AFX_DATA

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BvhLoader_H__80A18743_A0D3_4593_AADE_3EB5BA220CC5__INCLUDED_)
