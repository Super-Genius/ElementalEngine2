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
#include "CGUIItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT(CGUIItem);

REGISTER_MESSAGE_HANDLER(GetSelectRect, OnGetSelectRect, CGUIItem);
REGISTER_MESSAGE_HANDLER(SetButtonImage, OnSetButtonImage, CGUIItem);

CGUIItem::CGUIItem(IHashString *parentName, IHashString *name) :
	OBJECTTEMPLATE(CGUIManager, CGUIItem, IGUIReference, parentName, name)	
{
	m_pVisItem = new GUIButton();

	//set all members to default vals
	m_szNorm = m_szHigh = m_szSelect = m_szGrayed = _T("");
	m_iNormFontColor = m_iHighFontColor = m_iSelFontColor = m_iGrayFontColor = 0;
	m_fCursorPosX = m_fCursorPosY = -1;
	m_szText = m_szTextHigh = m_szTextSelect = m_szTextGrayed = m_szFont = _T("");
	m_bSmartScale = false;
	m_iTextOffSetX = m_iTextOffSetY = 0;
	m_bCenterText = false;

	m_fRectX1 = m_fRectX2 = m_fRectY1 = m_fRectY2 = 0.f;
	m_iFontsize = 20; // default to 20 pt font
	m_fDecalOffsetX = 0.f;
	m_fDecalOffsetY = 0.f;

	m_fSaveVersion = 1.0f;
}

CGUIItem::~CGUIItem()
{
	delete m_pVisItem;
}

 IComponent *CGUIItem::Create(int nArgs, va_list argptr)
{
	IHashString *name;
	IHashString *parentName;
	IObject *self;

	name = va_arg(argptr, IHashString *);
	parentName = va_arg(argptr, IHashString *);
	// create the object however you must create it
	try
	{
		// try and create an instance
		self = new CGUIItem(parentName, name);
	}
	catch(...)
	{
		return NULL;
	}
	return self;
}

bool CGUIItem::Update()
{
	return true;
}

IHashString *CGUIItem::GetComponentType()
{
	static CHashString CGUIItemCT(_T("CGUIItem"));
	return &CGUIItemCT;
}

bool CGUIItem::IsKindOf(IHashString *compType)
{
	return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
}


