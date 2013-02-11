///==========================================================================
/// \file	FlackSquadComponent.h
/// \brief	Component that listens for singleton messages for FlackSquad
///
/// \date	09-13-2006
/// \author	Kyle Swaim
///==========================================================================

#ifndef	_FLACKSQUADCOMPONENT_H_
#define	_FLACKSQUADCOMPONENT_H_

// tolua_begin
struct FLACKSQUAD_GETPLAYERINPUT
{
	int iPlayerID;
	Vec3 v3StickA;
	Vec3 v3StickB;
};
// tolua_end

class GFXApp;

class FlackSquadComponent : public IComponent
{
private:
	SINGLETONCONSTRUCTOROVERRIDE( FlackSquadComponent );
	FlackSquadComponent();

public:

	/// \brief	Default Destructor
	~FlackSquadComponent( );

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
	
	/// Hook the the progress bar's to this component
//	void SetParent(CGFXApp *parent);

	/// Registers lua definitions
	void RegisterLuaDefinitions();

	/// \brief	Initialize the key bindings
	void InitKeyBindings();

	///	\brief	Deinitialize the key bindings
	void DeInitKeyBindings();

	/// \brief	Pause
	DWORD OnPause(DWORD msgSize, void *param);

	///	\brief	On middle mouse press
	DWORD OnMiddleMousePress(DWORD msgSize, void *param);

	///	\brief	On middle mouse release
	DWORD OnMiddleMouseRelease(DWORD msgSize, void *param);

	///	\brief	On mouse move
	DWORD OnMouseMove(DWORD msgSize, void *param);

	DWORD OnGetPlayerInput( DWORD msgSize, void *param );

	////////////////////////////////////////////////////////////////////////////////
	/// \brief Camera movement
	DWORD OnCamMoveForward(DWORD msgSize, void *param);

	/// \brief Camera movement
	DWORD OnCamMoveBackward(DWORD msgSize, void *param);

	/// \brief Camera movement
	DWORD OnCamMoveLeft(DWORD msgSize, void *param);

	/// \brief Camera movement
	DWORD OnCamMoveRight(DWORD msgSize, void *param);

	/////////////////////////////////////////////////////////////////////////////////
	
	DWORD OnRadialDirectionX( DWORD msgSize, void *param );

	DWORD OnRadialDirectionY( DWORD msgSize, void *param );

	DWORD OnRotationalDirectionX( DWORD msgSize, void *param );

	DWORD OnRotationalDirectionY( DWORD msgSize, void *param );

	DWORD OnEndMission(DWORD msgSize, void *param);

	DWORD OnPlaneExplode(DWORD msgSize, void *param);

	DWORD OnGotoMenu(DWORD msgSize, void *param);

	DWORD OnESCKeyPress(DWORD msgSize, void *param);

	DWORD OnEnablePostProcessGlow( DWORD msgSize, void * param);
	DWORD OnDisablePostProcessGlow( DWORD msgSize, void * param);	

	DWORD OnEnableBWPostProcess(DWORD size, void *param);
	DWORD OnDisableBWPostProcess(DWORD size, void *param);

	DWORD OnLogMessage(DWORD size, void *param);

	DWORD OnProfilerOnOff(DWORD size, void *param);

	DWORD OnPhysicsVisualizationOnOff(DWORD size, void* param);

	DWORD OnWireframeOnOff(DWORD size, void *param);

	DWORD OnDisplayFPS(DWORD size, void *param);

	DWORD OnNullRendererOnOff( DWORD size, void* param );

	DWORD OnEnableContinueMode(DWORD size, void *param);
	DWORD OnDisableContinueMode(DWORD size, void *param);

	DWORD OnToggleCameraMode(DWORD size, void *param);

	DWORD OnInitGameKeybindings(DWORD size, void *param);

	DWORD OnUpArrowPress(DWORD size, void *param);
	DWORD OnDownArrowPress(DWORD size, void *param);
	DWORD OnLeftArrowPress(DWORD size, void *param);
	DWORD OnRightArrowPress(DWORD size, void *param);
	DWORD OnSpaceKeyPress(DWORD size, void *param);
	DWORD OnQKeyPress(DWORD size, void *param);
	DWORD OnEKeyPress(DWORD size, void *param);
	DWORD OnFKeyPress(DWORD size, void *param);
	DWORD OnRKeyPress(DWORD size, void *param);


