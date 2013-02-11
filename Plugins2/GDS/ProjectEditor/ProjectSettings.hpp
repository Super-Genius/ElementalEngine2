///============================================================================
/// \file		ProjectSettings.hpp
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

#ifndef __PROJECTSETTINGS_H__
#define __PROJECTSETTINGS_H__

class CProjectSettings : public IComponent
{
private:
	SINGLETONCONSTRUCTOROVERRIDE(CProjectSettings);

	//---------------------------------------------------------------------
	// Function:	CProjectSettings
	// Description:	Default Constructor - Calls parent's constuctor + intializes. 
	// Parameters:	.
	//---------------------------------------------------------------------
	CProjectSettings();

public:

	//---------------------------------------------------------------------
	// Function:	~CProjectSettings
	// Description:	Default Destructor
	//---------------------------------------------------------------------
	~CProjectSettings();

	//---------------------------------------------------------------------
	// Function:	Create
	// Description:	Returns the only instance of the class (singleton)
	// Parameters:	nArgs - Number of arguments - not in use 
	//				argptr - The argument list - not in use
	// Returns:		the instance
	//---------------------------------------------------------------------
	static IComponent *Create(int nArgs, va_list argptr);	

	/// return a string of the component type
	/// \return IHashString *reference to textual classname
	virtual IHashString *GetComponentType();

	/// Checks if this is similar to it's base classes component
	/// \return true if it is simlar, else false
	virtual bool IsKindOf(IHashString *compType);

	//---------------------------------------------------------------------
	// Function:	Serialize
	// Description:	serializes the values
	//				(deserializaton if ar.IsReading() returns ture)
	//              The order of the values if bound
	// Parameters:	ar - IArchive object which shows if we are reading or writing
	//---------------------------------------------------------------------
	void Serialize(IArchive &ar);

	//---------------------------------------------------------------------
	// Function:	Init
	// Description:	Sets every class variable to its default.
	// Returns:		void
	//---------------------------------------------------------------------
	void Init();

	//---------------------------------------------------------------------
	// Function:	GetProjectName
	// Description:	Getter function for the m_szProjectName private variable
	// Returns:		the value of m_szDirectory
	//---------------------------------------------------------------------
	const TCHAR *GetProjectName() { return m_szProjectName.c_str(); }

	//---------------------------------------------------------------------
	// Function:	SetProjectName
	// Description:	Setter function for the m_szProjectName private variable
	// Parameters:	inProjName - the new value of m_szDirectory
	//---------------------------------------------------------------------
	void SetProjectName(TCHAR *inProjName) 
	{ 
		if( _tcscmp (m_szProjectName,inProjName) )
		{
			IsChangedSinceLastSave = TRUE;
		}
		m_szProjectName = inProjName;
	}

	//---------------------------------------------------------------------
	// Function:	GetDirectory
	// Description:	Getter function for the m_szDirectory private variable
	// Returns:		the value of m_szDirectory
	//---------------------------------------------------------------------
	const TCHAR *GetDirectory() { return m_szDirectory.c_str(); }

