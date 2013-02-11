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

#include "stdafx.h"
#include ".\resourceoptions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern UINT AddOptionPageMessage;
/////////////////////////////////////////////////////////////////////////////
// CResourceOptions dialog


CResourceOptions::CResourceOptions()
	: CPropertyPage(CResourceOptions::IDD)
{
	//{{AFX_DATA_INIT(CResourceOptions)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	// we can do this because we are part of gds.  Other
	// options pages should use BroadCastMessages
	AfxGetMainWnd()->SendMessage(AddOptionPageMessage, 0, (LPARAM)this);
	
	m_iCullDistance = 999999;
	float tmpCullDistance = (float)m_iCullDistance;
	static DWORD msgHash_GetObjectCullDistance = CHashString(_T("GetObjectCullDistance")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_GetObjectCullDistance, sizeof(tmpCullDistance), &tmpCullDistance );
}


void CResourceOptions::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text( pDX, IDC_CULLOBJECTSDISTANCE, m_iCullDistance );	
	DDX_Control(pDX, IDC_CULLOBJECTSDISTANCE, m_ctrlCullObjectDistance);
	DDX_Control(pDX, IDC_CULLOBJECTSFLAG, m_ctrlCullObjectFlag);
}


BEGIN_MESSAGE_MAP(CResourceOptions, CPropertyPage)
	//{{AFX_MSG_MAP(CResourceOptions)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CULLOBJECTSFLAG, OnBnClickedCullobjectsflag)
	ON_EN_KILLFOCUS(IDC_CULLOBJECTSDISTANCE, OnEnKillfocusCullobjectsdistance)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResourceOptions message handlers

void CResourceOptions::OnOK()
{
	//TODO: Save the data
}

void CResourceOptions::OnCancel()
{
	//TODO: Cancel data here
}

BOOL CResourceOptions::OnApply()
{
	// do validation here.
	//return CPropertyPage::OnApply();
	OnOK();

	return FALSE;
}

void CResourceOptions::OnBnClickedCullobjectsflag()
{
	// TODO: Add your control notification handler code here
	m_ctrlCullObjectDistance.EnableWindow( m_ctrlCullObjectFlag.GetCheck() );
	if( m_ctrlCullObjectFlag.GetCheck() )
	{
		float tmpCullDistance = (float)m_iCullDistance;
		static DWORD msgHash_SetObjectCullDistance = CHashString(_T("SetObjectCullDistance")).GetUniqueID();
		EngineGetToolBox()->SendMessage(msgHash_SetObjectCullDistance, sizeof(tmpCullDistance), &tmpCullDistance );
	}
	else
	{
		float tmpCullDistance = 999999.0f;
		static DWORD msgHash_SetObjectCullDistance = CHashString(_T("SetObjectCullDistance")).GetUniqueID();
		EngineGetToolBox()->SendMessage(msgHash_SetObjectCullDistance, sizeof(tmpCullDistance), &tmpCullDistance );
	}
}

void CResourceOptions::OnEnKillfocusCullobjectsdistance()
{
	TCHAR szBuffer[256];
	szBuffer[0] = '\0';
	m_ctrlCullObjectDistance.GetWindowText( szBuffer, 256 );
	m_iCullDistance = (int)atoi( StdString(szBuffer) );

	// TODO: Add your control notification handler code here
	if( m_iCullDistance < 0 )
	{
		m_iCullDistance = 0;
		m_ctrlCullObjectDistance.SetWindowText( _T("0") );
	}
	if( m_iCullDistance > 999999 )
	{
		m_iCullDistance = 999999;
		m_ctrlCullObjectDistance.SetWindowText( _T("999999") );
	}

	float tmpCullDistance = (float)m_iCullDistance;
	static DWORD msgHash_SetObjectCullDistance = CHashString(_T("SetObjectCullDistance")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_SetObjectCullDistance, sizeof(tmpCullDistance), &tmpCullDistance );
}
