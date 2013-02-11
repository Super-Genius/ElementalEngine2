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
#include "CGUIComboBox.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	COMBOSIZE 32

REGISTER_COMPONENT(CGUIComboBox);

REGISTER_MESSAGE_HANDLER(GetComboData, OnGetComboData, CGUIComboBox);
REGISTER_MESSAGE_HANDLER(AddEntry, OnAddEntry, CGUIComboBox);
REGISTER_MESSAGE_HANDLER(ClearComboBox, OnClearComboBox, CGUIComboBox);

CGUIComboBox::CGUIComboBox(IHashString *parentName, IHashString *name) :
	OBJECTTEMPLATE(CGUIManager, CGUIComboBox, IGUIReference, parentName, name)	
{
	// set all members to default vals
	m_szSelectBoxNormTex = _T("");
	m_szSelectBoxGrayTex = _T("");
	
	m_szArrowNormTex = _T("");
	m_szArrowSelTex = _T("");
	m_szArrowGrayTex = _T("");

	m_szDropDownTex = _T("");
	m_szHighlightTex = _T("");

	m_szListFile = _T("");
	m_szFont = _T("");

	m_iNormFontColor = m_iSelectFontColor = m_iGrayFontColor = 0;

	m_fCursorPosX = m_fCursorPosY = -1;

	m_szDefaultText = _T("");

	m_iTextOffSetX = 0;
	m_iTextOffSetY = 0;

	m_iFontsize = 20; // default to 20 pt font

	m_bButtonOnRight = false;

	m_iListTextOffsetX = 0;
	m_iListTextOffsetY = 0;
	m_iListBoxOffsetY = 0;
	m_iListBoxHeightOffset = 0;
}

CGUIComboBox::~CGUIComboBox()
{
}

IComponent *CGUIComboBox::Create(int nArgs, va_list argptr)
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
		self = new CGUIComboBox(parentName, name);
	}
	catch(...)
	{
		return NULL;
	}
	return self;
}

bool CGUIComboBox::Update()
{
	return true;
}

IHashString *CGUIComboBox::GetComponentType()
{
	static CHashString CGUIComboBoxCT(_T("CGUIComboBox"));
	return &CGUIComboBoxCT;
}

bool CGUIComboBox::IsKindOf(IHashString *compType)
{
	return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
}

