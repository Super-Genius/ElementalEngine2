///==========================================================================
/// \file	GameComponent.cpp
/// \brief	Component that listens for singleton messages for Game app
/// \date	11/16/2006
/// \author	Nick Tourte
///==========================================================================

#include "StdAfx.h"
#include "GameComponent.h"

#ifdef _SHELL
using namespace Gvr::Zeus::GvrIO;
#endif

void TerminateApp( int iReturnCode );

// THIS NUMBER TO BE ASSIGNED TO US FROM GLOBAL VR
int g_GameID = 0;

// registers ourselves with Elemental Engine, so we get messages
// it's not really a singleton to us, but to EE it is.
REGISTER_COMPONENT_SINGLETON(GameComponent);

REGISTER_MESSAGE_HANDLER(Exit, OnExit, GameComponent);
REGISTER_MESSAGE_HANDLER(AddPlayerContinue, OnAddPlayerContinue, GameComponent);
REGISTER_MESSAGE_HANDLER(UpdateHighScores, OnUpdateHighScores, GameComponent);
REGISTER_MESSAGE_HANDLER(GetCreditCount, OnGetCreditCount, GameComponent);
REGISTER_MESSAGE_HANDLER(DecrementCredits, OnDecrementCredits, GameComponent);


#ifdef _DEBUG
REGISTER_MESSAGE_HANDLER(MarkMemCheckPoint, OnMarkMemCheckPoint, GameComponent);
REGISTER_MESSAGE_HANDLER(CompareCheckPoint, OnCompareCheckPoint, GameComponent);
REGISTER_MESSAGE_HANDLER(EnableAllocHook, OnEnableAllocHook, GameComponent ); 
#endif


const string DEFAULT_Multiplier = "1";
const string DEFAULT_CreditStart = "1";
const string DEFAULT_CreditCont = "1";
const bool DEFAULT_Freeplay = false;
const string DEFAULT_PlayTime = "120";
const string DEFAULT_ContTime = "120";
const string DEFAULT_Language = "0";
const string DEFAULT_Violence = "0";

// get the singleton for this
IComponent *GameComponent::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE(GameComponent);
}

GameComponent::GameComponent()
{
	m_ToolBox = EngineGetToolBox();

	m_iMultiplier = _ttoi( DEFAULT_Multiplier.c_str() );
	m_iCreditStart = _ttoi( DEFAULT_CreditStart.c_str() );
	m_iCreditCont = _ttoi( DEFAULT_CreditCont.c_str() );
	m_bFreeplay = DEFAULT_Freeplay;
	m_iPlayTime = _ttoi( DEFAULT_PlayTime.c_str() );
	m_iContTime = _ttoi( DEFAULT_ContTime.c_str() );
	m_iLanguage = _ttoi( DEFAULT_Language.c_str() );
	m_iViolence = _ttoi( DEFAULT_Violence.c_str() );
	m_iPlayerContinues = 0;
}

GameComponent::~GameComponent()
{
}

bool GameComponent::Init()
{
	return true;
}

void GameComponent::Update()
{
}

bool GameComponent::DeInit()
{
	return true;
}

// serialize function for this object
void GameComponent::Serialize(IArchive &ar)
{
}

// get component name for this object
IHashString *GameComponent::GetComponentType()
{
	static CHashString compName(_T("GameComponent"));
	return &compName;
}

bool GameComponent::IsKindOf(IHashString *compType)
{
	return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
}

