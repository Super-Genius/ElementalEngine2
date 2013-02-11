///==========================================================================
/// \file	FlackSquadComponent.cpp
/// \brief	Component that listens for singleton messages for FlackSquad
/// \date	07-29-05
/// \author	Halbert Nakagawa
///==========================================================================

#include "StdAfx.h"
#include "Vec3.h"

void TerminateApp( int iReturnCode );

// registers ourselves with Elemental Engine, so we get messages
// it's not really a singleton to us, but to EE it is.
REGISTER_COMPONENT_SINGLETON(FlackSquadComponent);

// messag handlers for keyboard and mouse input
REGISTER_MESSAGE_HANDLER(UpArrow, OnUpArrowPress, FlackSquadComponent);
REGISTER_MESSAGE_HANDLER(DownArrow, OnDownArrowPress, FlackSquadComponent);
REGISTER_MESSAGE_HANDLER(LeftArrow, OnLeftArrowPress, FlackSquadComponent);
REGISTER_MESSAGE_HANDLER(RightArrow, OnRightArrowPress, FlackSquadComponent);

REGISTER_MESSAGE_HANDLER(SpaceKeyPress, OnSpaceKeyPress, FlackSquadComponent);
REGISTER_MESSAGE_HANDLER(ESCKeyPress, OnESCKeyPress, FlackSquadComponent);
REGISTER_MESSAGE_HANDLER(KeyPressQ, OnQKeyPress, FlackSquadComponent);
REGISTER_MESSAGE_HANDLER(KeyPressE, OnEKeyPress, FlackSquadComponent);
REGISTER_MESSAGE_HANDLER(KeyPressF, OnFKeyPress, FlackSquadComponent);
REGISTER_MESSAGE_HANDLER(KeyPressR, OnRKeyPress, FlackSquadComponent);

REGISTER_MESSAGE_HANDLER(MiddleMousePress, OnMiddleMousePress, FlackSquadComponent);
REGISTER_MESSAGE_HANDLER(MiddleMouseRelease, OnMiddleMouseRelease, FlackSquadComponent);
REGISTER_MESSAGE_HANDLER(MouseMove, OnMouseMove, FlackSquadComponent);
REGISTER_MESSAGE_HANDLER(LeftMousePress, OnLeftMousePress, FlackSquadComponent);
REGISTER_MESSAGE_HANDLER(LeftMouseRelease, OnLeftMouseRelease, FlackSquadComponent);
REGISTER_MESSAGE_HANDLER(RightMousePress, OnRightMousePress, FlackSquadComponent);
REGISTER_MESSAGE_HANDLER(RightMouseRelease, OnRightMouseRelease, FlackSquadComponent);
REGISTER_MESSAGE_HANDLER(Scroll, OnMouseWheel, FlackSquadComponent);

REGISTER_MESSAGE_HANDLER(GetPlayerInput, OnGetPlayerInput, FlackSquadComponent);
REGISTER_MESSAGE_HANDLER(Pause, OnPause, FlackSquadComponent);
REGISTER_MESSAGE_HANDLER(cam_move_forward, OnCamMoveForward, FlackSquadComponent);
REGISTER_MESSAGE_HANDLER(cam_move_backward, OnCamMoveBackward, FlackSquadComponent);
REGISTER_MESSAGE_HANDLER(cam_strafe_left, OnCamMoveLeft, FlackSquadComponent);
REGISTER_MESSAGE_HANDLER(cam_strafe_right, OnCamMoveRight, FlackSquadComponent);
REGISTER_MESSAGE_HANDLER(EndMission, OnEndMission, FlackSquadComponent);
REGISTER_MESSAGE_HANDLER(PlaneExplode, OnPlaneExplode, FlackSquadComponent);
REGISTER_MESSAGE_HANDLER(GotoMenu, OnGotoMenu, FlackSquadComponent);
REGISTER_MESSAGE_HANDLER(ProfilerOnOff, OnProfilerOnOff, FlackSquadComponent);
REGISTER_MESSAGE_HANDLER(PhysicsVisualizationOnOff, OnPhysicsVisualizationOnOff, FlackSquadComponent);
REGISTER_MESSAGE_HANDLER(WireframeOnOff, OnWireframeOnOff, FlackSquadComponent);
REGISTER_MESSAGE_HANDLER(NullRendererOnOff, OnNullRendererOnOff, FlackSquadComponent);
REGISTER_MESSAGE_HANDLER(EnableContinueMode, OnEnableContinueMode, FlackSquadComponent);
REGISTER_MESSAGE_HANDLER(DisableContinueMode, OnDisableContinueMode, FlackSquadComponent);
REGISTER_MESSAGE_HANDLER(ToggleCameraMode, OnToggleCameraMode, FlackSquadComponent);
REGISTER_MESSAGE_HANDLER(InitGameKeybindings, OnInitGameKeybindings, FlackSquadComponent);
REGISTER_MESSAGE_HANDLER_PRIORITY(LogMessage, OnLogMessage, FlackSquadComponent, 0x70000000);
REGISTER_MESSAGE_HANDLER(EnablePostProcessGlow, OnEnablePostProcessGlow, FlackSquadComponent);
REGISTER_MESSAGE_HANDLER(DisablePostProcessGlow, OnDisablePostProcessGlow, FlackSquadComponent);
REGISTER_MESSAGE_HANDLER(EnableBWPostProcess, OnEnableBWPostProcess, FlackSquadComponent);
REGISTER_MESSAGE_HANDLER(DisableBWPostProcess, OnDisableBWPostProcess, FlackSquadComponent);
REGISTER_MESSAGE_HANDLER(DisplayFPS, OnDisplayFPS, FlackSquadComponent);



REGISTER_MESSAGE_HANDLER(GetPlayerAxisSensitivity, OnGetPlayerAxisSensitivity, FlackSquadComponent);
REGISTER_MESSAGE_HANDLER(SetPlayerAxisSensitivity, OnSetPlayerAxisSensitivity, FlackSquadComponent);

// Joystick message handlers

REGISTER_MESSAGE_HANDLER(RadialDirectionX, OnRadialDirectionX, FlackSquadComponent);
REGISTER_MESSAGE_HANDLER(RadialDirectionY, OnRadialDirectionY, FlackSquadComponent);
REGISTER_MESSAGE_HANDLER(RotationalDirectionX, OnRotationalDirectionX, FlackSquadComponent);
REGISTER_MESSAGE_HANDLER(RotationalDirectionY, OnRotationalDirectionY, FlackSquadComponent);

REGISTER_MESSAGE_HANDLER(AButton, OnAButton, FlackSquadComponent);
REGISTER_MESSAGE_HANDLER(BButton, OnBButton, FlackSquadComponent);
REGISTER_MESSAGE_HANDLER(XButton, OnXButton, FlackSquadComponent);
REGISTER_MESSAGE_HANDLER(YButton, OnYButton, FlackSquadComponent);
REGISTER_MESSAGE_HANDLER(BackButton, OnBackButton, FlackSquadComponent);
REGISTER_MESSAGE_HANDLER(StartButton, OnStartButton, FlackSquadComponent);
REGISTER_MESSAGE_HANDLER(LBButton, OnLBButton, FlackSquadComponent);
REGISTER_MESSAGE_HANDLER(RBButton, OnRBButton, FlackSquadComponent);
REGISTER_MESSAGE_HANDLER(LeftTrigger, OnLeftTrigger, FlackSquadComponent);
REGISTER_MESSAGE_HANDLER(RightTrigger, OnRightTrigger, FlackSquadComponent);
REGISTER_MESSAGE_HANDLER(LeftAnalogPress, OnLeftAnalogPress, FlackSquadComponent);
REGISTER_MESSAGE_HANDLER(DPadUp, OnDPadUp, FlackSquadComponent);
REGISTER_MESSAGE_HANDLER(DPadDown, OnDPadDown, FlackSquadComponent);
REGISTER_MESSAGE_HANDLER(DPadLeft, OnDPadLeft, FlackSquadComponent);
REGISTER_MESSAGE_HANDLER(DPadRight, OnDPadRight, FlackSquadComponent);

REGISTER_MESSAGE_HANDLER(SetLeftMotorSpeed, OnSetLeftMotorSpeed, FlackSquadComponent);
REGISTER_MESSAGE_HANDLER(SetRightMotorSpeed, OnSetRightMotorSpeed, FlackSquadComponent);

REGISTER_MESSAGE_HANDLER(JoystickConnected, OnJoystickConnected, FlackSquadComponent);
REGISTER_MESSAGE_HANDLER(JoystickDisconnected, OnJoystickDisconnected, FlackSquadComponent);

// LUA MESSAGES
REGISTER_MESSAGE_HANDLER( ShowMenu, OnShowMenu, FlackSquadComponent );
REGISTER_MESSAGE_HANDLER( HideMenu, OnHideMenu, FlackSquadComponent );
REGISTER_MESSAGE_HANDLER( StartMission, OnStartMission, FlackSquadComponent );

// get the singleton for this
IComponent *FlackSquadComponent::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE(FlackSquadComponent);
}

FlackSquadComponent::FlackSquadComponent()
{
	m_ToolBox = EngineGetToolBox();
	m_LogFile = NULL;

	m_bPaused = false;
	m_bIsMenuShown = true;
	m_bProfilerOn = false;
	m_bPhysVisOn = false;
	m_bWireframeMode = false;
	m_bNullRenderer = false;
	m_bContinueMode = false;
	m_bFirstMenuKeyPress = true;
	m_bCamLocked = true;
	m_fCameraSensitivity2 = 10.0f;
	m_bShowFpsDisplay = false;

	m_hszCameraName = _T("");
	m_hszCameraType = _T("CCamera");
	m_pGlowPostProcessEffect = NULL;
	m_pBWPostProcessEffect = NULL;

	RegisterLuaDefinitions();
}

