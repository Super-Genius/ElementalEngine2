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

#ifndef OPENTEXTUREFILEDIALOG_H
#define OPENTEXTUREFILEDIALOG_H

// COpenTextureFileDialog

class COpenTextureFileDialog : public CFileDialog
{
	DECLARE_DYNAMIC(COpenTextureFileDialog)

public:
	COpenTextureFileDialog(StdString const & ResourcePath, CWnd * hwndOwner = NULL, StdString const & title = _T("Select Base Texture"));
	StdString GetFileName();
	virtual ~COpenTextureFileDialog();

protected:
	DECLARE_MESSAGE_MAP()

private:
	CString m_ResourcePath;
	CString m_title;
};

#endif		// #ifndef OPENTEXTUREFILEDIALOG_H
