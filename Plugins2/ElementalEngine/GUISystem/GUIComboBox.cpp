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

enum COMBOBOXSTATE { COMBO_NORM_STATE=0, COMBO_DROPDOWN_STATE };

GUIComboBox::GUIComboBox()
{
	m_szFontName = _T("");
	m_szListFile = _T("");
	m_iFontsize = 0;

	m_iTextOffSetX = m_iTextOffSetY = 0;

	m_szDisplayText = _T("");
	m_iIndex = 0;

	// grab the render interface
	GETRENDERERINTERFACEPARAMS renderInterface;

	static DWORD msgHash_GetRendererInterface = CHashString(_T("GetRendererInterface")).GetUniqueID();
	DWORD retval = EngineGetToolBox()->SendMessage(msgHash_GetRendererInterface, 
					sizeof(GETRENDERERINTERFACEPARAMS), 
					&renderInterface, 
					NULL, 
					NULL);

	m_Renderer = renderInterface.m_RendererInterface;

	m_bButtonOnRight = false;			

	m_iListTextOffSetX = 0;
	m_iListTextOffSetY = 0;	
	m_iBoxOffsetY = 0;
	m_iBoxHeightOffset = 0;
}

GUIComboBox::~GUIComboBox()
{}

void GUIComboBox::SetFont(const TCHAR *szFontName, int fontsize)
{
	m_szFontName = szFontName;
	m_iFontsize = fontsize;

	m_SelectBox.SetText(m_szFontName, _T(""), _T(""), _T(""), _T(""), _T(""), m_iFontsize);
	
	m_DropDownBox.SetFont(szFontName, m_iFontsize);			
}

void GUIComboBox::SetDefaultText(const TCHAR *szDisplayText)
{
	m_szDisplayText = szDisplayText;
	m_SelectBox.SetText(m_szFontName, _T(""), szDisplayText, szDisplayText, szDisplayText, szDisplayText, m_iFontsize);
	m_iIndex = 0;	
}

void GUIComboBox::SetSelectBox(const TCHAR *szNorm, const TCHAR *szHigh, const TCHAR *szSelect, const TCHAR *szGray) 
{
	m_SelectBox.SetScreenMasks(szNorm, szHigh, szSelect, szGray);
	m_SelectBox.SetSmartScale(true);
	
	m_SelectBox.SetTextPos(m_iTextOffSetX, m_iTextOffSetY);
}

void GUIComboBox::SetDropDownBox(const TCHAR * szNorm, const TCHAR * szFileName, int yOffset, int heightOffset, 
		int textXOffset, int textYOffset)
{
	m_DropDownBox.WrapText(false);
	m_DropDownBox.SetBackMask(szNorm);
	m_DropDownBox.SetVisible(false);

	m_iListTextOffSetX = textXOffset;
	m_iListTextOffSetY = textYOffset;

	m_iBoxHeightOffset = heightOffset;
	m_DropDownBox.SetTextPos(m_iListTextOffSetX, m_iListTextOffSetY);

	m_szListFile = szFileName;
	if (_tcscmp(m_szListFile, _T("")) != 0)
	{
		m_DropDownBox.ResetList();
		m_DropDownBox.SetFileName(m_szListFile);
	}	
	
	m_iBoxOffsetY = yOffset;
}

void GUIComboBox::SetArrowBox(const TCHAR *szNorm, const TCHAR *szSelect, const TCHAR *szGray)
{
	m_Arrow.SetScreenMasks(szNorm, szNorm, szSelect, szGray);
	m_Arrow.SetSmartScale(true);
}

void GUIComboBox::SetTextPos(int iTextOffSetX, int iTextOffSetY)
{
	m_iTextOffSetX = iTextOffSetX;
	m_iTextOffSetY = iTextOffSetY;
}