FlackSquadComponent::~FlackSquadComponent()
{
	if (m_LogFile != NULL)
	{
		fclose(m_LogFile);
	}
	DeInitKeyBindings();
}

bool FlackSquadComponent::Init()
{	
	// init our keyboard control variable
	deltaMouse.x = 0;
	deltaMouse.y = 0;

	return true;
}

void FlackSquadComponent::Update()
{		
}

bool FlackSquadComponent::DeInit()
{	
	return true;
}

// serialize function for this object
void FlackSquadComponent::Serialize(IArchive &ar)
{
}

// get component name for this object
IHashString *FlackSquadComponent::GetComponentType()
{
	static CHashString compName(_T("FlackSquadComponent"));
	return &compName;
}

bool FlackSquadComponent::IsKindOf(IHashString *compType)
{
	return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
}
/*
void FlackSquadComponent::SetParent(CGFXApp *parent)
{
	m_FlackSquadApp = parent;
}	
*/

TOLUA_API int luaopen_FlackSquadScript (lua_State* tolua_S);
void FlackSquadComponent::RegisterLuaDefinitions()
{
	lua_State *pLuaState;
	static DWORD msgHash_GetMasterScriptState = CHashString(_T( "GetMasterScriptState" )).GetUniqueID();
	DWORD amessageHandle = m_ToolBox->SendMessage(msgHash_GetMasterScriptState, sizeof( lua_State * ), &pLuaState );
	if( amessageHandle != MSG_HANDLED)
	{
		StdString error( "Error missing Master Script State Data\n" );

		// log error
		EngineGetToolBox()->SetErrorValue(WARN_INVALID_OPERATION);
		EngineGetToolBox()->Log(LOGWARNING, error);
    }
	else
	{
		// register our additional structures/handlers with LUA master
		luaopen_FlackSquadScript(pLuaState);
	}
}

