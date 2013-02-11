///=====================================================================
/// \file	SceneUndoActions.h
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
#ifndef _SCENE_UNDO_ACTIONS_H_
#define _SCENE_UNDO_ACTIONS_H_

/////////////// CWorldChangeNameAction /////////////////////////////////////////
class CWorldChangeNameAction : public CBaseUndoRedoAction
{
public:
	///	\brief	CWorldChangeNameAction constructor
	///	\param	pName - name of the world
	///	\param	pNewName - new name of the world
	CWorldChangeNameAction(LPCTSTR pName, LPCTSTR pNewName)
		: m_sName(pName)
		, m_sNewName(pNewName)
	{
	};

	///	\brief	execute command
	///	\return	code of EE message result
	virtual DWORD Execute();

private:
	StdString m_sName;
	StdString m_sNewName;
};

#endif //_SCENE_UNDO_ACTIONS_H_
