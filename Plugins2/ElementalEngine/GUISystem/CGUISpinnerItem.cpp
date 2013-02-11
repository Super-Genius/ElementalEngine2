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
#include "CGUISpinnerItem.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	SPINNERSIZE 32

REGISTER_COMPONENT(CGUISpinnerItem);

//REGISTER_MESSAGE_HANDLER(GetText, OnGetText, CGUISpinnerItem);
REGISTER_MESSAGE_HANDLER(SetSpinnerMaxValue, OnSetSpinnerMaxValue, CGUISpinnerItem);
REGISTER_MESSAGE_HANDLER(SetSpinnerMinValue, OnSetSpinnerMinValue, CGUISpinnerItem);

CGUISpinnerItem::CGUISpinnerItem(IHashString *parentName, IHashString *name) :
	OBJECTTEMPLATE(CGUIManager, CGUISpinnerItem, IGUIReference, parentName, name)	
{
	// set all members to default vals
	m_szEditBoxNormTex = _T("");
	m_szEditBoxGrayTex = _T("");

	m_szArrowUpNormTex = _T("");
	m_szArrowUpSelTex = _T("");
	m_szArrowUpGrayTex = _T("");

	m_szArrowDownNormTex = _T("");
	m_szArrowDownSelTex = _T("");
	m_szArrowDownGrayTex = _T("");

	m_szFont = _T("");

	m_szNumType	= _T("");

	m_iNormFontColor = m_iGrayFontColor = -1;
	m_iCursorColor = -1;

	m_fCursorPosX = m_fCursorPosY = -1;

	m_szDefaultText = _T("");

	m_iTextOffSetX = 8;
	m_iTextOffSetY = 5;

	m_fMinRange = 0.f;
	m_fMaxRange = 0.f;
	m_fIncrement = 1.f;

	m_iFontsize = 20; // default to 20 pt font

	m_bButtonsOnRight = false;
}

CGUISpinnerItem::~CGUISpinnerItem()
{}

IComponent *CGUISpinnerItem::Create(int nArgs, va_list argptr)
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
		self = new CGUISpinnerItem(parentName, name);
	}
	catch(...)
	{
		return NULL;
	}
	return self;
}

bool CGUISpinnerItem::Update()
{
	return true;
}

IHashString *CGUISpinnerItem::GetComponentType()
{
	static CHashString CGUISpinnerItemCT(_T("CGUISpinnerItem"));
	return &CGUISpinnerItemCT;
}

bool CGUISpinnerItem::IsKindOf(IHashString *compType)
{
	return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
}

void CGUISpinnerItem::Serialize( IArchive &ar )
{
	static float saveVersion = 1.0f;
	if(ar.IsReading())
	{
		CHashString versionName;
		static DWORD msgHash_GetFileVersion = CHashString(_T("GetFileVersion")).GetUniqueID();
		DWORD retval = EngineGetToolBox()->SendMessage(msgHash_GetFileVersion, sizeof(IHashString), &versionName);

		static DWORD Vers2_0b = CHashString(_T("2.0b")).GetUniqueID();
		static DWORD Vers2_0c = CHashString(_T("2.0c")).GetUniqueID();

		if ((versionName.GetUniqueID() != Vers2_0b) && (versionName.GetUniqueID() != Vers2_0c))
		{
			m_ToolBox->Log(LOGERROR, _T("Bad GUI file version!"));
			assert(0);
			return;
		}
		
		float readVersion = 0.0f;
		ar.Read(readVersion);
		if (readVersion == 0.0f)
		{
			// read from initial serial from editor; schema must be up to date
			readVersion = saveVersion;
		}

		ar.Read(m_szEditBoxNormTex, _T("EditBoxNormTex"));
		ar.Read(m_szEditBoxGrayTex, _T("EditBoxGrayTex"));
		ar.Read(m_szArrowUpNormTex, _T("ArrowUpNormTex"));
		ar.Read(m_szArrowUpSelTex, _T("ArrowUpSelTex"));
		ar.Read(m_szArrowUpGrayTex, _T("ArrowUpGrayTex"));
		ar.Read(m_szArrowDownNormTex, _T("ArrowDownNormTex"));
		ar.Read(m_szArrowDownSelTex, _T("ArrowDownSelTex"));
		ar.Read(m_szArrowDownGrayTex, _T("ArrowDownGrayTex"));
		ar.Read(m_szNumType, _T("NumType"));
		ar.Read(m_szDefaultText, _T("DefaultText"));
		ar.Read(m_szFont, _T("Font"));
		ar.Read(m_iFontsize, _T("Fontsize"));		
		ar.Read(m_fIncrement, _T("Increment"));
		ar.Read(m_fMaxRange, _T("MaxRange"));
		ar.Read(m_fMinRange, _T("MinRange"));
		ar.Read(m_iCursorColor, _T("CursorColor"));
		ar.Read(m_iNormFontColor, _T("NormFontColor"));
		ar.Read(m_iGrayFontColor, _T("GrayFontColor"));
		ar.Read(m_iTextOffSetX, _T("TextOffSetX"));
		ar.Read(m_iTextOffSetY, _T("TextOffSetY"));
		ar.Read(m_bButtonsOnRight, _T("ButtonsOnRight"));

		CreateItem();
	}
	else
	{
		ar.Write(saveVersion, _T("Version"));
		ar.Write(m_szEditBoxNormTex, _T("EditBoxNormTex"));
		ar.Write(m_szEditBoxGrayTex, _T("EditBoxGrayTex"));
		ar.Write(m_szArrowUpNormTex, _T("ArrowUpNormTex"));
		ar.Write(m_szArrowUpSelTex, _T("ArrowUpSelTex"));
		ar.Write(m_szArrowUpGrayTex, _T("ArrowUpGrayTex"));
		ar.Write(m_szArrowDownNormTex, _T("ArrowDownNormTex"));
		ar.Write(m_szArrowDownSelTex, _T("ArrowDownSelTex"));
		ar.Write(m_szArrowDownGrayTex, _T("ArrowDownGrayTex"));
		ar.Write(m_szNumType, _T("NumType"));
		ar.Write(m_szDefaultText, _T("DefaultText"));
		ar.Write(m_szFont, _T("Font"));
		ar.Write(m_iFontsize, _T("Fontsize"));
		ar.Write(m_fIncrement, _T("Increment"));
		ar.Write(m_fMaxRange, _T("MaxRange"));
		ar.Write(m_fMinRange, _T("MinRange"));
		ar.Write(m_iCursorColor, _T("CursorColor"));
		ar.Write(m_iNormFontColor, _T("NormFontColor"));
		ar.Write(m_iGrayFontColor, _T("GrayFontColor"));
		ar.Write(m_iTextOffSetX, _T("TextOffSetX"));
		ar.Write(m_iTextOffSetY, _T("TextOffSetY"));
		ar.Write(m_bButtonsOnRight, _T("ButtonsOnRight"));
	}
}