void FlackSquadComponent::InitKeyBindings()
{
	KEYBIND keybind;
	CHashString upArrow(_T("UpArrow"));
	CHashString downArrow(_T("DownArrow"));
	CHashString leftArrow(_T("LeftArrow"));
	CHashString rightArrow(_T("RightArrow"));
	CHashString exit(_T("Exit"));
	CHashString pause(_T("Pause"));
	CHashString addCredit(_T("AddCredit"));
	CHashString fps_move_forward(_T("cam_move_forward"));
	CHashString fps_move_backward(_T("cam_move_backward"));
	CHashString fps_strafe_left(_T("cam_strafe_left"));
	CHashString fps_strafe_right(_T("cam_strafe_right"));
	CHashString reloadshaders(_T("ReloadAllShaders"));
	CHashString savecubes(_T("SaveAllCalCubes"));
	CHashString GotoMenu(_T("GotoMenu"));
	CHashString ESCKeyPress(_T("ESCKeyPress"));
	CHashString SpaceKeyPress(_T("SpaceKeyPress"));
  
	CHashString keyPressQ(_T("KeyPressQ"));
	CHashString keyPressE(_T("KeyPressE"));
	CHashString keyPressF(_T("KeyPressF"));
	CHashString keyPressR(_T("KeyPressR"));

	CHashString profilerOnOff(_T("ProfilerOnOff"));
	CHashString PhysicsVisualizationOnOff(_T("PhysicsVisualizationOnOff"));
	CHashString wireframeOnOff(_T("WireframeOnOff"));
	CHashString nullRendererOnOff(_T("NullRendererOnOff"));

	// Seems to be not in use
//	CHashString turret_fire(_T("FireTurret"));
//	CHashString turret_fire_second(_T("FireTurretSecondary"));


	CHashString end_mission(_T("EndMission"));

	CHashString cameraToggle(_T("ToggleCameraMode"));

	CHashString moveProfilerUp(_T("MoveProfilerUp"));
	CHashString moveProfilerDown(_T("MoveProfilerDown"));
	CHashString DisplayFPS(_T("DisplayFPS"));

	static DWORD msgSetKeyBinding = CHashString( _T("SetKeyBinding") ).GetUniqueID();
	
	CHashString kbName(_T("EEDemo_kb"));

	// setup camera params
	m_fCameraSpeed = 5.0f;
	m_fCameraSensitivity = 2.0f;
	m_MDragStartPointX = 0;
	m_MDragStartPointY = 0;
	m_lCursorPosX = 0;
	m_lCursorPosY = 0;


	//setup keys
	keybind.m_kbNameID = kbName.GetUniqueID();
	keybind.m_KeyID = EEK_ESC;
	keybind.m_MessageID = ESCKeyPress.GetUniqueID();
	keybind.m_bHitOnce = true;
	m_ToolBox->SendMessage(msgSetKeyBinding, sizeof(KEYBIND), &keybind);

	keybind.m_kbNameID = kbName.GetUniqueID();
	keybind.m_KeyID = EEK_LSHIFT;
	keybind.m_MessageID = addCredit.GetUniqueID();
	keybind.m_bHitOnce = true;
	m_ToolBox->SendMessage(msgSetKeyBinding, sizeof(KEYBIND), &keybind);

	keybind.m_kbNameID = kbName.GetUniqueID();
	keybind.m_KeyID = EEK_UP;
	keybind.m_MessageID = upArrow.GetUniqueID();
	keybind.m_bHitOnce = false;
	keybind.m_cBindType = KEYBIND_PRESSED | KEYBIND_RELEASED;
	m_ToolBox->SendMessage(msgSetKeyBinding, sizeof(KEYBIND), &keybind);

	keybind.m_kbNameID = kbName.GetUniqueID();
	keybind.m_KeyID = EEK_DOWN;
	keybind.m_MessageID = downArrow.GetUniqueID();
	keybind.m_bHitOnce = false;
	m_ToolBox->SendMessage(msgSetKeyBinding, sizeof(KEYBIND), &keybind);

	keybind.m_kbNameID = kbName.GetUniqueID();
	keybind.m_KeyID = EEK_LEFT;
	keybind.m_MessageID = leftArrow.GetUniqueID();
	keybind.m_bHitOnce = false;
	keybind.m_cBindType = KEYBIND_PRESSED | KEYBIND_RELEASED;
	m_ToolBox->SendMessage(msgSetKeyBinding, sizeof(KEYBIND), &keybind);

	keybind.m_kbNameID = kbName.GetUniqueID();
	keybind.m_KeyID = EEK_RIGHT;
	keybind.m_MessageID = rightArrow.GetUniqueID();
	keybind.m_bHitOnce = false;
	keybind.m_cBindType = KEYBIND_PRESSED | KEYBIND_RELEASED;
	m_ToolBox->SendMessage(msgSetKeyBinding, sizeof(KEYBIND), &keybind);

	keybind.m_kbNameID = kbName.GetUniqueID();
	keybind.m_KeyID = EEK_W;
	keybind.m_MessageID = upArrow.GetUniqueID();
	keybind.m_bHitOnce = false;
	keybind.m_cBindType = KEYBIND_PRESSED | KEYBIND_RELEASED;
	m_ToolBox->SendMessage(msgSetKeyBinding, sizeof(KEYBIND), &keybind);

	keybind.m_kbNameID = kbName.GetUniqueID();
	keybind.m_KeyID = EEK_S;
	keybind.m_MessageID = downArrow.GetUniqueID();
	keybind.m_bHitOnce = false;
	keybind.m_cBindType = KEYBIND_PRESSED | KEYBIND_RELEASED;
	m_ToolBox->SendMessage(msgSetKeyBinding, sizeof(KEYBIND), &keybind);

	keybind.m_kbNameID = kbName.GetUniqueID();
	keybind.m_KeyID = EEK_A;
	keybind.m_MessageID = leftArrow.GetUniqueID();
	keybind.m_bHitOnce = false;
	keybind.m_cBindType = KEYBIND_PRESSED | KEYBIND_RELEASED;
	m_ToolBox->SendMessage(msgSetKeyBinding, sizeof(KEYBIND), &keybind);

	keybind.m_kbNameID = kbName.GetUniqueID();
	keybind.m_KeyID = EEK_D;
	keybind.m_MessageID = rightArrow.GetUniqueID();
	keybind.m_bHitOnce = false;
	keybind.m_cBindType = KEYBIND_PRESSED | KEYBIND_RELEASED;
	m_ToolBox->SendMessage(msgSetKeyBinding, sizeof(KEYBIND), &keybind);

	//TODO: New keys to camera movement
	//keybind.m_kbNameID = kbName.GetUniqueID();
	//keybind.m_KeyID = EEK_W;
	//keybind.m_MessageID = fps_move_forward.GetUniqueID();
	//keybind.m_bHitOnce = false;
	//m_ToolBox->SendMessage(msgSetKeyBinding, sizeof(KEYBIND), &keybind);

	//keybind.m_kbNameID = kbName.GetUniqueID();
	//keybind.m_KeyID = EEK_S;
	//keybind.m_MessageID = fps_move_backward.GetUniqueID();
	//keybind.m_bHitOnce = false;
	//m_ToolBox->SendMessage(msgSetKeyBinding, sizeof(KEYBIND), &keybind);

	//keybind.m_kbNameID = kbName.GetUniqueID();
	//keybind.m_KeyID = EEK_A;
	//keybind.m_MessageID = fps_strafe_left.GetUniqueID();
	//keybind.m_bHitOnce = false;
	//m_ToolBox->SendMessage(msgSetKeyBinding, sizeof(KEYBIND), &keybind);

	//keybind.m_kbNameID = kbName.GetUniqueID();
	//keybind.m_KeyID = EEK_D;
	//keybind.m_MessageID = fps_strafe_right.GetUniqueID();
	//keybind.m_bHitOnce = false;
	//m_ToolBox->SendMessage(msgSetKeyBinding, sizeof(KEYBIND), &keybind);

	keybind.m_kbNameID = kbName.GetUniqueID();
	keybind.m_KeyID = EEK_SPACE;
	keybind.m_MessageID = SpaceKeyPress.GetUniqueID();
	keybind.m_bHitOnce = true;
	keybind.m_bReleaseOnce = true;
	keybind.m_cBindType = KEYBIND_PRESSED | KEYBIND_RELEASED;
	m_ToolBox->SendMessage(msgSetKeyBinding, sizeof(KEYBIND), &keybind);

	keybind.m_kbNameID = kbName.GetUniqueID();
	keybind.m_KeyID = EEK_ENTER;
	keybind.m_MessageID = SpaceKeyPress.GetUniqueID();
	keybind.m_bHitOnce = true;
	keybind.m_bReleaseOnce = true;
	keybind.m_cBindType = KEYBIND_PRESSED | KEYBIND_RELEASED;
	m_ToolBox->SendMessage(msgSetKeyBinding, sizeof(KEYBIND), &keybind);

	keybind.m_kbNameID = kbName.GetUniqueID();
	keybind.m_KeyID = EEK_Q;
	keybind.m_MessageID = keyPressQ.GetUniqueID();
	keybind.m_bHitOnce = true;
	keybind.m_bReleaseOnce = true;
	keybind.m_cBindType = KEYBIND_PRESSED;
	m_ToolBox->SendMessage(msgSetKeyBinding, sizeof(KEYBIND), &keybind);

	keybind.m_kbNameID = kbName.GetUniqueID();
	keybind.m_KeyID = EEK_E;
	keybind.m_MessageID = keyPressE.GetUniqueID();
	keybind.m_bHitOnce = true;
	keybind.m_bReleaseOnce = true;
	keybind.m_cBindType = KEYBIND_PRESSED;
	m_ToolBox->SendMessage(msgSetKeyBinding, sizeof(KEYBIND), &keybind);

	keybind.m_kbNameID = kbName.GetUniqueID();
	keybind.m_KeyID = EEK_F;
	keybind.m_MessageID = keyPressF.GetUniqueID();
	keybind.m_bHitOnce = true;
	keybind.m_bReleaseOnce = true;
	keybind.m_cBindType = KEYBIND_PRESSED | KEYBIND_RELEASED;
	m_ToolBox->SendMessage(msgSetKeyBinding, sizeof(KEYBIND), &keybind);

	keybind.m_kbNameID = kbName.GetUniqueID();
	keybind.m_KeyID = EEK_R;
	keybind.m_MessageID = keyPressR.GetUniqueID();
	keybind.m_bHitOnce = true;
	keybind.m_cBindType = KEYBIND_PRESSED | KEYBIND_RELEASED;
	m_ToolBox->SendMessage(msgSetKeyBinding, sizeof(KEYBIND), &keybind);

	keybind.m_kbNameID = kbName.GetUniqueID();
	keybind.m_KeyID = EEK_F1;
	keybind.m_MessageID = end_mission.GetUniqueID();
	keybind.m_bHitOnce = true;
	keybind.m_cBindType = KEYBIND_PRESSED;
	m_ToolBox->SendMessage(msgSetKeyBinding, sizeof(KEYBIND), &keybind);

	keybind.m_kbNameID = kbName.GetUniqueID();
	keybind.m_KeyID = EEK_F2;
	keybind.m_MessageID = PhysicsVisualizationOnOff.GetUniqueID();
	keybind.m_bHitOnce = true;
	m_ToolBox->SendMessage(msgSetKeyBinding, sizeof(KEYBIND), &keybind);

	keybind.m_kbNameID = kbName.GetUniqueID();
	keybind.m_KeyID = EEK_F3;
	keybind.m_MessageID = wireframeOnOff.GetUniqueID();
	keybind.m_bHitOnce = true;
	m_ToolBox->SendMessage(msgSetKeyBinding, sizeof(KEYBIND), &keybind);

	keybind.m_kbNameID = kbName.GetUniqueID();
	keybind.m_KeyID = EEK_F4;
	keybind.m_MessageID = profilerOnOff.GetUniqueID();
	keybind.m_bHitOnce = true;
	m_ToolBox->SendMessage(msgSetKeyBinding, sizeof(KEYBIND), &keybind);

	keybind.m_kbNameID = kbName.GetUniqueID();
	keybind.m_KeyID = EEK_F5;
	keybind.m_MessageID = reloadshaders.GetUniqueID();
	keybind.m_bHitOnce = true;
	m_ToolBox->SendMessage(msgSetKeyBinding, sizeof(KEYBIND), &keybind);

#ifdef _DEBUG
	CHashString hszMarkMemCheckPoint(_T("MarkMemCheckPoint"));
	CHashString hszCompareCheckPoint(_T("CompareCheckPoint"));
	CHashString hszEnableAllocHook(_T("EnableAllocHook"));

	keybind.m_kbNameID = kbName.GetUniqueID();
	keybind.m_KeyID = EEK_F6;
	keybind.m_MessageID = hszMarkMemCheckPoint.GetUniqueID();
	keybind.m_bHitOnce = true;
	m_ToolBox->SendMessage(msgSetKeyBinding, sizeof(KEYBIND), &keybind);

	keybind.m_kbNameID = kbName.GetUniqueID();
	keybind.m_KeyID = EEK_F7;
	keybind.m_MessageID = hszCompareCheckPoint.GetUniqueID();
	keybind.m_bHitOnce = true;
	m_ToolBox->SendMessage(msgSetKeyBinding, sizeof(KEYBIND), &keybind);

	keybind.m_kbNameID = kbName.GetUniqueID();
	keybind.m_KeyID = EEK_F8;
	keybind.m_MessageID = hszEnableAllocHook.GetUniqueID();
	keybind.m_bHitOnce = true;
	m_ToolBox->SendMessage(msgSetKeyBinding, sizeof(KEYBIND), &keybind);
#endif

	keybind.m_kbNameID = kbName.GetUniqueID();
	keybind.m_KeyID = EEK_F10;
	keybind.m_MessageID = exit.GetUniqueID();
	keybind.m_bHitOnce = true;
	m_ToolBox->SendMessage(msgSetKeyBinding, sizeof(KEYBIND), &keybind);

	
	keybind.m_kbNameID = kbName.GetUniqueID();
	keybind.m_KeyID = EEK_F11;
	keybind.m_MessageID = DisplayFPS.GetUniqueID();
	keybind.m_bHitOnce = true;
	m_ToolBox->SendMessage(msgSetKeyBinding, sizeof(KEYBIND), &keybind);

	keybind.m_kbNameID = kbName.GetUniqueID();
	keybind.m_KeyID = EEK_N;
	keybind.m_MessageID = nullRendererOnOff.GetUniqueID();
	keybind.m_bHitOnce = true;
	m_ToolBox->SendMessage(msgSetKeyBinding, sizeof(KEYBIND), &keybind);

	keybind.m_kbNameID = kbName.GetUniqueID();
	keybind.m_KeyID = EEK_C;
	keybind.m_MessageID = cameraToggle.GetUniqueID();
	keybind.m_bHitOnce = true;
	m_ToolBox->SendMessage(msgSetKeyBinding, sizeof(KEYBIND), &keybind);

	keybind.m_kbNameID = kbName.GetUniqueID();
	keybind.m_KeyID = EEK_P;
	keybind.m_MessageID = pause.GetUniqueID();
	keybind.m_bHitOnce = true;
	m_ToolBox->SendMessage(msgSetKeyBinding, sizeof(KEYBIND), &keybind);

	keybind.m_kbNameID = kbName.GetUniqueID();
	keybind.m_KeyID = EEK_PGDN;
	keybind.m_MessageID = moveProfilerDown.GetUniqueID();
	keybind.m_bHitOnce = false;
	m_ToolBox->SendMessage(msgSetKeyBinding, sizeof(KEYBIND), &keybind);

	keybind.m_kbNameID = kbName.GetUniqueID();
	keybind.m_KeyID = EEK_PGUP;
	keybind.m_MessageID = moveProfilerUp.GetUniqueID();
	keybind.m_bHitOnce = false;
	m_ToolBox->SendMessage(msgSetKeyBinding, sizeof(KEYBIND), &keybind);

	static DWORD msgSetActiveKeyBinding = CHashString( _T("SetActiveKeyBinding") ).GetUniqueID();
	m_ToolBox->SendMessage(msgSetActiveKeyBinding, sizeof(DWORD*), &keybind.m_kbNameID);

	SetJoystickBinding(EEJ_BUTTON0, CHashString(_T("AButton")).GetUniqueID(), KEYBIND_PRESSED|KEYBIND_RELEASED, false);
	SetJoystickBinding(EEJ_BUTTON1, CHashString(_T("BButton")).GetUniqueID(), KEYBIND_PRESSED|KEYBIND_RELEASED, false);
	SetJoystickBinding(EEJ_BUTTON2, CHashString(_T("XButton")).GetUniqueID(), KEYBIND_PRESSED|KEYBIND_RELEASED, false);
	SetJoystickBinding(EEJ_BUTTON3, CHashString(_T("YButton")).GetUniqueID(), KEYBIND_PRESSED|KEYBIND_RELEASED, false);
	SetJoystickBinding(EEJ_BUTTON4, CHashString(_T("LBButton")).GetUniqueID(), KEYBIND_PRESSED|KEYBIND_RELEASED, false);
	SetJoystickBinding(EEJ_BUTTON5, CHashString(_T("RBButton")).GetUniqueID(), KEYBIND_PRESSED|KEYBIND_RELEASED, false);
	SetJoystickBinding(EEJ_BUTTON6, CHashString(_T("BackButton")).GetUniqueID(), KEYBIND_PRESSED|KEYBIND_RELEASED, false);
	SetJoystickBinding(EEJ_BUTTON7, CHashString(_T("StartButton")).GetUniqueID(), KEYBIND_PRESSED|KEYBIND_RELEASED, false);
	SetJoystickBinding(EEJ_BUTTON8, CHashString(_T("LeftAnalogPress")).GetUniqueID(), KEYBIND_PRESSED|KEYBIND_RELEASED, false);
	SetJoystickBinding(EEJ_BUTTON10, CHashString(_T("DPadUp")).GetUniqueID(), KEYBIND_PRESSED|KEYBIND_RELEASED, false);
	SetJoystickBinding(EEJ_BUTTON11, CHashString(_T("DPadDown")).GetUniqueID(), KEYBIND_PRESSED|KEYBIND_RELEASED, false);
	SetJoystickBinding(EEJ_BUTTON12, CHashString(_T("DPadLeft")).GetUniqueID(), KEYBIND_PRESSED|KEYBIND_RELEASED, false);
	SetJoystickBinding(EEJ_BUTTON13, CHashString(_T("DPadRight")).GetUniqueID(), KEYBIND_PRESSED|KEYBIND_RELEASED, false);

	// main analog
	SetJoystickBinding(EEJ_AXISX, CHashString(_T("RadialDirectionX")).GetUniqueID(), KEYBIND_ANALOG_CHANGED, false);
	SetJoystickBinding(EEJ_AXISY, CHashString(_T("RadialDirectionY")).GetUniqueID(), KEYBIND_ANALOG_CHANGED, false);

	// rotational analog
	SetJoystickBinding(EEJ_AXISRX, CHashString(_T("RotationalDirectionX")).GetUniqueID(), KEYBIND_ANALOG_CHANGED, false);
	SetJoystickBinding(EEJ_AXISRY, CHashString(_T("RotationalDirectionY")).GetUniqueID(), KEYBIND_ANALOG_CHANGED, false);

	// triggers (considered axes)
	SetJoystickBinding(EEJ_AXISZ, CHashString(_T("LeftTrigger")).GetUniqueID(), KEYBIND_ANALOG_CHANGED, false);
	SetJoystickBinding(EEJ_AXISRZ, CHashString(_T("RightTrigger")).GetUniqueID(), KEYBIND_ANALOG_CHANGED, false);

	// coordinates with joystick bind name below
	static DWORD joystickName = CHashString(_T("EEDemo_joystick")).GetUniqueID();
	static DWORD msgSetActiveJoystickBinding = CHashString( _T("SetActiveJoystickKeyBinding") ).GetUniqueID();
	DWORD activeJoystickBindingID = joystickName;
	m_ToolBox->SendMessage(msgSetActiveJoystickBinding, sizeof(DWORD*), &activeJoystickBindingID);
}

