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
#include "CGUIInstance.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT(CGUIInstance);

REGISTER_MESSAGE_HANDLER(SetPosition, OnSetPosition, CGUIInstance);
REGISTER_MESSAGE_HANDLER(SetPositionRelative, OnSetPositionRelative, CGUIInstance);
REGISTER_MESSAGE_HANDLER(GetPositionRelative, OnGetPositionRelative, CGUIInstance);
REGISTER_MESSAGE_HANDLER(SetSize, OnSetSize, CGUIInstance);
REGISTER_MESSAGE_HANDLER(GetPosition, OnGetPosition, CGUIInstance);
REGISTER_MESSAGE_HANDLER(GetSize, OnGetSize, CGUIInstance);
REGISTER_MESSAGE_HANDLER(GetZOrder, OnGetZOrder, CGUIInstance);
REGISTER_MESSAGE_HANDLER(SetZOrder, OnSetZOrder, CGUIInstance);
REGISTER_MESSAGE_HANDLER(SetColor, OnSetColor, CGUIInstance);
REGISTER_MESSAGE_HANDLER(GetColor, OnGetColor, CGUIInstance);
REGISTER_MESSAGE_HANDLER(SetAlpha, OnSetAlpha, CGUIInstance);
REGISTER_MESSAGE_HANDLER(GetAngle, OnGetAngle, CGUIInstance);
REGISTER_MESSAGE_HANDLER(SetAngle, OnSetAngle, CGUIInstance);
REGISTER_MESSAGE_HANDLER(SetAngleFromLua, OnSetAngleFromLua, CGUIInstance);

REGISTER_MESSAGE_HANDLER(GUIAnim_PosX, OnGUIAnimPosX, CGUIInstance);
REGISTER_MESSAGE_HANDLER(GUIAnim_PosY, OnGUIAnimPosY, CGUIInstance);
REGISTER_MESSAGE_HANDLER(GUIAnim_SizeX, OnGUIAnimSizeX, CGUIInstance);
REGISTER_MESSAGE_HANDLER(GUIAnim_SizeY, OnGUIAnimSizeY, CGUIInstance);
REGISTER_MESSAGE_HANDLER(GUIAnim_AlphaCol, OnGUIAnimAlphaCol, CGUIInstance);
REGISTER_MESSAGE_HANDLER(GUIAnim_RCol, OnGUIAnimRCol, CGUIInstance);
REGISTER_MESSAGE_HANDLER(GUIAnim_GCol, OnGUIAnimGCol, CGUIInstance);
REGISTER_MESSAGE_HANDLER(GUIAnim_BCol, OnGUIAnimBCol, CGUIInstance);

REGISTER_MESSAGE_HANDLER(SetItemReference, OnSetItemRef, CGUIInstance);
REGISTER_MESSAGE_HANDLER(GetItemReference, OnGetItemRef, CGUIInstance);
REGISTER_MESSAGE_HANDLER(SelectBtPressed, OnSelectBtPressed, CGUIInstance);
REGISTER_MESSAGE_HANDLER(BackBtPressed, OnBackBtPressed, CGUIInstance);
REGISTER_MESSAGE_HANDLER(ToggleUpBtPressed, OnToggleUpBtPressed, CGUIInstance);
REGISTER_MESSAGE_HANDLER(ToggleDownBtPressed, OnToggleDownBtPressed, CGUIInstance);

REGISTER_MESSAGE_HANDLER(GetItemText, OnGetItemText, CGUIInstance);
REGISTER_MESSAGE_HANDLER(SetItemText, OnSetItemText, CGUIInstance);

REGISTER_MESSAGE_HANDLER(GetGUIParentName, OnGetGUIParentName, CGUIInstance);

CGUIInstance::CGUIInstance(IHashString *parentName, IHashString *name) :
	CGUIElement(parentName, name)
{
	m_ItemRef = NULL;

	m_fPosX = m_fPosY = 0.f;
	m_fSizeX = m_fSizeY = 0.f;

	m_szItemCopyName = _T("");
	m_szItemCopyType = _T("");

	m_fWidthRatio = 1.f;
	m_fHeightRatio = 1.f;
	m_fZoomFactor = 1.f;
	m_iPageOffsetX = 0;
	m_iPageOffsetY = 0;

	m_iColor = -1;
	m_bColorChanged = false;
	m_fAngle = 0.f;

	m_VertexBuffer = NULL;
	m_VBOffset = 0;

	m_fSaveVersion = 1.0f;

	m_State = 0;

	GETRENDERERINTERFACEPARAMS renderInterface;

	static DWORD msgHash_GetRendererInterface = CHashString(_T("GetRendererInterface")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_GetRendererInterface, 
					sizeof(GETRENDERERINTERFACEPARAMS), 
					&renderInterface, 
					NULL, 
					NULL);

	m_Renderer = renderInterface.m_RendererInterface;

	m_Manager = SINGLETONINSTANCE(CGUIManager);
}

CGUIInstance::~CGUIInstance()
{
	if (m_pFont.size() > 0)
	{
		for (UINT i=0; i<m_pFont.size(); i++)
		{
			CREATEFONTINTERFACE cfi;
			cfi.m_FontInterface = m_pFont[i];
			static DWORD msgHash_DeleteFont = CHashString(_T("DeleteFont")).GetUniqueID();
			EngineGetToolBox()->SendMessage(msgHash_DeleteFont, sizeof(CREATEFONTINTERFACE), &cfi);
		}
		m_pFont.clear();
	}
}

