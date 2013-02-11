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

#include "stdafx.h"

// MFC Message Macros
BEGIN_MESSAGE_MAP(CGraph, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

IMPLEMENT_DYNAMIC(CGraph, CWnd)

CGraph::CGraph()
{
	RegisterWindowClass();
}

BOOL CGraph::RegisterWindowClass()
{
	WNDCLASS wndcls;
	HINSTANCE hInst = AfxGetInstanceHandle();

	if (!(::GetClassInfo(hInst, GRAPHCONTROL_CLASSNAME, &wndcls)))
	{
		// otherwise we need to register a new class
		wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
		wndcls.lpfnWndProc      = ::DefWindowProc;
		wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
		wndcls.hInstance        = hInst;
		wndcls.hIcon            = NULL;
		wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
		wndcls.hbrBackground    = (HBRUSH) (COLOR_3DFACE + 1);
		wndcls.lpszMenuName     = NULL;
		wndcls.lpszClassName    = GRAPHCONTROL_CLASSNAME;

		if (!AfxRegisterClass(&wndcls))
		{
			AfxThrowResourceException();
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CGraph::Create(const RECT& rect, CWnd* pParentWnd, UINT nID)// DWORD dwStyle /*=WS_VISIBLE*/)
{
	/*
	return CWnd::Create(
		GRAPHCONTROL_CLASSNAME, 
		_T(""), 
		WS_VISIBLE | WS_CHILD | WS_VSCROLL | WS_HSCROLL, 
		rect, 
		pParentWnd, 
		nID);
	*/
	return CWnd::CreateEx(
		WS_EX_LEFTSCROLLBAR,
		GRAPHCONTROL_CLASSNAME, 
		_T(""), 
		WS_VISIBLE | WS_CHILD | WS_VSCROLL | WS_HSCROLL, 
		rect, 
		pParentWnd, 
		nID);
}

int CGraph::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate (lpCreateStruct) == -1)
		return -1;
	
	m_fScaledDomainUpperBound = 0.0f;
	m_fScaledDomainLowerBound = 0.0f;
	m_fScaledRangeUpperBound = 0.0f;
	m_fScaledRangeLowerBound = 0.0f;
	m_fScaledDomain = 0.0f;
	m_fScaledRange = 0.0f;

	m_pCurGraphKeys = NULL;
	m_pLastGraphKeys = NULL;
	m_bLMouseDown = FALSE;
	m_KeySelected = -1;
	m_KeyMouseOver = -1;
	return 0;
}

void CGraph::SetData(KEYFRAME_DATA_SET * lpKeyFrameData)
{
//	DEBUG
	if (lpKeyFrameData == NULL || lpKeyFrameData->m_Keys == NULL)
	{
#ifdef _DEBUG
		GenerateDebugGraph(DATA3);

		m_fScaledRange = GetRangeUsedScaledLength();
		m_fScaledDomain = GetDomainUsedScaledLength();
		// Get Scaled Boundaries of Graph.
		GetDomainUsedScaledInterval( m_fScaledDomainUpperBound, m_fScaledDomainLowerBound);
		GetRangeUsedScaledInterval( m_fScaledRangeUpperBound, m_fScaledRangeLowerBound);
#endif /* #ifdef _DEBUG */
	}
	else
	{
		if (lpKeyFrameData->m_Keys != NULL)
		{
			m_pCurGraphKeys = lpKeyFrameData;

			m_fScaledRange = GetRangeUsedScaledLength();
			m_fScaledDomain = GetDomainUsedScaledLength();
			// Get Scaled Boundaries of Graph.
			GetDomainUsedScaledInterval( m_fScaledDomainUpperBound, m_fScaledDomainLowerBound);
			GetRangeUsedScaledInterval( m_fScaledRangeUpperBound, m_fScaledRangeLowerBound);

			float range = GetRangeMaxLength( );
			float domain = GetDomainMaxLength();
			SetScrollValues( domain, range );
		}
	}

	Invalidate();
}

void CGraph::GenerateDebugGraph(unsigned int type)
{
	if (DATA1 == type) // 1.0 1.0
	{
		m_pCurGraphKeys = new KEYFRAME_DATA_SET();
		m_pCurGraphKeys->m_TimeMax = 1.0f;
		m_pCurGraphKeys->m_TimeMin = 0.0f;
		m_pCurGraphKeys->m_ValueMax = 1.0f;
		m_pCurGraphKeys->m_ValueMin = 0.0f;
		m_pCurGraphKeys->m_Keys = new KEYFRAME_DATA[11];
		m_pCurGraphKeys->m_Size = 0;
		for (int i = 0; i < 10; i++)
		{
			m_pCurGraphKeys->m_Keys[i].time = (float)i / 10.0f;
			if ( i % 2 )
			{
				m_pCurGraphKeys->m_Keys[i].value = (float)i / 10.0f;
			}
			else
			{
				m_pCurGraphKeys->m_Keys[i].value = (float)i / 20.0f;
			}
			m_pCurGraphKeys->m_Size++;
		}
	}
	if (DATA2 == type)
	{
		m_pCurGraphKeys = new KEYFRAME_DATA_SET();
		m_pCurGraphKeys->m_TimeMax = 1.0f;
		m_pCurGraphKeys->m_TimeMin = 0.0f;
		m_pCurGraphKeys->m_ValueMax = 1000.0f;
		m_pCurGraphKeys->m_ValueMin = 0.0f;
		m_pCurGraphKeys->m_Keys = new KEYFRAME_DATA[11];
		m_pCurGraphKeys->m_Size = 0;
		for (int i = 0; i < 10; i++)
		{
			m_pCurGraphKeys->m_Keys[i].time = (float)i / 10.f;
			if ( i % 2 )
			{
				m_pCurGraphKeys->m_Keys[i].value = (float)i*2.f;
			}
			else
			{
				m_pCurGraphKeys->m_Keys[i].value = (float)i;
			}
			m_pCurGraphKeys->m_Size++;
		}
	}
	if (DATA3 == type)
	{
		m_pCurGraphKeys = new KEYFRAME_DATA_SET();
		m_pCurGraphKeys->m_TimeMax = 1.0f;
		m_pCurGraphKeys->m_TimeMin = 0.0f;
		m_pCurGraphKeys->m_ValueMax = 1000.0f;
		m_pCurGraphKeys->m_ValueMin = -1000.0f;
		m_pCurGraphKeys->m_Keys = new KEYFRAME_DATA[11];
		m_pCurGraphKeys->m_Size = 0;
		for (int i = 0; i < 10; i++)
		{
			m_pCurGraphKeys->m_Keys[i].time = (float)i / 10.f;
			if ( i % 2 )
			{
				m_pCurGraphKeys->m_Keys[i].value = (float)((float)pow(-1.f,i)* i*2.f);
			}
			else
			{
				m_pCurGraphKeys->m_Keys[i].value = (float)i;
			}
			m_pCurGraphKeys->m_Size++;
		}

	}
	
}

void CGraph::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	SCROLLINFO si;
	GetScrollInfo (SB_HORZ, &si, SIF_ALL);
	int iPos = si.nPos;
	int iMax = si.nMax;
	UpdateScrollBar(SB_HORZ, nPos, nSBCode);
	if (!m_pCurGraphKeys)
		return;
	float fGraphDomain = GetDomainMaxLength();
	float domain = m_pCurGraphKeys->m_TimeMax - m_pCurGraphKeys->m_TimeMin - fGraphDomain; 	// KELTEMP: SUSPECT
	float domainincrement = domain /iMax; 	// KELTEMP: SUSPECT

	//m_GraphScrollX = m_pCurGraphKeys->m_TimeMin + domainincrement * iPos; 	// KELTEMP: SUSPECT
	GetDomainUsedScaledInterval( m_fScaledDomainUpperBound, m_fScaledDomainLowerBound);
	Invalidate();
}

void CGraph::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	SCROLLINFO si;
	GetScrollInfo (SB_VERT, &si, SIF_ALL);	
	int iPos = si.nPos;
	int iMax = si.nMax;
	UpdateScrollBar(SB_VERT, nPos, nSBCode);
	if (!m_pCurGraphKeys)
		return;
//	float range = m_pCurGraphKeys->m_ValueMax - m_pCurGraphKeys->m_ValueMin - GetDomainMaxLength(); 	// KELTEMP: SUSPECT
//	float rangeincrement = range / iMax;	// KELTEMP: SUSPECT
//	m_GraphScrollY = m_pCurGraphKeys->m_ValueMin + rangeincrement * (iMax - iPos);	// KELTEMP: SUSPECT

//	float range = m_pCurGraphKeys->m_ValueMax - m_pCurGraphKeys->m_ValueMin - GetRangeMaxLength(); 	// KELTEMP: SUSPECT
//	float rangeincrement = range / iMax;	// KELTEMP: SUSPECT

	//	m_GraphScrollY = m_pCurGraphKeys->m_ValueMin + fGraphRange * (iMax - iPos);	// KELTEMP: SUSPECT
	GetRangeUsedScaledInterval( m_fScaledRangeUpperBound, m_fScaledRangeLowerBound);
	Invalidate();
}
void CGraph::UpdateScrollBar( UINT ScrollBar, UINT nPos, UINT nSBCode )
{
	SCROLLINFO si;
	GetScrollInfo(ScrollBar, &si, SIF_ALL);
	bool redraw = TRUE;
	int CurPos = si.nPos;

	// Determine the new position of scroll box.
	switch (nSBCode)
	{
	case SB_LEFT:      // Scroll to far left.
		CurPos = 0;
		break;

	case SB_RIGHT:      // Scroll to far right.
		CurPos = si.nMax;
		break;

	case SB_ENDSCROLL:   // End scroll.
		break;
	case SB_LINELEFT:      // Scroll left.
		if (CurPos > 0)
			CurPos--;
		else if (CurPos == 0)
			CurPos = 0;
		else
			redraw = FALSE;
		break;

	case SB_LINERIGHT:   // Scroll right.
		if (CurPos < si.nMax)
			CurPos++;
		else if (CurPos == si.nMax)
			CurPos = si.nMax;
		else
			redraw = FALSE;
		break;

	case SB_PAGELEFT:    // Scroll one page left.
		{
   			if (CurPos > 0)
				CurPos = max(0, CurPos - si.nPage);
			else
				redraw = FALSE;
		}
		break;

	case SB_PAGERIGHT:      // Scroll one page right
		{
			if (CurPos < si.nMax)
				CurPos = min((int)si.nMax, CurPos + (int)si.nPage);
			else
				redraw = FALSE;
		}
		break;


	case SB_THUMBPOSITION: // Scroll to absolute position. nPos is the position
		CurPos = nPos;//si.nTrackPos;      // of the scroll box at the end of the drag operation.
		redraw = TRUE; // UNDONE do something less insane here.
		break;

	case SB_THUMBTRACK:   // Drag scroll box to specified position. nPos is the
		CurPos = nPos;//si.nTrackPos;     // position that the scroll box has been dragged to.
		redraw = TRUE;
		break;
	}

	// Set the new position of the thumb (scroll box).
	si.fMask = SIF_POS;
	si.nPos = CurPos;
	SetScrollInfo(ScrollBar, &si, redraw);
}



