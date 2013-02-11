// unitTestDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "CTestDlgVisitor.h"

class CunitTestDlg;


#define MAX_MSG_SIZE 1024

#define REPORT(x)									\
if ((x))											\
{													\
	m_REditBox.ReplaceSel(_T("Success!\n"));		\
}													\
else												\
{													\
	m_REditBox.ReplaceSel(_T("Failed!\n"));			\
}	

typedef void (CunitTestDlg::*TESTFUNCTION)();

typedef struct TESTINFO
{
	TCHAR *testName;			// the name of the test
	TESTFUNCTION funcPtr;		// the actual pointer to the function
} TESTINFO;

#ifdef _DEBUG
// major version in upper 16 bits
// low word =  minor version, low bit debug =0, release=1
#define INTERFACEDLLVERSION 0x00006100
#else
#define INTERFACEDLLVERSION 0x00006101
#endif

#define MAX_LOADSTRING 100

typedef DWORD (*GETDLLVERSION)();
typedef void (*INITDLL)();
typedef DWORD (*GETPRIORITY)();

typedef struct DLLINFO
{
	HINSTANCE handle;
	INITDLL initDLL;
} DLLINFO;

typedef map<DWORD, DLLINFO> DLLINFOMAP;

// CunitTestDlg dialog
class CunitTestDlg : public CDialog
{
// Construction
public:
	CunitTestDlg(CWnd* pParent = NULL);	// standard constructor
	~CunitTestDlg();

// Dialog Data
	enum { IDD = IDD_UNITTEST_DIALOG };

	bool OnIdle(LONG lCount);


protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support



// Implementation
protected:
	HICON m_hIcon;
	IToolBox *m_ToolBox;
	HINSTANCE m_hWLLib;		// library to load a world file into elemental engine.
	HINSTANCE m_hEELua;		// library to load a world file into elemental engine.
	HINSTANCE m_hLuaNSM;		// library to load a world file into elemental engine.
	HINSTANCE m_hStateMachine;	// library to load a state machine into elemental engine.
	HINSTANCE m_hEntityQuery;	// library to load an entity query into elemental engine.
	HINSTANCE m_hSoundM;	// sound manager dlo
	HINSTANCE m_hDatabase;	// library to load a database into elemental engine.
	HINSTANCE m_hNodeSystem;	// library to load a node system into elemental engine.

	HINSTANCE m_hStateLoader;
	HINSTANCE m_hDatabaseLoader;

	HINSTANCE m_hLanguageLocalizer;
	HINSTANCE m_hLanguageLoader;

	HINSTANCE m_hWin32Network;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	void TestHierarchyManager();
	void TestHierarchyManagerMessage();
	void TestInputManager();
	void TestAll();
	void FillHierarchyTree();
	void TestWorldLoader();
	void TestXMLArchiveIn();
	void TestXMLArchiveOut();
	void TestFilterStrings();
	void DumpHashStrings();
	void TestUpdateFunctions();
	void TestMemArchive();
	void TestRenderManager();
	void TestPhysManager();
	void TestSelectionManager();
	void TestLuaManager();
	void TestXMLArchiveSeek();
	void TestEntityManager();
	void TestSoundManager();
	void TestQHStateMachine();
	void TestEntityQuery();
	void TestDatabase();
	void TestNodeSystem();
	void TestMathLibrary();
	void TestZippedMemoryStream();
	void TestPhysFSStream();
	void TestLanguageLocalizer();
	void TestWin32Network();

private:		
	CRichEditCtrl m_REditBox;
	DWORD m_CurTest;			// current test in dialog
	DWORD m_CurTestAll;			// current test while doing test all
	DWORD m_EndTick;			// ending tick for tracking messages
	DWORD m_RenderEndTick;		// ending tick of rendering messages
	DWORD m_MessageTicks;		// number of ticks between messages
	DWORD m_TickCount;			// number of ticks reported
	DWORD m_CntUpdates;			// number of times message sent

    static TESTINFO tests[];
	CStatic m_StaticTestName;
	CTreeCtrl m_HierarchyTree;
	CTestDlgVisitor m_DlgVisitor;

	OBJECTLIST m_RenderObjList;	// render list

	void LoadDLLS();
	void SearchDLLS(const string &pathName);
	void InitDLLS();
	void GetALLInterfaces(const string& pathName);

	/// Interface DLL structures in global application class
	DLLINFOMAP m_DLLInfos;

	void (CunitTestDlg::*m_CurIdleCallBack)();
	void (CunitTestDlg::*m_EndIdleCallBack)();
	void StartNextTest();
	void testGlobalUpdateTick();
	void testRendererTick();
	void testRendererTickEnd();
	void testInputTickEnd();
	void testMyRenderTick();
	void TestObjectNameChange();
	//void TestIHashStringPtr(IHashStringPtr theHashString);
	//void TestYasper();
	void TestVariableInterface();
	void TestOpenALAudio();
	void TestALInIdle();

	CSoundTest m_SoundTest; // no better place for this...

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedNextTest();
	afx_msg void OnBnClickedPreviousTest();
	afx_msg void OnBnClickedTest();
	afx_msg void OnBnClickedClearButton();

	CButton m_RenderWindowButton;
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedExit();
protected:
	virtual void OnCancel();
};
