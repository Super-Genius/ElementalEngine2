///============================================================================
/// \file		CGUIStaticText.cpp
/// \brief		Header file for CGUIStaticText
///				An adapter into Visions' VMenuItem_cl
///				Used for static text
/// \date		03-10-2005
/// \author		Halbert Nakagawa
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
#include "CGUIStaticText.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT(CGUIStaticText);

REGISTER_MESSAGE_HANDLER(SetText, OnSetText, CGUIStaticText);
REGISTER_MESSAGE_HANDLER(GetText, OnGetText, CGUIStaticText);
REGISTER_MESSAGE_HANDLER(SetTextFile, OnSetTextFile, CGUIStaticText);
REGISTER_MESSAGE_HANDLER(SetTextColor, OnSetTextColor, CGUIStaticText);
REGISTER_MESSAGE_HANDLER(UpdateLanguageEntryParams, OnUpdateLanguageEntryParams, CGUIStaticText);
REGISTER_MESSAGE_HANDLER(ChangeLanguageEntryKey, OnChangeLanguageEntryKey, CGUIStaticText);

CGUIStaticText::CGUIStaticText(IHashString *parentName, IHashString *name) :
	OBJECTTEMPLATE(CGUIManager, CGUIStaticText, IGUIReference, parentName, name)	
{
	m_pStaticText = new GUIButton();

	//set all members to default vals
	m_szNorm = _T("");
	m_szText = m_szFont = _T("");
	m_iNormFontColor = -1;
	m_iHighFontColor = -1;
	m_iSelectFontColor = -1;
	m_iGrayFontColor = -1;
	m_fCursorPosX = m_fCursorPosY = -1;
	m_bHideTexture = false;

	m_iTextOffSetX = m_iTextOffSetY = 0;
	m_bCenterText = false;
	m_iFontsize = 20; // default to 20 pt font

	m_bLoadFromFile = false;
	
	m_bUseFontShadow = false;
	m_iFontShadowColor = 0xff000000;
	m_iFontShadowXScale = 0;
	m_iFontShadowYScale = 0;
	m_iFontShadowXOffset = 0;
	m_iFontShadowYOffset = 0;

	m_bUseLanguageEntry = false;
	m_szLangEntryKey = _T("");

	m_LanguageTextParams = NULL;
	m_iLangTextParamSize = 0;
}

CGUIStaticText::~CGUIStaticText()
{
	delete m_pStaticText;
	if (m_LanguageTextParams != NULL)
	{
		delete [] m_LanguageTextParams;
		m_LanguageTextParams = NULL;
	}
}

 IComponent *CGUIStaticText::Create(int nArgs, va_list argptr)
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
		self = new CGUIStaticText(parentName, name);
	}
	catch(...)
	{
		return NULL;
	}
	return self;
}

bool CGUIStaticText::Update()
{
	return true;
}

IHashString *CGUIStaticText::GetComponentType()
{
	static CHashString CGUIStaticTextCT(_T("CGUIStaticText"));
	return &CGUIStaticTextCT;
}

bool CGUIStaticText::IsKindOf(IHashString *compType)
{
	return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
}

