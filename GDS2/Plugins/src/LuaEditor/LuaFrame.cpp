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

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define NUMICONS 4
#define NICONSWIDE 4

/////////////////////////////////////////////////////////////////////////////
// CLuaFrame

IMPLEMENT_DYNCREATE(CLuaFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CLuaFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CLuaFrame)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
	ON_WM_CREATE()
	ON_COMMAND(ID_LUA_COMPILE, OnCompile)
	ON_COMMAND(ID_LUA_EXECUTE, OnExecute)
	ON_COMMAND(ID_LUA_GO, OnGo)
	ON_COMMAND(ID_DEBUG_STEPINTO, OnDebugStepinto)
	ON_UPDATE_COMMAND_UI(ID_DEBUG_STEPINTO, OnUpdateDebugStepinto)
	ON_COMMAND(ID_DEBUG_RESTART, OnDebugRestart)
	ON_UPDATE_COMMAND_UI(ID_DEBUG_RESTART, OnUpdateDebugRestart)
	ON_COMMAND(ID_DEBUG_RUNTOCURSOR, OnDebugRuntocursor)
	ON_UPDATE_COMMAND_UI(ID_DEBUG_RUNTOCURSOR, OnUpdateDebugRuntocursor)
	ON_COMMAND(ID_DEBUG_STEPOUT, OnDebugStepout)
	ON_UPDATE_COMMAND_UI(ID_DEBUG_STEPOUT, OnUpdateDebugStepout)
	ON_COMMAND(ID_DEBUG_STEPOVER, OnDebugStepover)
	ON_UPDATE_COMMAND_UI(ID_DEBUG_STEPOVER, OnUpdateDebugStepover)
	ON_COMMAND(ID_DEBUG_STOPDEBUGGING, OnDebugStopdebugging)
	ON_UPDATE_COMMAND_UI(ID_DEBUG_STOPDEBUGGING, OnUpdateDebugStopdebugging)
	ON_COMMAND(ID_LUA_VIEW_TOOLBAR, OnLuaViewToolbar)
	ON_UPDATE_COMMAND_UI(ID_LUA_VIEW_TOOLBAR, OnUpdateLuaViewToolbar)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLuaFrame construction/destruction

CLuaFrame::CLuaFrame()
{
	// TODO: add member initialization code here
	m_ToolBox = EngineGetToolBox();
	m_bDebugging = false;
	
}

CLuaFrame::~CLuaFrame()
{
}

BOOL CLuaFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	if (!CMDIChildWnd::PreCreateWindow(cs))
	{
		return FALSE;
	}

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CLuaFrame diagnostics

#ifdef _DEBUG
void CLuaFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CLuaFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLuaFrame message handlers

BOOL CLuaFrame::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CMDIChildWnd::PreTranslateMessage(pMsg);
}

int CLuaFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	CMDIFrameWnd *mainWnd;
		
	// Register the doc templates we provide to the app
	CWinApp* pApp = AfxGetApp();
	ASSERT(pApp != NULL);
	mainWnd = DYNAMIC_DOWNCAST(CMDIFrameWnd, pApp->m_pMainWnd);

	// get the apps instance handle for loading resources
	m_AppInst = AfxGetResourceHandle();
	// now tell the application to look in dll for resources
	AfxSetResourceHandle(LuaEditorDLL.hModule);

	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	if (!m_LuaToolPal.CreateEx(mainWnd, TBSTYLE_FLAT | TBSTYLE_WRAPABLE , WS_CHILD | WS_VISIBLE | CBRS_RIGHT
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_FIXED, CRect(0, 2, 2, 4)) ||
		!m_LuaToolPal.LoadToolBar(IDR_LUA_TOOL_PALETTE))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	m_LuaToolPal.SetBorders(2, 2, 2, 2);

	m_LuaToolPal.EnableDocking(CBRS_ORIENT_ANY);
	// make control bar notify us.

	mainWnd->DockControlBar(&m_LuaToolPal, CBRS_ALIGN_TOP);

	CRect tcRect;
	m_LuaToolPal.GetToolBarCtrl().SetRows(1, FALSE, tcRect);
	int iconWidth;
	int iconHeight;
	int nRows;

	iconWidth = tcRect.Width()/NUMICONS;
	iconHeight = tcRect.Height();
	nRows = (NUMICONS / NICONSWIDE) + (((NUMICONS % NICONSWIDE) == 0) ? 0 : 1);

	tcRect.SetRect(0, 0, iconWidth * NICONSWIDE, (iconHeight * nRows));

	// Set the window size
	m_LuaToolPal.SetWindowPos(NULL, 0, 0, tcRect.Width(), tcRect.Height(),
		SWP_NOACTIVATE  | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOREDRAW);

	
	mainWnd->ShowControlBar(&m_LuaToolPal, FALSE, FALSE);

	AfxSetResourceHandle(m_AppInst);

	return 0;
}