void CGraph::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize (nType, cx, cy);

	GetClientRect(&m_graphSize);

	if (m_pCurGraphKeys)
	{
		float range = m_pCurGraphKeys->m_ValueMax - m_pCurGraphKeys->m_ValueMin;
		float domain = m_pCurGraphKeys->m_TimeMax - m_pCurGraphKeys->m_TimeMin;
		SetScrollValues( domain, range );
	}
	else
	{
		SetScrollValues( 1.0f, 1.0f );
	}
/*	
	SCROLLINFO si;
	si.fMask = SIF_RANGE | SIF_POS | SIF_PAGE;
	si.nMin = 0;
	si.nMax = m_graphSize.bottom-1;
	si.nPage = 1; // UNDONE: Set Page Cleaner
	si.nPos = m_graphSize.bottom-1;
	SetScrollInfo (SB_VERT, &si, TRUE);

	si.nMin = 0;
	si.nMax = m_graphSize.right-1;
	si.nPage = 1;	// UNDONE: Set Page Cleaner
	si.nPos = 0;
	SetScrollInfo (SB_HORZ, &si, TRUE);
*/


	m_graphClip.left = m_graphSize.left + GRAPH_AXIS_BUFFER;
	m_graphClip.right = m_graphSize.right - 5;
	m_graphClip.top = m_graphSize.top + 5;
	m_graphClip.bottom = m_graphSize.bottom - GRAPH_AXIS_BUFFER;

}

