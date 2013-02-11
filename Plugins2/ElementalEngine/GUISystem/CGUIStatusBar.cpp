///============================================================================
/// \file		CGUIStatusBar
/// \brief		Implmentation of CGUIStatusBar
/// \date		07-29-05
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
#include "CGUIStatusBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT(CGUIStatusBar);

REGISTER_MESSAGE_HANDLER(UpdateStatusBarPos, OnUpdateStatusBarPos, CGUIStatusBar);
REGISTER_MESSAGE_HANDLER(SetStatusBarAnimated, OnSetStatusBarAnimated, CGUIStatusBar);
REGISTER_MESSAGE_HANDLER(SetStatusBarFrozen, OnSetStatusBarFrozen, CGUIStatusBar);
REGISTER_MESSAGE_HANDLER(SetStatusBarLinger, OnSetStatusBarLinger, CGUIStatusBar);
REGISTER_MESSAGE_HANDLER(LoadingUpdate, OnLoadingUpdate, CGUIStatusBar);

CGUIStatusBar::CGUIStatusBar(IHashString *parentName, IHashString *name) :
 	OBJECTTEMPLATE(CGUIManager, CGUIStatusBar, IGUIReference, parentName, name)
{
	m_szBackgroundTex = _T("");
	m_szFullTex = _T("");
	m_szThreeQuarterTex = _T("");
	m_szHalfTex = _T("");
	m_szQuarterTex = _T("");

	m_bStatusSmartScale = false;
	m_bBackgroundSmartScale = false;

	m_fPercentComplete = 0.f;

	m_szTwoHundredTex = _T("");
	m_szThreeHundredTex = _T("");
	m_szFourHundredTex = _T("");
	m_szAnimGhostBarTex = _T("");

	m_bAnimateBarShift = false;
	m_fBarAnimationTime = 0.0f;
	m_fBarAnimationET = 0.0f;

	m_fLastPercentage = 0.0f;

	m_fSaveVersion = 1.3f;
	m_bFrozenTimer = false;
	m_bLastPercentLinger = false;
	m_fLingerTime = 1.0f;

	m_bIsLoadingBar = false;
}

CGUIStatusBar::~CGUIStatusBar()
{
}

// create function for this world loader
IComponent *CGUIStatusBar::Create(int nArgs, va_list argptr)
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
 		self = new CGUIStatusBar(parentName, name);
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

bool CGUIStatusBar::Update()
{
	if (m_bAnimateBarShift)
	{
		float lingerFactor = 0.0f;
		if (m_bLastPercentLinger)
		{
			lingerFactor = m_fLingerTime;
		}
		if ((m_fBarAnimationET < m_fBarAnimationTime+lingerFactor) && (!m_bFrozenTimer))
		{
			static CHashString hsGameTime(GAMEDELTATIMER);
			float deltaT = m_Timer->GetTimeDelta( &hsGameTime );
			
			m_fBarAnimationET += deltaT;
			if (m_fBarAnimationET > m_fBarAnimationTime+lingerFactor)
			{
				m_fBarAnimationET = m_fBarAnimationTime+lingerFactor;
			}
		}
	}
	return true;
}

IHashString *CGUIStatusBar::GetComponentType()
{
	static CHashString CGUIItemCT(_T("CGUIStatusBar"));
	return &CGUIItemCT;
}

bool CGUIStatusBar::IsKindOf(IHashString *compType)
{
	return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
}

