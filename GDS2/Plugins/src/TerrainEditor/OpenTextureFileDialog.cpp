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

#include "stdafx.h"

// COpenTextureFileDialog

IMPLEMENT_DYNAMIC(COpenTextureFileDialog, CFileDialog)

COpenTextureFileDialog::COpenTextureFileDialog(StdString const & ResourcePath, CWnd * hwndOwner, StdString const & title)
:	CFileDialog(TRUE, NULL,	NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR, _T("Textures|*.TGA;*.BMP;*.TEX;*.DDS||"), hwndOwner),
	m_ResourcePath(ResourcePath.c_str()),
	m_title(title.c_str())
{
	m_ofn.lpstrTitle = m_title;
	m_ofn.lpstrInitialDir = m_ResourcePath;
}

StdString COpenTextureFileDialog::GetFileName()
{
	if (IDOK != DoModal())
		return _T("");
	
	//otherwise set the file
	//check if valid relative path:
	CString OpenFileName = GetPathName();
	OpenFileName.MakeLower();
	m_ResourcePath.MakeLower();
	if (OpenFileName.Find(m_ResourcePath.GetBuffer()) == 0)
	{
		//convert to relative path
		int len = m_ResourcePath.GetLength();
		//eliminate leading slash
		if( OpenFileName[ len ] == _T('\\') || OpenFileName[ len ] == _T('/') )
		{
			len++;
		}
		return OpenFileName.GetBuffer() +  len ;
	}

	//failed, reset string to null
	return _T("");
}

COpenTextureFileDialog::~COpenTextureFileDialog()
{
}


BEGIN_MESSAGE_MAP(COpenTextureFileDialog, CFileDialog)
END_MESSAGE_MAP()

// COpenTextureFileDialog message handlers