void CGUIStaticText::Serialize( IArchive &ar )
{
	float fSaveVersion = 1.2f;
	if(ar.IsReading())
	{
		CHashString versionName;
		static DWORD msgHash_GetFileVersion = CHashString(_T("GetFileVersion")).GetUniqueID();
		DWORD retval = EngineGetToolBox()->SendMessage(msgHash_GetFileVersion, sizeof(IHashString), &versionName);

		static DWORD vers2_0a = CHashString(_T("2.0a")).GetUniqueID();
		static DWORD vers2_0b = CHashString(_T("2.0b")).GetUniqueID();
		static DWORD vers2_0c = CHashString(_T("2.0c")).GetUniqueID();

		int verNum = 0;
		if (versionName.GetUniqueID() == vers2_0a)
		{
			verNum = 3;
		}
		else if (versionName.GetUniqueID() == vers2_0b)
		{
			verNum = 4;
		}
		else if (versionName.GetUniqueID() == vers2_0c)
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
				loadVersion = fSaveVersion;
			}
		}

		ar.Read(m_szNorm);
		ar.Read(m_szText);
		ar.Read(m_szFilename);
		ar.Read(m_bLoadFromFile);

		if (loadVersion >= 1.2f)
		{
			ar.Read(m_bUseLanguageEntry);
			ar.Read(m_szLangEntryKey);
		}
		
		ar.Read(m_szFont);
		ar.Read(m_iFontsize);
		
		ar.Read(m_iNormFontColor);
		ar.Read(m_iHighFontColor);
		ar.Read(m_iSelectFontColor);
		ar.Read(m_iGrayFontColor);
		ar.Read(m_iTextOffSetX);
		ar.Read(m_iTextOffSetY);
		ar.Read(m_bCenterText);
		ar.Read(m_bHideTexture);

		if (loadVersion >= 1.1f)
		{
			ar.Read(m_bUseFontShadow, _T("UseFontDropShadow"));
			ar.Read(m_iFontShadowColor, _T("FontShadowColor"));
			ar.Read(m_iFontShadowXScale, _T("FontShadowXScale"));
			ar.Read(m_iFontShadowYScale, _T("FontShadowYScale"));
			ar.Read(m_iFontShadowXOffset, _T("FontShadowXOffset"));
			ar.Read(m_iFontShadowYOffset, _T("FontShadowYOffset"));
		}

		CreateItem();
	}
	else
	{
		ar.Write(fSaveVersion, _T("Version"));
		ar.Write(m_szNorm, _T("NormTexture"));
		ar.Write(m_szText, _T("StaticText"));
		ar.Write(m_szFilename, _T("Filename"));
		ar.Write(m_bLoadFromFile, _T("LoadFromFile"));
		ar.Write(m_bUseLanguageEntry, _T("UseLanguageEntry"));
		ar.Write(m_szLangEntryKey, _T("LanguageEntryKey"));
		ar.Write(m_szFont, _T("FontName"));
		ar.Write(m_iFontsize, _T("Fontsize"));
		ar.Write(m_iNormFontColor, _T("NormFontColor"));
		ar.Write(m_iHighFontColor, _T("HighFontColor"));
		ar.Write(m_iSelectFontColor, _T("SelectFontColor"));
		ar.Write(m_iGrayFontColor, _T("GrayFontColor"));
		ar.Write(m_iTextOffSetX, _T("TextOffSetX"));
		ar.Write(m_iTextOffSetY, _T("TextOffSetY"));
		ar.Write(m_bCenterText, _T("CenterText"));
		ar.Write(m_bHideTexture, _T("HideTexture"));
		ar.Write(m_bUseFontShadow, _T("UseFontDropShadow"));
		ar.Write(m_iFontShadowColor, _T("FontShadowColor"));
		ar.Write(m_iFontShadowXScale, _T("FontShadowXScale"));
		ar.Write(m_iFontShadowYScale, _T("FontShadowYScale"));
		ar.Write(m_iFontShadowXOffset, _T("FontShadowXOffset"));
		ar.Write(m_iFontShadowYOffset, _T("FontShadowYOffset"));
	}
}

bool CGUIStaticText::LoadTextFile(const TCHAR *szFilename)
{
	if (_tcscmp(szFilename, "") != 0)
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
			m_szText = szTemp;
			return true;
		}
		else 
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

void CGUIStaticText::CreateItem()
{
	m_pStaticText->SetScreenMasks(m_szNorm, m_szNorm, m_szNorm, m_szNorm);

	m_pStaticText->ShowTexture(!m_bHideTexture);

	m_pStaticText->SetTextColors(m_iNormFontColor, m_iHighFontColor, m_iSelectFontColor, m_iGrayFontColor);
	m_pStaticText->SetTextPos(m_iTextOffSetX, m_iTextOffSetY);

	if (m_bUseLanguageEntry)
	{
		UpdateLanguageEntryText();
	}
	else
	{
		if (!m_bLoadFromFile)
		{
			if(!m_szFont.empty() && !m_szText.empty())
			{
				m_pStaticText->SetText(m_szFont, "", m_szText, m_szText, m_szText, m_szText, m_iFontsize);
			}
			else
			{
				m_pStaticText->SetText(m_szFont, "", "", "", "", "", 0);
			}
		}
		else
		{
			LoadTextFile(m_szFilename);
			if(!m_szFont.empty() && !m_szText.empty())
			{
				m_pStaticText->SetText(m_szFont, "", m_szText, m_szText, m_szText, m_szText, m_iFontsize);
			}
			else
			{
				m_pStaticText->SetText(m_szFont, "", "", "", "", "", 0);
			}
		}
	}
	m_pStaticText->CenterText(m_bCenterText);
	m_pStaticText->SetSmartScale(true);

	if (m_bUseFontShadow)
	{
		DWORD color = 0xff000000;
		color += m_iFontShadowColor;
		m_pStaticText->EnableFontShadow(true);
		m_pStaticText->SetFontShadowVals(color, m_iFontShadowXScale, m_iFontShadowYScale, m_iFontShadowXOffset, m_iFontShadowYOffset);
	}
	else
	{
		m_pStaticText->EnableFontShadow(false);	
	}	
}

