// unitTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "unitTest.h"
#include "unitTestDlg.h"

#include "IEEStream.h"
#include "IXMLArchive.h"
#include "IArchive.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

UINT gMyIdleMessage = RegisterWindowMessage(_T("ENTERIDLE"));

TESTINFO CunitTestDlg::tests[] = {
	{ _T("Run All Tests"), &TestAll },
	{ _T("Test the Hierarchy Manager Direct Create"), &TestHierarchyManager },
	{ _T("Test the Hierarchy Manager Message Create"), &TestHierarchyManagerMessage },
	{ _T("Test the Input Manager"), &TestInputManager },
	{ _T("Test World Loader"), &TestWorldLoader },
	{ _T("Test Filter String Retrieval"), &TestFilterStrings },
	{ _T("Test XML Archive Input"), &TestXMLArchiveIn },
	{ _T("Test XML Archive Output"), &TestXMLArchiveOut },
	{ _T("Dump HashString Table"), &DumpHashStrings },
	{ _T("Test Update Functions"), &TestUpdateFunctions },
	{ _T("Test Memory Archive Input/Output"), &TestMemArchive },
	{ _T("Test Render Manager"), &TestRenderManager },
	{ _T("Test Physics Manager"), &TestPhysManager },
	{ _T("Test Selection Manager"), &TestSelectionManager },
	{ _T("Test Lua Manager"), &TestLuaManager },
	{ _T("Test Object Name Change"), &TestObjectNameChange },
//	{ _T("Test Smart Pointers"), &TestYasper },
	{ _T("Test XML Archive Seek"), &TestXMLArchiveSeek },
	{ _T("Test Entity Manager"), &TestEntityManager },
//	{ _T("Test Sound Manager"), &TestSoundManager },
	{ _T("Test State Machine Manager"), &TestQHStateMachine },
	{ _T("Entity Query"), &TestEntityQuery },
	{ _T("Test Open AL Audio"), &TestOpenALAudio },
	{ _T("Test Database"), &TestDatabase },
	{ _T("Test Node System"), &TestNodeSystem },
	{ _T("Test Math Library"), &TestMathLibrary },
	{ _T("Test Zipped Memory Stream"), &TestZippedMemoryStream },
	{ _T("Test PhysFS Stream"), &TestPhysFSStream}, 
	{ _T("Test Language Localizer"), &TestLanguageLocalizer},
	{ _T("Test Win32 Network"), &TestWin32Network}
};

#define NUM_TESTS (sizeof(tests) / sizeof(TESTINFO))

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CunitTestDlg dialog



CunitTestDlg::CunitTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CunitTestDlg::IDD, pParent), 
		m_DlgVisitor(&m_HierarchyTree, &m_REditBox)
{
	m_CurIdleCallBack = NULL;
	m_CurTestAll = 0;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	// see if we can get a pointer to the toolbox
	m_ToolBox			= EngineGetToolBox();
	m_hWLLib			= AfxLoadLibrary(_T("plugins\\LoadSaveManager.dlo"));
	m_hEELua			= AfxLoadLibrary(_T("EELuaScript.dlo"));
	m_hLuaNSM			= AfxLoadLibrary(_T("LuaNSM.dlo"));
	m_hStateMachine		= AfxLoadLibrary(_T("plugins\\QHStateMachine.dlo"));
	m_hEntityQuery		= AfxLoadLibrary(_T("plugins\\EntityQuery.dlo"));
	m_hSoundM			= AfxLoadLibrary(_T("plugins\\OpenAL.dlo"));
	m_hDatabase			= AfxLoadLibrary(_T("plugins\\Database.dlo"));
	m_hNodeSystem		= AfxLoadLibrary(_T("plugins\\NodeSystem.dlo"));

	m_hStateLoader		= AfxLoadLibrary(_T("plugins\\StateLoader.dls"));
	m_hDatabaseLoader	= AfxLoadLibrary(_T("plugins\\DatabaseIO.dls"));

	m_hLanguageLocalizer	= AfxLoadLibrary(_T("plugins\\LanguageLocalizer.dlo"));
	m_hLanguageLoader		= AfxLoadLibrary(_T("plugins\\LanguageLoader.dls"));
	
	m_hWin32Network		= AfxLoadLibrary(_T("plugins\\Win32Network.dlo"));
}

CunitTestDlg::~CunitTestDlg()
{
	AfxFreeLibrary(m_hWLLib);
	AfxFreeLibrary(m_hEELua);
	AfxFreeLibrary(m_hLuaNSM);
	AfxFreeLibrary(m_hStateMachine);
	AfxFreeLibrary(m_hEntityQuery);
	AfxFreeLibrary(m_hSoundM);
	AfxFreeLibrary(m_hDatabase);

	AfxFreeLibrary(m_hStateLoader);
	AfxFreeLibrary(m_hDatabaseLoader);

	AfxFreeLibrary(m_hLanguageLocalizer);
	AfxFreeLibrary(m_hLanguageLoader);

	AfxFreeLibrary(m_hWin32Network);
}

void CunitTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RICHEDIT21, m_REditBox);
	DDX_Control(pDX, IDC_CURRENT_TEST_NAME, m_StaticTestName);
	DDX_Control(pDX, IDC_HIERARCHYTREE, m_HierarchyTree);
	DDX_Control(pDX, IDC_RENDERWINDOW, m_RenderWindowButton);
}

BEGIN_MESSAGE_MAP(CunitTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_NEXT_TEST, OnBnClickedNextTest)
	ON_BN_CLICKED(IDC_PREVIOUS_TEST, OnBnClickedPreviousTest)
	ON_BN_CLICKED(IDC_TEST, OnBnClickedTest)
	ON_BN_CLICKED(IDC_CLEAR_BUTTON, OnBnClickedClearButton)
	ON_WM_KEYDOWN()
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_EXIT, OnBnClickedExit)
END_MESSAGE_MAP()


// CunitTestDlg message handlers

BOOL CunitTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	assert((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	assert(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_CurTest = 0;
	m_StaticTestName.SetWindowText(tests[m_CurTest].testName);


	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CunitTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CunitTestDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CunitTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//
// test the hierarchy manager
//
void CunitTestDlg::TestHierarchyManager()
{
	TCHAR tempMsg[MAX_MSG_SIZE];
	DWORD tMsgID;
	IObject *aiObject1;
	IObject *aiObject2;
	IObject *aiObject3;
	IComponent *entityComponent;
	IObject *entityObject;

	// these have to persist because objects persist
	CHashString ent1Name(_T("Entity1"));
	CHashString ai1Name(_T("AIObject1"));
	CHashString ai2Name(_T("AIObject2"));

	m_REditBox.ReplaceSel(_T("Testing Creation of AI Manager - "));

	IManager *AIManager = dynamic_cast<IManager *>(
		m_ToolBox->CreateComponent(&CHashString(_T("CAIManager")), 0));
	REPORT(AIManager != NULL);

	m_REditBox.ReplaceSel(_T("Testing creation of component CEntity - "));
	entityComponent = m_ToolBox->CreateComponent(&CHashString(_T("CEntity")), 0);
	REPORT(entityComponent != NULL);

	entityObject = dynamic_cast<IObject *>(entityComponent);
	assert(entityObject);
	entityObject->SetParentName(NULL);
	// this could be done through the archive
	entityObject->SetName(&ent1Name);
	entityObject->Init();

	IComponent *aiComponent;
	m_REditBox.ReplaceSel(_T("Testing creation of component CAIObject1 - "));
	aiComponent = m_ToolBox->CreateComponent(&CHashString(_T("CAIObject")), 0);
	REPORT(aiComponent != NULL);

	aiObject1 = dynamic_cast<IObject *>(aiComponent);
	assert(aiObject1);
	aiObject1->SetParentName(&ent1Name);
	// this could be done through the archive
	aiObject1->SetName(&ai1Name);
	aiObject1->Init();

	m_REditBox.ReplaceSel(_T("Testing creation of component CAIObject2 - "));
	aiComponent = m_ToolBox->CreateComponent(&CHashString(_T("CAIObject")), 0);
	REPORT(aiComponent != NULL);

	aiObject2 = dynamic_cast<IObject *>(aiComponent);
	assert(aiObject2);
	aiObject2->SetParentName(&ent1Name);
	// this could be done through the archive
	aiObject2->SetName(&ai2Name);
	aiObject2->Init();

	m_REditBox.ReplaceSel(_T("Testing creation of component CAIObject3 - "));
	aiComponent = m_ToolBox->CreateComponent(&CHashString(_T("CAIObject")), 0);
	REPORT(aiComponent != NULL);

	aiObject3 = dynamic_cast<IObject *>(aiComponent);
	assert(aiObject3);
	aiObject3->SetParentName(&CHashString(_T("Entity1")));
	// this could be done through the archive
	aiObject3->SetName(&CHashString(_T("AIObject3")));
	aiObject3->Init();

	tMsgID = m_ToolBox->GetMessageID(&CHashString(_T("TestMessage")));
	// now a small hierarchy is built.
	sprintf(tempMsg, _T("Sending message To Entity containing AIObject TestMessage returned: "));

	// first send message to one object's parent entity
	m_ToolBox->SendMessage(tMsgID, (DWORD)(MAX_MSG_SIZE-_tcslen(tempMsg)-1), 
		&tempMsg[_tcslen(tempMsg)], &CHashString(_T("Entity1")), 
			&CHashString(_T("CAIObject")));

	m_REditBox.ReplaceSel(tempMsg);
	m_REditBox.ReplaceSel(_T("\n"));

	tMsgID = m_ToolBox->GetMessageID(&CHashString(_T("TestMessage2")));
	sprintf(tempMsg, _T("Sending message to all AIObjects TestMessage2 returned: "));

	// now send message to all objects
	m_ToolBox->SendMessage(tMsgID, (DWORD)(MAX_MSG_SIZE-_tcslen(tempMsg)-1), 
		&tempMsg[_tcslen(tempMsg)], NULL, &CHashString(_T("CAIObject")));

	m_REditBox.ReplaceSel(tempMsg);
	m_REditBox.ReplaceSel(_T("\n"));

	sprintf(tempMsg, _T("Sending message to very specific AIObject (AIObject1) TestMessage2 returned: "));

	// now send message to very specific object, with message as Text
	static DWORD msgHash_TestMessage2 = CHashString(_T("TestMessage2")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_TestMessage2, (DWORD)(MAX_MSG_SIZE-_tcslen(tempMsg)-1), 
		&tempMsg[_tcslen(tempMsg)], &CHashString(_T("AIObject1")), &CHashString(_T("CAIObject")));

	m_REditBox.ReplaceSel(tempMsg);
	m_REditBox.ReplaceSel(_T("\n"));

	FillHierarchyTree();

	// delete all the objects
	aiObject3->DeInit();
	aiObject2->DeInit();
	aiObject1->DeInit();

	entityObject->DeInit();
}

void CunitTestDlg::TestHierarchyManagerMessage()
{
	CREATEOBJECTPARAMS cop;
	INITOBJECTPARAMS iop;
	DELETEOBJECTPARAMS dop;

	CHashString szEntity1(_T("Entity1"));
	CHashString szAIObject1(_T("AIObject1"));
	CHashString szAIObject2(_T("AIObject2"));
	CHashString szCEntity(_T("CEntity"));
	CHashString szCAIObject(_T("CAIObject"));

	m_REditBox.ReplaceSel(_T("Testing Creation of Entity 1 - \n"));

	// now let's try and create the objects through messages, which should be
	// the way the game does it.
	cop.name = &szEntity1;
	cop.parentName = NULL;
	cop.typeName = &szCEntity;
	static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop, NULL, NULL);
	iop.name = cop.name;
	static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop, NULL, NULL);

	m_REditBox.ReplaceSel(_T("Testing Creation of AI Object4 - \n"));

	// now let's try and create the objects through messages, which should be
	// the way the game does it.
	cop.name = &szAIObject1;
	cop.parentName = &szEntity1;
	cop.typeName = &szCAIObject;
	m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop, NULL, NULL);
	iop.name = cop.name;
	m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop, NULL, NULL);

	// now let's try and create the objects through messages, which should be
	// the way the game does it.
	cop.name = &szAIObject2;
	cop.parentName = &szEntity1;
	cop.typeName = &szCAIObject;
	m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop, NULL, NULL);
	iop.name = cop.name;
	m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop, NULL, NULL);

	FillHierarchyTree();

	// try out of order deleting
	dop.name = &szAIObject1;
	static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof(DELETEOBJECTPARAMS), &dop, NULL, NULL);

	FillHierarchyTree();

	dop.name = &szAIObject2;
	m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof(DELETEOBJECTPARAMS), &dop, NULL, NULL);


	dop.name = &szEntity1;
	m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof(DELETEOBJECTPARAMS), &dop, NULL, NULL);
}


