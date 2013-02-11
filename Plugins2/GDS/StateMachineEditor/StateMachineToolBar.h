///=====================================================================
/// \file	StateMachineToolBar.h
/// \brief	Declaration of CStateMachineToolBar
/// \date	12/04/2007
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
///=====================================================================
#ifndef _STATE_MACHINE_TOOLBAR_H_
#define _STATE_MACHINE_TOOLBAR_H_

class CStateMachineToolBar : public CGuiDialogBar
{
	DECLARE_DYNAMIC(CStateMachineToolBar);
	
	enum { IDD = IDD_STATEMACHINE_TOOLBAR };

public:
	CStateMachineToolBar();
	virtual ~CStateMachineToolBar();

	bool IsTransitionMode() const;
	void SetTransitionMode( bool mode );
	bool IsFlatMode() const;
	void SetFlatMode( bool mode );

protected:
	DECLARE_MESSAGE_MAP()

	virtual void DoDataExchange( CDataExchange* pDX );

	//{{AFX_MSG(CStateMachineToolBar)
	afx_msg LONG OnInitDialog( UINT, LONG );
	//}}AFX_MSG

private:
	void SetButtonIcon( int buttonId, int iconId );
};

#endif // _STATE_MACHINE_TOOLBAR_H_