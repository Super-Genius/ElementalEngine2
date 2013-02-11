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
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDebugger* CDebugger::m_pDebugger = NULL;

CDebugger::CDebugger()
{
	m_pDebugger = this;
	m_pThread = NULL;
	m_ToolBox = EngineGetToolBox();
}

CDebugger::~CDebugger()
{
	if(m_pThread!=NULL)
 		delete m_pThread;
}

BOOL CDebugger::Prepare(CString &fileName)
{
	CMDIFrameWnd *mainFrame = DYNAMIC_DOWNCAST(CMDIFrameWnd, AfxGetMainWnd());
	CMDIChildWnd *childFrame = mainFrame->MDIGetActive();

	// store the handle of the child frame
	m_hWndMainFrame = childFrame->GetSafeHwnd();

	m_lua.PrepareDebugger(fileName);

	m_nMode = DMOD_NONE;
	
	return TRUE;
}

BOOL CDebugger::Start()
{
	if(m_pThread != NULL)
	{
 		delete m_pThread;
	}
	m_pThread = AfxBeginThread(StartDebugger, this,0,0,CREATE_SUSPENDED);
 	if(m_pThread!=NULL)
 	{
 		m_pThread->m_bAutoDelete=false;
 		m_pThread->ResumeThread();
 	}

	return m_pThread!=NULL;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// thread functions
//////////////////////////////////////////////////////////////////////////////////////////////

UINT CDebugger::StartDebugger()
{
	m_nLine = 0;
	m_nLevel = 0;
	
	::SendMessage(m_hWndMainFrame, DMSG_DEBUG_START, 0, 0);

	int nRet = m_lua.StartDebugger();
	EndThread();
	
	return nRet;
}

UINT CDebugger::StartDebugger( LPVOID pParam )
{	
	return ((CDebugger*)pParam)->StartDebugger();
}

void CDebugger::Write(const char* szMsg)
{
	::SendMessage(m_hWndMainFrame, DMSG_WRITE_DEBUG, (WPARAM)szMsg, 0);
}

void CDebugger::LineHook(const char *szFile, int nLine)
{
	if ( m_nMode == DMOD_STOP )
		EndThread();

	if ( ::SendMessage(m_hWndMainFrame, DMSG_HAS_BREAKPOINT, (WPARAM)szFile, (LPARAM)nLine) ||
		m_nMode==DMOD_STEP_INTO || 
		m_nMode==DMOD_BREAK ||
		(m_nMode==DMOD_STEP_OVER && m_nLevel<=0) || 
		(m_nMode==DMOD_STEP_OUT && m_nLevel<0) ||
		(m_nMode==DMOD_RUN_TO_CURSOR && m_strPathName.CompareNoCase(szFile)==0 && m_nLine==nLine) )
	{
		DebugBreak(szFile, nLine);		
	}
}

void CDebugger::FunctionHook(const char *szFile, int nLine, BOOL bCall)
{
	if ( m_nMode == DMOD_STOP )
		EndThread();

	m_nLevel += (bCall?1:-1);
}

void CDebugger::DebugBreak(const char *szFile, int nLine)
{
	m_nMode = DMOD_NONE;

	::SendMessage(m_hWndMainFrame, DMSG_GOTO_FILELINE, (WPARAM)szFile, (LPARAM)nLine);

	m_lua.DrawStackTrace();
	m_lua.DrawGlobalVariables();
	::SendMessage(m_hWndMainFrame, DMSG_REDRAW_WATCHES, 0, 0);
	::SendMessage(m_hWndMainFrame, DMSG_GOTO_STACKTRACE_LEVEL, 0, 0);

	m_event.ResetEvent();
	
	::SendMessage(m_hWndMainFrame, DMSG_DEBUG_BREAK, 0, 0);
	CSingleLock lock(&m_event, TRUE);

	if ( m_nMode == DMOD_STOP )
		EndThread();
}

//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

void CDebugger::Go()
{
	m_event.SetEvent();
	::SendMessage(m_hWndMainFrame, DMSG_DEBUG_START, 0, 0);
}

void CDebugger::StepInto()
{
	m_nMode = DMOD_STEP_INTO;
	Go();
}

void CDebugger::StepOver()
{
	m_nMode = DMOD_STEP_OVER;
	m_strPathName = m_lua.GetSource();
	m_nLevel = 0;
	Go();
}

void CDebugger::StepOut()
{
	m_nMode = DMOD_STEP_OUT;
	m_strPathName = m_lua.GetSource();
	m_nLevel = 0;
	Go();
}

void CDebugger::RunToCursor()
{
	CMDIFrameWnd *mainWnd = DYNAMIC_DOWNCAST(CMDIFrameWnd, AfxGetMainWnd());

	CLuaView* pView = DYNAMIC_DOWNCAST(CLuaView, mainWnd->GetActiveView());
	CLuaDoc* pDoc = pView->GetDocument();

	m_nMode = DMOD_RUN_TO_CURSOR;
	
	m_strPathName = pDoc->GetPathName();

	CLuaEditCtrl *editor = pView->GetEditor();
	int nLine = editor->GetCurrentLine();
	m_nLine = pDoc->GetNearestDebugLine(nLine);

	Go();
}

void CDebugger::Stop()
{
	m_nMode = DMOD_STOP;
	Go();

	MSG msg;
	while ( WaitForSingleObject (m_pThread->m_hThread, 1)==WAIT_TIMEOUT )
		if ( ::PeekMessage (&msg, NULL, 0, 0, PM_NOREMOVE) )
			AfxGetThread()->PumpMessage ();

	delete m_pThread;
 	m_pThread=NULL;
}

void CDebugger::Break()
{
	m_nMode = DMOD_BREAK;
}

void CDebugger::ClearStackTrace()
{
	::SendMessage(m_hWndMainFrame, DMSG_CLEAR_STACKTRACE, 0, 0);
}

void CDebugger::AddStackTrace(const char* szDesc, const char* szFile, int nLine)
{
	StackTrace st;
	st.szDesc = szDesc;
	st.szFile = szFile;
	st.nLine = nLine;

	::SendMessage(m_hWndMainFrame, DMSG_ADD_STACKTRACE, (WPARAM)&st, 0);
}

int CDebugger::GetStackTraceLevel()
{
	return ::SendMessage(m_hWndMainFrame, DMSG_GET_STACKTRACE_LEVEL, 0, 0);
}

void CDebugger::StackLevelChanged()
{
	m_lua.DrawLocalVariables();
}

void CDebugger::ClearLocalVariables()
{
	::SendMessage(m_hWndMainFrame, DMSG_CLEAR_LOCALVARIABLES, 0, 0);
}

void CDebugger::AddLocalVariable(const char *name, const char *type, const char *value)
{
	Variable var;
	var.szName = name;
	var.szType = type;
	var.szValue = value;

	::SendMessage(m_hWndMainFrame, DMSG_ADD_LOCALVARIABLE, (WPARAM)&var, 0);
}

void CDebugger::ClearGlobalVariables()
{
	::SendMessage(m_hWndMainFrame, DMSG_CLEAR_GLOBALVARIABLES, 0, 0);
}

void CDebugger::AddGlobalVariable(const char *name, const char *type, const char *value)
{
	Variable var;
	var.szName = name;
	var.szType = type;
	var.szValue = value;

	::SendMessage(m_hWndMainFrame, DMSG_ADD_GLOBALVARIABLE, (WPARAM)&var, 0);
}

BOOL CDebugger::GetCalltip(const char *szWord, char *szCalltip)
{
	return m_lua.GetCalltip(szWord, szCalltip);
}

void CDebugger::EndThread()
{
	FreeConsole();
	::SendMessage(m_pDebugger->m_hWndMainFrame, DMSG_DEBUG_END, 0, 0);
	m_pDebugger->Write("The program has exited...\n");
	m_pDebugger->m_lua.StopDebugger();
	AfxEndThread(0);
}

CString CDebugger::Eval(CString strCode)
{
	
	strCode = "return " + strCode;
	CString strRet;
	m_lua.Eval(strCode.GetBuffer(0), strRet.GetBuffer(256));
	strRet.ReleaseBuffer();

	return strRet;
}

void CDebugger::Execute(StdString &strInterpreter, CString &name)
{

	_spawnl( _P_NOWAIT, strInterpreter, strInterpreter, "\"" + name + "\"", NULL );	
//	_spawnl( _P_NOWAIT, strInterpreter, strInterpreter, NULL );	
}
