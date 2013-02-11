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
int FileRegistered = CControlSchema::RegisterCreateObject(CHashString(_T("file")).GetUniqueID(),
											CFileViewObject::CreateFormatObject);
}

CFileViewObject::CFileViewObject()
{
	m_szViewType = _T("file");
}

CFileViewObject::CFileViewObject(StdString &name, StdString &basicType)
{
	m_szName = name;
	m_szBasicType = basicType;
	m_szDefFile = _T("");
	m_szDefExt = _T("");
	m_szExtFilter = _T("");

	m_szViewType = _T("file");
}

CFileViewObject::~CFileViewObject() 
{
	printf("delete file");
	m_szDefExt = _T("");
	m_szExtFilter = _T("");
	m_szDefFile = _T("");
}

/// Get the default file
StdString& CFileViewObject::GetDefFile()
{
	return m_szDefFile;
}

/// Set the default file
void CFileViewObject::SetDefFile(StdString &defFile)
{
	m_szDefFile = defFile;
}

/// Set the default file extension
void CFileViewObject::SetDefExt(StdString &defext)
{
	m_szDefExt = defext;
}

/// Set the extension filters
void CFileViewObject::SetExtFilter(StdString &extfilter)
{
	m_szExtFilter = extfilter;
}

/// Get the extension filters
StdString& CFileViewObject::GetExtFilter()
{
	return m_szExtFilter;
}

/// Get the default file extension
StdString& CFileViewObject::GetDefExt()
{
	return m_szDefExt;
}

ViewFormatObject* CFileViewObject::CreateFormatObject(IXMLArchive &ar)
{
	CFileViewObject *fileObject = new CFileViewObject();

	StdString type;
	StdString name;
	StdString fileName;
	StdString defExt;
	StdString filter;

	ar.Read(type);
	ar.Read(name);
	ar.Read(fileName);
	ar.Read(defExt);
	ar.Read(filter);

	fileObject->SetName(name);
	fileObject->SetType(type);
	fileObject->SetDefFile(fileName);
	fileObject->SetDefExt(defExt);
	fileObject->SetExtFilter(filter);

	return fileObject;
}