void CunitTestDlg::TestInputManager()
{
	DWORD tMsgID;
	std::string str;

	/////////////////////////////////////////////////////////////////////////////////
	// TESTING INPUT MANAGER
	m_REditBox.ReplaceSel(_T("Testing Creation of Input Manager - "));
	IManager *mngr = dynamic_cast<IManager *>(
		m_ToolBox->CreateComponent(&CHashString(_T("CInputManager")), 0));
	REPORT(mngr != NULL);

	tMsgID = m_ToolBox->GetMessageID(&CHashString(_T("SetKeyBinding")));

	// temporary key binding
	KEYBIND keybindtest;
	CHashString name(_T("testing"));
	keybindtest.m_kbNameID = name.GetUniqueID();

	// fill in with some sample data
	keybindtest.m_KeyID = 0x00c8;

	// weird bug here!
	CHashString msg(_T("GO_FORWARD"));
	keybindtest.m_MessageID = msg.GetUniqueID();

	// send message to InputManager
	static DWORD msgHash_SetKeyBinding = CHashString(_T("SetKeyBinding")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetKeyBinding, sizeof(KEYBIND), &keybindtest);
	char tmpBuf[255];

	m_REditBox.ReplaceSel(_T("Message Sent: '"));
	str += (char)keybindtest.m_KeyID;
	m_REditBox.ReplaceSel(str.c_str());
	m_REditBox.ReplaceSel(_T("' Mapped to - "));
	StdString printString;

	_itot((int)keybindtest.m_MessageID, tmpBuf, 10);
	printString = tmpBuf;
	m_REditBox.ReplaceSel(printString);//m_REditBox.ReplaceSel(keybindtest.m_Message->GetString());
	m_REditBox.ReplaceSel(_T("\n"));

	tMsgID = m_ToolBox->GetMessageID(&CHashString(_T("GetKeyBinding")));

	// temporary structure to hold information 
	KEYBIND keybindtest2;
	keybindtest2.m_KeyID = 0x00c8;
	keybindtest2.m_MessageID = CHashString(_T("")).GetUniqueID();

	// send message to InputManager
	static DWORD msgHash_GetKeyBinding = CHashString(_T("GetKeyBinding")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetKeyBinding, sizeof(KEYBIND), &keybindtest2);

	// display results
	m_REditBox.ReplaceSel(_T("Getting KEYBINDING message for '"));
	str.erase();
	str += (char)keybindtest2.m_KeyID;
	m_REditBox.ReplaceSel(str.c_str());
	m_REditBox.ReplaceSel(_T("'...RETURNED: "));
	if (keybindtest2.m_MessageID != CHashString(_T("")).GetUniqueID())
	{
		StdString printString;
		char tmpBuf[255];
		_itot((int)keybindtest2.m_MessageID, tmpBuf, 10);
		printString = tmpBuf;
		m_REditBox.ReplaceSel(printString);
	}
	m_REditBox.ReplaceSel(_T("\n"));

	// Start InputManager
	INPUTENGPARAMS inputParams;

	HMODULE mod = GetModuleHandle(NULL);
	inputParams.hMod = mod;
	inputParams.hWnd = m_hWnd;
	inputParams.mouseImmediate = true;
	inputParams.mouseExclusive = false;
	inputParams.mouseFG = true;
	inputParams.kbImmediate = true;
	inputParams.kbExclusive = true;
	inputParams.kbFG = true;

	m_REditBox.ReplaceSel(_T("Starting Input...\n"));
	static DWORD msgHash_Start = CHashString(_T("Start")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_Start, sizeof(INPUTENGPARAMS), &inputParams, NULL, &CHashString(_T("CInputManager")));

	// wait 1/2 second to display messages()
	m_MessageTicks = 500;
	m_EndTick = GetTickCount() + m_MessageTicks;
	m_TickCount = 0;
	m_CntUpdates = 0;
	// setup OnIdleCallBack
	m_CurIdleCallBack = &CunitTestDlg::testGlobalUpdateTick;
	m_EndIdleCallBack = &CunitTestDlg::testInputTickEnd;
	
	// END INPUT MANAGER TEST
	///////////////////////////////////////////////////////////////////////////////////
}

void CunitTestDlg::testInputTickEnd()
{
	m_REditBox.ReplaceSel(_T("Stopping Input...\n"));
	int nothing;
	static DWORD msgHash_Stop = CHashString(_T("Stop")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_Stop, sizeof(int), &nothing, NULL, &CHashString(_T("CInputManager")));
}

void CunitTestDlg::TestAll()
{
	m_CurTestAll = 1;
	StartNextTest();
}

void CunitTestDlg::StartNextTest()
{
	while (m_CurTestAll < NUM_TESTS)
	{
		(this->*(tests[m_CurTestAll].funcPtr))();
		// test still in progress?, then exit
		if (m_CurIdleCallBack != NULL)
		{
			break;
		}
		m_CurTestAll++;
	}

	// have we done all tests?
	if (m_CurTestAll > NUM_TESTS)
		m_CurTestAll = 0;

}

void CunitTestDlg::OnBnClickedNextTest()
{
	m_CurTest = ((m_CurTest + 1) % NUM_TESTS);
	m_StaticTestName.SetWindowText(tests[m_CurTest].testName);
}

void CunitTestDlg::OnBnClickedPreviousTest()
{
	m_CurTest = (m_CurTest > 0) ? (m_CurTest - 1) : (NUM_TESTS-1);
	m_StaticTestName.SetWindowText(tests[m_CurTest].testName);
}


void CunitTestDlg::OnBnClickedTest()
{
	m_CurIdleCallBack = NULL;
	(this->*(tests[m_CurTest].funcPtr))();
}

void CunitTestDlg::OnBnClickedClearButton()
{
	m_REditBox.SetSel(0, -1);
	m_REditBox.ReplaceSel(_T(""));
}

// fill in the hierarchy tree
void CunitTestDlg::FillHierarchyTree()
{
	VISITHIERARCHYDATA vhd;

	vhd.name = NULL;
	vhd.visitor = &m_DlgVisitor;

	m_DlgVisitor.Reset();

	// ??? This fixes the problem with DeleteAllItems making
	// tree not show up.  I don't know why, ask Microsoft.
	SetWindowRedraw(m_HierarchyTree, FALSE);

	// bug in Microsoft MFC, that won't let this work?
	m_HierarchyTree.DeleteAllItems();
	m_HierarchyTree.SetFocus();

	// now let's try and create the objects through messages, which should be
	// the way the game does it.
	static DWORD msgHash_VisitHierarchy = CHashString(_T("VisitHierarchy")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_VisitHierarchy, sizeof(VISITHIERARCHYDATA), &vhd,
		NULL, NULL);

	SetWindowRedraw(m_HierarchyTree, TRUE);

}

void CunitTestDlg::TestWorldLoader()
{
	LOADFILEEXTPARAMS lfep;

	lfep.fileName = _T("testHierarchy.wxl");
	lfep.bInternalLoad = true;
	// now let's try and create the objects through messages, which should be
	// the way the game does it.
	static DWORD msgHash_LoadFileByExtension = CHashString(_T("LoadFileByExtension")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_LoadFileByExtension, sizeof(LOADFILEEXTPARAMS), &lfep);
	FillHierarchyTree();
}

void CunitTestDlg::TestXMLArchiveIn()
{
	IXMLArchive *ar;
	CREATEARCHIVE ca;

	ca.streamData = _T("C:/Development/ElementalEngine/Data/Complex2.xml");
	ca.mode = STREAM_MODE_READ;

	//ar.Init( "c:/xml/testTypes.xml" );
	static DWORD msgHash_CreateXMLArchive = CHashString(_T("CreateXMLArchive")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_CreateXMLArchive, sizeof(CREATEARCHIVE),
		&ca) != MSG_HANDLED)
	{
		m_REditBox.ReplaceSel(_T("Message to Create XML Archive failed!"));
		return;
	}

	ar = dynamic_cast<IXMLArchive *>(ca.archive);
//	int result;

	float mass;
	float friction;
	int x, y, z;
//	bool castShadows;
//	bool isStatic;
	StdString shader_name;
	StdString texture_name;
	StdString model_name;

	StdString World, World2;
	StdString header2, header4;
	StdString header3, header5;

	
	StdString PhysObj, PhysObj2;
	StdString PhysMat;
	StdString Vec3;
	StdString RenderObj;
	StdString Shader;
	StdString Texture;
	StdString Model;
	
/* NOTES
	//ar.Read( World );
	//bool ar.GetNode( &string );
	string Parent = NULL;
	while( ar.GetNode( &string ) )
	{
		IObject* CurComponent = m_Toolbox->CreateComponenet(string);

		if( CurComponent != NULL )
		{
			CurComponent->SetName( Parent );
			CurComponent->Init();
			CurComponent->Serialize( ar ); <--------- Calls the Read Functions
		}
	}
*/

	//DWORD testNodeCount = ar->GetNumberOfNodes();

	ar->SeekTo(0, SEEK_END);

#define USE_GET_NODE 1
#define USE_GET_PARENT 1

#if USE_GET_NODE
	ar->GetNode( World );
#endif
	ar->Read( header2 );
	ar->Read( header3 );
	
#if USE_GET_NODE
	ar->GetNode( PhysObj );
#endif