DWORD CGUIStaticText::OnSetText(DWORD msgSize, void *param)
{
	TEXTDATA *td;
	VERIFY_MESSAGE_SIZE(msgSize, sizeof(TEXTDATA));

	if (m_bUseLanguageEntry)
	{
		m_ToolBox->Log(LOGWARNING, _T("Set to use language entry; use alternate message for updating variables in string\n"));
		return MSG_NOT_HANDLED;
	}
	else
	{
		td  = (TEXTDATA*)param;
		if (td->text)
		{
			m_szText = td->text->GetString();
			m_pStaticText->SetText(m_szFont, "", m_szText, m_szText, m_szText, m_szText, m_iFontsize);
		}
		else // text is null, clear text here
		{
			m_pStaticText->SetText(m_szFont, "", "", "", "", "", m_iFontsize);
		}
	}
	return MSG_HANDLED_STOP;
}

DWORD CGUIStaticText::OnGetText(DWORD msgSize, void *param)
{
	TEXTDATA *td;
	VERIFY_MESSAGE_SIZE(msgSize, sizeof(TEXTDATA));
	
	td = (TEXTDATA*)param;
	CHashString chsText(m_szText);
	td->text = &chsText;

	return MSG_HANDLED_STOP;
}

DWORD CGUIStaticText::OnSetTextFile(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(IHashString));
	IHashString *filename = (IHashString*) param;

	if (m_bUseLanguageEntry)
	{
		m_ToolBox->Log(LOGWARNING, _T("Set to use language entry; use alternate message for updating variables in string\n"));
		return MSG_NOT_HANDLED;
	}
	else
	{
		m_bLoadFromFile = true;
		LoadTextFile(filename->GetString());
		m_pStaticText->SetText(m_szFont, "", m_szText, m_szText, m_szText, m_szText, m_iFontsize);

		return MSG_HANDLED_STOP;
	}
}

void CGUIStaticText::GetRenderParams(vector<GUIVERTEXINFO> &vertexData, float x, float y, float width, float height, 
									 DWORD state, float angle, DWORD color)
{
	if ((m_pStaticText) && (!m_bHideTexture))
	{
		m_pStaticText->LoadVertexBuffer(vertexData, x, y, width, height, state, angle, color);
	}
}

void CGUIStaticText::GetFontParams(vector<GUIFONTINFO> &fontData, float x, float y, float width, float height, 
						   DWORD state, float angle, DWORD color)
{
	if (m_pStaticText)
	{
		m_pStaticText->UpdateFontExt(fontData, x, y, width, height, state);
	}
}

DWORD CGUIStaticText::OnSetTextColor(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(DWORD));
	DWORD color = *(DWORD*)param;

	if (m_pStaticText)
	{
		m_pStaticText->SetTextColors((int)color, (int)color, (int)color, (int)color);
	}
	return MSG_HANDLED_STOP;
}

DWORD CGUIStaticText::GetHashText(DWORD inState)
{
	CHashString hsText(m_szText);
	return hsText.GetUniqueID();
}

