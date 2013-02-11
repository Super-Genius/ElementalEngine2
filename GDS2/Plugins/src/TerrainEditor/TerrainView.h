#pragma once


// CTerrainView view

class CTerrainView : public CView
{
	DECLARE_DYNCREATE(CTerrainView)

protected:
	CTerrainView();           // protected constructor used by dynamic creation
	virtual ~CTerrainView();
	CTerrainToolPal	m_TerrainToolPal;

public:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()

private:
public:
	afx_msg void OnBnClickedRaise();
	afx_msg void OnBnClickedLower();
	afx_msg void OnEnChangeSizeEdit();
	afx_msg void OnEnHscrollSizeEdit();
	afx_msg void OnEnVscrollSizeEdit();
	afx_msg void OnEnChangeStrengthEdit();
	afx_msg void OnEnHscrollStrengthEdit();
protected:
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
public:
	virtual void OnInitialUpdate();
};