DWORD GUIComboBox::MouseOver(float fMouseCursorX, float fMouseCursorY, bool bButtonPressed, bool bButtonRelease, 
		float xPos, float yPos, float width, float height, DWORD lastState)
{
	DWORD arrowState, selectState, listState, curState;
	curState = UnpackState(lastState, arrowState, selectState, listState);

	float arrowX, arrowWidth, selectX, selectWidth;

	arrowWidth = height;
	if (width*2.0f < height)
	{
		arrowWidth = width / 2.0f;
	}

	selectWidth = width - arrowWidth;

	if (m_bButtonOnRight)
	{
		arrowX = xPos + selectWidth;
		selectX = xPos;
	}
	else
	{
		arrowX = xPos;
		selectX = xPos + arrowWidth;
	}

	if (!bButtonPressed && !bButtonRelease)
	{
		m_Arrow.MouseOver(fMouseCursorX, fMouseCursorY, arrowX, yPos, arrowWidth, height, &arrowState);
		
		m_SelectBox.MouseOver(fMouseCursorX, fMouseCursorY, selectX, yPos, selectWidth, height, &selectState);
		
		if (curState == COMBO_DROPDOWN_STATE)
		{
			m_DropDownBox.MouseOverListBox(fMouseCursorX, fMouseCursorY, false, false, selectX, yPos + height, 
				selectWidth, m_DropDownBox.GetMaxHeight(), &listState);			
		}
	}
	else if(bButtonPressed)
	{
		m_Arrow.MouseButton(fMouseCursorX, fMouseCursorY, true, false, arrowX, yPos, arrowWidth, height, &arrowState);
		
		m_SelectBox.MouseButton(fMouseCursorX, fMouseCursorY, true, false, selectX, yPos, selectWidth, height, &selectState);
	}
	else // button released
	{
		bool arrowClicked = m_Arrow.MouseButton(fMouseCursorX, fMouseCursorY, false, true, arrowX, yPos, arrowWidth, height, &arrowState);
		
		m_SelectBox.MouseButton(fMouseCursorX, fMouseCursorY, false, true, selectX, yPos, selectWidth, height, &selectState);
		
		if (arrowClicked)
		{
			if (curState == COMBO_DROPDOWN_STATE)
			{
				curState = COMBO_NORM_STATE;
			}
			else
			{
				curState = COMBO_DROPDOWN_STATE;
			}
		}
		else
		{
			if (curState == COMBO_DROPDOWN_STATE)
			{
				bool selected = m_DropDownBox.MouseOverListBox(fMouseCursorX, fMouseCursorY, false, true, selectX, yPos + height,
					selectWidth, m_DropDownBox.GetMaxHeight(), &listState);

				if (selected)
				{
					curState = COMBO_NORM_STATE;
					StdString selectedText = m_DropDownBox.GetSelectedText();
					SetDefaultText(m_DropDownBox.GetSelectedText());
				}
			}
		}
	}	

	return PackStates(curState, arrowState, selectState, listState);
}

void GUIComboBox::SetTextColors(int iNormColor, int iHighColor, int iSelectColor, int iGrayColor)
{
	m_SelectBox.SetTextColors(iNormColor, iNormColor, iNormColor, iGrayColor);
	
	m_DropDownBox.SetColor(iNormColor, iSelectColor);	
}

const TCHAR* GUIComboBox::GetSelectedText()
{
	return m_szDisplayText;
}

int GUIComboBox::GetSelectedIndex()
{
	return m_iIndex;	
}


void GUIComboBox::AddEntry(const TCHAR* szDisplayName, int iColor)
{
	m_DropDownBox.AddEntry(szDisplayName, iColor, 0);	
}

void GUIComboBox::Reset()
{
	m_DropDownBox.ResetList();
	m_iIndex = 0;
	m_szDisplayText = _T("");
	m_SelectBox.SetText(m_szFontName, _T(""), _T(""), _T(""), _T(""), _T(""), m_iFontsize);	
}

void GUIComboBox::LoadVertexBuffer(vector<GUIVERTEXINFO> &vertexData, float x, float y, float width, 
								   float height, UINT state, float angle, DWORD color)
{
	DWORD arrowState, selectState, listState, curState;
	curState = UnpackState(state, arrowState, selectState, listState);
	float arrowWidth = height;
	if (width < height * 2.0f)
	{
		arrowWidth = width / 2.0f;
	}
		
	float selectWidth = width - arrowWidth;
	if (!m_bButtonOnRight)
	{
		m_Arrow.LoadVertexBuffer(vertexData, x, y, arrowWidth, height, arrowState, angle, color);	
		m_SelectBox.LoadVertexBuffer(vertexData, x + arrowWidth, y, selectWidth, height, selectState, angle, color);	

		if (curState == COMBO_DROPDOWN_STATE)
		{
			m_DropDownBox.LoadVertexBuffer(vertexData, x + arrowWidth, y + height + m_iBoxOffsetY, selectWidth, m_DropDownBox.GetMaxHeight() + m_iBoxHeightOffset, listState, angle, color);
		}
	}
	else
	{
		m_Arrow.LoadVertexBuffer(vertexData, x + selectWidth, y, arrowWidth, height, arrowState, angle, color);	
		m_SelectBox.LoadVertexBuffer(vertexData, x, y, selectWidth, height, selectState, angle, color);	

		if (curState == COMBO_DROPDOWN_STATE)
		{
			m_DropDownBox.LoadVertexBuffer(vertexData, x, y + height + m_iBoxOffsetY, selectWidth, m_DropDownBox.GetMaxHeight() + m_iBoxHeightOffset, listState, angle, color);	
		}
	}
}

DWORD GUIComboBox::UnpackState(DWORD state, DWORD &arrowState, DWORD &selectState, DWORD &listState)
{
	DWORD outState = state >> 28;
	listState = (state >> 8) & 0xfffff;
	arrowState = (state >> 4) & 0xf;
	selectState = state & 0xf;
	return outState;
}
	