DWORD CGUIStaticText::SetHashText(DWORD inState, DWORD hashText)
{
	if (m_bUseLanguageEntry)
	{
		m_ToolBox->Log(LOGWARNING, _T("Set to use language entry; use alternate message for updating variables in string\n"));
	}
	else
	{
		m_szText = m_ToolBox->GetHashString(hashText);
		if (m_pStaticText)
		{
			m_pStaticText->SetText(m_szFont, "", m_szText, m_szText, m_szText, m_szText, m_iFontsize);
		}
	}
	return inState;
}

void CGUIStaticText::FlagLanguageEntryChanged()
{
	if (m_bUseLanguageEntry)
	{
		UpdateLanguageEntryText();
	}
}

void CGUIStaticText::UpdateLanguageEntryText()
{
	CHashString hszLangKey(m_szLangEntryKey);
	CHashString hszLangTextOut(_T(""));

	static DWORD dwZeroString = CHashString(_T("")).GetUniqueID();
	if (hszLangKey.GetUniqueID() != dwZeroString)
	{
		IArchive *ar = CreateAndFillArchive();
		
		GETLANGUAGETEXTPARAMS gltp;
		gltp.arParams = ar;
		gltp.hszKeyString = &hszLangKey;
		gltp.hszTranslatedText = &hszLangTextOut;
		static DWORD msgGetLanguageText = CHashString(_T("GetLanguageText")).GetUniqueID();

		m_ToolBox->SendMessage(msgGetLanguageText, sizeof(GETLANGUAGETEXTPARAMS), &gltp);

		if (m_pStaticText)
		{
			m_pStaticText->SetText(m_szFont, "", hszLangTextOut.GetString(), hszLangTextOut.GetString(), hszLangTextOut.GetString(), 
				hszLangTextOut.GetString(), m_iFontsize);
		}

		if (ar)
		{
			ar->Close();
		}
	}
}

IArchive* CGUIStaticText::CreateAndFillArchive()
{
	if (m_LanguageTextParams)
	{
		CREATEARCHIVE ca;
		static CHashString memType(_T("Memory"));

		ca.mode = STREAM_MODE_WRITE | STREAM_MODE_READ;
		ca.streamData = NULL;
		ca.streamSize = 0;
		ca.streamType = &memType;
		static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca);

		IArchive *ar = ca.archive;
		if (ar)
		{
			ar->SetIsWriting(true);
			ar->SeekTo(0);
			ar->Write(m_LanguageTextParams, m_iLangTextParamSize);
		}
		return ar;
	}
	else
	{
		return NULL;
	}
}

DWORD CGUIStaticText::OnUpdateLanguageEntryParams(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(IArchive*));
	IArchive* ar = (IArchive*) param;

	if (ar)
	{
		if (m_LanguageTextParams != NULL)
		{
			delete [] m_LanguageTextParams;
			m_LanguageTextParams = NULL;
		}
		// do deep copy of archive params
		UINT archiveSize = ar->SeekTo(0, SEEK_END);

		if (archiveSize > 0)
		{		
			ar->SeekTo(0, SEEK_SET);
			ar->SetIsWriting(false);

			m_LanguageTextParams = new BYTE[archiveSize];
			ar->Read(m_LanguageTextParams, archiveSize);

			m_iLangTextParamSize = archiveSize;
		}
	}

	UpdateLanguageEntryText();

	return MSG_HANDLED_STOP;
}

DWORD CGUIStaticText::OnChangeLanguageEntryKey(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(CHANGELANGUAGEKEYPARAMS));
	CHANGELANGUAGEKEYPARAMS clkp = *(CHANGELANGUAGEKEYPARAMS*)param;

	if (m_bUseLanguageEntry)
	{
		if (clkp.hszKey)
		{
			m_szLangEntryKey = clkp.hszKey->GetString();

			// regardless of whether or not the archive is null, we need to nullify our current data,
			// since this key probably doesn't have the same params or param order
			if (m_LanguageTextParams)
			{
				delete [] m_LanguageTextParams;
				m_LanguageTextParams = NULL;
			}

			// this will also update text
			OnUpdateLanguageEntryParams(sizeof(IArchive*), clkp.archive);
		}

		return MSG_HANDLED_STOP;
	}
	else
	{
		m_ToolBox->Log(LOGWARNING, _T("%s %d: not set up to use a language entry!\n"), 
			__FILE__, __LINE__);
		return MSG_NOT_HANDLED;
	}
}