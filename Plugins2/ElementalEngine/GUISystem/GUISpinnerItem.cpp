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

GUISpinnerItem::GUISpinnerItem() 
{

	m_pEditBox = NULL;
	m_pUpArrow = NULL;
	m_pDownArrow = NULL;

	m_bIsEnabled = true;
	m_bVisible = true;
	m_bFocus = false;
	
	// grab the render interface
	GETRENDERERINTERFACEPARAMS renderInterface;

	static DWORD msgHash_GetRendererInterface = CHashString(_T("GetRendererInterface")).GetUniqueID();
	DWORD retval = EngineGetToolBox()->SendMessage(msgHash_GetRendererInterface, 
					sizeof(GETRENDERERINTERFACEPARAMS), 
					&renderInterface, 
					NULL, 
					NULL);

	m_Renderer = renderInterface.m_RendererInterface;


	m_fEditValue = 0.f;
	m_iEditValue = 0;

	m_fIncrement = 0.f;
	m_fMinRange = 0.f;
	m_fMaxRange = 0.f;

	m_iTextOffSetX = 0;
	m_iTextOffSetY = 0;
	m_iFontsize = 0;

	m_NumType = SPINNER_INT;

	m_bButtonsOnRight = false;

	m_HashBias = 0xd00f0000;
	
	m_iDefaultVal = 0;
	m_fDefaultVal = 0.0f;
}

GUISpinnerItem::~GUISpinnerItem()
{
	DeInit();
}

void GUISpinnerItem::DeInit()
{
	SAFE_DELETE(m_pEditBox);
	SAFE_DELETE(m_pUpArrow);
	SAFE_DELETE(m_pDownArrow);
}


void GUISpinnerItem::SetTextColors(int iColorNorm, int iColorGray)
{
	if (m_pEditBox)
	{
		m_pEditBox->SetTextColors(iColorNorm, iColorNorm, iColorNorm, iColorGray);
	}
}

void GUISpinnerItem::SetFont(const TCHAR *szFontName, int fontsize)
{
	m_szFontName = szFontName;
	m_iFontsize = fontsize;

	if (m_pEditBox)
	{
		m_pEditBox->SetText(m_szFontName, _T(""), _T(""), _T(""), _T(""), _T(""), fontsize);			
	}
}

void GUISpinnerItem::SetEditInt(int iNum)
{
	m_NumType = SPINNER_INT;
	m_iEditValue = iNum;
	SetEditText();
}

void GUISpinnerItem::SetEditFloat(float fNum)
{
	m_NumType = SPINNER_FLOAT;
	m_fEditValue = fNum;
	SetEditText();
}

void GUISpinnerItem::SetIncrementValue(float fInc)
{
	m_fIncrement = fInc;
}

void GUISpinnerItem::SetRange(float fMin, float fMax)
{
	m_fMinRange = fMin;
	m_fMaxRange = fMax;
}

void GUISpinnerItem::SetEditBox(const TCHAR *szNorm, const TCHAR *szGray, int iCursorColor)
{
	SAFE_DELETE(m_pEditBox);

	m_pEditBox = new GUIButton();
	m_pEditBox->SetScreenMasks(szNorm, szNorm, szNorm, szGray);
	m_pEditBox->SetTextPos(m_iTextOffSetX, m_iTextOffSetY);	
	m_pEditBox->SetSmartScale(true);
}

void GUISpinnerItem::SetUpArrowBox(const TCHAR *szNorm, const TCHAR* szSel, const TCHAR *szGray)
{
	SAFE_DELETE(m_pUpArrow);

	m_pUpArrow = new GUIButton();

	m_pUpArrow->SetScreenMasks(szNorm, szNorm, szSel, szGray);
	m_pUpArrow->SetSmartScale(false); // turn it on?	
}

void GUISpinnerItem::SetDownArrowBox(const TCHAR *szNorm, const TCHAR *szSel, const TCHAR *szGray)
{
	SAFE_DELETE(m_pDownArrow);

	m_pDownArrow = new GUIButton();

	m_pDownArrow->SetScreenMasks(szNorm, szNorm, szSel, szGray);
	m_pDownArrow->SetSmartScale(false); // turn it on?	
}

void GUISpinnerItem::SetTextPos(int iOffSetX, int iOffSetY)
{
	m_iTextOffSetX = iOffSetX;
	m_iTextOffSetY = iOffSetY;
}