void FlackSquadComponent::DeInitKeyBindings()
{
}

DWORD FlackSquadComponent::OnMouseMove(DWORD msgSize, void *param)
{
	DWORD retVal = MSG_HANDLED_PROCEED;
	MOUSECOORDS *mCoords;

	VERIFY_MESSAGE_SIZE(msgSize, sizeof(MOUSECOORDS));
	mCoords = (MOUSECOORDS*)param;

	m_lCursorPosX = mCoords->m_lXPos;
	m_lCursorPosY = mCoords->m_lYPos;

	// camera rotation
	POINT deltaMouse;

	deltaMouse.x = -(long)(mCoords->m_lXDelta );
	deltaMouse.y = (long)(mCoords->m_lYDelta );

	///////////////////////////////////////////////////////////////////////
	// Fine tuning mouse movements
    // FUNCTION: sign(x)*( ( x / a )^ b ) * a
	// a: @ point (a) the function intersects the F(x) = x function --> f(a) = a
    // b: exponent, influences the gradient of the function. 

	int sigX = deltaMouse.x > 0 ? 1 : -1;
	int sigY = deltaMouse.y > 0 ? 1 : -1;

	float correctionValue = 1.25f;
	float linearIntersectionPoint = 35.0f;

  	deltaMouse.x = (LONG)(sigX * pow( abs((int)deltaMouse.x / linearIntersectionPoint ) , correctionValue) * linearIntersectionPoint);
	deltaMouse.y = (LONG)(sigY * pow( abs((int)deltaMouse.y / linearIntersectionPoint ) , correctionValue) * linearIntersectionPoint);

	// if the camera is locked to turret, go ahead and control the turret
	
	if( !m_bIsMenuShown )
	{
		if (m_bCamLocked)
		{
			m_Player.OnPlayerMotionInput( deltaMouse );
		}
		else
		{
			RotateCamera(deltaMouse);
		}	
	}

	return retVal;
}

DWORD FlackSquadComponent::OnGetPlayerInput( DWORD size, void *param )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(FLACKSQUAD_GETPLAYERINPUT) );
	FLACKSQUAD_GETPLAYERINPUT &fs_gpi = *(FLACKSQUAD_GETPLAYERINPUT*)param;

	m_Player.WritePlayerControlVec( fs_gpi.iPlayerID, fs_gpi.v3StickA );
	m_Player.WritePlayerRControlVec( fs_gpi.iPlayerID, fs_gpi.v3StickB );

	return MSG_HANDLED_STOP;
}

/*//////////////////////////////////////////////////////////////////////////
// The original middle mouse handlers
DWORD FlackSquadComponent::OnMiddleMousePress(DWORD msgSize, void *param)
{

	m_MDragStartPointX = m_lCursorPosX; 
	m_MDragStartPointY = m_lCursorPosY;

	return MSG_HANDLED_PROCEED;
}

DWORD FlackSquadComponent::OnMiddleMouseRelease(DWORD msgSize, void *param)
{

	return MSG_HANDLED_PROCEED;
}
///////////////////////////////////////////////////////////////////////////*/

DWORD FlackSquadComponent::OnMiddleMousePress(DWORD msgSize, void *param)
{
	m_Player.OnZoomStart();
	return MSG_HANDLED_PROCEED;
}

DWORD FlackSquadComponent::OnMiddleMouseRelease(DWORD msgSize, void *param)
{
	m_Player.OnZoomStop();
	return MSG_HANDLED_PROCEED;
}

// The original action handler caused pause without showing pause menu. 
// Pressing ESC pauses the game and shows the menu.
// This action handler is switched to Key ESC handler for Key P.
DWORD FlackSquadComponent::OnPause(DWORD msgSize, void *param)
{
	// in the menu we dont want Key P the act like ESC.
	if( !m_bIsMenuShown )
	{
		// Calling ESC key handler.
		OnESCKeyPress( msgSize, param );
	}
	return MSG_HANDLED_STOP;
}

void FlackSquadComponent::RotateCamera(POINT &delta)
{
	EulerAngle vEuler(0.0f, (float)-delta.y, (float)-delta.x);	
	
	ROTATECAMERAPARAMS rotatecam;
	rotatecam.eRotateOperation = kCameraRotateAddEulerAnglesClampPitch;
	rotatecam.vEuler = &vEuler;
	rotatecam.fClampPitchMin = -90;
	rotatecam.fClampPitchMax = 90;

	static DWORD msgRotateCamera = CHashString( _T("RotateCamera") ).GetUniqueID();
	m_ToolBox->SendMessage( msgRotateCamera, sizeof(rotatecam), &rotatecam, &m_hszCameraName, &m_hszCameraType );	
}

void FlackSquadComponent::MoveCameraVectorZ(float fZDelta)
{
	MOVECAMERAPARAMS movecam;
	movecam.bRelative = true;
	movecam.fForwardDelta = (fZDelta * m_fCameraSpeed) * m_fCameraSensitivity2;

	// get data from camera
	static DWORD msgMoveCamera = CHashString( _T("MoveCamera") ).GetUniqueID();
	m_ToolBox->SendMessage( msgMoveCamera, sizeof(MOVECAMERAPARAMS), &movecam, &m_hszCameraName, &m_hszCameraType );
}

void FlackSquadComponent::StrafeCamera(POINT &delta)
{
	float fXDelta = ((float)delta.x * m_fCameraSpeed) * m_fCameraSensitivity2;
	float fYDelta = ((float)delta.y * -m_fCameraSpeed) * m_fCameraSensitivity2;

	MOVECAMERAPARAMS movecam;
	movecam.bRelative = true;
	movecam.fUpDelta = fYDelta;
	movecam.fRightDelta = fXDelta;

	// get data from camera
	static DWORD msgMoveCamera = CHashString( _T("MoveCamera") ).GetUniqueID();
	m_ToolBox->SendMessage( msgMoveCamera, sizeof(MOVECAMERAPARAMS), &movecam, &m_hszCameraName, &m_hszCameraType );
}

DWORD FlackSquadComponent::OnCamMoveForward(DWORD msgSize, void *param)
{
	if (!m_bCamLocked)
	{
		MoveCameraVectorZ(1.0f);
	}
	return MSG_HANDLED_PROCEED;
}

DWORD FlackSquadComponent::OnCamMoveBackward(DWORD msgSize, void *param)
{
	if (!m_bCamLocked)
	{
		MoveCameraVectorZ(-1.0f);
	}
	return MSG_HANDLED_PROCEED;
}

DWORD FlackSquadComponent::OnCamMoveLeft(DWORD msgSize, void *param)
{
	if (!m_bCamLocked)
	{
		POINT p = { 1, 0 };
		StrafeCamera(p);
	}
	return MSG_HANDLED_PROCEED;
}

