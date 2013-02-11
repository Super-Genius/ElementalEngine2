///============================================================================
/// \file		ProfileBox.h
/// \brief		Dialog box with profile data
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
#include ".\profilebox.h"

// CProfileBox dialog

IMPLEMENT_DYNAMIC(CProfileBox, CDialog)
CProfileBox::CProfileBox(PROFILERDISPLAYTYPE type, const TCHAR *name, CWnd* pParent)
	: CDialog(CProfileBox::IDD, pParent)
{
	m_iCollumnSort = -1;
	m_bAscendSort = false;
	m_DisplayType = NULL;
	m_hsTabName.Init(name);
	m_bClearMapOnTick = false;
	m_szFilterString = _T("");

	m_bExcludeFilter = false;

	if (type == NAME_TIME)
	{
		m_DisplayType = new CNameTimeDisplay();
	}
	else if (type == NAME_NUMBER)
	{
		m_DisplayType = new CNameNumberDisplay();
	}
}

CProfileBox::~CProfileBox()
{
	if (m_DisplayType)
	{
		delete m_DisplayType;
		m_DisplayType = NULL;
	}
}

void CProfileBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROFILELISTCTRL, m_ListCtrl);
	DDX_Control(pDX, IDC_FILTERCOMBO, m_FilterStringCombo);
}


BEGIN_MESSAGE_MAP(CProfileBox, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_WM_CREATE()
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_PROFILELISTCTRL, OnLvnColumnclickProfilelistctrl)
	ON_BN_CLICKED(IDC_CLEARMAP, OnBnClickedClearmap)
	ON_BN_CLICKED(IDC_CLEARTICKCHECK, OnBnClickedCleartickcheck)
	ON_CBN_KILLFOCUS(IDC_FILTERCOMBO, OnCbnKillfocusFiltercombo)	
	ON_CBN_SELCHANGE(IDC_FILTERCOMBO, OnCbnSelchangeFiltercombo)
	ON_BN_CLICKED(IDC_RADIOINCLUSIVE, OnBnClickedRadioinclusive)
	ON_BN_CLICKED(IDC_RADIOEXCLUSIVE, OnBnClickedRadioexclusive)
END_MESSAGE_MAP()


// CProfileBox message handlers

void CProfileBox::SetWindowSize(int x, int y, int width, int height)
{
	SetWindowPos(NULL, x, y, width, height, 0);

	// want to maintain placement so we can have buttons across the top
	CRect boxRect, listRect;
	GetWindowRect(&boxRect);
	m_ListCtrl.GetWindowRect(&listRect);
	
	int diffX = listRect.left - boxRect.left;
	int diffY = listRect.top - boxRect.top;
	int newWidth = width - diffX;
	int newHeight = height - diffY;
	
	m_ListCtrl.SetWindowPos(NULL, 0, 0, newWidth, newHeight, SWP_NOMOVE);		
}

void CProfileBox::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
//	OnOK();		
}

void CProfileBox::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
//	OnCancel();
}

void CProfileBox::UpdateProfile(PERFORMANCEPROFILERMAP &map)
{
	if (m_bClearMapOnTick)
	{
		// clear peaks and avgs too
		m_DisplayType->ClearSubmaps();

		ClearInternalMap();
	}

	UpdateInternalMap(map);

	if (m_DisplayType)
	{
		m_DisplayType->UpdateProfile(m_InternalPerformanceMap, m_ListCtrl);
	}
}

int CProfileBox::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;	
	return 0;
}

void CProfileBox::Init()
{
	if (m_DisplayType)
	{
		m_DisplayType->InsertColumns(m_ListCtrl);
	}	
}

void CProfileBox::OnLvnColumnclickProfilelistctrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	int newColumn = pNMLV->iSubItem;	

	if (m_iCollumnSort != newColumn)
	{
		m_bAscendSort = true;
		m_iCollumnSort = newColumn;
	}
	else
	{
		m_bAscendSort = !m_bAscendSort;
	}

	if (m_DisplayType)
	{
		m_DisplayType->SortColumn(newColumn, m_bAscendSort, m_ListCtrl);
	}	
}