void CGUIItem::Serialize( IArchive &ar )
{
	if(ar.IsReading())
	{
		CHashString versionName;
		static DWORD msgHash_GetFileVersion = CHashString(_T("GetFileVersion")).GetUniqueID();
		DWORD retval = EngineGetToolBox()->SendMessage(msgHash_GetFileVersion, sizeof(IHashString), &versionName);

		static DWORD Vers2_0a = CHashString(_T("2.0a")).GetUniqueID();
		static DWORD Vers2_0b = CHashString(_T("2.0b")).GetUniqueID();
		static DWORD Vers2_0c = CHashString(_T("2.0c")).GetUniqueID();

		int verNum = 0;
		
		if (versionName.GetUniqueID() == Vers2_0a)
		{
			verNum = 3;
		}
		else if (versionName.GetUniqueID() == Vers2_0b)
		{
			verNum = 4;
		}
		else if (versionName.GetUniqueID() == Vers2_0c)
		{
			verNum = 5;
		}
		else 
		{
			m_ToolBox->Log(LOGERROR, _T("Bad GUI file version!"));
			assert(0);
			return;
		}

		float loadVersion = 0.0f;

		if (verNum >= 4)
		{
			ar.Read(loadVersion);
			if (loadVersion == 0.0f)
			{
				// initial serialize from editor; set to save version (up to date schema)
				loadVersion = m_fSaveVersion;
			}
		}

		ar.Read(m_szNorm);
		ar.Read(m_szHigh);
		ar.Read(m_szSelect);
		ar.Read(m_szGrayed);		
		ar.Read(m_szDecalNorm);
		ar.Read(m_szDecalHigh);
		ar.Read(m_szDecalSelect);
		ar.Read(m_szDecalGrayed);
		ar.Read(m_fDecalOffsetX);
		ar.Read(m_fDecalOffsetY);		
		ar.Read(m_szText);		
		ar.Read(m_szTextHigh);
		ar.Read(m_szTextSelect);
		ar.Read(m_szTextGrayed);
		ar.Read(m_szFont);		
		ar.Read(m_szFontBold);	
		ar.Read(m_iFontsize);
		ar.Read(m_iNormFontColor);
		ar.Read(m_iHighFontColor);
		ar.Read(m_iSelFontColor);
		ar.Read(m_iGrayFontColor);
		ar.Read(m_iTextOffSetX);
		ar.Read(m_iTextOffSetY);
		ar.Read(m_bCenterText);
		ar.Read(m_bSmartScale);
	
		CreateItem();
	}
	else
	{
		ar.Write(m_fSaveVersion, _T("Version"));
		ar.Write(m_szNorm, _T("NormTexture"));
		ar.Write(m_szHigh, _T("HighTexture"));
		ar.Write(m_szSelect, _T("SelectTexture"));
		ar.Write(m_szGrayed, _T("GrayTexture"));
		ar.Write(m_szDecalNorm, _T("NormDecalTexture"));
		ar.Write(m_szDecalHigh, _T("HighDecalTexture"));
		ar.Write(m_szDecalSelect, _T("SelectDecalTexture"));
		ar.Write(m_szDecalGrayed, _T("GrayDecalTexture"));
		ar.Write(m_fDecalOffsetX, _T("DecalOffsetX"));
		ar.Write(m_fDecalOffsetY, _T("DecalOffsetY"));		
		ar.Write(m_szText, _T("StaticText"));
		ar.Write(m_szTextHigh, _T("StaticTextHigh"));
		ar.Write(m_szTextSelect, _T("StaticTextSelect"));
		ar.Write(m_szTextGrayed, _T("StaticTextGrayed"));		
		ar.Write(m_szFont, _T("FontName"));
		ar.Write(m_szFontBold, _T("BoldFontName"));
		ar.Write(m_iFontsize, _T("Fontsize"));
		ar.Write(m_iNormFontColor, _T("NormFontColor"));
		ar.Write(m_iHighFontColor, _T("HighFontColor"));
		ar.Write(m_iSelFontColor, _T("SelFontColor"));
		ar.Write(m_iGrayFontColor, _T("GrayFontColor"));
		ar.Write(m_iTextOffSetX, _T("TextOffSetX"));
		ar.Write(m_iTextOffSetY, _T("TextOffSetY"));
		ar.Write(m_bCenterText, _T("CenterText"));
		ar.Write(m_bSmartScale, _T("SmartScale"));
	}
}

DWORD CGUIItem::CheckMouseState(float mouseX, float mouseY, MOUSESTATE mouseState, 
							   float x, float y, float width, float height, DWORD dwLastState)
{ 
	DWORD btState;
	if (mouseState == MOUSE_OPEN)
	{
		m_pVisItem->MouseOver(mouseX, mouseY, x, y, width, height, &btState);
		return btState;
	}
	else if (mouseState == MOUSE_LEFTPRESSED)
	{
		m_pVisItem->MouseButton(mouseX, mouseY, true, false, x, y, width, height, &btState);
		return btState;
	}
	else if (mouseState == MOUSE_LEFTRELEASED)
	{
		if ( m_pVisItem->MouseButton(mouseX, mouseY, false, true, x, y, width, height, &btState) )
		{
			StdString eventName = _T("RELEASED_");
			eventName += GetName()->GetString();
			CHashString eName(eventName.c_str());
		
			GUIEVENT ge;
			ge.event = eName.GetUniqueID();
			ge.ar = NULL;
			static DWORD msgHash_QueueGUIStateEvent = CHashString(_T("QueueGUIStateEvent")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_QueueGUIStateEvent, sizeof(GUIEVENT), &ge);
		}
		return btState;
	}
	return 0; 
}