IComponent *CGUIInstance::Create(int nArgs, va_list argptr)
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
		self = new CGUIInstance(parentName, name);
	}
	catch(...)
	{
		return NULL;
	}
	return self;
}

bool CGUIInstance::Update()
{
	// reset render info
	m_VertexInfo.clear();
	if (m_ItemRef)
	{
		m_ItemRef->Update();
		m_ItemRef->GetRenderParams(m_VertexInfo, GetPosX(), GetPosY(), GetWidth(), GetHeight(), m_State, m_fAngle, m_iColor);
		vector<GUIFONTINFO> newFontData;
		m_ItemRef->GetFontParams(newFontData, GetPosX(), GetPosY(), GetWidth(), GetHeight(), m_State);

		for (UINT i=0; i<newFontData.size(); i++)
		{
			DWORD dwNewPrimaryFont = newFontData[i].dwPrimaryFontName;
			DWORD dwNewSecondaryFont = newFontData[i].dwSecondaryFontName;
			DWORD dwNewText = newFontData[i].dwText;

			if (i >= m_pFont.size())
			{
				// create new fonts!
				CREATEFONTINTERFACE cfi;
				static DWORD msgHash_CreateFont = CHashString(_T("CreateFont")).GetUniqueID();
				EngineGetToolBox()->SendMessage(msgHash_CreateFont, sizeof(CREATEFONTINTERFACE), &cfi);
				if (cfi.m_FontInterface == NULL)
				{
					EngineGetToolBox()->Log(LOGERROR, _T("Instance could not create a new font!\n"));
					assert(cfi.m_FontInterface);
					return false;
				}
				m_pFont.push_back(cfi.m_FontInterface);
				m_pFont[i]->UseKerning(false);
				m_pFont[i]->SetKerningScale( 1.0f );
				m_pFont[i]->SetVisible( true );
				m_pFont[i]->SetLeadingScale( 1.0f );					
			}

			if (i >= m_FontInfo.size())
			{
				m_pFont[i]->SetFont(m_ToolBox->GetHashString(dwNewPrimaryFont), newFontData[i].iFontSize);
				m_pFont[i]->SetFontBold(m_ToolBox->GetHashString(dwNewSecondaryFont), newFontData[i].iFontSize);
				m_pFont[i]->SetRenderRect(newFontData[i].renderRect.left, newFontData[i].renderRect.top, 
					newFontData[i].renderRect.right, newFontData[i].renderRect.bottom);				
				
				StdString colorAdjust = MultiplyTextColor(m_ToolBox->GetHashString(dwNewText));
				m_pFont[i]->SetText(colorAdjust);
				if (newFontData[i].bCentered)
				{
					float centerX = GetWidth() / 2.f;
					float centerY = GetHeight() / 2.f;
                    StdString cleanStr = RemoveFormatting(m_ToolBox->GetHashString(dwNewText));
					int textWidth = m_pFont[i]->GetFontPixWidth(cleanStr);
					int textHeight = newFontData[i].iFontSize * m_pFont[i]->GetNumLines();
					float textPosX = centerX - ((float)textWidth / 2.f) + newFontData[i].iXOffset;
					float textPosY = centerY - ((float)textHeight / 2.f) + newFontData[i].iYOffset;
					m_pFont[i]->SetOffset(textPosX, textPosY);
				}
				else
				{
					m_pFont[i]->SetOffset((float)newFontData[i].iXOffset, (float)newFontData[i].iYOffset);
				}

				m_pFont[i]->SetPageOffset(m_iPageOffsetX, m_iPageOffsetY);					
				m_pFont[i]->SetWidthRatio(m_fWidthRatio);
				m_pFont[i]->SetHeightRatio(m_fHeightRatio);
				m_pFont[i]->SetZoomFactor(m_fZoomFactor);

				if (newFontData[i].bUseShadow)
				{
					m_pFont[i]->EnableDropShadow(true);
					m_pFont[i]->SetShadowVals(newFontData[i].iShadowColor, newFontData[i].iShadowXScale, newFontData[i].iShadowYScale, newFontData[i].iShadowXOffset, newFontData[i].iShadowYOffset);
				}

				GUIFONTINFO entry;
				entry.dwPrimaryFontName = newFontData[i].dwPrimaryFontName;
				entry.dwSecondaryFontName = newFontData[i].dwSecondaryFontName;
				entry.dwText = newFontData[i].dwText;
				entry.iFontSize = newFontData[i].iFontSize;
				entry.bCentered = newFontData[i].bCentered;
				entry.iXOffset = newFontData[i].iXOffset;
				entry.iYOffset = newFontData[i].iYOffset;
				entry.renderRect = newFontData[i].renderRect;
				entry.clippingRect = newFontData[i].clippingRect;
				entry.bUseClippingRect = newFontData[i].bUseClippingRect;
				entry.bRender = true;
				entry.bUseShadow = newFontData[i].bUseShadow;
				entry.iShadowColor = newFontData[i].iShadowColor;
				entry.iShadowXScale = newFontData[i].iShadowXScale;
				entry.iShadowYScale = newFontData[i].iShadowYScale;
				entry.iShadowXOffset = newFontData[i].iShadowXOffset;
				entry.iShadowYOffset = newFontData[i].iShadowYOffset;
				
				m_FontInfo.push_back(entry);
			}
			// compare elements
			else
			{	
				if (m_FontInfo[i].dwPrimaryFontName != newFontData[i].dwPrimaryFontName)
				{
					m_pFont[i]->SetFont(m_ToolBox->GetHashString(dwNewPrimaryFont), newFontData[i].iFontSize);
					m_FontInfo[i].dwPrimaryFontName = newFontData[i].dwPrimaryFontName;
					m_FontInfo[i].iFontSize = newFontData[i].iFontSize;
				}
				if (m_FontInfo[i].dwSecondaryFontName != newFontData[i].dwSecondaryFontName)
				{
					m_pFont[i]->SetFontBold(m_ToolBox->GetHashString(dwNewSecondaryFont), newFontData[i].iFontSize);
					m_FontInfo[i].dwSecondaryFontName = newFontData[i].dwSecondaryFontName;
					m_FontInfo[i].iFontSize = newFontData[i].iFontSize;
				}
				if (m_FontInfo[i].iFontSize != newFontData[i].iFontSize)
				{
					m_pFont[i]->SetFont(m_ToolBox->GetHashString(dwNewPrimaryFont), newFontData[i].iFontSize);
					m_pFont[i]->SetFontBold(m_ToolBox->GetHashString(dwNewSecondaryFont), newFontData[i].iFontSize);
					m_FontInfo[i].iFontSize = newFontData[i].iFontSize;
				}
				if (m_FontInfo[i].dwText != newFontData[i].dwText)
				{
					StdString colorAdjust = MultiplyTextColor(m_ToolBox->GetHashString(dwNewText));
					m_pFont[i]->SetText(colorAdjust);
					m_FontInfo[i].dwText = newFontData[i].dwText;
					if (newFontData[i].bCentered)
					{
						float centerX = GetWidth() / 2.f;
						float centerY = GetHeight() / 2.f;
						StdString cleanStr = RemoveFormatting(m_ToolBox->GetHashString(dwNewText));
						int textWidth = m_pFont[i]->GetFontPixWidth(cleanStr);
						int textHeight = m_FontInfo[i].iFontSize * m_pFont[i]->GetNumLines();
						float textPosX = centerX - ((float)textWidth / 2.f) + m_FontInfo[i].iXOffset;
						float textPosY = centerY - ((float)textHeight / 2.f) + m_FontInfo[i].iYOffset;
						m_pFont[i]->SetOffset(textPosX, textPosY);
					}
				}
				if (m_FontInfo[i].bCentered != newFontData[i].bCentered)
				{
					m_FontInfo[i].bCentered = newFontData[i].bCentered;
					if (newFontData[i].bCentered)
					{
						float centerX = GetWidth() / 2.f;
						float centerY = GetHeight() / 2.f;
						StdString cleanStr = RemoveFormatting(m_ToolBox->GetHashString(dwNewText));
						int textWidth = m_pFont[i]->GetFontPixWidth(cleanStr);
						int textHeight = m_FontInfo[i].iFontSize * m_pFont[i]->GetNumLines();
						float textPosX = centerX - ((float)textWidth / 2.f) + newFontData[i].iXOffset;
						float textPosY = centerY - ((float)textHeight / 2.f) + newFontData[i].iYOffset;
						m_pFont[i]->SetOffset(textPosX, textPosY);
					}
					else
					{
						m_pFont[i]->SetOffset((float)newFontData[i].iXOffset, (float)newFontData[i].iYOffset);
					}
				}
				if ((m_FontInfo[i].iXOffset != newFontData[i].iXOffset) || (m_FontInfo[i].iYOffset != newFontData[i].iYOffset))
				{
					if (!m_FontInfo[i].bCentered)
					{
						m_pFont[i]->SetOffset((float)newFontData[i].iXOffset, (float)newFontData[i].iYOffset);
					}
					m_FontInfo[i].iXOffset = newFontData[i].iXOffset;
					m_FontInfo[i].iYOffset = newFontData[i].iYOffset;
				}
				if ((m_FontInfo[i].renderRect.bottom != newFontData[i].renderRect.bottom) ||
					(m_FontInfo[i].renderRect.top != newFontData[i].renderRect.top) ||
					(m_FontInfo[i].renderRect.left != newFontData[i].renderRect.left) ||
					(m_FontInfo[i].renderRect.right != newFontData[i].renderRect.right))
				{
					m_pFont[i]->SetRenderRect(newFontData[i].renderRect.left, newFontData[i].renderRect.top, 
						newFontData[i].renderRect.right, newFontData[i].renderRect.bottom);
					m_FontInfo[i].renderRect = newFontData[i].renderRect;
					if (newFontData[i].bCentered)
					{
						float centerX = GetWidth() / 2.f;
						float centerY = GetHeight() / 2.f;
						StdString cleanStr = RemoveFormatting(m_ToolBox->GetHashString(dwNewText));
						int textWidth = m_pFont[i]->GetFontPixWidth(cleanStr);
						int textHeight = m_FontInfo[i].iFontSize * m_pFont[i]->GetNumLines();
						float textPosX = centerX - ((float)textWidth / 2.f) + newFontData[i].iXOffset;
						float textPosY = centerY - ((float)textHeight / 2.f) + newFontData[i].iYOffset;
						m_pFont[i]->SetOffset(textPosX, textPosY);
					}
					else
					{
						m_pFont[i]->SetOffset((float)newFontData[i].iXOffset, (float)newFontData[i].iYOffset);
					}
				}

				if (m_FontInfo[i].bUseShadow != newFontData[i].bUseShadow)
				{
					m_FontInfo[i].bUseShadow = newFontData[i].bUseShadow;
					m_FontInfo[i].iShadowColor = newFontData[i].iShadowColor;
					m_FontInfo[i].iShadowXScale = newFontData[i].iShadowXScale;
					m_FontInfo[i].iShadowYScale = newFontData[i].iShadowYScale;
					m_FontInfo[i].iShadowXOffset = newFontData[i].iShadowXOffset;
					m_FontInfo[i].iShadowYOffset = newFontData[i].iShadowYOffset;

					if (m_FontInfo[i].bUseShadow)
					{
						m_pFont[i]->EnableDropShadow(true);
						m_pFont[i]->SetShadowVals(m_FontInfo[i].iShadowColor, m_FontInfo[i].iShadowXScale, m_FontInfo[i].iShadowYScale, m_FontInfo[i].iShadowXOffset, m_FontInfo[i].iShadowYOffset);
					}
					else
					{
						m_pFont[i]->EnableDropShadow(false);
					}
				}
				else if(m_FontInfo[i].bUseShadow)					
				{
					m_FontInfo[i].iShadowColor = newFontData[i].iShadowColor;
					m_FontInfo[i].iShadowXScale = newFontData[i].iShadowXScale;
					m_FontInfo[i].iShadowYScale = newFontData[i].iShadowYScale;
					m_FontInfo[i].iShadowXOffset = newFontData[i].iShadowXOffset;
					m_FontInfo[i].iShadowYOffset = newFontData[i].iShadowYOffset;
					m_pFont[i]->SetShadowVals(m_FontInfo[i].iShadowColor, m_FontInfo[i].iShadowXScale, m_FontInfo[i].iShadowYScale, m_FontInfo[i].iShadowXOffset, m_FontInfo[i].iShadowYOffset);
				}
				
				m_FontInfo[i].clippingRect = newFontData[i].clippingRect;
				m_FontInfo[i].bUseClippingRect = newFontData[i].bUseClippingRect;					

				m_FontInfo[i].bRender = true;
			}
			if (m_bColorChanged)
			{
				StdString colorChanged = MultiplyTextColor(m_ToolBox->GetHashString(dwNewText));
				m_pFont[i]->SetText(colorChanged);				
			}
		}

		// disable any fonts that haven't been updated
		for (UINT j=newFontData.size(); j<m_FontInfo.size(); j++)
		{
			m_FontInfo[j].bRender = false;
		}
	}

	return true;
}