void GUISpinnerItem::SetEditText()
{
	TCHAR buff[50];
	if (m_NumType == SPINNER_FLOAT)
	{
		_stprintf(buff, "%.2f", m_fEditValue);
		if (m_pEditBox)
		{
			m_pEditBox->SetText(m_szFontName, _T(""), buff, buff, buff, buff, m_iFontsize);
		}
	}
	else // m_NUMTYPE == SPINNER_INT
	{
		_itot(m_iEditValue, buff, 10);
		if(m_pEditBox)
		{
			m_pEditBox->SetText(m_szFontName, _T(""), buff, buff, buff, buff, m_iFontsize);
		}
	}
}

void GUISpinnerItem::IncrementValue(bool bUp)
{
	MathUtil math;
	if (bUp)
	{
		m_fEditValue += m_fIncrement;
		m_iEditValue += (int)math.Floor(m_fIncrement);

	}
	else
	{
		m_fEditValue -= m_fIncrement;
		m_iEditValue -= (int)math.Floor(m_fIncrement);
	}
	ClampValue();
}

void GUISpinnerItem::ClampValue()
{
	if ( m_fEditValue < m_fMinRange )
	{
		m_fEditValue = m_fMinRange;
	}
	if ( m_fEditValue > m_fMaxRange )
	{
		m_fEditValue = m_fMaxRange;
	}

	MathUtil math;

	if ( m_iEditValue < (int)math.Floor(m_fMinRange) )
	{
		m_iEditValue = (int)math.Floor(m_fMinRange);
	}
	if ( m_iEditValue > (int)math.Floor(m_fMaxRange) )
	{
		m_iEditValue = (int)math.Floor(m_fMaxRange);
	}
}

void GUISpinnerItem::UpdateFontExt(vector<GUIFONTINFO> &fontData, float xPos, float yPos, float width, 
								   float height, DWORD state)
{
	DWORD edState, upState, downState;
	UnpackState(state, edState, upState, downState);
	// quirky little deal here
	// the edit state has the spinner value...
	LoadHashVal(edState);

	float arrowX, editX, arrowWidth, editWidth;
	GetSubElemSpots(xPos, yPos, width, height, arrowX, editX, arrowWidth, editWidth);

	if (m_pEditBox)
	{		
		m_pEditBox->UpdateFontExt(fontData, editX, yPos, editWidth, height, 0);
	}
}

void GUISpinnerItem::LoadVertexBuffer(vector<GUIVERTEXINFO> &vertexData, float x, float y, float width, 
									  float height, DWORD state, float angle, DWORD color)
{
	DWORD edState, upState, downState;
	UnpackState(state, edState, upState, downState);
	
	float arrowX, editX, arrowWidth, editWidth;
	GetSubElemSpots(x, y, width, height, arrowX, editX, arrowWidth, editWidth);

	if (m_pUpArrow)
	{
		m_pUpArrow->LoadVertexBuffer(vertexData, arrowX, y, arrowWidth, height / 2.0f, upState, angle, color);
	}

	if (m_pDownArrow)
	{
		m_pDownArrow->LoadVertexBuffer(vertexData, arrowX, y + height / 2.0f, arrowWidth, height / 2.0f, downState, angle, color);
	}

	if (m_pEditBox)
	{
		m_pEditBox->LoadVertexBuffer(vertexData, editX, y, editWidth, height, 0, angle, color);
	}
}

void GUISpinnerItem::GetSubElemSpots(float x, float y, float width, float height, float &arrowX, float &editX, 
		float &arrowWidth, float &editWidth)
{
	arrowWidth = height / 2.0f;
	if (arrowWidth > width / 4.0f)
	{
		arrowWidth = width / 4.0f;
	}

	editWidth = width - arrowWidth;

	if (m_bButtonsOnRight)
	{
		arrowX = x + editWidth;
		editX = x;
	}
	else
	{
		arrowX = x;
		editX = x + arrowWidth;
	}
}

DWORD GUISpinnerItem::PackStates(DWORD editState, DWORD upArrowState, DWORD downArrowState)
{
	DWORD ret = 0;
	ret += editState << 8;
	ret += upArrowState << 4;
	ret += downArrowState;
	return ret;
}
	
