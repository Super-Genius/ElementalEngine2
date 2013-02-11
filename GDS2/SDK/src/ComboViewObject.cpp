///============================================================================
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
///============================================================================

#include "StdAfx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern "C"
{
int ComboRegistered = CControlSchema::RegisterCreateObject(CHashString(_T("combo")).GetUniqueID(),
											CComboViewObject::CreateFormatObject);
}

CComboViewObject::CComboViewObject() 
{
	m_ComboList = new COMBOLIST;

	m_szViewType = _T("combo");
}

CComboViewObject::CComboViewObject(StdString &name, StdString &basicType)
{
	m_szName = name;
	m_szBasicType = basicType;
	m_ComboList = new COMBOLIST;
	m_szViewType = _T("combo");
}

CComboViewObject::~CComboViewObject() 
{
	delete m_ComboList;
}

/// Get the selected item
StdString& CComboViewObject::GetDefSelect()
{
	return m_szDefSelect;
}

/// Set the selected item
void CComboViewObject::SetDefSelect(StdString &select)
{	
	m_szDefSelect = select;
}	

/// Add a selectable item to the dropdown list
void CComboViewObject::AddComboEntry(StdString &entry)
{
	m_ComboList->push_back(entry);
}

/// Get the dropdown list
COMBOLIST* CComboViewObject::GetComboList()
{
	return m_ComboList;
}

ViewFormatObject* CComboViewObject::CreateFormatObject(IXMLArchive &ar)
{
	CComboViewObject *comboObject = new CComboViewObject();

	int count = 0;
	StdString type;
	StdString name;
	StdString defSelect;
	StdString item;
	bool bKey = false;

	ar.Read(type);
	ar.Read(name);
	ar.Read(bKey);
	ar.Read(defSelect);
	ar.Read(count);
	
	comboObject->SetDefSelect(defSelect);
	comboObject->SetName(name);
	comboObject->SetType(type);
	comboObject->SetKey(bKey);

	while(count != 0)
	{
		ar.Read(item);
		comboObject->AddComboEntry(item);
		count--;
	}

	return comboObject;
}
