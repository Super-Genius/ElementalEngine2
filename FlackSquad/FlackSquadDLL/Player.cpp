///==========================================================================
/// \file	Player.cpp
/// \brief	Class implementation for CPlayer
/// \date	12/22/2006
/// \author	Patrick Ghiocel
///==========================================================================

#include "StdAfx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static MathUtil math;
#define JOYSTICK_DEADZONE 0.25f

CPlayer::CPlayer()
{
	m_ToolBox = EngineGetToolBox();
	for( int i = 0; i < MAXNUMJOYSTICKS; i++ )
	{
		m_vPlayerDirectionInput[i].Set(0.0f, 0.0f, 0.0f);
		m_vPlayerRDirectionInput[i].Set(0.0f, 0.0f, 0.0f);
		m_fZoomInput[i] = 0.0f;
		m_fTriggerInput[i] = 0.0f;
		m_bTriggerHeld[i] = false;
	}

	m_fTriggerThreshold = 0.5f;
	m_fAxisSensitivity = 1.25f;
}

CPlayer::~CPlayer()
{
}

void CPlayer::OnPlayerResetMotionInput( POINT deltaMouse )
{
	if ( ( deltaMouse.x == 0 ) && ( deltaMouse.y == 0 ) )
	{
		m_vPlayerDirectionInput[0].x = 0;
		m_vPlayerDirectionInput[0].y = 0;
	}
}

void CPlayer::OnPlayerMotionInput( POINT deltaMotion )
{
	GETTIMEDELTAPARAMS gtdp;
	static CHashString msgGetTimeDelta( _T("GetTimeDelta") );
	static CHashString hszDeltaName( _T("GameDeltaTimer") );

	gtdp.hsTimerName = &hszDeltaName;
	m_ToolBox->SendMessage( msgGetTimeDelta.GetUniqueID(), sizeof( GETTIMEDELTAPARAMS ), &gtdp );

	if( gtdp.fDeltaTime > 0 )
	{
		float fPitchDelta = (float)deltaMotion.y;
		float fYawDelta = (float)deltaMotion.x;

		m_vPlayerDirectionInput[0].x = fYawDelta / 20.0f;
		m_vPlayerDirectionInput[0].y = fPitchDelta / 20.0f;
	}
	return;
	////////////////////////////////////////////////
	// THE ORIGINAL VERSION FOLLOWS
	//GETTIMEDELTAPARAMS gtdp;
	//static CHashString msgGetTimeDelta( _T("GetTimeDelta") );
	//static CHashString hszDeltaName( _T("GameDeltaTimer") );

	//gtdp.hsTimerName = &hszDeltaName;
	//m_ToolBox->SendMessage( msgGetTimeDelta.GetUniqueID(), sizeof( GETTIMEDELTAPARAMS ), &gtdp );

	//if( gtdp.fDeltaTime > 0 )
	//{
	//	float fPitchDelta = (float)deltaMotion.y;
	//	float fYawDelta = (float)deltaMotion.x;
	//	Vec3 vDeltaMotion( fYawDelta, fPitchDelta, 0.0f );
	//	
	//	TRIGGEREVENTINMGRPARAMS aeventParams;
	//	
	//	CHashString astateObjectName( _T("GameStateObject") );
	//	CHashString aeventName( _T("Game_PlayerDeltaMotion_Event") );

	//	aeventParams.StateObjectName = &astateObjectName;
	//	aeventParams.EventName = &aeventName;

	//	// construct the archive to be sent to the event.
	//	int size = sizeof(vDeltaMotion);
	//	char *memchunk = new char[size];

	//	CREATEARCHIVE ca;
	//	ca.mode = STREAM_MODE_WRITE|STREAM_MODE_READ;
	//	ca.streamSize = size;
	//	ca.streamData = (void *)memchunk;
	//	CHashString streamType(_T("Memory"));
	//	ca.streamType = &streamType;

	//	static DWORD msgCreateArchive = CHashString( _T("CreateArchive") ).GetUniqueID();
	//	if (m_ToolBox->SendMessage(msgCreateArchive, sizeof(CREATEARCHIVE), &ca) != MSG_HANDLED)
	//	{
	//		return;
	//	}
	//	aeventParams.EventParamsArchive = ca.archive;
	//	aeventParams.EventParamsArchive->Write(vDeltaMotion);

	//	static DWORD msgTriggerEventInStateMachine = CHashString( _T("TriggerEventInStateMachine") ).GetUniqueID();
	//	DWORD retVal = m_ToolBox->SendMessage( msgTriggerEventInStateMachine, sizeof( TRIGGEREVENTINMGRPARAMS ), &aeventParams );

	//	ca.archive->Close();
		//delete [] memchunk;
	//}
}