IHashString *CGUIInstance::GetComponentType()
{
	static CHashString CGUIInstanceCT(_T("CGUIInstance"));
	return &CGUIInstanceCT;
}

bool CGUIInstance::IsKindOf(IHashString *compType)
{
	if(CGUIElement::IsKindOf(compType)) 
	{
		return true;
	}
	return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
}


bool CGUIInstance::Render(UINT pass, IEffect * override)
{
	// don't update if no alpha
	if ((m_iColor & 0xff000000) > 0)
	{
		if ((m_ItemRef) && (m_Renderer) && (m_Manager))
		{
			DWORD curOffset = m_VBOffset;
			for (UINT i=0; i<m_VertexInfo.size(); i++)
			{				
				m_Renderer->SetTexture(0, m_VertexInfo[i].pTexture);
				m_Renderer->SetEffect(0, m_VertexInfo[i].pEffect);
				m_Renderer->RenderVertexBuffer(m_VertexBuffer, curOffset, m_VertexInfo[i].iNumPrims);
				curOffset += m_VertexInfo[i].iNumPrims*3;
			}					
		}
		for (UINT i=0; i<m_pFont.size(); i++)
		{
			if (m_pFont[i])
			{
				if (m_FontInfo[i].bRender)
				{
					if (m_FontInfo[i].bUseClippingRect)
					{
						m_Renderer->EnableScissorClip(true);
						
						int guiWidth=0, guiHeight=0;
						m_Manager->GetScreenSize(&guiWidth, &guiHeight);
						
						int viewWidth=0, viewHeight=0;
						m_Renderer->GetViewDimensions(viewWidth, viewHeight);
						
						float xAspect=1.0f, yAspect=1.0f;
						if (guiWidth !=0 && guiHeight != 0)
						{
							xAspect = (float)viewWidth / (float)guiWidth;
							yAspect = (float)viewHeight / (float)guiHeight;
						}

						Matrix4x4 worldMat;
						m_Renderer->GetMatrix(WORLD_MATRIX, worldMat.GetMatrix());

						Vec3 trans = worldMat.GetTranslation();
						Vec3 scale = worldMat.GetScale();

						int x1, y1, x2, y2;

						x1 = (int)(((((float)m_FontInfo[i].clippingRect.left) * scale.x) + trans.x) * xAspect);
						y1 = (int)(((((float)m_FontInfo[i].clippingRect.top) * scale.y) + trans.y) * yAspect);
						x2 = (int)(((((float)m_FontInfo[i].clippingRect.right) * scale.x) + trans.x) * xAspect);
						y2 = (int)(((((float)m_FontInfo[i].clippingRect.bottom) * scale.y) + trans.y) * yAspect);
						
						m_Renderer->SetScissorRect(x1, y1, x2, y2);
					}
					m_pFont[i]->Render();
					if (m_FontInfo[i].bUseClippingRect)
					{
						m_Renderer->EnableScissorClip(false);
					}
				}
			}
		}
	}
	return true;
}