void CGraph::SetScrollValues (float domain, float range)
{
	int visRange = (int)GetRangeMaxLength();
	int visDomain = (int)GetDomainMaxLength();

	SCROLLINFO si;
	si.fMask = SIF_RANGE | SIF_PAGE;
	si.nMin = 0;
	if (visRange)
		si.nMax = (int)(range)/visRange - 1;
	else
		si.nMax = 0;
	si.nPage = 1; // UNDONE: Set Page Cleaner
	SetScrollInfo (SB_VERT, &si, TRUE);

	si.nMin = 0;
	if (visDomain)
		si.nMax = (int)(domain)/visDomain - 1;
	else
		si.nMax = 0;
	si.nPage = 1;	// UNDONE: Set Page Cleaner
	SetScrollInfo (SB_HORZ, &si, TRUE);
}
// Safe Functions for retrieving data.
bool CGraph::IsValidGraph()
{
	if (m_pCurGraphKeys)
		return true;
	else
		return false;
}

float CGraph::GetValueMax()
{
	if (m_pCurGraphKeys)
        return m_pCurGraphKeys->m_ValueMax; 
	else
		return 0.0f;
}
float CGraph::GetValueMin()
{
	if (m_pCurGraphKeys)
        return m_pCurGraphKeys->m_ValueMin; 
	else
		return 0.0f;
}
float CGraph::GetTimeMax()
{
	if (m_pCurGraphKeys)
        return m_pCurGraphKeys->m_TimeMax; 
	else
		return 0.0f;
}
float CGraph::GetTimeMin()
{
	if (m_pCurGraphKeys)
        return m_pCurGraphKeys->m_TimeMin; 
	else
		return 0.0f;
}
int CGraph::GetSize()
{
	if (m_pCurGraphKeys)
        return m_pCurGraphKeys->m_Size; 
	else
		return 0;
}
float CGraph::GetKeyTime(int i)
{
	if (m_pCurGraphKeys && (i >= 0) && (i < GetSize()))
        return m_pCurGraphKeys->m_Keys[i].time;
	else
		return 0.0f;
}

float CGraph::GetKeyValue(int i)
{
	if (IsValidGraph() && (i >= 0) && (i < GetSize()))
        return m_pCurGraphKeys->m_Keys[i].value;
	else
		return 0.0f;
}

float CGraph::GetRangeMaxLength( )
{
	if (!IsValidGraph())
		return 0.0f;
	return (m_pCurGraphKeys->m_ValueMax - m_pCurGraphKeys->m_ValueMin);//debug
}

float CGraph::GetDomainMaxLength( )
{

	if (!IsValidGraph())
		return 0.f;
	return (m_pCurGraphKeys->m_TimeMax - m_pCurGraphKeys->m_TimeMin); //debug

}

float CGraph::GetRangeUsedLength( )
{
	if (!IsValidGraph())
		return 0.f;
	float upper = 0.0f;
	float lower = 0.0f;
	GetRangeUsedInterval( upper, lower);
	return upper - lower;
}
float CGraph::GetDomainUsedLength( )
{
	if (!IsValidGraph())
		return 0.f;
	float upper = 0.0f;
	float lower = 0.0f;
	GetDomainUsedInterval( upper, lower);
	return upper - lower;
}
float CGraph::GetRangeUsedScaledLength( )
{
	if (!IsValidGraph())
		return 0.f;
	float upper = 0.0f;
	float lower = 0.0f;
	GetRangeUsedScaledInterval( upper, lower);
	return upper - lower;
}
float CGraph::GetDomainUsedScaledLength( )
{
	if (!IsValidGraph())
		return 0.f;
	float upper = 0.0f;
	float lower = 0.0f;
	GetDomainUsedScaledInterval( upper, lower);
	return upper - lower;
}

void CGraph::GetRangeUsedInterval( float &upper, float &lower)
{
	// Find find upper and lower bounds of graph.
	upper = 0.0f;
	lower = 0.0f;
	if (IsValidGraph())
	{
		for (int i = 0; i < GetSize(); i++)
		{
			if (GetKeyValue(i) > upper)
				upper = GetKeyValue(i);
			if (GetKeyValue(i) < lower)
				lower = GetKeyValue(i);
		}
	}
}

void CGraph::GetDomainUsedInterval( float &upper, float &lower)
{
	// Find find upper and lower bounds of graph.
	upper = 0.0f;
	lower = 0.0f;
	if (IsValidGraph())
	{
		for (int i = 0; i < GetSize(); i++)
		{
			if (GetKeyTime(i) > upper)
				upper = GetKeyTime(i);
			if (GetKeyTime(i) < lower)
				lower = GetKeyTime(i);
		}
	}
}
void CGraph::GetRangeUsedScaledInterval( float &upper, float &lower, const float scale)
{
	// Find find upper and lower bounds of graph value.
	GetRangeUsedInterval( upper, lower);
	// Scale Range.
	upper *= scale;
	lower *= scale;
	// Adjustment for Used interval of [0,0]
	if ( upper < .10f * GetValueMax() )
		upper = .10f * GetValueMax();
	if ( lower > .10f * GetValueMin() ) 
		lower = .10f * GetValueMin();
	// Clamp
	if ( upper > GetValueMax() )
		upper = GetValueMax();
	if ( lower < GetValueMin() )
		lower = GetValueMin();
}
void CGraph::GetDomainUsedScaledInterval( float &upper, float &lower, const float scale)
{
	// Find find upper and lower bounds of graph time.
	GetDomainUsedInterval( upper, lower);
	// Scale Domain.
	upper *= scale;
	lower *= scale;
	// Adjustment for Used interval of [0,0]
	if ( upper < .10f * GetTimeMax() )
		upper = .10f * GetTimeMax();
	if ( lower > .10f * GetTimeMin() ) 
		lower = .10f * GetTimeMin();
	// Clamp
	if (upper > GetTimeMax())
		upper = GetTimeMax();
	if (lower < GetTimeMin())
		lower = GetTimeMin();
}

