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
#include "GUIEditor.h"
#include "GUIAnimationView.h"
#include ".\guianimationview.h"

// draw sizes
#define COLUMN1 98
#define COLUMN2 211
#define CELLWIDTH 10
#define CELLHEIGHT 16
#define TOPBORDER 26

// CGUIAnimationView dialog

IMPLEMENT_DYNAMIC(CGUIAnimationView, CDialog)
CGUIAnimationView::CGUIAnimationView(CWnd* pParent /*=NULL*/)
	: CDialog(CGUIAnimationView::IDD, pParent)
{
	m_iTimePosition = 0;
	m_iXOffset = 0;
	m_iYOffset = 0;
	m_iFrameRate = 0;
	m_iDotSpotClicked = 0;
	m_Doc = NULL;	
}

CGUIAnimationView::~CGUIAnimationView()
{
}

void CGUIAnimationView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ANIMSELCOMBO, m_AnimSelComboBox);
}


BEGIN_MESSAGE_MAP(CGUIAnimationView, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_VIEWANIMDATA, ViewAnimData)
	ON_COMMAND(ID_ADDKEY, AddOneKey)
	ON_COMMAND(ID_DELETEKEY, DeleteKey)	
	ON_CBN_SELCHANGE(IDC_ANIMSELCOMBO, OnCbnSelchangeAnimselcombo)
END_MESSAGE_MAP()


// CGUIAnimationView message handlers

void CGUIAnimationView::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
//	OnOK();
}

void CGUIAnimationView::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
//	OnCancel();
}

void CGUIAnimationView::OnMouseMove(UINT nFlags, CPoint point)
{
	CDialog::OnMouseMove(nFlags, point);
}

void CGUIAnimationView::OnPaint()
{
	CPaintDC dc(this); // device context for painting	
	dc.SetBkColor(RGB(255, 255, 255));
	dc.FloodFill(0, TOPBORDER, RGB(255, 255, 255));
	
	DrawStaticItems(&dc);
	DrawList(&dc);
	DrawDotSpots(&dc);
	DrawTimeBar(&dc);	
}

void CGUIAnimationView::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	Invalidate();
}

void CGUIAnimationView::DrawStaticItems(CPaintDC *dc)
{
	RECT rect;
	GetClientRect(&rect);
	rect.top += TOPBORDER;
	
	CPen linePen;
	linePen.CreatePen(PS_SOLID, 1, RGB(165, 165, 165));
	dc->SelectObject(&linePen);
	
	// border
	dc->MoveTo(rect.left, rect.top);
	dc->LineTo(rect.right-1, rect.top);
	dc->LineTo(rect.right-1, rect.bottom-1);
	dc->LineTo(rect.left, rect.bottom-1);
	dc->LineTo(rect.left, rect.top);

	// labels
	// NOTE: not offset by y, so we can always see the labels when we scroll down
	dc->TextOut(1 + m_iXOffset, 1 + TOPBORDER, _T("Objects"), _tcslen(_T("Objects")));
    dc->TextOut((COLUMN1+2) + m_iXOffset, 1 + TOPBORDER, _T("Channels"), _tcslen(_T("Channels")));	

	// label border
	// NOTE: also not offset by y
	dc->MoveTo(COLUMN1 + m_iXOffset, rect.top);
	dc->LineTo(COLUMN1 + m_iXOffset, rect.bottom-1);
	dc->MoveTo(COLUMN2-1 + m_iXOffset, rect.top);
	dc->LineTo(COLUMN2-1 + m_iXOffset, rect.bottom-1);
	dc->MoveTo(COLUMN2 + m_iXOffset, rect.top);
	dc->LineTo(COLUMN2 + m_iXOffset, rect.bottom-1);
	dc->MoveTo(rect.left, rect.top + CELLHEIGHT);
	dc->LineTo(rect.right, rect.top + CELLHEIGHT);

	int i;

	// grid stuff
	for (i=COLUMN2+CELLWIDTH; i<rect.right - m_iXOffset; i+=CELLWIDTH)
	{
		dc->MoveTo(i + m_iXOffset, rect.top + CELLHEIGHT);
		dc->LineTo(i + m_iXOffset, rect.bottom-1);	
	}

	for (i=CELLHEIGHT*2 + TOPBORDER; i<rect.bottom; i+=CELLHEIGHT)
	{
		dc->MoveTo(rect.left, i);
		dc->LineTo(rect.right, i);	
	}

    // ruler
	CFont *oldFont, rulerFont;
	rulerFont.CreateFont(10, 0, 0, 0, FW_DONTCARE, 0, 0, 0, 0, 0, 0, 0, 0, _T("Arial"));
	oldFont = dc->SelectObject(&rulerFont);
	UINT num = 5;
	TCHAR buf[10];
	for (i=COLUMN2 + (CELLWIDTH * 5); i<rect.right - m_iXOffset; i+=(CELLWIDTH * 5))
	{
		_stprintf(buf, "%d", num);
		dc->TextOut((i + (3*(2 - _tcslen(buf))))+m_iXOffset, (rect.top+4), buf, _tcslen(buf));
		num += 5;
	}
	dc->SelectObject(oldFont);
}