	DWORD OnLeftMousePress(DWORD size, void *param);
	DWORD OnLeftMouseRelease(DWORD size, void *param);
	DWORD OnRightMousePress(DWORD size, void *param);
	DWORD OnRightMouseRelease(DWORD size, void *param);
	DWORD OnMouseWheel(DWORD size, void *param);

	DWORD OnAButton(DWORD size, void *param);
	DWORD OnBButton(DWORD size, void *param);
	DWORD OnXButton(DWORD size, void *param);
	DWORD OnYButton(DWORD size, void *param);
	DWORD OnBackButton(DWORD size, void *param);
	DWORD OnStartButton(DWORD size, void *param);
	DWORD OnLBButton(DWORD size, void *param);
	DWORD OnRBButton(DWORD size, void *param);
	DWORD OnLeftTrigger(DWORD size, void *param);
	DWORD OnRightTrigger(DWORD size, void *param);
	DWORD OnLeftAnalogPress(DWORD size, void *param);
	DWORD OnDPadUp(DWORD size, void *param);
	DWORD OnDPadDown(DWORD size, void *param);
	DWORD OnDPadLeft(DWORD size, void *param);
	DWORD OnDPadRight(DWORD size, void *param);

	DWORD OnSetLeftMotorSpeed(DWORD size, void *param);
	DWORD OnSetRightMotorSpeed(DWORD size, void *param);
	
	DWORD OnGetPlayerAxisSensitivity(DWORD size, void *param);
	DWORD OnSetPlayerAxisSensitivity(DWORD size, void *param);

	DWORD OnJoystickConnected(DWORD size, void *param);
	DWORD OnJoystickDisconnected(DWORD size, void *param);

	////////LUA MESSAGE HANDLERS
	// They are used to differenciate the controls in menu and during game.
	// On the Joy: A button and RT buttons are used as main functions in game and menu.
	// But the mouse should have only one button as main action - the left mouse button.
	// In menu: LeftClick calls ButtonA message handler.
	// In game: LeftClick calls ButtonRT message handler.
	DWORD OnShowMenu(DWORD size, void *param);		// Called if menu gets visible
	DWORD OnHideMenu(DWORD size, void *param);		// Called if menu gets hidden
	DWORD OnStartMission(DWORD size, void *param);		// Called on mission start - used to reset variables.

public:
	void	RotateCamera( POINT &delta);
	void	MoveCameraVectorZ(float fZDelta);
	void	StrafeCamera( POINT &delta);

private:

	void SetJoystickBinding(DWORD key, DWORD msg, int filter, bool repeat);

	IToolBox* m_ToolBox;
	CPlayer m_Player;

	// camera movement vars
	float m_fCameraSpeed;
	float m_fCameraSensitivity;
	float m_fCameraSensitivity2;

	bool m_bIsMenuShown;		// True if the menu is visible.
	bool m_bPaused;
	bool m_bProfilerOn;
	bool m_bPhysVisOn;
	bool m_bShowFpsDisplay; /// True if the FPS display is shown.

	long m_MDragStartPointX;
	long m_MDragStartPointY;

	long m_lCursorPosX;
	long m_lCursorPosY;

	/// camera movement variables
	CHashString m_hszCameraName;
	float m_fCameraPitch, m_fCameraYaw;
	float m_fCameraZSpeed;

	// logging file
	FILE *m_LogFile;

	/// wireframe mode
	bool m_bWireframeMode;

	/// continue screen?
	bool m_bContinueMode;

	/// null renderer enable/disable
	bool m_bNullRenderer;

	/// indicator to make only one step in the menu on keypress
	bool m_bFirstMenuKeyPress;

	/// camera locked to pos (t) or free (f)
	bool m_bCamLocked; 	
	IEffect *m_pGlowPostProcessEffect;
	IEffect *m_pBWPostProcessEffect;

	CHashString m_hszCameraType;

	// handling multiple keys to enhance keyboard  contols:
	POINT deltaMouse;

	/// message hander object to return rendering information
//	CGUIFPSDisplay m_pGuiFpsDisplay;
};

#endif	// _FLACKSQUADCOMPONENT_H_