void CGUIInstance::Serialize( IArchive &ar )
{
	if(ar.IsReading())
	{
		CHashString versionName;
		static DWORD msgHash_GetFileVersion = CHashString(_T("GetFileVersion")).GetUniqueID();
		DWORD retval = EngineGetToolBox()->SendMessage(msgHash_GetFileVersion, sizeof(IHashString), &versionName);

		int verNum = 0;
		if (_tcscmp(versionName.GetString(), "1.1c") == 0)
		{
			verNum = 1;
		}
		else if (_tcscmp(versionName.GetString(), "1.1d") == 0)
		{
			verNum = 2;
		}
		else if (_tcscmp(versionName.GetString(), "2.0a") == 0)
		{
			verNum = 3;
		}
		else if (_tcscmp(versionName.GetString(), "2.0b") == 0)
		{
			verNum = 4;
		}
		else if (_tcscmp(versionName.GetString(), "2.0c") == 0)
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

		CGUIElement::Serialize(ar);

		UINT color;
		UINT alpha;
		DWORD mask = 0x00ffffff;
		ar.Read(color);
		ar.Read(alpha);
		
		m_iColor = ((DWORD)color & mask) + ((DWORD)alpha << 24);		
		ar.Read(m_fAngle);
		ar.Read(m_szItemCopyName);
		ar.Read(m_szItemCopyType);
		m_bColorChanged = true;
		CreateItem();
	}
	else
	{
		UINT color;
		UINT alpha;
		DWORD mask = 0x00ffffff;
		color = (UINT)(m_iColor & mask);
		mask = 0xff000000;
		alpha = (UINT)((m_iColor & mask) >> 24);
		ar.Write(m_fSaveVersion, _T("Version"));
		
		CGUIElement::Serialize(ar);

		ar.Write(color, _T("ItemColor"));
		ar.Write(alpha, _T("ItemAlpha"));
		ar.Write(m_fAngle, _T("ItemAngle"));
		ar.Write(m_szItemCopyName, _T("ItemReference"));
		ar.Write(m_szItemCopyType, _T("ItemRefType"));
	}
}

