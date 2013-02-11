///============================================================================
/// \file		TimeOfDayEditor.h
/// \brief		Declaration of TimeOfDay Editor
/// \date		08-12-2005
/// \author		D. Patrick Ghiocel
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

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DatabaseEditor.h : header file
//
#undef AFX_DATA
#define AFX_DATA AFX_DATA_IMPORT

/////////////////////////////////////////////////////////////////////////////
// CDatabaseEditor document
class CTimeOfDayToolPal;

class CTimeOfDayEditor : public IDLL
{
	SINGLETONCONSTRUCTOROVERRIDE(CTimeOfDayEditor);
	CTimeOfDayEditor();
	
// implementation
public:   
	~CTimeOfDayEditor();
    virtual CString *GetName();
	virtual DWORD GetPriority();
    virtual BOOL Initialize();

	void OnCreate(CMDIFrameWnd *mainWnd);

	BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra,
		AFX_CMDHANDLERINFO* pHandlerInfo);

	void OnGlobalUpdateTick( void );
	void ShowTimeOfDayDialog( bool bShow );

	/// Show Weather Effects Dialog
	/// \param bShow - show or hide
	/// \return void
	void ShowWeatherDialog(bool bShow);


	
// attributes
private:
	IToolBox *m_ToolBox;

	CTimeOfDayToolPal m_TimeOfDayToolPal;
	CTimeOfDayDialog m_TimeOfDayDlg;
	/// Weather effects dialog
	CWeatherEffectsDialog m_WeatherEffectsDlg;
	/// Is weather dialog created
	BOOL m_bWeatherDlg;
};

extern AFX_EXTENSION_MODULE TimeOfDayEditorDLL;

#undef AFX_DATA
#define AFX_DATA