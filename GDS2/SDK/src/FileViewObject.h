///============================================================================
/// \file		FileViewObject.h
/// \brief		Header file for file selection object in a view
/// \date		08-03-2005
/// \author		Kenneth Hurley
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

#ifndef _FILEVIEWOBJECT_H_
#define _FILEVIEWOBJECT_H_

class CFileViewObject : public IFileViewObject
{
public:

	CFileViewObject();
	CFileViewObject(StdString &name, StdString &basicType);
	~CFileViewObject();
	/// \brief	Creates a spinner item and adds it to the tree
	/// \param	ar - the xml archive to read data from
	/// \return	ViewFormatObject* - returns a new ViewFormatObject 
	static ViewFormatObject* CreateFormatObject(IXMLArchive &ar);


	/// Get the default file
	StdString& GetDefFile();

	/// Set the default file
	void SetDefFile(StdString &defFile);

	/// Set the default file extension
	void SetDefExt(StdString &defext);

	/// Set the extension filters
	void SetExtFilter(StdString &extfilter);

	/// Get the extension filters
	StdString& GetExtFilter();

	/// Get the default file extension
	StdString& GetDefExt();

private:
	/// the default file extension
	StdString	m_szDefExt;
	/// the extension filters
	StdString	m_szExtFilter;
	/// the default file
	StdString	m_szDefFile;
};

#endif // #ifndef _FILEVIEWOBJECT_H_