void CProfileBox::FilterMapNames()
{
	if (_tcscmp(m_szFilterString, _T("")) != 0)
	{
		// now remove from internal map
		PERFORMANCEPROFILERMAP::iterator intMapIter = m_InternalPerformanceMap.begin();
		for (; intMapIter != m_InternalPerformanceMap.end(); )
		{
			// erase from internal map and list control
			StdString name = EngineGetToolBox()->GetHashString(intMapIter->first);
			name.tolower();

			bool remove = false;
			if (_tcsstr(name, m_szFilterString) != NULL)
			{
				if (m_bExcludeFilter)
				{
					remove = true;
				}
			}
			else
			{
				if (!m_bExcludeFilter)
				{
					remove = true;
				}
			}

			if (remove)
			{	
				LVFINDINFO lvFindInfo;
				lvFindInfo.flags = LVFI_STRING;
				lvFindInfo.psz = name.c_str();

				LVITEM lvItem;
				lvItem.mask = LVIF_TEXT;

				int findResult = m_ListCtrl.FindItem(&lvFindInfo);
				if (findResult != -1)
				{
					m_ListCtrl.DeleteItem(findResult);
				}			

				intMapIter = m_InternalPerformanceMap.erase(intMapIter);
			}
			else
			{
				intMapIter++;
			}
		}
	}	
}

void CProfileBox::UpdateInternalMap(PERFORMANCEPROFILERMAP &map)
{
	// iterate thru our internal map to see if there's anything we need to remove
	PERFORMANCEPROFILERMAP::iterator intMapIter, extMapIter;
	
	intMapIter = m_InternalPerformanceMap.begin();
	for (; intMapIter != m_InternalPerformanceMap.end(); intMapIter++)
	{
		extMapIter = map.find(intMapIter->first);

		// in map
		if (extMapIter != map.end())
		{
			// update internal map, remove from external map
			intMapIter->second = extMapIter->second;
			map.erase(extMapIter);			
		}		
	}

	// now add everything still left in external map
	extMapIter = map.begin();
	for (; extMapIter != map.end(); extMapIter++)
	{
		m_InternalPerformanceMap[extMapIter->first] = extMapIter->second;
	}

	FilterMapNames();
}

void CProfileBox::OnBnClickedClearmap()
{
	// clear peaks and avgs too
	m_DisplayType->ClearSubmaps();

	ClearInternalMap();
}

void CProfileBox::OnBnClickedCleartickcheck()
{
	m_bClearMapOnTick = !m_bClearMapOnTick;
}

void CProfileBox::OnCbnKillfocusFiltercombo()
{
	// get string from edit control, add to combo box
	TCHAR buf[1024];
	m_FilterStringCombo.GetWindowText(buf, 1024);

	if (m_FilterStringCombo.FindStringExact(0, buf) == CB_ERR)
	{
		m_FilterStringCombo.AddString(buf);
	}

	m_szFilterString = buf;
	m_szFilterString.tolower();
}

void CProfileBox::OnCbnSelchangeFiltercombo()
{
	// get string from edit control, add to combo box
	TCHAR buf[1024];
	int selection = m_FilterStringCombo.GetCurSel();
	
	if (selection != CB_ERR)
	{
		m_FilterStringCombo.GetLBText(selection, buf);
	}	

	m_szFilterString = buf;
	m_szFilterString.tolower();
}

void CProfileBox::OnBnClickedRadioinclusive()
{
	m_bExcludeFilter = false;
}

void CProfileBox::OnBnClickedRadioexclusive()
{
	m_bExcludeFilter = true;
}

void CProfileBox::ClearInternalMap()
{
	m_ListCtrl.DeleteAllItems();
	m_InternalPerformanceMap.clear();
}