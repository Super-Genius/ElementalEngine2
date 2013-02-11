// TerrainDoc.cpp : implementation file
//

#include "stdafx.h"
#include "TerrainEditor.h"
#include "TerrainDoc.h"


// CTerrainDoc

IMPLEMENT_DYNCREATE(CTerrainDoc, CDocument)

CTerrainDoc::CTerrainDoc()
{
}

BOOL CTerrainDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CTerrainDoc::~CTerrainDoc()
{
}


BEGIN_MESSAGE_MAP(CTerrainDoc, CDocument)
END_MESSAGE_MAP()


// CTerrainDoc diagnostics

#ifdef _DEBUG
void CTerrainDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTerrainDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CTerrainDoc serialization

void CTerrainDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CTerrainDoc commands