void CGUIStatusBar::Serialize(IArchive &ar)
{
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

		float unitVersion = 0.0f;
		
		if (verNum >= 4)
		{
			ar.Read(unitVersion);
			if (unitVersion == 0.0f)
			{
				// initial serialize from editor; set to save version (up to date schema)
				unitVersion = m_fSaveVersion;
			}
		}
		ar.Read(m_szFullTex);
		ar.Read(m_szBackgroundTex);		
		ar.Read(m_szTwoHundredTex);
		ar.Read(m_szThreeHundredTex);
		ar.Read(m_szFourHundredTex);
		if (unitVersion >= 1.1f)
		{
			ar.Read(m_szAnimGhostBarTex, _T("AnimGhostBarTex"));
		}
		ar.Read(m_bForwardRender);
		ar.Read(m_iLeftOffset);
		ar.Read(m_iTopOffset);
		ar.Read(m_iRightOffset);
		ar.Read(m_iBottomOffset);
		int percentage;
		ar.Read(percentage);
		m_fLastPercentage = m_fPercentComplete;
		m_fPercentComplete = (float)percentage / 100.f;		

		if (unitVersion >= 1.1f)
		{
			ar.Read(m_bAnimateBarShift, _T("AnimateBarShift"));
			ar.Read(m_fBarAnimationTime, _T("BarAnimationTime"));	
		}

		if (unitVersion >= 1.2f)
		{
			ar.Read(m_bLastPercentLinger, _T("LingerGhostAnim"));
			ar.Read(m_fLingerTime, _T("LingerTime"));
		}

		if (unitVersion >= 1.3f)
		{
			ar.Read(m_bIsLoadingBar, _T("IsLoadingBar"));
		}

		m_fBarAnimationET = 0.0f;
		
		CreateStatusBar();
	}
	else
	{
		ar.Write(m_fSaveVersion, _T("Version"));
		ar.Write(m_szFullTex, _T("FullTex"));
		ar.Write(m_szBackgroundTex, _T("BGTex"));
		ar.Write(m_szTwoHundredTex, _T("TwoHndrPcntTex"));
		ar.Write(m_szThreeHundredTex, _T("ThreeHndrPcntTex"));
		ar.Write(m_szFourHundredTex, _T("FourHndrPcntTex"));
		ar.Write(m_szAnimGhostBarTex, _T("AnimGhostBarTex"));
		ar.Write(m_bForwardRender, _T("FowardRender"));
		ar.Write(m_iLeftOffset, _T("LeftOffset"));
		ar.Write(m_iTopOffset, _T("TopOffset"));
		ar.Write(m_iRightOffset, _T("RightOffset"));
		ar.Write(m_iBottomOffset, _T("BottomOffset"));
		ar.Write(0, _T("EditorRenderPercent"));		
		ar.Write(m_bAnimateBarShift, _T("AnimateBarShift"));
		ar.Write(m_fBarAnimationTime, _T("BarAnimationTime"));
		ar.Write(m_bLastPercentLinger, _T("LingerGhostAnim"));
		ar.Write(m_fLingerTime, _T("LingerTime"));
		ar.Write(m_bIsLoadingBar, _T("IsLoadingBar"));
	}
}

void CGUIStatusBar::CreateStatusBar()
{
	m_StatusBar.SetTextures(m_szBackgroundTex, m_szFullTex, m_szTwoHundredTex, m_szThreeHundredTex, m_szFourHundredTex);
	m_StatusBar.SetGhostTexture(m_szAnimGhostBarTex);
}

DWORD CGUIStatusBar::OnUpdateStatusBarPos(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(LOADINGUPDATEPARAMS));
	LOADINGUPDATEPARAMS pbp = *(LOADINGUPDATEPARAMS*) param;	
	
	float timeVal = 1.0f;

	if (!m_bLastPercentLinger)
	{
		m_fLastPercentage = m_fPercentComplete;
	}		
	else
	{
		if (m_fBarAnimationET >= m_fLingerTime)
		{
			timeVal = (m_fBarAnimationET - m_fLingerTime) / m_fBarAnimationTime;
		}
		else
		{
			timeVal = 0.0f;
		}
	}

	m_fBarAnimationET = 0.0f;

	if (pbp.totalSize > 0)
	{
		m_fPercentComplete = (float)pbp.currentPosition / (float)pbp.totalSize;
	}
	else
	{
		m_fPercentComplete = 0.0f;
	}

	if (m_bLastPercentLinger)
	{
		float last = m_StatusBar.CalcSquaredPercentage(m_fLastPercentage, m_fPercentComplete, timeVal);
		m_fLastPercentage = last;
	}
	
	return MSG_HANDLED_STOP;
}

void CGUIStatusBar::GetRenderParams(vector<GUIVERTEXINFO> &vertexData, float x, float y, float width, float height, 
									DWORD state, float angle, DWORD color)
{
	float timeVal = 1.0f;
	if (m_fBarAnimationTime != 0.0f)
	{
		if (!m_bLastPercentLinger)
		{
			timeVal = m_fBarAnimationET / m_fBarAnimationTime;
		}
		else
		{
			if (m_fBarAnimationET >= m_fLingerTime)
			{
				timeVal = (m_fBarAnimationET - m_fLingerTime) / m_fBarAnimationTime;
			}
			else
			{
				timeVal = 0.0f;
			}
		}
	}
	m_StatusBar.LoadVertexBuffer(vertexData, x, y, width, height, m_fPercentComplete, m_iLeftOffset,
		m_iTopOffset, m_iRightOffset, m_iBottomOffset, m_bForwardRender, angle, color,
		m_fLastPercentage, timeVal);
}

DWORD CGUIStatusBar::OnSetStatusBarAnimated(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(bool));
	bool val = *(bool*)param;

	m_bAnimateBarShift = val;

	return MSG_HANDLED_STOP;
}

DWORD CGUIStatusBar::OnSetStatusBarFrozen(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(bool));
	bool val = *(bool*)param;

	m_bFrozenTimer = val;

	return MSG_HANDLED_STOP;
}

DWORD CGUIStatusBar::OnSetStatusBarLinger(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(bool));
	bool val = *(bool*)param;

	m_bLastPercentLinger = val;

	return MSG_HANDLED_STOP;
}

DWORD CGUIStatusBar::OnLoadingUpdate(DWORD size, void *param)
{
	if (m_bIsLoadingBar)
	{
		OnUpdateStatusBarPos(size, param);
	}

	return MSG_HANDLED_PROCEED;
}