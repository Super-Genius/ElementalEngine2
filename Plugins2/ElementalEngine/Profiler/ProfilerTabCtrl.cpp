///============================================================================
/// \file		ProfilerTabCtrl.cpp
/// \brief		Dialog box with tabs 
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
#include ".\profilertabctrl.h"

// CProfilerTabCtrl dialog

IMPLEMENT_DYNAMIC(CProfilerTabCtrl, CDialog)
CProfilerTabCtrl::CProfilerTabCtrl(CWnd* pParent /*=NULL*/)
	: CDialog(CProfilerTabCtrl::IDD, pParent)
{
	m_fUpdateTime = 0.1f;

	static DWORD msgSetProfilerStrobeTime = CHashString(_T("SetProfilerStrobeTime")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgSetProfilerStrobeTime, sizeof(float), &m_fUpdateTime);
}

CProfilerTabCtrl::~CProfilerTabCtrl()
{
	PROFILEBOXVEC::iterator listIter = m_InternalBoxes.begin();
	for (; listIter!=m_InternalBoxes.end(); listIter++)
	{
		CWnd *current = (*listIter);
		current->DestroyWindow();
		delete current;
	}
}

void CProfilerTabCtrl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TABCTRL, m_TabControl);
	DDX_Control(pDX, IDC_STROBEEDIT, m_StrobeEdit);
	DDX_Control(pDX, IDC_STROBESPIN, m_StrobeSpinner);
	DDX_Control(pDX, IDC_STATICFPS, m_FPSCtrl);
}

void CProfilerTabCtrl::AddTab(const TCHAR *tabName, PROFILERDISPLAYTYPE type)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	TCHAR tempName[256];
	_tcscpy(tempName, tabName);

	TCITEM tcItem;
	tcItem.mask = TCIF_TEXT;
	tcItem.pszText = tempName;

    int itemSpot = m_TabControl.GetItemCount();
	m_TabControl.InsertItem(itemSpot, &tcItem);

	CProfileBox *newBox = new CProfileBox(type, tabName);
	newBox->Create(IDD_PROFILEBOX, this);

	ResizeInternalBox(newBox);

	newBox->Init();

	if (m_TabControl.GetCurSel() == itemSpot)
	{
		newBox->ShowWindow(SW_SHOW);
	}
	else
	{
		newBox->ShowWindow(SW_HIDE);
	}

	m_InternalBoxes.push_back(newBox);

	DWORD dwName = CHashString(tabName).GetUniqueID();

	m_TabNames.insert(pair<DWORD, int>(dwName, itemSpot));
}

void CProfilerTabCtrl::ResizeInternalBox(CProfileBox *box)
{
	if (box)
	{
		CRect tabRect, parentRect;
		int nX, nY, nXc, nYc;

		GetWindowRect(&parentRect);
		m_TabControl.GetWindowRect(&tabRect);
		m_TabControl.AdjustRect(FALSE, &tabRect);
		
		nX = tabRect.left - parentRect.left;
		nY = tabRect.top - parentRect.top;
		nXc = tabRect.right - tabRect.left;
		nYc = tabRect.bottom - tabRect.top;

		box->SetWindowSize(nX, nY, nXc, nYc);
	}
}

void CProfilerTabCtrl::ResizeWindow(int x, int y, int width, int height)
{
	RECT parentRect;
	GetWindowRect(&parentRect);

	SetWindowPos(m_pParentWnd, x, y, width, height, 0);	

	RECT rect;
	m_TabControl.GetWindowRect(&rect);

	m_TabControl.AdjustRect(TRUE, &rect);
	
	rect.left = rect.left - parentRect.left;
	rect.top = rect.top - parentRect.top;	
	rect.right = width;
	rect.bottom = height;

	m_TabControl.AdjustRect(FALSE, &rect);	
	m_TabControl.MoveWindow(&rect, TRUE);	

	PROFILEBOXVEC::iterator listIter = m_InternalBoxes.begin();
	for (; listIter != m_InternalBoxes.end(); listIter++)
	{
		ResizeInternalBox((*listIter));
	}

	// update time display
	TCHAR buf[20];
	_stprintf(buf, _T("%.2f sec"), m_fUpdateTime);
	m_StrobeEdit.SetWindowText(buf);
}

