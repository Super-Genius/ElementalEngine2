///============================================================================
/// \file		WatchListCtrl.h
/// \brief		Header file for Watch List Control
/// \date		06-21-2005
/// \author		Josh Lulewicz
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

#pragma once


// CWatchListCtrl

class CWatchListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CWatchListCtrl)

public:
	/// Constructor
	/// \param void
	/// \return void
	CWatchListCtrl();

	/// Destructor
	/// \param void
	/// \return void
	virtual ~CWatchListCtrl();

	/// Adds and empty row to the list control
	/// \param void
	/// \return void
	void AddEmptyRow();

	/// Updates the values of the watches
	/// \param void
	/// \return void
	void Redraw();

	/// Update specific row of the list control
	/// \param iItem - index into string array
	/// \return void
	void UpdateRow(int iItem);

	/// Add item to list control
	/// \param item - item to add
	/// \return void
	void AddEditItem(LVITEM& item);

protected:
	/// Array of items (stored as strings) in the control
	CStringArray m_exps;

	DECLARE_MESSAGE_MAP()
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	afx_msg void OnLvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
};


