#pragma once


// CTerrainDoc document

class CTerrainDoc : public CDocument
{
	DECLARE_DYNCREATE(CTerrainDoc)

public:
	CTerrainDoc();
	virtual ~CTerrainDoc();
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual BOOL OnNewDocument();

	DECLARE_MESSAGE_MAP()
};