void CLuaFrame::OnCompile() 
{
	CompileCode();
}

bool CLuaFrame::CompileCode()
{
	CExecutor m_exe;
	CString strCmdLine;

	// clear the output
	Clear(outputBuild);

	// get doc
	CLuaDoc *pDoc = DYNAMIC_DOWNCAST(CLuaDoc, GetActiveView()->GetDocument());
	// get view
	CLuaView *view = DYNAMIC_DOWNCAST(CLuaView, GetActiveView());

	// if the doc has been modified
	if (pDoc->IsModified())
	{
		// save it
		pDoc->DoFileSave();

		// writing new lines changes breakpoint positions
		CLuaEditCtrl *editor = view->GetEditor();	
		editor->SetBreakPointsIn(pDoc);
	}

	CString name = pDoc->GetPathNameNoExt();
	name = name + ".out";

	strCmdLine.Format("\"%s\" -o \"%s\" \"%s\"", 
		"luac.exe", name, pDoc->GetPathName());

	CString strMsg;
	strMsg.Format("------------------ Compiling %s -------------------\n\n", pDoc->GetName() + ".lua");

	Write(&StdString(strMsg));
	
	m_exe.Execute(strCmdLine);

	CString strOutput = m_exe.GetOutputString();

	// if compilation failed, write error to output window
	if ( !strOutput.IsEmpty() )
	{
		Write(&StdString(strOutput));
		return false;
	}

	strMsg = _T("---------------------- Done ----------------------\n");
	strMsg += _T("Compiled Successfully\n");

	Write(&StdString(strMsg));

	return true;
}

void CLuaFrame::OnExecute() 
{
	if(!CompileCode())
	{
		return;
	}

	// get view
	CLuaView *view = DYNAMIC_DOWNCAST(CLuaView, GetActiveView());
	// get the doc
	CLuaDoc *pDoc = view->GetDocument();
	// get file name
	CString name = pDoc->GetPathNameNoExt();
	name = name + ".out";

	//StdString strInterpreter = "C:\\Development\\Graphics Dev System\\bin\\debug\\lua.exe";
	// lua executable
	StdString strInterpreter = "lua.exe";

	// execute
	m_debug.Execute(strInterpreter, name);
}

void CLuaFrame::OnGo()
{
	// if we aren't debugggin prepare the debugger and start it
	if (!m_bDebugging)
	{
		// make sure script is compiled
		if(!CompileCode())
		{
			return;
		}

		// Clear output
		Clear(outputDebug);

		// get view
		CLuaView *view = DYNAMIC_DOWNCAST(CLuaView, GetActiveView());
		// get document
		CLuaDoc *doc = view->GetDocument();
		// get file name
		CString name = doc->GetPathName();
		
		// make sure breakpoints are valid
		if (doc->PositionBreakPoints(name) )
		{
			AfxMessageBox("One or more breakpoints are not positioned on valid lines. These breakpoints have been moved to the next valid line.", MB_OK);
		}
		
		// prepare debugger
		m_debug.Prepare(name);
		// start debugging
		m_debug.Start();
	}
	else
	{
		// otherwise just proceed
		m_debug.Go();
	}
}