	//---------------------------------------------------------------------
	// Function:	SetDirectory
	// Description:	Setter function for the m_szDirectory private variable
	// Parameters:	SetDirectory - the new value of m_szDirectory
	//---------------------------------------------------------------------
	void SetDirectory(TCHAR *inDir, bool isNewProjetSettings ) 
	{ 
		if( _tcscmp (m_szDirectory,inDir) )
		{			
			// check whether new settings are created or existing ones are
			// under change.
			if ( !isNewProjetSettings )
			{
				// if the settings were created earlier ask for dir update
				int result = AfxMessageBox("Changing the resource directory needs to close every open document. Are you sure?", MB_YESNO);

				// check the result
				if ( result != IDYES ) 
				{
					// user don't want to change this dir, let's return.
					return;
				}
				
				// TODO: Close everything()
				
			}

			//Change EE resource dir
			StdString resourceDir = inDir;
			EngineGetToolBox()->SetDirectories(NULL , &resourceDir);

			//We need to change the current working dir of the OS as well.
			//This plugin is win32 dependent, so we can call this directly:
			SetCurrentDirectory( resourceDir.c_str() );


			// set resource directories for renderer	
			SETWORKINGDIRECTORIESPARAMS swdp;
			swdp.ResourceDirectory = NULL;
			swdp.BaseDirectory = &resourceDir;
			static DWORD msgHash_SetDirectories = CHashString(_T("SetResourceDirectory")).GetUniqueID();
			EngineGetToolBox()->SendMessage(msgHash_SetDirectories, sizeof(SETWORKINGDIRECTORIESPARAMS), &swdp );
			
			// set the flag that we alter the projet settings
			IsChangedSinceLastSave = TRUE;
			
		}
		m_szDirectory = inDir;
	}

	//---------------------------------------------------------------------
	// Function:	GetVersion
	// Description:	Getter function for the m_fVersion private variable
	// Returns:		the value of m_szDirectory
	//---------------------------------------------------------------------
	const TCHAR *GetVersion() { return m_szVersion.c_str(); }

	//---------------------------------------------------------------------
	// Function:	SetVersion
	// Description:	Setter function for the m_fVersion private variable
	// Parameters:	SetDirectory - the new value of m_szDirectory
	//---------------------------------------------------------------------
	void SetVersion(TCHAR *inVer)
	{ 
		if( _tcscmp (inVer,m_szVersion) )
		{
			IsChangedSinceLastSave = TRUE;
		}
		m_szVersion = inVer;
	}

	//---------------------------------------------------------------------
	// Function:	GetStartUpScript
	// Description:	Getter function for the m_szStartScriptName private variable
	// Returns:		the value of m_szDirectory
	//---------------------------------------------------------------------
	const TCHAR *GetStartUpScript() { return m_szStartScriptName.c_str(); }

	//---------------------------------------------------------------------
	// Function:	SetStartUpScript
	// Description:	Setter function for the m_szStartScriptName private variable
	// Parameters:	inSturtUpScript - the new value of m_szDirectory
	//---------------------------------------------------------------------
	void SetStartUpScript(TCHAR *inSturtUpScript) 
	{
		if( _tcscmp (inSturtUpScript,m_szStartScriptName) )
		{
			IsChangedSinceLastSave = TRUE;
		}
		m_szStartScriptName = inSturtUpScript;
	}

	//---------------------------------------------------------------------
	// Function:	GetLastWorldFile
	// Description:	Getter function for the m_szLastWorldFile private variable
	// Returns:		the value of m_szDirectory
	//---------------------------------------------------------------------
	const TCHAR *GetLastProjectFile() { return m_szLastWorldFile.c_str(); }

	//---------------------------------------------------------------------
	// Function:	SetLastWorldFile
	// Description:	Setter function for the m_szLastWorldFile private variable
	// Parameters:	inWordFile - the new value of m_szDirectory
	//---------------------------------------------------------------------
	void SetLastWorldFile(TCHAR *inWordFile)
	{ 
		if( _tcscmp (m_szLastWorldFile,inWordFile) )
		{
			IsChangedSinceLastSave = TRUE;
		}
		m_szLastWorldFile = inWordFile;
	}

	//---------------------------------------------------------------------
	// Function:	GetProjectDllList
	// Description:	Getter function of the additional DLL's list
	// Returns:		 additional DLL's list
	//---------------------------------------------------------------------
	vector<StdString> *GetProjectDllList() { return &m_ProjectDLLs; }