void CGUIComboBox::Serialize( IArchive &ar )
{
	static float fSaveVersion = 1.1f;
	if(ar.IsReading())
	{

		CHashString versionName;
		static DWORD msgHash_GetFileVersion = CHashString(_T("GetFileVersion")).GetUniqueID();
		DWORD retval = EngineGetToolBox()->SendMessage(msgHash_GetFileVersion, sizeof(IHashString), &versionName);

		static DWORD Vers2_0b = CHashString(_T("2.0b")).GetUniqueID();
		static DWORD Vers2_0c = CHashString(_T("2.0c")).GetUniqueID();

		int verNum = 0;

		if (versionName.GetUniqueID() == Vers2_0b)
		{
			verNum = 1;
		}
		else if (versionName.GetUniqueID() == Vers2_0c)
		{
			verNum = 2;
		}
		else 
		{
			m_ToolBox->Log(LOGERROR, _T("Bad GUI file version!"));
			assert(0);
			return;
		}

		float fComboVersion = 0.0f;
		ar.Read(fComboVersion);
		if (fComboVersion == 0.0f)
		{
			// was read in from gui editor, set to save version (serialization is up to date)
			fComboVersion = fSaveVersion;
		}
		ar.Read(m_szSelectBoxNormTex);
		ar.Read(m_szSelectBoxGrayTex);
		ar.Read(m_szArrowNormTex);
		ar.Read(m_szArrowSelTex);
		ar.Read(m_szArrowGrayTex);
		ar.Read(m_szDropDownTex);
		if (fComboVersion >= 1.1f)
		{
			ar.Read(m_iListBoxOffsetY, _T("DropDownOffsetY"));
			ar.Read(m_iListBoxHeightOffset, _T("DropDownHeightOffset"));
		}
		ar.Read(m_szHighlightTex);
		ar.Read(m_szDefaultText);
		ar.Read(m_szListFile);
		ar.Read(m_szFont);
		ar.Read(m_iFontsize);		
		ar.Read(m_iNormFontColor);
		ar.Read(m_iSelectFontColor);
		ar.Read(m_iGrayFontColor);
		ar.Read(m_iTextOffSetX);
		ar.Read(m_iTextOffSetY);
		if (fComboVersion >= 1.1f)
		{
			ar.Read(m_iListTextOffsetX, _T("ListTextOffsetX"));
			ar.Read(m_iListTextOffsetY, _T("ListTextOffsetY"));
		}
		ar.Read(m_bButtonOnRight);

		CreateItem();
	}
	else
	{
		ar.Write(fSaveVersion, _T("Version"));
		ar.Write(m_szSelectBoxNormTex, _T("ComboBoxNormTex"));
		ar.Write(m_szSelectBoxGrayTex, _T("ComboBoxGrayTex"));
		ar.Write(m_szArrowNormTex, _T("ArrowNormTex"));
		ar.Write(m_szArrowSelTex, _T("ArrowSelTex"));
		ar.Write(m_szArrowGrayTex, _T("ArrowGrayTex"));
		ar.Write(m_szDropDownTex, _T("DropDownTex"));
		ar.Write(m_iListBoxOffsetY, _T("DropDownOffsetY"));
		ar.Write(m_iListBoxHeightOffset, _T("DropDownHeightOffset"));
		ar.Write(m_szHighlightTex, _T("HighlightTex"));
		ar.Write(m_szDefaultText, _T("DefaultText"));
		ar.Write(m_szListFile, _T("DropDownList"));
		ar.Write(m_szFont, _T("FontName"));
		ar.Write(m_iFontsize, _T("Fontsize"));
		ar.Write(m_iNormFontColor, _T("NormFontColor"));
		ar.Write(m_iSelectFontColor, _T("SelectFontColor"));
		ar.Write(m_iGrayFontColor, _T("GrayFontColor"));
		ar.Write(m_iTextOffSetX, _T("TextOffSetX"));
		ar.Write(m_iTextOffSetY, _T("TextOffSetY"));
		ar.Write(m_iListTextOffsetX, _T("ListTextOffsetX"));
		ar.Write(m_iListTextOffsetY, _T("ListTextOffsetY"));
		ar.Write(m_bButtonOnRight, _T("ButtonOnRight"));		
	}
}

DWORD CGUIComboBox::OnKeyPress(DWORD msgSize, void *keyStruct)
{
	return MSG_HANDLED_PROCEED;
}

void CGUIComboBox::CreateItem()
{
	m_ComboBox.SetTextPos(m_iTextOffSetX, m_iTextOffSetY);
	m_ComboBox.SetSelectBox(m_szSelectBoxNormTex, m_szSelectBoxNormTex, m_szSelectBoxNormTex, m_szSelectBoxGrayTex);
	m_ComboBox.SetArrowBox(m_szArrowNormTex, m_szArrowSelTex, m_szArrowGrayTex);
	m_ComboBox.SetFont(m_szFont, m_iFontsize);
	m_ComboBox.SetDropDownBox(m_szDropDownTex, m_szListFile, m_iListBoxOffsetY, m_iListBoxHeightOffset, m_iListTextOffsetX, m_iListTextOffsetY);
	m_ComboBox.SetTextColors(m_iNormFontColor, m_iNormFontColor, m_iSelectFontColor, m_iGrayFontColor);
	m_ComboBox.SetDefaultText(m_szDefaultText);
	m_ComboBox.SetButtonOnRight(m_bButtonOnRight);	
	m_ComboBox.SetHighlightTex(m_szHighlightTex);
}

