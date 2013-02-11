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

enum BUTTONSTATE { NORM_STATE=0, HIGH_STATE, SELECT_STATE, GRAY_STATE };

GUIButton::GUIButton() 
{
	m_szNorm = m_szHigh = m_szSelect = m_szGray = _T("");
	m_iColor = m_iColorHigh = m_iColorSelect = m_iColorGray = 0;
	m_bAutoSize = true;

	m_szFontName = NULL;
	m_szBoldFontName = NULL;
	m_szItalicFontName = NULL;
	m_szBoldItalicFontName = NULL;
	m_szText = "";
	m_iFontsize = 0;
	
	m_bCenterText = false;	

	// grab the render interface
	GETRENDERERINTERFACEPARAMS renderInterface;

	static DWORD msgHash_GetRendererInterface = CHashString(_T("GetRendererInterface")).GetUniqueID();
	DWORD retval = EngineGetToolBox()->SendMessage(msgHash_GetRendererInterface, 
					sizeof(GETRENDERERINTERFACEPARAMS), 
					&renderInterface, 
					NULL, 
					NULL);

	m_Renderer = renderInterface.m_RendererInterface;

	m_pNormBase = NULL;
	m_pHighBase = NULL;
	m_pSelectBase = NULL;
	m_pGrayBase = NULL;

	m_pNormDecal = NULL;
	m_pHighDecal = NULL;
	m_pSelectDecal = NULL;
	m_pGrayDecal = NULL;
	
	m_bSmartScale = false;

	m_iTextOffSetX = 0;
	m_iTextOffSetY = 0;
	m_fDecalOffsetX = 0.0f;
	m_fDecalOffsetY = 0.0f;

	m_bShowTexture = true;

	m_pEffect = NULL;

	m_bUseFontShadow = false;
	m_iFontShadowColor = 0xff000000;
	m_iFontShadowXScale = 0;
	m_iFontShadowYScale = 0;
	m_iFontShadowXOffset = 0;
	m_iFontShadowYOffset = 0;

	m_CurrentBaseMask = NULL;
	m_CurrentDecalMask = NULL;
}

GUIButton::~GUIButton()
{
	DeInit();
}

void GUIButton::DeInit()
{
	SAFE_DELETE(m_pNormBase);
	SAFE_DELETE(m_pHighBase);
	SAFE_DELETE(m_pSelectBase);
	SAFE_DELETE(m_pGrayBase);

	SAFE_DELETE(m_pNormDecal);
	SAFE_DELETE(m_pHighDecal);
	SAFE_DELETE(m_pSelectDecal);
	SAFE_DELETE(m_pGrayDecal);
}

void GUIButton::SetTextColors(int iColorNorm, int iColorHigh, int iColorSelect, int iColorGray)
{
	m_iColor = iColorNorm;
	m_iColorHigh = iColorHigh;
	m_iColorSelect = iColorSelect;
	m_iColorGray = iColorGray;
	m_iTextcol = CreateColorRef(iColorNorm);
}

void GUIButton::SetScreenMasks(const TCHAR* szNorm, const TCHAR* szHigh, const TCHAR* szSelect, const TCHAR* szGray)
{
	SAFE_DELETE(m_pNormBase);
	SAFE_DELETE(m_pHighBase);
	SAFE_DELETE(m_pSelectBase);
	SAFE_DELETE(m_pGrayBase);

	if(szNorm != NULL)
	{
		m_szNorm = szNorm;
		m_pNormBase = new CLayeredMask();
		m_pNormBase->LoadTexture(m_szNorm);
	}
	else
	{
		m_szNorm = _T("");
	}

	if(szHigh != NULL)
	{
		m_szHigh = szHigh;
		m_pHighBase = new CLayeredMask();
		m_pHighBase->LoadTexture(m_szHigh);
	}
	else 
	{
		m_szHigh = _T("");
	}
	
	if (szSelect != NULL)
	{
		m_szSelect = szSelect;
		m_pSelectBase = new CLayeredMask();
		m_pSelectBase->LoadTexture(m_szSelect);
	}
	else
	{
		m_szSelect = _T("");
	}

	if(szGray != NULL)
	{
		m_szGray = szGray;
		m_pGrayBase = new CLayeredMask();
		m_pGrayBase->LoadTexture(m_szGray);
	}
	else
	{
		m_szGray = _T("");
	}
}

