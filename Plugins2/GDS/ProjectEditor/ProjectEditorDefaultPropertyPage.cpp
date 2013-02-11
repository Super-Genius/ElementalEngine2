///============================================================================
/// \file		ProjectEditorBasePropertyPage.cpp
/// \brief		Declaration of the new project settings base property page.
/// \date		01-03-2009
/// \author		Zsolt Matyas
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
#include "ProjectEditorDefaultPropertyPage.hpp"

// for the regular expressions:
#include <atlrx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CProjectEditorDefaultPropertyPage, IProjectSettingsPropertyPage)
	ON_BN_CLICKED(IDC_DIRECTORY_SEL, &CProjectEditorDefaultPropertyPage::OnBnClickedDirectorySel)
	ON_BN_CLICKED(IDC_BUTTON_LISTBOX_ADD, &CProjectEditorDefaultPropertyPage::OnBnClickedButtonListboxAdd)
	ON_BN_CLICKED(IDC_BUTTON_LISTBOX_REMOVE, &CProjectEditorDefaultPropertyPage::OnBnClickedButtonListboxRemove)
	ON_BN_CLICKED(IDC_BUTTON_STARTUP_SCRIPT_SELECTOR, &CProjectEditorDefaultPropertyPage::OnBnClickedButtonStartupScriptSelector)
//	ON_BN_CLICKED(IDC_DIRECTORY_SEL, &CProjectEditorDefaultPropertyPage::OnBnClickedDirectorySel)
END_MESSAGE_MAP()

IMPLEMENT_DYNAMIC(CProjectEditorDefaultPropertyPage, IProjectSettingsPropertyPage)

CProjectEditorDefaultPropertyPage::CProjectEditorDefaultPropertyPage( void )
: IProjectSettingsPropertyPage( CProjectEditorDefaultPropertyPage::IDD )
{
}

CProjectEditorDefaultPropertyPage::~CProjectEditorDefaultPropertyPage(void)
{
}

void CProjectEditorDefaultPropertyPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_PROJECT_DIR, m_ProjectDirEdit);
	DDX_Control(pDX, IDC_COMBOBOX_PROJECT_VERSION, m_sVersion);
	DDX_Control(pDX, IDC_EDIT_PROJECT_NAME, m_sProjectName);
	DDX_Control(pDX, IDC_EDIT_PROJECT_STARTUP_SCRIPT, m_szProjectStartupScript);
	DDX_Control(pDX, IDC_ADDITIONAL_DLL_LIST, m_listAdditionalDLLs);
	DDX_Control(pDX, IDC_BUTTON_STARTUP_SCRIPT_SELECTOR, m_StartupScriptOFNButton);
	DDX_Control(pDX, IDC_BUTTON_LISTBOX_ADD, m_buttonListBoxAddElement);
	DDX_Control(pDX, IDC_BUTTON_LISTBOX_REMOVE, m_buttonListBoxRemoveElement);
}
void CProjectEditorDefaultPropertyPage::OnOK()
{
	// if we get to this point, every entered data is valid.
	// Directory change requires restart, so we check whether it
	// is changed or not.
	CString inputString;
	bool bDirHasChanged = false;
	TCHAR* originalDirectory = (TCHAR*) m_projectSettings->GetDirectory();
	
	m_ProjectDirEdit.GetWindowText( inputString );
	TCHAR* newDirectory = (TCHAR*)inputString.GetString();

	// check if changed (incasesensitive)
	if( _tcsicmp( originalDirectory, newDirectory ) == 0 )
	{
		bDirHasChanged = true;	
	}
	// The project settings are being updated from this point

	// update project dir - case sensitive, update can be necessary
	// if bDirHasChanged is false as well. 
	m_projectSettings->SetDirectory( newDirectory, m_bAllowNew );

	m_sProjectName.GetWindowText(inputString);
	m_projectSettings->SetProjectName( (TCHAR*)inputString.GetString() );

	m_szProjectStartupScript.GetWindowText(inputString);
	m_projectSettings->SetStartUpScript( (TCHAR*)inputString.GetString() );

	m_sVersion.GetWindowText(inputString);
	m_projectSettings->SetVersion( (TCHAR*)inputString.GetString() );

	// save the additional DLL list
	int iterator;
	vector<StdString> DllList;

	//DllList->clear();
	for ( iterator = 0; iterator < 	m_listAdditionalDLLs.GetItemCount(); iterator++ )
	{
		// insert the current text into the temporary list
		StdString currentItemText = m_listAdditionalDLLs.GetItemText(iterator,0);
		DllList.push_back ( currentItemText );
	}
	// set the project DLLs
	m_projectSettings->SetProjectDllList ( &DllList );

	// the datas are ok and saved, we should set the settings consistency to true
	m_projectSettings->SetSettingsStateConsistency( TRUE );

}
void CProjectEditorDefaultPropertyPage::OnCancel()
{
	//TODO: Restore values (currently the show method restores everything at the 
	// next display time, it all the values are restored again.
}

