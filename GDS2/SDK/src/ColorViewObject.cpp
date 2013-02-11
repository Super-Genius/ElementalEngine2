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
int ColorRegistered = CControlSchema::RegisterCreateObject(CHashString(_T("color")).GetUniqueID(),
											CColorViewObject::CreateFormatObject);
}

CColorViewObject::CColorViewObject()
{
	m_szViewType = _T("color");
	m_iDefColor = 0;
}

CColorViewObject::CColorViewObject(StdString &name, StdString &basicType)
{
	m_szName = name;
	m_szBasicType = basicType;

	m_iDefColor = 0;

	m_szViewType = _T("color");
}

CColorViewObject::~CColorViewObject()
{
}

ViewFormatObject* CColorViewObject::CreateFormatObject(IXMLArchive &ar)
{
	CColorViewObject *colorObject = new CColorViewObject();

	StdString type;
	StdString name;
	int defColor;

	ar.Read(type);
	ar.Read(name);
	ar.Read(defColor);

	colorObject->SetName(name);
	colorObject->SetType(type);
	colorObject->SetDefaultColor(defColor);

	return colorObject;
}