void GUISpinnerItem::UnpackState(DWORD inState, DWORD &editState, DWORD &upArrowState, DWORD &downArrowState)
{
	editState = inState >> 8;
	upArrowState = (inState >> 4) & 0xf;
	downArrowState = inState & 0xf;
}

bool GUISpinnerItem::MouseOver(float fCursorPosX, float fCursorPosY, bool bLeftMouseDown, bool bLeftMouseUp, 
			   float xPos, float yPos, float width, float height, DWORD *state)
{
	bool ret = false;
	DWORD edState, upState, downState;
	UnpackState(*state, edState, upState, downState);
	// quirky little deal here
	// the edit state has the spinner value...
	LoadHashVal(edState);	

	float arrowX, editX, arrowWidth, editWidth;
	GetSubElemSpots(xPos, yPos, width, height, arrowX, editX, arrowWidth, editWidth);

	if (bLeftMouseDown)
	{
		if (m_pUpArrow)
		{
			bool arrowUp = m_pUpArrow->MouseButton(fCursorPosX, fCursorPosY, true, false, arrowX, yPos, arrowWidth, height / 2.0f, &upState);
			if (arrowUp)
			{
				ret = true;	
				IncrementValue(true);
				edState = GetHashVal();
			}
		}
		if (m_pDownArrow)
		{
			bool arrowDown = m_pDownArrow->MouseButton(fCursorPosX, fCursorPosY, true, false, arrowX, yPos + (height/2.0f), arrowWidth, height / 2.0f, &downState);
			if (arrowDown)
			{
				ret = true;	
				IncrementValue(false);
				edState = GetHashVal();
			}
		}
	}

	*state = PackStates(edState, upState, downState);
	return ret;
}

DWORD GUISpinnerItem::GetHashVal()
{
	TCHAR buf[50];
	if (m_NumType == SPINNER_FLOAT)
	{
		_stprintf(buf, _T("%.2f"), m_fEditValue);
	}
	else
	{
		_stprintf(buf, _T("%d"), m_iEditValue);
	}

	CHashString valueHash(buf);
	DWORD ret = valueHash.GetUniqueID() - m_HashBias;
	return ret;
}
	
void GUISpinnerItem::LoadHashVal(DWORD val)
{
	// value has changed
	if (val > 0)
	{
		DWORD hash = val + m_HashBias;
		StdString szVal = EngineGetToolBox()->GetHashString(hash);
		if (m_NumType == SPINNER_FLOAT)
		{
			SetEditFloat((float)_tstof(szVal));
		}
		else
		{
			SetEditInt(_tstoi(szVal));
		}
	}
	// state is uninitialized; use default
	else
	{
		if (m_NumType == SPINNER_FLOAT)
		{
			SetEditFloat(m_fDefaultVal);
		}
		else
		{
			SetEditInt(m_iDefaultVal);
		}
	}
}

void GUISpinnerItem::SetDefaultValue(float val)
{
	m_fDefaultVal = val;
	SetEditFloat(val);
}
	
void GUISpinnerItem::SetDefaultValue(int val)
{
	m_iDefaultVal = val;
	SetEditInt(val);
}

DWORD GUISpinnerItem::ToggleUp(DWORD lastState)
{
	DWORD edState, upState, downState;
	UnpackState(lastState, edState, upState, downState);
	// quirky little deal here
	// the edit state has the spinner value...
	LoadHashVal(edState);	

	IncrementValue(true);
	edState = GetHashVal();

	return PackStates(edState, upState, downState);
}

DWORD GUISpinnerItem::ToggleDown(DWORD lastState)
{
	DWORD edState, upState, downState;
	UnpackState(lastState, edState, upState, downState);
	// quirky little deal here
	// the edit state has the spinner value...
	LoadHashVal(edState);	

	IncrementValue(false);
	edState = GetHashVal();

	return PackStates(edState, upState, downState);
}

DWORD GUISpinnerItem::GetHashText(DWORD inState)
{
	DWORD edState, upState, downState;
	UnpackState(inState, edState, upState, downState);
	
	return edState + m_HashBias;
}

DWORD GUISpinnerItem::SetHashText(DWORD inState, DWORD hashText)
{
	DWORD edState, upState, downState;
	UnpackState(inState, edState, upState, downState);
	
	// again, quirky... edState is actually the hash of the text - the bias...
	edState = hashText - m_HashBias;

	return PackStates(edState, upState, downState);
}