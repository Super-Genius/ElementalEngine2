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

#if !defined(AFX_GDS_H__21823C45_287B_4C51_812A_8E8906CC4EB1__INCLUDED_)
#define AFX_GDS_H__21823C45_287B_4C51_812A_8E8906CC4EB1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

using namespace std;

typedef void (*INITDLL)();
typedef DWORD (*GETPRIORITY)();
typedef void (*DEINITDLL)();

struct GDSINITDLLINFO
{
	StdString dllName;
	INITDLL initDLL;
};

struct StrCmp : public greater<TCHAR *> 
{
	bool operator()(TCHAR *x, TCHAR *y)
	{
		// check if the names are the same
		if ( _tcscmp(x, y) != 0 )
		{
			// they are not the same return the order of the names
			return _tcscmp(x, y) < 0;
		}
		else
		{
			//The two filter names are the same: check the extensions
			TCHAR *x_ext, *y_ext;

			// get the length of the names: (they must be the same, if 
			// we are in the else case...)
			int filterNameLenght = (int) _tcslen( x );

			// set the extension pointers (names* + length(name) + 1)
			x_ext = x + filterNameLenght + 1; 
			y_ext = y + filterNameLenght + 1; 

			return _tcscmp(x_ext, y_ext) < 0;
		}
	}
};

typedef list<IDLL *> STDDLLLIST;
typedef list<HINSTANCE> HANDLELIST;
typedef map<DWORD, GDSINITDLLINFO> DLLINITMAP;
typedef	set< TCHAR *, StrCmp> FILTERSET;

#define INI_SubSection _T("Graphics Dev System")
#define INI_LastProjectPath _T("LastProjectPath") //TODO: This seems to be not in use any more
#define INI_ToolbarScrollDelay _T("ToolbarDelay")

class CMainFrame;
/////////////////////////////////////////////////////////////////////////////
// CGDSApp:
// See gds.cpp for the implementation of this class
//
class CGDSApp : public CWinApp
{
private:
	IToolBox *m_ToolBox;
	CLogger *m_Logger;
	
public:
	CGDSApp();
	~CGDSApp();
	CRecentFileList *GetRecentFileList();

	// Interface DLL structures in global application class
	STDDLLLIST m_DLLS;
	HANDLELIST m_DLLHandles;

	//---------------------------------------------------------------------
	// Function:	GetFilterStrLength
	// Description:	reads memory to double '\0'-s and returns length
	//				Used, becaus filter strings are delimited by single 
	//				'\0'-s, and ended by double null-s. ("\0\0").
	// Parameters:	filterStr - pointer to the filterstring
	// Returns:		Length of filtersting in the memory
	//---------------------------------------------------------------------
	DWORD GetFilterStrLength(LPCTSTR filterStr);

	//---------------------------------------------------------------------
	// Function:	BuildFilterLists
	// Description:	Collects the different filters, sorts them alphateically
	//				and copies the corresponding string representations
	//				into the filter pointer 
	// Returns:		void
	//---------------------------------------------------------------------
	void BuildFilterLists();

	BOOL OnOpenRecentFile(UINT nID);

	inline IToolBox *GetToolBox()
	{
		return m_ToolBox;
	}

	inline CLogger *GetLogger()
	{
		return m_Logger;
	}

	virtual const CString& GetAppPath();
//	virtual const CString& GetResourcePath();
//	virtual void SetResourcePath(const TCHAR *path);
	virtual const TCHAR * GetResourcePath();
private:
	BOOL LoadFile(LPCTSTR szFileName);

private:
	void LoadDLLS();
	void DeInitDLLS();
    void GetALLInterfaces(const CString& pathName);
    void InitAllDLLS();
    void SearchDLLS(const CString& pathName);

	/// Path to the process's original working dir
	CString m_AppPath;
	/// Path to the resource dir
	StdString m_ResourcePath;
	/// Previous tick count
	DWORD m_iPreviousTickCount;
	/// DLL initialization map
	DLLINITMAP m_DLLInits;

	CSplashDialog m_splashDlg;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGDSApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL OnIdle(LONG lCount);
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CGDSApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFileOpen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern CGDSApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GDS_H__21823C45_287B_4C51_812A_8E8906CC4EB1__INCLUDED_)
