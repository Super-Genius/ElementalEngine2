///=====================================================================
/// \file	TextureDataAction.cpp
/// \brief	Implementation of CTextureDataAction
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
#include "StdAfx.h"

CTextureDataAction::CTextureDataAction(IHashString *pFilePath)
	: m_hashFilePath(pFilePath->GetUniqueID())
{
	ASSERT(pFilePath->GetString() != NULL && *pFilePath->GetString() != _T('\0'));
}

CTextureDataAction::~CTextureDataAction()
{
}

///	\brief	initialize this action
///			the action can be executed if return value is MSG_HANDLED
///	\return	code of EE message result
DWORD CTextureDataAction::Initialize()
{
	ITextureObject *pTexture = LoadTexture(m_hashFilePath);
	return GetTextureData(pTexture, m_Data);
}

///	\brief	execute command
///	\return	code of EE message result
DWORD CTextureDataAction::Execute()
{
	ASSERT(!m_Data.empty());
	ITextureObject *pTexture = LoadTexture(m_hashFilePath);
	if (pTexture != NULL)
	{
		// write texture buffer
		ASSERT(GetTextureDataSize(pTexture) == m_Data.size());
		pTexture->Write(&m_Data.front());
	}

	return MSG_ERROR;
}

///	\brief	determine useless undo action
///			undo action checks against current state
///	\return	true if any change exists
bool CTextureDataAction::HasChanges() const
{
	if (m_Data.empty())
	{
		return false;
	}
	Buffer currentData;
	ITextureObject *pTexture = LoadTexture(m_hashFilePath);
	if (MSG_HANDLED != GetTextureData(pTexture, currentData) || currentData.empty())
	{
		return false;
	}

	if (m_Data.size() != currentData.size())
	{
		return true;
	}

	return equal(m_Data.begin(), m_Data.end(), currentData.begin());
}