void CGUIAnimationView::DrawList(CPaintDC *dc)
{
	CFont *oldFont, objFont;
	objFont.CreateFont(14, 0, 0, 0, FW_NORMAL, 0, 0, 0, 0, 0, 0, 0, 0, _T("Arial"));
	oldFont = dc->SelectObject(&objFont);
	UINT position = 0;
	for (UINT i=0; i<m_vObjNames.size(); i++)
	{
		RECT objRect, channelRect;
		objRect.top = ((position*CELLHEIGHT) + (CELLHEIGHT+2)) + m_iYOffset + TOPBORDER;
		objRect.bottom = (((position+1)*CELLHEIGHT) + CELLHEIGHT) + m_iYOffset + TOPBORDER;
		objRect.left = 3 + m_iXOffset;
		objRect.right = (COLUMN1-1) + m_iXOffset;

        if (objRect.top > CELLHEIGHT + TOPBORDER)
		{
			dc->DrawText((const TCHAR*)m_vObjNames[i], -1, &objRect, 0);
		}

		channelRect.top = (((position)*CELLHEIGHT) + (CELLHEIGHT+2)) + m_iYOffset + TOPBORDER;
		channelRect.bottom = (((position+1)*CELLHEIGHT) + CELLHEIGHT) + m_iYOffset + TOPBORDER;
		channelRect.left = (COLUMN1+3) + m_iXOffset;
		channelRect.right = (COLUMN2-2) + m_iXOffset;

		if (channelRect.top > CELLHEIGHT + TOPBORDER)
		{
			dc->DrawText(_T("Position"), -1, &channelRect, 0);
		}

		channelRect.top = (((position+1)*CELLHEIGHT) + (CELLHEIGHT+2)) + m_iYOffset + TOPBORDER;
		channelRect.bottom = (((position+2)*CELLHEIGHT) + CELLHEIGHT) + m_iYOffset + TOPBORDER;

		if (channelRect.top > CELLHEIGHT + TOPBORDER)
		{
			dc->DrawText(_T("Size"), -1, &channelRect, 0);
		}

		channelRect.top = (((position+2)*CELLHEIGHT) + (CELLHEIGHT+2)) + m_iYOffset + TOPBORDER;
		channelRect.bottom = (((position+3)*CELLHEIGHT) + CELLHEIGHT) + m_iYOffset + TOPBORDER;

		if (channelRect.top > CELLHEIGHT + TOPBORDER)
		{
			dc->DrawText(_T("Color"), -1, &channelRect, 0);
		}

		channelRect.top = (((position+3)*CELLHEIGHT) + (CELLHEIGHT+2)) + m_iYOffset + TOPBORDER;
		channelRect.bottom = (((position+4)*CELLHEIGHT) + CELLHEIGHT) + m_iYOffset + TOPBORDER;

		if (channelRect.top > CELLHEIGHT + TOPBORDER)
		{
			dc->DrawText(_T("Rotation"), -1, &channelRect, 0);
		}

		position += 4;
	}
	dc->SelectObject(oldFont);
}