void GameComponent::ParseCommandLine( LPTSTR lpCmdLine )
{
	m_CmdLineParse.clear();
	if( _tcscmp( "", lpCmdLine ) != 0 )
	{
		CCmdLine CmdLine;
		CmdLine.SplitLine( lpCmdLine );

		if( CmdLine.HasSwitch( "-multiplier" ) )
		{
			m_iMultiplier = _ttoi( CmdLine.GetSafeArgument( "-multiplier", 0, DEFAULT_Multiplier.c_str() ).c_str() );
			StdString parseLine = _T("Cmd line argument: multiplier\n");
			m_CmdLineParse.push_back(parseLine);
		}

		if( CmdLine.HasSwitch( "-creditstart" ) )
		{
			m_iCreditStart = _ttoi( CmdLine.GetSafeArgument( "-creditstart", 0, DEFAULT_CreditStart.c_str() ).c_str() );
			StdString parseLine = _T("Cmd line argument: creditstart\n");
			m_CmdLineParse.push_back(parseLine);
		}

		if( CmdLine.HasSwitch( "-creditcont" ) )
		{
			m_iCreditCont = _ttoi( CmdLine.GetSafeArgument( "-creditcont", 0, DEFAULT_CreditCont.c_str() ).c_str() );
			StdString parseLine = _T("Cmd line argument: creditcont\n");
			m_CmdLineParse.push_back(parseLine);
		}

		if( CmdLine.HasSwitch( "-freeplay" ) )
		{
			m_bFreeplay = !(DEFAULT_Freeplay);
			StdString parseLine = _T("Cmd line argument: freeplay\n");
			m_CmdLineParse.push_back(parseLine);
		}
		else
		{
			m_bFreeplay = DEFAULT_Freeplay;
		}

		if( CmdLine.HasSwitch( "-time" ) )
		{
			m_iPlayTime = _ttoi( CmdLine.GetSafeArgument( "-time", 0, DEFAULT_PlayTime.c_str() ).c_str() );
			StdString parseLine = _T("Cmd line argument: time\n");
			m_CmdLineParse.push_back(parseLine);
		}

		if( CmdLine.HasSwitch( "-contime" ) )
		{
			m_iContTime = _ttoi( CmdLine.GetSafeArgument( "-contime", 0, DEFAULT_ContTime.c_str() ).c_str() );
			StdString parseLine = _T("Cmd line argument: contime\n");
			m_CmdLineParse.push_back(parseLine);
		}

		if( CmdLine.HasSwitch( "-language" ) )
		{
			m_iLanguage = _ttoi( CmdLine.GetSafeArgument( "-language", 0, DEFAULT_ContTime.c_str() ).c_str() );
			StdString parseLine = _T("Cmd line argument: language\n");
			m_CmdLineParse.push_back(parseLine);
		}

		if( CmdLine.HasSwitch( "-violence" ) )
		{
			m_iViolence = _ttoi( CmdLine.GetSafeArgument( "-violence", 0, DEFAULT_Violence.c_str() ).c_str() );
			StdString parseLine = _T("Cmd line argument: violence\n");
			m_CmdLineParse.push_back(parseLine);
		}

		// in free play mode some things need to be zeroed out
		if (m_bFreeplay)
		{
			m_iCreditStart = 0;
			m_iCreditCont = 0;
			m_iMultiplier = 0;			
		}
	}
}

void GameComponent::WriteGlobalAttributes()
{
	SetGlobalAttribute( &CHashString( _T("Multiplier") ), m_iMultiplier );
	SetGlobalAttribute( &CHashString( _T("CreditStart") ), m_iCreditStart );
	SetGlobalAttribute( &CHashString( _T("CreditCont") ), m_iCreditCont );
	SetGlobalAttribute( &CHashString( _T("FreePlay") ), m_bFreeplay );
	SetGlobalAttribute( &CHashString( _T("Playtime") ), m_iPlayTime );
	SetGlobalAttribute( &CHashString( _T("ContTime") ), m_iContTime );
	SetGlobalAttribute( &CHashString( _T("Language") ), m_iLanguage );
	SetGlobalAttribute( &CHashString( _T("Violence") ), m_iViolence );
}

void GameComponent::SetParent(CGFXApp *parent)
{
	m_GameApp = parent;
}	

DWORD GameComponent::OnExit(DWORD msgSize, void *param)
{
	if (m_GameApp)
	{
		m_GameApp->Quit();	
	}	
	return MSG_HANDLED_STOP;
}

