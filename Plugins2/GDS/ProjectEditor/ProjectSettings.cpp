///============================================================================
/// \file		ProjectSettings.cpp
/// \brief		Implentation of CProjectSettings Class
/// \date		12/16/2008
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

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT_SINGLETON(CProjectSettings);

	afx_msg void OnDestroy();
//---------------------------------------------------------------------
// Function:	CProjectSettings
// Description:	Default Constructor - Calls parent's constuctor + intializes. 
// Parameters:	.
//---------------------------------------------------------------------
CProjectSettings::CProjectSettings()
{
	Init();
}

void CProjectSettings::Init()
{
	m_szProjectName = _T( "" );
	m_szVersion = _T( "1.0" );
	m_szDirectory = _T( "" );
	m_szStartScriptName = _T( "" );
	m_szLastWorldFile = _T( "" );
	m_iNumDlls = 0;
	m_ProjectDLLs.clear();

	// clear the previous Dll list
	m_ProjectDLLs.clear();

	// let user save the data only if data is changed 
	IsChangedSinceLastSave = FALSE;

	// filename to save
	m_szProjectFileName = _T( "" );

	// not to let save project settings without setting every necessary variable
	SetSettingsStateConsistency( FALSE );
}

//---------------------------------------------------------------------
// Function:	~CProjectSettings
// Description:	Default Destructor
//---------------------------------------------------------------------
CProjectSettings::~CProjectSettings()
{
}

//---------------------------------------------------------------------
// Function:	Create
// Description:	Returns the only instance of the class (singleton)
// Parameters:	nArgs - Number of arguments - not in use 
//				argptr - The argument list - not in use
// Returns:		the instance
//---------------------------------------------------------------------
IComponent *CProjectSettings::Create( int nArgs, va_list argptr )
{
	return SINGLETONINSTANCE(CProjectSettings);
}

IHashString *CProjectSettings::GetComponentType()
{
	static CHashString compType(_T("CProjectSettings"));
	return &compType;
}

bool CProjectSettings::IsKindOf(IHashString *compType)
{
	static DWORD compTypeID = CHashString(_T("CProjectSettings")).GetUniqueID();

	if (compType->GetUniqueID() == compTypeID)
	{
		return true;
	}
	return false;
}

//---------------------------------------------------------------------
// Function:	Serialize
// Description:	serializes the values
//				(deserializaton if ar.IsReading() returns ture)
//              The order of the values if bound
// Parameters:	ar - IArchive object which shows if we are reading or writing
//---------------------------------------------------------------------
void CProjectSettings::Serialize(IArchive &ar)
{
	unsigned int i;
	if (ar.IsReading())
	{
		// Restore everthing to default
		Init();

		// set the variables according to the data in the file
		ar.Read(m_szProjectName);
		ar.Read(m_szVersion);
		ar.Read(m_szDirectory);
		ar.Read(m_szStartScriptName);
		ar.Read(m_szLastWorldFile);
		ar.Read(m_iNumDlls);

		// clear the previous Dll list
		m_ProjectDLLs.clear();

		for (i=0; i<m_iNumDlls; i++)
		{
			StdString dllName;
			ar.Read(dllName);
			m_ProjectDLLs.push_back( dllName );
		}

		//reading the other tags are done in the loadSaver project
		//it attempts to call the serialize function of the objects
		//with the names of the xml tags.
	}
	else
	{
		ar.Write(m_szProjectName, "ProjectName");
		ar.Write(m_szVersion, "Version");
		ar.Write(m_szDirectory, "Directory");
		ar.Write(m_szStartScriptName, "StartupScript");
		ar.Write(m_szLastWorldFile, "LastWorldFile");
		ar.Write(m_iNumDlls, "NumDLLS");
		for (i=0; i<m_iNumDlls; i++)
		{
			ar.StartClass("ProjectDLL");
			ar.Write(m_ProjectDLLs[i], "name");
			ar.EndClass();
		}
	}

	// both save and load means that no change has happend "since last save"
	IsChangedSinceLastSave = FALSE;

	// make the project ready to save/close
	// save happens only if IsChangedSince... is also true, so incorrect
	// data can't be saved, because the dialog box checks consistency.
	SetSettingsStateConsistency( TRUE );

	//TODO: the following are necessary only on project load?
	// than they can be moved to the load part of the if branches above...

	// change EE resource directory manually
	EngineGetToolBox()->SetDirectories(NULL , &m_szDirectory);

	// Loading a new project usually changes the resource directory as well.
	// As the m_szDirectory setter would send out the change message but it is
	// not called if on project load, we need to send it manually
	// Although the new dir is the content of the message, the most plugins will
	// use only the message to recognize they need to refresh their value and they
	// we get the new value from EE toolbox instead of the content of this message.
	SETWORKINGDIRECTORIESPARAMS swdp;
	swdp.ResourceDirectory = NULL;
	swdp.BaseDirectory = &m_szDirectory;
	static DWORD msgHash_SetDirectories = CHashString(_T("SetResourceDirectory")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_SetDirectories, sizeof(SETWORKINGDIRECTORIESPARAMS), &swdp );

	// we need to set the current working dir of the OS. 
	// this is a win32 plugin, so we can call it directly:
	SetCurrentDirectory( m_szDirectory.c_str() );
}

bool CProjectSettings::CloseProjectSettings()
{
	// check bool if settings have changed
	if ( IsChangedSinceLastSave ) 
	{
		// popup to ask if save - yes/no/(cancel)
		int answer = AfxMessageBox( "Do you want to save the changes in the project?", MB_YESNOCANCEL );

		// do what the user clicked
		if (answer == IDCANCEL)
		{
			// Cancel: we dont want to do anything...
			return FALSE;
		}
		else if ( answer == IDYES )
		{
			// get the filename
			StdString fileName = getSaveFileName();

			// check if it is a valid path
			if ( fileName.length() == 0 )
			{
				// Selecting a file
				StdString typeExtension( _T("gdsprj") );
				StdString filterStr = "GDS project files (*." + typeExtension + ")|*." + typeExtension + "||"; 
				CFileDialog myFileDialog(FALSE, typeExtension, NULL, OFN_ENABLESIZING|OFN_EXPLORER|OFN_OVERWRITEPROMPT, filterStr);

				// set the file selector default dir to project dir
				myFileDialog.m_ofn.lpstrInitialDir = GetDirectory();

				// Adding it to the listBox
				if ( myFileDialog.DoModal() == IDOK ) 
				{
					// get the selected file with path
					fileName = myFileDialog.GetPathName();
					
					// saving the filename in the object as well
					setSaveFileName( fileName );
				}
				else
				{
					// the user clicked cancel: exiting save dialog
					return FALSE;
				}
			}

			// send message to make projectloader serialize us.
			CHashString hszProjectLoader(_T("CProjectLoadSave"));
			CHashString hszNewParticleSaveFilepath( fileName );
			static DWORD msgHash_SaveFile = CHashString(_T("SaveFile")).GetUniqueID();
			DWORD result = EngineGetToolBox()->SendMessage(msgHash_SaveFile, sizeof(TCHAR *), (void *)hszNewParticleSaveFilepath.GetString(), 0, &hszProjectLoader);

			// mark settings as saved
			IsChangedSinceLastSave = FALSE;
		}
		else
		{
			// we are here, if the user clicked NO SAVE
			// then we don't save anything, but return TRUE
			// that will lead to close GDS
			return TRUE;
		}
	}
	// Closing project destroys consistency	
	SetSettingsStateConsistency( FALSE );
	
	return TRUE;
}



