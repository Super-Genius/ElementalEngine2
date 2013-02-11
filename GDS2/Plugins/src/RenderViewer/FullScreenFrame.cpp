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

#include "stdafx.h"
#include "FullScreenFrame.h"


// CFullScreenFrame

IMPLEMENT_DYNCREATE(CFullScreenFrame, CFrameWnd)

CFullScreenFrame::CFullScreenFrame()
{
	m_ToolBox = EngineGetToolBox();
	m_StopMessage = new CHashString(_T("StopPlaying"));
}

CFullScreenFrame::~CFullScreenFrame()
{
	delete m_StopMessage;
}

void CFullScreenFrame::SetupInput()
{
	INPUTENGPARAMS inputParams;
	KEYBIND binding;
	CHashString kbName(_T("GDS_kb"));
	
	CHashString inputManager(_T("CInputManager"));
	CHashString StopMessage(_T("StopPlaying"));
	CHashString MoveForward(_T("CamMoveForward"));
	CHashString MoveBackward(_T("CamMoveBackward"));
	CHashString StrafeLeft(_T("CamStrafeLeft"));
	CHashString StrafeRight(_T("CamStrafeRight"));

	// create the input manager
	m_ToolBox->CreateComponent(&CHashString(_T("CInputManager")), 0);

	// start the input manager
	HMODULE mod = GetModuleHandle(NULL);
	inputParams.hMod = mod;
	inputParams.hWnd = GetSafeHwnd();
	inputParams.mouseImmediate = true;
	inputParams.mouseExclusive = false;
	inputParams.mouseFG = true;
	inputParams.kbImmediate = true;
	inputParams.kbExclusive = false;
	inputParams.kbFG = true;
	
	static DWORD msgHash_Begin = CHashString(_T("Begin")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_Begin, sizeof(INPUTENGPARAMS), &inputParams, NULL, &inputManager);
	static DWORD msgHash_Start = CHashString(_T("Start")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_Start, 0, NULL, NULL, &inputManager);

	binding.m_kbNameID = kbName.GetUniqueID();
	binding.m_KeyID = EEK_ESC;//0x1; 
	binding.m_MessageID = StopMessage.GetUniqueID();
	binding.m_bHitOnce = true;
	static DWORD msgHash_SetKeyBinding = CHashString(_T("SetKeyBinding")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetKeyBinding, sizeof(KEYBIND), &binding, NULL, &inputManager);

	binding.m_kbNameID = kbName.GetUniqueID();
	binding.m_KeyID = EEK_LEFT;
	binding.m_MessageID = StrafeLeft.GetUniqueID();
	binding.m_bHitOnce = false;
	m_ToolBox->SendMessage(msgHash_SetKeyBinding, sizeof(KEYBIND), &binding, NULL, &inputManager);

	binding.m_kbNameID = kbName.GetUniqueID();
	binding.m_KeyID = EEK_RIGHT;
	binding.m_MessageID = StrafeRight.GetUniqueID();
	binding.m_bHitOnce = false;
	m_ToolBox->SendMessage(msgHash_SetKeyBinding, sizeof(KEYBIND), &binding, NULL, &inputManager);

	binding.m_kbNameID = kbName.GetUniqueID();
	binding.m_KeyID = EEK_UP;
	binding.m_MessageID = MoveForward.GetUniqueID();
	binding.m_bHitOnce = false;
	m_ToolBox->SendMessage(msgHash_SetKeyBinding, sizeof(KEYBIND), &binding, NULL, &inputManager);

	binding.m_kbNameID = kbName.GetUniqueID();
	binding.m_KeyID = EEK_DOWN;
	binding.m_MessageID = MoveBackward.GetUniqueID();
	binding.m_bHitOnce = false;
	m_ToolBox->SendMessage(msgHash_SetKeyBinding, sizeof(KEYBIND), &binding, NULL, &inputManager);

	binding.m_kbNameID = kbName.GetUniqueID();
	binding.m_KeyID = EEK_A;
	binding.m_MessageID = StrafeLeft.GetUniqueID();
	binding.m_bHitOnce = false;
	m_ToolBox->SendMessage(msgHash_SetKeyBinding, sizeof(KEYBIND), &binding, NULL, &inputManager);

	binding.m_kbNameID = kbName.GetUniqueID();
	binding.m_KeyID = EEK_D;
	binding.m_MessageID = StrafeRight.GetUniqueID();
	binding.m_bHitOnce = false;
	m_ToolBox->SendMessage(msgHash_SetKeyBinding, sizeof(KEYBIND), &binding, NULL, &inputManager);

	binding.m_kbNameID = kbName.GetUniqueID();
	binding.m_KeyID = EEK_W;
	binding.m_MessageID = MoveForward.GetUniqueID();
	binding.m_bHitOnce = false;
	m_ToolBox->SendMessage(msgHash_SetKeyBinding, sizeof(KEYBIND), &binding, NULL, &inputManager);

	CHashString reloadshaders(_T("ReloadAllShaders"));
	binding.m_kbNameID = kbName.GetUniqueID();
	binding.m_KeyID = EEK_S;
	binding.m_MessageID = MoveBackward.GetUniqueID();
	binding.m_bHitOnce = false;
	m_ToolBox->SendMessage(msgHash_SetKeyBinding, sizeof(KEYBIND), &binding, NULL, &inputManager);

	binding.m_kbNameID = kbName.GetUniqueID();
	binding.m_KeyID = EEK_Q;
	binding.m_MessageID = reloadshaders.GetUniqueID();
	binding.m_bHitOnce = true;
	m_ToolBox->SendMessage(msgHash_SetKeyBinding, sizeof(KEYBIND), &binding, NULL, &inputManager);

	static DWORD msgHash_SetActiveKeyBinding = CHashString(_T("SetActiveKeyBinding")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetActiveKeyBinding, sizeof(DWORD*), &binding.m_kbNameID, &inputManager);
}

BEGIN_MESSAGE_MAP(CFullScreenFrame, CFrameWnd)
END_MESSAGE_MAP()


// CFullScreenFrame message handlers