#if USE_GET_PARENT
	ar->GotoParentNode( World2 );
	#if USE_GET_NODE
	assert( World == World2 );
	#endif
	ar->Read( header4 );
	assert( header2 == header4 );
	//ar->Read( header5 );
	//assert( header3 == header5 );
	ar->ReturnToChildNode( PhysObj2 );
	#if USE_GET_NODE
    assert( PhysObj == PhysObj2 );
	#endif
#endif
	ar->Read( mass );

#if USE_GET_NODE
	ar->GetNode( PhysMat );
#endif
	ar->Read( friction );

#if USE_GET_NODE
	ar->GetNode( Vec3 );
#endif
	ar->Read( x ); 
	ar->Read( y ); 
	ar->Read( z );

#if USE_GET_NODE
	ar->GetNode( RenderObj );
#endif

#if USE_GET_NODE
	ar->GetNode( Shader );
#endif
	ar->Read( shader_name );

#if USE_GET_NODE
	ar->GetNode( Texture);
#endif
	ar->Read( texture_name );

#if USE_GET_NODE
	ar->GetNode( Model );
#endif
	ar->Read( model_name );

	while( ar->GetNode(Vec3) && (_tcsicmp( Vec3.c_str(), _T("Vec3") ) == 0) )
	{
		ar->Read( x ); 
		ar->Read( y ); 
		ar->Read( z ); 
	}

	ar->Close();
}

void CunitTestDlg::TestXMLArchiveSeek()
{
	IXMLArchive *ar;
	CREATEARCHIVE ca;

	ca.streamData = _T("C:/Development/ElementalEngine/Data/Complex2.xml");
	ca.mode = STREAM_MODE_READ;

	//ar.Init( "c:/xml/testTypes.xml" );
	static DWORD msgHash_CreateXMLArchive = CHashString(_T("CreateXMLArchive")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_CreateXMLArchive, sizeof(CREATEARCHIVE),
		&ca) != MSG_HANDLED)
	{
		m_REditBox.ReplaceSel(_T("Message to Create XML Archive failed!"));
		return;
	}

	ar = dynamic_cast<IXMLArchive *>(ca.archive);

	
	StdString name;

	m_REditBox.ReplaceSel(_T("Are you ready for some SEEKING????\n"));
	m_REditBox.ReplaceSel(_T("Currently we are at the start of Complex2.xml\n"));
	m_REditBox.ReplaceSel(_T("Node name is: "));

	ar->GetNode( name );
	m_REditBox.ReplaceSel(name);
	m_REditBox.ReplaceSel(_T("\n"));

	m_REditBox.ReplaceSel(_T("Let's jump to the end!\n"));
	ar->SeekTo(0, SEEK_END);

	m_REditBox.ReplaceSel(_T("Whew...looks like we grabbed a: "));
	ar->GetNode( name );
	m_REditBox.ReplaceSel(name);
	m_REditBox.ReplaceSel(_T("\n"));
	
	m_REditBox.ReplaceSel(_T("Back to the beginning for: "));
	ar->SeekTo(0, SEEK_SET);
	ar->GetNode( name );
	m_REditBox.ReplaceSel(name);
	m_REditBox.ReplaceSel(_T("\n"));

	m_REditBox.ReplaceSel(_T("Ooooo let's try somewhere in the middle (2 from the current to be exact)!\n"));
	ar->SeekTo(2, SEEK_CUR);

	m_REditBox.ReplaceSel(_T("Node name is: "));
	ar->GetNode( name );
	m_REditBox.ReplaceSel(name);
	m_REditBox.ReplaceSel(_T("\n"));

	m_REditBox.ReplaceSel(_T("How about in reverse???\n"));
	ar->SeekTo(-1, SEEK_CUR);

	m_REditBox.ReplaceSel(_T("Node name is: "));
	ar->GetNode( name );
	m_REditBox.ReplaceSel(name);
	m_REditBox.ReplaceSel(_T("\n"));

	m_REditBox.ReplaceSel(_T("What a ride, thank you SeekTo!"));

	ar->Close();
}

void CunitTestDlg::TestXMLArchiveOut()
{
	
	IXMLArchive *ar;
	CREATEARCHIVE ca;
	ca.streamData = _T("C:/Development/ElementalEngine/Data/Out.xml");
	ca.mode = STREAM_MODE_WRITE;

	static DWORD msgHash_CreateXMLArchive = CHashString(_T("CreateXMLArchive")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_CreateXMLArchive, sizeof(CREATEARCHIVE),
		&ca) != MSG_HANDLED)
	{
		m_REditBox.ReplaceSel(_T("Message to Create XML Archive failed!"));
		return;
	}

	ar = dynamic_cast<IXMLArchive *>(ca.archive);

	ar->StartClass( _T("World") );
    	ar->StartClass( _T("PhysObj") );
			ar->Write( 100.0f, _T("mass") );
			ar->StartClass( _T("Vec3") );
				ar->Write( 1.0f, _T("x") );
				ar->Write( 1.0f, _T("y") );
				ar->Write( 1.0f, _T("z") );
			ar->EndClass();
		ar->EndClass();

		ar->StartClass( _T("RenderObj") );
			ar->StartClass( _T("Shader") );
				ar->Write( _T("blah"), _T("file") );
			ar->EndClass();
			ar->StartClass( _T("Texture") );
				ar->Write( _T("foo"), _T("file") );
			ar->EndClass();
			ar->StartClass( _T("Model") );
				ar->Write( _T("bar"), _T("file") );
				ar->StartClass( _T("Vec3") );
					ar->Write( 1.0f, _T("x") );
					ar->Write( 2.0f, _T("y") );
					ar->Write( 3.0f, _T("z") );
				ar->EndClass();
				ar->StartClass( _T("Vec3") );
					ar->Write( 4.0f, _T("x") );
					ar->Write( 5.0f, _T("y") );
					ar->Write( 6.0f, _T("z") );
				ar->EndClass();
				ar->StartClass( _T("Vec3") );
					ar->Write( 7.0f, _T("x") );
					ar->Write( 8.0f, _T("y") );
					ar->Write( 9.0f, _T("z") );
				ar->EndClass();
				ar->StartClass( _T("Vec3") );
					ar->Write( 10.0f, _T("x") );
					ar->Write( 11.0f, _T("y") );
					ar->Write( 12.0f, _T("z") );
				ar->EndClass();
				ar->StartClass( _T("Vec3") );
					ar->Write( 13.0f, _T("x") );
					ar->Write( 14.0f, _T("y") );
					ar->Write( 15.0f, _T("z") );
				ar->EndClass();
			ar->EndClass();
		ar->EndClass();
	ar->EndClass();

	ar->Close();
}

void CunitTestDlg::TestFilterStrings()
{
	FILTERSTRINGSLIST *fsl;
	FILTERSTRINGSLIST::iterator fslIter;
	FILTERSTRINGS *filterStrings;
	DWORD retVal;

	m_REditBox.SetSel(0, -1);
	m_REditBox.ReplaceSel(_T(""));
	static DWORD msgHash_GetFilterStrings = CHashString(_T("GetFilterStrings")).GetUniqueID();
	retVal = m_ToolBox->SendMessage(msgHash_GetFilterStrings, sizeof(FILTERSTRINGSLIST **),
		&fsl, NULL, NULL);

	if (retVal != MSG_HANDLED)
	{
		m_REditBox.ReplaceSel(_T("Message to get filter String failed!"));
		return;
	}

	for (fslIter = fsl->begin(); fslIter != fsl->end(); fslIter++)
	{
		filterStrings = *fslIter;
		m_REditBox.ReplaceSel(_T("Load Filter: "));
		m_REditBox.ReplaceSel(filterStrings->m_LoadFilter);
		m_REditBox.ReplaceSel(_T("\n"));
		m_REditBox.ReplaceSel(_T("Save Filter: "));
		m_REditBox.ReplaceSel(filterStrings->m_SaveFilter);
		m_REditBox.ReplaceSel(_T("\n"));
	}
}

void CunitTestDlg::DumpHashStrings()
{
	HASHSTRINGMAP *hsm;
	HASHSTRINGMAP::iterator hsmIter;
	const TCHAR *ihs;

	m_REditBox.SetSel(0, -1);
	m_REditBox.ReplaceSel(_T(""));
	hsm = m_ToolBox->GetHashStringMap();

	for (hsmIter = hsm->begin(); hsmIter != hsm->end(); hsmIter++)
	{
		ihs = hsmIter->first;
		m_REditBox.ReplaceSel(ihs);
		m_REditBox.ReplaceSel(_T("\n"));
	}
}

void CunitTestDlg::TestUpdateFunctions()
{
	int i;
	m_REditBox.SetSel(0, -1);
	m_REditBox.ReplaceSel(_T(""));
	TCHAR *components[] = {
	_T("CHierarchyManager"),
	_T("CAIManager"),
	_T("CPhysicsManager"),
	_T("CSoundManager"),
	_T("CGuiManager"),
	_T("CRenderManager"),
	_T("CInputManager"),
	_T("CSelectionManager")
	};

#define NUM_COMPONENT_INIT (sizeof(components) / sizeof(components[0]))

	IComponent *compInit[NUM_COMPONENT_INIT];


	for (i=0; i<NUM_COMPONENT_INIT; i++)
	{
		// need to instantiate some objects so that they register their listeners
		compInit[i] = m_ToolBox->CreateComponent(&CHashString(components[i]), 0);
	}

	// wait 1/2 second to display messages()
	m_MessageTicks = 500;
	m_EndTick = GetTickCount() + m_MessageTicks;
	m_TickCount = 0;
	m_CntUpdates = 0;
	// setup OnIdleCallBack
	m_CurIdleCallBack = &CunitTestDlg::testGlobalUpdateTick;
	m_EndIdleCallBack = NULL;

	for (i=0; i<NUM_COMPONENT_INIT; i++)
	{
		// need to instantiate some objects so that they register their listeners
//		delete compInit[i];
	}

}

void CunitTestDlg::testGlobalUpdateTick()
{
	string tStr;
	TCHAR tempNum[100];

	if (m_EndTick > GetTickCount())
	{
		// wait m_MessageTicks second to display messge
		m_EndTick = GetTickCount() + m_MessageTicks;
		_ltot(m_CntUpdates, tempNum, 10);
		tStr = tempNum;
		tStr += _T(" updates Send Message Updates have occured at tick ");
		_ltot(m_TickCount, tempNum, 10);
		tStr +=  tempNum;
		tStr += _T("\n");
		m_REditBox.ReplaceSel(tStr.c_str());
		m_TickCount++;
	}

	GLOBALTICKPARAMS gtp;
	gtp.tickCount = 0;

	static DWORD msgHash_GlobalUpdateTick = CHashString(_T("GlobalUpdateTick")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GlobalUpdateTick, sizeof(GLOBALTICKPARAMS), &gtp, NULL, NULL);
	m_CntUpdates++;

}

