///==========================================================================
/// \file	CLanguageEntry.cpp
/// \brief	Implementation for a language entry
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

REGISTER_COMPONENT(CLanguageEntry);

CLanguageEntry::CLanguageEntry(IHashString *parentName, IHashString *name) :
	OBJECTTEMPLATE(CLanguageManager, CLanguageEntry, ILanguageEntry, parentName, name)
{
	m_hszCompType = _T("CLanguageEntry");
	m_hszKeyString = _T("");
	m_hszText = _T("");
	m_hszEntryText = _T("");
}

CLanguageEntry::~CLanguageEntry()
{}

IComponent* CLanguageEntry::Create(int nArgs, va_list argptr)
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
		self = new CLanguageEntry(parentName, name);
	}
	catch(...)
	{
		return NULL;
	}
	return self;
}

void CLanguageEntry::Serialize( IArchive &ar )
{
	float saveVersion = 1.0f;
	
	if (ar.IsReading())
	{
		float readVersion;
		StdString szKey;
		StdString szText;
		UINT iNumParams;

		ar.Read(readVersion, _T("Version"));
		ar.Read(szKey, _T("KeyString"));
		ar.Read(szText, _T("Text"));
		ar.Read(iNumParams, _T("NumParams"));

		m_hszKeyString = szKey;
		m_hszText = szText;

		for (UINT i=0; i<iNumParams; i++)
		{
			StdString szName, szType;
			ar.Read(szName, _T("ParamName"));
			ar.Read(szType, _T("ParamType"));

			CHashString hszName(szName);
			CHashString hszType(szType);

			static DWORD dwString = CHashString(_T("string")).GetUniqueID();
			static DWORD dwFloat = CHashString(_T("float")).GetUniqueID();
			static DWORD dwInt = CHashString(_T("int")).GetUniqueID();

			FORMATPARAMTYPE eType;
			if (hszType.GetUniqueID() == dwString)
			{
				eType = P_STRING;
			}
			else if (hszType.GetUniqueID() == dwFloat)
			{
				eType = P_FLOAT;
			}
			else if (hszType.GetUniqueID() == dwInt)
			{
				eType = P_INT;
			}
			else
			{
				m_ToolBox->Log(LOGERROR, _T("%s %d: parameter %s is not a valid format!\n"), 
					__FILE__, __LINE__, hszName.GetString());
			}

			ENTRYPARAM entry;
			entry.dwHashName = hszName.GetUniqueID();
			entry.eParamType = eType;
			m_vParamVector.push_back(entry);
		}
	}
	else
	{
		ar.Write(saveVersion, _T("Version"));
		ar.Write(m_hszKeyString.GetString(), _T("KeyString"));
		ar.Write(m_hszText.GetString(), _T("Text"));
		ar.Write((int)m_vParamVector.size(), _T("NumParams"));

		for (UINT i=0; i<m_vParamVector.size(); i++)
		{
			ar.StartClass(_T("Param"));

			StdString szName = m_ToolBox->GetHashString(m_vParamVector[i].dwHashName);
			ar.Write(szName, _T("ParamName"));

			switch(m_vParamVector[i].eParamType)
			{
				case P_STRING:
				{
					ar.Write(_T("string"), _T("ParamType"));
					break;
				}
				case P_INT:
				{
					ar.Write(_T("int"), _T("ParamType"));
					break;
				}
				case P_FLOAT:
				{
					ar.Write(_T("float"), _T("ParamType"));
					break;
				}
			}

			ar.EndClass();
		}
	}
}

IHashString* CLanguageEntry::GetComponentType()
{
	return &m_hszCompType;
}

bool CLanguageEntry::IsKindOf(IHashString *compType)
{
	if (compType)
	{
		return (compType->GetUniqueID() == m_hszCompType.GetUniqueID());
	}
	return false;
}

IHashString* CLanguageEntry::GetKey()
{
	return &m_hszKeyString;
}

IHashString* CLanguageEntry::GetEntryText(IArchive *ar)
{
	if (ar)
	{
		ar->SetIsWriting(false);
		ar->SeekTo(0);
	}
	// first, get map of param strings by param name
	map<DWORD, StdString> paramStringMap;
	for (UINT i=0; i<m_vParamVector.size(); i++)
	{
		StdString temp;
		DWORD name = FillInParamText(ar, i, temp);
		paramStringMap[name] = temp;
	}

	// now, iterate thru actual string and place params where they need to go
	StdString szSource = m_hszText.GetString();
	StdString szOutputText = _T("");
	StdString szToken;

	// get first token, add it right away
	szSource.GetToken(_T("%"), szToken);
	szOutputText += szToken;
	bool bDone = false;
	do
	{
		// format should be %index% (one-indexed, not zero-indexed)
		// this will be closing %
		bDone = !szSource.GetToken(_T("%"), szToken);
		if (_tcscmp(szToken, _T("")) != 0)
		{
			CHashString hszParamName(szToken);
			map<DWORD, StdString>::iterator mapIter = paramStringMap.find(hszParamName.GetUniqueID());
			if (mapIter != paramStringMap.end())
			{
				// convert to zero-index
				szOutputText += mapIter->second;
			}
			else
			{
				// bad formatting
				m_ToolBox->Log(LOGWARNING, _T("%s %d: bad formatting in source text for entry %s\n"), 
					__FILE__, __LINE__, m_hszKeyString.GetString());
			}
		}
		else if (!bDone)
		{
			// if "" was returned, and the eos hasn't been reached, could be %% (which outputs %)
			szOutputText += _T("%");
		}

		// this will be opening for next param, so push everything in between onto output string
		szSource.GetToken(_T("%"), szToken);
		szOutputText += szToken;
		
	}while (!bDone);

	m_hszEntryText = szOutputText;
	// safe, since it's not local
	return &m_hszEntryText;
}

DWORD CLanguageEntry::FillInParamText(IArchive *ar, UINT index, StdString &szOutput)
{
	szOutput = _T("");
	
	if (index < m_vParamVector.size())
	{
		FORMATPARAMTYPE type = m_vParamVector[index].eParamType;

		if ((ar) && (!ar->IsEndOfFile()))
		{
			switch(type)
			{
				case P_STRING:
				{
					ar->Read(szOutput);
					break;
				}
				case P_INT:
				{
					TCHAR buf[32];
					int val;
					ar->Read(val);
					_stprintf(buf, _T("%d"), val);
					szOutput = buf;
					break;
				}
				case P_FLOAT:
				{
					TCHAR buf[32];
					float val;
					ar->Read(val);

					// TODO: support different float formats (i.e. %.2f, %3.4f, etc etc)
					_stprintf(buf, _T("%f"), val);
					szOutput = buf;
					break;
				}
			}
		}
		else
		{
			switch(type)
			{
				case P_STRING:
				{
					// already defaulted to empty string
					break;
				}
				case P_INT:
				{
					TCHAR buf[4];
					_stprintf(buf, _T("0"));
					szOutput = buf;
					break;
				}
				case P_FLOAT:
				{
					TCHAR buf[4];
					_stprintf(buf, _T("0.0"));
					szOutput = buf;
					break;
				}
			}
		}

		return m_vParamVector[index].dwHashName;
	}
	return 0;
}