///============================================================================
/// \file		CNameTimeDisplay.h
/// \brief		Implements IProfileDisplay for data with name and times
/// \date		06/14/2007
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

CNameTimeDisplay::CNameTimeDisplay()
{
	m_iNumColumns = 0;
	// need to find some way of modifying this...
	m_iMaxAvgStep = 50;
}

CNameTimeDisplay::~CNameTimeDisplay()
{}

void CNameTimeDisplay::SortColumn(UINT column, bool bAscend, CListCtrl &listCtrl)
{
	TCHAR buf[1024];

	vector<ITEMSZVEC> itemVec;

	// grab all the item strings
	for (int i=0; i<listCtrl.GetItemCount(); i++)
	{
		StdString insString;

		ITEMSZVEC stringVec;

		for (UINT j=0; j<m_iNumColumns; j++)
		{
			listCtrl.GetItemText(i, j, buf, 1024);
			insString = buf;
			stringVec.push_back(insString);			
		}

		itemVec.push_back(stringVec);
	}

	// remove everything
	listCtrl.DeleteAllItems();

	// figure out what map to use and how to sort
	if (column == 0)
	{
		STRINGSORTMAP sortMap;
		vector<ITEMSZVEC>::iterator vecIter = itemVec.begin();
		for (; vecIter != itemVec.end(); vecIter++)
		{
			StdString sortString = (*vecIter)[column];
			sortMap.insert(pair<StdString, ITEMSZVEC>(sortString, (*vecIter)));
		}
		
		UINT itemCount = 0;
		if (bAscend)
		{			
			for (STRINGSORTMAP::iterator mapIter = sortMap.begin(); mapIter != sortMap.end(); mapIter++)
			{
				ITEMSZVEC vec = mapIter->second;
				
				LVITEM lvItem;
				lvItem.mask = LVIF_TEXT;
				lvItem.pszText = (TCHAR*)vec[0].c_str();
				lvItem.iItem = itemCount;
				lvItem.iSubItem = 0;
				listCtrl.InsertItem(&lvItem);

				lvItem.iSubItem = 1;
				lvItem.pszText = (TCHAR*)vec[1].c_str();
				listCtrl.InsertItem(&lvItem);

				itemCount++;
			}
		}
		else
		{
			for (STRINGSORTMAP::reverse_iterator mapIter = sortMap.rbegin(); mapIter != sortMap.rend(); mapIter++)
			{
				ITEMSZVEC vec = mapIter->second;
				
				LVITEM lvItem;
				lvItem.mask = LVIF_TEXT;
				lvItem.pszText = (TCHAR*)vec[0].c_str();
				lvItem.iItem = itemCount;
				lvItem.iSubItem = 0;
				listCtrl.InsertItem(&lvItem);

				lvItem.iSubItem = 1;
				lvItem.pszText = (TCHAR*)vec[1].c_str();
				listCtrl.InsertItem(&lvItem);

				itemCount++;
			}
		}
	}
	else
	{
		FLOATSORTMAP sortMap;
		vector<ITEMSZVEC>::iterator vecIter = itemVec.begin();
		for (; vecIter != itemVec.end(); vecIter++)
		{
			StdString sortString = (*vecIter)[column];
			float insFloat = (float)_tstof(sortString.c_str());
			sortMap.insert(pair<float, ITEMSZVEC>(insFloat, (*vecIter)));
		}

		UINT itemCount = 0;
		if (bAscend)
		{			
			for (FLOATSORTMAP::iterator mapIter = sortMap.begin(); mapIter != sortMap.end(); mapIter++)
			{
				ITEMSZVEC vec = mapIter->second;
				
				LVITEM lvItem;
				lvItem.mask = LVIF_TEXT;
				lvItem.pszText = (TCHAR*)vec[0].c_str();
				lvItem.iItem = itemCount;
				lvItem.iSubItem = 0;
				listCtrl.InsertItem(&lvItem);

				lvItem.iSubItem = 1;
				lvItem.pszText = (TCHAR*)vec[1].c_str();
				listCtrl.InsertItem(&lvItem);

				itemCount++;
			}
		}
		else
		{
			for (FLOATSORTMAP::reverse_iterator mapIter = sortMap.rbegin(); mapIter != sortMap.rend(); mapIter++)
			{
				ITEMSZVEC vec = mapIter->second;
				
				LVITEM lvItem;
				lvItem.mask = LVIF_TEXT;
				lvItem.pszText = (TCHAR*)vec[0].c_str();
				lvItem.iItem = itemCount;
				lvItem.iSubItem = 0;
				listCtrl.InsertItem(&lvItem);

				lvItem.iSubItem = 1;
				lvItem.pszText = (TCHAR*)vec[1].c_str();
				listCtrl.InsertItem(&lvItem);

				itemCount++;
			}
		}
	}
}