void CunitTestDlg::TestMemArchive()
{
	IArchive *MemArchive;	
	CHashString memType(_T("Memory"));

	// CHUNK-o-Mem (1 Meg)
	char* chunkomem = new char[1024*1024];
	memset( chunkomem, 0, sizeof(char)*1024*1024 );

	float testFloat = 4.56f;
	double testDouble = 3.789f;
	int testInt = 3;
	short testShort = 7;
	char testChar = 'P';
	unsigned testUnsigned = 42;
	bool testBool = true;
	TCHAR *testwchar = _T("HI");
	StdString testWstring = _T("HEHE HOHO");
	StdString testWstringA = _T("yadayadayada");
	float testFloatLast = 13.45f;

	CREATEARCHIVE ca;

	ca.mode = STREAM_MODE_WRITE | STREAM_MODE_READ;
	ca.streamData = chunkomem;
	ca.streamSize = sizeof(char)*1024*1024;
	ca.streamType = &memType;
	static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE),
		&ca) != MSG_HANDLED)
	{
		m_REditBox.ReplaceSel(_T("Message to Create archive failed!"));
		return;
	}

	MemArchive = ca.archive;

	MemArchive->Write( testFloat );
	MemArchive->Write( testDouble );
	MemArchive->Write( testInt );
	MemArchive->Write( testShort );
	MemArchive->Write( testChar );
	MemArchive->Write( testUnsigned );
	MemArchive->Write( testBool );
	//MemArchive->Write( testwchar );
	MemArchive->Write( testWstring ); //error begins here...doesn't write null terminating string
	MemArchive->Write( testWstringA ); 
	MemArchive->Write( testFloatLast );

	float resultFloat = 0.00f;
	double resultDouble = 0.000f;
	int resultInt = 0;
	short resultShort = 0;
	char resultChar = 0;
	unsigned resultUnsigned = 0;
	bool resultBool = false;
	StdString resultWstring = _T("hhjhj");
	StdString resultWstringA = _T("");
	float resultFloat2 = 0.00f;

	MemArchive->Read( resultFloat );
	MemArchive->Read( resultDouble );
	MemArchive->Read( resultInt );
	MemArchive->Read( resultShort );
	MemArchive->Read( resultChar );
	MemArchive->Read( resultUnsigned );
	MemArchive->Read( resultBool );
	MemArchive->Read( resultWstring ); //so on read..
	MemArchive->Read( resultWstringA ); //this string gets attached to the string above
	MemArchive->Read( resultFloat2 ); //as well as these values...


	MemArchive->Close();

	delete [] chunkomem;
	chunkomem = NULL;
}

/*
LRESULT CALLBACK WndProc(	HWND	hWnd,			// Handle For This Window
							UINT	uMsg,			// Message For This Window
							WPARAM	wParam,			// Additional Message Information
							LPARAM	lParam)			// Additional Message Information
{
	switch (uMsg)									// Check For Windows Messages
	{
		case WM_ACTIVATE:							// Watch For Window Activate Message
		{
			return 0;								// Return To The Message Loop
		}
		case WM_CLOSE:								// Did We Receive A Close Message?
		{
			PostQuitMessage(0);						// Send A Quit Message
			return 0;								// Jump Back
		}
		case WM_SIZE:								// Resize The OpenGL Window
		{
			return 0;								// Jump Back
		}
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}
*/

void CunitTestDlg::TestRenderManager()
{
	int i;
//	DWORD iContextIndex;
	CREATERENDERCONTEXTPARAMS createContextMessage;
	m_REditBox.SetSel(0, -1);
	m_REditBox.ReplaceSel(_T(""));
	string tStr;
	TCHAR *components[] = {
	_T("CHierarchyManager"),
	_T("CAIManager"),
	_T("CPhysManager"),
	_T("CSoundManager"),
	_T("CGuiManager"),
	_T("CRenderManager"),
	_T("CInputManager"),
	_T("CSelectionManager")
	};
	//Create a new window
	/*{
		WNDCLASS	wc;						// Windows Class Structure
		DWORD		dwExStyle;				// Window Extended Style
		DWORD		dwStyle;				// Window Style
		RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
		WindowRect.left=(long)0;			// Set Left Value To 0
		WindowRect.right=(long)800;		// Set Right Value To Requested Width
		WindowRect.top=(long)0;				// Set Top Value To 0
		WindowRect.bottom=(long)600;		// Set Bottom Value To Requested Height

		HINSTANCE hInstance	= GetModuleHandle(NULL);				// Grab An Instance For Our Window
		wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
		wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
		wc.cbClsExtra		= 0;									// No Extra Window Data
		wc.cbWndExtra		= 0;									// No Extra Window Data
		wc.hInstance		= hInstance;							// Set The Instance
		wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
		wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
		wc.hbrBackground	= NULL;									// No Background Required For GL
		wc.lpszMenuName		= NULL;									// We Don't Want A Menu
		wc.lpszClassName	= "OpenGL";								// Set The Class Name

		if (!RegisterClass(&wc))									// Attempt To Register The Window Class
		{
			::MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
			return;											// Return FALSE
		}
		
		{
			dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
			dwStyle=WS_OVERLAPPEDWINDOW;							// Windows Style
		}

		AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

		// Create The Window
		if (!(hWnd=CreateWindowEx(	dwExStyle,							// Extended Style For The Window
									"OpenGL",							// Class Name
									"Renderer Test",								// Window Title
									dwStyle |							// Defined Window Style
									WS_CLIPSIBLINGS |					// Required Window Style
									WS_CLIPCHILDREN,					// Required Window Style
									150, 150,								// Window Position
									WindowRect.right-WindowRect.left,	// Calculate Window Width
									WindowRect.bottom-WindowRect.top,	// Calculate Window Height
									NULL,								// No Parent Window
									NULL,								// No Menu
									hInstance,							// Instance
									NULL)))								// Dont Pass Anything To WM_CREATE
		{
			UnregisterClass("OpenGL",hInstance);
			::MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
			return;
		}

		::ShowWindow(hWnd,SW_SHOW);						// Show The Window
		::SetForegroundWindow(hWnd);						// Slightly Higher Priority
		::SetFocus(hWnd);
	}*/

	//Create a render context using the window(right now we just use the default hwnd, it will
	//draw the background color black
	{
		// made a button, so now render into it.
		createContextMessage.hwindow = m_RenderWindowButton.GetSafeHwnd();
		// get the window size
		RECT winRect;
		m_RenderWindowButton.GetClientRect(&winRect);
		createContextMessage.width = (winRect.right - winRect.left);
		createContextMessage.height = (winRect.bottom - winRect.top);
		createContextMessage.depthbits = 24;
		createContextMessage.colorbits = 32;
		//we save off the context Index in iContextIndex
//		createContextMessage.retVal_RenderID = &iContextIndex;
		//Not creating a context right now because we want to just use the main window
		static DWORD msgHash_CreateRenderContext = CHashString(_T("CreateRenderContext")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_CreateRenderContext, sizeof(createContextMessage ),
			&createContextMessage, NULL, NULL );
	}
	

#define NUM_COMPONENT_INIT (sizeof(components) / sizeof(components[0]))

	IComponent *compInit[NUM_COMPONENT_INIT];

	//Create some new window to render screen to
	for (i=0; i<NUM_COMPONENT_INIT; i++)
	{
		// need to instantiate some objects so that they register their listeners
		compInit[i] = m_ToolBox->CreateComponent(&CHashString(components[i]), 0);
	}

	// wait 2 seconds to display messages()
	m_MessageTicks = 2000;
	m_EndTick = GetTickCount() + m_MessageTicks;
	m_TickCount = 0;
	m_CntUpdates = 0;
	// wait 1/60th of a second to update
	m_RenderEndTick = GetTickCount() + 1000/60;

	//create a test class for rendering
	IComponent * cro = m_ToolBox->CreateComponent(&CHashString(_T("CV3ORenderObject")), NULL, NULL );
	m_RenderObjList.push_back( (IObject*) cro );

	// setup OnIdleCallBack
	m_CurIdleCallBack = &CunitTestDlg::testRendererTick;
	m_EndIdleCallBack = &CunitTestDlg::testRendererTickEnd;
			
	for (i=0; i<NUM_COMPONENT_INIT; i++)
	{
		// need to instantiate some objects so that they register their listeners
//		delete compInit[i];
	}

}

void CunitTestDlg::testRendererTick()
{
	ADDRENDERLISTPARAMS renderMessage;		
	string tStr;
	TCHAR tempNum[100];

	if (m_EndTick > GetTickCount())
	{
		// wait m_MessageTicks second to display messge
		m_EndTick = GetTickCount() + m_MessageTicks;
		_ltot(m_CntUpdates, tempNum, 10);
		tStr = tempNum;
		tStr += _T(" Render Updates have occured at tick ");
		_ltot(m_TickCount, tempNum, 10);
		tStr +=  tempNum;
		tStr += _T("\n");
		m_REditBox.ReplaceSel(tStr.c_str());
		m_TickCount++;
	}

	if (m_RenderEndTick > GetTickCount())
	{
		// wait 1/60th of a second to update
		m_RenderEndTick = GetTickCount() + (1000/60);
		//create a message to render per frame
		renderMessage.objList = &m_RenderObjList;
		renderMessage.dwSortMethod = SORT_FIFO;
		renderMessage.dwListPriorityKey = RENDERPRIORITY_OPAQUE;

		//change the current context to render to
		static DWORD msgHash_DrawToRenderContext = CHashString(_T("DrawToRenderContext")).GetUniqueID();
		//m_ToolBox->SendMessage(msgHash_DrawToRenderContext, sizeof(UINT ), &iContextIndex, NULL, NULL );
		//pass on the list to add to the render manager using the above sort method(will change method
		//of passing this on where sort is in this message
		static DWORD msgHash_AddRenderList = CHashString(_T("AddRenderList")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_AddRenderList, sizeof(renderMessage), &renderMessage, NULL, NULL );

		GLOBALTICKPARAMS gtp;
		gtp.tickCount = 0;
		static DWORD msgHash_GlobalUpdateTick = CHashString(_T("GlobalUpdateTick")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GlobalUpdateTick, sizeof(GLOBALTICKPARAMS), &gtp, NULL, NULL);
		m_CntUpdates++;
	}
}

void CunitTestDlg::testRendererTickEnd()
{
	//We should delete the window here, but might leave this for testing so we can see what we drew
	static DWORD msgHash_DestroyRenderer = CHashString(_T("DestroyRenderer")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_DestroyRenderer, 0, NULL, NULL, NULL );
}