BEGIN_MESSAGE_MAP(CProfilerTabCtrl, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TABCTRL, OnTcnSelchangeTabctrl)
	ON_NOTIFY(UDN_DELTAPOS, IDC_STROBESPIN, OnDeltaposStrobespin)
	ON_EN_KILLFOCUS(IDC_STROBEEDIT, OnEnKillfocusStrobeedit)
	ON_BN_CLICKED(IDC_HIDEWINDOW, OnBnClickedHidewindow)
END_MESSAGE_MAP()


// CProfilerTabCtrl message handlers

void CProfilerTabCtrl::OnTcnSelchangeTabctrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;

	int selection = m_TabControl.GetCurSel();

	PROFILEBOXVEC::iterator listIter = m_InternalBoxes.begin();

	int counter = 0;
	for(; listIter!=m_InternalBoxes.end(); listIter++)
	{
		if (counter == selection)
		{
			(*listIter)->ShowWindow(SW_SHOW);
		}
		else
		{
			(*listIter)->ShowWindow(SW_HIDE);
		}
		counter++;
	}
}

void CProfilerTabCtrl::UpdateProfiles(PERFORMANCEPROFILERTYPEMAP &map)
{
	PERFORMANCEPROFILERTYPEMAP::iterator perfmapIter = map.begin();
	for (; perfmapIter != map.end(); perfmapIter++)
	{
		DWORD dwType = perfmapIter->first;
		NAMEINDEXMAP::iterator mapIter = m_TabNames.find(dwType);
		if (mapIter == m_TabNames.end())
		{
			AddTab(EngineGetToolBox()->GetHashString(dwType), (perfmapIter->second).displayType);
		}
		else
		{
			int index = mapIter->second;
			if (m_InternalBoxes.size() > (UINT)index)
			{
				CProfileBox *box = m_InternalBoxes[index];
				if ((box) && (box->IsWindowVisible() == TRUE))
				{
					box->UpdateProfile((perfmapIter->second).map);
				}
			}
		}
	}
}

void CProfilerTabCtrl::OnDeltaposStrobespin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	*pResult = 0;

	m_fUpdateTime -= ((float)pNMUpDown->iDelta * 0.02f);

	if (m_fUpdateTime < 0.0f)
	{
		m_fUpdateTime = 0.0f;
	}

	static DWORD msgSetProfilerStrobeTime = CHashString(_T("SetProfilerStrobeTime")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgSetProfilerStrobeTime, sizeof(float), &m_fUpdateTime);

	TCHAR buf[20];
	_stprintf(buf, _T("%.2f sec"), m_fUpdateTime);
	m_StrobeEdit.SetWindowText(buf);
}

void CProfilerTabCtrl::OnEnKillfocusStrobeedit()
{
	TCHAR buf[100];
	m_StrobeEdit.GetWindowText(buf, 100);	
	float newVal = (float)_tstof(buf);

	m_fUpdateTime = newVal;

	if (m_fUpdateTime < 0.0f)
	{
		m_fUpdateTime = 0.0f;
	}

	static DWORD msgSetProfilerStrobeTime = CHashString(_T("SetProfilerStrobeTime")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgSetProfilerStrobeTime, sizeof(float), &m_fUpdateTime);

	_stprintf(buf, _T("%.2f sec"), m_fUpdateTime);
	m_StrobeEdit.SetWindowText(buf);
}

void CProfilerTabCtrl::OnBnClickedHidewindow()
{
	bool show = false;
	static DWORD msgShowHideProfilerWindow = CHashString(_T("ShowHideProfilerWindow")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgShowHideProfilerWindow, sizeof(bool), &show);
}

void CProfilerTabCtrl::UpdateFPS(float fps)
{
	TCHAR buf[16];
	_stprintf(buf, "%.2f", fps);
	m_FPSCtrl.SetWindowText(buf);
}
