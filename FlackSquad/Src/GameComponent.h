///==========================================================================
/// \file	GameComponent.h
/// \brief	Component that listens for singleton messages for generic game app
/// \date	11/16/2006
/// \author	Nick Tourte
///==========================================================================

#ifndef	_GAMECOMPONENT_H_
#define	_GAMECOMPONENT_H_

#ifdef _SHELL
#include "GvrIO.h"
using namespace Gvr::Zeus::GvrIO;
#endif

class CGFXApp;

class GameComponent : public IComponent
{
private:
	SINGLETONCONSTRUCTOROVERRIDE( GameComponent );
	GameComponent();

public:
	/// \brief	Default Destructor
	~GameComponent( );

	static IComponent *Create(int nArgs, va_list argptr);

	/// \brief	Initializes the AI Manager and AI System
	/// \return	True on sucess, False on failure
	virtual bool Init();

	/// \brief	Update function for the AI Manager and AI System
	virtual void Update();
	
	/// \brief	DeInitializes the AI Manager and AI System
	/// \return	True on sucess, False on failure
	virtual bool DeInit();

	/// Serialize this component to the archive.
	///	\param ar = reference to the archive class
	virtual void Serialize(IArchive &ar);

	/// return a string of the component type
	/// \return IHashString *reference to textual classname
	virtual IHashString *GetComponentType();

	/// Checks if this is similar to it's base classes component
	/// \return true if it is simlar, else false
	virtual bool IsKindOf(IHashString *compType);
	
	/// Hook the parent app to this component
	void SetParent(CGFXApp *parent);

	/// Parse Command Line
	void ParseCommandLine( LPTSTR lpCmdLine );

	/// Writes the parsed command line values to the global attributes
	void WriteGlobalAttributes();

	/// Exits the app
	/// \param size = 0
	/// \param param = NULL
	/// \return MSG_HANDLED_STOP
	DWORD OnExit(DWORD size, void *param);

	/// Adds a continue
	/// \param size = 0
	/// \param param = NULL
	/// \return MSG_HANDLED_STOP;
	DWORD OnAddPlayerContinue(DWORD size, void *param);

	/// Updates this player's score before we save it into the high scores
	/// \param size = sizeof(DWORD)
	/// \param param = DWORD with score
	/// \return MSG_HANDLED_STOP;
	DWORD OnUpdateHighScores(DWORD size, void *param);

	/// Gets the current credit count (whether software or hardware)
	/// \param size = sizeof(DWORD)
	/// \param param = DWORD to get out credit count
	/// \return MSG_HANDLED_STOP;
	DWORD OnGetCreditCount(DWORD size, void *param);

	/// Removes credits from hardware or software
	/// \param size = sizeof(DWORD)
	/// \param param = DWORD with credits to remove
	/// \return MSG_HANDLED_STOP;
	DWORD OnDecrementCredits(DWORD size, void *param);

	void InitGVRLibrary();
	void ShutdownGVRLibrary();
	int GetCreditCount();
	int DeductCredits( int credits );	
	void WriteGameData();
	void LogCmdLineParse();
#ifdef _SHELL
	void InitGVRIO(HWND hwnd, MessageCallbackFN callback);
#endif
	void PollGVRIO();
	void ShutdownGVRIO();
	int GetStartCreditCount() { return m_iCreditStart; }

#ifdef _DEBUG
	DWORD OnMarkMemCheckPoint(DWORD size, void *param);
	DWORD OnCompareCheckPoint(DWORD size, void *param);
	DWORD OnEnableAllocHook(DWORD size, void *param);
	_CrtMemState m_MemState;
#endif

private:
	void GetGlobalAttribute( IHashString *attributeName, IArchive *archive );

	void SetGlobalAttribute( IHashString *attributeName, IArchive *archive );

	void SetGlobalAttribute( IHashString *attributeName, int value );

	void SetGlobalAttribute( IHashString *attributeName, bool value );

	IArchive *CreateMemoryArchive();

	IToolBox* m_ToolBox;

	CGFXApp* m_GameApp;

	/* GloablVR Arguments */
	/// multiplier applied to each coin drop to calculate the real number of credits
	/// -multiplier XXX
	int m_iMultiplier;

	/// amount of credits it takes to start a new game
	/// -creditstart XXX
	int m_iCreditStart;

	/// amount of credits it takes to continue a new game
	/// -creditcont XXX
	int m_iCreditCont;

	/// disables any credit counting (ignores -multiplier, -creditstart, -creditcont)
	/// -freeplay
	bool m_bFreeplay;

	/// amount of time to give the player on starting a game (in seconds)
	/// -time XXX
	int m_iPlayTime;

	/// amount of time to give the player on a continue (in seconds)
	/// -continue XXX
	int m_iContTime;

	/// the language to use
	/// -language XXX
	int m_iLanguage;

	/// the violence setting (0=light, 1=normal, 2=heavy)
	/// -violence XXX
	int m_iViolence;

	/// number of continues player has 'purchased'
	int m_iPlayerContinues;

	/// player's score
	DWORD m_iPlayerScore;

	vector<StdString> m_CmdLineParse;

#ifdef _SHELL
	cGvrIO m_GvrIO;
#endif
};

#endif	// _GAMECOMPONENT_H_