DWORD GUIComboBox::PackStates(DWORD curState, DWORD arrowState, DWORD selectState, DWORD listState)
{
	DWORD retState = 0;
	retState += curState << 28;
	retState += (listState & 0xfffff) << 8;	
	retState += (arrowState << 4);
	retState += selectState;	
	return retState;
}

void GUIComboBox::UpdateFontExt(vector<GUIFONTINFO> &fontData, float xPos, float yPos, float width, 
								float height, DWORD state)
{
	DWORD arrowState, selectState, listState, curState;
	curState = UnpackState(state, arrowState, selectState, listState);

	m_DropDownBox.SetSelection(listState);
	SetDefaultText(m_DropDownBox.GetSelectedText());
	
	float arrowWidth = height;
	if (width < height * 2.0f)
	{
		arrowWidth = width / 2.0f;
	}
		
	float selectWidth = width - arrowWidth;
	if (!m_bButtonOnRight)
	{
		m_SelectBox.UpdateFontExt(fontData, xPos + arrowWidth, yPos, selectWidth, height, selectState);

		if (curState == COMBO_DROPDOWN_STATE)
		{
			m_DropDownBox.UpdateFontExt(fontData, xPos + arrowWidth, yPos + height + m_iBoxOffsetY, selectWidth, m_DropDownBox.GetMaxHeight() + m_iBoxHeightOffset, listState);
		}		
	}
	else
	{
		m_SelectBox.UpdateFontExt(fontData, xPos, yPos, selectWidth, height, selectState);

		if (curState == COMBO_DROPDOWN_STATE)
		{
			m_DropDownBox.UpdateFontExt(fontData, xPos, yPos + height + m_iBoxOffsetY, selectWidth, m_DropDownBox.GetMaxHeight() + m_iBoxHeightOffset, listState);
		}		
	}
}

DWORD GUIComboBox::ToggleBack(DWORD lastState)
{
	DWORD arrowState, selectState, listState, curState;
	curState = UnpackState(lastState, arrowState, selectState, listState);	

	if (curState == COMBO_DROPDOWN_STATE)
	{
		curState = COMBO_NORM_STATE;
	}

	return PackStates(curState, arrowState, selectState, listState);
}
	
DWORD GUIComboBox::ToggleSelect(DWORD lastState)
{
	DWORD arrowState, selectState, listState, curState;
	curState = UnpackState(lastState, arrowState, selectState, listState);
	
	if (curState == COMBO_DROPDOWN_STATE)
	{
		curState = COMBO_NORM_STATE;
	}
	else
	{
		curState = COMBO_DROPDOWN_STATE;
		if (m_DropDownBox.GetItemCount() > 0)
		{
			listState = 1;
		}
	}

	return PackStates(curState, arrowState, selectState, listState);
}
	
DWORD GUIComboBox::ToggleUp(DWORD lastState)
{
	DWORD arrowState, selectState, listState, curState;
	curState = UnpackState(lastState, arrowState, selectState, listState);
	if (curState == COMBO_DROPDOWN_STATE)
	{
		m_DropDownBox.SetSelection(listState);
		listState = m_DropDownBox.SelectPrev();
	}
	else
	{
		curState = COMBO_DROPDOWN_STATE;
		if (m_DropDownBox.GetItemCount() > 0)
		{
			m_DropDownBox.SetSelection(1); // select first element
			listState = 1;
		}
	}

	return PackStates(curState, arrowState, selectState, listState);
}
	
DWORD GUIComboBox::ToggleDown(DWORD lastState)
{
	DWORD arrowState, selectState, listState, curState;
	curState = UnpackState(lastState, arrowState, selectState, listState);

	if (curState == COMBO_DROPDOWN_STATE)
	{
		m_DropDownBox.SetSelection(listState);
		listState = m_DropDownBox.SelectNext();
	}
	else
	{
		curState = COMBO_DROPDOWN_STATE;
		if (m_DropDownBox.GetItemCount() > 0)
		{
			m_DropDownBox.SetSelection(1); // select first element
			listState = 1;
		}
	}

	return PackStates(curState, arrowState, selectState, listState);
}

DWORD GUIComboBox::GetHashText(DWORD inState)
{
	DWORD arrowState, selectState, listState, curState;
	curState = UnpackState(inState, arrowState, selectState, listState);

	m_DropDownBox.SetSelection(listState);
	CHashString hashText(m_DropDownBox.GetSelectedText());
	
	return hashText.GetUniqueID();
}

DWORD GUIComboBox::SetHashText(DWORD inState, DWORD hashText)
{
	DWORD arrowState, selectState, listState, curState;
	curState = UnpackState(inState, arrowState, selectState, listState);

	const TCHAR* inText = EngineGetToolBox()->GetHashString(hashText);
    listState = m_DropDownBox.SetSelectedText(inText);
	
	return PackStates(curState, arrowState, selectState, listState);
}