void CunitTestDlg::TestPhysManager()
{
	// Set up the managers
	int i;
	m_REditBox.SetSel(0, -1);
	m_REditBox.ReplaceSel(_T(""));
	string tStr;
	TCHAR *components[] = { _T("CHierarchyManager"),
							_T("CAIManager"),
							_T("CPhysManager"),
							_T("CSoundManager"),
							_T("CGuiManager"),
							_T("CRenderManager"),
							_T("CInputManager"),
							_T("CSelectionManager") };

	#define NUM_COMPONENT_INIT (sizeof(components) / sizeof(components[0]))

	IComponent *compInit[NUM_COMPONENT_INIT];

	//Create some new window to render screen to
	for (i=0; i<NUM_COMPONENT_INIT; i++)
	{
		// need to instantiate some objects so that they register their listeners
		compInit[i] = m_ToolBox->CreateComponent(&CHashString(components[i]), 0);
	}

	// wait 2 seconds to display messages()
	m_MessageTicks = 2000;
	m_EndTick = GetTickCount() + m_MessageTicks;
	m_TickCount = 0;
	m_CntUpdates = 0;
	// wait 1/60th of a second to update
	m_RenderEndTick = GetTickCount() + 1000/60;

/*
	// Start testing the serialization methods
	IXMLArchive *ar;
	CREATEARCHIVE ca;

	ca.streamData = _T("C:/Development/ElementalEngine/Data/physTest.xml");
	ca.mode = STREAM_MODE_READ;

	//ar.Init( "c:/xml/testTypes.xml" );
	static DWORD msgHash_CreateXMLArchive = CHashString(_T("CreateXMLArchive")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_CreateXMLArchive, sizeof(CREATEARCHIVE),
		&ca) != MSG_HANDLED)
	{
		m_REditBox.ReplaceSel(_T("Message to Create XML Archive failed!"));
		return;
	}

	ar = dynamic_cast<IXMLArchive *>(ca.archive);

	IComponent* physObj = m_ToolBox->CreateComponent( &CHashString( _T("CPhysObject") ), NULL, NULL );


	if( physObj != NULL )
	{
		physObj->Serialize( *ar );
	}

	IXMLArchive *ar2;
	ca.streamData = _T("C:/Development/ElementalEngine/Data/physTestOut.xml");
	ca.mode = STREAM_MODE_WRITE;

	//ar.Init( "c:/xml/testTypes.xml" );
	static DWORD msgHash_CreateXMLArchive = CHashString(_T("CreateXMLArchive")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_CreateXMLArchive, sizeof(CREATEARCHIVE),
		&ca) != MSG_HANDLED)
	{
		m_REditBox.ReplaceSel(_T("Message to Create XML Archive failed!"));
		return;
	}

	ar2 = dynamic_cast<IXMLArchive *>(ca.archive);
	if( physObj != NULL )
	{
		physObj->Serialize( *ar2 );
	}

	ar->Close();
*/
}

bool CunitTestDlg::OnIdle(LONG count)
{
	if (m_CurIdleCallBack != NULL)
	{
		(this->*m_CurIdleCallBack)();
	}

	return false;
}

LRESULT CunitTestDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    DWORD QueueStatus;
    LRESULT resValue = 0;
    bool OnIdleRetVal = true;

	if (message == gMyIdleMessage) 
	{
        OnIdleRetVal = OnIdle((UINT)wParam);
        if(!OnIdleRetVal)
            wParam = 0;
		GLOBALTICKPARAMS gtp;
		gtp.tickCount = 0;
		static DWORD msgHash_GlobalUpdateTick = CHashString(_T("GlobalUpdateTick")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GlobalUpdateTick, sizeof(GLOBALTICKPARAMS), &gtp);
    } 
	else
	{
        resValue = CDialog::WindowProc(message, 
        wParam, lParam);
	}

    QueueStatus = GetQueueStatus(QS_ALLINPUT);

    if(HIWORD(QueueStatus) == 0)
        PostMessage(gMyIdleMessage, 
            wParam + (OnIdleRetVal ? 1 : 0), 0);

    return resValue;
}

void CunitTestDlg::OnBnClickedCancel()
{
	if (m_CurIdleCallBack != NULL)
	{
		m_CurIdleCallBack = NULL;
		if (m_EndIdleCallBack != NULL)
		{
			(this->*m_EndIdleCallBack)();
			m_EndIdleCallBack = NULL;
		}

		// if we are currently testing all functions
		if (m_CurTestAll != 0)
		{
			StartNextTest();
		}
	}
	else
		OnCancel();
}

void CunitTestDlg::OnBnClickedExit()
{
	OnCancel();
}

void CunitTestDlg::TestSelectionManager()
{
	int i;
//	DWORD iContextIndex;
	CREATERENDERCONTEXTPARAMS createContextMessage;
	m_REditBox.SetSel(0, -1);
	m_REditBox.ReplaceSel(_T(""));
	string tStr;
	TCHAR *components[] = {
	_T("CHierarchyManager"),
	_T("CAIManager"),
	_T("CPhysicsManager"),
	_T("CSoundManager"),
	_T("CGuiManager"),
	_T("CRenderManager"),
	_T("CInputManager"),
	_T("CSelectionManager")
	};

	//Create a render context using the window(right now we just use the default hwnd, it will
	//draw the background color black
	{
		// made a button, so now render into it.
		createContextMessage.hwindow = m_RenderWindowButton.GetSafeHwnd();
		// get the window size
		RECT winRect;
		m_RenderWindowButton.GetClientRect(&winRect);
		createContextMessage.width = (winRect.right - winRect.left);
		createContextMessage.height = (winRect.bottom - winRect.top);
		createContextMessage.depthbits = 24;
		createContextMessage.colorbits = 32;
		//we save off the context Index in iContextIndex
//		createContextMessage.retVal_RenderID = &iContextIndex;
		//Not creating a context right now because we want to just use the main window
		static DWORD msgHash_CreateRenderContext = CHashString(_T("CreateRenderContext")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_CreateRenderContext, sizeof(createContextMessage ),
			&createContextMessage, NULL, NULL );
	}
	

#define NUM_COMPONENT_INIT (sizeof(components) / sizeof(components[0]))

	IComponent *compInit[NUM_COMPONENT_INIT];

	//Create some new window to render screen to
	for (i=0; i<NUM_COMPONENT_INIT; i++)
	{
		// need to instantiate some objects so that they register their listeners
		compInit[i] = m_ToolBox->CreateComponent(&CHashString(components[i]), 0);
	}

	////////////////////////////////////////////////
	// start input
	// Start InputManager
	// Start InputManager
	INPUTENGPARAMS inputParams;

	HMODULE mod = GetModuleHandle(NULL);
	inputParams.hMod = mod;
	inputParams.hWnd = m_hWnd;
	inputParams.mouseImmediate = true;
	inputParams.mouseExclusive = false;
	inputParams.mouseFG = true;
	inputParams.kbImmediate = true;
	inputParams.kbExclusive = true;
	inputParams.kbFG = true;

	m_REditBox.ReplaceSel(_T("Starting Input...\n"));
	static DWORD msgHash_Start = CHashString(_T("Start")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_Start, sizeof(INPUTENGPARAMS), &inputParams, NULL, &CHashString(_T("CInputManager")));
	//////////////////////////////////////////////////////////////

	// wait 2 seconds to display messages()
	m_MessageTicks = 2000;
	m_EndTick = GetTickCount() + m_MessageTicks;
	m_TickCount = 0;
	m_CntUpdates = 0;
	// wait 1/60th of a second to update
	m_RenderEndTick = GetTickCount() + 1000/60;

	// setup OnIdleCallBack
	m_CurIdleCallBack = &CunitTestDlg::testMyRenderTick;
	m_EndIdleCallBack = NULL;
			
	for (i=0; i<NUM_COMPONENT_INIT; i++)
	{
		// need to instantiate some objects so that they register their listeners
//		delete compInit[i];
	}
}

void CunitTestDlg::testMyRenderTick()
{
	ADDRENDERLISTPARAMS renderMessage;		
	string tStr;
	TCHAR tempNum[100];

	if (m_EndTick > GetTickCount())
	{
		// wait m_MessageTicks second to display messge
		m_EndTick = GetTickCount() + m_MessageTicks;
		_ltot(m_CntUpdates, tempNum, 10);
		tStr = tempNum;
		tStr += _T(" Render Updates have occured at tick ");
		_ltot(m_TickCount, tempNum, 10);
		tStr +=  tempNum;
		tStr += _T("\n");
		m_REditBox.ReplaceSel(tStr.c_str());
		m_TickCount++;
	}

	// wait 1/60th of a second to update
	m_RenderEndTick = GetTickCount() + (1000/60);
	//create a message to render per frame
	renderMessage.objList = &m_RenderObjList;
	renderMessage.dwSortMethod = SORT_FIFO;
	renderMessage.dwListPriorityKey = RENDERPRIORITY_OPAQUE;

	//pass on the list to add to the render manager using the above sort method(will change method
	//of passing this on where sort is in this message
	static DWORD msgHash_AddRenderList = CHashString(_T("AddRenderList")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_AddRenderList, sizeof(renderMessage), &renderMessage, NULL, NULL );

	GLOBALTICKPARAMS gtp;
	gtp.tickCount = 0;
	static DWORD msgHash_GlobalUpdateTick = CHashString(_T("GlobalUpdateTick")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GlobalUpdateTick, sizeof(GLOBALTICKPARAMS), &gtp, NULL, NULL);
	m_CntUpdates++;
}

void CunitTestDlg::TestLuaManager()
{
	LOADFILEEXTPARAMS lfep;

	lfep.fileName = _T("exampleActions.lua");
	lfep.bInternalLoad = true;

	static DWORD msgHash_LoadFileByExtension = CHashString(_T("LoadFileByExtension")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_LoadFileByExtension, sizeof(LOADFILEEXTPARAMS), &lfep);

	lfep.fileName = _T("JoshTest.xms");
	m_ToolBox->SendMessage(msgHash_LoadFileByExtension, sizeof(LOADFILEEXTPARAMS), &lfep);

	CREATEOBJECTPARAMS cop;
	INITOBJECTPARAMS iop;

	CHashString szScript1(_T("ScriptObject1"));
	CHashString szCLuaScript(_T("CLuaScript"));

	m_REditBox.ReplaceSel(_T("Testing Creation of Script Object 1 - \n"));

	CHashString szDummyEntity(_T("MyEntity"));
	CHashString szCEntity(_T("CEntity"));
	
	// create entity
	cop.name = &szDummyEntity;
	cop.parentName = NULL;
	cop.typeName = &szCEntity;
	static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop, NULL, NULL);

	// create script
	cop.name = &szScript1;
	cop.parentName = &szDummyEntity;
	cop.typeName = &szCLuaScript;
	m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop, NULL, NULL);

	// create archive
	CREATEARCHIVE ca;
	SERIALIZEOBJECTPARAMS sop;
	IArchive *ar;
	CHashString memType(_T("Memory"));
	char * memBuffer;
	DWORD dataSize = 0;

	dataSize += sizeof(TCHAR)*((DWORD)_tcslen(lfep.fileName)+1);

	memBuffer = new char[dataSize];
	memset(memBuffer, 0, sizeof(char)*dataSize);

	ca.mode = STREAM_MODE_WRITE | STREAM_MODE_READ;
	ca.streamData = memBuffer;
	ca.streamSize = sizeof(char)*dataSize;
	ca.streamType = &memType;
	int id = sizeof(CREATEARCHIVE);
	int bid = sizeof(ca);
	static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca);
	ar = ca.archive;

	ar->Write(lfep.fileName);

	sop.name = cop.name;
	sop.archive = ar;
	// serialize object
	static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop);

	ar->Close();

	iop.name = cop.name;
	static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop, NULL, NULL);


}

