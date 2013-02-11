///============================================================================
/// \file	EventTransitionsPage.h
/// \brief	Declaration for event transitions property page
/// \date	12/06/2007
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
#ifndef __STATE_MACHINE_EVENT_TRANSITIONS_PAGE_H__
#define __STATE_MACHINE_EVENT_TRANSITIONS_PAGE_H__

class CEventTransitionsPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CEventTransitionsPage)

	enum { IDD = IDD_EVENT_TRANSITIONS };

	enum
	{
		COL_DATA = 0,
		COL_WEIGHT = 0,
		COL_TARGET = 1,
		COL_COUNT = 2,
	};

	struct CTransitionInfo
	{
		CHashString hszObjectName;
		StdString sName;
		int iWeight;
		StdString sTargetState;

		DWORD GetNameHash() const
		{
			return hszObjectName.GetUniqueID();
		}
	};

	typedef vector<CTransitionInfo> InfoList;

public:
	CEventTransitionsPage( LPCTSTR szEventName, bool isAbstract );
	virtual ~CEventTransitionsPage();

protected:

	virtual BOOL OnInitDialog();
	virtual BOOL OnApply();

	bool Validate();

	afx_msg void OnBnClickedAdd();
	afx_msg void OnBnClickedRemove();
	afx_msg void OnGridSelChanged( NMHDR *pNotifyStruct, LRESULT *result );
	afx_msg void OnGridEndEdit( NMHDR *pNotifyStruct, LRESULT *result );

	DECLARE_MESSAGE_MAP()

	///	\breif	create grid and initialize columns
	void InitalizeGrid();
	///	\brief	load grid from m_Transitions array
	void RefreshGrid();
	///	\brief	add new transition to the grid
	///	\param	details of new transition
	void AddTransition( const CTransitionInfo &details );
	///	\breif	insert new row to the grid and the specific position
	///	\param	row - desired row index. if this value is -1 than new row will
	///				  be appended to the grid
	///	\return	index of inserted row
	int InsertRow( int row );

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

	///	\breif	load current transition handler states for the event
	void LoadTransitions();

	///	\brief	load states list for combo box filling for specific row
	///	\param	row - index of row to fill
	void LoadStates( int row );

	///	\breif	check if changes in the grid exist against original data
	///	\return	TRUE if something was changed in the grid
	BOOL HasChanges() const;

	///	\brief	update dialog buttons state based on the grid selection
	void UpdateButtonStates();

	///	\brief	check if delete is allowed
	///	\return true if delete operation is allowed
	bool CanDelete() const;

private:
	///	\breif	set button type to icon, load and set icon
	void SetButtonIcon( int id );

	///	\brief	write error message to EE log and show message box with message
	///	\param	resourceId - id of error string resource
	void ShowError( int resourceId );

	///	\brief	fill state list
	void FillStates();

private:
	static CHashString m_hszCQHStateMachineTransition;
	static CHashString m_hszCQHStateMachineEvent;
	CHashString	m_hszEventName;
	CGridCtrl	m_Grid;
	BOOL		m_bModified;
	InfoList	m_Transitions;
	CStringArray m_States;
	bool		m_isAbstract;
};

#endif //__STATE_MACHINE_EVENT_TRANSITIONS_PAGE_H__