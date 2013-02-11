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
int RadioRegistered = CControlSchema::RegisterCreateObject(CHashString(_T("radio")).GetUniqueID(),
											CRadioViewObject::CreateFormatObject);
}

CRadioViewObject::CRadioViewObject() 
{
	m_szViewType = _T("radio");

	m_RadioList = new RADIOLIST;
}

CRadioViewObject::CRadioViewObject(StdString &name, StdString &basicType)
{
	m_szName = name;
	m_szBasicType = basicType;
	m_szViewType = _T("edit");
	m_szViewType = _T("radio");

	m_RadioList = new RADIOLIST;
}

CRadioViewObject::~CRadioViewObject() 
{
	delete m_RadioList;
}

/// Get the list of selectable items
RADIOLIST* CRadioViewObject::GetRadioList()
{
	return m_RadioList;
}

/// Get the selected item
StdString& CRadioViewObject::GetDefSelect()
{
	return m_szDefSelect;
}

/// Set the item that is initially selected
void CRadioViewObject::SetDefSelect(StdString& defSelect)
{
	m_szDefSelect = defSelect;
}

/// Add a selectable item
void CRadioViewObject::AddListEntry(StdString &entry)
{
	m_RadioList->push_back(entry);
}

ViewFormatObject* CRadioViewObject::CreateFormatObject(IXMLArchive &ar)
{
	CRadioViewObject *radioObject = new CRadioViewObject();

	int count;
	StdString type;
	StdString name;
	StdString defSelect;
	StdString item;

	ar.Read(type);
	ar.Read(name);
	ar.Read(defSelect);
	ar.Read(count);
	
	radioObject->SetName(name);
	radioObject->SetType(type);
	radioObject->SetDefSelect(defSelect);

	while(count != 0)
	{
		ar.Read(item);
		radioObject->AddListEntry(item);
		count--;
	}

	return radioObject;
}