BOOL CProjectEditorDefaultPropertyPage::OnApply()
{
	OnOK();

	// returning TRUE caused the handlers to be destroyed.
	return FALSE;
}

BOOL CProjectEditorDefaultPropertyPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// let's init our variable. We are going to use it later.
	m_projectSettings = SINGLETONINSTANCE(CProjectSettings);

	if (!m_bAllowNew)
	{
		// call the function to set the edit fields on the proterty page
		RefreshPageValues();
	}	
	else
	{
		// Setting the editable values
		m_sProjectName.SetWindowText ( _T("") );
		m_ProjectDirEdit.SetWindowText( _T(""));
		m_sVersion.SelectString (0,  _T("1.0") );
		m_szProjectStartupScript.SetWindowText ( _T(""));

		// disable inputs which needs project dir to be specified
		EnableInputs( FALSE );
	}

	return TRUE;  // return TRUE unless you set the focus to a control
}

///////////////////////////////////////////////////////////////////////////
//MESSAGE HANDLER

void CProjectEditorDefaultPropertyPage::OnBnClickedDirectorySel()
{
	TCHAR szPathBuffer[MAX_PATH];
	
	BROWSEINFO bi = { 0 };
	bi.lpszTitle = _T("Pick A Project Directory");

	_tcsncpy(szPathBuffer, m_projectSettings->GetDirectory(), MAX_PATH);
	/// Use SHXXX function to set the directorz to szPathBuffer before
	/// calling SHBrowseForFolder
	LPITEMIDLIST pidl = SHBrowseForFolder( &bi );

	if( pidl != 0 )
	{	
		if( SHGetPathFromIDList( pidl, szPathBuffer ) )
		{
			// Why would we set the variable before clicking 
			// Ok on the settings window??? (in the copied source already)
			//projSettings->SetDirectory(szPathBuffer);

			// set the string editor 
			m_ProjectDirEdit.SetWindowText(szPathBuffer);

			// enable the buttons depending on the project dir
			EnableInputs( TRUE );
		}

		// free directory selection stuff
		IMalloc* imalloc = 0;
		if( SUCCEEDED( SHGetMalloc( &imalloc ) ) )
		{
			imalloc->Free( pidl );
			imalloc->Release( );
		}
	}
}
void CProjectEditorDefaultPropertyPage::OnBnClickedButtonListboxAdd()
{
	// get the project directory string
	CString projectDirString;	
	m_ProjectDirEdit.GetWindowText( projectDirString );

	// we want to make the user to specify the project
	// directory first. If it is not set yet returning.
	if ( projectDirString.GetLength() == 0 )
	{
		AfxMessageBox( "Please, specify the project directory first!" );
		return; 
	}

	// Selecting a file
	CFileDialog myFileDialog(TRUE, NULL, NULL, OFN_ENABLESIZING|OFN_EXPLORER|OFN_FILEMUSTEXIST, NULL);

	// set the file selector default dir to project dir
	myFileDialog.m_ofn.lpstrInitialDir = projectDirString;

	// Adding it to the listBox
	if ( myFileDialog.DoModal() == IDOK ) 
	{
		// get the selected file with path
		CString filename = myFileDialog.GetPathName();

		// to create relative paths remove the project 
		// directory if it matches the first part of the path
		// Note '== 0' means, that it matches the substring 
		// starting at position 0.
		if ( filename.Find ( projectDirString , 0) == 0 )
		{
			// remove the project directory part
			filename.Replace( projectDirString , "" );

			// Differentiate drive paths, because they end 
			// with '\', but other paths does not.
			// Remove trailing '\' only from non drive paths.
			if ( filename.GetAt(0) == '\\' )
			{
				filename.Delete(0,1);
			}	
		}

		// check the list to avoid duplicates
		LVFINDINFO newItem;
		newItem.flags = LVFI_STRING;
		LPSTR fileNameLPSTR = filename.GetBuffer( filename.GetLength() );
		newItem.psz = _T( fileNameLPSTR );
		if( m_listAdditionalDLLs.FindItem( &newItem ) != -1 )
		{
			// warn the user about the duplicate
			AfxMessageBox( "The selected path is already in the list!" ); 
		}
		else
		{
			// insert the result into the list
			m_listAdditionalDLLs.InsertItem( LVIF_TEXT|LVIF_STATE, 0, filename, 0, 0,0,0);
		}
	}
}