DWORD CGUIInstance::OnLeftMousePress(DWORD msgSize, void *keyStruct)
{
	VERIFY_MESSAGE_SIZE(msgSize, sizeof(MOUSECOORDS));
	MOUSECOORDS mouse = *(MOUSECOORDS*) keyStruct;
	if (m_ItemRef)
	{
		DWORD newState = m_ItemRef->CheckMouseState((float)mouse.m_lXPos, (float)mouse.m_lYPos, MOUSE_LEFTPRESSED, 
			GetPosX(), GetPosY(), GetWidth(), GetHeight(), m_State);
		if (newState != m_State)
		{
			m_State = newState;
			return MSG_HANDLED_STOP;
		}
		else
		{
			return MSG_HANDLED_PROCEED;
		}
	}
	return MSG_NOT_HANDLED;	
}

DWORD CGUIInstance::OnLeftMouseRelease(DWORD msgSize, void *keyStruct)
{
	VERIFY_MESSAGE_SIZE(msgSize, sizeof(MOUSECOORDS));
	MOUSECOORDS mouse = *(MOUSECOORDS*) keyStruct;
	if (m_ItemRef)
	{
		DWORD newState = m_ItemRef->CheckMouseState((float)mouse.m_lXPos, (float)mouse.m_lYPos, MOUSE_LEFTRELEASED, 
			GetPosX(), GetPosY(), GetWidth(), GetHeight(), m_State);
		if (newState != m_State)
		{
			m_State = newState;
			return MSG_HANDLED_STOP;
		}
		else
		{
			return MSG_HANDLED_PROCEED;
		}
	}
	return MSG_NOT_HANDLED;	
}

DWORD CGUIInstance::OnMousePosition(DWORD msgSize, void *keyStruct)
{
	VERIFY_MESSAGE_SIZE(msgSize, sizeof(MOUSECOORDS));
	MOUSECOORDS mouse = *(MOUSECOORDS*) keyStruct;
	if (m_ItemRef)
	{
		DWORD newState = m_ItemRef->CheckMouseState((float)mouse.m_lXPos, (float)mouse.m_lYPos, MOUSE_OPEN, 
			GetPosX(), GetPosY(), GetWidth(), GetHeight(), m_State);
		if (newState != m_State)
		{
			m_State = newState;
			return MSG_HANDLED_STOP;
		}
		else
		{
			return MSG_HANDLED_PROCEED;
		}
	}
	return MSG_NOT_HANDLED;	
}

DWORD CGUIInstance::OnKeyPress(DWORD msgSize, void *keyStruct)
{
	if (m_ItemRef)
	{
//		return m_ItemRef->OnKeyPress(msgSize, keyStruct);
	}
	return MSG_HANDLED_PROCEED;
}

DWORD CGUIInstance::OnSetPosition(DWORD msgSize, void *param)
{
	CGUIElement::OnSetPosition(msgSize, param);
	
	return MSG_HANDLED_STOP;
}

DWORD CGUIInstance::OnSetPositionRelative(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(POSITIONDATA));
	POSITIONDATA pd = *(POSITIONDATA*)param;

	IHashString *parentName = GetParentName();
	POSITIONDATA parentPos;
	parentPos.m_fXPos = 0.0f;
	parentPos.m_fYPos = 0.0f;

	if (parentName != NULL)
	{
		// go ahead and assume parent is a cguigroup... can't be anything else
		static DWORD msgGetPosition = CHashString(_T("GetPosition")).GetUniqueID();
		static CHashString groupType(_T("CGUIGroup"));
		m_ToolBox->SendMessage(msgGetPosition, sizeof(POSITIONDATA), &parentPos, parentName, &groupType);
	}
	
	pd.m_fXPos += parentPos.m_fXPos;
	pd.m_fYPos += parentPos.m_fYPos;

	CGUIElement::OnSetPosition(size, &pd);

	return MSG_HANDLED_STOP;
}