void GameComponent::SetGlobalAttribute( IHashString *attributeName, IArchive *archive )
{
	if( attributeName != NULL && archive != NULL )
	{
		DATABASEATTRIBUTEPARAMS databaseAttributeParams;
		databaseAttributeParams.m_AttributeName = attributeName;
		databaseAttributeParams.m_AttributeArchive = archive;

		static DWORD msgHash_SetGlobalAttribute = CHashString(_T("SetGlobalAttribute")).GetUniqueID();
		m_ToolBox->SendMessage( msgHash_SetGlobalAttribute, sizeof(DATABASEATTRIBUTEPARAMS), &databaseAttributeParams );
	}
}

void GameComponent::GetGlobalAttribute( IHashString *attributeName, IArchive *archive )
{
	if( attributeName != NULL && archive != NULL )
	{
		DATABASEATTRIBUTEPARAMS databaseAttributeParams;
		databaseAttributeParams.m_AttributeName = attributeName;
		databaseAttributeParams.m_AttributeArchive = archive;

		static DWORD msgHash_GetGlobalAttribute = CHashString(_T("GetGlobalAttribute")).GetUniqueID();
		m_ToolBox->SendMessage( msgHash_GetGlobalAttribute, sizeof(DATABASEATTRIBUTEPARAMS), &databaseAttributeParams );
	}
}

void GameComponent::SetGlobalAttribute( IHashString *attributeName, int value )
{
	if( attributeName != NULL )
	{
		IArchive *archive = CreateMemoryArchive();
		archive->SetIsWriting( true );
		archive->Write( (int)value );
		archive->SetIsWriting( false );

		SetGlobalAttribute( attributeName, archive );

		archive->Close();
	}
}

void GameComponent::SetGlobalAttribute( IHashString *attributeName, bool value )
{
	if( attributeName != NULL )
	{
		IArchive *archive = CreateMemoryArchive();
		archive->SetIsWriting( true );
		archive->Write( (bool)value );
		archive->SetIsWriting( false );

		SetGlobalAttribute( attributeName, archive );

		archive->Close();
	}
}

IArchive *GameComponent::CreateMemoryArchive()
{
	CREATEARCHIVE ca;
	CHashString memType(_T("Memory"));

	ca.mode = STREAM_MODE_WRITE | STREAM_MODE_READ;
	ca.streamData = NULL;
	ca.streamSize = 0;
	ca.streamType = &memType;

	static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca);
	
	return ca.archive;
}

void GameComponent::InitGVRLibrary()
{
#ifdef _SHELL
	::Init( g_GameID, m_iPlayTime, m_iContTime, m_iMultiplier );
#endif
}

void GameComponent::ShutdownGVRLibrary()
{
#ifdef _SHELL
	::Shutdown();
#endif
}

int GameComponent::GetCreditCount()
{
#ifdef _SHELL
	// get hardware credits
	return ::GetCreditCount();	
#else
	// get software credits
	IArchive *archive = CreateMemoryArchive();
	if (archive)
	{
		archive->SetIsWriting(true);
		CHashString hsAttribName(_T("Credits"));
		GetGlobalAttribute(&hsAttribName, archive);

		archive->SetIsWriting(false);
		int credits = 0;
		archive->Read(credits, _T("Credits"));
		archive->Close();
		return credits;
	}

	return 0;

#endif
}
	
int GameComponent::DeductCredits( int credits )
{
#ifdef _SHELL
	// deduct hardware credits
	return ::DeductCredits( credits );
#else
	// deduct software credits
	IArchive *archive = CreateMemoryArchive();
	if (archive)
	{
		archive->SetIsWriting(true);
		CHashString hsAttribName(_T("Credits"));
		GetGlobalAttribute(&hsAttribName, archive);

		archive->SetIsWriting(false);
		int currCredits = 0;
		archive->Read(currCredits, _T("Credits"));
		
		currCredits -= credits;
        SetGlobalAttribute(&hsAttribName, currCredits);

		archive->Close();
	}

	return 0;
#endif
}

void GameComponent::WriteGameData()
{
#ifdef _SHELL
	// write continue stats
	::WriteStats( m_iPlayerContinues );

	// write score stuffs
	// TODO: figure out exactly what to display on high score screen
	// for now, let's say name and score
	TCHAR *scoreTex[2];
	TCHAR dummyName[] = _T("Blank Name");
	TCHAR score[32];
	_stprintf(score, _T("%d"), m_iPlayerScore);

	scoreTex[0] = dummyName;
	scoreTex[1] = score;

	::WriteGameResult( m_iPlayerScore,		// sorting index
						0,					// index of player name placeholder
						scoreTex,			// double array of strings
						2 );				// size of array
#endif
}