DWORD FlackSquadComponent::OnCamMoveRight(DWORD msgSize, void *param)
{
	if (!m_bCamLocked)
	{
		POINT p = { -1, 0 };
		StrafeCamera(p);
	}
	return MSG_HANDLED_PROCEED;
}

DWORD FlackSquadComponent::OnRadialDirectionX( DWORD msgSize, void *param )
{
	VERIFY_MESSAGE_SIZE(msgSize, sizeof(KEYEVENTPARAMS));
	KEYEVENTPARAMS *pEvent = (KEYEVENTPARAMS*)param;

	float radialValue = pEvent->m_fValue;
	m_Player.OnRadialDirectionX( pEvent->m_iDeviceID, radialValue );
	return MSG_HANDLED_STOP;
}

DWORD FlackSquadComponent::OnRadialDirectionY( DWORD msgSize, void *param )
{
	VERIFY_MESSAGE_SIZE(msgSize, sizeof(KEYEVENTPARAMS));
	KEYEVENTPARAMS *pEvent = (KEYEVENTPARAMS*)param;

	float radialValue = pEvent->m_fValue;
	m_Player.OnRadialDirectionY( pEvent->m_iDeviceID, radialValue );
	return MSG_HANDLED_STOP;
}

DWORD FlackSquadComponent::OnRotationalDirectionX( DWORD msgSize, void *param )
{
	VERIFY_MESSAGE_SIZE(msgSize, sizeof(KEYEVENTPARAMS));
	KEYEVENTPARAMS *pEvent = (KEYEVENTPARAMS*)param;

	float rotationalValue = pEvent->m_fValue;
	m_Player.OnRotationalDirectionX( pEvent->m_iDeviceID, rotationalValue );
	return MSG_HANDLED_STOP;
}

DWORD FlackSquadComponent::OnRotationalDirectionY( DWORD msgSize, void *param )
{
	VERIFY_MESSAGE_SIZE(msgSize, sizeof(KEYEVENTPARAMS));
	KEYEVENTPARAMS *pEvent = (KEYEVENTPARAMS*)param;

	float rotationalValue = pEvent->m_fValue;
	m_Player.OnRotationalDirectionY( pEvent->m_iDeviceID, rotationalValue );
	return MSG_HANDLED_STOP;
}

DWORD FlackSquadComponent::OnEndMission(DWORD msgSize, void *param)
{
	// "Game_Play_MISSIONEND_Event"

	CHashString tempName( _T("Game_Play_MISSIONEND_Event") );
	CHashString astateObjectName( _T("GameStateObject") );

	TRIGGEREVENTINMGRPARAMS aeventParams;
	aeventParams.EventName = &tempName;
	aeventParams.StateObjectName = &astateObjectName;
	aeventParams.EventParamsArchive = NULL;

	// Raise the event
	static DWORD msgTriggerEventInStateMachine = CHashString( _T("TriggerEventInStateMachine") ).GetUniqueID();
	DWORD retVal = m_ToolBox->SendMessage( msgTriggerEventInStateMachine, sizeof(TRIGGEREVENTINMGRPARAMS), &aeventParams, NULL, NULL);

	return retVal;
}
DWORD FlackSquadComponent::OnPlaneExplode(DWORD msgSize, void *param)
{
	LOADFILEEXTPARAMS lfep;
	lfep.fileName = "Spawners\\Bf109E.sxl";
	lfep.bInternalLoad = true;

	static DWORD msgLoadFileByExtension = CHashString( _T("LoadFileByExtension") ).GetUniqueID();
	DWORD retVal = m_ToolBox->SendMessage( msgLoadFileByExtension, sizeof(lfep), &lfep );
	return retVal;
}

DWORD FlackSquadComponent::OnGotoMenu(DWORD msgSize, void *param)
{
	CHashString temp( _T("Start Menu") );

	static DWORD msgGUIManagerSetActivePage = CHashString( _T("GUIManagerSetActivePage") ).GetUniqueID();
    return EngineGetToolBox()->SendMessage(msgGUIManagerSetActivePage, sizeof(DWORD), &temp);
}

DWORD FlackSquadComponent::OnESCKeyPress(DWORD msgSize, void *param)
{
	// ESC key shows the menu, but in the menu we don't need it to act as Start button.
	// The Start button starts the game while ESC should not. 
	if ( !m_bIsMenuShown ) 
	{
		m_Player.OnPlayerStartButton( 0, TRUE );
	} 
	else
	{
		// In the menu ESC is used as back button just as ButtonB on the controller
		m_Player.OnPlayerBButton( 0, TRUE );
	}
	return MSG_HANDLED_PROCEED;
}

DWORD FlackSquadComponent::OnSpaceKeyPress(DWORD msgSize, void *param)
{
	VERIFY_MESSAGE_SIZE( msgSize, sizeof(KEYEVENTPARAMS) );
	KEYEVENTPARAMS *keyEvent = (KEYEVENTPARAMS*)param;
	bool pressed = true;
	if ((keyEvent) && (keyEvent->m_cBindType == KEYBIND_RELEASED))
	{
		pressed = false;
	}

	if( m_bIsMenuShown ) 
	{	
		// in the menu we select by pressing space
		m_Player.OnPlayerAButton( 0, pressed );
	}
	else
	{
		// in the game space key is used to fire
		if(pressed)
		{
			m_Player.OnPlayerFirePressed();		
		}
		else 
		{
			m_Player.OnPlayerFireReleased();		
		}
	}
	return MSG_HANDLED_PROCEED;
}

struct HIERARCHICALOFFSETPARAM
{
	Vec3 *	m_Offset;
	UINT	m_Child;
	HIERARCHICALOFFSETPARAM()
	{
		m_Offset = NULL;
		m_Child = 0;
	}
};

struct HIERARCHICALMODELGETCHILDID
{
	IHashString * m_Childname;
	UINT		  m_RetID;
};

DWORD FlackSquadComponent::OnEnablePostProcessGlow( DWORD msgSize, void * param)
{
	// Glow Post Process Effect
	POSTPROCESSEFFECTPARAMS	ppep;
	if (!m_pGlowPostProcessEffect)
	{
		// add effect
		CREATEEFFECTINTERFACE cei;
		CHashString filename( _T("shaders/GlowPostProcess.efx") );
		cei.m_Name = &filename;
		static DWORD msgHash_GetEffectInterface = CHashString(_T("GetEffectInterface")).GetUniqueID();
		EngineGetToolBox()->SendMessage(msgHash_GetEffectInterface, sizeof(cei), &cei );
		m_pGlowPostProcessEffect = cei.m_EffectInterface;
		if (m_pGlowPostProcessEffect)
		{
			ppep.effect = m_pGlowPostProcessEffect;
			static DWORD msgHash_AddPostProcessEffect = CHashString(_T("AddPostProcessEffect")).GetUniqueID();
			m_ToolBox->SendMessage( msgHash_AddPostProcessEffect, sizeof(ppep), &ppep );
		}
	}
	return MSG_HANDLED_STOP;
}

DWORD FlackSquadComponent::OnDisablePostProcessGlow(DWORD size, void *param)
{
	// Glow Post Process Effect
	POSTPROCESSEFFECTPARAMS	ppep;
	if (m_pGlowPostProcessEffect)
	{
		// remove effect
		ppep.effect = m_pGlowPostProcessEffect;
		static DWORD msgHash_RemovePostProcessEffect = CHashString(_T("RemovePostProcessEffect")).GetUniqueID();
		m_ToolBox->SendMessage( msgHash_RemovePostProcessEffect, sizeof(ppep), &ppep );

		m_pGlowPostProcessEffect = NULL;
	}
	return MSG_HANDLED_STOP;
}

DWORD FlackSquadComponent::OnEnableBWPostProcess(DWORD size, void *param)
{
	POSTPROCESSEFFECTPARAMS	ppep;
	if (!m_pBWPostProcessEffect)
	{
		// add effect
		CREATEEFFECTINTERFACE cei;
		CHashString filename( _T("shaders/BWPostProcess.efx") );
		cei.m_Name = &filename;
		static DWORD msgHash_GetEffectInterface = CHashString(_T("GetEffectInterface")).GetUniqueID();
		EngineGetToolBox()->SendMessage(msgHash_GetEffectInterface, sizeof(cei), &cei );
		m_pBWPostProcessEffect = cei.m_EffectInterface;
		if (m_pBWPostProcessEffect)
		{
			ppep.effect = m_pBWPostProcessEffect;
			static DWORD msgHash_AddPostProcessEffect = CHashString(_T("AddPostProcessEffect")).GetUniqueID();
			m_ToolBox->SendMessage( msgHash_AddPostProcessEffect, sizeof(ppep), &ppep );
		}
	}
	return MSG_HANDLED_STOP;
}
	
DWORD FlackSquadComponent::OnDisableBWPostProcess(DWORD size, void *param)
{
	POSTPROCESSEFFECTPARAMS	ppep;
	if (m_pBWPostProcessEffect)
	{
		// remove effect
		ppep.effect = m_pBWPostProcessEffect;
		static DWORD msgHash_RemovePostProcessEffect = CHashString(_T("RemovePostProcessEffect")).GetUniqueID();
		m_ToolBox->SendMessage( msgHash_RemovePostProcessEffect, sizeof(ppep), &ppep );

		m_pBWPostProcessEffect = NULL;
	}
	return MSG_HANDLED_STOP;
}