LRESULT CLuaFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// if we received a debug message
	if ( message >= _DMSG_FIRST_MSG && message <= _DMSG_LAST_MSG )
	{
		return DebugMessage(message, wParam, lParam);
	}

	// otherwise forward message to child
	return CMDIChildWnd::WindowProc(message, wParam, lParam);
}

LRESULT CLuaFrame::DebugMessage(UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	CCallStack *callStack = NULL;

	// only get the callstack if the message pertains to it
	if (nMsg >= DMSG_CLEAR_STACKTRACE && nMsg <= DMSG_GET_STACKTRACE_LEVEL || 
		nMsg == DMSG_DEBUG_END)
	{
		// get the main instance
		CLuaEditor *theEditor = (CLuaEditor*)GetDLLInterface();
		ASSERT(theEditor);
		// get output window
		CLuaDockToolTab *outputWnd = theEditor->GetOutputWnd();
		ASSERT(outputWnd->GetSafeHwnd());
		// Get Call Stack
		callStack = outputWnd->GetCallStack();
		ASSERT(callStack);
	}

	switch( nMsg )
	{
	case DMSG_WRITE_DEBUG:
		{
			const char* msg = (const char*)wParam;
			StdString string(msg);
			Write(&string, outputDebug);
		}
		break;
	case DMSG_HAS_BREAKPOINT:
		{
			CLuaView *view = DYNAMIC_DOWNCAST(CLuaView, GetActiveView());
			CLuaDoc *doc = view->GetDocument();
			
			return doc->HasBreakPoint((int)lParam);
		}
		break;
	case DMSG_GOTO_FILELINE:
		GotoLine((int)lParam);
		break;
	case DMSG_DEBUG_START:
		m_bDebugging = true;
		break;
	case DMSG_DEBUG_BREAK:
		break;
	case DMSG_DEBUG_END:
		{
			ClearMarkers();
			if (callStack)
			{
				callStack->Clear();
			}
			m_bDebugging = false;
		}
		break;
	case DMSG_CLEAR_STACKTRACE:
		{
			if (callStack)
			{
				callStack->Clear();
			}
		}
		break;
	case DMSG_ADD_STACKTRACE:
		{
			if (callStack)
			{
				callStack->Add(((StackTrace*)wParam)->szDesc, 
								((StackTrace*)wParam)->szFile, 
								((StackTrace*)wParam)->nLine);
			}
		}
		break;
	case DMSG_GOTO_STACKTRACE_LEVEL:
		{
			if (callStack)
			{
				callStack->GotoStackTraceLevel(wParam);
			}
		}
		break;
	case DMSG_GET_STACKTRACE_LEVEL:
		{
			if (callStack)
			{
				return callStack->GetLevel();
			}
		}
		break;
	case DMSG_CLEAR_LOCALVARIABLES:
//		m_wndLocals.RemoveAll();
		break;
	case DMSG_ADD_LOCALVARIABLE:
//		m_wndLocals.AddVariable(((Variable*)wParam)->szName, 
//			((Variable*)wParam)->szType, 
//			((Variable*)wParam)->szValue);
		break;
	case DMSG_REDRAW_WATCHES:
		UpdateWatches();
		break;
	default:
		break;
	}

	return 0;
}

void CLuaFrame::Write(StdString *message, outputTypes tab)
{
	// get the main instance
	CLuaEditor *theEditor = (CLuaEditor*)GetDLLInterface();
	ASSERT(theEditor);
	// get output window
	CLuaDockToolTab *outputWnd = theEditor->GetOutputWnd();
	ASSERT(outputWnd->GetSafeHwnd());
	// write data
	outputWnd->SetText(message, tab);
}

void CLuaFrame::Clear(outputTypes tab)
{
	// get the main instance
	CLuaEditor *theEditor = (CLuaEditor*)GetDLLInterface();
	ASSERT(theEditor);
	// get output window
	CLuaDockToolTab *outputWnd = theEditor->GetOutputWnd();
	ASSERT(outputWnd->GetSafeHwnd());
	// write data
	outputWnd->ClearOutput(tab);
}