DWORD CGUIInstance::OnGetPositionRelative(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(POSITIONDATA));
	POSITIONDATA *pd = (POSITIONDATA*)param;

	CGUIElement::OnGetPosition(size, pd);

	IHashString *parentName = GetParentName();
	POSITIONDATA parentPos;
	parentPos.m_fXPos = 0.0f;
	parentPos.m_fYPos = 0.0f;

	if (parentName != NULL)
	{
		static DWORD msgGetPosition = CHashString(_T("GetPosition")).GetUniqueID();
		m_ToolBox->SendMessage(msgGetPosition, sizeof(POSITIONDATA), &parentPos, parentName);
	}
	
	pd->m_fXPos -= parentPos.m_fXPos;
	pd->m_fYPos -= parentPos.m_fYPos;	

	return MSG_HANDLED_STOP;
}


DWORD CGUIInstance::OnSetSize(DWORD msgSize, void *param)
{
	CGUIElement::OnSetSize(msgSize, param);
	
	if (m_ItemRef)
	{
		float width, height;
		width = GetWidth();
		height = GetHeight();	
//		m_ItemRef->CheckExtSize(&width, &height);
		SetWidth(width);
		SetHeight(height);
	}
	
	return MSG_HANDLED_STOP;
}

void CGUIInstance::CreateItem()
{
	FINDGUIOBJECT fgo;
	CHashString sName(m_szItemCopyName.c_str());
	CHashString sType(m_szItemCopyType.c_str());
	fgo.name = &sName;

	static DWORD msgHash_FindGUIObject = CHashString(_T("FindGUIObject")).GetUniqueID();
	DWORD retval = EngineGetToolBox()->SendMessage(msgHash_FindGUIObject, sizeof(FINDGUIOBJECT), &fgo);

	if ((_tcscmp(sName.GetString(), _T("")) != 0) && (_tcscmp(sType.GetString(), _T("")) != 0))
	{

		// if the item is already registered with the manager, great!
		if (fgo.bFound)
		{
			if (fgo.object)
			{
				m_ItemRef = dynamic_cast<IGUIReference*>(fgo.object);
			}
		}

		// otherwise, we have to create it in advance and get the object afterwards
		else
		{
			static CHashString pName = _T("World");
			CREATEOBJECTPARAMS cop;
			cop.name = &sName;
			cop.typeName = &sType;
			cop.parentName = &pName;
			static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
			retval = EngineGetToolBox()->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop);
			if (retval == MSG_HANDLED)
			{
				INITOBJECTPARAMS iop;
				iop.name = &sName;
				static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
				retval = EngineGetToolBox()->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop);
			}
		
			static DWORD msgHash_FindGUIObject = CHashString(_T("FindGUIObject")).GetUniqueID();
			retval = EngineGetToolBox()->SendMessage(msgHash_FindGUIObject, sizeof(FINDGUIOBJECT), &fgo);
			
			if (fgo.bFound)
			{
				if (fgo.object)
				{
					m_ItemRef = dynamic_cast<IGUIReference*>(fgo.object);
				}
			}
			else
			{
				EngineGetToolBox()->Log(LOGERROR, _T("GUI object %s could not be created from instance.\n"), sName.GetString());
			}
		}
	}
}

UINT CGUIInstance::UpdateBuffer(void **pDest, UINT offset, IVertexBufferObject *vb)
{
	// don't update if no alpha
	if ((m_iColor & 0xff000000) > 0)
	{
		if ((vb) &&(pDest))
		{
			m_VertexBuffer = vb;
			m_VBOffset = offset;
			UINT vertsWritten = 0;		

			for (UINT i=0; i<m_VertexInfo.size(); i++)
			{
				UINT numVerts = m_VertexInfo[i].iNumPrims * 3;
				memcpy( (*pDest), m_VertexInfo[i].VertexData, numVerts*GUIVERTEXSIZE );	
				vertsWritten += numVerts;

				// advance the pointer
				(*pDest) = (void*)((char*)(*pDest) + (numVerts*GUIVERTEXSIZE));			
			}

			return vertsWritten;
		}
	}
	return 0;
}

DWORD CGUIInstance::OnSetItemRef(DWORD size, void *param)
{
    VERIFY_MESSAGE_SIZE(size, sizeof(CHashString));
	CHashString name = *(CHashString*) param;
	
	FINDGUIOBJECT fgo;
	fgo.name = &name;
	static DWORD msgHash_FindGUIObject = CHashString(_T("FindGUIObject")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_FindGUIObject, sizeof(FINDGUIOBJECT), &fgo);
	if (fgo.bFound)
	{
		if (fgo.object)
		{
			m_ItemRef = dynamic_cast<IGUIReference*>(fgo.object);
		}
	}
	m_szItemCopyName = name.GetString();
	m_szItemCopyType = fgo.type->GetString();
	return MSG_HANDLED_STOP;
}

DWORD CGUIInstance::OnGetItemRef(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(IHashString));
	IHashString *name = (IHashString*) param;
	CHashString temp(m_szItemCopyName.c_str());
	*name = temp.GetString();
	return MSG_HANDLED_STOP;
}

StdString CGUIInstance::RemoveFormatting(StdString inStr)
{
	StdString outStr, tempToken;
	inStr.GetToken(_T("["), tempToken);
	
	do
	{
		outStr += tempToken;
		inStr.GetToken(_T("]"), tempToken);
		inStr.GetToken(_T("["), tempToken);
	}while (_tcscmp((const TCHAR*)tempToken, _T("")) != 0);
	return outStr;
}