void CGraph::OnPaint()
{
	CPaintDC dc (this);

	DrawAxis(dc);
	DrawGraph(dc);
	DrawDetails(dc);
}
int GetVerticleTabNum(int fScreenHeight)
{
	return VERTICAL_TAB_NUM;
}
int GetHorizontalTabNum(int fScreenWidth)
{
	return HORIZONTAL_TAB_NUM;
}

void CGraph::DrawAxis( CPaintDC &dc)
{	
 	CBrush Brush( RGB(255,255,255 ) );
	dc.FillRect(&m_graphSize, &Brush);
	CPen thickpen (PS_SOLID, 2, RGB(0,0,0));
	CPen thinpen (PS_SOLID, 0, RGB(0,0,0));
	CPen thickgraypen (PS_SOLID, 2, RGB(128,128,128));
	if( m_pCurGraphKeys )
	{
		// Draw Axis
		dc.SelectObject(&thickpen);
			// Vertical Axis
		dc.MoveTo( m_graphClip.left, m_graphClip.bottom);
 		dc.LineTo( m_graphClip.left, m_graphClip.top);
			//Horizontal Axis
//		dc.MoveTo( m_graphClip.left, m_graphClip.bottom);
//		dc.LineTo( m_graphClip.right, m_graphClip.bottom);
	}

	// Draw Text
	int screenwidth = m_graphClip.right - m_graphClip.left;
	int screenheight = m_graphClip.bottom - m_graphClip.top;
	// UNDONE: FIX THIS DYNAMIC RESOLUTION SO IT ONLY DISPLAYS THOSE ELEMENTS THAT FIT IN THE AREA
	int VerticalVariableTabHeight = screenheight /  GetVerticleTabNum(screenheight); // Dynamic resolution
	int HorizontalVariableTabWidth = screenwidth / GetHorizontalTabNum(screenwidth); // Dynamic resolution
	if( m_pCurGraphKeys )
	{
		RECT r;
		char buffer[256];
		
		// Draw Grey Line for 0.0f
		float fZeroX = m_fScaledDomainLowerBound;
		float fZeroY = 0.0f;
		float fZeroXEnd = m_fScaledDomainUpperBound;
		float fZeroYEnd = 0.0f;

		//clip
		ClipGraphToScreen( fZeroXEnd, fZeroX, m_fScaledDomainLowerBound, m_fScaledDomainUpperBound,
							fZeroYEnd, fZeroY, m_fScaledRangeLowerBound, m_fScaledRangeUpperBound );
		GraphToScreen( fZeroXEnd, fZeroYEnd );
		GraphToScreen( fZeroX, fZeroY );
		dc.SelectObject(&thickgraypen);
		dc.MoveTo( (int)fZeroX, (int)fZeroY );
		dc.LineTo( (int)fZeroXEnd,(int)fZeroYEnd );

		dc.SelectObject(&thinpen);
		for (int i = 0; i <= VERTICAL_TAB_NUM ; i++)
		{
			// Small ticks to mark place.
			dc.MoveTo( (int)m_graphClip.left, (int)m_graphClip.bottom - i*VerticalVariableTabHeight);
			dc.LineTo( (int)m_graphClip.left - 5,(int)m_graphClip.bottom - i*VerticalVariableTabHeight);

			// Text box for text
			r.left = m_graphSize.left;
			r.top = m_graphClip.bottom - (i)*VerticalVariableTabHeight - 7;
			r.bottom = r.top + 30;
			r.right = r.left + 100;

			// Convert screenspace to coordinate space
			// VerticalVariableTabHeight = the hight of each mark seperation.
			// i*VerticalVariableTabHeight = the height of current mark seperation from min.
			// screenheight is the total height of the window. Dividing i*VerticalVariableTabHeight by this
			// gives the % of the total visible graph range(m_GraphRange). 

			// Get Scaled Values:
//			float fScaledRangeUpperBound = 0.0f;
//			float fScaledRangeLowerBound = 0.0f;
//			GetRangeUsedScaledInterval( fScaledRangeUpperBound, fScaledRangeLowerBound);

			float value = (float)(i * VerticalVariableTabHeight);
			value /= (float)screenheight;// 0 - 1
			value *= m_fScaledRange;
			value += m_fScaledRangeLowerBound; //add the screen coordinate offset based on clip
			if( abs(value) <= 1.f )
			{
				sprintf(buffer, "%5.2f", value );
			}
			else if (abs(value) >= 1000)
			{
				sprintf(buffer, "%5.0f", value );
			}
			else
			{
				sprintf(buffer, "%5.1f", value );
			}
			dc.DrawText( buffer, -1, &r, 0 );
		}
		for( int i = 0; i <= HORIZONTAL_TAB_NUM ; i++ )
		{
			// Small ticks to mark place.
			dc.MoveTo( (int)m_graphClip.left + i*HorizontalVariableTabWidth, (int)m_graphClip.bottom);
			if ( i % 2 )
				dc.LineTo( (int)m_graphClip.left + i*HorizontalVariableTabWidth,(int)m_graphClip.bottom + 5);
			else
				dc.LineTo( (int)m_graphClip.left + i*HorizontalVariableTabWidth,(int)m_graphClip.bottom + 25);
			// Text box for text
			r.left = m_graphClip.left - 18 + i*HorizontalVariableTabWidth;
			r.top = m_graphSize.bottom - 21 - (i % 2)*15; // Off centered sequence
			r.bottom = r.top + 30;
			r.right = r.left + 100;

//			float fScaledDomainUpperBound = 0.0f;
//			float fScaledDomainLowerBound = 0.0f;
//			GetDomainUsedScaledInterval( fScaledDomainUpperBound, fScaledDomainLowerBound);

			// Set Value to the number of pixels in each tab times number of tabs.
			float value = (float)(i*HorizontalVariableTabWidth);
			// Divide Value by the total number of pixels to give a float between 0 and 1.
			value /= screenwidth;
			// Multiply this by the total scaled domain.
			value *= m_fScaledDomain;
			// Add the offset based on currently scrolled location.
			value += m_fScaledDomainLowerBound;

			if( value <= 1.f )
			{
				sprintf(buffer, "%5.2f", value );
			}
			else if (value >= 1000)
			{
				sprintf(buffer, "%5.0f", value );
			}
			else
			{
				sprintf(buffer, "%5.1f", value );
			}
			dc.DrawText( buffer, -1, &r, 0 );
		}
	}
	else 
	{
		// Numbers
		dc.DrawText(_T("NO GRAPH OR GRAPH KEYFRAME DATA"), -1, &m_graphSize, DT_SINGLELINE | DT_CENTER | DT_VCENTER );
	}
}
void CGraph::DrawGraph( CPaintDC &dc )
{
	int screenwidth = m_graphClip.right - m_graphClip.left;
	int screenheight = m_graphClip.bottom - m_graphClip.top;
	CPen blackpen (PS_SOLID, 0, RGB(0,0,0));
	CPen redpen (PS_SOLID, 1, RGB(255, 63, 63));

	dc.SelectObject(&blackpen);
	//translate lines to clipping window and draw them
	if( m_pCurGraphKeys )
	{
		KEYFRAME_DATA_SET &CurGraph = *m_pCurGraphKeys;
		if( CurGraph.m_Size > 0 )
		{
			float lastx = CurGraph.m_Keys[ 0 ].time;
			float lasty = CurGraph.m_Keys[ 0 ].value;
			float x = 0;
			float y = 0;
			//start at 1
			for( int i = 1; i < CurGraph.m_Size; i++ )
			{
				x = CurGraph.m_Keys[ i ].time;
				y = CurGraph.m_Keys[ i ].value;
/*				if( x < m_GraphScrollX ||
					y < m_GraphScrollY ||
					//out of view
					( x >= m_GraphScrollX + m_GraphDomain && 
					lastx >= m_GraphScrollX + m_GraphDomain ) ||
					( y  >= m_GraphScrollY  + m_GraphRange && 
					lasty >= m_GraphScrollY  + m_GraphRange)
					)//out of range, set for next point
				{
					lastx = x;
					lasty = y;
					continue; //don't draw this point out of range
				}
*/
				//save off untransformed x and y
				float tempx = x;
				float tempy = y;

				// Get Scaled Boundaries of Graph.
//				float fScaledDomainUpperBound = 0.0f;
//				float fScaledDomainLowerBound = 0.0f;
//				GetDomainUsedScaledInterval( fScaledDomainUpperBound, fScaledDomainLowerBound);
//				float fScaledRangeUpperBound = 0.0f;
//				float fScaledRangeLowerBound = 0.0f;
//				GetRangeUsedScaledInterval( fScaledRangeUpperBound, fScaledRangeLowerBound);

				//clip
				ClipGraphToScreen( x, lastx, m_fScaledDomainLowerBound, m_fScaledDomainUpperBound,
									y, lasty, m_fScaledRangeLowerBound, m_fScaledRangeUpperBound );
				GraphToScreen( x, y );
				GraphToScreen( lastx, lasty );
				//before we draw this make sure these points are in screen space
/*				if( lastx >= m_GraphClip.left &&
					lastx <= m_GraphClip.right &&
					lasty >= m_GraphClip.top &&
					lasty <= m_GraphClip.bottom &&
					x >= m_GraphClip.left &&
					x <= m_GraphClip.right &&
					y >= m_GraphClip.top &&
					y <= m_GraphClip.bottom )
				{
*/
				dc.SelectObject(&blackpen);
				dc.MoveTo( (int)lastx,(int)lasty );			//draw line start
				dc.LineTo( (int)x,(int)y );					//draw line end

				dc.Rectangle ((int)lastx-2, (int)lasty-2, (int)lastx+3, (int)lasty+3);
				
				// Display Mouse Over Box;
				dc.SelectObject(&redpen);
				if ( i-1 == m_KeyMouseOver )
				{
					dc.MoveTo( (int)lastx-4,(int)lasty-4 );
					dc.LineTo( (int)lastx+4,(int)lasty-4 );
					dc.LineTo( (int)lastx+4,(int)lasty+4 );
					dc.LineTo( (int)lastx-4,(int)lasty+4 );
					dc.LineTo( (int)lastx-4,(int)lasty-4 );
				}
				if ( i == m_KeyMouseOver )
				{
					dc.MoveTo( (int)x-4,(int)y-4 );
					dc.LineTo( (int)x+4,(int)y-4 );
					dc.LineTo( (int)x+4,(int)y+4 );
					dc.LineTo( (int)x-4,(int)y+4 );
					dc.LineTo( (int)x-4,(int)y-4 );
				}

				lastx = tempx;
				lasty = tempy;
			}
			if ( CurGraph.m_Size == 1 )
			{
				x = lastx;
				y = lasty;
				GraphToScreen( x, y );
			}
			dc.SelectObject(&blackpen);
			dc.Rectangle ((int)x-2, (int)y-2, (int)x+3, (int)y+3);
//			dc.MoveTo( (int)x-2,(int)y-2 );
//			dc.LineTo( (int)x+2,(int)y-2 );
//			dc.LineTo( (int)x+2,(int)y+2 );
//			dc.LineTo( (int)x-2,(int)y+2 );
//			dc.LineTo( (int)x-2,(int)y-2 );
			if ( (0 == m_KeyMouseOver) && (CurGraph.m_Size == 1))
			{
				dc.SelectObject(&redpen);
				dc.MoveTo( (int)x-4,(int)y-4 );
				dc.LineTo( (int)x+4,(int)y-4 );
				dc.LineTo( (int)x+4,(int)y+4 );
				dc.LineTo( (int)x-4,(int)y+4 );
				dc.LineTo( (int)x-4,(int)y-4 );
			}
			if ( (int)x < m_graphClip.right )
			{
				dc.SelectObject(&blackpen);
				dc.MoveTo((int)x, (int)y);
				dc.LineTo( m_graphClip.right, (int)y);
			}
		}
#ifdef _DEBUG
		char debugbuffer[256];
		sprintf(debugbuffer, "Debug: %d", CurGraph.m_Size);
		dc.DrawText(debugbuffer, -1, &m_graphClip, DT_SINGLELINE | DT_CENTER );
#endif /* #ifdef _DEBUG */
	}
	else 
	{
//		dc.DrawText(_T("NO GRAPH OR GRAPH KEYFRAME DATA"), -1, &m_graphClip, DT_SINGLELINE | DT_CENTER | DT_VCENTER );
	}
}
void CGraph::DrawDetails( CPaintDC &dc)
{
	CPen graypen (PS_SOLID, 1, RGB(127, 127, 127));

	KEYFRAME_DATA_SET &CurGraph = *m_pCurGraphKeys;
	CurGraph.m_Keys[m_KeySelected].time;


	// Guide Lines
	dc.SelectObject(&graypen);
	if (m_bLMouseDown && ( m_KeySelected != -1 ))
	{
		dc.MoveTo(m_graphSize.left, m_ptMousePos.y);
		dc.LineTo(m_graphSize.right, m_ptMousePos.y);
		dc.MoveTo(m_ptMousePos.x, m_graphSize.top);
		dc.LineTo(m_ptMousePos.x, m_graphSize.bottom);
	}
}
void CGraph::ClipGraphToScreen( float &x, float &lastx, float xmin, float xmax,
												  float &y, float &lasty, float ymin, float ymax )
{
	if( x > xmax )
	{
		//too big, use lastx and lasty to determine new point
		float slope = (y - lasty)/(x - lastx );
		float offset = y - slope*x;
		//now determine new line given final x
        y = slope*xmax + offset;
		x = xmax;
	}

//	if( x < xmin )
//	{
//	
//	}
	if( y > ymax )
	{
		//too big, use lastx and lasty to determine new point
		float slope = (y - lasty)/(x - lastx );
		float offset = y - slope*x;
		//now determine new line given final x
        x = (ymax - offset)/slope;
		y = ymax;
	}
	if ( y < ymin )
	{
		float slope = (y - lasty)/(x - lastx );
		float offset = y - slope*x;
		x = (ymin - offset)/slope;
		y = ymin;

	}
	if( lastx < xmin )
	{
		//too big, use lastx and lasty to determine new point
		float slope = (y - lasty)/(x - lastx );
		float offset = y - slope*x;
		//now determine new line given final x
        lasty = slope*xmin + offset;
		lastx = xmin;
	}
	
	if( lasty < ymin )
	{
		//too big, use lastx and lasty to determine new point
		float slope = (y - lasty)/(x - lastx );
		float offset = y - slope*x;
		//now determine new line given final x
        lastx = (ymin - offset)/slope;
		lasty = ymin;
	}
}