void CGUISpinnerItem::CreateItem()
{
	float fVal = (float)_tstof(m_szDefaultText);
	m_Spinner.SetTextPos(m_iTextOffSetX, m_iTextOffSetY);
	m_Spinner.SetEditBox(m_szEditBoxNormTex, m_szEditBoxGrayTex, m_iCursorColor);
	m_Spinner.SetUpArrowBox(m_szArrowUpNormTex, m_szArrowUpSelTex, m_szArrowUpGrayTex);
	m_Spinner.SetDownArrowBox(m_szArrowDownNormTex, m_szArrowDownSelTex, m_szArrowDownGrayTex);
	m_Spinner.SetFont(m_szFont, m_iFontsize);
	m_Spinner.SetTextColors(m_iNormFontColor, m_iGrayFontColor);
	m_Spinner.SetIncrementValue(m_fIncrement);
	m_Spinner.SetRange(m_fMinRange, m_fMaxRange);
	m_Spinner.SetButtonsOnRight(m_bButtonsOnRight);

	if ( _tcscmp(m_szNumType, _T("FLOAT")) == 0 )
	{
		m_Spinner.SetDefaultValue(fVal);
	}
	else
	{
		MathUtil math;
		int iVal = (int)(math.Floor(fVal));
		m_Spinner.SetDefaultValue(iVal);		
	}
}

void CGUISpinnerItem::GetRenderParams(vector<GUIVERTEXINFO> &vertexData, float x, float y, 
									  float width, float height, DWORD state, float angle, DWORD color)
{
	m_Spinner.LoadVertexBuffer(vertexData, x, y, width, height, state, angle, color);
}

void CGUISpinnerItem::GetFontParams(vector<GUIFONTINFO> &fontData, float x, float y, float width, 
									float height, DWORD state, float angle, DWORD color)
{
	m_Spinner.UpdateFontExt(fontData, x, y, width, height, state);
}

DWORD CGUISpinnerItem::CheckMouseState(float mouseX, float mouseY, MOUSESTATE mouseState, float x, 
									   float y, float width, float height, DWORD dwLastState)
{
	if (mouseState == MOUSE_OPEN)
	{
		m_Spinner.MouseOver(mouseX, mouseY, false, false, x, y, width, height, &dwLastState);
	}
	else if (mouseState == MOUSE_LEFTPRESSED)
	{
		m_Spinner.MouseOver(mouseX, mouseY, true, false, x, y, width, height, &dwLastState);
	}
	else if (mouseState == MOUSE_LEFTRELEASED)
	{
		m_Spinner.MouseOver(mouseX, mouseY, false, true, x, y, width, height, &dwLastState);
	}

	return dwLastState;
}

DWORD CGUISpinnerItem::ToggleUp(DWORD dwLastState)
{
	return m_Spinner.ToggleUp(dwLastState);
}

DWORD CGUISpinnerItem::ToggleDown(DWORD dwLastState)
{
	return m_Spinner.ToggleDown(dwLastState);
}

DWORD CGUISpinnerItem::ToggleSelect(DWORD dwLastState)
{
	return dwLastState;
}

DWORD CGUISpinnerItem::ToggleBack(DWORD dwLastState)
{
	return dwLastState;
}

DWORD CGUISpinnerItem::GetHashText(DWORD inState)
{
	return m_Spinner.GetHashText(inState);
}

DWORD CGUISpinnerItem::SetHashText(DWORD inState, DWORD hashText)
{
	return m_Spinner.SetHashText(inState, hashText);
}

DWORD CGUISpinnerItem::OnSetSpinnerMaxValue(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(float));
	float val = *(float*)param;

	m_fMaxRange = val;
	m_Spinner.SetRange(m_fMinRange, m_fMaxRange);

	return MSG_HANDLED_STOP;
}

DWORD CGUISpinnerItem::OnSetSpinnerMinValue(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(float));
	float val = *(float*)param;

	m_fMinRange = val;
	m_Spinner.SetRange(m_fMinRange, m_fMaxRange);

	return MSG_HANDLED_STOP;
}