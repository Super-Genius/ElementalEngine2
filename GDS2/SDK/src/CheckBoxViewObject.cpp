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
int CheckBoxRegistered = CControlSchema::RegisterCreateObject(CHashString(_T("check")).GetUniqueID(),
											CCheckBoxViewObject::CreateFormatObject);
}

CCheckBoxViewObject::CCheckBoxViewObject() 
{
	m_szViewType = _T("check");
}

CCheckBoxViewObject::CCheckBoxViewObject(StdString &name, StdString &basicType)
{
	m_szName = name;
	m_szBasicType = basicType;

	m_szViewType = _T("check");
}

CCheckBoxViewObject::~CCheckBoxViewObject()
{
}

ViewFormatObject* CCheckBoxViewObject::CreateFormatObject(IXMLArchive &ar)
{
	CCheckBoxViewObject	*checkboxObject = new CCheckBoxViewObject();

	StdString type;
	StdString name;

	ar.Read(type);
	ar.Read(name);
	
	checkboxObject->SetName(name);
	checkboxObject->SetType(type);

	return checkboxObject;
	
}


