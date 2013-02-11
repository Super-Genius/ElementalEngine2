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

#include "SString.h"
#include "SciLexer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLuaEditCtrl

CLuaEditCtrl::CLuaEditCtrl()
{
	m_bShowCalltips = TRUE;
}

CLuaEditCtrl::~CLuaEditCtrl()
{
}


BEGIN_MESSAGE_MAP(CLuaEditCtrl, CWnd)
	//{{AFX_MSG_MAP(CLuaEditCtrl)
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CLuaEditCtrl message handlers

BOOL CLuaEditCtrl::Create(CWnd *pParentWnd, UINT nCtrlId)
{
	BOOL bCreated = CreateEx(0, "Scintilla","", WS_CHILD|WS_VISIBLE|WS_TABSTOP,
		CRect(0,0,1,1),pParentWnd,nCtrlId);

	if ( !bCreated )
		return FALSE;

	m_fnScintilla = (int (__cdecl *)(void *,int,int,int))
		SendMessage(SCI_GETDIRECTFUNCTION,0,0);
	m_ptrScintilla = (void *)SendMessage(SCI_GETDIRECTPOINTER,0,0);

	Sci(SCI_SETMARGINWIDTHN, 1, 0);

	return TRUE;
}

int CLuaEditCtrl::Sci(int nCmd, int wParam, int lParam)
{
	ASSERT(m_fnScintilla);
	ASSERT(m_ptrScintilla);

	return m_fnScintilla(m_ptrScintilla, nCmd, wParam, lParam);
}


int CLuaEditCtrl::LineFromPoint(CPoint &pt)
{
	return 1+Sci(SCI_LINEFROMPOSITION, Sci(SCI_POSITIONFROMPOINT, pt.x, pt.y), 0);
}

BOOL CLuaEditCtrl::ToggleBreakpoint(int nLine)
{
	if ( Sci(SCI_MARKERGET, nLine-1) & 1 )
	{
		Sci(SCI_MARKERDELETE, nLine-1, 0);
		return FALSE;
	}
	else

	Sci(SCI_MARKERADD, nLine-1, 0);
	return TRUE;
}

BOOL CLuaEditCtrl::Load(CFile* pFile)
{
	const int blockSize = 131072;

	Sci(SCI_CLEARALL);

	char data[blockSize];
	size_t lenFile = pFile->Read(data, sizeof(data));
	while (lenFile > 0) 
	{
		Sci(SCI_ADDTEXT, lenFile, (long)data);
		lenFile = pFile->Read(data, sizeof(data));
	}

	Sci(SCI_SETEOLMODE, SC_EOL_CRLF);
	Sci(SCI_SETUNDOCOLLECTION, 1);
	Sci(SCI_SETSAVEPOINT);
	Sci(SCI_GOTOPOS, 0);

	return TRUE;
}

BOOL CLuaEditCtrl::Load(char *loadData, int size)
{
	Sci(SCI_CLEARALL);

	Sci(SCI_ADDTEXT, size, (long)loadData);

	Sci(SCI_SETEOLMODE, SC_EOL_CRLF);
	Sci(SCI_SETUNDOCOLLECTION, 1);
	Sci(SCI_SETSAVEPOINT);
	Sci(SCI_GOTOPOS, 0);

	return TRUE;
}

BOOL CLuaEditCtrl::Save(CFile* pFile)
{
	const int blockSize = 131072;

	TextRange tr;
	char data[blockSize + 1];
	int lengthDoc = Sci(SCI_GETLENGTH);
	for (int i = 0; i < lengthDoc; i += blockSize) 
	{
		int grabSize = lengthDoc - i;
		if (grabSize > blockSize)
			grabSize = blockSize;

		tr.chrg.cpMin = i;
		tr.chrg.cpMax = i + grabSize;
		tr.lpstrText = data;
		Sci(SCI_GETTEXTRANGE, 0, long(&tr));
	
		pFile->Write(data, grabSize);
	}

	Sci(SCI_SETSAVEPOINT);

	return TRUE;	  
}

int CLuaEditCtrl::GetDataSize()
{
	return Sci(SCI_GETLENGTH);
}

void CLuaEditCtrl::GetData(char *data)
{
	const int blockSize = 131072;

	TextRange tr;
	int lengthDoc = Sci(SCI_GETLENGTH);
	for (int i = 0; i < lengthDoc; i += blockSize) 
	{
		int grabSize = lengthDoc - i;
		if (grabSize > blockSize)
			grabSize = blockSize;

		tr.chrg.cpMin = i;
		tr.chrg.cpMax = i + grabSize;
		tr.lpstrText = data;
		Sci(SCI_GETTEXTRANGE, 0, long(&tr));
	}
}

