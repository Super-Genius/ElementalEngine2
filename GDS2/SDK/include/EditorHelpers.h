///=====================================================================
/// \file	EditorHelpers.h
/// \brief	Set of helper functions for editors
/// \date	11/27/2007
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
#ifndef _GDS_EDITOR_HELPERS_H
#define _GDS_EDITOR_HELPERS_H

namespace GDS_SDK_Helpers
{

///	\brief	set active scene name
///	\param	sceneName - pointer to IHashString with name of scene.
///						 empty name is set if NULL is passed
///	\return	EE return code
DWORD SetActiveScene(IHashString *sceneName = NULL);

///	\brief	get active scene name
///	\return	active scene name
CHashString GetActiveScene();

///	\brief	find opened document in doc template document list
///			and activate first view for this document
///	\param	pDocTemplate - pointer to CDocTemplate for iterating document
///	\param	szFilename - filename to find
///	\return	true if document was found
bool ActivateOpenedDocument(CDocTemplate *pDocTemplate, LPCTSTR szFilename);

///	\brief	load resource string
///	\param	nResourceId - id of resource string
///	\return	string with loaded resource
CString FormatString(UINT nResourceId, ...);

///	\brief	get last error code and format message
///	\return	string with error message
CString GetLastErrorMessage();

/// Reads from an INI key in the GDS INI file
/// If the key is missing, defaultVal will both be returned and written to the INI 
///		as the value for the key
/// \param key = key to search for
/// \param value = reference to string to write return val into
/// \param subCategory = name to subsort keys under
/// \param defaultVal = default string to return if key is missing
void ReadINIKey(StdString key, StdString &value, StdString subCategory=_T("Graphics Dev System"), StdString defaultVal=_T(""));

/// Writes to the INI key in the GDS INI file
/// \param key = key to write under
/// \param value = value to write
/// \param subCategory = name to subsort keys under
void WriteINIKey(StdString key, StdString value, StdString subCategory=_T("Graphics Dev System"));

/// \brief	Provides ability to work temporary with specific archive version.
///			Then previous file version will be restored in class destructor
class CFileVersionSetter
{
public:
	/// \brief	This constructor sets latest file version
	CFileVersionSetter();

	/// \brief	This constructor sets specific file version
	/// \param	szFileVersion - file version string
	CFileVersionSetter(LPCTSTR szFileVersion);

	/// \brief	This constructor sets specific file version
	/// \param	pVersion - ptr to the IHashString version
	CFileVersionSetter(IHashString *pFileVersion);

	/// \brief	Revert previous file version back
	~CFileVersionSetter();

	/// \brief	Set passed value as file version
	/// \param	pVersion - ptr to the IHashString version
	static void Apply(IHashString *pVersion);

private:
	// \brief	private member for protecting from copying
	CFileVersionSetter(const CFileVersionSetter&);
	// \brief	private member for protecting from copying
	void operator=(const CFileVersionSetter&);

	/// \brief	does initial file version setting
	///			it saves current file version in m_hszPreviousFileVersion
	/// \param	pVersion - ptr to the IHashString version
	void Init(IHashString *pVersion);

private:
	CHashString	m_hszPreviousFileVersion;
};

}; // namespace GDS_SDK_Helpers

#endif //_GDS_EDITOR_HELPERS_H
