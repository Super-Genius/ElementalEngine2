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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

GUIListBox::GUIListBox()
{

	GETRENDERERINTERFACEPARAMS renderInterface;

	static DWORD msgHash_GetRendererInterface = CHashString(_T("GetRendererInterface")).GetUniqueID();
	DWORD retval = EngineGetToolBox()->SendMessage(msgHash_GetRendererInterface, 
					sizeof(GETRENDERERINTERFACEPARAMS), 
					&renderInterface, 
					NULL, 
					NULL);

	m_Renderer = renderInterface.m_RendererInterface;

	m_iListCount = 0;
	m_iNumWrappedLines = 0;

	m_iListFontOrder = 0;
	m_FontColor = RGBA_WHITE;

	m_bViewChanged  = false;
	m_bResortList   = false;
	m_iCurrentSelIndex = m_iLastSelIndex = 0;
	m_fMaxOfs = 0.f;
	m_fMaxWidthOfs = 0.f;

	m_bSortingEnabled = false;
	m_bAllowSelection = true;

	m_szBackGndMask = NULL;
	m_BGMask = NULL;

	m_szHighLightMask = NULL;
	m_HLMask = NULL;
	m_HighLightColor = RGBA_YELLOW;

	m_szListFontName = _T("");

	m_iMaxLineWidth = 0;

	m_bWrapText = false;

	m_bVisible = true;

	m_szSelectText =_T("");

	m_iTextOffSetX = 0;
	m_iTextOffSetY = 0;

	m_bEmptyText = false;

	m_pSelElement = NULL;

	m_bIsEnabled = true;

	m_bFileLoaded = false;

	m_pEffect = NULL;
}

GUIListBox::~GUIListBox()
{
	DeInit();
}

void GUIListBox::DeInit()
{
	ResetList();
	
	SAFE_DELETE(m_BGMask);
	SAFE_DELETE(m_BGMask);
}

void GUIListBox::SetFont(const TCHAR *szFontname, int fontsize)
{
	m_szListFontName = szFontname;
	m_iFontsize = fontsize;	
}

void GUIListBox::SetColor(int iFontColor, int iSelectFontColor)
{
	unsigned int normR, normG, normB,
		hlR, hlG, hlB, mask;
	
	mask = 0xff0000;
	normR = (iFontColor & mask) >> 16;
	hlR = (iSelectFontColor & mask) >> 16;
	mask = 0x00ff00;
	normG = (iFontColor & mask) >> 8;
	hlG = (iSelectFontColor & mask) >> 8;
	mask = 0x0000ff;
	normB = (iFontColor & mask);
	hlB = (iSelectFontColor & mask);

	m_FontColor = CColorRef(normR, normG, normB);
	m_HighLightColor = CColorRef(hlR, hlG, hlB);
}

void GUIListBox::SetBackMask(const TCHAR *szBackMask)
{
	m_szBackGndMask = szBackMask;
	SAFE_DELETE(m_BGMask);
	m_BGMask = new CLayeredMask();
	StdString ssBackMask(szBackMask);
	m_BGMask->LoadTexture(ssBackMask);
}

void GUIListBox::SetHighLightMask(const TCHAR *szHighLightMask)
{
	m_szHighLightMask = szHighLightMask;
	SAFE_DELETE(m_HLMask);
	if (szHighLightMask && (_tcscmp(szHighLightMask, _T("")) != 0))
	{
		m_HLMask = new CLayeredMask();
		StdString ssHighLightMask(szHighLightMask);
		m_HLMask->LoadTexture(ssHighLightMask);
	}
}

void GUIListBox::SetTextPos(int iTextOffSetX, int iTextOffSetY)
{
	m_iTextOffSetX = iTextOffSetX;
	m_iTextOffSetY = iTextOffSetY;
}

const TCHAR* GUIListBox::GetSelectedText()
{
	if (m_pSelElement)
	{
		return m_pSelElement->m_szDisplayName;
	}
	else
	{
		return _T("");
	}
}

bool GUIListBox::MouseOverListBox(float fCursorPosX, float fCursorPosY, bool bButtonPressed, bool bButtonRelease, 
								  float xPos, float yPos, float width, float height, DWORD *inState)
{
	SetSelection(*inState);
	int sx = (int)fCursorPosX;
	int sy = (int)fCursorPosY;
	int newIndex = GetPickedIndex(xPos, yPos, sx,sy);
	// check if the picked item is valid
	if (newIndex >= 0)
	{
		if (m_iCurrentSelIndex != newIndex)
		{
			SetSelection(newIndex+1);
			*inState = newIndex+1;
		}
		return true;
	}
	else
	{
		// nothing selected
		SetSelection(0);
		*inState = 0;		
		return false;
	}
}

