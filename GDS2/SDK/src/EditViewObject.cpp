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
int EditRegistered = CControlSchema::RegisterCreateObject(CHashString(_T("edit")).GetUniqueID(),
														  CEditViewObject::CreateFormatObject);
}

CEditViewObject::CEditViewObject()
{
	m_szViewType = _T("edit");
}

CEditViewObject::CEditViewObject(StdString &name, StdString &basicType)
{
	m_szName = name;
	m_szBasicType = basicType;
	m_szViewType = _T("edit");

	m_szViewType = _T("edit");
}

CEditViewObject::~CEditViewObject()
{
}

/// \brief	Creates an editable item and adds it to the tree
///			Will also create and return ViewFormatObject containing 
///			formatting data for the created option tree item.
/// \param	ar - the xml archive to read data from
/// \return	ViewFormatObject* - returns a new ViewFormatObject
ViewFormatObject* CEditViewObject::CreateFormatObject(IXMLArchive &ar)
{
	CEditViewObject *editObject = new CEditViewObject();

	StdString type;
	StdString name;
	bool bKey = false;
	int iVal = 0;
	float fVal = 0;
	double dVal = 0;
	StdString szVal = _T("");

	ar.Read(type);
	ar.Read(name);
	ar.Read(bKey);

	editObject->SetName(name);
	editObject->SetType(type);
	editObject->SetKey(bKey);

	return editObject;
}

