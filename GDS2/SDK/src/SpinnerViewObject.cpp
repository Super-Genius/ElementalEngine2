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
int SpinnerRegistered = CControlSchema::RegisterCreateObject(CHashString(_T("spinner")).GetUniqueID(),
											CSpinnerViewObject::CreateFormatObject);
}

CSpinnerViewObject::CSpinnerViewObject()
{
	m_szViewType = _T("spinner");
	m_fDefaultVal = 0.f;
}

CSpinnerViewObject::CSpinnerViewObject(StdString &name, StdString &basicType)
{
	m_szName = name;
	m_szBasicType = basicType;
	m_fMin = 0.f;
	m_fMax = 0.f;
	m_fIncVal = 0.f;

	m_szViewType = _T("spinner");
	m_fDefaultVal = 0.f;
}

CSpinnerViewObject::~CSpinnerViewObject()
{
}

// Set the min, max range, and increment value for the spinner
void CSpinnerViewObject::SetSpinnerFormat(float fMin, float fMax, float fIncVal)
{
	m_fMin = fMin;
	m_fMax = fMax;
	m_fIncVal = fIncVal;
}

/// Get the minimum range
float CSpinnerViewObject::GetMin()
{
	return m_fMin;
}

/// Get the maximum range
float CSpinnerViewObject::GetMax()
{
	return m_fMax;
}

/// Get the increment value
float CSpinnerViewObject::GetIncVal()
{
	return m_fIncVal;
}


ViewFormatObject* CSpinnerViewObject::CreateFormatObject(IXMLArchive &ar)
{
	CSpinnerViewObject* spinnerObject = new CSpinnerViewObject();

	StdString type;
	StdString name;
	int iVal = 0;
	float fVal = 0;
	double dVal= 0;

	float fMin;
	float fMax;
	float fIncVal;
	float fDefaultVal;

	ar.Read(type);
	ar.Read(name);
	ar.Read(fMin);
	ar.Read(fMax);
	ar.Read(fIncVal);
	ar.Read(fDefaultVal);

	spinnerObject->SetName(name);
	spinnerObject->SetType(type);
	spinnerObject->SetSpinnerFormat(fMin, fMax, fIncVal);
	spinnerObject->SetDefaultValue(fDefaultVal);
	
	return spinnerObject;
}
