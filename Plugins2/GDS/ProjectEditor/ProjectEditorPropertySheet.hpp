///============================================================================
/// \file		ProjectEditorPropertySheet.hpp
/// \brief		Declaration of the new project settings property sheet.
/// \date		01-03-2009
/// \author		Zsolt Matyas
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

#ifndef _PROJECTEDITORPROPERTYSHEET_H
#define _PROJECTEDITORPROPERTYSHEET_H

#include "StdAfx.h"
#include "afxdlgs.h"

class CProjectEditorPropertySheet :
	public CPropertySheet
{
	DECLARE_DYNAMIC( CProjectEditorPropertySheet )

public:
	// ctor + destor
	//	CProjectEditorPropertySheet(void); // this was the default, we do not use it
	CProjectEditorPropertySheet::CProjectEditorPropertySheet( LPCTSTR pszCaption,
   CWnd* pParentWnd);
	~CProjectEditorPropertySheet(void);

	///---------------------------------------------------------------------
	/// Function:	SetModelessProperty
	/// Description:	Sets the flages to show the property sheet as a modal dialog
	/// although it is a modeless window
	/// Parameters:	newState - TRUE to make it modeless, false to modal
	/// Returns: TRUE.
	///---------------------------------------------------------------------
	BOOL SetModelessProperty(BOOL newState);

	///---------------------------------------------------------------------
	/// Function:	RefreshPageValues
	/// Description:	Calles the function with the same name for every
	///					property Page.
	/// Parameters:	.
	/// Returns: void
	///---------------------------------------------------------------------
	 void RefreshPageValues();


protected:
	// Message handlers
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();


	// BUTTON HANDLERS
	afx_msg void OnCancelButtonClick();
	afx_msg void OnOkButtonClick(); 

	DECLARE_MESSAGE_MAP()

private:
};

#endif /* #ifndef _PROJECTEDITORPROPERTYSHEET_H */