void CGUIAnimationView::DrawTimeBar(CPaintDC *dc)
{
	CPen linePen;
	linePen.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	dc->SelectObject(&linePen);

	int pos = ((m_iTimePosition * CELLWIDTH) + COLUMN2 + 5) + m_iXOffset;
	
	// draw diamond handle
	dc->MoveTo(pos, CELLHEIGHT + TOPBORDER);
	dc->LineTo(pos - 3, 4 + TOPBORDER);
	dc->LineTo(pos, 1 + TOPBORDER);
	dc->LineTo(pos + 3, 4 + TOPBORDER);
	dc->LineTo(pos, CELLHEIGHT + TOPBORDER);

	// draw line down
	RECT rect;
	GetClientRect(&rect);
	rect.top += TOPBORDER;

	dc->LineTo(pos, rect.bottom);
}

void CGUIAnimationView::DrawDotSpots(CPaintDC *dc)
{
	CPen circlePen;
	circlePen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	dc->SelectObject(&circlePen);

	CBrush circleBrush;
	circleBrush.CreateSolidBrush(RGB(0, 0, 0));
	dc->SelectObject(&circleBrush);

	for (UINT i=0; i<m_vDotSpots.size(); i++)
	{
		UINT xPos = ((m_vDotSpots[i].key * CELLWIDTH) + COLUMN2 + 5) + m_iXOffset;
		UINT yPos = ((m_vDotSpots[i].channel * CELLHEIGHT) + CELLHEIGHT + 9) + m_iYOffset + TOPBORDER;
		
		// don't draw into the time bar
		if (yPos > CELLHEIGHT + TOPBORDER)
		{
			dc->Ellipse(xPos-4, yPos-4, xPos+4, yPos+4);
		}
	}
}

void CGUIAnimationView::OnLButtonDown(UINT nFlags, CPoint point)
{
	int channel, key;
	CheckClickSpace(point, &channel, &key);

	if (key > -1)
	{
		// if clicked in ruler region
		if (channel == -1)
		{
			m_iTimePosition = key;
			if (!(nFlags & MK_CONTROL))
			{
				float time = (float)m_iTimePosition / (float)m_iFrameRate;
				static DWORD msgHash_ScrubGUIAnimation = CHashString(_T("ScrubGUIAnimation")).GetUniqueID();
				EngineGetToolBox()->SendMessage(msgHash_ScrubGUIAnimation, sizeof(float), &time, &m_hszLoadedAnimation, &CHashString(_T("CGUIAnimation")));
			}
			Invalidate();
		}
	}
	CDialog::OnLButtonDown(nFlags, point);
}

void CGUIAnimationView::CheckClickSpace(CPoint point, int *channel, int *key)
{
	// check channel
	*channel = -1;
	*key = -1;
	
	// return right away if clicked left of clickable region
	if (point.x < COLUMN2 + m_iXOffset)
	{
		return;
	}
	
	// otherwise, check region
	int yCheck = CELLHEIGHT + m_iYOffset + TOPBORDER;
	int xCheck = COLUMN2 + m_iXOffset;

	// if the user clicked the ruler region, return right away
	if ((point.y < CELLHEIGHT + TOPBORDER) && (point.y > TOPBORDER))
	{
		*channel = -1;
	}
	else if (point.y < TOPBORDER)
	{
		// not in ruler region...
		*channel = -2;
		return;
	}
	else
	{
		while (point.y > (int)yCheck)
		{
			*channel = *channel + 1;
			yCheck += CELLHEIGHT;
		}
	}
	while (point.x > (int)xCheck)
	{
		*key = *key + 1;
		xCheck += CELLWIDTH;
	}
}

