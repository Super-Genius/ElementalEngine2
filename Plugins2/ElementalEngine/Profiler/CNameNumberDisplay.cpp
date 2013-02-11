///============================================================================
/// \file		CNameNumberDisplay.h
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

CNameNumberDisplay::CNameNumberDisplay()
{
	m_iNumColumns = 0;
	// need to find some way of modifying this...
	m_iMaxAvgStep = 50;
}

CNameNumberDisplay::~CNameNumberDisplay()
{}

void CNameNumberDisplay::SortColumn(UINT column, bool bAscend, CListCtrl &listCtrl)
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
		INTSORTMAP sortMap;
		vector<ITEMSZVEC>::iterator vecIter = itemVec.begin();
		for (; vecIter != itemVec.end(); vecIter++)
		{
			StdString sortString = (*vecIter)[column];
			int insInt = _tstoi(sortString.c_str());
			sortMap.insert(pair<int, ITEMSZVEC>(insInt, (*vecIter)));
		}

		UINT itemCount = 0;
		if (bAscend)
		{			
			for (INTSORTMAP::iterator mapIter = sortMap.begin(); mapIter != sortMap.end(); mapIter++)
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
			for (INTSORTMAP::reverse_iterator mapIter = sortMap.rbegin(); mapIter != sortMap.rend(); mapIter++)
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

void CNameNumberDisplay::InsertColumns(CListCtrl &listCtrl)
{
	listCtrl.InsertColumn(0, _T("Profile Name"), LVCFMT_LEFT, 400);
	listCtrl.InsertColumn(1, _T("Object Count"), LVCFMT_LEFT, 90);
	m_iNumColumns = 2;
}

void CNameNumberDisplay::UpdateProfile(PERFORMANCEPROFILERMAP &map, CListCtrl &listCtrl)
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

		// object count
		TCHAR buf[200];
		int iObjectCount = (int)mapIter->second;
		_stprintf(buf, _T("%i"), iObjectCount);

		lvItem.iSubItem = 1;
		lvItem.pszText = buf;		
		listCtrl.SetItem(&lvItem);		
	}
}

void CNameNumberDisplay::ClearSubmaps()
{
	m_PeakTimes.clear();
	m_AvgTimes.clear();
}