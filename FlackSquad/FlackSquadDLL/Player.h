///==========================================================================
/// \file		Player.h
/// \brief		Handles Player Code on the C++ side of things
/// \date		01/18/2007
/// \author		Richard Nguyen
/// \note       Copyright (c) 2007 Signature Devices, Inc.
///==========================================================================

#ifndef _PLAYER_H_
#define _PLAYER_H_

class CPlayer
{
public:
	CPlayer();
	~CPlayer();

	void OnPlayerMotionInput( POINT deltaMotion );
	void OnRadialDirectionX( int joystickId, float radialValue );
	void OnRadialDirectionY( int joystickId, float radialValue );
	void OnRotationalDirectionX( int joystickId, float rotationalValue );
	void OnRotationalDirectionY( int joystickId, float rotationalValue );
	void WritePlayerControlVec( int joystickId, Vec3 &direction );
	void WritePlayerRControlVec( int joystickId, Vec3 &direction );
	void OnPlayerAButton( int joystickId, bool pressed );
	void OnPlayerBButton( int joystickId, bool pressed );
	void OnPlayerXButton( int joystickId, bool pressed );
	void OnPlayerYButton( int joystickId, bool pressed );
	void OnPlayerBackButton( int joystickId, bool pressed );
	void OnPlayerStartButton( int joystickId, bool pressed );
	void OnPlayerLBButton( int joystickId, bool pressed );
	void OnPlayerRBButton( int joystickId, bool pressed );
	void OnPlayerLeftAnalog( int joystickId, bool pressed );
	void OnPlayerDPadUp( int joystickId, bool pressed );
	void OnPlayerDPadDown( int joystickId, bool pressed );
	void OnPlayerDPadLeft( int joystickId, bool pressed );
	void OnPlayerDPadRight( int joystickId, bool pressed );
	void UpdatePlayerTriggers( int joystickId, DWORD triggerKey, float val);
	void SetAxisSensitivity(float fSensitivity) { m_fAxisSensitivity = fSensitivity; }
	float GetAxisSensitivity() { return m_fAxisSensitivity; }
	void SetLeftMotorSpeed( JOYSTICKMOTORSPEEDPARAMS *motorSpeedParams );
	void SetRightMotorSpeed( JOYSTICKMOTORSPEEDPARAMS *motorSpeedParams );
	
	// Menu actions
	void OnPlayerMenuNavigateUp();
	void OnPlayerMenuNavigateDown();
	void OnPlayerMenuNavigateLeft();
	void OnPlayerMenuNavigateRight();

	// Pressing and releasing left mouse button: firing
	void OnPlayerFirePressed();
	void OnPlayerFireReleased();
	void OnPlayerFireSecondaryPressed();
	void OnPlayerFireSecondaryReleased();
	void OnZoomStart();
	void OnZoomStop();
	void ChangeWeaponNext();
	void ChangeWeaponPrevious();

	// the joystik sends continously 0 when no motion detected.
	// For mouse and keyboard we need to do it explicit:
	void OnPlayerResetMotionInput(  POINT deltaMotion );


private:
	void TriggerEventInGameStateMachine( const TCHAR *szEventm, int iJoystickID );
	Vec3 m_vPlayerDirectionInput[MAXNUMJOYSTICKS];
	Vec3 m_vPlayerRDirectionInput[MAXNUMJOYSTICKS];
	IToolBox* m_ToolBox;
	float m_fZoomInput[MAXNUMJOYSTICKS];
	float m_fTriggerInput[MAXNUMJOYSTICKS];
	float m_fTriggerThreshold;
	bool m_bTriggerHeld[MAXNUMJOYSTICKS];
	float m_fAxisSensitivity;

};


#endif // #ifndef _PLAYER_H_