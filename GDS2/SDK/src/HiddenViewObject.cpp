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
int HiddenRegistered = CControlSchema::RegisterCreateObject(CHashString(_T("hidden")).GetUniqueID(),
															 CHiddenViewObject::CreateFormatObject);
}
	
CHiddenViewObject::CHiddenViewObject()
{
	m_szViewType = _T("hidden");
}

CHiddenViewObject::CHiddenViewObject(StdString &name, StdString &basicType)
{
	m_szName = name;
	m_szBasicType = basicType;
	m_szViewType = _T("hidden");
}

CHiddenViewObject::~CHiddenViewObject()
{
}

ViewFormatObject* CHiddenViewObject::CreateFormatObject(IXMLArchive &ar)
{
	CHiddenViewObject *hiddenObject = new CHiddenViewObject();

	StdString type;

	ar.Read(type);

	hiddenObject->SetType(type);

	return hiddenObject;
}