void CLuaEditCtrl::SetSavePoint()
{
	Sci(SCI_SETSAVEPOINT);
}

void CLuaEditCtrl::ClearAllBreakpoints()
{
	Sci(SCI_MARKERDELETEALL, 0);
}

void CLuaEditCtrl::SetBreakpoint(int nLine)
{
	Sci(SCI_MARKERADD, nLine-1, 0);
}

void CLuaEditCtrl::GotoLine(int nLine)
{
	Sci(SCI_GOTOLINE, nLine-1);

	Sci(SCI_MARKERDELETEALL, 1);
	Sci(SCI_MARKERADD, nLine-1, 1);
}

void CLuaEditCtrl::ClearMarkers()
{
	Sci(SCI_MARKERDELETEALL, 1);
}


void CLuaEditCtrl::SetBreakPointsIn(CLuaDoc *pDoc)
{
	pDoc->RemoveAllBreakPoints();

	int nLine = Sci(SCI_MARKERNEXT, 0, 1);
	while ( nLine>=0 )
	{
		pDoc->AddBreakPoint(nLine+1); // from 0-based to 1-based 
		nLine = Sci(SCI_MARKERNEXT, nLine+1, 1);
	}	
}

BOOL CLuaEditCtrl::CanUndo()
{
	return Sci(SCI_CANUNDO);
}

void CLuaEditCtrl::Undo()
{
	Sci(SCI_UNDO);
}

BOOL CLuaEditCtrl::CanRedo()
{
	return Sci(SCI_CANREDO);
}

void CLuaEditCtrl::Redo()
{
	Sci(SCI_REDO);
}

void CLuaEditCtrl::SelectAll()
{
	Sci(SCI_SELECTALL);
}

BOOL CLuaEditCtrl::CanCutOrClear()
{
	int currentPos = Sci(SCI_GETCURRENTPOS);
	int anchor = Sci(SCI_GETANCHOR);

	return currentPos != anchor;
}

void CLuaEditCtrl::Cut()
{
	Sci(SCI_CUT);
}

void CLuaEditCtrl::Clear()
{
	Sci(SCI_CLEAR);
}

BOOL CLuaEditCtrl::CanPaste()
{
	return Sci(SCI_CANPASTE);
}

void CLuaEditCtrl::Paste()
{
	Sci(SCI_PASTE);
}

void CLuaEditCtrl::Copy()
{
	Sci(SCI_COPY);
}

void CLuaEditCtrl::GrabFocus()
{
	Sci(SCI_GRABFOCUS);
}

void CLuaEditCtrl::SetEditorMargins()
{
	Sci(SCI_SETMARGINTYPEN, 0, SC_MARGIN_NUMBER);
	int pixelWidth = 6 * Sci(SCI_TEXTWIDTH, STYLE_LINENUMBER, (int)"9");
	Sci(SCI_SETMARGINWIDTHN, 0, pixelWidth);

	Sci(SCI_SETMARGINTYPEN, 1, SC_MARGIN_SYMBOL);
	Sci(SCI_SETMARGINWIDTHN, 1, 10);
	Sci(SCI_SETMARGINSENSITIVEN, 1, TRUE);

	Sci(SCI_MARKERDEFINE, 0, SC_MARK_CIRCLE);
	Sci(SCI_MARKERSETFORE, 0, RGB(0xff, 0x00, 0x00));
	Sci(SCI_MARKERSETBACK, 0, RGB(0xff, 0x00, 0x00));

	Sci(SCI_MARKERDEFINE, 1, SC_MARK_ARROW);
}

void CLuaEditCtrl::SetCallStackMargins()
{
	Sci(SCI_SETMARGINTYPEN, 1, SC_MARGIN_SYMBOL);
	Sci(SCI_SETMARGINWIDTHN, 1, 10);
	Sci(SCI_SETMARGINSENSITIVEN, 1, FALSE);

	Sci(SCI_MARKERDEFINE, 0, SC_MARK_ARROW);
}

void CLuaEditCtrl::SetReadOnly(BOOL bReadOnly)
{
	Sci(SCI_SETREADONLY, bReadOnly);
}

void CLuaEditCtrl::AddText(const char* szText)
{
	Sci(SCI_ADDTEXT, strlen(szText), (int)szText);
}

void CLuaEditCtrl::ClearAll()
{
	Sci(SCI_CLEARALL);
}

CString CLuaEditCtrl::GetLine(int nLine)
{
	CString strLine;
	int nLineLength = Sci(SCI_LINELENGTH, nLine-1);
	if ( nLineLength>0 )
	{
		Sci(SCI_GETLINE, nLine-1, (int)strLine.GetBuffer(nLineLength));
		strLine.ReleaseBuffer();
	}

	return strLine;
}

