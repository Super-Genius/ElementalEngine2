///==========================================================================
/// \file	LanguageManager.cpp
/// \brief	Manager for languages (implementation)
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

REGISTER_COMPONENT_SINGLETON(CLanguageManager);
REGISTER_MESSAGE_HANDLER(LoadLanguage, OnLoadLanguage, CLanguageManager);
REGISTER_MESSAGE_HANDLER(GetLanguageText, OnGetLanguageText, CLanguageManager);
REGISTER_MESSAGE_HANDLER(ClearLanguage, OnClearLanguage, CLanguageManager);

CLanguageManager::CLanguageManager() : 
	CManager(_T("CLanguageManager"))
{
	m_CurrentLanguage = NULL;
}

CLanguageManager::~CLanguageManager()
{
	DeInit();
}

IComponent* CLanguageManager::Create(int nArgs, va_list argptr)
{
	// Return the singleton instance of this class
	return SINGLETONINSTANCE( CLanguageManager );
}

DWORD CLanguageManager::OnLoadLanguage(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(IArchive*));
	IArchive *ar = (IArchive*) param;

	// delete current language if loaded!
	if (m_CurrentLanguage)
	{
		DELETEOBJECTPARAMS dop;
		dop.name = m_CurrentLanguage->GetName();
		static DWORD msgDeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
		m_ToolBox->SendMessage(msgDeleteObject, sizeof(DELETEOBJECTPARAMS), &dop);

		m_CurrentLanguage = NULL;
	}

	if (ar)
	{
		CREATEOBJECTPARAMS cop;
		INITOBJECTPARAMS iop;
		SERIALIZEOBJECTPARAMS sop;
		static DWORD msgCreateObject = CHashString(_T("CreateObject")).GetUniqueID();
		static DWORD msgInitObject = CHashString(_T("InitObject")).GetUniqueID();
		static DWORD msgSerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();

		static CHashString hszLangName = _T("LanguageManager_CurrentLanguage");
		static CHashString hszLangType(_T("CLanguage"));

		cop.name = &hszLangName;
		cop.parentName = NULL;
		cop.typeName = &hszLangType;
		if (m_ToolBox->SendMessage(msgCreateObject, sizeof(CREATEOBJECTPARAMS), &cop) == MSG_HANDLED)
		{
			sop.name = &hszLangName;
			sop.archive = ar;
			m_ToolBox->SendMessage(msgSerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop);

			iop.name = &hszLangName;
			m_ToolBox->SendMessage(msgInitObject, sizeof(INITOBJECTPARAMS), &iop);

			// should be able to get the object from the manager map now
			IObject *langObj = GetObjectByName(&hszLangName, &hszLangType);
			if (langObj)
			{
				ILanguage *language = dynamic_cast<ILanguage*>(langObj);
				if (language)
				{
					m_CurrentLanguage = language;
				}
				else
				{
					m_ToolBox->Log(LOGERROR, _T("%s %d: could not cast to language object!\n"), 
							__FILE__, __LINE__);
					return MSG_ERROR;
				}
			}
			else
			{
				m_ToolBox->Log(LOGERROR, _T("%s %d: could not get language object!\n"), 
							__FILE__, __LINE__);
				return MSG_ERROR;
			}
		}
		else
		{
			m_ToolBox->Log(LOGERROR, _T("%s %d: could not create language object!\n"), 
							__FILE__, __LINE__);
			return MSG_ERROR;
		}
	}
	
	return MSG_HANDLED_PROCEED;
}

DWORD CLanguageManager::OnGetLanguageText(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(GETLANGUAGETEXTPARAMS));
	GETLANGUAGETEXTPARAMS *gltp = (GETLANGUAGETEXTPARAMS*)param;

	if ((gltp) && 
		(gltp->hszKeyString) &&
		(gltp->hszTranslatedText) &&
		(m_CurrentLanguage))
	{
		IHashString *ret = m_CurrentLanguage->GetEntry(gltp->hszKeyString, gltp->arParams);
		if (ret)
		{
			*gltp->hszTranslatedText = ret->GetString();
		}
	}
	else if (m_CurrentLanguage == NULL)
	{
		m_ToolBox->Log(LOGWARNING, _T("No language currently set; can not get translated text\n"));
		return MSG_NOT_HANDLED;
	}

	return MSG_HANDLED_STOP;
}

DWORD CLanguageManager::OnClearLanguage(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, 0);

	// delete current language if loaded!
	if (m_CurrentLanguage)
	{
		DELETEOBJECTPARAMS dop;
		dop.name = m_CurrentLanguage->GetName();
		static DWORD msgDeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
		m_ToolBox->SendMessage(msgDeleteObject, sizeof(DELETEOBJECTPARAMS), &dop);

		m_CurrentLanguage = NULL;
	}

	return MSG_HANDLED_PROCEED;
}