void CunitTestDlg::TestObjectNameChange()
{
	CREATEOBJECTPARAMS cop;
	INITOBJECTPARAMS iop;
	DELETEOBJECTPARAMS dop;
	CHANGEOBJNAME cnp;

	CHashString szEntity1(_T("Entity1"));
	CHashString szCEntity(_T("CEntity"));
	CHashString szEntity2(_T("Entity2"));
	CHashString szEntity3(_T("Entity3"));

	m_REditBox.ReplaceSel(_T("Creating Entity 1\n"));

	// now let's try and create the objects through messages, which should be
	// the way the game does it.
	cop.name = &szEntity1;
	cop.parentName = NULL;
	cop.typeName = &szCEntity;
	static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop, NULL, NULL);
	iop.name = cop.name;
	static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop, NULL, NULL);

	m_REditBox.ReplaceSel(_T("Creating Entity 2\n"));
	cop.name = &szEntity2;
	m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop, NULL, NULL);
	iop.name = cop.name;
	m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop, NULL, NULL);

	m_REditBox.ReplaceSel(_T("Trying to rename object 1 to object 2, which should fail\n"));

	cnp.oldName = &szEntity1;
	cnp.newName = &szEntity2;
	cnp.compType = &szCEntity;
	static DWORD msgHash_ChangeObjectName = CHashString(_T("ChangeObjectName")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_ChangeObjectName, sizeof(CHANGEOBJNAME), &cnp, NULL, NULL) != MSG_WARNING)
	{
		m_REditBox.ReplaceSel(_T("Rename succeeded when it should have failed!\n"));
	}
	else
	{
		m_REditBox.ReplaceSel(_T("Rename failed like it should have\n"));
	}

	cnp.newName = &szEntity3;
	if (m_ToolBox->SendMessage(msgHash_ChangeObjectName, sizeof(CHANGEOBJNAME), &cnp, NULL, NULL) == MSG_HANDLED)
	{
		m_REditBox.ReplaceSel(_T("Rename worked like it should have\n"));
	}
	else
	{
		m_REditBox.ReplaceSel(_T("Rename failed when it should have succeeded!\n"));
	}

	FillHierarchyTree();

	dop.name = &szEntity1;
	static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof(DELETEOBJECTPARAMS), &dop, NULL, NULL);

	dop.name = &szEntity2;
	m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof(DELETEOBJECTPARAMS), &dop, NULL, NULL);

}
/*
void CunitTestDlg::TestIHashStringPtr(IHashStringPtr theHashString)
{
	m_REditBox.ReplaceSel(theHashString->GetString());
	m_REditBox.ReplaceSel(_T("\n"));
}

void CunitTestDlg::TestYasper()
{
	CHashString *temp;

	temp = new CHashString(_T("This is a test"));

	TestIHashStringPtr(temp);

	delete temp;

	TestIHashStringPtr(temp);

}
*/
void CunitTestDlg::TestEntityManager()
{
	SERIALIZEOBJECTPARAMS sop;
	CREATEOBJECTPARAMS cop;
	INITOBJECTPARAMS iop;
	VISITENTITYDATA ved;
	CREATEARCHIVE ca;
	CHashString memType(_T("Memory"));

	char * chunkomem;
	chunkomem = new char[100];
	IArchive *memArchive;
	ca.mode = STREAM_MODE_WRITE | STREAM_MODE_READ | ARCHIVE_MODE_WRITE;
	ca.streamData = chunkomem;
	ca.streamSize = sizeof(char)*100;
	ca.streamType = &memType;
	static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE),
		&ca) != MSG_HANDLED)
	{
		return;
	}
	memArchive = ca.archive;

	StdString entityType1 = _T("NK::Soldiers::Corporals");
	StdString entityType2 = _T("NK::Soldiers::Seargents");
	StdString entityType3 = _T("NK::Soldiers");
	StdString entityType4 = _T("Tank");
	
	CHashString entity1 = _T("NK::Soldiers::Corporals::OneEye");
	CHashString entity2 = _T("Loser");
	CHashString entity3 = _T("Hamlet");

	CHashString search1 = _T("NK::Soldiers");
	CHashString search2 = _T("NK::Soldiers::Corporals");
	CHashString search3 = _T("NK::Soldiers::Seargents::blah");
	CHashString search4 = _T("NK::Soldiers1::Corporals");
	CHashString search5 = _T("NK::Soldiers::");

	CHashString compType = _T("CEntity");
	CHashString em = _T("CEntityManager");

	m_ToolBox->CreateComponent(&em, 0);

	cop.name = &entity1;
	cop.typeName = &compType;
	cop.parentName = NULL;
	iop.name = &entity1;
	static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop);
	memArchive->Write(entityType1);
	memArchive->SetIsWriting(false);
	memArchive->SeekTo(0,SEEK_SET);
	sop.archive = memArchive;
	sop.name = &entity1;
	static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop);
	static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop);

	cop.name = &entity2;
	iop.name = &entity2;
	m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop);
	memArchive->SetIsWriting(true);
	memArchive->SeekTo(0,SEEK_SET);
	memArchive->Write(entityType2);
	memArchive->SetIsWriting(false);
	memArchive->SeekTo(0,SEEK_SET);
	sop.archive = memArchive;
	sop.name = &entity2;
	m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop);
	m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop);

	cop.name = &entity3;
	iop.name = &entity3;
	m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop);
	memArchive->SetIsWriting(true);
	memArchive->SeekTo(0,SEEK_SET);
	memArchive->Write(entityType3);
	memArchive->SetIsWriting(false);
	memArchive->SeekTo(0,SEEK_SET);
	sop.archive = memArchive;
	sop.name = &entity3;
	m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop);
	m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop);

	ved.searchString = &search1;
	ved.visitor = NULL;
	static DWORD msgHash_VisitEntities = CHashString(_T("VisitEntities")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_VisitEntities, sizeof(VISITENTITYDATA), &ved);

	ved.searchString = &search2;
	ved.visitor = NULL;
	m_ToolBox->SendMessage(msgHash_VisitEntities, sizeof(VISITENTITYDATA), &ved);

	ved.searchString = &search3;
	ved.visitor = NULL;
	m_ToolBox->SendMessage(msgHash_VisitEntities, sizeof(VISITENTITYDATA), &ved);

	ved.searchString = &search4;
	ved.visitor = NULL;
	m_ToolBox->SendMessage(msgHash_VisitEntities, sizeof(VISITENTITYDATA), &ved);

	ved.searchString = &search5;
	ved.visitor = NULL;
	m_ToolBox->SendMessage(msgHash_VisitEntities, sizeof(VISITENTITYDATA), &ved);
}

void CunitTestDlg::TestSoundManager()
{
	//create CSoundManager;
	CHashString sm(_T("CSoundManager"));
	m_REditBox.ReplaceSel(_T("Testing Creation of Sound Manager - "));
	m_ToolBox->CreateComponent(&sm, 0);

	// create archive for serialize
	CREATEARCHIVE ca;
	CHashString memType(_T("Memory"));
	char * chunkomem;
	chunkomem = new char[100];
	IArchive *memArchive;
	ca.mode = STREAM_MODE_WRITE | STREAM_MODE_READ | ARCHIVE_MODE_WRITE;
	ca.streamData = chunkomem;
	ca.streamSize = sizeof(char)*100;
	ca.streamType = &memType;
	static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE),
		&ca) != MSG_HANDLED)
	{
		return;
	}
	memArchive = ca.archive;
	
	// setup sound object
	StdString szSoundFile = _T("c:\\redphoenix\\sound\\welcome.wav");
	bool bLooping = false;
	bool b3DSound = false;
	float x = 0.f;
	float y = 0.f;
	float z = 0.f;
	bool bAutoRemove = false;
	float fVolume = 1.0f;

	// add sound parameters to sound object
	memArchive->Write(szSoundFile);
	memArchive->Write(bLooping);
	memArchive->Write(b3DSound);
	memArchive->Write(x);
	memArchive->Write(y);
	memArchive->Write(z);
	memArchive->Write(bAutoRemove);
	memArchive->Write(fVolume);

	// reset archive and set to read
	memArchive->SeekTo(0, SEEK_SET);
	memArchive->SetIsWriting(false);

	//now lets try sending a message to create a sound object
	CREATEOBJECTPARAMS sound;
	SERIALIZEOBJECTPARAMS sop;
	CHashString sndName(_T("Song"));
	CHashString compType(_T("CSoundObject"));

	sound.name = &sndName;
	sound.parentName = NULL;
	sound.typeName = &compType;

	m_REditBox.ReplaceSel(_T("Testing Creation of SoundObj1 -\n"));

	static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &sound);

	sop.name = &sndName;
	sop.archive = memArchive;
	static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop);

	// play sound
	static DWORD msgHash_PlaySound = CHashString(_T("PlaySound")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_PlaySound, sizeof(IHashString*), &sndName);
}

void CunitTestDlg::TestQHStateMachine()
{
	CHashString sm(_T("CQHStateMachineManager"));
	m_REditBox.ReplaceSel(_T("Testing QH State Machine...\n"));

	CStateMachineTest stateMachineTest( &m_REditBox );
	stateMachineTest.Initialize();

	m_REditBox.ReplaceSel(_T("  TestLoadStateMachine1 - "));
	REPORT( stateMachineTest.TestLoadStateMachine1() );

	m_REditBox.ReplaceSel(_T("  TestCreateStateObject1 - "));
	REPORT( stateMachineTest.TestCreateStateObject1() );

	m_REditBox.ReplaceSel(_T("  TestStateMachineEvents1 - "));
	REPORT( stateMachineTest.TestStateMachineEvents1() );

	m_REditBox.ReplaceSel(_T("  TestQueryStateMachineEvents1 - "));
	REPORT( stateMachineTest.TestQueryStateMachineEvents1() );

	m_REditBox.ReplaceSel(_T("  TestDerivedStateMachine - "));
	REPORT( stateMachineTest.TestDerivedStateMachine() );

	m_REditBox.ReplaceSel(_T("  TestDerivedStateMachineEvents - "));
	REPORT( stateMachineTest.TestDerivedStateMachineEvents() );

	//m_REditBox.ReplaceSel(_T("  TestQueryStateMachineEvents2 - "));
	//REPORT( stateMachineTest.TestQueryStateMachineEvents2() );

	m_REditBox.ReplaceSel(_T("  Actions Reordering - "));
	REPORT( stateMachineTest.TestActionsReordering() );

	m_REditBox.ReplaceSel(_T("  Change Parent - "));
	REPORT( stateMachineTest.TestChangeParent() );

	stateMachineTest.DeInitialize();
}

void CunitTestDlg::TestEntityQuery()
{
	m_REditBox.ReplaceSel(_T("Testing Entity Query...\n"));

	CEntityQueryTest entityQueryTest;
	entityQueryTest.Initialize();

	m_REditBox.ReplaceSel(_T("  TestEmptyQuery - "));
	REPORT( entityQueryTest.TestEmptyQuery() );

	m_REditBox.ReplaceSel(_T("  TestQuery1 - "));
	REPORT( entityQueryTest.TestQuery1() );
}

void CunitTestDlg::OnCancel()
{
	CDialog::OnCancel();
}