void CNameTimeDisplay::InsertColumns(CListCtrl &listCtrl)
{
	listCtrl.InsertColumn(0, _T("Profile Name"), LVCFMT_LEFT, 400);
	listCtrl.InsertColumn(1, _T("Current Time"), LVCFMT_LEFT, 90);
	listCtrl.InsertColumn(2, _T("Avg Time"), LVCFMT_LEFT, 90);
	listCtrl.InsertColumn(3, _T("Peak Time"), LVCFMT_LEFT, 90);
	m_iNumColumns = 4;
}

void CNameTimeDisplay::UpdateProfile(PERFORMANCEPROFILERMAP &map, CListCtrl &listCtrl)
{
	IToolBox *toolBox = EngineGetToolBox();

	PERFORMANCEPROFILERMAP::iterator mapIter = map.begin();
	for (; mapIter != map.end(); mapIter++)
	{
		const TCHAR *name = toolBox->GetHashString(mapIter->first);		
	
		LVFINDINFO lvFindInfo;
		lvFindInfo.flags = LVFI_STRING;
		lvFindInfo.psz = name;

		LVITEM lvItem;
		lvItem.mask = LVIF_TEXT;

		int findResult = listCtrl.FindItem(&lvFindInfo);
		if (findResult == -1)
		{	
			lvItem.iSubItem = 0;
			lvItem.iItem = listCtrl.GetItemCount();			
			lvItem.pszText = (TCHAR*)name;
			listCtrl.InsertItem(&lvItem);
		}
		else
		{
			lvItem.iItem = findResult;
		}

		// current time
		TCHAR buf[20];
		float timeAdjust = mapIter->second * 1000.0f;
		_stprintf(buf, _T("%.2fms"), (timeAdjust));

		lvItem.iSubItem = 1;
		lvItem.pszText = buf;		
		listCtrl.SetItem(&lvItem);		

		// avg time
		float insTime = 0.0f;
		
		AVGTIMEMAP::iterator avgIter = m_AvgTimes.find(mapIter->first);
		if (avgIter == m_AvgTimes.end())
		{
			AVGTIMEVEC avgVec;
			avgVec.push_back(timeAdjust);
			insTime = timeAdjust;
			m_AvgTimes.insert(pair<DWORD, AVGTIMEVEC>(mapIter->first, avgVec));
		}
		else
		{
			AVGTIMEVEC *avgVec = &avgIter->second;
			if (avgVec)
			{
				UINT vecSize = avgVec->size();
				avgVec->push_back(timeAdjust);
				if (vecSize > m_iMaxAvgStep)
				{
					avgVec->erase(avgVec->begin());
				}
				
				for (AVGTIMEVEC::iterator vecIter = avgVec->begin(); vecIter != avgVec->end(); vecIter++)
				{
					insTime += *vecIter;
				}
				
				if (vecSize > 0)
				{
					insTime /= (float)vecSize;
				}
			}
		}
		
		_stprintf(buf, _T("%.2fms"), (insTime));
		lvItem.iSubItem = 2;
		lvItem.pszText = buf;		
		listCtrl.SetItem(&lvItem);	

		// peak time
		insTime = 0.0f;

		PEAKTIMEMAP::iterator peakIter = m_PeakTimes.find(mapIter->first);
		if (peakIter == m_PeakTimes.end())
		{
			m_PeakTimes.insert(pair<DWORD, float>(mapIter->first, timeAdjust));
			insTime = timeAdjust;
		}
		else
		{
			if (peakIter->second < timeAdjust)
			{
				peakIter->second = timeAdjust;
			}

			insTime = peakIter->second;
		}

		_stprintf(buf, _T("%.2fms"), (insTime));
		lvItem.iSubItem = 3;
		lvItem.pszText = buf;		
		listCtrl.SetItem(&lvItem);	
	}
}

void CNameTimeDisplay::ClearSubmaps()
{
	m_PeakTimes.clear();
	m_AvgTimes.clear();
}