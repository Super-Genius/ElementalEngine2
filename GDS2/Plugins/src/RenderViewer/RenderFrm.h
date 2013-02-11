///============================================================================
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

typedef list<LPDEVMODE> DISPLAYLIST;

// CRenderFrm frame

//class CFullScreenFrame;
#include "FullScreenFrame.h"

class CRenderFrm : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CRenderFrm)

public: 
	/// \brief	Setup the play fullscreen menu
	void SetupFullScreenMenu();

protected:
	CRenderFrm();           // protected constructor used by dynamic creation
	virtual ~CRenderFrm();
	
	/// \brief	Disable fullscreen mode
	void OnStopFullScreen();

	/// \brief	Select the play fullscreen option
	///	\param	uID = the id of the resolution selected
	void OnClickPlayFullScreen(UINT uID);

	HINSTANCE m_AppInst;

protected:
	// Generated message map functions
	//{{AFX_MSG(CRenderFrm)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	/// \brief	Creates a list of compatible display resolutions
	void CreateDisplayList();

	/// \brief	Clears the list of display resolutions
	void RemoveDisplayList();

private:
	DISPLAYLIST m_DisplayList;
	IToolBox * m_ToolBox;
public:
	virtual void ActivateFrame(int nCmdShow = -1);
};