DWORD CGUIInstance::OnGUIAnimPosX(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(float));
	float data = *(float*) param;

	POSITIONDATA pd;
	pd.m_fXPos = data;
	pd.m_fYPos = GetPosY();
	OnSetPosition(sizeof(POSITIONDATA), &pd);
	return MSG_HANDLED_STOP;
}

DWORD CGUIInstance::OnGUIAnimPosY(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(float));
	float data = *(float*) param;

	POSITIONDATA pd;
	pd.m_fXPos = GetPosX();
	pd.m_fYPos = data;
	OnSetPosition(sizeof(POSITIONDATA), &pd);
	return MSG_HANDLED_STOP;
}

DWORD CGUIInstance::OnGUIAnimSizeX(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(float));
	float data = *(float*) param;

	SIZEDATA sd;
	sd.m_fXSize = data;
	sd.m_fYSize = GetHeight();
	OnSetSize(sizeof(SIZEDATA), &sd);
	return MSG_HANDLED_STOP;
}

DWORD CGUIInstance::OnGUIAnimSizeY(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(float));
	float data = *(float*) param;

	SIZEDATA sd;
	sd.m_fXSize = GetWidth();
	sd.m_fYSize = data;
	OnSetSize(sizeof(SIZEDATA), &sd);
	return MSG_HANDLED_STOP;
}

DWORD CGUIInstance::OnGUIAnimAlphaCol(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(float));
	float data = *(float*) param;

	if (data < 0.f)
	{
		data = 0.f;
	}
	if (data > 255.f)
	{
		data = 255.f;
	}

	DWORD mask = 0x00ffffff;
	DWORD noAlpha = m_iColor & mask;
	DWORD justAlpha = (DWORD)data << 24;
	m_iColor = justAlpha + noAlpha;
	m_bColorChanged = true;
	return MSG_HANDLED_STOP;
}

DWORD CGUIInstance::OnGUIAnimRCol(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(float));
	float data = *(float*) param;

	if (data < 0.f)
	{
		data = 0.f;
	}
	if (data > 255.f)
	{
		data = 255.f;
	}
	DWORD mask = 0xff00ffff;
	DWORD noR = m_iColor & mask;
	DWORD justR = (DWORD)data << 16;
	m_iColor = justR + noR;
	m_bColorChanged = true;
	return MSG_HANDLED_STOP;
}

DWORD CGUIInstance::OnGUIAnimGCol(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(float));
	float data = *(float*) param;

	if (data < 0.f)
	{
		data = 0.f;
	}
	if (data > 255.f)
	{
		data = 255.f;
	}
	DWORD mask = 0xffff00ff;
	DWORD noG = m_iColor & mask;
	DWORD justG = (DWORD)data << 8;
	m_iColor = justG + noG;
	m_bColorChanged = true;
	return MSG_HANDLED_STOP;
}

DWORD CGUIInstance::OnGUIAnimBCol(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(float));
	float data = *(float*) param;

	if (data < 0.f)
	{
		data = 0.f;
	}
	if (data > 255.f)
	{
		data = 255.f;
	}
	DWORD mask = 0xffffff00;
	DWORD noB = m_iColor & mask;
	DWORD justB = (DWORD)data;
	m_iColor = justB + noB;
	m_bColorChanged = true;
	return MSG_HANDLED_STOP;
}

DWORD CGUIInstance::OnSetColor(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(DWORD));
	DWORD col = *(DWORD*)param;
	m_iColor = col;
	m_bColorChanged = true;
	return MSG_HANDLED_STOP;
}

DWORD CGUIInstance::OnGetColor(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(DWORD));
	DWORD *col = (DWORD*)param;
	*col = m_iColor;
	return MSG_HANDLED_STOP;
}
DWORD CGUIInstance::OnSetAlpha(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(DWORD));
	DWORD data = *(DWORD*)param;

	DWORD mask = 0x00ffffff;
	DWORD noAlpha = m_iColor & mask;
	DWORD justAlpha = data << 24;
	m_iColor = justAlpha + noAlpha;
	m_bColorChanged = true;
	return MSG_HANDLED_STOP;	
}