DWORD GameComponent::OnAddPlayerContinue(DWORD size, void *param)
{
	m_iPlayerContinues++;
	return MSG_HANDLED_STOP;
}

DWORD GameComponent::OnUpdateHighScores(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(DWORD));
	DWORD score = *(DWORD*)param;
	m_iPlayerScore = score;
	return MSG_HANDLED_STOP;
}

DWORD GameComponent::OnGetCreditCount(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(DWORD));
	DWORD *count = (DWORD*)param;
	*count = GetCreditCount();
	return MSG_HANDLED_STOP;
}

DWORD GameComponent::OnDecrementCredits(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(DWORD));
	DWORD decrement = *(DWORD*)param;
	DeductCredits((int)decrement);
	return MSG_HANDLED_STOP;
}

void GameComponent::LogCmdLineParse()
{
	for (UINT i=0; i<m_CmdLineParse.size(); i++)
	{
		m_ToolBox->Log(LOGINFORMATION, m_CmdLineParse[i]);
	}
}

#ifdef _SHELL
void GameComponent::InitGVRIO(HWND hwnd, MessageCallbackFN callback)
{
#ifdef _SHELL
	m_GvrIO.Initialize(JOYSTICK_DEVICE, hwnd, callback);
#endif
}
#endif

void GameComponent::PollGVRIO()
{
#ifdef _SHELL
	sMessage GvrIOmsg;
	int *pValue;
	int xAxis, yAxis, zAxis;

	GvrIOmsg.deviceType = ALL_DEVICE;	//Tell everything to update
	GvrIOmsg.handled = false;
	GvrIOmsg.msg = GVRIO_UPDATE;
	GvrIOMessageSend(&GvrIOmsg);

	GvrIOmsg.deviceType = ALL_DEVICE;
	GvrIOmsg.handled = false;
	GvrIOmsg.msg = GVRIO_GET_ENCODER_DELTA;
	GvrIOmsg.param1 = 0;	//X_AXIS
	pValue = (int *)GvrIOMessageSend(&GvrIOmsg);
	if(pValue)
	{
		xAxis = *pValue;
	}

	GvrIOmsg.handled = false;
	GvrIOmsg.param1 = 1;	//y_AXIS
	pValue = (int *)GvrIOMessageSend(&GvrIOmsg);
	if(pValue)
	{
		yAxis = *pValue;
	}
	GvrIOmsg.handled = false;
	GvrIOmsg.param1 = 2;	//Z_AXIS
	pValue = (int *)GvrIOMessageSend(&GvrIOmsg);
	if(pValue)
	{
		zAxis = *pValue;
	}

	static DWORD msgUpdateHeadsetAxes = CHashString(_T("UpdateHeadsetAxes")).GetUniqueID();
	Vec3 axes((float)xAxis, (float)yAxis, (float)zAxis);

	m_ToolBox->SendMessage(msgUpdateHeadsetAxes, sizeof(Vec3), &axes);
#endif
}

void GameComponent::ShutdownGVRIO()
{
#ifdef _SHELL
//	m_GvrIO.Finalize();
#endif
}

#ifdef _DEBUG
DWORD GameComponent::OnMarkMemCheckPoint(DWORD size, void *param)
{
	_CrtMemCheckpoint( &m_MemState );

	return MSG_HANDLED_STOP;
}

DWORD GameComponent::OnCompareCheckPoint(DWORD size, void *param)
{
	_CrtMemDumpAllObjectsSince( &m_MemState );
	
	return MSG_HANDLED_STOP;
}

extern bool g_bEnableAllocHook;

DWORD GameComponent::OnEnableAllocHook(DWORD size, void *param)
{
	g_bEnableAllocHook = !g_bEnableAllocHook;
	
	return MSG_HANDLED_STOP;
}
#endif