void CProjectEditorDefaultPropertyPage::OnBnClickedButtonListboxRemove()
{
	// get the selected item's number
	int Selection = m_listAdditionalDLLs.GetSelectionMark();

	// check if anything is selected
	if (Selection != -1)
	{
		// if there is a selected item remove it
		m_listAdditionalDLLs.DeleteItem( Selection ); 
	}

	// remove  the selection mark
	m_listAdditionalDLLs.SetSelectionMark( -1 );
}

void CProjectEditorDefaultPropertyPage::OnBnClickedButtonStartupScriptSelector()
{
	// get the project directory string
	CString projectDirString;	
	m_ProjectDirEdit.GetWindowText( projectDirString );

	// we want to make the user to specify the project
	// directory first. If it is not set yet returning.
	if ( projectDirString.GetLength() == 0 )
	{
		AfxMessageBox( "Please, specify the project directory first!" );
		return; 
	}

	// Selecting a file
	CFileDialog *myFileDialog = new CFileDialog(true);
	
	// set the file selector default dir to project dir
	myFileDialog->m_ofn.lpstrInitialDir = projectDirString;

	// Adding it to the listBox
	if ( myFileDialog->DoModal() == IDOK ) 
	{
		// get the selected file with path
		CString filename = myFileDialog->GetPathName();

		// to create relative paths remove the project 
		// directory if it matches the first part of the path
		// Note '== 0' means, that it matches the substring 
		// starting at position 0.
		if ( filename.Find ( projectDirString , 0) == 0 )
		{
			// remove the project directory part
			// could be solved by string::Delete method as well
			filename.Replace( projectDirString , "" );

			// Differentiate drive paths, because they end 
			// with '\', but other paths does not.
			// Remove trailing '\' only from non drive paths.
			if ( filename.GetAt(0) == '\\' )
			{
				filename.Delete(0,1);
			}		
		} 
		else
		{
			AfxMessageBox( "The startup script should be inside the project directory!" );
			return; 
		}

		// set the startup script
		m_szProjectStartupScript.SetWindowText( filename );
	}
}


////////////////////////////////////////////////////////////////////
// HELPER FUNCTIONS

void CProjectEditorDefaultPropertyPage::EnableInputs ( bool bEnable ) 
{
		// disable the buttons and edits depending on the project dir
		// startup script field and button
		m_szProjectStartupScript.EnableWindow( bEnable );
		m_StartupScriptOFNButton.EnableWindow( bEnable );

		// additional dependencies buttons
		m_buttonListBoxAddElement.EnableWindow( bEnable );
		m_buttonListBoxRemoveElement.EnableWindow( bEnable );

		// additional dependencies listView
		m_listAdditionalDLLs.EnableWindow( bEnable );
}