void CGraph::GraphToScreen( float &x, float &y )
{
	float screenwidth = (float)(m_graphClip.right - m_graphClip.left);
	float screenheight = (float)(m_graphClip.bottom - m_graphClip.top);

	// Get Scaled Boundaries of Graph.
//	float fScaledDomainUpperBound = 0.0f;
//	float fScaledDomainLowerBound = 0.0f;
//	GetDomainUsedScaledInterval( fScaledDomainUpperBound, fScaledDomainLowerBound);
//	float fScaledRangeUpperBound = 0.0f;
//	float fScaledRangeLowerBound = 0.0f;
//	GetRangeUsedScaledInterval( fScaledRangeUpperBound, fScaledRangeLowerBound);

	//transform to screenspace
	x -= m_fScaledDomainLowerBound; //offset
	x /= m_fScaledDomain; // 0 - 1.f
	x *= screenwidth; // get screenspace
	x += (m_graphClip.left); //screenspace offset

	y -= m_fScaledRangeLowerBound;//offset
	y /= m_fScaledRange;	// 0 - 1.f
	y *= screenheight;	// get screenspace
	y = m_graphClip.bottom - y; //screenspace offset		
}

void CGraph::ScreenToGraph( float &x, float &y )
{
	float screenwidth = (float)(m_graphClip.right - m_graphClip.left);
	float screenheight = (float)(m_graphClip.bottom - m_graphClip.top);

	// Get Scaled Boundaries of Graph.
//	float fScaledDomainUpperBound = 0.0f;
//	float fScaledDomainLowerBound = 0.0f;
//	GetDomainUsedScaledInterval( fScaledDomainUpperBound, fScaledDomainLowerBound);
//	float fScaledRangeUpperBound = 0.0f;
//	float fScaledRangeLowerBound = 0.0f;
//	GetRangeUsedScaledInterval( fScaledRangeUpperBound, fScaledRangeLowerBound);

    y -= m_graphClip.top;
	y /= screenheight;// 0 - 1
	y = 1.f - y;
	y *= m_fScaledRange; //expand it into screen coordinate scaling
	y += m_fScaledRangeLowerBound; //add the screen coordinate offset based on clip

	x -= m_graphClip.left;
	x /= screenwidth;// 0 - 1
	x *= m_fScaledDomain; //expand it into screen coordinate scaling
	x += m_fScaledDomainLowerBound; //add the screen coordinate offset based on clip
}