void CPlayer::OnRadialDirectionX( int joystickId, float radialValue )
{
	m_vPlayerDirectionInput[joystickId].x = 1.0f - radialValue * 2.0f;
}

void CPlayer::OnRadialDirectionY( int joystickId, float radialValue )
{
	m_vPlayerDirectionInput[joystickId].y = 1.0f - radialValue * 2.0f;
}

void CPlayer::OnRotationalDirectionX( int joystickId, float rotationalValue )
{
	m_vPlayerRDirectionInput[joystickId].x = 1.0f - rotationalValue * 2.0f;
}

void CPlayer::OnRotationalDirectionY( int joystickId, float rotationalValue )
{
	m_vPlayerRDirectionInput[joystickId].y = 1.0f - rotationalValue * 2.0f;
}

void CPlayer::OnPlayerAButton( int joystickId, bool pressed )
{
	if (pressed)
	{
		TriggerEventInGameStateMachine( _T("Game_PlayerAButton_Pressed_Event"), joystickId );
	}
	else
	{
		TriggerEventInGameStateMachine( _T("Game_PlayerAButton_Released_Event"), joystickId );
	}	
}
	
void CPlayer::OnPlayerBButton( int joystickId, bool pressed )
{
	if (pressed)
	{
		TriggerEventInGameStateMachine( _T("Game_PlayerBButton_Pressed_Event"), joystickId );
	}
	else
	{
		TriggerEventInGameStateMachine( _T("Game_PlayerBButton_Released_Event"), joystickId );
	}
}

void CPlayer::OnPlayerXButton( int joystickId, bool pressed )
{
	if (pressed)
	{
		TriggerEventInGameStateMachine( _T("Game_PlayerXButton_Pressed_Event"), joystickId );
	}
	else
	{
		TriggerEventInGameStateMachine( _T("Game_PlayerXButton_Released_Event"), joystickId );
	}
}

void CPlayer::OnPlayerYButton( int joystickId, bool pressed )
{
	if (pressed)
	{
		TriggerEventInGameStateMachine( _T("Game_PlayerYButton_Pressed_Event"), joystickId );
	}
	else
	{
		TriggerEventInGameStateMachine( _T("Game_PlayerYButton_Released_Event"), joystickId );
	}
}

void CPlayer::OnPlayerBackButton( int joystickId, bool pressed )
{
	if (pressed)
	{
		TriggerEventInGameStateMachine( _T("Game_PlayerBackButton_Pressed_Event"), joystickId );
	}
	else
	{
		TriggerEventInGameStateMachine( _T("Game_PlayerBackButton_Released_Event"), joystickId );
	}
}

void CPlayer::OnPlayerStartButton( int joystickId, bool pressed )
{
	if (pressed)
	{
		TriggerEventInGameStateMachine( _T("Game_PlayerStartButton_Pressed_Event"), joystickId );
	}
	else
	{
		TriggerEventInGameStateMachine( _T("Game_PlayerStartButton_Released_Event"), joystickId );
	}
}

void CPlayer::OnPlayerLBButton( int joystickId, bool pressed )
{
	if (pressed)
	{
		TriggerEventInGameStateMachine( _T("Game_PlayerLBButton_Pressed_Event"), joystickId );
	}
	else
	{
		TriggerEventInGameStateMachine( _T("Game_PlayerLBButton_Released_Event"), joystickId );
	}
}

void CPlayer::OnPlayerRBButton( int joystickId, bool pressed )
{
	if (pressed)
	{
		TriggerEventInGameStateMachine( _T("Game_PlayerRBButton_Pressed_Event"), joystickId );
	}
	else
	{
		TriggerEventInGameStateMachine( _T("Game_PlayerRBButton_Released_Event"), joystickId );
	}
}