StdString CGUIInstance::MultiplyTextColor(StdString inStr)
{
	StdString strOut = _T(""), token;
	StdString flag;
	
	inStr.GetToken(_T("["), token);
	strOut += token;
	inStr.GetToken(_T("]"), flag);
	while (flag != _T(""))
	{
		StdString symbol;
		flag.GetToken(_T("="), symbol);
		if (_tcscmp(symbol, _T("FColor")) == 0)
		{
			StdString rStr, gStr, bStr, aStr;
			flag.GetToken(_T(","), rStr);
			flag.GetToken(_T(","), gStr);
			flag.GetToken(_T(","), bStr);
			flag.GetToken(_T(","), aStr);

			float curR, curG, curB, curA, readR, readG, readB, readA;
			UINT r, g, b, a;
			// NOTE: m_iColor = argb
			curA = (float)((m_iColor & 0xff000000) >> 24) / 256.f;
			curR = (float)((m_iColor & 0x00ff0000) >> 16) / 256.f;
			curG = (float)((m_iColor & 0x0000ff00) >> 8) / 256.f;
			curB = (float)(m_iColor & 0x000000ff) / 256.f;

			readR = (float)atof(rStr) / 256.f;
			readG = (float)atof(gStr) / 256.f;
			readB = (float)atof(bStr) / 256.f;
			readA = (float)atof(aStr) / 256.f;

			r = (UINT)((curR*readR)*256.f);
			g = (UINT)((curG*readG)*256.f);
			b = (UINT)((curB*readB)*256.f);
			a = (UINT)((curA*readA)*256.f);

			TCHAR buf[100];
			_stprintf(buf, "[FColor=%d,%d,%d,%d]", r, g, b, a);
			strOut += buf;
		}
		
		else if (_tcscmp(symbol, _T("/FColor")) == 0)
		{
			StdString rStr, gStr, bStr, aStr;
			flag.GetToken(_T(","), rStr);
			flag.GetToken(_T(","), gStr);
			flag.GetToken(_T(","), bStr);
			flag.GetToken(_T(","), aStr);

			if (_tcscmp(rStr, _T("")) != 0)
			{

				float curR, curG, curB, curA, readR, readG, readB, readA;
				UINT r, g, b, a;
				// NOTE: m_iColor = argb
				curA = (float)((m_iColor & 0xff000000) >> 24) / 256.f;
				curR = (float)((m_iColor & 0x00ff0000) >> 16) / 256.f;
				curG = (float)((m_iColor & 0x0000ff00) >> 8) / 256.f;
				curB = (float)(m_iColor & 0x000000ff) / 256.f;

				readR = (float)atof(rStr) / 256.f;
				readG = (float)atof(gStr) / 256.f;
				readB = (float)atof(bStr) / 256.f;
				readA = (float)atof(aStr) / 256.f;

				r = (UINT)((curR*readR)*256.f);
				g = (UINT)((curG*readG)*256.f);
				b = (UINT)((curB*readB)*256.f);
				a = (UINT)((curA*readA)*256.f);

				TCHAR buf[100];
				_stprintf(buf, "[FColor=%d,%d,%d,%d]", r, g, b, a);
				strOut += buf;
			}
			else 
			{
				strOut += _T("[/FColor]");
			}
		}
		else
		{
			strOut += _T("[");
			strOut += flag;
			strOut += _T("]");
		}
		inStr.GetToken(_T("["), token);
		strOut += token;
		inStr.GetToken(_T("]"), flag);
	}

	m_bColorChanged = false;

	return strOut;
}

DWORD CGUIInstance::OnGetAngle(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(float));
	float *ret = (float*)param;
	*ret = m_fAngle;
	return MSG_HANDLED_STOP;
}

DWORD CGUIInstance::OnSetAngle(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(float));
	float val = *(float*) param;
	m_fAngle = val;
	return MSG_HANDLED_STOP;
}

DWORD CGUIInstance::OnSetAngleFromLua(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(GUIFLOAT));
	GUIFLOAT gf = *(GUIFLOAT*) param;
	m_fAngle = gf.data;

	return MSG_HANDLED_STOP;
}

DWORD CGUIInstance::OnSelectBtPressed(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(bool));
	bool *doneToggle = (bool*)param;

	if (m_ItemRef)
	{
		DWORD newState = m_ItemRef->ToggleSelect(m_State);
		if (newState == m_State)
		{
			*doneToggle = true;
		}
		else
		{
			*doneToggle = false;
			m_State = newState;
		}
	}
	else
	{
		*doneToggle = true;
	}
	
	return MSG_HANDLED_STOP;
}
	
DWORD CGUIInstance::OnBackBtPressed(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(bool));
	bool *doneToggle = (bool*)param;

	if (m_ItemRef)
	{
		DWORD newState = m_ItemRef->ToggleBack(m_State);
		if (newState == m_State)
		{
			*doneToggle = true;
		}
		else
		{
			*doneToggle = false;
			m_State = newState;
		}
	}
	else
	{
		*doneToggle = true;
	}

	return MSG_HANDLED_STOP;
}

DWORD CGUIInstance::OnToggleUpBtPressed(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(bool));
	bool *doneToggle = (bool*)param;

	if (m_ItemRef)
	{
		DWORD newState = m_ItemRef->ToggleUp(m_State);
		if (newState == m_State)
		{
			*doneToggle = true;
		}
		else
		{
			*doneToggle = false;
			m_State = newState;
		}
	}
	else
	{
		*doneToggle = true;
	}

	return MSG_HANDLED_STOP;
}

DWORD CGUIInstance::OnToggleDownBtPressed(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(bool));
	bool *doneToggle = (bool*)param;

	if (m_ItemRef)
	{
		DWORD newState = m_ItemRef->ToggleDown(m_State);
		if (newState == m_State)
		{
			*doneToggle = true;
		}
		else
		{
			*doneToggle = false;
			m_State = newState;
		}
	}
	else
	{
		*doneToggle = true;
	}

	return MSG_HANDLED_STOP;
}

DWORD CGUIInstance::OnGetItemText(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(IHashString*));
	IHashString* hsParam = (IHashString*)param;

	if ((hsParam != NULL) && (m_ItemRef != NULL))
	{
		DWORD textHash = m_ItemRef->GetHashText(m_State);
		if (textHash != 0)
		{
			*hsParam = m_ToolBox->GetHashString(textHash);
		}
	}

	return MSG_HANDLED_STOP;
}

DWORD CGUIInstance::OnSetItemText(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(IHashString*));
	IHashString* hsParam = (IHashString*)param;
	
	if ((hsParam != NULL) && (m_ItemRef != NULL))
	{
		m_State = m_ItemRef->SetHashText(m_State, hsParam->GetUniqueID());
	}

	return MSG_HANDLED_STOP;
}

DWORD CGUIInstance::OnGetGUIParentName(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(IHashString*));
	IHashString *hsName = (IHashString*)param;
	IHashString *parentName = GetParentName();
	if ((hsName) && (parentName))
	{
		*hsName = parentName->GetString();
	}

	return MSG_HANDLED_STOP;
}