void CLuaEditCtrl::GotoLastLine()
{
	int nLine = Sci(SCI_GETLINECOUNT);
	Sci(SCI_GOTOLINE, nLine);
}

int CLuaEditCtrl::GetCurrentLine()
{
	return Sci(SCI_LINEFROMPOSITION, Sci(SCI_GETCURRENTPOS)) + 1;

}

void CLuaEditCtrl::SetStackTraceLevel(int nLevel)
{
	Sci(SCI_MARKERDELETEALL, 0);
	Sci(SCI_MARKERADD, nLevel, 0);
}

CharacterRange CLuaEditCtrl::GetSelection() 
{
	CharacterRange crange;
	crange.cpMin = Sci(SCI_GETSELECTIONSTART);
	crange.cpMax = Sci(SCI_GETSELECTIONEND);
	return crange;
}

BOOL CLuaEditCtrl::PreparePrint(CDC *pDC, CPrintInfo *pInfo)
{
	CharacterRange crange = GetSelection();
	int startPos = crange.cpMin;
	int endPos = crange.cpMax;

	LONG lengthDoc = Sci(SCI_GETLENGTH);
	LONG lengthPrinted = 0;
	LONG lengthDocMax = lengthDoc;

	// Requested to print selection
	if (pInfo->m_pPD->m_pd.Flags & PD_SELECTION) {
		if (startPos > endPos) {
			lengthPrinted = endPos;
			lengthDoc = startPos;
		} else {
			lengthPrinted = startPos;
			lengthDoc = endPos;
		}

		if (lengthPrinted < 0)
			lengthPrinted = 0;
		if (lengthDoc > lengthDocMax)
			lengthDoc = lengthDocMax;
	}

	Sci(SCI_SETWRAPMODE, SC_WRAP_WORD);

	m_pages.RemoveAll();

	RangeToFormat frPrint;
	frPrint.hdc = pDC->GetSafeHdc();
	frPrint.hdcTarget = pDC->m_hAttribDC;
	frPrint.rcPage.left		= frPrint.rc.left	= 0;
	frPrint.rcPage.right	= frPrint.rc.right	= pDC->GetDeviceCaps(HORZRES);
	frPrint.rcPage.top		= frPrint.rc.top	= 0;
	frPrint.rcPage.bottom	= frPrint.rc.bottom = pDC->GetDeviceCaps(VERTRES);

	while (lengthPrinted < lengthDoc) {
		frPrint.chrg.cpMin = lengthPrinted;
		frPrint.chrg.cpMax = lengthDoc;

		m_pages.Add(lengthPrinted);

		lengthPrinted = Sci(SCI_FORMATRANGE, FALSE,
		                           reinterpret_cast<LPARAM>(&frPrint));
	}

	Sci(SCI_FORMATRANGE, FALSE, 0);

	pInfo->SetMaxPage(m_pages.GetSize());

	return TRUE;
}

void CLuaEditCtrl::PrintPage(CDC* pDC, CPrintInfo* pInfo)
{
	RangeToFormat frPrint;
	frPrint.hdc = pDC->GetSafeHdc();
	frPrint.hdcTarget = pDC->m_hAttribDC;
	frPrint.rc.left = pInfo->m_rectDraw.left;
	frPrint.rc.right = pInfo->m_rectDraw.right;
	frPrint.rc.top = pInfo->m_rectDraw.top;
	frPrint.rc.bottom = pInfo->m_rectDraw.bottom;
	frPrint.rcPage.left = pInfo->m_rectDraw.left;
	frPrint.rcPage.right = pInfo->m_rectDraw.right;
	frPrint.rcPage.top = pInfo->m_rectDraw.top;
	frPrint.rcPage.bottom = pInfo->m_rectDraw.bottom;

	frPrint.chrg.cpMin = m_pages[pInfo->m_nCurPage - 1];
	frPrint.chrg.cpMax = Sci(SCI_GETLENGTH);

	Sci(SCI_FORMATRANGE, TRUE, reinterpret_cast<LPARAM>(&frPrint));
}

void CLuaEditCtrl::EndPrint(CDC *pDC, CPrintInfo *pInfo)
{
	Sci(SCI_SETWRAPMODE, SC_WRAP_NONE);
}