void CLuaFrame::UpdateWatches()
{
	// get the main instance
	CLuaEditor *theEditor = (CLuaEditor*)GetDLLInterface();
	ASSERT(theEditor);
	// get output window
	CLuaDockToolTab *outputWnd = theEditor->GetOutputWnd();
	ASSERT(outputWnd->GetSafeHwnd());
	// write data
	outputWnd->UpdateWatches();
}


void CLuaFrame::GotoLine(int nLine)
{
	// get view
	CLuaView *view = DYNAMIC_DOWNCAST(CLuaView, GetActiveView());
	// get edit control
	CLuaEditCtrl *editCtrl = view->GetEditor();
	// put arrow at current line 
	editCtrl->GotoLine(nLine);
}

void CLuaFrame::ClearMarkers()
{
	// get current view
	CLuaView *view = DYNAMIC_DOWNCAST(CLuaView, GetActiveView());
	// get the edit control of that view
	CLuaEditCtrl *editCtrl = view->GetEditor();
	// clear the arrow markers
	editCtrl->ClearMarkers();
}


void CLuaFrame::OnDebugStepinto()
{
	// TODO: Add your command handler code here
	m_debug.StepInto();
}

void CLuaFrame::OnUpdateDebugStepinto(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if (m_bDebugging)
		pCmdUI->Enable(true);
	else
		pCmdUI->Enable(false);
}

void CLuaFrame::OnDebugRestart()
{
	// TODO: Add your command handler code here
}

void CLuaFrame::OnUpdateDebugRestart(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if (m_bDebugging)
		pCmdUI->Enable(true);
	else
		pCmdUI->Enable(false);
}

void CLuaFrame::OnDebugRuntocursor()
{
	// TODO: Add your command handler code here
	m_debug.RunToCursor();
}

void CLuaFrame::OnUpdateDebugRuntocursor(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if (m_bDebugging)
		pCmdUI->Enable(true);
	else
		pCmdUI->Enable(false);
}

void CLuaFrame::OnDebugStepout()
{
	// TODO: Add your command handler code here
	m_debug.StepOut();
}

void CLuaFrame::OnUpdateDebugStepout(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if (m_bDebugging)
		pCmdUI->Enable(true);
	else
		pCmdUI->Enable(false);
}

void CLuaFrame::OnDebugStepover()
{
	// TODO: Add your command handler code here
	m_debug.StepOver();
}

void CLuaFrame::OnUpdateDebugStepover(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if (m_bDebugging)
		pCmdUI->Enable(true);
	else
		pCmdUI->Enable(false);
}

void CLuaFrame::OnDebugStopdebugging()
{
	// TODO: Add your command handler code here
	m_debug.Stop();
}

void CLuaFrame::OnUpdateDebugStopdebugging(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if (m_bDebugging)
		pCmdUI->Enable(true);
	else
		pCmdUI->Enable(false);
}

void CLuaFrame::OnLuaViewToolbar()
{
	// TODO: Add your command handler code here
	// get the editor
	CLuaEditor *theEditor = (CLuaEditor*)GetDLLInterface();
	ASSERT(theEditor);
	// get output window
	CLuaDockToolTab *outputWnd = theEditor->GetOutputWnd();
	ASSERT(outputWnd->GetSafeHwnd());

	if (outputWnd->IsVisible() || outputWnd->IsMinimized())
	{
	//	ShowControlBar(outputWnd, FALSE, FALSE);	//The original version
		outputWnd->Close();
	}
	else
	{
//		ShowControlBar(outputWnd, TRUE, FALSE);		//The original version
		outputWnd->Show();
	}
}

void CLuaFrame::OnUpdateLuaViewToolbar(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	// get the editor
	CLuaEditor *theEditor = (CLuaEditor*)GetDLLInterface();
	ASSERT(theEditor);
	// get output window
	CLuaDockToolTab *outputWnd = theEditor->GetOutputWnd();
	ASSERT(outputWnd->GetSafeHwnd());

	pCmdUI->SetCheck( outputWnd->IsVisible() || outputWnd->IsMinimized() );
	pCmdUI->Enable();
}