void CunitTestDlg::TestOpenALAudio()
{
	m_REditBox.ReplaceSel(_T("Testing Open AL Audio...\n"));
	m_CurIdleCallBack = &CunitTestDlg::TestALInIdle;
}

void CunitTestDlg::TestALInIdle()
{
	UINT testState = m_SoundTest.GetTestState();
	static TCHAR* soundFile1 = _T("basedata\\hover_bot_float.wav");
	static TCHAR* soundFile2 = _T("basedata\\punch1.wav");
	static TCHAR* soundFile3 = _T("basedata\\modok_laugh01.ogg");
	static TCHAR* playlistFile1 = _T("basedata\\wavplaylisttest.xmp");
	static TCHAR* playlistFile2 = _T("basedata\\oggplaylisttest.xmp");

	TCHAR outputBuffer[2048];
	GETSOUNDFILEINFOPARAMS gsfip;
	switch(testState)
	{
		case 0: // load test file
		{
			m_REditBox.ReplaceSel(_T("   Test file 1 loading - "));

			bool ret = m_SoundTest.TestFileExists(soundFile1, &gsfip);
			REPORT(ret);
			if (ret)
			{
				_stprintf(outputBuffer, _T("\tfilename=%s\n\tfreq=%d\n\tbitdepth=%d\n\tbitrate=%d\n\tchannels=%d\n\tfilesize=%d\n\tduration=%.3f\n"), 
					soundFile1, gsfip.frequency, gsfip.bitDepth, gsfip.bitRate, gsfip.channels, gsfip.fileSize, gsfip.duration);
				m_REditBox.ReplaceSel(outputBuffer);
			}
			m_SoundTest.IncrementTestState();
			break;
		}
		case 1: // play test file
		{
            m_REditBox.ReplaceSel(_T("   Test 2D sound playing with no loop - "));

			bool ret = m_SoundTest.Test2DFilePlaybackNoLoop(soundFile1);
			REPORT(ret);
			
			m_SoundTest.IncrementTestState();
			if (ret)
			{
				m_SoundTest.SetTimeToCheck(6.0f);
				m_REditBox.ReplaceSel(_T("   A quick pause before moving on...\n"));			
			}
			else
			{
				m_SoundTest.SetTimeToCheck(0.0f);
			}
			
			break;	
		}
		case 2: // wait a bit
		{
			bool ret = m_SoundTest.TickTime();
			if (ret)
			{
				m_SoundTest.IncrementTestState();
			}
			break;
		}		
		case 3: // load different test file
		{
            m_REditBox.ReplaceSel(_T("   Test file 2 loading - "));

			bool ret = m_SoundTest.TestFileExists(soundFile2, &gsfip);
			REPORT(ret);
			if (ret)
			{
				_stprintf(outputBuffer, _T("\tfilename=%s\n\tfreq=%d\n\tbitdepth=%d\n\tbitrate=%d\n\tchannels=%d\n\tfilesize=%d\n\tduration=%.3f\n"), 
					soundFile2, gsfip.frequency, gsfip.bitDepth, gsfip.bitRate, gsfip.channels, gsfip.fileSize, gsfip.duration);
				m_REditBox.ReplaceSel(outputBuffer);
			}
			
			m_SoundTest.IncrementTestState();
			break;
		}
		case 4: // play different test file, looped
		{
            m_REditBox.ReplaceSel(_T("   Test 2D sound playing with loop - "));

			bool ret = m_SoundTest.Test2DFilePlaybackLoopStart(soundFile2);
			REPORT(ret);
			
			m_SoundTest.IncrementTestState();

			if (ret)
			{
				m_SoundTest.SetTimeToCheck(3.072f);
				m_REditBox.ReplaceSel(_T("   A quick pause before moving on (should hear 8 loops)...\n"));
			}
			else
			{
				m_SoundTest.SetTimeToCheck(0.0f);
			}

			break;	
		}
		case 5: // wait a bit
		{
			bool ret = m_SoundTest.TickTime();
			if (ret)
			{
				m_SoundTest.IncrementTestState();
			}
			break;
		}		
		case 6: // stop looping sound
		{ 
			m_REditBox.ReplaceSel(_T("   Test 2D sound stop looping sound - "));

			bool ret = m_SoundTest.Test2DFilePlaybackLoopStop();
			REPORT(ret);
			
			m_SoundTest.IncrementTestState();				
			
			break;
		}
		case 7: // play 3d positioned sound
		{
            m_REditBox.ReplaceSel(_T("   Test 3D sound playing static right - "));
			Vec3 testPos(0.0f, 50.0f, 0.0f);
			bool ret = m_SoundTest.Test3DFilePlaybackStatic(soundFile1, testPos);
			REPORT(ret);
			
			m_SoundTest.IncrementTestState();
			
			if (ret)
			{
				m_SoundTest.SetTimeToCheck(6.0f);
				m_REditBox.ReplaceSel(_T("   A quick pause before moving on...\n"));
			}
			else
			{
				m_SoundTest.SetTimeToCheck(0.0f);
			}
			
			break;	
		}
		case 8: // wait a bit
		{
			bool ret = m_SoundTest.TickTime();
			if (ret)
			{
				m_SoundTest.IncrementTestState();
			}
			break;
		}	
		case 9: // play 3d positioned sound
		{
            m_REditBox.ReplaceSel(_T("   Test 3D sound playing static left - "));
			Vec3 testPos(0.0f, -50.0f, 0.0f);
			bool ret = m_SoundTest.Test3DFilePlaybackStatic(soundFile1, testPos);
			REPORT(ret);
			
			m_SoundTest.IncrementTestState();
			
			if (ret)
			{
				m_SoundTest.SetTimeToCheck(6.0f);
				m_REditBox.ReplaceSel(_T("   A quick pause before moving on...\n"));
			}
			else
			{
				m_SoundTest.SetTimeToCheck(0.0f);
			}
			
			break;	
		}
		case 10: // wait a bit
		{
			bool ret = m_SoundTest.TickTime();
			if (ret)
			{
				m_SoundTest.IncrementTestState();
			}
			break;
		}	
		case 11: // play 3d positioned sound
		{
            m_REditBox.ReplaceSel(_T("   Test 3D sound spawner static unlooped - "));
			bool ret = m_SoundTest.Test3DSoundSpawner(_T("basedata\\soundspawntestnoloop.sxl"));
			REPORT(ret);
			
			m_SoundTest.IncrementTestState();
			
			if (ret)
			{
				m_SoundTest.SetTimeToCheck(6.0f);
				m_REditBox.ReplaceSel(_T("   A quick pause before moving on...\n"));
			}
			else
			{
				m_SoundTest.SetTimeToCheck(0.0f);
			}
			
			break;	
		}
		case 12: // wait a bit
		{
			bool ret = m_SoundTest.TickTime();
			if (ret)
			{
				m_SoundTest.IncrementTestState();
			}
			break;
		}	
		case 13: // play different test file, looped
		{
            m_REditBox.ReplaceSel(_T("   Test 3D spawned sound playing with loop - "));

			bool ret = m_SoundTest.Test3DSoundSpawner(_T("basedata\\soundspawntestlooping.sxl"));
			REPORT(ret);
			
			m_SoundTest.IncrementTestState();

			if (ret)
			{
				m_SoundTest.SetTimeToCheck(6.0f);
				m_REditBox.ReplaceSel(_T("   A quick pause before moving on...\n"));
			}
			else
			{
				m_SoundTest.SetTimeToCheck(0.0f);
			}

			break;	
		}
		case 14: // wait a bit
		{
			bool ret = m_SoundTest.TickTime();
			if (ret)
			{
				m_SoundTest.IncrementTestState();
			}
			break;
		}		
		case 15: // stop looping sound
		{ 
			m_REditBox.ReplaceSel(_T("   Test 3D spawned sound stop looping sound - "));

			bool ret = m_SoundTest.Test3DStopLoopingSpawnedSound();
			REPORT(ret);
			
			m_SoundTest.IncrementTestState();				
			
			break;
		}
		case 16: // play different test file, looped
		{
            m_REditBox.ReplaceSel(_T("   Test 3D spawned sound playing with loop moving - "));

			bool ret = m_SoundTest.Test3DSoundSpawner(_T("basedata\\soundspawntestlooping.sxl"));
			REPORT(ret);
			
			m_SoundTest.IncrementTestState();

			if (ret)
			{
				m_SoundTest.SetTimeToCheck(15.0f);
				m_REditBox.ReplaceSel(_T("   A quick pause before moving on (should hear sound pan right to left)...\n"));
			}
			else
			{
				m_SoundTest.SetTimeToCheck(0.0f);
			}

			break;	
		}
		case 17: // wait a bit
		{
			m_SoundTest.Update3DSoundPositionLinear();
			bool ret = m_SoundTest.TickTime();
			if (ret)
			{
				m_SoundTest.IncrementTestState();
			}
			break;
		}		
		case 18: // stop looping sound
		{ 
			m_REditBox.ReplaceSel(_T("   Test 3D spawned sound stop looping sound - "));

			bool ret = m_SoundTest.Test3DStopLoopingSpawnedSound();
			REPORT(ret);
			
			m_SoundTest.IncrementTestState();				
			
			break;
		}
		case 19: // play different test file, looped
		{
            m_REditBox.ReplaceSel(_T("   Test 3D spawned sound playing with loop moving - "));

			bool ret = m_SoundTest.Test3DSoundSpawner(_T("basedata\\soundspawntestlooping.sxl"));
			REPORT(ret);
			
			m_SoundTest.IncrementTestState();

			if (ret)
			{
				m_SoundTest.SetTimeToCheck(15.0f);
				m_REditBox.ReplaceSel(_T("   A quick pause before moving on (should hear sound revolve around you)...\n"));
			}
			else
			{
				m_SoundTest.SetTimeToCheck(0.0f);
			}

			break;	
		}
		case 20: // wait a bit
		{
			m_SoundTest.Update3DSoundPositionRadial();
			bool ret = m_SoundTest.TickTime();
			if (ret)
			{
				m_SoundTest.IncrementTestState();
			}
			break;
		}		
		case 21: // stop looping sound
		{ 
			m_REditBox.ReplaceSel(_T("   Test 3D spawned sound stop looping sound - "));

			bool ret = m_SoundTest.Test3DStopLoopingSpawnedSound();
			REPORT(ret);
			
			m_SoundTest.IncrementTestState();				
			
			break;
		}
		case 22: // play test file
		{
            m_REditBox.ReplaceSel(_T("   Test OGG file playing - "));

			bool ret = m_SoundTest.Test2DFilePlaybackNoLoop(soundFile3);
			REPORT(ret);
			
			m_SoundTest.IncrementTestState();
			if (ret)
			{
				m_SoundTest.SetTimeToCheck(6.0f);
				m_REditBox.ReplaceSel(_T("   A quick pause before moving on...\n"));			
			}
			else
			{
				m_SoundTest.SetTimeToCheck(0.0f);
			}
			
			break;	
		}
		case 23: // wait a bit
		{
			m_SoundTest.Update3DSoundPositionRadial();
			bool ret = m_SoundTest.TickTime();
			if (ret)
			{
				m_SoundTest.IncrementTestState();
			}
			break;
		}
		case 24: // play test file
		{
            m_REditBox.ReplaceSel(_T("   Test OGG file playing looped - "));

			bool ret = m_SoundTest.Test2DFilePlaybackLoopStart(soundFile3);
			REPORT(ret);
			
			m_SoundTest.IncrementTestState();
			if (ret)
			{
				m_SoundTest.SetTimeToCheck(20.0f);
				m_REditBox.ReplaceSel(_T("   A quick pause before moving on...\n"));			
			}
			else
			{
				m_SoundTest.SetTimeToCheck(0.0f);
			}
			
			break;	
		}
		case 25: // wait a bit
		{
			bool ret = m_SoundTest.TickTime();
			if (ret)
			{
				m_SoundTest.IncrementTestState();
			}
			break;
		}
		case 26: // stop looping sound
		{ 
			m_REditBox.ReplaceSel(_T("   Test 2D sound stop looping sound - "));

			bool ret = m_SoundTest.Test2DFilePlaybackLoopStop();
			REPORT(ret);
			
			m_SoundTest.IncrementTestState();				
			
			break;
		}
		case 27: // load playlist file
		{
			m_REditBox.ReplaceSel(_T("   Test wav playlist loading - "));

			bool ret = m_SoundTest.TestPlaylistLoad(playlistFile1);
			REPORT(ret);			
			m_SoundTest.IncrementTestState();
			break;
		}
		case 28: // play test file
		{
            m_REditBox.ReplaceSel(_T("   Test wav playlist playing - "));

			bool ret = m_SoundTest.TestPlaylistPlay(playlistFile1);
			REPORT(ret);
			
			m_SoundTest.IncrementTestState();
			if (ret)
			{
				m_SoundTest.SetTimeToCheck(15.0f);
				m_REditBox.ReplaceSel(_T("   A quick pause before moving on...\n"));			
			}
			else
			{
				m_SoundTest.SetTimeToCheck(0.0f);
			}
			
			break;	
		}
		case 29: // wait a bit
		{
			bool ret = m_SoundTest.TickTime();
			if (ret)
			{
				m_SoundTest.IncrementTestState();
			}
			break;
		}	
		case 30: // load playlist file
		{
			m_REditBox.ReplaceSel(_T("   Test ogg playlist loading - "));

			bool ret = m_SoundTest.TestPlaylistLoad(playlistFile2);
			REPORT(ret);			
			m_SoundTest.IncrementTestState();
			break;
		}
		case 31: // play test file
		{
            m_REditBox.ReplaceSel(_T("   Test ogg playlist playing - "));

			bool ret = m_SoundTest.TestPlaylistPlay(playlistFile2);
			REPORT(ret);
			
			m_SoundTest.IncrementTestState();
			if (ret)
			{
				m_SoundTest.SetTimeToCheck(4.0f);
				m_REditBox.ReplaceSel(_T("   A quick pause before moving on...\n"));			
			}
			else
			{
				m_SoundTest.SetTimeToCheck(0.0f);
			}
			
			break;	
		}
		case 32: // wait a bit
		{
			bool ret = m_SoundTest.TickTime();
			if (ret)
			{
				m_SoundTest.IncrementTestState();
			}
			break;
		}	
		default:
		{
			m_REditBox.ReplaceSel(_T("Test Complete.\n"));
			m_CurIdleCallBack = NULL;		
			m_SoundTest.ResetTestState();
		}
	}
}