void CGUIAnimationView::SetAnimation(CHashString animName)
{
	m_vObjNames.clear();
	m_vDotSpots.clear();
	GUIANIMDATA gad;	
	static DWORD msgHash_GetAnimationData = CHashString(_T("GetAnimationData")).GetUniqueID();
	DWORD retval = EngineGetToolBox()->SendMessage(msgHash_GetAnimationData, sizeof(GUIANIMDATA), &gad, &animName, &CHashString(_T("CGUIAnimation")));
	if (retval == MSG_HANDLED)
	{
		m_iFrameRate = gad.iFrameRate;
		m_hszLoadedAnimation = gad.animName;
		for (UINT i=0; i<gad.vObjects.size(); i++)
		{
			UINT j;
			m_vObjNames.push_back(gad.vObjects[i].objName);
			for (j=0; j<gad.vObjects[i].posChannel.xPos.size(); j++)
			{
				DOTSPOT ds;
				ds.key = gad.vObjects[i].posChannel.xPos[j].key;
				ds.channel = (i*4);
				m_vDotSpots.push_back(ds);
			}
			for (j=0; j<gad.vObjects[i].sizeChannel.width.size(); j++)
			{
				DOTSPOT ds;
				ds.key = gad.vObjects[i].sizeChannel.width[j].key;
				ds.channel = (i*4) + 1;
				m_vDotSpots.push_back(ds);
			}
			for (j=0; j<gad.vObjects[i].colorChannel.r.size(); j++)
			{
				DOTSPOT ds;
				ds.key = gad.vObjects[i].colorChannel.r[j].key;
				ds.channel = (i*4) + 2;
				m_vDotSpots.push_back(ds);
			}
			for (j=0; j<gad.vObjects[i].rotationChannel.rot.size(); j++)
			{
				DOTSPOT ds;
				ds.key = gad.vObjects[i].rotationChannel.rot[j].key;
				ds.channel = (i*4) + 3;
				m_vDotSpots.push_back(ds);
			}
		}
	}
	Invalidate();

	m_AnimSelComboBox.SetWindowText(animName.GetString());
}
void CGUIAnimationView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
	 // Get the minimum and maximum scroll-bar positions.
	int minpos;
	int maxpos;
	GetScrollRange(SB_HORZ, &minpos, &maxpos); 
	maxpos = GetScrollLimit(SB_HORZ);

	// Get the current position of scroll box.
	int curpos = GetScrollPos(SB_HORZ);

	// Determine the new position of scroll box.
	switch (nSBCode)
	{
		case SB_LEFT:      // Scroll to far left.
		{
			curpos = minpos;
			break;
		}
		case SB_RIGHT:      // Scroll to far right.
		{
			curpos = maxpos;
			break;
		}
		case SB_ENDSCROLL:   // End scroll.
		{
			break;
		}
		case SB_LINELEFT:      // Scroll left.
		{
			if (curpos > minpos)
			{
				curpos--;
			}
			break;
		}
		case SB_LINERIGHT:   // Scroll right.
		{
			if (curpos < maxpos)
			{
				curpos++;
			}
			break;
		}
		case SB_PAGELEFT:    // Scroll one page left.
		{
			// Get the page size. 
			SCROLLINFO   info;
			GetScrollInfo(SB_HORZ, &info, SIF_ALL);
   
			if (curpos > minpos)
			{
				curpos = max(minpos, curpos - (int) info.nPage);
			}
			break;
		}
		case SB_PAGERIGHT:      // Scroll one page right.
		{
			// Get the page size. 
			SCROLLINFO   info;
			GetScrollInfo(SB_HORZ, &info, SIF_ALL);

			if (curpos < maxpos)
			{
				curpos = min(maxpos, curpos + (int) info.nPage);
			}
			break;
		}
		case SB_THUMBPOSITION: // Scroll to absolute position. nPos is the position
		{
			curpos = nPos;      // of the scroll box at the end of the drag operation.
			break;
		}
		case SB_THUMBTRACK:   // Drag scroll box to specified position. nPos is the
		{
			curpos = nPos;     // position that the scroll box has been dragged to.
			break;
		}
	}

	// Set the new position of the thumb (scroll box).
	SetScrollPos(SB_HORZ, curpos);
	
	m_iXOffset = -curpos * CELLWIDTH;
	Invalidate();
}

