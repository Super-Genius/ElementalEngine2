///============================================================================
/// \file		ProfilerManager.cpp
/// \brief		Manager for external profile window
/// \date		06/11/2007
/// \author		Nick Tourte
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
//#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT_SINGLETON(CProfilerManager);

REGISTER_MESSAGE_HANDLER(SetProfilerStrobeTime, OnSetProfilerStrobeTime, CProfilerManager);
REGISTER_MESSAGE_HANDLER(ShowHideProfilerWindow, OnShowHideProfilerWindow, CProfilerManager);

extern AFX_EXTENSION_MODULE ProfileEditorDLL;

CProfilerManager::CProfilerManager() : 
	CManager(_T("CProfilerManager"), PROFILERPRIORITY)
{
	m_pWindow = NULL;

	m_fUpdateTime = 0.1f;
	m_fLastTime = 0.0f;

	m_bShowWindow = false;

	CreateProfilerWindow();	

	Init();
}

CProfilerManager::~CProfilerManager()
{	
	DeInit();
}

IComponent *CProfilerManager::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE(CProfilerManager);
}

bool CProfilerManager::Init()
{
	m_fLastTime = m_Timer->GetTime();
	return true;
}

bool CProfilerManager::DeInit()
{
	if (m_pWindow)
	{
		m_pWindow->DestroyWindow();
		delete m_pWindow;
		m_pWindow = NULL;
	}
	CManager::DeInit();
	return true;
}

void CProfilerManager::Update(DWORD tickCount)
{
	if (m_pWindow && m_bShowWindow == true)
	{
		float currentTime = m_Timer->GetTime();
		if (currentTime - m_fLastTime > m_fUpdateTime)
		{
			m_fLastTime = currentTime;

			GETPERFORMANCEPROFILERS gpp;			
			PERFORMANCEPROFILERTYPEMAP *perfMap = NULL;
			static DWORD msgGetPerformanceProfilers = CHashString(_T("GetPerformanceProfilers")).GetUniqueID();
			if (m_ToolBox->SendMessage(msgGetPerformanceProfilers, sizeof(GETPERFORMANCEPROFILERS), &gpp) == MSG_HANDLED)			
			{
				perfMap = gpp.pPerformanceProfilerMap;
				if (perfMap)
				{
					m_pWindow->UpdateProfiles(*perfMap);					
				}
			}
			
			static DWORD msgGetCurrentFPS = CHashString(_T("GetCurrentFPS")).GetUniqueID();
			float fps;
			if (m_ToolBox->SendMessage(msgGetCurrentFPS, sizeof(float), &fps) == MSG_HANDLED)
			{
				m_pWindow->UpdateFPS(fps);
			}			

			PROFILERMESSAGECOUNTMAP* messageProfilerCountMap = m_ToolBox->GetMessageProfilerCountMap();

			PROFILERMESSAGECOUNTMAP::iterator iter = messageProfilerCountMap->begin();
			for( iter = messageProfilerCountMap->begin(); iter != messageProfilerCountMap->end(); iter++ )
			{
				DWORD message = (DWORD)( iter->first );
				int count = (int)( iter->second );
				CHashString hszName( m_ToolBox->GetHashString( message ) );

				StdString szName = GetComponentType()->GetString();
				CHashString hszType(_T("Message Counts"));
				PERFORMANCEPROFILERUPDATE msg;
				msg.name = &hszName;
				msg.time = (float)count;
				msg.displayType = NAME_TIME;
				msg.type = &hszType;
				static DWORD msgHash_UpdatePerformanceProfiler = CHashString(_T("UpdatePerformanceProfiler")).GetUniqueID();
				EngineGetToolBox()->SendMessage(msgHash_UpdatePerformanceProfiler, sizeof(msg), &msg );
			}

			messageProfilerCountMap->clear();				
		}		
	}	
}

void CProfilerManager::CreateProfilerWindow()
{
	if (m_pWindow == NULL)
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());

		m_pWindow = new CProfilerWindow();

		if (m_pWindow)
		{
			CBrush bkgBrush;
			bkgBrush.CreateSolidBrush(RGB(255,255,255));

			CString className = AfxRegisterWndClass(CS_VREDRAW | CS_HREDRAW | CS_NOCLOSE, NULL, (HBRUSH)bkgBrush);
			m_pWindow->CreateEx(0, className, _T("Profile Window"), WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU, 
				0, 0, 800, 600, NULL, NULL);		
		}
	}
}

DWORD CProfilerManager::OnSetProfilerStrobeTime(DWORD size, void* param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(float));
	float newStrobe = *(float*)param;

	m_fUpdateTime = newStrobe;
	return MSG_HANDLED_STOP;
}

DWORD CProfilerManager::OnShowHideProfilerWindow(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(bool));
	bool show = *(bool*)param;

	m_bShowWindow = show;

	if (show == true)
	{
		if (m_pWindow)
		{
			m_pWindow->ShowWindow(SW_SHOW);
		}
	}
	else
	{
		if (m_pWindow)
		{
			m_pWindow->ShowWindow(SW_HIDE);
		}
	}

	return MSG_HANDLED_STOP;
}