bool CProjectEditorDefaultPropertyPage::regExpValidate(CString p_validateThis, CString p_pattern, CString p_errorString)
{
	// Regular expression object
	CAtlRegExp<> RegExpValidator;

	// Match object for the Regular Expression
	CAtlREMatchContext<> mcMatchContext;

	//RegExpValidator.Parse("^([a-zA-Z ]+)$"); //^[\a]+$
	if ( RegExpValidator.Parse( p_pattern, FALSE ) != REPARSE_ERROR_OK )
	{
		AfxMessageBox( "A minor issue has happened. Though, it was minor, this is not your lucky day, the function will be aborted... (cause: regExp pattern can't be parsed. Is it syntactically correct?" );
		return FALSE; 
	}

	// Let's try match the regular expression
	if ( !RegExpValidator.Match( p_validateThis, &mcMatchContext) )
	{
		// if failed, show errorstring and return false
		AfxMessageBox( p_errorString );
		return FALSE;
	}
	
	// match succeed: return true
	return TRUE;
}

BOOL CProjectEditorDefaultPropertyPage::ValidateValues()
{
// Ensure that your UI got the necessary input 
	// from the user before closing the dialog.

	// Validator string
	CString sValidator;

	// first we validate the prjName.
	m_sProjectName.GetWindowText ( sValidator );

	// remove unnecessary whitspace characters
	sValidator.Trim();

	// Validating lenght
	if ( sValidator.GetLength() == 0 )
	{
		AfxMessageBox("Please, set the Project name.");
		return FALSE; 
	}
	
	// Validating content
	if ( ! regExpValidate( sValidator, "^([a-zA-Z1-9_ ]+)$", "Incorrect Project Name. Please use only alphanumerical characters.") )
		return FALSE;

	// Validating project directory
	m_ProjectDirEdit.GetWindowText ( sValidator );
	
	if ( sValidator.GetLength() == 0 )
	{
		AfxMessageBox("Please, set the Project Directory.");
		return FALSE; 
	}

	// Directory string does not need to be validated because
	// it is set thru the selector --> no incorrect strings

	// Validating the startup scirpt name
	m_szProjectStartupScript.GetWindowText ( sValidator ); 

	// remove unnecessary whitspace characters
	sValidator.Trim();

	if ( sValidator.GetLength() == 0 )
	{
		AfxMessageBox("Please, set the Startup Script.");
		return FALSE; 
	}

	// Validating content
	if ( ! regExpValidate( sValidator, "^([a-zA-Z1-9_\\.\\- :\\\\]+)$", "Incorrect Startup Script") )
		return FALSE;

	return TRUE;
}

BOOL CProjectEditorDefaultPropertyPage::RefreshPageValues()
{
	// Setting the editable values
	m_sProjectName.SetWindowText ( m_projectSettings->GetProjectName() );

	StdString szDirectory = m_projectSettings->GetDirectory();
	m_ProjectDirEdit.SetWindowText( szDirectory );
	m_sVersion.SelectString (0,  m_projectSettings->GetVersion() );
	m_szProjectStartupScript.SetWindowText ( m_projectSettings->GetStartUpScript() );
	// get the dll list
	vector<StdString> *DllList = m_projectSettings->GetProjectDllList();
	
	// populate dllList editor - first remove the old items
	m_listAdditionalDLLs.DeleteAllItems();

	// add the new items
	unsigned int iterator;
	for ( iterator = 0; iterator < 	DllList->size(); iterator++ )
	{
		// insert current element into the list
		StdString filename = DllList->at( iterator );
		m_listAdditionalDLLs.InsertItem( LVIF_TEXT|LVIF_STATE, 0, filename, 0, 0,0,0);
	}

	// if the project directory is not corretly set disable the 
	// depending edit fields and buttons
	if ( szDirectory.length() == 0 ) 
	{
		EnableInputs( FALSE );
	}

	return TRUE;
}