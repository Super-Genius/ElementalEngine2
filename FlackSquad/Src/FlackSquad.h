#pragma once

#include "res/resource.h"

//This section defining INTERFACEDLLVERSION is a PURE HACK until Ken checks-in his IDLL interface changes.
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
typedef void (*DEINITDLL)();

typedef struct DLLINFO
{
	HINSTANCE handle;
	INITDLL initDLL;
	//Just to help debugging
	string file;
} DLLINFO;

typedef map<DWORD, DLLINFO> DLLINFOMAP;

class CGFXApp
{
public:
	/// contructor for application class
	CGFXApp();
	/// destructor for application class
	~CGFXApp();

	/// Parses Command Line Arguments
	void ParseCommandLine( LPTSTR lpCmdLine );

	/// Initializae the instance of the application
	BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);

	/// Exit the instance of the application
	void ExitInstance();

	/// the main loop for the appplication
	void Run();
	
	/// quit the app
	void Quit();

#ifdef WIN32
	/// function to register our window class
	ATOM MyRegisterClass(HINSTANCE hInstance);
	/// static message pump function
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	/// static dialog box function for about box.
	static LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
#endif//WIN32

	/// Intialize the renderer
	void InitRenderer(BOOL fullScreen);

	/// Initialize the Camera
	void InitializeCamera();

	/// the main update function for the system
	void Update();

	// function to get the instance handle for current application
	inline HINSTANCE GetInstanceHandle()
	{
		return m_hInst;
	}
	// this should come from a .ini file
	bool m_EnableLogging;

	/// Gets and resets keyboard input with switching mouse exclusivity
	/// \param value = exclusive mouse (t/f)
	static void SetMouseExclusivity(bool value);
#ifdef _SHELL
	void InitGVRLibrary();
	void ShutdownGVRLibrary();
	void WriteGameData();
	void LogCmdLineParse();
	void InitGVRIO();
	void ShutdownGVRIO();
	bool CheckAndDecrementStartCredits();

	static MessageCallbackFN GVRIO_Callback(sMessage *pMsg);

	void PollGVRIO();
#endif

protected:
	void LoadDLLS();
	void SearchDLLS(const string &pathName);
	void InitDLLS();
	void DeInitDLLS();
	void GetALLInterfaces(const string& pathName);
	void WriteGlobalAttributes();	

	void RegisterZipFiles(const string& pathName);

	/// Interface DLL structures in global application class
	DLLINFOMAP m_DLLInfos;
	/// pointer to the general Elemental Engine Toolbox
	IToolBox *m_ToolBox;
	/// current instance
	HINSTANCE m_hInst;
	/// The title bar text
	TCHAR m_szTitle[MAX_LOADSTRING];
	/// the main window class name
	TCHAR m_szWindowClass[MAX_LOADSTRING];
	/// handle for my accelerators
	HACCEL m_hAccelTable;
	/// the main application window
	HWND m_hWnd;
	/// the context to draw to
	DWORD m_iContextID;
	/// the width of the view port
	DWORD m_iViewWidth;
	/// the height of the view port
	DWORD m_iViewHeight;
	/// Previous tick count
	DWORD m_iPreviousTickCount;
	/// Path to the process's original working dir 
	string m_AppPath;

	
};
