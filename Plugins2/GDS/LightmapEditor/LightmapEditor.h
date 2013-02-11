///============================================================================
/// \file		LightmapEditor.h
/// \brief		Declaration of Lightmap Editor
/// \date		09-07-2005
/// \author		Marvin Gouw
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

#undef AFX_DATA
#define AFX_DATA AFX_DATA_IMPORT

//class CLightmapToolPal;
class CLightmapEditor : public IDLL
{
	SINGLETONCONSTRUCTOROVERRIDE(CLightmapEditor);
	CLightmapEditor();
	
// implementation
public:   
	~CLightmapEditor();
    virtual CString *GetName();
	virtual DWORD GetPriority();
    virtual BOOL Initialize();

	void OnCreate(CMDIFrameWnd *mainWnd);

	BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra,
		AFX_CMDHANDLERINFO* pHandlerInfo);

	void OnGlobalUpdateTick( void );
//	void ShowLightmapDialog( bool bShow );

	
// attributes
private:
	IToolBox *m_ToolBox;
	HMENU m_hMenu;
	HACCEL m_hAccel;

	// just needs to be be here to be instianted so it registers
	// it's menu handlers.
	CLightmapCmdTarget m_LightmapCmdTarget;
};

extern AFX_EXTENSION_MODULE LightmapEditorDLL;

#undef AFX_DATA
#define AFX_DATA