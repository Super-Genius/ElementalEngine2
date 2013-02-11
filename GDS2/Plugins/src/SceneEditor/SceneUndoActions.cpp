///=====================================================================
/// \file	SceneUndoActions.cpp
/// \brief	Declaration of actions for filling undo and redo stack
/// \date	11/18/2007
/// \author	Andrey Ivanov
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
///=====================================================================
#include "stdafx.h"

/////////////// CWorldChangeNameAction /////////////////////////////////////////
///	\brief	execute command
///	\return	code of EE message result
DWORD CWorldChangeNameAction::Execute()
{
	// preparing parameters for sending notify message
	WORLDCHANGEFILENAMEPARAMS wchfp;
	wchfp.oldName = const_cast<LPTSTR>(m_sName.c_str());
	wchfp.newName = const_cast<LPTSTR>(m_sNewName.c_str());

	static DWORD msgHash_WorldChangeFileName = CHashString(_T("WorldChangeFileName")).GetUniqueID();
	m_pToolBox->SendMessage(msgHash_WorldChangeFileName, sizeof(wchfp), &wchfp);

	// make file name up to date
	SINGLETONINSTANCE(CSceneEditor)->SetWorldFileName(m_sNewName.c_str());

	return MSG_HANDLED;
}