/// message to handle default logging capabilities
DWORD FlackSquadComponent::OnLogMessage(DWORD size, void *param)
{
	LOGMESSAGESTRUCT *logMessage;
	// can't do a verify message size, as that might cause recursion
	if (size != sizeof(LOGMESSAGESTRUCT))
	{
		return MSG_ERROR;
	}

	logMessage = (LOGMESSAGESTRUCT *)param;
//	if ((m_FlackSquadApp) && (m_FlackSquadApp->m_EnableLogging))
	{
#ifndef XBOX // don't log to file on xbox
		if (m_LogFile == NULL)
		{
			m_LogFile = fopen(_T("FlackSquad.log"), "w+");
			if (m_LogFile == NULL)
			{
#ifdef WIN32
				MessageBox(NULL, _T("Unable to open log file.\naborting!..."), _T("Error"), MB_OK);
#endif//WIN32
			}
		}

		fprintf(m_LogFile,logMessage->msg);
		fflush(m_LogFile);
#endif
		if (logMessage->severity == LOGFATALERROR)
		{
			if (m_LogFile)
				fclose(m_LogFile);
			static DWORD msgExit = CHashString( _T("Exit") ).GetUniqueID();
			m_ToolBox->SendMessage(msgExit, 0, NULL);
		}
		return MSG_HANDLED_PROCEED;
	}

	return MSG_NOT_HANDLED;
}

DWORD FlackSquadComponent::OnProfilerOnOff(DWORD size, void *param)
{
	m_bProfilerOn = !m_bProfilerOn;
	
	static DWORD msgShowHideProfilerWindow = CHashString(_T("ShowHideProfilerWindow")).GetUniqueID();
	m_ToolBox->SendMessage(msgShowHideProfilerWindow, sizeof(bool), &m_bProfilerOn);

	return MSG_HANDLED_STOP;
}

DWORD FlackSquadComponent::OnPhysicsVisualizationOnOff(DWORD size, void* param)
{
	CHashString physObject = (_T("CPhysicsObject"));
	if( m_bPhysVisOn )
	{
		m_bPhysVisOn = false;
		static DWORD msgDisableVisualization = CHashString( _T("DisableVisualization") ).GetUniqueID();
		m_ToolBox->SendMessage(msgDisableVisualization, 0, NULL, NULL, &physObject);
	}
	else
	{
		m_bPhysVisOn = true;
		static DWORD msgEnableVisualization = CHashString( _T("EnableVisualization") ).GetUniqueID();
		m_ToolBox->SendMessage(msgEnableVisualization, 0, NULL, NULL, &physObject);
	}

	return MSG_HANDLED_STOP;
}

DWORD FlackSquadComponent::OnWireframeOnOff(DWORD size, void *param)
{
	m_bWireframeMode = !m_bWireframeMode;

	static DWORD msgSetWireFrameMode = CHashString( _T("SetWireFrameMode") ).GetUniqueID();
	m_ToolBox->SendMessage(msgSetWireFrameMode, sizeof(bool), &m_bWireframeMode);
	return MSG_HANDLED_STOP;
}

DWORD FlackSquadComponent::OnNullRendererOnOff( DWORD size, void* param )
{
	m_bNullRenderer = !m_bNullRenderer;

	static DWORD msgHash_SetNullRenderer = CHashString( _T("SetNullRenderer") ).GetUniqueID();
	m_ToolBox->SendMessage( msgHash_SetNullRenderer, sizeof(bool), &m_bNullRenderer );

	return MSG_HANDLED_STOP;
}

DWORD FlackSquadComponent::OnEnableContinueMode(DWORD size, void *param)
{
	m_bContinueMode = true;
	return MSG_HANDLED_STOP;
}
	
DWORD FlackSquadComponent::OnDisableContinueMode(DWORD size, void *param)
{
	m_bContinueMode = false;
	return MSG_HANDLED_STOP;
}


///////////////////////////////////////////////////////////////////////////
// Arrow key message handlers
DWORD FlackSquadComponent::OnUpArrowPress(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE( size, sizeof(KEYEVENTPARAMS) );
	KEYEVENTPARAMS *keyEvent = (KEYEVENTPARAMS*)param;

	// Check if this is press or release event
	bool pressed = true;
	if ((keyEvent) && (keyEvent->m_cBindType == KEYBIND_RELEASED))
	{
		pressed = false;
	}

	if ( pressed ) 
	{
		if ( m_bIsMenuShown ) 
		{
			if ( m_bFirstMenuKeyPress )
			{
				m_Player.OnPlayerMenuNavigateUp();
				m_bFirstMenuKeyPress = FALSE;
			}
		}
		else 
		{
			if (m_bCamLocked)
			{
				deltaMouse.y = -10;
				m_Player.OnPlayerMotionInput( deltaMouse );
			}
			else 
			{
				MoveCameraVectorZ(1.0f);
			}
		}
	}
	else 
	{
		if ( m_bIsMenuShown ) 
		{
			m_bFirstMenuKeyPress = TRUE;
		}
		else 
		{
			deltaMouse.y = 0;
			m_Player.OnPlayerResetMotionInput( deltaMouse );
		}		
	}
	return MSG_HANDLED_STOP;
}

DWORD FlackSquadComponent::OnDownArrowPress(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE( size, sizeof(KEYEVENTPARAMS) );
	KEYEVENTPARAMS *keyEvent = (KEYEVENTPARAMS*)param;

	// Check if this is press or release event
	bool pressed = true;
	if ((keyEvent) && (keyEvent->m_cBindType == KEYBIND_RELEASED))
	{
		pressed = false;
	}

	if ( pressed ) 
	{
		if ( m_bIsMenuShown ) 
		{
			if ( m_bFirstMenuKeyPress )
			{
				m_Player.OnPlayerMenuNavigateDown();
				m_bFirstMenuKeyPress = FALSE;
			}
		}
		else 
		{
			if (m_bCamLocked)
			{
				deltaMouse.y = 10;
				m_Player.OnPlayerMotionInput( deltaMouse );
			}
			else 
			{
				MoveCameraVectorZ(-1.0f);
			}
		}
	}
	else 
	{
		if ( m_bIsMenuShown ) 
		{
			m_bFirstMenuKeyPress = TRUE;
		}
		else 
		{
			deltaMouse.y = 0;
			m_Player.OnPlayerResetMotionInput( deltaMouse );
		}		
	}
	return MSG_HANDLED_STOP;
}

DWORD FlackSquadComponent::OnLeftArrowPress(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE( size, sizeof(KEYEVENTPARAMS) );
	KEYEVENTPARAMS *keyEvent = (KEYEVENTPARAMS*)param;

	// Check if this is press or release event
	bool pressed = true;
	if ((keyEvent) && (keyEvent->m_cBindType == KEYBIND_RELEASED))
	{
		pressed = false;
	}

	if ( pressed ) 
	{
		if ( m_bIsMenuShown ) 
		{
			if ( m_bFirstMenuKeyPress )
			{
				m_Player.OnPlayerMenuNavigateLeft();
				m_bFirstMenuKeyPress = FALSE;
			}
		}
		else 
		{
			if (m_bCamLocked)
			{
				deltaMouse.x = 10;
				m_Player.OnPlayerMotionInput( deltaMouse );
			}
			else
			{
				POINT p = { 1, 0 };
				StrafeCamera(p);
			}
		}
	}
	else 
	{
		if ( m_bIsMenuShown ) 
		{
			m_bFirstMenuKeyPress = TRUE;
		}
		else 
		{
			deltaMouse.x = 0;
			m_Player.OnPlayerResetMotionInput( deltaMouse );
		}		
	}
	return MSG_HANDLED_STOP;
}

DWORD FlackSquadComponent::OnRightArrowPress(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE( size, sizeof(KEYEVENTPARAMS) );
	KEYEVENTPARAMS *keyEvent = (KEYEVENTPARAMS*)param;

	// Check if this is press or release event
	bool pressed = true;
	if ((keyEvent) && (keyEvent->m_cBindType == KEYBIND_RELEASED))
	{
		pressed = false;
	}

	if ( pressed ) 
	{
		if ( m_bIsMenuShown ) 
		{
			if ( m_bFirstMenuKeyPress )
			{
				m_Player.OnPlayerMenuNavigateRight();
				m_bFirstMenuKeyPress = FALSE;
			}
		}
		else 
		{
			if (m_bCamLocked)
			{
				deltaMouse.x = -10;
				m_Player.OnPlayerMotionInput( deltaMouse );
			}
			else 
			{
				POINT p = { -1, 0 };
				StrafeCamera(p);
			}
		}
	}
	else 
	{
		if ( m_bIsMenuShown ) 
		{
			m_bFirstMenuKeyPress = TRUE;
		}
		else 
		{
			deltaMouse.x = 0;
			m_Player.OnPlayerResetMotionInput( deltaMouse );
		}		
	}
	return MSG_HANDLED_STOP;
}

/////////////////////////////////////////////////////////////////////////
//Weapon selection
DWORD FlackSquadComponent::OnQKeyPress(DWORD size, void *param)
{
	m_Player.ChangeWeaponNext();
	return MSG_HANDLED_STOP;
}

DWORD FlackSquadComponent::OnEKeyPress(DWORD size, void *param)
{
	m_Player.ChangeWeaponPrevious();
	return MSG_HANDLED_STOP;
}

DWORD FlackSquadComponent::OnFKeyPress(DWORD size, void *param)
{	
	VERIFY_MESSAGE_SIZE( size, sizeof(KEYEVENTPARAMS) );
	KEYEVENTPARAMS *keyEvent = (KEYEVENTPARAMS*)param;

	// Check if this is press or release event
	bool pressed = true;
	if ((keyEvent) && (keyEvent->m_cBindType == KEYBIND_RELEASED))
	{
		pressed = false;
	}

	if ( pressed ) 
	{
		m_Player.OnZoomStart();
	}
	else
	{
		m_Player.OnZoomStop();
	}
	return MSG_HANDLED_STOP;
}

