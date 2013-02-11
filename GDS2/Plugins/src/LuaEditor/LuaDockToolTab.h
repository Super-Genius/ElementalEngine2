///============================================================================
/// \file		LuaDockToolTab.h
/// \brief		Header file for CLuaDockToolTab
/// \date		06-14-2005
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
#include "guicontrolbar.h"

enum outputTypes{
	outputBuild = 0,
	outputDebug,
	outputFiF
};

/// \class CLuaDockToolTab
/// \brief Tool window for Lua Editor
///
/// Tool window to display debugging information,
/// watches, call stack etc.
class CLuaDockToolTab : public CGuiControlBar
{
public:
	CLuaDockToolTab(void);
	~CLuaDockToolTab(void);

	/// Add text to the output window
	/// \param string - text you want added to the window
	/// \param type - output window you want to use (build, debug, etc)
	/// \return void
	void SetText(StdString *string, outputTypes type = outputBuild);

	/// Clears a specified output window
	/// \param type - window to be cleared
	/// \return void
	void ClearOutput(outputTypes type = outputBuild);

	/// Update the values of the variables in the watch list
	/// \param void
	/// \return void
	void UpdateWatches();

	/// Get the call stack
	/// \param void
	/// \return pointer to the call stack object
	CCallStack* GetCallStack() { return &m_CallStackWnd; }

	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

protected:
	/// Tabbed window
	CGuiWorkTab	m_WorkTabWnd;
	/// Debug output window
	CRichEditCtrl m_DebugWnd;
	/// Build output window
	CRichEditCtrl m_BuildWnd;
	/// Call stack output window
	CCallStack m_CallStackWnd;
	/// Variable watch window
	CWatchListCtrl m_WatchWnd;
};
