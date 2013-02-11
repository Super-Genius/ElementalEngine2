///============================================================================
/// \file	EventActionsPage.h
/// \brief	Declaration for event actions property page
/// \date	12/05/2007
/// \author	Andrey Ivanov
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
#ifndef __STATE_MACHINE_EVENT_ACTIONS_PAGE_H__
#define __STATE_MACHINE_EVENT_ACTIONS_PAGE_H__

class CEventActionsPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CEventActionsPage)

	enum { IDD = IDD_EVENT_ACTIONS };

	enum
	{
		COL_DATA = 0,
		COL_ACTION_NAME = 0,
		COL_COUNT = 1,
	};

	struct CActionInfo
	{
		CHashString hszObjectName;
		StdString sName;
		StdString sActionName;

		DWORD GetNameHash() const
		{
			return hszObjectName.GetUniqueID();
		}
	};

	typedef vector<CActionInfo> InfoList;

public:
	CEventActionsPage( LPCTSTR szEventName, bool isAbstract );
	virtual ~CEventActionsPage();

protected:

	virtual BOOL OnInitDialog();
	virtual BOOL OnApply();

	bool Validate();

	afx_msg void OnBnClickedAdd();
	afx_msg void OnBnClickedRemove();
	afx_msg void OnBnClickedUp();
	afx_msg void OnBnClickedDown();
	afx_msg void OnGridSelChanged( NMHDR *pNotifyStruct, LRESULT *result );
	afx_msg void OnGridEndEdit( NMHDR *pNotifyStruct, LRESULT *result );

	DECLARE_MESSAGE_MAP()

	///	\breif	create grid and initialize columns
	void InitalizeGrid();
	///	\brief	load grid from m_Actions array
	void RefreshGrid();
	///	\brief	add new action to the grid
	///	\param	details of new action
	void AddAction( const CActionInfo &details );
	///	\breif	insert new row to the grid and the specific position
	///	\param	row - desired row index. if this value is -1 than new row will
	///				  be appended to the grid
	///	\return	index of inserted row
	int InsertRow( int row );
	///	\breif	move row from one position to another
	///	\param	sourceRow - current row index
	///	\param	targetRow - new row index
	void MoveRow( int sourceRow, int targetRow );

	///	\breif	extract object name for the grid row
	///	\param	row - index of the row
	///	\return	pointer to string with object name
	LPCTSTR GetItemData( int row ) const
	{
		return reinterpret_cast<LPCTSTR>( m_Grid.GetItemData( row, COL_DATA ) );
	}

	///	\breif	set object name as data for the grid row
	///	\param	row - index of the row
	///	\param	name - pointer to IHashString with object name
	void SetItemData( int row, const CHashString &name )
	{
		m_Grid.SetItemData( row, COL_DATA, PtrToLong( name.GetString() ) );
	}

	///	\breif	load current action handler states for the event
	void LoadActions();

	///	\breif	check if changes in the grid exist against original data
	///	\return	TRUE if something was changed in the grid
	BOOL HasChanges() const;

	///	\brief	update dialog buttons state based on the grid selection
	void UpdateButtonStates();

	///	\brief	check if delete is allowed
	///	\return true if delete operation is allowed
	bool CanDelete() const;

	///	\brief	check if move up operation is allowed
	///	\return true if move up operation is allowed
	bool CanMoveUp() const;

	///	\brief	check if move down operation is allowed
	///	\return true if move down operation is allowed
	bool CanMoveDown() const;

private:
	///	\breif	set button type to icon, load and set icon
	void SetButtonIcon( int id );

	///	\brief	write error message to EE log and show message box with message
	///	\param	resourceId - id of error string resource
	void ShowError( int resourceId );

private:
	static CHashString m_hszCQHStateMachineActionHandler;
	static CHashString m_hszCQHStateMachineEvent;
	CHashString	m_hszEventName;
	CGridCtrl	m_Grid;
	BOOL		m_bModified;
	InfoList	m_Actions;
	bool		m_isAbstract;
};

#endif //__STATE_MACHINE_EVENT_ACTIONS_PAGE_H__