float GUIListBox::GetMaxHeight()
{
	return (float)((m_iListCount*m_iFontsize) + (m_iTextOffSetY*2));	
}

int GUIListBox::GetPickedIndex(float xPos, float yPos, int sx, int sy)
{
	int i;
	// test all list items for mouse over
	for (i=0;i<m_iListCount;i++)
	{
		ListBoxElement *pElem = m_List[i];
		if (!pElem)
		{
			continue;
		}
		if (pElem->IsInsideBB((sx-m_iTextOffSetX) - (int)xPos, (sy-m_iTextOffSetY) - (int)yPos, 0, 0))
		{
			return i;
		}
	}
	return -1;
}

DWORD GUIListBox::SelectNext()
{
	if (m_iCurrentSelIndex>=m_iListCount-1) return (DWORD)m_iCurrentSelIndex;
	m_iCurrentSelIndex++;
	m_bViewChanged = true;
//	EnsureVisible(m_List[m_iCurrentSelIndex]);
	SetSelection(m_iCurrentSelIndex + 1);
	return (DWORD)m_iCurrentSelIndex+1;
}

DWORD GUIListBox::SelectPrev()
{
	if (m_iCurrentSelIndex<1) return (DWORD)m_iCurrentSelIndex;
	m_iCurrentSelIndex--;
	m_bViewChanged = true;
//	EnsureVisible(m_List[m_iCurrentSelIndex]);
	SetSelection(m_iCurrentSelIndex + 1);
	return (DWORD)m_iCurrentSelIndex+1;
}

void GUIListBox::ResetList()
{
	for( unsigned int i = 0; i < m_List.size(); i++ )
	{
		if( m_List[i] != NULL )
		{
			delete m_List[i];
			m_List[i] = NULL;
		}
	}
	m_List.clear();
	if (m_iListCount)
	{
		m_bViewChanged = true;
	}
	m_iListCount = 0;
	m_iLastSelIndex     = 0;
	m_iCurrentSelIndex = 0;
	m_bFileLoaded = false;
}

ListBoxElement* GUIListBox::AddEntry( const TCHAR *szDisplayName, int lNum, CColorRef iColor, int iSorting)
{
	int index = m_iListCount;
	m_iListCount++;
	ListBoxElement *pNew = new ListBoxElement(szDisplayName, szDisplayName, NULL);

	// strip formatting temporarily
	StdString tempStr = "";
	unsigned int i;
	bool open = false;
	for (i=0; i<_tcslen(szDisplayName); i++)
	{
		if (szDisplayName[i] == '[')
		{
			open = true;
		}
		else if (szDisplayName[i] == ']')
		{
			open = false;
		}
		else 
		{
			if (open == false)
			{
				tempStr += szDisplayName[i];
			}
		}
	}

	int width = 0;
	FONTPIXWIDTHQUERY fpwq;
	CHashString fileName(m_szListFontName);
	fpwq.szFilename = &fileName;
	fpwq.iFontsize = m_iFontsize;
	fpwq.szText = tempStr;
	static DWORD msgHash_GetFontPixWidth = CHashString(_T("GetFontPixWidth")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_GetFontPixWidth, sizeof(FONTPIXWIDTHQUERY), &fpwq);
	width = fpwq.iWidth;

	pNew->m_iBBox[0] = 0;
	pNew->m_iBBox[1] = lNum * m_iFontsize;
	pNew->m_iBBox[2] = width;
	pNew->m_iBBox[3] = pNew->m_iBBox[1] + m_iFontsize;

	m_List.push_back(pNew);
	pNew->SetColor(iColor);
	pNew->SetSortingKey(iSorting);

	m_bResortList   = true;
	m_bViewChanged  = true;

	return pNew;
}

bool GUIListBox::LoadTextFile(const TCHAR *szFilename, int iColor)
{
	CREATEARCHIVE ca;
	ca.mode = 0x00000001;
	static CHashString type = _T("File");
	ca.streamType = &type;
	ca.streamData = (void*) szFilename;
	static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
	DWORD retval = EngineGetToolBox()->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca);
	if ((retval == MSG_NOT_HANDLED) || (retval == MSG_ERROR) || (retval == MSG_WARNING))
	{
		return false;
	}
	
	IArchive *ar = ca.archive;
	StdString szTemp;
	unsigned int numBytes = ar->Read(szTemp);
	ar->Close();
	if (numBytes > 0)
	{
		m_bFileLoaded = true;
		SetTextMultiLine(szTemp, iColor);
		return true;
	}
	else 
	{
		return false;
	}	
}

