///=====================================================================
/// \file	EditingPropertiesHolder.cpp
/// \brief	Implementation of CEditingPropertiesHolder
/// \date	01/27/2008
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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////// EE registration ///////////////////////////////////////
REGISTER_COMPONENT_SINGLETON(CEditingPropertiesHolder);

REGISTER_MESSAGE_HANDLER(GetEditingProperties, OnGetEditingProperties, CEditingPropertiesHolder);
REGISTER_MESSAGE_HANDLER(SetEditingProperties, OnSetEditingProperties, CEditingPropertiesHolder);
REGISTER_MESSAGE_HANDLER(SaveEditingProperties, OnSaveEditingProperties, CEditingPropertiesHolder);
REGISTER_MESSAGE_HANDLER_PRIORITY(DeleteObject, OnDeleteObject, CEditingPropertiesHolder, 0x70000000);

CEditingPropertiesHolder::CEditingPropertiesHolder()
	: m_pToolBox(EngineGetToolBox())
{
}

CEditingPropertiesHolder::~CEditingPropertiesHolder()
{
}

IComponent *CEditingPropertiesHolder::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE(CEditingPropertiesHolder);
}

/// Serialize this component to the archive.
///	\param ar = reference to the archive class
void CEditingPropertiesHolder::Serialize(IArchive &ar)
{
}

/// return a string of the component type
/// \return IHashString *reference to textual classname
IHashString *CEditingPropertiesHolder::GetComponentType()
{
	static CHashString compName(_T("CEditingPropertiesHolder"));
	return &compName;
}

bool CEditingPropertiesHolder::IsKindOf(IHashString *compType)
{
	return *compType == *GetComponentType();
}

///	get editing properties for object with passed name
DWORD CEditingPropertiesHolder::OnGetEditingProperties(DWORD size, void *param)
{
	EDITINGPROPERTIESPARAMS *pParams = static_cast<EDITINGPROPERTIESPARAMS *>(param);
	VERIFY_MESSAGE_SIZE(size, sizeof(*pParams));
	if (pParams->name == NULL || pParams->name->IsEmpty())
	{
		m_pToolBox->Log(LOGERROR, _T("Empty object name was passed"));
		return MSG_ERROR;
	}

	map<DWORD, PropertyMap>::const_iterator it;
	it = m_Properties.find(pParams->name->GetUniqueID());
	if (it != m_Properties.end())
	{
		pParams->params = it->second;
		return MSG_HANDLED_STOP;
	}

	return MSG_NOT_HANDLED;
}

/// set editing properties for object with passed name
DWORD CEditingPropertiesHolder::OnSetEditingProperties(DWORD size, void *param)
{
	EDITINGPROPERTIESPARAMS *pParams = static_cast<EDITINGPROPERTIESPARAMS *>(param);
	VERIFY_MESSAGE_SIZE(size, sizeof(*pParams));
	if (pParams->name == NULL || pParams->name->IsEmpty())
	{
		m_pToolBox->Log(LOGERROR, _T("Empty object name was passed"));
		return MSG_ERROR;
	}

	m_Properties[pParams->name->GetUniqueID()] = pParams->params;

	return MSG_HANDLED_STOP;
}

///	save object's editing properties to archive
DWORD CEditingPropertiesHolder::OnSaveEditingProperties(DWORD size, void *param)
{
	SAVEEDITINGPROPERTIESPARAMS *pParams = static_cast<SAVEEDITINGPROPERTIESPARAMS *>(param);
	VERIFY_MESSAGE_SIZE(size, sizeof(*pParams));
	IHashString *name = pParams->name;
	if (name == NULL || name->IsEmpty())
	{
		m_pToolBox->Log(LOGERROR, _T("Empty object name was passed"));
		return MSG_ERROR;
	}

	map<DWORD, PropertyMap>::const_iterator it;
	it = m_Properties.find(name->GetUniqueID());
	if (it != m_Properties.end() && !it->second.empty())
	{
		IArchive *ar = pParams->ar;
		const PropertyMap &props = it->second;
		ar->StartClass(_T("CEditingProperties"));
		ar->Write(props.size(), _T("Count"));

		StdString name;
		StdString value;
		TCHAR bufName[16] = _T("Name");
		TCHAR bufValue[16] = _T("Value");
		LPTSTR pNameNum = bufName + _tcslen(bufName);
		LPTSTR pValueNum = bufValue + _tcslen(bufValue);

		PropertyMap::const_iterator itProp = props.begin();
		for (int i = 1; itProp != props.end(); ++itProp, ++i)
		{
			// prepare tags
			_itot(i, pNameNum, 10);
			_tcscpy(pValueNum, pNameNum);

			ar->Write(itProp->first, bufName);
			ar->Write(itProp->second, bufValue);
		}

		pParams->ar->EndClass();

		return MSG_HANDLED_STOP;
	}

	return MSG_NOT_HANDLED;
}

///	delete editing properties for passed object
DWORD CEditingPropertiesHolder::OnDeleteObject(DWORD size, void *param)
{
	DELETEOBJECTPARAMS *dop = static_cast<DELETEOBJECTPARAMS *>(param);
	VERIFY_MESSAGE_SIZE(size, sizeof(*dop));
	IHashString *name = dop->name;
	if (name == NULL || name->IsEmpty())
	{
		m_pToolBox->Log(LOGERROR, _T("Empty object name was passed"));
		return MSG_ERROR;
	}

	m_Properties.erase(name->GetUniqueID());

	return MSG_HANDLED_PROCEED;
}

void CEditingPropertiesHolder::SetProperties(IHashString *name, const PropertyMap &properties)
{
	if( name != NULL && !name->IsEmpty() )
	{
		m_Properties[name->GetUniqueID()] = properties;
	}
}