DWORD CGUIItem::OnKeyPress(DWORD msgSize, void *keyStruct)
{
	return MSG_NOT_HANDLED;
}

DWORD CGUIItem::OnGetSelectRect(DWORD msgSize, void *param)
{
	SELECTRECT *pSelRect;
	VERIFY_MESSAGE_SIZE(msgSize, sizeof(SELECTRECT));
	
	pSelRect = (SELECTRECT*)param;
	pSelRect->x1 = m_fRectX1;
	pSelRect->x2 = m_fRectX2;
	pSelRect->y1 = m_fRectY1;
	pSelRect->y2 = m_fRectY2;

	return MSG_HANDLED_STOP;
}

void CGUIItem::CreateItem()
{
	m_pVisItem->SetScreenMasks(m_szNorm, m_szHigh, m_szSelect, m_szGrayed);
	m_pVisItem->SetDecalMasks(m_szDecalNorm, m_szDecalHigh, m_szDecalSelect, 
		m_szDecalGrayed, m_fDecalOffsetX, m_fDecalOffsetY);
	
	// older serialization doesn't have different texts
	if (_tcscmp(m_szTextHigh, "") == 0)
	{
		m_szTextHigh = m_szText;
	}
	if (_tcscmp(m_szTextSelect, "") == 0)
	{
		m_szTextSelect = m_szText;
	}
	if (_tcscmp(m_szTextGrayed, "") == 0)
	{
		m_szTextGrayed = m_szText;
	}

	m_pVisItem->SetText(m_szFont, m_szFontBold, m_szText, m_szTextHigh, 
		m_szTextSelect, m_szTextGrayed, m_iFontsize);
	m_pVisItem->SetTextPos(m_iTextOffSetX, m_iTextOffSetY);
	m_pVisItem->SetTextColors(m_iNormFontColor, m_iHighFontColor, m_iSelFontColor, m_iGrayFontColor);
	m_pVisItem->CenterText(m_bCenterText);
	m_pVisItem->SetSmartScale(m_bSmartScale);	
}

DWORD CGUIItem::OnEnable(DWORD msgSize, void *param)
{
	return MSG_HANDLED_STOP;
}

DWORD CGUIItem::OnDisable(DWORD msgSize, void *param)
{
	return MSG_HANDLED_STOP;
}

DWORD CGUIItem::OnSetButtonImage(DWORD msgSize, void *param)
{
	BUTTONIMAGE *bi;
	VERIFY_MESSAGE_SIZE(msgSize, sizeof(BUTTONIMAGE));

	bi = (BUTTONIMAGE*)param;
	
	m_pVisItem->SetScreenMasks(bi->szNorm, bi->szHigh, bi->szSelect, bi->szGray);

	return MSG_HANDLED_STOP;
}

void CGUIItem::GetRenderParams(vector<GUIVERTEXINFO> &vertexData, float x, float y, float width, float height, 
							   DWORD state, float angle, DWORD color)
{
	if (m_pVisItem)
	{
		m_pVisItem->LoadVertexBuffer(vertexData, x, y, width, height, state, angle, color);
	}
}

void CGUIItem::GetFontParams(vector<GUIFONTINFO> &fontData, float x, float y, 
								float width, float height, DWORD state, float angle, DWORD color)
{
	m_pVisItem->UpdateFontExt(fontData, x, y, width, height, state);
}

DWORD CGUIItem::ToggleUp(DWORD dwLastState)
{
	// TODO
	return dwLastState;
}

DWORD CGUIItem::ToggleDown(DWORD dwLastState)
{
	// TODO
	return dwLastState;
}
	
DWORD CGUIItem::ToggleSelect(DWORD dwLastState)
{
	// TODO
	return dwLastState;
}
	
DWORD CGUIItem::ToggleBack(DWORD dwLastState)
{
	// TODO
	return dwLastState;
}

void CGUIItem::FlagLanguageEntryChanged()
{
	// TODO
}