void CGUIAnimationView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
	// Get the minimum and maximum scroll-bar positions.
	int minpos;
	int maxpos;
	GetScrollRange(SB_VERT, &minpos, &maxpos); 
	maxpos = GetScrollLimit(SB_VERT);

	// Get the current position of scroll box.
	int curpos = GetScrollPos(SB_VERT);

	// Determine the new position of scroll box.
	switch (nSBCode)
	{
		case SB_TOP:      
		{
			curpos = minpos;
			break;
		}
		case SB_BOTTOM:   
		{
			curpos = maxpos;
			break;
		}
		case SB_ENDSCROLL:
		{
			break;
		}
		case SB_LINEUP:   
		{
			if (curpos > minpos)
			{
				curpos--;
			}
			break;
		}
		case SB_LINEDOWN: 
		{
			if (curpos < maxpos)
			{
				curpos++;
			}
			break;
		}
		case SB_PAGEUP:   
		{
			// Get the page size. 
			SCROLLINFO   info;
			GetScrollInfo(SB_VERT, &info, SIF_ALL);
   
			if (curpos > minpos)
			{
				curpos = max(minpos, curpos - (int) info.nPage);
			}
			break;
		}
		case SB_PAGEDOWN: 
		{
			// Get the page size. 
			SCROLLINFO   info;
			GetScrollInfo(SB_VERT, &info, SIF_ALL);

			if (curpos < maxpos)
			{
				curpos = min(maxpos, curpos + (int) info.nPage);
			}
			break;
		}
		case SB_THUMBPOSITION: 
		{
			curpos = nPos;    
			break;
		}
		case SB_THUMBTRACK:   
		{
			curpos = nPos;    
			break;
		}
	}

	// Set the new position of the thumb (scroll box).
	SetScrollPos(SB_VERT, curpos);
	
	m_iYOffset = -curpos * CELLHEIGHT;
	Invalidate();
}

void CGUIAnimationView::SetAnimBarPos(UINT pos)
{
	if (m_iTimePosition != pos)
	{
		m_iTimePosition = pos;
		Invalidate();
	}
}
void CGUIAnimationView::OnRButtonDown(UINT nFlags, CPoint point)
{
	CDialog::OnRButtonDown(nFlags, point);
	int channel, key;
	CheckClickSpace(point, &channel, &key);
	if (key > -1)
	{
		if (channel > -1)
		{
			for (UINT i=0; i<m_vDotSpots.size(); i++)
			{
				if ((m_vDotSpots[i].key == key) && (m_vDotSpots[i].channel == channel))
				{
					m_iDotSpotClicked = i;
					CMenu menu;
					menu.LoadMenu(IDR_ANIMVIEWMENU);
					CMenu *pMenu = menu.GetSubMenu(0);

					RECT rect;
					GetWindowRect(&rect);	
					if (pMenu)
					{
						pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x + rect.left, point.y + rect.top, this);
					}
					return;
				}
			}
			m_EmptyChannelClicked.channel = channel;
			m_EmptyChannelClicked.key = key;

			UINT objNum = channel / 4;
			if (objNum < m_vObjNames.size())
			{
				CMenu menu;
				menu.LoadMenu(IDR_EMPTYSPOT);
				CMenu *pMenu = menu.GetSubMenu(0);

				RECT rect;
				GetWindowRect(&rect);	
				if (pMenu)
				{
					pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x + rect.left, point.y + rect.top, this);
				}
			}
		}
	}
}