DWORD CGUIComboBox::OnGetComboData(DWORD msgSize, void *param)
{
	COMBODATA *cd;
	VERIFY_MESSAGE_SIZE(msgSize, sizeof(COMBODATA));
	
	cd = (COMBODATA*)param;

	cd->text = (TCHAR*)m_ComboBox.GetSelectedText();
	cd->index = m_ComboBox.GetSelectedIndex();

	return MSG_HANDLED_STOP;
}

DWORD CGUIComboBox::OnClearComboBox(DWORD msgSize, void *param)
{
	m_ComboBox.Reset();
	return MSG_HANDLED_STOP;
}

DWORD CGUIComboBox::OnAddEntry(DWORD msgSize, void *param)
{
	CColorRef color;
	StdString szText;
	ADDLISTBOXENTRY *albe;
	VERIFY_MESSAGE_SIZE(msgSize, sizeof(ADDLISTBOXENTRY));
	
	albe = (ADDLISTBOXENTRY*)param;

	szText = albe->entryText;
	color = CColorRef(albe->iRedColor, albe->iGreenColor, albe->iBlueColor);

	// add a '\n' char to the end of sztext
	szText += _T("\n");

	m_ComboBox.AddEntry(szText, color);

	return MSG_HANDLED_STOP;
}

DWORD CGUIComboBox::OnEnable(DWORD msgSize, void *param)
{
	return MSG_HANDLED_STOP;
}

DWORD CGUIComboBox::OnDisable(DWORD msgSize, void *param)
{
	return MSG_HANDLED_STOP;
}

void CGUIComboBox::CheckSize(float *width, float *height)
{
	if (*height > COMBOSIZE)
	{
		*height = COMBOSIZE;
	}
}

DWORD CGUIComboBox::CheckMouseState(float mouseX, float mouseY, MOUSESTATE mouseState, float x, 
								   float y, float width, float height, DWORD dwLastState)
{
	DWORD retState = dwLastState;	
	if (mouseState == MOUSE_OPEN)
	{
		retState = m_ComboBox.MouseOver(mouseX, mouseY, false, false, x, y, width, height, dwLastState);
	}
	else if (mouseState == MOUSE_LEFTPRESSED)
	{
		retState = m_ComboBox.MouseOver(mouseX, mouseY, true, false, x, y, width, height, dwLastState);
	}
	else if (mouseState == MOUSE_LEFTRELEASED)
	{
		retState = m_ComboBox.MouseOver(mouseX, mouseY, false, true, x, y, width, height, dwLastState);
	}
	return retState;	
}

void CGUIComboBox::GetFontParams(vector<GUIFONTINFO> &fontData, float x, float y, float width, 
								 float height, DWORD state, float angle, DWORD color)
{
	m_ComboBox.UpdateFontExt(fontData, x, y, width, height, state);
}

void CGUIComboBox::GetRenderParams(vector<GUIVERTEXINFO> &vertexData, float x, float y, float width, 
								   float height, DWORD state, float angle, DWORD color)
{
	m_ComboBox.LoadVertexBuffer(vertexData, x, y, width, height, state, angle, color);
}

DWORD CGUIComboBox::ToggleUp(DWORD dwLastState)
{
	return m_ComboBox.ToggleUp(dwLastState);
}

DWORD CGUIComboBox::ToggleDown(DWORD dwLastState)
{
	return m_ComboBox.ToggleDown(dwLastState);
}
	
DWORD CGUIComboBox::ToggleSelect(DWORD dwLastState)
{
	return m_ComboBox.ToggleSelect(dwLastState);
}
	
DWORD CGUIComboBox::ToggleBack(DWORD dwLastState)
{
	return m_ComboBox.ToggleBack(dwLastState);
}

DWORD CGUIComboBox::GetHashText(DWORD inState)
{
	return m_ComboBox.GetHashText(inState);
}

DWORD CGUIComboBox::SetHashText(DWORD inState, DWORD hashText)
{
	return m_ComboBox.SetHashText(inState, hashText);
}