void CLuaEditCtrl::SetLuaLexer()
{
   const char font[] = "Verdana";
   const char monospace[] = "Courier";
   const short fontsize = 9;
   const char keywords[] = "and break do else elseif end false for function global if in local nil not or repeat return then true until while";

   // set style bits, choose the right lexer (Lua) and set the keywords list
   Sci(SCI_SETSTYLEBITS,5,0);
   Sci(SCI_SETLEXER,SCLEX_LUA,0);
   Sci(SCI_SETKEYWORDS,0,(LPARAM)keywords);
   
   // set up basic features (iguides on, tab=3, tabs-to-spaces, EOL=CRLF)
   Sci(SCI_SETINDENTATIONGUIDES,1,0);
   Sci(SCI_SETTABWIDTH,3,0);
   Sci(SCI_SETUSETABS,0,0);
   Sci(SCI_SETEOLMODE,SC_EOL_CRLF,0);

   // now set up the styles (remember you have to set up font name for each style;
   // if you fail to do so, bold/italics will not work (only color will work)
   // !!colors are in format BGR!!

   // style 32: default
   Sci(SCI_STYLESETFONT,32, (LPARAM) font);
   Sci(SCI_STYLESETSIZE,32, fontsize);
   // style 0: whitespace
   Sci(SCI_STYLESETFORE,0, 0x808080);
   // style 1: comment (not used in Lua)
   // style 2: line comment (green)
   Sci(SCI_STYLESETFONT,2, (int)monospace);
   Sci(SCI_STYLESETSIZE,2, fontsize);
   Sci(SCI_STYLESETFORE,2, 0x00AA00);
   // style 3: doc comment (grey???)
   Sci(SCI_STYLESETFORE,3, 0x7F7F7F);      
   // style 4: numbers (blue)
   Sci(SCI_STYLESETFORE,4, 0xFF0000);
   // style 5: keywords (black bold)
   Sci(SCI_STYLESETFONT,5, (int)font);
   Sci(SCI_STYLESETSIZE,5, (int)fontsize);
   Sci(SCI_STYLESETFORE,5, 0x000000);
   Sci(SCI_STYLESETBOLD,5, 1);
   // style 6: double qouted strings (???)
   Sci(SCI_STYLESETFORE,6, 0x7F007F);
   // style 7: single quoted strings (???)
   Sci(SCI_STYLESETFORE,7, 0x7F007F);
   // style 8: UUIDs (IDL only, not used in Lua)
   // style 9: preprocessor directives (not used in Lua 4)
   // style 10: operators (black bold)
   Sci(SCI_STYLESETFONT,10, (int)font);
   Sci(SCI_STYLESETSIZE,10, fontsize);
   Sci(SCI_STYLESETFORE,10, 0x000000);
   Sci(SCI_STYLESETBOLD,10, 1);
   // style 11: identifiers (leave to default)
   // style 12: end of line where string is not closed (black on violet, eol-filled)
   Sci(SCI_STYLESETFORE,12, 0x000000);
   Sci(SCI_STYLESETBACK,12, 0xE0C0E0);
   Sci(SCI_STYLESETEOLFILLED,12, 1);
}


void CLuaEditCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CMDIFrameWnd *frameWnd = DYNAMIC_DOWNCAST(CMDIFrameWnd, AfxGetMainWnd());
	CLuaFrame *pFrame = DYNAMIC_DOWNCAST(CLuaFrame, frameWnd->MDIGetActive());

	if (m_bShowCalltips && pFrame != NULL && pFrame->IsDebugging())
	{
		char  linebuf[1000];
		int  pos  =  Sci(SCI_POSITIONFROMPOINT, point.x, point.y);
		int start = Sci(SCI_WORDSTARTPOSITION, pos, TRUE);
		int end = Sci(SCI_WORDENDPOSITION, pos, TRUE);
		TextRange tr;
		tr.chrg.cpMin = start;
		tr.chrg.cpMax = end;
		tr.lpstrText = linebuf;
		Sci(SCI_GETTEXTRANGE, 0, long(&tr));
		
		CString strCalltip;
		CDebugger *pDebugger = pFrame->GetDebugger();

		if ( pDebugger->GetCalltip(linebuf, strCalltip.GetBuffer(1000)) )
		{
			if  (Sci(SCI_CALLTIPACTIVE) && m_strCallTip!=strCalltip)
			{
				Sci(SCI_CALLTIPCANCEL);
			}

			if (!Sci(SCI_CALLTIPACTIVE))
			{
				Sci(SCI_CALLTIPSHOW,  start,  (int)strCalltip.GetBuffer(0));
				strCalltip.ReleaseBuffer();
				m_strCallTip = strCalltip;
			}
		}
		else if (Sci(SCI_CALLTIPACTIVE))
		{
			Sci(SCI_CALLTIPCANCEL);
		}
	}
	else if (Sci(SCI_CALLTIPACTIVE))
	{
		Sci(SCI_CALLTIPCANCEL);
	}

	CWnd::OnMouseMove(nFlags, point);
}