void GUIButton::SetDecalMasks(const TCHAR* szNorm, const TCHAR* szHigh, 
							  const TCHAR* szSelect, const TCHAR* szGray, 
							  float offsetX, float offsetY)
{
	SAFE_DELETE(m_pNormDecal);
	SAFE_DELETE(m_pHighDecal);
	SAFE_DELETE(m_pSelectDecal);
	SAFE_DELETE(m_pGrayDecal);

	if(szNorm != NULL)
	{
		m_szDecalNorm = szNorm;
		m_pNormDecal = new CLayeredMask();
		m_pNormDecal->LoadTexture(m_szDecalNorm);
	}
	else
	{
		m_szDecalNorm = _T("");
	}

	if(szHigh != NULL)
	{
		m_szDecalHigh = szHigh;
		m_pHighDecal = new CLayeredMask();
		m_pHighDecal->LoadTexture(m_szDecalHigh);
	}
	else 
	{
		m_szDecalHigh = _T("");
	}
	
	if (szSelect != NULL)
	{
		m_szDecalSelect = szSelect;
		m_pSelectDecal = new CLayeredMask();
		m_pSelectDecal->LoadTexture(m_szDecalSelect);
	}
	else
	{
		m_szDecalSelect = _T("");
	}

	if(szGray != NULL)
	{
		m_szDecalGray = szGray;
		m_pGrayDecal = new CLayeredMask();
		m_pGrayDecal->LoadTexture(m_szDecalGray);
	}
	else
	{
		m_szDecalGray = _T("");
	}

	m_fDecalOffsetX = offsetX;
	m_fDecalOffsetY = offsetY;
}

void GUIButton::ChangeText(const TCHAR *szNewText)
{
	if (!szNewText)
	{
		m_szText = "";
	}
	else
	{
		m_szText = szNewText;
	}
}

void GUIButton::SetText(const TCHAR *szFontname, const TCHAR *szBFontname, const TCHAR *szNormText, 
		const TCHAR *szHighText, const TCHAR *szSelectText, const TCHAR *szGrayedText,
		int fontsize)
{
	if (strcmp(szFontname, "") != 0)
	{
		m_szFontName = szFontname;
		m_szBoldFontName = szBFontname;

		if (szNormText != NULL)
		{
			m_szText = szNormText;
		}
		if (szHighText != NULL)
		{
			m_szTextHigh = szHighText;
		}
		if (szSelectText != NULL)
		{
			m_szTextSelect = szSelectText;
		}
		if (szGrayedText != NULL)
		{
			m_szTextGrayed = szGrayedText;
		}
		m_iFontsize = fontsize;
	}
}

void GUIButton::SetTextPos(int iOffSetX, int iOffSetY)
{
	m_iTextOffSetX = iOffSetX;
	m_iTextOffSetY = iOffSetY;
}

bool GUIButton::MouseOver(float fCursorPosX, float fCursorPosY, float xPos, float yPos, float width, float height, DWORD *state)
{
	bool bVal = false;
	// now check if mouse is over button
	if((fCursorPosX < xPos + width) && 
		(fCursorPosX > width) &&
		(fCursorPosY < yPos + height) && 
		(fCursorPosY > yPos))
	{
		*state = HIGH_STATE;
		bVal = true;
	}
	else
	{
		*state = NORM_STATE; //can also go back to HIGH_STATE...
		bVal = false;
	}
	
	return bVal;
}

bool GUIButton::MouseButton(float fCursorPosX, float fCursorPosY, bool bButtonPressed, bool bButtonRelease, 
							float xPos, float yPos, float width, float height, DWORD *state)
{
	// check if mouse is over button first
	bool bVal = MouseOver(fCursorPosX, fCursorPosY, xPos, yPos, width, height, state);

	if(bVal)
	{
		if(bButtonPressed)
		{
			*state = SELECT_STATE;
			bVal = true;
		}
		else if (bButtonRelease)
		{
			*state = HIGH_STATE;
			bVal = true;
		}
		else
		{
			bVal = false;
		}
	}
	return bVal;
}

void GUIButton::EnableNorm()
{
	m_iTextcol = CreateColorRef(m_iColor);

	m_CurrentBaseMask = m_pNormBase;
	m_CurrentDecalMask = m_pNormDecal;

	StdString tempStr="";
	tempStr += "[FColor=";
	tempStr += m_iTextcol.ToString();
	tempStr += "]";
	tempStr += m_szText;
	tempStr += "[/FColor]";
	m_szCurrentText = tempStr;
}

