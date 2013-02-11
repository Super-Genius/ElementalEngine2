///=====================================================================
/// \file	TextureDataAction.h
/// \brief	Declaration of CTextureDataAction
///			This class implements funcitonality for reading object 
///			proeprties and setting they back on Execute
/// \date	10/02/2007
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
#ifndef _GDS_UNDOREDO_TEXTURE_DATA_ACTION_H_
#define _GDS_UNDOREDO_TEXTURE_DATA_ACTION_H_

#include "IUndoRedoAction.h"

class CTextureDataAction : public IUndoRedoAction
{
public:
	CTextureDataAction(IHashString *pFilePath);
	virtual ~CTextureDataAction();

	///	\brief	initialize this action
	///			the action can be executed if return value is MSG_HANDLED
	///	\return	code of EE message result
	virtual DWORD Initialize();

	///	\brief	execute command
	///	\return	code of EE message result
	virtual DWORD Execute();

	///	\brief	determine useless undo action
	///			undo action checks against current state
	///	\return	true if any change exists
	virtual bool HasChanges() const;

// helper functions for interaction with command data class

	///	\brief	get hash of texture path name
	///	\return	hash value
	DWORD GetName() const
	{
		return m_hashFilePath;
	}

	///	\brief	fill passed buffer with texture data
	///	\param	data [out] - copy of saved texture data
	void GetData(Buffer &data) const
	{
		data = m_Data;
	}

private:
	DWORD	m_hashFilePath;
	Buffer	m_Data;
};

#endif // _GDS_UNDOREDO_TEXTURE_DATA_ACTION_H_