void CGraph::SetKeyFrame(int iKey, float x, float y)
{
	if (0 <= iKey && iKey < m_pCurGraphKeys->m_Size)
	{
		// KEYFRAMES CANNOT MOVE BEYOND THEIR NEIGHBOORS
		// UNDONE: If a point is added in between too points that are very close,
		// it will cause the point to be placed out of order!

		if (iKey != 0)
			if ( x < m_pCurGraphKeys->m_Keys[iKey-1].time )
				x = m_pCurGraphKeys->m_Keys[iKey-1].time + m_fScaledDomain / 100.f;
		if (iKey != m_pCurGraphKeys->m_Size-1)
			if ( x > m_pCurGraphKeys->m_Keys[iKey+1].time )
				x = m_pCurGraphKeys->m_Keys[iKey+1].time - m_fScaledDomain / 100.f;

		// X TOO LOW
		if ( x < m_pCurGraphKeys->m_TimeMin ) 
			m_pCurGraphKeys->m_Keys[iKey].time = m_pCurGraphKeys->m_TimeMin;
		// X TOO HIGH
		else if ( x > m_pCurGraphKeys->m_TimeMax )
			m_pCurGraphKeys->m_Keys[iKey].time = m_pCurGraphKeys->m_TimeMax;
		// X JUST RIGHT
		else
			m_pCurGraphKeys->m_Keys[iKey].time = x;
		// Y TOO LOW
		if ( y < m_pCurGraphKeys->m_ValueMin )
			m_pCurGraphKeys->m_Keys[iKey].value = m_pCurGraphKeys->m_ValueMin;
		// Y TOO HIGH
		else if ( y > m_pCurGraphKeys->m_ValueMax )
			m_pCurGraphKeys->m_Keys[iKey].value = m_pCurGraphKeys->m_ValueMax;
		// Y JUST RIGHT
		else
			m_pCurGraphKeys->m_Keys[iKey].value = y;

		if (iKey == 0)
			m_pCurGraphKeys->m_Keys[iKey].time = m_pCurGraphKeys->m_TimeMin;

		CGraphCtrlDockBar * temp = (CGraphCtrlDockBar*)GetParent();
		if (temp)
		{
			if (iKey == 0)
				temp->ApplyChanges(m_pCurGraphKeys->m_Keys[iKey].time);
			else if (iKey >= 1)
				temp->ApplyChanges(m_pCurGraphKeys->m_Keys[iKey-1].time);
		}
	}
	else
	{
		// KEY NOT EXTANT
	}
}

