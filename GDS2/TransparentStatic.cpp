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
#include "TransparentStatic.h"
#include ".\transparentstatic.h"

	// Map "Static Styles" to "Text Styles"
#define MAP_STYLE(src, dest) if(dwStyle & (src)) dwText |= (dest)
#define NMAP_STYLE(src, dest) if(!(dwStyle & (src))) dwText |= (dest)

// CTransparentStatic

IMPLEMENT_DYNAMIC(CTransparentStatic, CStatic)
CTransparentStatic::CTransparentStatic()
{
}

CTransparentStatic::~CTransparentStatic()
{
}


BEGIN_MESSAGE_MAP(CTransparentStatic, CStatic)
	ON_WM_PAINT()
	ON_MESSAGE(WM_SETTEXT, OnSetText)
END_MESSAGE_MAP()



// CTransparentStatic message handlers

void CTransparentStatic::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	// Where to draw text
	CRect client_rect;
	GetClientRect(client_rect);

	// Get the caption
	CString szText;
	GetWindowText(szText);

	// Get the font
	CFont *pFont, *pOldFont;
	pFont = GetFont();
	pOldFont = dc.SelectObject(pFont);

	DWORD dwStyle = GetStyle(), dwText = 0;

	MAP_STYLE(	SS_RIGHT,			DT_RIGHT					);
	MAP_STYLE(	SS_CENTER,			DT_CENTER					);
	MAP_STYLE(	SS_CENTERIMAGE,		DT_VCENTER | DT_SINGLELINE	);
	MAP_STYLE(	SS_NOPREFIX,		DT_NOPREFIX					);
	MAP_STYLE(	SS_WORDELLIPSIS,	DT_WORD_ELLIPSIS			);
	MAP_STYLE(	SS_ENDELLIPSIS,		DT_END_ELLIPSIS				);
	MAP_STYLE(	SS_PATHELLIPSIS,	DT_PATH_ELLIPSIS			);

	NMAP_STYLE(	SS_LEFTNOWORDWRAP |
				SS_CENTERIMAGE |
				SS_WORDELLIPSIS |
				SS_ENDELLIPSIS |
				SS_PATHELLIPSIS,	DT_WORDBREAK				);

	// Set transparent background
	dc.SetBkMode(TRANSPARENT);

	// Draw the text
	dc.DrawText(szText, client_rect, dwText);

	// Select old font
	dc.SelectObject(pOldFont);
}

LRESULT CTransparentStatic::OnSetText(WPARAM wParam, LPARAM lParam)
{
	SetRedraw(false);
	LRESULT result = Default();
	CRect rect;
	GetWindowRect(&rect);
	SetRedraw(true);
	Invalidate(false);
	GetParent()->ScreenToClient(&rect);
	GetParent()->RedrawWindow(&rect, 0, RDW_INVALIDATE|RDW_UPDATENOW);
	return result;
}