void GUIListBox::SetTextMultiLine(const TCHAR *szText, int iColor)
{
	StdString szTemp, szInput;
	int lineNum = 0;
	szInput = szText;
	szInput.GetToken(_T("\n"), szTemp);
	while (_tcscmp(szTemp, "") != 0)
	{
		AddEntry(szTemp, lineNum);
		lineNum++;
		szInput.GetToken(_T("\n"), szTemp);
	}
	GenerateStringFromList();
}

void GUIListBox::GenerateStringFromList()
{
	m_BlockForm = "";
	m_BlockForm += "[FColor=";
	m_BlockForm += m_FontColor.ToString();
	m_BlockForm += "]";
	unsigned int i;
	StdString tempStr;
	for (i=0; i<m_List.size(); i++)
	{
		if (m_pSelElement == m_List[i])
		{
			m_BlockForm += "[/FColor]";
			m_BlockForm += "[FColor=";
			m_BlockForm += m_HighLightColor.ToString();
			m_BlockForm += "]";
		}
		m_BlockForm += m_List[i]->m_szDisplayName;
		if( i+1 < m_List.size() )
		{
			m_BlockForm += "\n";
		}
		if (m_pSelElement == m_List[i])
		{
			m_BlockForm += "[/FColor]";
			m_BlockForm += "[FColor=";
			m_BlockForm += m_FontColor.ToString();
			m_BlockForm += "]";
		}
	}
	m_BlockForm += "[/FColor]";
}

void GUIListBox::UpdateFontExt(vector<GUIFONTINFO> &fontData, float xPos, float yPos, float width, float height, DWORD state)
{
	SetSelection(state);
	if (_tcscmp((const TCHAR*)m_szListFontName, _T("")) != 0)
	{
		CHashString hsFontName(m_szListFontName);		
		CHashString hsText(m_BlockForm);

		GUIFONTINFO entry;
		entry.dwPrimaryFontName = hsFontName.GetUniqueID();
		entry.dwSecondaryFontName = hsFontName.GetUniqueID();
		entry.dwText = hsText.GetUniqueID();
		entry.iFontSize = m_iFontsize;
		entry.bCentered = false;
		entry.iXOffset = m_iTextOffSetX;
		entry.iYOffset = m_iTextOffSetY;
		
		RECT rRect, cRect;
		cRect.left = (int)xPos;
		cRect.right = (int)xPos + (int)width;
		cRect.top = (int)yPos;
		cRect.bottom = (int)yPos + (int)height;
		rRect.left = cRect.left;
		rRect.right = cRect.right + 3000;
		rRect.top = cRect.top;
		rRect.bottom = cRect.bottom + 3000;
		
		entry.renderRect = rRect;
		entry.clippingRect = cRect;
		entry.bUseClippingRect = true;

		fontData.push_back(entry);
	}		
}

void GUIListBox::LoadVertexBuffer(vector<GUIVERTEXINFO> &vertexData, float x, float y, float width, float height, DWORD state, 
												float angle, DWORD color)
{
	// should be after the font has loaded
	if (!m_bFileLoaded)
	{
		LoadTextFile(m_szFileName, 0);
	}
	SetSelection(state);
	GUIVERTEXINFO entry;
	entry.iNumPrims = m_BGMask->LoadVertexData(entry.VertexData, x, y, width, height, 
		angle, color, true);
	entry.pTexture = m_BGMask->GetMask();

	vertexData.push_back(entry);	

	if ((m_HLMask) && (m_pSelElement))
	{
		float maskY, maskHeight;
		maskY = (m_iFontsize * m_iCurrentSelIndex) + y + m_iTextOffSetY;
		maskHeight = (float)m_iFontsize;

		entry.iNumPrims = m_HLMask->LoadVertexData(entry.VertexData, x, maskY, width, maskHeight, 
			angle, color, true);
		entry.pTexture = m_HLMask->GetMask();

		vertexData.push_back(entry);	
	}
}

void GUIListBox::SetSelection(DWORD inState)
{
	if (inState > 0)
	{
		m_iCurrentSelIndex = inState -1;
		m_pSelElement = m_List[m_iCurrentSelIndex];
		GenerateStringFromList();
	}
	else
	{
		m_iCurrentSelIndex = -1;
		m_pSelElement = NULL;
		GenerateStringFromList();
	}
}

DWORD GUIListBox::SetSelectedText(const TCHAR* text)
{
	for (UINT i=0; i<m_List.size(); i++)
	{
		if (_tcscmp(text, m_List[i]->m_szDisplayName) == 0)
		{
			return i+1;
		}
	}

	return 0;
}