void CunitTestDlg::TestDatabase()
{
	m_REditBox.ReplaceSel(_T("Testing Database...\n"));
	CDatabaseTest databaseTest( &m_REditBox );
	databaseTest.Initialize();
	databaseTest.RunTests();
}

void CunitTestDlg::TestNodeSystem()
{
	m_REditBox.ReplaceSel(_T("Testing Node System...\n"));
	CNodeSystemTest nodeSystemTest( &m_REditBox );
	nodeSystemTest.Initialize();
	REPORT( nodeSystemTest.RunTests() == 0 )
}

void CunitTestDlg::TestMathLibrary()
{
	m_REditBox.ReplaceSel(_T("Testing Math Library...\n"));
	CMathLibraryTest mathLibraryTest( &m_REditBox );
	mathLibraryTest.Initialize();
	int failedTests = mathLibraryTest.RunTests();
	m_REditBox.ReplaceSel(_T("Math Library test cases: "));
	REPORT( failedTests == 0 );
}

void CunitTestDlg::TestZippedMemoryStream()
{
	CREATEARCHIVE ca;
	CHashString memType(_T("ZippedMemory"));

	ca.mode = STREAM_MODE_WRITE | STREAM_MODE_READ;
	ca.streamData = NULL;
	ca.streamSize = 0;
	ca.streamType = &memType;

	static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
	DWORD retVal = m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca);
	ASSERT( retVal == MSG_HANDLED );

	IArchive *pArchive = ca.archive;

	DWORD dwTick = GetTickCount();
	srand((unsigned int)time(NULL));
	std::vector<char> data;
	std::vector<char> res;
	data.reserve(0x10000);
	res.reserve(0x10000);
	for (size_t nSize = 0x10; nSize <= 0x10000; nSize <<= 1)
	{
		DWORD dwStartTick = GetTickCount();
		pArchive->SetIsWriting(true);
		pArchive->SeekTo(0);
		data.resize(nSize);
		generate(data.begin(), data.end(), rand);
		//fill(data.begin(), data.end(), 1);
		pArchive->Write(&data.front(), data.size());
		pArchive->SetIsWriting(false);
		pArchive->SeekTo(0);
		DWORD dwReadTick = GetTickCount();
		res.resize(nSize);
		for (std::vector<char>::iterator it = res.begin(); it != res.end(); ++it)
		{
			pArchive->Read(*it);
		}
		//pArchive->Read(&res.front(), res.size());

		DWORD dwEndTick = GetTickCount();
		if (!equal(data.begin(), data.end(), res.begin()))
		{
			
			m_REditBox.ReplaceSel(_T("Test failed: data is not equal\n"));
		}
		else
		{
			m_REditBox.ReplaceSel(_T("Test passed\n"));
		}
		TRACE(_T("ZippedMemoryStream(%i). Write: %i; Read: %i;\n"), nSize, dwReadTick - dwStartTick, dwEndTick - dwReadTick);
	}

	TRACE(_T("TestZippedMemoryStream duaration is %i\n"), GetTickCount() - dwTick);
}

void CunitTestDlg::TestPhysFSStream()
{
	bool failed = false;
	IArchive *pArchive;
	StdString sResourcePath = _T(".\\");
	m_ToolBox->SetDirectories(NULL, &sResourcePath);

	static DWORD msgHash_AddPhysFSDir = CHashString(_T("AddPhysFSDir")).GetUniqueID();
	DWORD retVal = m_ToolBox->SendMessage(msgHash_AddPhysFSDir, sizeof(LPCTSTR), _T("physfs.zip"));
	if (MSG_HANDLED != retVal)
	{
		m_REditBox.ReplaceSel(_T("PhysFS: Add search path message failed!\n"));
		failed = true;
	}

	srand(1);
	std::vector<char> data(0x10000);
	std::vector<char> res(data.size());
	fill(data.begin(), data.end(), 1);
	//generate(data.begin(), data.end(), rand);

	CREATEARCHIVE ca;
	CHashString PhysFSType(_T("PhysFS"));
	ca.mode = STREAM_MODE_WRITE;
	ca.streamData = "test.dat";
	ca.streamSize = 0;
	ca.streamType = &PhysFSType;

	static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
	retVal = m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca);
	if (retVal == MSG_HANDLED)
	{
		pArchive = ca.archive;
		pArchive->SetIsWriting(true);
		pArchive->SeekTo(0);
		pArchive->Write(&data.front(), data.size());
		pArchive->Close();
	}
	else
	{
		m_REditBox.ReplaceSel(_T("PhysFS: Failed to create write stream!\n"));
		failed = true;
	}

	ca.mode = STREAM_MODE_READ;
	retVal = m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca);
	if (retVal == MSG_HANDLED)
	{
		pArchive = ca.archive;
		pArchive->SetIsWriting(false);
		pArchive->SeekTo(0);

		// fill buffer with random data before check
		generate(res.begin(), res.end(), rand);
		for (std::vector<char>::iterator it = res.begin(); it != res.end(); ++it)
		{
			pArchive->Read(*it);
		}
		pArchive->Close();

		if (!equal(data.begin(), data.end(), res.begin()))
		{
			m_REditBox.ReplaceSel(_T("PhysFS: Test failed. Readed data is not equal\n"));
			failed = true;
		}
	}
	else
	{
		m_REditBox.ReplaceSel(_T("PhysFS: Failed to create read stream!\n"));
		failed = true;
	}

	DeleteFile(_T("test.dat"));

	ca.streamData = "zippedtest.dat";
	retVal = m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca);
	if (retVal == MSG_HANDLED)
	{
		pArchive = ca.archive;
		pArchive->SetIsWriting(false);
		pArchive->SeekTo(0);

		// fill buffer with random data before check
		generate(res.begin(), res.end(), rand);
		for (std::vector<char>::iterator it = res.begin(); it != res.end(); ++it)
		{
			pArchive->Read(*it);
		}
		pArchive->Close();

		if (!equal(data.begin(), data.end(), res.begin()))
		{
			m_REditBox.ReplaceSel(_T("PhysFS: Test failed. Readed data from zip file is incorrect\n"));
			failed = true;
		}
	}
	else
	{
		m_REditBox.ReplaceSel(_T("PhysFS: Failed to create zip read stream!\n"));
		failed = true;
	}

	if (failed)
	{
		m_REditBox.ReplaceSel(_T("PhysFS: Test failed\n"));
	}
	else
	{
		m_REditBox.ReplaceSel(_T("PhysFS: Test passed\n"));
	}
}

void CunitTestDlg::TestLanguageLocalizer()
{
	m_REditBox.ReplaceSel(_T("Testing Language Localizer...\n"));
	CLanguageTest languageTest( &m_REditBox );
	languageTest.Initialize();
	languageTest.RunTests();
}

void CunitTestDlg::TestWin32Network( void )
{
	m_REditBox.ReplaceSel(_T("Testing Win32 Network Pluging...\n"));
	CWin32NetworkTest networkTest( &m_REditBox );
	networkTest.Initialize();
	networkTest.SetTimeToCheck( 1.0f );
	while( !networkTest.TickTime() )
	{
	}
	
	networkTest.TestNetworkPeerCreateHost();
	networkTest.TestNetworkPeerCreateClient();
	networkTest.SetTimeToCheck( 1.0f );
	while( !networkTest.TickTime() )
	{
	}

	networkTest.TestNetworkPeerConnect();
	networkTest.SetTimeToCheck( 1.0f );
	while( !networkTest.TickTime() )
	{
	}

	networkTest.TestNetworkPeerCommunicate();
	networkTest.SetTimeToCheck( 1.0f );
	while( !networkTest.TickTime() )
	{
	}

	networkTest.TestNetworkPeerDisconect();
}