DWORD FlackSquadComponent::OnRKeyPress(DWORD size, void *param)
{	
	VERIFY_MESSAGE_SIZE( size, sizeof(KEYEVENTPARAMS) );
	KEYEVENTPARAMS *keyEvent = (KEYEVENTPARAMS*)param;

	// Check if this is press or release event
	bool pressed = true;
	if ((keyEvent) && (keyEvent->m_cBindType == KEYBIND_RELEASED))
	{
		pressed = false;
	}

	if ( pressed ) 
	{
		m_Player.OnPlayerFireSecondaryPressed();
	}
	else
	{
		m_Player.OnPlayerFireSecondaryReleased();
	}
	return MSG_HANDLED_STOP;
}

DWORD FlackSquadComponent::OnToggleCameraMode(DWORD size, void *param)
{
	m_bCamLocked = !m_bCamLocked;
	
	CHashString astateObjectName( _T("CameraStateObject") );

	TRIGGEREVENTINMGRPARAMS aeventParams;

	static DWORD msgTriggerEventInStateMachine = CHashString( _T("TriggerEventInStateMachine") ).GetUniqueID();

	if (m_bCamLocked)
	{
		CHashString tempName = (_T("Camera_Main_Attach_Event"));	
		aeventParams.EventName = &tempName;
		aeventParams.StateObjectName = &astateObjectName;
		aeventParams.EventParamsArchive = NULL;
		
		// Raise the event
		m_ToolBox->SendMessage( msgTriggerEventInStateMachine, sizeof(TRIGGEREVENTINMGRPARAMS), &aeventParams);
	}
	else
	{
		CHashString tempName = (_T("Camera_Main_Detach_Event"));	
		aeventParams.EventName = &tempName;
		aeventParams.StateObjectName = &astateObjectName;
		aeventParams.EventParamsArchive = NULL;
		
		// Raise the event
		m_ToolBox->SendMessage( msgTriggerEventInStateMachine, sizeof(TRIGGEREVENTINMGRPARAMS), &aeventParams, NULL, NULL);
		
		// grab the camera name
		CONTEXTCAMPARAMS ccp;
		ccp.pRC = NULL;
		ccp.szCameraName = &m_hszCameraName;
		static DWORD msgGetActiveCamera = CHashString( _T("GetActiveCamera") ).GetUniqueID();
		m_ToolBox->SendMessage(msgGetActiveCamera, sizeof(CONTEXTCAMPARAMS), &ccp);
	}

	return MSG_HANDLED_STOP;
}

DWORD FlackSquadComponent::OnInitGameKeybindings(DWORD size, void *param)
{
	InitKeyBindings();
	return MSG_HANDLED_STOP;
}

DWORD FlackSquadComponent::OnLeftMousePress(DWORD size, void *param)
{
	if ( m_bIsMenuShown )
	{
		// In the menu - use left button to select
		m_Player.OnPlayerAButton( 0, TRUE );
	}
	else 
	{
		// In the game - use left button to fire
		m_Player.OnPlayerFirePressed();
	}
	return MSG_HANDLED_PROCEED;
}

DWORD FlackSquadComponent::OnLeftMouseRelease(DWORD size, void *param)
{
	m_Player.OnPlayerFireReleased();
	return MSG_HANDLED_PROCEED;
}

DWORD FlackSquadComponent::OnRightMousePress(DWORD size, void *param)
{
	if ( m_bIsMenuShown )
	{
		// TODO: perhaps rigth mouse should be 'Back' in the menu
		m_Player.OnPlayerBButton( 0, TRUE );
	}
	else 
	{
		// In the game - use left button to fire
		m_Player.OnPlayerFireSecondaryPressed();
	}
	
	return MSG_HANDLED_PROCEED;
}


DWORD FlackSquadComponent::OnRightMouseRelease(DWORD size, void *param)
{
	m_Player.OnPlayerFireSecondaryReleased();
	return MSG_HANDLED_PROCEED;
}

DWORD FlackSquadComponent::OnMouseWheel(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE( size, sizeof(float) );
	float wheelDelta  = *( (float *) param );
	if ( m_bIsMenuShown )
	{
		if (wheelDelta < 0)
		{
			m_Player.OnPlayerMenuNavigateUp();
		}
		else 
		{
			m_Player.OnPlayerMenuNavigateDown();
		}
	}
	else {
		if (wheelDelta < 0)
		{
			m_Player.ChangeWeaponNext();
		}
		else 
		{
			m_Player.ChangeWeaponPrevious();
		}
	}
	return MSG_HANDLED_PROCEED;
}

DWORD FlackSquadComponent::OnAButton(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE( size, sizeof(KEYEVENTPARAMS) );
	KEYEVENTPARAMS *keyEvent = (KEYEVENTPARAMS*)param;
	bool pressed = true;
	if ((keyEvent) && (keyEvent->m_cBindType == KEYBIND_RELEASED))
	{
		pressed = false;
	}

	m_Player.OnPlayerAButton( keyEvent->m_iDeviceID, pressed );
	return MSG_HANDLED_PROCEED;
}
	
DWORD FlackSquadComponent::OnBButton(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE( size, sizeof(KEYEVENTPARAMS) );
	KEYEVENTPARAMS *keyEvent = (KEYEVENTPARAMS*)param;
	bool pressed = true;
	if ((keyEvent) && (keyEvent->m_cBindType == KEYBIND_RELEASED))
	{
		pressed = false;
	}

	m_Player.OnPlayerBButton( keyEvent->m_iDeviceID, pressed );
	return MSG_HANDLED_PROCEED;
}

DWORD FlackSquadComponent::OnXButton(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE( size, sizeof(KEYEVENTPARAMS) );
	KEYEVENTPARAMS *keyEvent = (KEYEVENTPARAMS*)param;
	bool pressed = true;
	if ((keyEvent) && (keyEvent->m_cBindType == KEYBIND_RELEASED))
	{
		pressed = false;
	}

	m_Player.OnPlayerXButton( keyEvent->m_iDeviceID, pressed );
	return MSG_HANDLED_PROCEED;
}

DWORD FlackSquadComponent::OnYButton(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE( size, sizeof(KEYEVENTPARAMS) );
	KEYEVENTPARAMS *keyEvent = (KEYEVENTPARAMS*)param;
	bool pressed = true;
	if ((keyEvent) && (keyEvent->m_cBindType == KEYBIND_RELEASED))
	{
		pressed = false;
	}

	m_Player.OnPlayerYButton( keyEvent->m_iDeviceID, pressed );
	return MSG_HANDLED_PROCEED;
}

DWORD FlackSquadComponent::OnBackButton(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE( size, sizeof(KEYEVENTPARAMS) );
	KEYEVENTPARAMS *keyEvent = (KEYEVENTPARAMS*)param;
	bool pressed = true;
	if ((keyEvent) && (keyEvent->m_cBindType == KEYBIND_RELEASED))
	{
		pressed = false;
	}

	m_Player.OnPlayerBackButton( keyEvent->m_iDeviceID, pressed );
	return MSG_HANDLED_PROCEED;
}

DWORD FlackSquadComponent::OnStartButton(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE( size, sizeof(KEYEVENTPARAMS) );
	KEYEVENTPARAMS *keyEvent = (KEYEVENTPARAMS*)param;
	bool pressed = true;
	if ((keyEvent) && (keyEvent->m_cBindType == KEYBIND_RELEASED))
	{
		pressed = false;
	}

	m_Player.OnPlayerStartButton( keyEvent->m_iDeviceID, pressed );
	return MSG_HANDLED_PROCEED;
}


DWORD FlackSquadComponent::OnLBButton(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE( size, sizeof(KEYEVENTPARAMS) );
	KEYEVENTPARAMS *keyEvent = (KEYEVENTPARAMS*)param;
	bool pressed = true;
	if ((keyEvent) && (keyEvent->m_cBindType == KEYBIND_RELEASED))
	{
		pressed = false;
	}

	m_Player.OnPlayerLBButton( keyEvent->m_iDeviceID, pressed );
	return MSG_HANDLED_PROCEED;
}

DWORD FlackSquadComponent::OnRBButton(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE( size, sizeof(KEYEVENTPARAMS) );
	KEYEVENTPARAMS *keyEvent = (KEYEVENTPARAMS*)param;
	bool pressed = true;
	if ((keyEvent) && (keyEvent->m_cBindType == KEYBIND_RELEASED))
	{
		pressed = false;
	}

	m_Player.OnPlayerRBButton( keyEvent->m_iDeviceID, pressed );
	return MSG_HANDLED_PROCEED;
}

void FlackSquadComponent::SetJoystickBinding(DWORD key, DWORD msg, int filter, bool repeat)
{
	JOYSTICKKEYBINDPARAMS joybind;
	static DWORD msgHash_SetJoystickKeyBinding = CHashString(_T("SetJoystickKeyBinding")).GetUniqueID();
	static DWORD joystickName = CHashString(_T("EEDemo_joystick")).GetUniqueID();
	
	//setup joystick	
	joybind.m_kbNameID = joystickName;
	joybind.m_KeyID = key;
	joybind.m_MessageID = msg;
	joybind.m_EventFilter = filter;
	joybind.m_bRepeat = repeat;
	for( int i = 0; i < MAXNUMJOYSTICKS; i++ )
	{
		joybind.m_DeviceID = i;
		m_ToolBox->SendMessage(msgHash_SetJoystickKeyBinding, sizeof(JOYSTICKKEYBINDPARAMS), &joybind);
	}
}

DWORD FlackSquadComponent::OnLeftTrigger(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE( size, sizeof(KEYEVENTPARAMS) );
	KEYEVENTPARAMS *keyEvent = (KEYEVENTPARAMS*)param;
	//m_ToolBox->Log(LOGWARNING, _T("Left trigger pressed: %f\n"), keyEvent->m_fValue);
	m_Player.UpdatePlayerTriggers( keyEvent->m_iDeviceID, EEJ_AXISZ, keyEvent->m_fValue );
	return MSG_HANDLED_PROCEED;
}