void CGraph::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	BOOL redraw = FALSE;

	//Check if it's within the graph area
	/*
	if( //m_bLMouseDown &&
		point.x < m_graphClip.right && 
		point.x > m_graphClip.left &&
		point.y < m_graphClip.bottom &&
		point.y > m_graphClip.top &&
		m_pCurGraphKeys)
	
	*/
	m_ptMousePos.x = point.x;
	m_ptMousePos.y = point.y;

	float x = (float)point.x;	// DEBUG
	float y = (float)point.y;	// DEBUG
	ScreenToGraph( x, y );		// DEBUG
	m_fMouseX = x;				// DEBUG
	m_fMouseY = y;				// DEBUG
	if (m_pCurGraphKeys)
	{
		KEYFRAME_DATA_SET &CurGraph = *m_pCurGraphKeys;
		int frame = GetNearKeyFrame( CurGraph, point );
		// A POINT SELECTED:
		if (m_KeySelected != -1)
		{
			if ( 0 <= m_KeySelected && (m_KeySelected < CurGraph.m_Size))
			{	
				SetKeyFrame(m_KeySelected, x, y);
				m_KeyMouseOver = frame;
				redraw = TRUE;
			}
			// Error condition, Key Selected out of bounds
			else
			{
				m_KeySelected = -1;
				redraw = TRUE;
			}
		}
		// NO KEYFRAME SELECTED
		else // Near a Keyframe for Mouse Over?
		{
			// MOUSE OVER EXTANT KEYFRAME
			if( ( frame >= 0 ) && ( frame < (int)CurGraph.m_Size ) )
			{
				// This avoids redrawing every mouse wiggle.
				// Mouse moves within the selection box, no redraw.
				if (m_KeyMouseOver == frame)
				{
					redraw = FALSE;
				}
				// Mouse moves from outside the selection box into selection box, redraw.
				else
				{
					m_KeyMouseOver = frame;
					redraw = TRUE;
				}
			}
			// MOUSE NOT OVER EXTANT KEYFRAME
			else
			{
				// Mouse moves around outside selection box, no redraw.
				if (m_KeyMouseOver == -1)
				{
					redraw = FALSE;
				}
				// Mouse moves from inside the selection box to outside selection box, redraw.
				else
				{
					m_KeyMouseOver = -1;
					redraw = TRUE;
				}
			}
		}
	}
	// Invalidate Window if required
	if (redraw)
		Invalidate(); 

	CWnd::OnMouseMove(nFlags, point);
}
void CGraph::OnLButtonDown(UINT nFlags, CPoint point)
{
	BOOL redraw = FALSE;
	SetCapture();
	m_bLMouseDown = TRUE;

	if (m_pCurGraphKeys)
	{
		KEYFRAME_DATA_SET &CurGraph = *m_pCurGraphKeys;
		int frame = GetNearKeyFrame( CurGraph, point );
		//CLICK EVENT ON KEYFRAME
		if( ( frame >= 0 ) && ( frame < (int)CurGraph.m_Size ) )
		{
			m_KeySelected = frame;
			redraw = TRUE;
		}
		//CLICK EVENT ON LINE OR WHITESPACE
		else
		{
			float x;
			float y;
			//CLICK EVENT ON LINE
			if (GetNearPointOnKeyGraph(x, y, point))
			{
				m_KeySelected = CreateNewKeyFrame(x, y);
				redraw = TRUE;
			}
			//CLICK EVENT ON WHITESPACE
			else
			{
				m_KeySelected = -1;
				redraw = FALSE;
			}
		}
	}

	if (redraw)
		Invalidate();

}

void CGraph::OnLButtonUp(UINT nFlags, CPoint point)
{	
	if (GetCapture())
		::ReleaseCapture();
	m_bLMouseDown = FALSE;
	if (m_KeySelected != -1)
	{
		CGraphCtrlDockBar * pGraphViewParent = (CGraphCtrlDockBar*)GetParent();
		if (pGraphViewParent)
			pGraphViewParent->GraphUpdateComplete(0.0);
	}
	m_KeySelected = -1;

	m_fScaledRange = GetRangeUsedScaledLength();
	m_fScaledDomain = GetDomainUsedScaledLength();
	GetDomainUsedScaledInterval( m_fScaledDomainUpperBound, m_fScaledDomainLowerBound);
	GetRangeUsedScaledInterval( m_fScaledRangeUpperBound, m_fScaledRangeLowerBound);

	
}
void CGraph::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (!m_bLMouseDown)
	{
		BOOL redraw = FALSE;
		if (m_pCurGraphKeys)
		{
			KEYFRAME_DATA_SET &CurGraph = *m_pCurGraphKeys;
			int frame = GetNearKeyFrame( CurGraph, point );
			//CLICK EVENT ON KEYFRAME
			if ( frame == 0)
			{
				// Do nothing, destroying first keyframe causes terrible things.
				redraw = FALSE;
			}
			else if( ( frame > 0 ) && ( frame < (int)CurGraph.m_Size ) )
			{
				DestroyKeyFrame( frame );
				redraw = TRUE;
			}
			//CLICK EVENT ON LINE OR WHITESPACE
			else
			{
				m_KeySelected = -1;
				redraw = FALSE;
			}
		}
		if (redraw)
			Invalidate();
	}
}



