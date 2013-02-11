///=====================================================================
/// \file	EditingProperties.cpp
/// \brief	Implementation of CEditingProperties
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
REGISTER_COMPONENT(CEditingProperties);

CEditingProperties::CEditingProperties(IHashString *parentName, IHashString *name)
	: CObjectTemplate<IObject>(_T(""), _T("CEditingPropertiesHolder"), parentName, name)
{
}

CEditingProperties::~CEditingProperties()
{
}

IComponent *CEditingProperties::Create(int nArgs, va_list argptr)
{
	IHashString *name;
	IHashString *parentName;
	IObject *self;

	name = va_arg(argptr, IHashString *);
	parentName = va_arg(argptr, IHashString *);
	try
	{
		// try and create an instance
		self = new CEditingProperties(parentName, name);
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

/// Serialize this component to the archive.
///	\param ar = reference to the archive class
void CEditingProperties::Serialize(IArchive &ar)
{
	if (ar.IsReading())
	{
		int count;
		ar.Read(count, _T("Count"));
		
		StdString name;
		StdString value;
		TCHAR bufName[16] = _T("Name");
		TCHAR bufValue[16] = _T("Value");

		LPTSTR pNameNum = bufName + _tcslen(bufName);
		LPTSTR pValueNum = bufValue + _tcslen(bufValue);
		for (int i = 1; i <= count; ++i)
		{
			// prepare tags
			_itot(i, pNameNum, 10);
			_tcscpy(pValueNum, pNameNum);

			ar.Read(name, bufName);
			ar.Read(value, bufValue);
			m_properties.insert(make_pair(name, value));
		}
	}
	else
	{
		ASSERT(!"This object does not support serialization");
		m_ToolBox->Log(LOGERROR, _T("This object does not support serialization"));
	}
}

void CEditingProperties::Init()
{
	CEditingPropertiesHolder *pManager = SINGLETONINSTANCE(CEditingPropertiesHolder);
	pManager->SetProperties(GetParentName(), m_properties);

	DELETEOBJECTPARAMS dop;
	dop.name = GetName();
	static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof(dop), &dop);
}
