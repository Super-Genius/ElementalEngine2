///============================================================================
/// \file		Executor.h
/// \brief		Header file for the file executor 
/// \date		06-17-2005
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

#if !defined(AFX_EXECUTOR_H__B467D883_9302_4A7E_810B_058B60881E4A__INCLUDED_)
#define AFX_EXECUTOR_H__B467D883_9302_4A7E_810B_058B60881E4A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CExecutor  
{
public:
	BOOL SaveOutput(CString strPathName);
	CString GetOutputString();
	CExecutor();
	virtual ~CExecutor();
	BOOL Execute(CString strCmdLine, CMemFile* pInput=NULL);

protected:
	void Close();
	void PrepAndLaunchRedirectedChild(HANDLE hChildStdOut, HANDLE hChildStdIn, HANDLE hChildStdErr,
											 CString strCmdLine);
	BOOL Write(LPBYTE lpData, int nSize);
	static DWORD WINAPI ReadAndHandleOutput(LPVOID lpvThreadParam);

	HANDLE m_hOutputRead, m_hInputWrite;
	HANDLE m_hThread;
	CMemFile m_output;
};

#endif // !defined(AFX_EXECUTOR_H__B467D883_9302_4A7E_810B_058B60881E4A__INCLUDED_)