void CGUIAnimationView::ViewAnimData()
{
	GUIANIMDATA gad;	
	static DWORD msgHash_GetAnimationData = CHashString(_T("GetAnimationData")).GetUniqueID();
	DWORD retval = EngineGetToolBox()->SendMessage(msgHash_GetAnimationData, sizeof(GUIANIMDATA), &gad, &m_hszLoadedAnimation, &CHashString(_T("CGUIAnimation")));
	if (retval == MSG_HANDLED)
	{
		DOTSPOT ds = m_vDotSpots[m_iDotSpotClicked];
		UINT objNum = ds.channel / 4;
		UINT channelType = ds.channel % 4;

		if (channelType == 0) // pos channel
		{
			for (UINT i=0; i<gad.vObjects[objNum].posChannel.xPos.size(); i++)
			{
				if (gad.vObjects[objNum].posChannel.xPos[i].key == ds.key)
				{
					float x, y;
					x = gad.vObjects[objNum].posChannel.xPos[i].value;
					y = gad.vObjects[objNum].posChannel.yPos[i].value;

					if (m_Doc)
					{
						m_Doc->ShowPosData(ds.key, gad.vObjects[objNum].objName, x, y);
					}
					
					return;
				}
			}
		}
		else if (channelType == 1) // size channel
		{
			for (UINT i=0; i<gad.vObjects[objNum].sizeChannel.width.size(); i++)
			{
				if (gad.vObjects[objNum].sizeChannel.width[i].key == ds.key)
				{
					float width, height;
					width = gad.vObjects[objNum].sizeChannel.width[i].value;
					height = gad.vObjects[objNum].sizeChannel.height[i].value;

					if (m_Doc)
					{
						m_Doc->ShowSizeData(ds.key, gad.vObjects[objNum].objName, width, height);
					}
					
					return;
				}
			}
		}
		else if (channelType == 2) // color channel
		{
			for (UINT i=0; i<gad.vObjects[objNum].colorChannel.alpha.size(); i++)
			{
				if (gad.vObjects[objNum].colorChannel.alpha[i].key == ds.key)
				{
					float a, r, g, b;
					a = gad.vObjects[objNum].colorChannel.alpha[i].value;
					r = gad.vObjects[objNum].colorChannel.r[i].value;
					g = gad.vObjects[objNum].colorChannel.g[i].value;
					b = gad.vObjects[objNum].colorChannel.b[i].value;

					if (m_Doc)
					{
						m_Doc->ShowColorData(ds.key, gad.vObjects[objNum].objName, a, r, g, b);
					}
					
					return;
				}
			}
		}
		else if (channelType == 3) // rotation channel
		{
			for (UINT i=0; i<gad.vObjects[objNum].rotationChannel.rot.size(); i++)
			{
				if (gad.vObjects[objNum].rotationChannel.rot[i].key == ds.key)
				{
					float rot;
					rot = gad.vObjects[objNum].rotationChannel.rot[i].value;					

					if (m_Doc)
					{
						m_Doc->ShowRotationData(ds.key, gad.vObjects[objNum].objName, rot);
					}
					
					return;
				}
			}
		}
	}
}

void CGUIAnimationView::AddOneKey()
{
	UINT objNum = m_EmptyChannelClicked.channel / 4;
	UINT channelType = m_EmptyChannelClicked.channel % 4;

	if (m_Doc)
	{
		m_Doc->AddOneKey(m_vObjNames[objNum], m_EmptyChannelClicked.key, channelType);
	}	
}

void CGUIAnimationView::DeleteKey()
{
	DOTSPOT ds = m_vDotSpots[m_iDotSpotClicked];

	// figure out what key index this is
	UINT keyPos = 0;
	for (int i=(int)m_iDotSpotClicked - 1; i>=0; i--)
	{
		if (m_vDotSpots[i].channel != ds.channel)
		{
			break;
		}
		keyPos++;
	}

	UINT objNum = ds.channel / 4;
	UINT channelType = ds.channel % 4;

	if (m_Doc)
	{
		m_Doc->DeleteOneKey(m_vObjNames[objNum], keyPos, channelType);
	}	
}

void CGUIAnimationView::LoadAnimations(vector<StdString> anims)
{
	for (UINT i=0; i<anims.size(); i++)
	{
		m_AnimSelComboBox.InsertString(-1, anims[i]);
	}
}

void CGUIAnimationView::LoadAnimation(StdString anim)
{
	m_AnimSelComboBox.InsertString(-1, anim);	
}

void CGUIAnimationView::OnCbnSelchangeAnimselcombo()
{
	int selection = m_AnimSelComboBox.GetCurSel();
	CString animName;
	m_AnimSelComboBox.GetLBText(selection, animName);
	CHashString selectedAnim(animName);
	if (m_Doc)
	{
		m_Doc->SetCurrentAnimation(selectedAnim);
	}
}