	//---------------------------------------------------------------------
	// Function:	SetProjectDllList
	// Description:	Setter function for additional DLLs list (Vector)
	// Parameters:	inDllList - the new value of m_ProjectDLLs
	//---------------------------------------------------------------------
	void SetProjectDllList( vector<StdString> * inDllList )
	{
		bool changed = false;

		// the vectors should be sorted if not edited in the exported XML
		// first compare the size then the elements in the vectors
		if(inDllList->size()  != m_ProjectDLLs.size() )
		{
			changed = TRUE;
		}
		else 
		{
			// iterate thru the elements
			for ( unsigned int i = 0; i < inDllList->size(); i++ )
			{
				// compare each element
				if ( inDllList->at( i ).compare( m_ProjectDLLs.at( i ) ) != 0 )
				{
					changed = TRUE;
				}
			}
		}

		// update list if changed
		if ( changed )
		{
			m_iNumDlls = (unsigned int)inDllList->size();
			m_ProjectDLLs.clear();
			unsigned int i;
			for ( i = 0; i < m_iNumDlls; i++ ) 
			{
				m_ProjectDLLs.push_back( (inDllList->at(i) ) );
			}

			// set global changed flag
			IsChangedSinceLastSave = TRUE;
		}
	}

	//---------------------------------------------------------------------
	// Function:	IsConsistentState
	// Description:	returns wether the project settings are consistent
	//				(ready to save)
	// Returns:		true if consistent
	//---------------------------------------------------------------------
	bool IsConsistentState () { return m_bIsConsistentState ; }

	//---------------------------------------------------------------------
	// Function:	SetProjectDllList
	// Description:	Setter function for the 'Consistency of the Settings' variable (m_bIsConsistentState)
	// Parameters:	p_bConsistent - the new value of m_bIsConsistentState
	//---------------------------------------------------------------------
	void SetSettingsStateConsistency (bool p_bConsistent) { m_bIsConsistentState = p_bConsistent; } 

	//---------------------------------------------------------------------
	// Function:	CloseProjectSettings
	// Description:	Closes project settings, asks for save if changed
	//				and sets state to inconsistent state to prevent
	//				saving again without loading or creating new settings 
	//				first.
	// Returns:		BOOL - TRUE if quit is confirmed by the user.
	//---------------------------------------------------------------------
	bool CloseProjectSettings();

	//---------------------------------------------------------------------
	// Function:	setSaveFileName
	// Description:	Setter function for the file name to save the project
	// Parameters:	p_fileName - the new value of m_szProjectFileName
	//---------------------------------------------------------------------
	void setSaveFileName( StdString p_fileName ) 
	{
		// Saving the file name occurs in a callback function
		// after setting the variables. The Changed flag is set correctly, 
		// no other function can change the save file name, so
		// there is no need to update the Changed flag on filename change.
		m_szProjectFileName = p_fileName;

		//if the filename is changed, we need to update the INI entry as well:
		GDS_SDK_Helpers::WriteINIKey( INI_LastProjectPath, m_szProjectFileName, INI_ProjectSettings_SubSection );

	}

	//---------------------------------------------------------------------
	// Function:	GetProjectDllList
	// Description:	Getter function of the filname to save the project (m_szProjectFileName)
	// Returns:		the value of filename
	//---------------------------------------------------------------------
	const StdString getSaveFileName() { return m_szProjectFileName; }

private:
	/// Name of the project
	StdString m_szProjectName;

	/// Version of the object (project)
	StdString m_szVersion;

	/// Source Directory of the object (project)
	StdString m_szDirectory;

	/// Startup Script name of the object (project)
	StdString m_szStartScriptName;

	/// Last Used World File of the object (project)
	StdString m_szLastWorldFile;

	/// The number of the DLLS of the object (project)
	unsigned int m_iNumDlls;

	/// The list of the DLLS-s in USE of the object (project)
	vector<StdString> m_ProjectDLLs;
	
	// Shows whether the settings are in a consistent state (ready to save/close)
	bool m_bIsConsistentState;

	StdString m_szProjectFileName;

	// shows if the project settings have changed (ask for save if true)
	bool IsChangedSinceLastSave;

};

#endif //#ifndef __PROJECTSETTINGS_H__