void GUIButton::EnableHigh()
{
	m_iTextcol = CreateColorRef(m_iColorHigh);

	m_CurrentBaseMask = m_pHighBase;
	m_CurrentDecalMask = m_pHighDecal;
	
	StdString tempStr="";
	tempStr += "[FColor=";
	tempStr += m_iTextcol.ToString();
	tempStr += "]";
	tempStr += m_szTextHigh;
	tempStr += "[/FColor]";
	m_szCurrentText = tempStr;
}

void GUIButton::EnableSelect()
{
	m_iTextcol = CreateColorRef(m_iColorSelect);

	m_CurrentBaseMask = m_pSelectBase;
	m_CurrentDecalMask = m_pSelectDecal;

	StdString tempStr="";
	tempStr += "[FColor=";
	tempStr += m_iTextcol.ToString();
	tempStr += "]";
	tempStr += m_szTextSelect;
	tempStr += "[/FColor]";
	m_szCurrentText = tempStr;
}

void GUIButton::EnableGray() 
{
	m_iTextcol = CreateColorRef(m_iColorGray);

	m_CurrentBaseMask = m_pGrayBase;
	m_CurrentDecalMask = m_pGrayDecal;

	StdString tempStr="";
	tempStr += "[FColor=";
	tempStr += m_iTextcol.ToString();
	tempStr += "]";
	tempStr += m_szTextGrayed;
	tempStr += "[/FColor]";
	m_szCurrentText = tempStr;
}


CColorRef GUIButton::CreateColorRef(int iColor)
{
	unsigned int r, g, b, mask;
	
	mask = 0xff0000;
	r = (iColor & mask) >> 16;
	
	mask = 0x00ff00;
	g = (iColor & mask) >> 8;

	mask = 0x0000ff;
	b = (iColor & mask);
	
	CColorRef newColor(r, g, b);
	return newColor;
}

void GUIButton::UpdateFontExt(vector<GUIFONTINFO> &fontData, float xPos, float yPos, float width, float height, DWORD state)
{
	switch(state)
	{
	case NORM_STATE:
		EnableNorm();
		break;
	case HIGH_STATE:
		EnableHigh();
		break;
	case SELECT_STATE:
		EnableSelect();
		break;
	case GRAY_STATE:
		EnableGray();
		break;
	}

	if ((m_szFontName != NULL) && (_tcscmp((const TCHAR*)m_szFontName, _T("")) != 0))
	{
		CHashString hsFontName(m_szFontName);
		CHashString hsBoldFontName(m_szBoldFontName);
		CHashString hsText(m_szCurrentText);

		GUIFONTINFO entry;
		entry.dwPrimaryFontName = hsFontName.GetUniqueID();
		entry.dwSecondaryFontName = hsBoldFontName.GetUniqueID();
		entry.dwText = hsText.GetUniqueID();
		entry.iFontSize = m_iFontsize;
		entry.bCentered = m_bCenterText;
		entry.iXOffset = m_iTextOffSetX;
		entry.iYOffset = m_iTextOffSetY;
		
		RECT rRect;
		rRect.left = (int)xPos;
		rRect.right = (int)xPos + (int)width + m_iTextOffSetX;
		rRect.top = (int)yPos;
		rRect.bottom = (int)yPos + (int)height + m_iTextOffSetY;
		entry.renderRect = rRect;
		
		entry.bUseClippingRect = false;

		entry.bUseShadow = m_bUseFontShadow;
		entry.iShadowColor = m_iFontShadowColor;
		entry.iShadowXOffset = m_iFontShadowXOffset;
		entry.iShadowYOffset = m_iFontShadowYOffset;
		entry.iShadowXScale = m_iFontShadowXScale;
		entry.iShadowYScale = m_iFontShadowYScale;

		fontData.push_back(entry);
	}		
}

void GUIButton::LoadVertexBuffer(vector<GUIVERTEXINFO> &vertexData, float x, float y, float width, float height, DWORD state, 
												float angle, DWORD color)
{
	switch(state)
	{
	case NORM_STATE:
		EnableNorm();
		break;
	case HIGH_STATE:
		EnableHigh();
		break;
	case SELECT_STATE:
		EnableSelect();
		break;
	case GRAY_STATE:
		EnableGray();
		break;
	}

	if (m_CurrentBaseMask)
	{
		GUIVERTEXINFO entry;
		entry.iNumPrims = m_CurrentBaseMask->LoadVertexData(entry.VertexData, x, y, width, height, 
			angle, color, m_bSmartScale);
		entry.pTexture = m_CurrentBaseMask->GetMask();

		vertexData.push_back(entry);	
	}
}