void CPlayer::OnPlayerLeftAnalog( int joystickId, bool pressed )
{
	if (pressed)
	{
		TriggerEventInGameStateMachine( _T("Game_PlayerLeftAnalogButton_Pressed_Event"), joystickId );
	}
	else
	{
		TriggerEventInGameStateMachine( _T("Game_PlayerLeftAnalogButton_Released_Event"), joystickId );
	}
}

void CPlayer::OnPlayerDPadUp( int joystickId, bool pressed )
{
	if (pressed)
	{
		TriggerEventInGameStateMachine( _T("Game_PlayerDPadUp_Pressed_Event"), joystickId );
	}
	else
	{
		TriggerEventInGameStateMachine( _T("Game_PlayerDPadUp_Released_Event"), joystickId );
	}
}

void CPlayer::OnPlayerDPadDown( int joystickId, bool pressed )
{
	if (pressed)
	{
		TriggerEventInGameStateMachine( _T("Game_PlayerDPadDown_Pressed_Event"), joystickId );
	}
	else
	{
		TriggerEventInGameStateMachine( _T("Game_PlayerDPadDown_Released_Event"), joystickId );
	}
}

void CPlayer::OnPlayerDPadLeft( int joystickId, bool pressed )
{
	if (pressed)
	{
		TriggerEventInGameStateMachine( _T("Game_PlayerDPadLeft_Pressed_Event"), joystickId );
	}
	else
	{
		TriggerEventInGameStateMachine( _T("Game_PlayerDPadLeft_Released_Event"), joystickId );
	}
}

void CPlayer::OnPlayerDPadRight( int joystickId, bool pressed )
{
	if (pressed)
	{
		TriggerEventInGameStateMachine( _T("Game_PlayerDPadRight_Pressed_Event"), joystickId );
	}
	else
	{
		TriggerEventInGameStateMachine( _T("Game_PlayerDPadRight_Released_Event"), joystickId );
	}
}

void CPlayer::SetLeftMotorSpeed( JOYSTICKMOTORSPEEDPARAMS *motorSpeedParams )
{
	static DWORD msgSetMotorSpeed = CHashString( _T("SetMotorSpeed") ).GetUniqueID();

	motorSpeedParams->cMotor = EEJ_MOTOR0;
	m_ToolBox->SendMessage( msgSetMotorSpeed, sizeof(JOYSTICKMOTORSPEEDPARAMS), motorSpeedParams );
}

void CPlayer::SetRightMotorSpeed( JOYSTICKMOTORSPEEDPARAMS *motorSpeedParams )
{
	static DWORD msgSetMotorSpeed = CHashString( _T("SetMotorSpeed") ).GetUniqueID();

	motorSpeedParams->cMotor = EEJ_MOTOR1;
	m_ToolBox->SendMessage( msgSetMotorSpeed, sizeof(JOYSTICKMOTORSPEEDPARAMS), motorSpeedParams );
}

void CPlayer::WritePlayerControlVec( int joystickId, Vec3 &direction )
{
	float len = 0.0f;
	if( joystickId < 0 )
	{
		direction.Set( 0.0f, 0.0f, 0.0f );
		for( int i = 0; i < MAXNUMJOYSTICKS; i++ )
		{
			float iteratedLength = m_vPlayerDirectionInput[i].Length();
			if( iteratedLength > len )
			{
				direction = m_vPlayerDirectionInput[i];
				len = iteratedLength;
			}
		}
	}
	else
	{
		if (joystickId > MAXNUMJOYSTICKS)
		{
			EngineGetToolBox()->Log(LOGERROR, _T("Joystick is out of range!\n"));
			return;
		}
		direction = m_vPlayerDirectionInput[joystickId];
		len = direction.Length();
	}

	if( len > 0.0f )
	{	
		// No need for length calculation if controlled by mouse

		//////////////////////////////////////////////////////////
		// Original length calculation for the controller
		//float newLen = 0.0f;

		//if( len > 1.0f )
		//{
		//	newLen = 1.0f;
		//}
		//else if( len > JOYSTICK_DEADZONE )
		//{
		//	newLen = (len - JOYSTICK_DEADZONE) / (1 - JOYSTICK_DEADZONE);
		//}
		//direction *= (newLen / len); // Same as calling direction.SetLength( newLen ), but a bit more performant since we already have the vector length
	}

	if( joystickId < 0 )
	{
		// If joystick id is invalid, then take trigger values from all controllers and apply the highest
		direction.z = 0.0f;
		for( int i = 0; i < MAXNUMJOYSTICKS; i++ )
		{
			if( direction.z < m_fZoomInput[i] )
			{
				direction.z = m_fZoomInput[i];
			}
		}
	}
	else
	{
		if (joystickId > MAXNUMJOYSTICKS)
		{
			EngineGetToolBox()->Log(LOGERROR, _T("Joystick is out of range!\n"));
			return;
		}

		direction.z = m_fZoomInput[joystickId];
	}
}

