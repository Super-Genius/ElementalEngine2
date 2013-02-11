///============================================================================
/// \file		TimeOfDayEditor.cpp
/// \brief		Implementation of TimeOfDay Editor
/// \date		08-12-2005
/// \author		D. Patrick Ghiocel
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
#include <afxdllx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifdef _DEBUG
// major version in upper 16 bits
// low word =  minor version, low bit debug =0, release=1
#define INTERFACEDLLVERSION 0x00006100
#else
#define INTERFACEDLLVERSION 0x00006101
#endif

// we don't need loading before anybody, so set priority rather high
#define TIMEOFDAYEDITOR_PRIORITY 18000
AFX_EXTENSION_MODULE TimeOfDayEditorDLL = {false, NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("TimeOfDayEditor.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(TimeOfDayEditorDLL, hInstance))
			return 0;

		// Insert this DLL into the resource chain
		// NOTE: If this Extension DLL is being implicitly linked to by
		//  an MFC Regular DLL (such as an ActiveX Control)
		//  instead of an MFC application, then you will want to
		//  remove this line from DllMain and put it in a separate
		//  function exported from this Extension DLL.  The Regular DLL
		//  that uses this Extension DLL should then explicitly call that
		//  function to initialize this Extension DLL.  Otherwise,
		//  the CDynLinkLibrary object will not be attached to the
		//  Regular DLL's resource chain, and serious problems will
		//  result.

		new CDynLinkLibrary(TimeOfDayEditorDLL);

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("TimeOfDayEditor.DLL Terminating!\n");

		// Terminate the library before destructors are called
		AfxTermExtensionModule(TimeOfDayEditorDLL);
	}
	return 1;   // ok
}

extern "C" DWORD GetDLLVersion()
{
	return INTERFACEDLLVERSION;
}

// Exported DLL initialization is run in context of running application
extern "C" IDLL *GetDLLInterface()
{
	// this has to be done after module is intialized.
	return SINGLETONINSTANCE(CTimeOfDayEditor);
}

//---------------------------------------------------------------------
// Function:	GetPriority
// Description:	DLL's priority
// Parameters:	.
// Returns:		DWORD = priority of DLL loaded
//---------------------------------------------------------------------
CTimeOfDayEditor::CTimeOfDayEditor( )
{
	SINGLETONINSTANCE(CTimeOfDayComponent)->SetParent(this);
	m_ToolBox = EngineGetToolBox();
	m_bWeatherDlg = false;

}

CTimeOfDayEditor::~CTimeOfDayEditor( )
{

}

BOOL CTimeOfDayEditor::Initialize( )
{
	return TRUE;
}

void CTimeOfDayEditor::OnCreate(CMDIFrameWnd *mainWnd)
{
	return; // TODO: resurrect the time of day editor

	if (!m_TimeOfDayToolPal.Create(mainWnd, CTimeOfDayToolPal::IDD,
      CBRS_TOP|CBRS_TOOLTIPS|CBRS_FLYBY, CTimeOfDayToolPal::IDD))
	{
		TRACE0("Failed to create dockbar\n");
		return;      // fail to create
	}

	TOOLBARSTRUCT atbs;

	atbs.hIcon = (HICON)LoadImage(TimeOfDayEditorDLL.hModule, 
		MAKEINTRESOURCE(IDR_TIMEOFDAYTYPE), IMAGE_ICON, 0, 0, 0);
	atbs.Text = _T("Time Of Day");
	atbs.cBar = &m_TimeOfDayToolPal;
	static DWORD msgHash_AddToolBar = CHashString(_T("AddToolBar")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_AddToolBar, sizeof(TOOLBARSTRUCT), &atbs);

	// set VISIBILE in the properties of the dialog, so it won't be shown during creation
	m_TimeOfDayDlg.Create(IDD_TOD_DIALOG, AfxGetApp()->GetMainWnd() );	
	m_bWeatherDlg = m_WeatherEffectsDlg.Create(IDD_WEATHEREFFECTS_DIALOG, AfxGetApp()->GetMainWnd() );	
	//ShowTimeOfDayDialog( false );
}

BOOL CTimeOfDayEditor::OnCmdMsg(UINT nID, int nCode, void* pExtra,
		AFX_CMDHANDLERINFO* pHandlerInfo)
{
	return FALSE;
}

CString* CTimeOfDayEditor::GetName( void )
{
	static CString name;
    LPTSTR tBuff;

    tBuff = name.GetBuffer(_MAX_PATH+1);
	GetModuleFileName(TimeOfDayEditorDLL.hModule, tBuff, _MAX_PATH);
    name.ReleaseBuffer();

    return &name;
}

DWORD CTimeOfDayEditor::GetPriority()
{
	return TIMEOFDAYEDITOR_PRIORITY;
}

void CTimeOfDayEditor::OnGlobalUpdateTick( void )
{
	m_TimeOfDayToolPal.OnGlobalUpdateTick();
}

void CTimeOfDayEditor::ShowTimeOfDayDialog( bool bShow )
{
	if (bShow)
	{
		m_TimeOfDayDlg.ShowWindow( SW_SHOWNORMAL );
		m_TimeOfDayDlg.SetActiveWindow();
		m_TimeOfDayDlg.BringWindowToTop();
		m_TimeOfDayDlg.PopulateFields();
	}
	else
	{
		m_TimeOfDayDlg.ShowWindow( SW_HIDE );
	}
}

void CTimeOfDayEditor::ShowWeatherDialog(bool bShow)
{
	if (m_bWeatherDlg)
	{
		if (bShow)
		{
			m_WeatherEffectsDlg.ShowWindow(SW_SHOWNORMAL);
			m_WeatherEffectsDlg.SetActiveWindow();
			m_WeatherEffectsDlg.BringWindowToTop();
			m_WeatherEffectsDlg.PopulateFields();
		}
		else
		{
			m_WeatherEffectsDlg.ShowWindow( SW_HIDE );
		}
	}
}