DWORD FlackSquadComponent::OnRightTrigger(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE( size, sizeof(KEYEVENTPARAMS) );
	KEYEVENTPARAMS *keyEvent = (KEYEVENTPARAMS*)param;
	//m_ToolBox->Log(LOGWARNING, _T("Right trigger pressed: %f\n"), keyEvent->m_fValue);
	m_Player.UpdatePlayerTriggers( keyEvent->m_iDeviceID, EEJ_AXISRZ, keyEvent->m_fValue );
	return MSG_HANDLED_PROCEED;
}

DWORD FlackSquadComponent::OnGetPlayerAxisSensitivity(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(float));

	float *axisOut = (float*)param;
	*axisOut = m_Player.GetAxisSensitivity();

	return MSG_HANDLED_PROCEED;
}
	
DWORD FlackSquadComponent::OnSetPlayerAxisSensitivity(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(float));

	float axisIn = *(float*)param;
	m_Player.SetAxisSensitivity(axisIn);

	return MSG_HANDLED_PROCEED;
}

DWORD FlackSquadComponent::OnLeftAnalogPress(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE( size, sizeof(KEYEVENTPARAMS) );
	KEYEVENTPARAMS *keyEvent = (KEYEVENTPARAMS*)param;
	bool pressed = true;
	if ((keyEvent) && (keyEvent->m_cBindType == KEYBIND_RELEASED))
	{
		pressed = false;
	}

	m_Player.OnPlayerLeftAnalog( keyEvent->m_iDeviceID, pressed );
	return MSG_HANDLED_PROCEED;
}

DWORD FlackSquadComponent::OnDPadUp(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE( size, sizeof(KEYEVENTPARAMS) );
	KEYEVENTPARAMS *keyEvent = (KEYEVENTPARAMS*)param;
	bool pressed = true;
	if ((keyEvent) && (keyEvent->m_cBindType == KEYBIND_RELEASED))
	{
		pressed = false;
	}

	m_Player.OnPlayerDPadUp( keyEvent->m_iDeviceID, pressed );
	return MSG_HANDLED_PROCEED;
}

DWORD FlackSquadComponent::OnDPadDown(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE( size, sizeof(KEYEVENTPARAMS) );
	KEYEVENTPARAMS *keyEvent = (KEYEVENTPARAMS*)param;
	bool pressed = true;
	if ((keyEvent) && (keyEvent->m_cBindType == KEYBIND_RELEASED))
	{
		pressed = false;
	}

	m_Player.OnPlayerDPadDown( keyEvent->m_iDeviceID, pressed );
	return MSG_HANDLED_PROCEED;
}

DWORD FlackSquadComponent::OnDPadLeft(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE( size, sizeof(KEYEVENTPARAMS) );
	KEYEVENTPARAMS *keyEvent = (KEYEVENTPARAMS*)param;
	bool pressed = true;
	if ((keyEvent) && (keyEvent->m_cBindType == KEYBIND_RELEASED))
	{
		pressed = false;
	}

	m_Player.OnPlayerDPadLeft( keyEvent->m_iDeviceID, pressed );
	return MSG_HANDLED_PROCEED;
}

DWORD FlackSquadComponent::OnDPadRight(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE( size, sizeof(KEYEVENTPARAMS) );
	KEYEVENTPARAMS *keyEvent = (KEYEVENTPARAMS*)param;
	bool pressed = true;
	if ((keyEvent) && (keyEvent->m_cBindType == KEYBIND_RELEASED))
	{
		pressed = false;
	}

	m_Player.OnPlayerDPadRight( keyEvent->m_iDeviceID, pressed );
	return MSG_HANDLED_PROCEED;
}


DWORD FlackSquadComponent::OnSetLeftMotorSpeed(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE( size, sizeof(JOYSTICKMOTORSPEEDPARAMS) );
	JOYSTICKMOTORSPEEDPARAMS *motorSpeedParams = (JOYSTICKMOTORSPEEDPARAMS*)param;
	m_Player.SetLeftMotorSpeed( motorSpeedParams );
	return MSG_HANDLED_PROCEED;
}

DWORD FlackSquadComponent::OnSetRightMotorSpeed(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE( size, sizeof(JOYSTICKMOTORSPEEDPARAMS) );
	JOYSTICKMOTORSPEEDPARAMS *motorSpeedParams = (JOYSTICKMOTORSPEEDPARAMS*)param;
	m_Player.SetRightMotorSpeed( motorSpeedParams );
	return MSG_HANDLED_PROCEED;
}

DWORD FlackSquadComponent::OnJoystickConnected(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE( size, sizeof(int) );
	int *iPlayerID = (int*)param;

	CREATEARCHIVE ca;
	CHashString memType(_T("Memory"));

	ca.mode = STREAM_MODE_WRITE | STREAM_MODE_READ;
	ca.streamData = NULL;
	ca.streamSize = sizeof(*iPlayerID);
	ca.streamType = &memType;

	static DWORD msgHash_CreateArchive = CHashString( _T("CreateArchive") ).GetUniqueID();
	m_ToolBox->SendMessage( msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca );

	ca.archive->Write( *iPlayerID );

	CHashString tempName( _T("Game_JoystickDisconnected_Event") );
	CHashString astateObjectName( _T("GameStateObject") );
	TRIGGEREVENTINMGRPARAMS aeventParams;
	aeventParams.EventName = &tempName;
	aeventParams.StateObjectName = &astateObjectName;
	aeventParams.EventParamsArchive = ca.archive;

	// Raise the event
	static DWORD msgTriggerEventInStateMachine = CHashString( _T("TriggerEventInStateMachine") ).GetUniqueID();
	m_ToolBox->SendMessage( msgTriggerEventInStateMachine, sizeof(TRIGGEREVENTINMGRPARAMS), &aeventParams, NULL, NULL);

	ca.archive->Close();

	return MSG_HANDLED_PROCEED;
}

DWORD FlackSquadComponent::OnJoystickDisconnected(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE( size, sizeof(int) );
	int *iPlayerID = (int*)param;

	CREATEARCHIVE ca;
	CHashString memType(_T("Memory"));

	ca.mode = STREAM_MODE_WRITE | STREAM_MODE_READ;
	ca.streamData = NULL;
	ca.streamSize = sizeof(*iPlayerID);
	ca.streamType = &memType;

	static DWORD msgHash_CreateArchive = CHashString( _T("CreateArchive") ).GetUniqueID();
	m_ToolBox->SendMessage( msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca );

	ca.archive->Write( *iPlayerID );

	CHashString tempName( _T("Game_JoystickDisconnected_Event") );
	CHashString astateObjectName( _T("GameStateObject") );
	TRIGGEREVENTINMGRPARAMS aeventParams;
	aeventParams.EventName = &tempName;
	aeventParams.StateObjectName = &astateObjectName;
	aeventParams.EventParamsArchive = ca.archive;

	// Raise the event
	static DWORD msgTriggerEventInStateMachine = CHashString( _T("TriggerEventInStateMachine") ).GetUniqueID();
	m_ToolBox->SendMessage( msgTriggerEventInStateMachine, sizeof(TRIGGEREVENTINMGRPARAMS), &aeventParams, NULL, NULL);

	ca.archive->Close();

	return MSG_HANDLED_PROCEED;
}

DWORD FlackSquadComponent::OnShowMenu(DWORD size, void *param)
{
	// Change controls, menu is visible
	m_bIsMenuShown = true;
	
	//reset menu keypress flag
	m_bFirstMenuKeyPress = true;

	return MSG_HANDLED_STOP;
}

DWORD FlackSquadComponent::OnHideMenu(DWORD size, void *param)
{
	m_bIsMenuShown = false;
	return MSG_HANDLED_STOP;
}

DWORD FlackSquadComponent::OnStartMission(DWORD size, void *param)
{
	// Controls changed, menu is hidden
	m_bIsMenuShown = false;

	// resetting the camera lock variable
	m_bCamLocked = true;

	return MSG_HANDLED_STOP;
}

DWORD FlackSquadComponent::OnDisplayFPS(DWORD size, void *param)
{
	//VERIFY message size
	VERIFY_MESSAGE_SIZE( size, sizeof(KEYEVENTPARAMS) );

	// change FPS display visibility
	m_bShowFpsDisplay = !m_bShowFpsDisplay;

	CREATEARCHIVE ca;
	CHashString memType(_T("Memory"));

	ca.mode = STREAM_MODE_WRITE | STREAM_MODE_READ;
	ca.streamData = NULL;
	ca.streamSize = sizeof(bool*);
	ca.streamType = &memType;

	static DWORD msgHash_CreateArchive = CHashString( _T("CreateArchive") ).GetUniqueID();
	m_ToolBox->SendMessage( msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca );

	ca.archive->Write( m_bShowFpsDisplay );

	CHashString tempName( _T("GUI_FpsDisplayVisibilySet_Event") );
	CHashString astateObjectName( _T("GameStateObject") );
	TRIGGEREVENTINMGRPARAMS aeventParams;
	aeventParams.EventName = &tempName;
	aeventParams.StateObjectName = &astateObjectName;
	aeventParams.EventParamsArchive = ca.archive;

	// Raise the event
	static DWORD msgTriggerEventInStateMachine = CHashString( _T("TriggerEventInStateMachine") ).GetUniqueID();
	m_ToolBox->SendMessage( msgTriggerEventInStateMachine, sizeof(TRIGGEREVENTINMGRPARAMS), &aeventParams, NULL, NULL);

	ca.archive->Close();

	return MSG_HANDLED_PROCEED;
}