void CPlayer::WritePlayerRControlVec( int joystickId, Vec3 &direction )
{
	float len = 0.0f;
	if( joystickId < 0 )
	{
		direction.Set( 0.0f, 0.0f, 0.0f );
		for( int i = 0; i < MAXNUMJOYSTICKS; i++ )
		{
			float iteratedLength = m_vPlayerRDirectionInput[i].Length();
			if( iteratedLength > len )
			{
				direction = m_vPlayerRDirectionInput[i];
				len = iteratedLength;
			}
		}
	}
	else
	{
		direction = m_vPlayerRDirectionInput[joystickId];
		len = direction.Length();
	}

	if( len > 0.0f )
	{
		float newLen = 0.0f;

		if( len > 1.0f )
		{
			newLen = 1.0f;
		}
		else if( len > JOYSTICK_DEADZONE )
		{
			newLen = (len - JOYSTICK_DEADZONE) / (1 - JOYSTICK_DEADZONE);
		}
		direction *= (newLen / len); // Same as calling direction.SetLength( newLen ), but a bit more performant since we already have the vector length
	}

	if( joystickId < 0 )
	{
		// If joystick id is invalid, then take trigger values from all controllers and apply the highest
		direction.z = 0.0f;
		for( int i = 0; i < MAXNUMJOYSTICKS; i++ )
		{
			if( direction.z < m_fTriggerInput[i] )
			{
				direction.z = m_fTriggerInput[i];
			}
		}
	}
	else
	{
		direction.z = m_fTriggerInput[joystickId];
	}
}

void CPlayer::UpdatePlayerTriggers( int joystickId, DWORD triggerKey, float val)
{
	if( joystickId >= 0 )
	{
		bool toggle = false;
		switch(triggerKey)
		{
			case EEJ_AXISZ:
			{
				// NOTE: in win32 mode, left trigger is 0.5-1.0 pressed, right trigger is 0.5-0.0 pressed
#ifdef WIN32
				if( val > 0.5f )
				{
					m_fTriggerInput[joystickId] = 0.0;
					m_fZoomInput[joystickId] = val / 0.5f - 1.0f;
				}
				else
				{
					m_fZoomInput[joystickId] = 0.0;
					m_fTriggerInput[joystickId] = 1.0f - val / 0.5f;
				}

				if( ( m_bTriggerHeld[joystickId] && m_fTriggerInput[joystickId] < m_fTriggerThreshold )
					|| ( !m_bTriggerHeld[joystickId] && m_fTriggerInput[joystickId] >= m_fTriggerThreshold ) )
				{
					toggle = true;
				}
#else
				m_fZoomInput[joystickId] = val;
#endif
				break;
			}
#ifndef WIN32
			case EEJ_AXISRZ:
			{
				m_fTriggerInput[joystickId] = val;
				if( ( m_bTriggerHeld[joystickId] && m_fTriggerInput[joystickId] < m_fTriggerThreshold )
					|| ( !m_bTriggerHeld[joystickId] && m_fTriggerInput[joystickId] >= m_fTriggerThreshold ) )
				{
					toggle = true;
				}
				break;
			}
#endif
			default:
			{
				break;
			}
		}

		if( toggle )
		{
			//EngineGetToolBox()->Log(LOGWARNING, _T("Toggle trigger press!\n"));
			m_bTriggerHeld[joystickId] = !m_bTriggerHeld[joystickId];
			
			if( m_bTriggerHeld[joystickId] )
			{
				TriggerEventInGameStateMachine( _T("Game_PlayerTriggerButton_Pressed_Event"), joystickId );
			}
			else
			{
				TriggerEventInGameStateMachine( _T("Game_PlayerTriggerButton_Released_Event"), joystickId );
			}
		}
	}
}

