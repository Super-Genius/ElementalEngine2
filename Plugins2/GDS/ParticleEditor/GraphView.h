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

#pragma once


class CParticleDoc;

// CGraphView view

class CGraphView : public CView
{
protected:
	DECLARE_DYNCREATE(CGraphView)
	DECLARE_MESSAGE_MAP()
	inline CParticleDoc* GetDocument()
	{ return (CParticleDoc*)m_pDocument; }

public:
	CGraphView();           // protected constructor used by dynamic creation
	virtual ~CGraphView();	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

	virtual void OnDraw(CDC* pDC);      // overridden to draw this view

	afx_msg int OnCreate(LPCREATESTRUCT lpcs);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	void ApplyChanges(float lasteffectedkeytime);
	void GraphUpdateComplete(float lasteffectedkeytime);
protected:
	CGraph			m_Graph;
	bool			m_bActive;
};