BOOL CGraph::GetNearPointOnKeyGraph(float &x, float &y, POINT p)
{
	//void CGraph::ClipGraphToScreen( float &x, float &lastx, float xmin, float xmax,
	//											  float &y, float &lasty, float ymin, float ymax )
	//			ClipGraphToScreen( x, lastx, m_GraphScrollX, m_GraphScrollX + m_GraphDomain,
	//								y, lasty, m_GraphScrollY, m_GraphScrollY + m_GraphRange );
	float fMouseX = (float) p.x;
	float fMouseY = (float) p.y;
	ScreenToGraph(fMouseX, fMouseY);
	KEYFRAME_DATA_SET &CurGraph = *m_pCurGraphKeys;
	float lasty = CurGraph.m_Keys[0].value;
	float lastx = CurGraph.m_Keys[0].time;
	float lx = 0.f;
	float ly = 0.f;
	for ( int i = 1; i <= CurGraph.m_Size ; i++)
	{
		if (i == CurGraph.m_Size)
		{
			lx = CurGraph.m_TimeMax;
			ly = CurGraph.m_Keys[i-1].value;
			if (lastx < fMouseX && fMouseX <= CurGraph.m_TimeMax)
			{
				x = fMouseX;
				y = lasty;
				//1234!
				if ( fabs( fMouseY - y ) > ( m_fScaledRange / 50.f )) // UNDONE current selection accuracy range is hardcoded.
					return FALSE;
				else
					return TRUE;
			}
		}
		else
		{
			lx = CurGraph.m_Keys[i].time;
			ly = CurGraph.m_Keys[i].value;

			if (lastx <= fMouseX && fMouseX <= lx)
			{
				float slope = (ly - lasty)/(lx - lastx);
	//			float offset = ly - slope*lx;
				x = fMouseX;
	//			y = slope*xmax + offset;
				y = lasty + (fMouseX - lastx)*slope;
				//1234!
				if ( fabs( fMouseY - y ) > ( m_fScaledRange / 50.f )) // UNDONE current selection accuracy range is hardcoded.
					return FALSE;
				else
					return TRUE;
			}

			lasty = ly;
			lastx = lx;
		}
	}
	return FALSE;
}


int CGraph::GetNearKeyFrame( KEYFRAME_DATA_SET &in, POINT p )
{
	float xdistance = 0.f;
	float ydistance = 0.f;
	float x = (float)p.x;
	float y = (float)p.y;
	ScreenToGraph( x, y );

	//check if we're near a keyframe, translate screen to currentgraph space
	//1234!
	float ySelectionRange = m_fScaledRange / 50.f;
	float xSelectionRange = m_fScaledDomain / 50.f ;

	//Usability HACK, it is easy to lose the points while dragging quickly, 
	//increase selection radius when a key is selected.
	if (m_KeySelected != -1)
	{
		ySelectionRange *=4.f;
		xSelectionRange *=4.f;
	}

	//check how near we are, we can select, or create a new keyframe
	for(  int i = 0; i < in.m_Size; i++ )
	{
		xdistance = fabs( in.m_Keys[ i ].time - x );
		ydistance = fabs(	in.m_Keys[ i ].value - y );
		if ( ( xdistance < xSelectionRange ) && ( ydistance < ySelectionRange ) )
			return i;
	}
	return -1;
}
// Returns new frame.
int CGraph::CreateNewKeyFrame( float x, float y )
{
	int newKey = 0;
	KEYFRAME_DATA_SET &CurGraph = *m_pCurGraphKeys;
	KEYFRAME_DATA * pKeys = new KEYFRAME_DATA[CurGraph.m_Size+1];

	pKeys[0].time = CurGraph.m_Keys[0].time;
	pKeys[0].value = CurGraph.m_Keys[0].value;
	int a = 0;
	for (int i = 1 ; i <= CurGraph.m_Size ; i++)
	{

		// LAST KEY
		if (i == CurGraph.m_Size)
		{
			if 	( ( CurGraph.m_Keys[i-1].time < x ) && ( x < CurGraph.m_TimeMax ) )
			{
				pKeys[i].time = x;
				pKeys[i].value = y;
				a++;
				newKey = i;
			}
			else
			{
				pKeys[i].time = CurGraph.m_Keys[i-a].time;
				pKeys[i].value = CurGraph.m_Keys[i-a].value;
			}
		}
		// AT KEY
		else if (i < CurGraph.m_Size)
		{
			if ( ( CurGraph.m_Keys[i-1].time < x ) && ( x < CurGraph.m_Keys[i].time ) )
			{
				pKeys[i].time = x;
				pKeys[i].value = y;
				a++;
				newKey = i;
			}
			else
			{
				pKeys[i].time = CurGraph.m_Keys[i-a].time;
				pKeys[i].value = CurGraph.m_Keys[i-a].value;
			}

		}

	}

	delete [] CurGraph.m_Keys;
	CurGraph.m_Keys = pKeys;
	CurGraph.m_Size++;

	return newKey;
}
void CGraph::DestroyKeyFrame( int frame )
{
	KEYFRAME_DATA_SET &CurGraph = *m_pCurGraphKeys;
	if( ( frame >= 0 ) && ( frame < (int)CurGraph.m_Size ) )
	{
		KEYFRAME_DATA * pKeys = new KEYFRAME_DATA[CurGraph.m_Size-1];
		int a = 0;
		for (int i = 0 ; (i < (CurGraph.m_Size)) ; i++)
		{
			// AT KEY
			if ( i == frame )
				a++;
			else
			{
				pKeys[i-a].time = CurGraph.m_Keys[i].time;
				pKeys[i-a].value = CurGraph.m_Keys[i].value;
			}
		}

		delete [] CurGraph.m_Keys;
		CurGraph.m_Keys = pKeys;
		CurGraph.m_Size--;
	}
	CGraphView * temp = (CGraphView*)GetParent();
	if (temp)
	{
		temp->ApplyChanges(0.0f);
	}


}

