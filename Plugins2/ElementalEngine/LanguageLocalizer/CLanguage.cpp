///==========================================================================
/// \file	CLanguage.cpp
/// \brief	Implementation for a language
/// \date	06/16/2008
/// \author	Nick Tourte
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
///==========================================================================

#include "stdafx.h"

REGISTER_COMPONENT(CLanguage);

CLanguage::CLanguage(IHashString *parentName, IHashString *name) :
	OBJECTTEMPLATE(CLanguageManager, CLanguage, ILanguage, parentName, name)
{
	m_hszCompType = _T("CLanguage");
	m_hszLanguageName = _T("");
	m_iNumEntries = 0;
}

CLanguage::~CLanguage()
{
	// go ahead and delete your child entries
	DELETEOBJECTPARAMS dop;
	static DWORD msgDeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
	ENTRYKEYMAP::iterator mapIter = m_mEntryMapByKey.begin();
	for(; mapIter != m_mEntryMapByKey.end(); mapIter++)
	{
		ILanguageEntry *entry = mapIter->second;
		if (entry)
		{
			dop.name = entry->GetName();
			m_ToolBox->SendMessage(msgDeleteObject, sizeof(DELETEOBJECTPARAMS), &dop);
		}
	}
	m_mEntryMapByKey.clear();
}

IComponent* CLanguage::Create(int nArgs, va_list argptr)
{
	IHashString *name;
	IHashString *parentName;
	IObject *self;

	name = va_arg(argptr, IHashString *);
	parentName = va_arg(argptr, IHashString *);
	// create the object however you must create it
	try
	{
		// try and create an instance
		self = new CLanguage(parentName, name);
	}
	catch(...)
	{
		return NULL;
	}
	return self;
}

void CLanguage::Serialize( IArchive &ar )
{
	float saveVersion = 1.0f;
	if (ar.IsReading())
	{
		float readVersion;
		StdString szLanguageName;
		
		ar.Read(readVersion, _T("Version"));
		ar.Read(szLanguageName, _T("LanguageName"));
		ar.Read(m_iNumEntries, _T("NumEntries"));
		
		m_hszLanguageName = szLanguageName;

		CREATEOBJECTPARAMS cop;
		INITOBJECTPARAMS iop;
		SERIALIZEOBJECTPARAMS sop;
		static DWORD msgCreateObject = CHashString(_T("CreateObject")).GetUniqueID();
		static DWORD msgSerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
		static DWORD msgInitObject = CHashString(_T("InitObject")).GetUniqueID();

		StdString szEntryObjName;
		CHashString hszEntryObjBase, hszEntryObjName;
		static CHashString hszEntryType(_T("CLanguageEntry"));

		for (UINT i=0; i<m_iNumEntries; i++)
		{
			szEntryObjName = m_hszLanguageName.GetString();
			szEntryObjName += _T("_Entry");
			hszEntryObjBase = szEntryObjName;

			GENERATEUNIQUEOBJECTNAMEPARAMS generateNameParams;
			generateNameParams.name = &hszEntryObjBase;
			generateNameParams.newname = &hszEntryObjName;
			static DWORD msgHash_GenerateUniqueObjectName = CHashString(_T("GenerateUniqueObjectName")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_GenerateUniqueObjectName, sizeof(generateNameParams), &generateNameParams );

			cop.name = &hszEntryObjName;
			cop.parentName = GetName();
			cop.typeName = &hszEntryType;

			if (m_ToolBox->SendMessage(msgCreateObject, sizeof(CREATEOBJECTPARAMS), &cop) == MSG_HANDLED)
			{
				sop.name = &hszEntryObjName;
				sop.archive = &ar;
				m_ToolBox->SendMessage(msgSerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop);

				iop.name = &hszEntryObjName;
				m_ToolBox->SendMessage(msgInitObject, sizeof(INITOBJECTPARAMS), &iop);

				IObject *entryObj = SINGLETONINSTANCE(CLanguageManager)->GetObjectByName(&hszEntryObjName, &hszEntryType);
				if (entryObj)
				{
					ILanguageEntry *langEntry = dynamic_cast<ILanguageEntry*>(entryObj);
					if (langEntry)
					{
						IHashString *hsKey = langEntry->GetKey();
						if (hsKey)
						{
							ENTRYKEYMAP::iterator mapIter = m_mEntryMapByKey.find(hsKey->GetUniqueID());
							if (mapIter == m_mEntryMapByKey.end())
							{
								m_mEntryMapByKey[hsKey->GetUniqueID()] = langEntry;
							}
							else
							{
								m_ToolBox->Log(LOGERROR, _T("%s %d: duplicate key found for %s, deleting language entry!\n"), 
									__FILE__, __LINE__, hsKey->GetString());

								DELETEOBJECTPARAMS dop;
								dop.name = &hszEntryObjName;
								
								static DWORD msgDeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
								m_ToolBox->SendMessage(msgDeleteObject, sizeof(DELETEOBJECTPARAMS), &dop);
							}
						}
						else
						{
							m_ToolBox->Log(LOGERROR, _T("%s %d: could not get key string from entry!\n"), 
								__FILE__, __LINE__);
						}
					}
					else
					{
						m_ToolBox->Log(LOGERROR, _T("%s %d: could not cast to language entry!\n"), 
								__FILE__, __LINE__);
					}
				}
				else
				{
					m_ToolBox->Log(LOGERROR, _T("%s %d: could not get object pointer from manager!\n"), 
								__FILE__, __LINE__);
				}
			}
			else
			{
				m_ToolBox->Log(LOGERROR, _T("%s %d: could not create entry object %s!\n"), 
					__FILE__, __LINE__, hszEntryObjName.GetString());
			}
		}
	}
	else
	{
		ar.Write(saveVersion, _T("Version"));
		ar.Write(m_hszLanguageName.GetString(), _T("LanguageName"));
		ar.Write(m_iNumEntries, _T("NumEntries"));

		ENTRYKEYMAP::iterator mapIter = m_mEntryMapByKey.begin();
		for (; mapIter != m_mEntryMapByKey.end(); mapIter++)
		{
			ILanguageEntry *entry = mapIter->second;
			if (entry)
			{
				ar.StartClass(_T("CLanguageEntry"));
				entry->Serialize(ar);
				ar.EndClass();
			}
		}
	}
}

IHashString* CLanguage::GetComponentType()
{
	return &m_hszCompType;
}

bool CLanguage::IsKindOf(IHashString *compType)
{
	if (compType)
	{
		return (compType->GetUniqueID() == m_hszCompType.GetUniqueID());
	}
	return false;
}

IHashString* CLanguage::GetLanguageName()
{
	return &m_hszLanguageName;
}

IHashString* CLanguage::GetEntry(IHashString *key, IArchive *ar)
{
	if (key)
	{
		ENTRYKEYMAP::iterator mapIter = m_mEntryMapByKey.find(key->GetUniqueID());
		if (mapIter == m_mEntryMapByKey.end())
		{
			m_ToolBox->Log(LOGWARNING, _T("%s %d: could not find entry for key %s\n"), 
				__FILE__, __LINE__, key->GetString());
		}
		else
		{
			ILanguageEntry *entry = mapIter->second;
			if (entry)
			{
				return entry->GetEntryText(ar);	
			}
		}
	}
	else
	{
		m_ToolBox->Log(LOGWARNING, _T("%s %d: key value passed in is NULL\n"), 
			__FILE__, __LINE__);
	}

	return NULL;
}