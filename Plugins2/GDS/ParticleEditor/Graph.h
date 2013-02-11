#ifndef GRAPH_H
#define GRAPH_H

// Usage: 
// Currently requires IParticle.h for KEYFRAME_DATA structures.
// in your dialog's OnInitDialog you could have the following code:
// m_Viewer.Create(NULL, _T(""), WS_VISIBLE, CRect(0,0,100,100), this, 1);
// m_Viewer.SetGraphData(...);


#define GRAPHCONTROL_CLASSNAME    _T("MFCGraphCtrl")
#define GRAPH_AXIS_BUFFER		40
#define VERTICAL_TAB_NUM		10  //UNDONE: DETERMIN TAB NUMBER BY FONT
#define HORIZONTAL_TAB_NUM		20	//UNDONE: DETERMIN TAB NUMBER BY FONT
#define DELTA_SELECTION			.01f
#define MAXFLOAT				100000000.f   // UNDONE better float value
#define DATA1					1
#define DATA2					2
#define DATA3					3

class CGraphView;

// Graph Data Storage class NOTE: Replace with struct?

class CGraph : public CWnd
{
protected:
	// Protected Internal Objects
	float m_fMouseX; // DEBUG
	float m_fMouseY; // DEBUG
	BOOL m_bLMouseDown;
	int m_KeySelected;
	int m_KeyMouseOver;
	POINT m_ptMousePos;
	KEYFRAME_DATA_SET *m_pCurGraphKeys;		// UNDONE?: DESTRUCTOR
	KEYFRAME_DATA_SET *m_pLastGraphKeys;	// UNDONE?: DESTRUCTOR
	RECT m_graphSize;
	RECT m_graphClip;
//	float m_GraphScrollY;
//	float m_GraphScrollX;
//	float m_GraphDomain;
//	float m_GraphRange;
//	CFont m_fontMain; UNDONE

	// MFC Protected Functions
	DECLARE_DYNAMIC(CGraph)
	DECLARE_MESSAGE_MAP()
	BOOL RegisterWindowClass();

	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//
	void UpdateScrollBar( UINT ScrollBar, UINT nPos, UINT nSBCode);
	void SetScrollValues (float domain, float range);
	void DrawGraph( CPaintDC &dc );
	void DrawAxis( CPaintDC &dc );
	void DrawDetails( CPaintDC &dc );

	void ScreenToGraph( float &x, float &y );
	void GraphToScreen( float &x, float &y );
	void ClipGraphToScreen( float &x, float &lastx, float xmin, float xmax,
												  float &y, float &lasty, float ymin, float ymax );

	float	m_fScaledDomainUpperBound;
	float	m_fScaledDomainLowerBound;
	float	m_fScaledRangeUpperBound;
	float	m_fScaledRangeLowerBound;
	float	m_fScaledDomain;
	float	m_fScaledRange;

	bool	IsValidGraph();
	float	GetValueMax();
	float	GetValueMin();
	float	GetTimeMax();
	float	GetTimeMin();
	int		GetSize();
	float	GetKeyTime(int i);
	float	GetKeyValue(int i);
	float	GetRangeMaxLength( );	
	float	GetDomainMaxLength( );
	float	GetRangeUsedLength( );
	float	GetDomainUsedLength( );
	float	GetRangeUsedScaledLength( );
	float	GetDomainUsedScaledLength( );
	void	GetRangeUsedInterval( float &upper, float &lower);
	void	GetDomainUsedInterval( float &upper, float &lower);
	void	GetRangeUsedScaledInterval( float &upper, float &lower, const float scale = 2.0f);
	void	GetDomainUsedScaledInterval( float &upper, float &lower, const float scale = 2.0f);

	void SetKeyFrame(int iKey, float x, float y);
	int GetNearKeyFrame( KEYFRAME_DATA_SET &in, POINT p );
	BOOL GetNearPointOnKeyGraph(float &x, float &y, POINT p);
	int CreateNewKeyFrame( float x, float y );
	void DestroyKeyFrame( int frame );

public:
	CGraph();
	void GenerateDebugGraph(unsigned int type);
	BOOL Create(const RECT& rect, CWnd* pParentWnd, UINT nID);
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void SetData(KEYFRAME_DATA_SET * lpKeyFrameData);
};
#endif /* #ifndef GRAPH_H */
