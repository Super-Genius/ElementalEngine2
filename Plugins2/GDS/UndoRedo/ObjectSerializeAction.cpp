///=====================================================================
/// \file	ObjectSerializeAction.cpp
/// \brief	Implementation of CObjectSerializeAction
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

CObjectSerializeAction::CObjectSerializeAction(IHashString *pName)
	: m_hashName(pName->GetUniqueID())
	, m_pData(NULL)
{
	ASSERT(*pName->GetString() != _T('\0'));
}

CObjectSerializeAction::~CObjectSerializeAction()
{
	if (m_pData != NULL)
	{
		m_pData->Close();
		m_pData = NULL;
	}
}

///	\brief	initialize this action
///			the action can be executed if return value is MSG_HANDLED
///	\return	code of EE message result
DWORD CObjectSerializeAction::Initialize()
{
	IToolBox *pToolBox = EngineGetToolBox();

	CHashString hszName = pToolBox->GetHashString(m_hashName);

	m_pData = GetEEObjectProperties(&hszName);
	if (m_pData == NULL)
	{
		return MSG_ERROR;
	}

	m_pData->SetIsWriting(true);
	m_pData->SeekTo(0, SEEK_END);

	EDITINGPROPERTIESPARAMS epp;
	epp.name = &hszName;
	static const DWORD hashMsg_GetEditingProperties = CHashString(_T("GetEditingProperties")).GetUniqueID();
	if (MSG_HANDLED == pToolBox->SendMessage(hashMsg_GetEditingProperties, sizeof(epp), &epp))
	{
		m_pData->Write((int)epp.params.size(), _T("count"));
		map<StdString, StdString>::const_iterator it = epp.params.begin();
		for( ; it != epp.params.end(); ++it)
		{
			m_pData->Write(it->first, _T("name"));
			m_pData->Write(it->second, _T("value"));
		}
	}
	else
	{
		m_pData->Write((int)0, _T("count"));
	}
	m_pData->SetIsWriting(false);

	return MSG_HANDLED;
}

///	\brief	execute command
///	\return	code of EE message result
DWORD CObjectSerializeAction::Execute()
{
	ASSERT(m_pData != NULL);
	m_pData->SeekTo(0);

	CFileVersionSetter lastVersionSetter;

	IToolBox *pToolBox = EngineGetToolBox();

	CHashString hszName = pToolBox->GetHashString(m_hashName);
	// setup up serialization parameters
	SERIALIZEOBJECTPARAMS sop;
	sop.name = &hszName;
	sop.archive = m_pData;
	// serialize object from archive
	static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
	DWORD res = pToolBox->SendMessage(msgHash_SerializeObject, sizeof(sop), &sop);
	if (res != MSG_HANDLED)
	{
		// log this error?
		return res;
	}

	int count = 0;
	m_pData->Read(count, _T("count"));
	if (count > 0)
	{
		EDITINGPROPERTIESPARAMS epp;
		epp.name = &hszName;
		while (count--)
		{
			StdString name;
			StdString value;
			m_pData->Read(name, _T("name"));
			m_pData->Read(value, _T("value"));
			epp.params.insert(make_pair(name, value));
		}
		static const DWORD hashMsg_SetEditingProperties = CHashString(_T("SetEditingProperties")).GetUniqueID();
		pToolBox->SendMessage(hashMsg_SetEditingProperties, sizeof(epp), &epp);
	}

	INITOBJECTPARAMS iop;
	// send message to EE to initialize the object
	iop.name = &hszName;
	static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
	res = pToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop);

	return res;
}

///	\brief	determine useless undo action
///	\return	true if any change exists
bool CObjectSerializeAction::HasChanges() const
{
	return true;
}

///	\brief	fill passed buffer with archive data
///	\param	data [out] - copy of saved archive stream data
void CObjectSerializeAction::GetData(Buffer &data) const
{
	GetArchiveStreamData(m_pData, data);
}