void CPlayer::TriggerEventInGameStateMachine( const TCHAR *szEvent, int iJoystickID )
{
	static CHashString astateObjectName( _T("GameStateObject") );
	CHashString hsEvent(szEvent);

	TRIGGEREVENTINMGRPARAMS aeventParams;
	aeventParams.StateObjectName = &astateObjectName;
	aeventParams.EventName = &hsEvent;

	// TODO: This archive is created for all key events. Should we just cache it?
	int size = sizeof(iJoystickID);
	char *memchunk = new char[size];

	CREATEARCHIVE ca;
	ca.mode = STREAM_MODE_WRITE|STREAM_MODE_READ;
	ca.streamSize = size;
	ca.streamData = (void *)memchunk;
	CHashString streamType(_T("Memory"));
	ca.streamType = &streamType;

	static DWORD msgCreateArchive = CHashString( _T("CreateArchive") ).GetUniqueID();
	if( m_ToolBox->SendMessage(msgCreateArchive, sizeof(CREATEARCHIVE), &ca) == MSG_HANDLED )
	{
		aeventParams.EventParamsArchive = ca.archive;
		aeventParams.EventParamsArchive->Write( iJoystickID );
		
		static DWORD msgTriggerEventInStateMachine = CHashString( _T("TriggerEventInStateMachine") ).GetUniqueID();
		m_ToolBox->SendMessage( msgTriggerEventInStateMachine, sizeof( TRIGGEREVENTINMGRPARAMS ), &aeventParams );

		ca.archive->Close();
	}
	delete [] memchunk;
}


/// Fire button and key event handler - start firing
/// Written by Zsolt
void CPlayer::OnPlayerFirePressed() 
{
	TriggerEventInGameStateMachine( _T("Game_PlayerTriggerButton_Pressed_Event"), 0 );
}

/// fire button release event handler - stop firing 
void CPlayer::OnPlayerFireReleased()
{
	TriggerEventInGameStateMachine( _T("Game_PlayerTriggerButton_Released_Event"), 0 );
}

void CPlayer::OnPlayerFireSecondaryPressed()
{
	bool pressed = TRUE;
	OnPlayerAButton( 0, pressed );
}

void CPlayer::OnPlayerFireSecondaryReleased()
{
	bool pressed = FALSE;
	OnPlayerAButton( 0, pressed );
}

void CPlayer::OnZoomStart()
{
	m_fZoomInput[0] = 0.5f;
}

void CPlayer::OnZoomStop()
{
	m_fZoomInput[0] = 0;
}

void CPlayer::ChangeWeaponNext()
{
	TriggerEventInGameStateMachine( _T("Game_PlayerXButton_Pressed_Event"), 0 );
}

void CPlayer::ChangeWeaponPrevious()
{
	TriggerEventInGameStateMachine( _T("Game_PlayerBButton_Pressed_Event"), 0 );
}

void CPlayer::OnPlayerMenuNavigateUp()
{
	TriggerEventInGameStateMachine( _T("Game_Menu_Navigate_Up_Event"), 0 );
}

void CPlayer::OnPlayerMenuNavigateDown()
{
	TriggerEventInGameStateMachine( _T("Game_Menu_Navigate_Down_Event"), 0 );
}

void CPlayer::OnPlayerMenuNavigateLeft()
{
	TriggerEventInGameStateMachine( _T("Game_Menu_Navigate_Left_Event"), 0 );
}

void CPlayer::OnPlayerMenuNavigateRight()
{
	TriggerEventInGameStateMachine( _T("Game_Menu_Navigate_Right_Event"), 0 );
}