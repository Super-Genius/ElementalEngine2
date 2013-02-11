///============================================================================
/// \file       ParticleDoc.h
/// \brief      Implementation file for GDS Particle Editor plugin Document
/// \date       09-13-2006
/// \author     Brendan Geiger
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
#include "ParticleDoc.h"
#include "ObjectHelpers.h"
#include "ArchiveHelpers.h"
#include "EditorHelpers.h"

using namespace GDS_SDK_Helpers;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExampleDoc

IMPLEMENT_DYNCREATE(CParticleDoc, CDocument)

BEGIN_MESSAGE_MAP(CParticleDoc, CDocument)
END_MESSAGE_MAP()

CParticleDoc * CParticleDoc::GetActiveDocument()
{
	CMDIChildWnd * pChild =
		((CMDIFrameWnd*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();

	if ( !pChild )
		return NULL;

	CDocument * pDoc = pChild->GetActiveDocument();

	if ( !pDoc )
		return NULL;

	// Fail if doc is of wrong kind
	if ( ! pDoc->IsKindOf( RUNTIME_CLASS(CParticleDoc) ) )
		return NULL;

	return (CParticleDoc *) pDoc;
} 

IParticleEmitter *CParticleDoc::GetEmitterPointer()
{
	return m_pParticleEmitter;
}

CParticleDoc::CParticleDoc()
	: m_hszParticleName(_T("EditorParticleDemo"))
	, m_hszEntityName(_T("ParticleViewerEntity"))
	, m_hszSceneName(_T("ParticleViewerScene"))
	, m_hszSceneType(_T("CWorld"))
{
	m_ToolBox = EngineGetToolBox();
	m_pParticleEmitter = NULL;
	m_iWorkingOnBrush = -1;
	m_bWorkingOnEmitter = false;
	m_iCurrentNewBrushIndex = 0;
}

CParticleDoc::~CParticleDoc()
{
	RemoveParticleSystem();
	DeleteEEObject(&m_hszSceneName);

	// clear document list for correct toolbar processing
	if (m_pDocTemplate != NULL)
	{
		m_pDocTemplate->RemoveDocument(this);
	}
	SINGLETONINSTANCE(CParticleEditor)->DisplayToolbar(false);
}

bool CParticleDoc::CreateNewEmitter(IHashString * hszFilepath)
{
	DWORD result = 0;

	TCHAR czFilename[256];
	m_ParticleToolBox.ExtractFileName(czFilename, hszFilepath->GetString());

	StdString szFilepath(hszFilepath->GetString());

	StdString szFilename(czFilename);
	CHashString hszFilename(czFilename);

	CREATE_EMITTER_STRING_TABLE(EmitterPropertyStringTable);

	// Create Data.
	// Create MemArchive using IAR
	IArchive *MemArchive;
	CHashString memType(_T("Memory"));

	CREATEARCHIVE caOut;
	caOut.mode = STREAM_MODE_READ | STREAM_MODE_WRITE;
	caOut.streamData = NULL;
	caOut.streamSize = 0;
	caOut.streamType = &memType;
	static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
	result = m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &caOut);
	if (result != MSG_HANDLED)
		return false;

	MemArchive = caOut.archive;

	// Change the Memarchive to write
	MemArchive->SetIsWriting(true);

	// Write Initial Emitter Properties:

	// WRITE INTERNAL DATA
//	MemArchive->Write( szFilename, _T("filename") );

	// WRITE FILE HEADER
	MemArchive->Write( _T("NewEmitter"), _T("name"));

	MemArchive->Write( PARTICLE_SERIALIZE_NORMAL, _T("flag"));

	MemArchive->Write( 0, _T("brushes"));
	MemArchive->Write( EMITTER_VARIABLE_MAX, _T("keyframes"));

	// WRITE BRUSHES
	for (int i = 0; i < 0; i++)
	{
		MemArchive->Write(_T("NewEmitter_NewBrush.pml"), _T("file"));
	}

	// WRITE PROPERTIES
	for (int i = 0; i < EMITTER_VARIABLE_MAX; i++)
	{
		MemArchive->Write(EmitterPropertyStringTable[i], _T("type"));
		MemArchive->Write(0.0f, _T("time"));
		MemArchive->Write(0.0f, _T("value"));
	}

	// Change the Memarchive to read
	MemArchive->SetIsWriting(false);

	CHashString hszParticleManager = _T("CParticleManager");
	PARTICLELOADINFO pli;
	pli.filepath = hszFilepath;
	pli.data = MemArchive;
	static DWORD msgHash_AddLoadedEmitter = CHashString(_T("AddLoadedEmitter")).GetUniqueID();
	result = m_ToolBox->SendMessage(msgHash_AddLoadedEmitter, sizeof( PARTICLELOADINFO), &pli, 0, &hszParticleManager);

	MemArchive->Close();

	if (result != MSG_HANDLED)
	{
		m_ToolBox->Log(LOGERROR, _T("ParticleLoader failed to add emitter to manager map\n"));
		return false;
	}
	return true;
}

void CParticleDoc::UpdateAllComponents(CView *pSender, LPARAM lHint, CObject *pHint)
{
	UpdateAllViews(pSender, lHint, pHint);
	if ((unsigned int) lHint & PARTICLE_UPDATE_TREEVIEW )
	{
		SINGLETONINSTANCE(CParticleEditor)->UpdateDockTree(pSender, lHint, pHint);
	}
	if ((unsigned int) lHint & PARTICLE_UPDATE_GRAPHVIEW )
	{
		CParticleEditor * pEditor = SINGLETONINSTANCE(CParticleEditor);
		if (pEditor)
		{
			pEditor->ShowGraph();
			pEditor->UpdateDockGraph(this);
		}
	}
}

BOOL CParticleDoc::RenameParticleComponent(int type, StdString &szOldName, StdString &szNewName)
{
	DWORD result;
	CHashString hszOldBrushFilepath(szOldName);
	CHashString hszNewBrushFilepath(szNewName);

	PARTICLEFILEPARAMS pfp;
	pfp.filename = &hszOldBrushFilepath;
	pfp.newfilename = &hszNewBrushFilepath;
	if (PARTICLE_EMITTER == type)
	{
		static DWORD msgHash_RenameParticleEmitter = CHashString(_T("RenameParticleEmitter")).GetUniqueID();
		result = m_ToolBox->SendMessage(msgHash_RenameParticleEmitter, sizeof(PARTICLEFILEPARAMS), &pfp);
	}
	else if (PARTICLE_BRUSH == type)
	{
		static DWORD msgHash_RenameParticleBrush = CHashString(_T("RenameParticleBrush")).GetUniqueID();
		result = m_ToolBox->SendMessage(msgHash_RenameParticleBrush, sizeof(PARTICLEFILEPARAMS), &pfp);
	}

	if (result != MSG_HANDLED)
		return false;
	else
	{
		UpdateAllComponents( NULL, PARTICLE_UPDATE_ALLVIEWS, NULL);
		return true;
	}
}

BOOL CParticleDoc::MoveParticleBrush(bool bUp, UINT iBrushID)
{
	if (iBrushID != -1)
	{
		m_pParticleEmitter->MoveBrush( bUp, iBrushID );
		ResetParticleSystem();
		UpdateAllComponents( NULL, PARTICLE_UPDATE_ALLVIEWS, NULL);
		return true;
	}
	else 
		return false;
}

CParticleEditorToolPal * CParticleDoc::GetToolBar()
{
	return SINGLETONINSTANCE(CParticleEditor)->GetToolBar();
}

void CParticleDoc::ResetParticleSystem()
{	if (GetToolBar()->AreParticlesShown())
	{
		RemoveParticleSystem();
		CreateParticleSystem();
	}
}
BOOL CParticleDoc::RemoveParticleBrush( StdString &szName, UINT iBrushID )
{
	if (iBrushID != -1)
	{
		IParticleType * pParticle;
		pParticle = m_pParticleEmitter->GetBrush(iBrushID);
		assert(pParticle);
		m_pParticleEmitter->RemoveBrush( pParticle );
	}
	else if (_T("") != szName)
	{
		DWORD result;
		IParticleType *pParticle;
		CHashString hszTempString(szName);
		// Check to see if particle exists.
		GETPARTICLETYPEMESSAGE gptm;
		gptm.filename = &hszTempString;
		static DWORD msgHash_GetParticleType = CHashString(_T("GetParticleType")).GetUniqueID();
		result = m_ToolBox->SendMessage(msgHash_GetParticleType, sizeof(GETPARTICLETYPEMESSAGE), &gptm);
		if (MSG_HANDLED != result)
			return false;

		assert(gptm.particletype);
		pParticle = gptm.particletype;

		m_pParticleEmitter->RemoveBrush( pParticle );
	}
	else
	{
		return false;
	}

	UpdateAllComponents( NULL, PARTICLE_UPDATE_ALLVIEWS, NULL);
	return true;
}

bool CParticleDoc::CreateNewBrush(IHashString * hszFilepath)
{
	DWORD result = 0;

	TCHAR czFilename[256];
	m_ParticleToolBox.ExtractFileName(czFilename, hszFilepath->GetString());

	StdString szFilepath(hszFilepath->GetString());

	StdString szFilename(czFilename);
	CHashString hszFilename(czFilename);

	CREATE_PARTICLE_STRING_TABLE(BrushPropertyStringTable);

	// Create Data.
	// Create MemArchive using IAR
	IArchive *MemArchive;
	CHashString memType(_T("Memory"));

	CREATEARCHIVE caOut;
	caOut.mode = STREAM_MODE_READ | STREAM_MODE_WRITE;
	caOut.streamData = NULL;
	caOut.streamSize = 0;
	caOut.streamType = &memType;
	static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
	result = m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &caOut);
	if (result != MSG_HANDLED)
		return false;

	MemArchive = caOut.archive;

	// Change the Memarchive to write
	MemArchive->SetIsWriting(true);

	// Write Initial Emitter Properties:

	// WRITE INTERNAL DATA
//	MemArchive->Write( szFilename, _T("filename") );
//	MemArchive->Write( szFilepath, _T("filepath") );

	// WRITE FILE HEADER
	MemArchive->Write( _T("NewBrush"), _T("name"));
	MemArchive->Write( false, _T("additive"));
	MemArchive->Write( 30.0f, _T("framerate"));
	MemArchive->Write( 30.0f, _T("texturerate"));
	MemArchive->Write( _T(""), _T("texture"));
	MemArchive->Write( false, _T("loop"));
	MemArchive->Write( 0.0f, _T("AttachToEmitter"));
	MemArchive->Write( _T(""), _T("shader"));

	MemArchive->Write( PARTICLE_SERIALIZE_NORMAL, _T("flag"));

	MemArchive->Write( PARTICLE_VARIABLE_MAX, _T("keyframes"));

	// WRITE PROPERTIES
	for (int i = 0; i < PARTICLE_VARIABLE_MAX; i++)
	{
		MemArchive->Write(BrushPropertyStringTable[i], _T("type"));
		MemArchive->Write(0.0f, _T("time"));
		if (!_tcscmp(BrushPropertyStringTable[i], _T("PARTICLE_VELOCITY")) ||
			!_tcscmp(BrushPropertyStringTable[i], _T("PARTICLE_WEIGHT")) ||
			!_tcscmp(BrushPropertyStringTable[i], _T("PARTICLE_SPIN")) ||
			!_tcscmp(BrushPropertyStringTable[i], _T("PARTICLE_MOTION_RAND")) ||
			_tcsstr(BrushPropertyStringTable[i], _T("VARIATION")))
			MemArchive->Write(0.0f, _T("value"));
		else if (!_tcscmp(BrushPropertyStringTable[i], _T("PARTICLE_SIZE")))
			MemArchive->Write(50.0f, _T("value"));
		else
			MemArchive->Write(1.0f, _T("value"));
	}

	// Change the Memarchive to read
	MemArchive->SetIsWriting(false);

	CHashString hszParticleManager = _T("CParticleManager");
	PARTICLELOADINFO pli;
	pli.filepath = hszFilepath;
	pli.data = MemArchive;
	static DWORD msgHash_AddLoadedBrush = CHashString(_T("AddLoadedBrush")).GetUniqueID();
	result = m_ToolBox->SendMessage(msgHash_AddLoadedBrush, sizeof( PARTICLELOADINFO), &pli, 0, &hszParticleManager);

	MemArchive->Close();

	if (result != MSG_HANDLED)
	{
		m_ToolBox->Log(LOGERROR, _T("ParticleLoader failed to add new brush to manager map"));
		return false;
	}
	return true;
}

bool CParticleDoc::AddNewBrush(StdString *szBrushpath)
{
	DWORD result;

	TCHAR czTempFilepath[_MAX_PATH];

	// If a Brushname is provided, (dialog box or something)
	// TODO: UnImplemented.
	if (szBrushpath)
	{
		CHashString hszNewBrushFilepath((TCHAR *)szBrushpath);
		// Build the Appropriate path
		m_ParticleToolBox.BuildParticleBrushPath(czTempFilepath, m_hszEmitterFilepath.GetString(), (TCHAR *)szBrushpath);

		// Check to see if particle exists.
		GETPARTICLETYPEMESSAGE gptm;
		gptm.filename = &hszNewBrushFilepath;
		static DWORD msgHash_GetParticleType = CHashString(_T("GetParticleType")).GetUniqueID();
		result = m_ToolBox->SendMessage(msgHash_GetParticleType, sizeof(GETPARTICLETYPEMESSAGE), &gptm);
		if (!gptm.particletype)
		{
			// ADD NEW BRUSH HERE;
			CreateNewBrush(&hszNewBrushFilepath);

			gptm.filename = &hszNewBrushFilepath;
			static DWORD msgHash_GetParticleType = CHashString(_T("GetParticleType")).GetUniqueID();
			result = m_ToolBox->SendMessage(msgHash_GetParticleType, sizeof(GETPARTICLETYPEMESSAGE), &gptm);

			//Add Brush to Particle Emitter
			if (gptm.particletype)
				if (m_pParticleEmitter)
					m_pParticleEmitter->AddParticleBrush(gptm.particletype);
		}
		else
		{
			return false;
		}

	}
	// If a Brushname is not provided, create one.
	else
	{

		bool loop = true;
		
		// Keep trying NewBrush###.pml til one is unused.
		StdString szNewBrushFilepath;
		StdString szNewBrushFilename = _T("NewBrush.pml");
		m_ParticleToolBox.BuildParticleBrushPath(czTempFilepath, m_hszEmitterFilepath.GetString(), szNewBrushFilename);
		szNewBrushFilepath = m_ToolBox->GetRelativePath(czTempFilepath);
		CHashString hszNewBrushFilepath(szNewBrushFilepath);

		while(1)
		{
			// Check to see if our "new brush path" is already loaded in the emitter.
			// This should be a truely new name so make a new one if it is taken.
			GETPARTICLETYPEMESSAGE gptm;
			gptm.filename = &hszNewBrushFilepath;
			static DWORD msgHash_GetParticleTypeNoLoad = CHashString(_T("GetParticleTypeNoLoad")).GetUniqueID();
			result = m_ToolBox->SendMessage(msgHash_GetParticleTypeNoLoad, sizeof(GETPARTICLETYPEMESSAGE), &gptm);

			if (!gptm.particletype)
			{
				// ADD NEW BRUSH HERE;
				CreateNewBrush(&hszNewBrushFilepath);

				gptm.filename = &hszNewBrushFilepath;
				static DWORD msgHash_GetParticleType = CHashString(_T("GetParticleType")).GetUniqueID();
				result = m_ToolBox->SendMessage(msgHash_GetParticleType, sizeof(GETPARTICLETYPEMESSAGE), &gptm);

				//Add Brush to Particle Emitter
				if (gptm.particletype)
					if (m_pParticleEmitter)
						m_pParticleEmitter->AddParticleBrush(gptm.particletype);

				break;
			}
			else
			{
				StdString szTempBrushName;
				TCHAR czBuffer[_MAX_PATH];
				TCHAR czIntTemp[_MAX_PATH];
				TCHAR czExtTemp[_MAX_EXT];
				TCHAR czFilenameTemp[_MAX_FNAME];
				TCHAR czDrive[_MAX_DRIVE];
				TCHAR czDir[_MAX_DIR];

				_tsplitpath(szNewBrushFilepath, czDrive, czDir, czFilenameTemp, czExtTemp);
				
				szTempBrushName = czFilenameTemp;
				szTempBrushName += _itot(m_iCurrentNewBrushIndex, czIntTemp, 10);

				_tmakepath(czBuffer, czDrive, czDir, szTempBrushName, czExtTemp);
				
				szNewBrushFilepath = czBuffer;
				m_iCurrentNewBrushIndex++;
				hszNewBrushFilepath = szNewBrushFilepath;
			} // continue.
		}
	}
	// use provided name

	UpdateAllComponents( NULL, PARTICLE_UPDATE_ALLVIEWS, NULL);
	return true;
}

void CParticleDoc::ScaleParticle(float fScale)
{
	int nBrushes = 0;
	nBrushes = m_pParticleEmitter->GetNumBrushes();
	for (int i = 0; i < nBrushes; i++)
	{
		IParticleType * pBrush;
		pBrush = m_pParticleEmitter->GetBrush(i);
		assert(pBrush); // Signifies Corrupted Data

		KEYFRAME_DATA_SET keys_size;
		pBrush->GetKeyData(PARTICLE_SIZE, keys_size);
		for (int j = 0; j < keys_size.m_Size; j++)
		{
			float fTempScale = fScale * keys_size.m_Keys[j].value;
			if (fTempScale <= keys_size.m_ValueMax)
				keys_size.m_Keys[j].value = fTempScale;
			else
				keys_size.m_Keys[j].value = keys_size.m_ValueMax;
		}
		pBrush->SetKeyData(PARTICLE_SIZE, keys_size);

		KEYFRAME_DATA_SET keys_velocity;
		pBrush->GetKeyData( PARTICLE_VELOCITY, keys_velocity);
		for (int j = 0; j < keys_velocity.m_Size; j++)
		{
			float fTempScale = fScale * keys_velocity.m_Keys[j].value;
			if (fTempScale <= keys_velocity.m_ValueMax)
				keys_velocity.m_Keys[j].value = fTempScale;
			else
				keys_velocity.m_Keys[j].value = keys_velocity.m_ValueMax;
		}
		pBrush->SetKeyData(PARTICLE_VELOCITY, keys_velocity);
	}
	UpdateAllComponents( NULL, PARTICLE_UPDATE_ALLVIEWS, NULL);
}

BOOL CParticleDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

//	CreateScene();

	DWORD result;

	CHashString hszNewEmitterFilepath(_T("NewEmitter.pml"));

	CreateNewEmitter(&hszNewEmitterFilepath);

	// Save off original file path for future use to check if save path is the same.
	m_hszEmitterFilepath = hszNewEmitterFilepath;

	GETPARTICLEEMITTERMESSAGE gpem;
	gpem.filename = &hszNewEmitterFilepath;
	static DWORD msgHash_GetParticleEmitterType = CHashString(_T("GetParticleEmitterType")).GetUniqueID();
	result = m_ToolBox->SendMessage(msgHash_GetParticleEmitterType, sizeof(GETPARTICLEEMITTERMESSAGE), &gpem);
	if (result == MSG_ERROR)
		return false;

	if (gpem.emittertype)
		m_pParticleEmitter = gpem.emittertype;
	else
		return false;

	UpdateAllComponents( NULL, PARTICLE_UPDATE_ALLVIEWS, NULL);

	return true;
}

BOOL CParticleDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!lpszPathName)
		return false;

//	CreateScene();

	DWORD result;

	// Save off original file path for future use to check if save path is the same.
	m_hszEmitterFilepath = lpszPathName;

	GETPARTICLEEMITTERMESSAGE gpem;
	gpem.filename = &m_hszEmitterFilepath;
	static DWORD msgHash_GetParticleEmitterType = CHashString(_T("GetParticleEmitterType")).GetUniqueID();
	result = m_ToolBox->SendMessage(msgHash_GetParticleEmitterType,sizeof(GETPARTICLEEMITTERMESSAGE), &gpem);
	if (result == MSG_ERROR)
		return false;

	if (gpem.emittertype)
	{
		m_pParticleEmitter = gpem.emittertype;
		return true;
	}
	else
		return false;
}

BOOL CParticleDoc::DoSave(LPCTSTR lpszPathName, BOOL bReplace)
	// Save the document data to a file
	// lpszPathName = path name where to save document file
	// if lpszPathName is NULL then the user will be prompted (SaveAs)
	// note: lpszPathName can be different than 'm_strPathName'
	// if 'bReplace' is TRUE will change file name if successful (SaveAs)
	// if 'bReplace' is FALSE will not change path name (SaveCopyAs)
{
	CString newName = lpszPathName;
	if (newName.IsEmpty())
	{
		CDocTemplate* pTemplate = GetDocTemplate();
		ASSERT(pTemplate != NULL);

		newName = m_strPathName;
		if (bReplace && newName.IsEmpty())
		{
			newName = m_strTitle;
			// check for dubious filename
			int iBad = newName.FindOneOf(_T(":/\\"));
			if (iBad != -1)
				newName.ReleaseBuffer(iBad);

			// append the default suffix if there is one
			CString strExt;
			if (pTemplate->GetDocString(strExt, CDocTemplate::filterExt) &&
			  !strExt.IsEmpty())
			{
				ASSERT(strExt[0] == '.');
				int iStart = 0;
				newName += strExt.Tokenize(_T(";"), iStart);
			}
		}

		if (!AfxGetApp()->DoPromptFileName(newName,
		  bReplace ? AFX_IDS_SAVEFILE : AFX_IDS_SAVEFILECOPY,
		  OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR, FALSE, pTemplate))
			return FALSE;       // don't even attempt to save
	}

	CWaitCursor wait;

	if (!OnSaveDocument(newName))
	{
		if (lpszPathName == NULL)
		{
			// be sure to delete the file
			TRY
			{
				CFile::Remove(newName);
			}
			CATCH_ALL(e)
			{
				TRACE(traceAppMsg, 0, "Warning: failed to delete file after failed SaveAs.\n");
//				DELETE_EXCEPTION(e);
			}
			END_CATCH_ALL
		}
		return FALSE;
	}

	// reset the title and change the document name
	if (bReplace)
		SetPathName(newName);

	return TRUE;        // success
}

BOOL CParticleDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	DWORD result = 0;
//	TCHAR czFilename[_MAX_PATH];

	if (!lpszPathName)
		return false;

	if (!m_pParticleEmitter)
		return false;

	CHashString hszNewEmitterSaveFilepath(lpszPathName);
	CHashString hszOldEmitterFilepath(m_pParticleEmitter->GetName());
	CHashString hszParticleLoader(_T("CParticleLoader"));

	//"Save"	If Filepath = current Filepath, save over original file
//	if (hszSaveFilepath == m_hszEmitterFilepath)
	if (hszNewEmitterSaveFilepath == m_hszEmitterFilepath)
	{
		int numBrushes = GetNumBrushes();
		for (int i = 0; i < GetNumBrushes(); i++)
		{
			StdString szOldBrushFilepath(m_pParticleEmitter->GetBrush(i)->GetName()->GetString());
//			CHashString hszOldbrushPath(m_pParticleEmitter->GetBrush(i)->GetFilepath());

			// Brush Exists, Brush is part of Hierarchy, Brush is Part of Manager, save.
			static DWORD msgHash_SaveBrushFile = CHashString(_T("SaveBrushFile")).GetUniqueID();
			result = m_ToolBox->SendMessage(msgHash_SaveBrushFile, sizeof(TCHAR *), (void *)(const TCHAR *)szOldBrushFilepath, 0, &hszParticleLoader);
			if (result == MSG_ERROR)
				return false;
		}

		//  EMITTER		
		//	MESSAGE TO THE PARTICLE LOADERSAVER TO SAVE OUT THAT EMITTER FILE TO A SPECIFIC LOCATION. (PASSED FROM THIS FUNCTION)
		static DWORD msgHash_SaveFile = CHashString(_T("SaveFile")).GetUniqueID();
		result = m_ToolBox->SendMessage(msgHash_SaveFile, sizeof(TCHAR *), (void *)hszNewEmitterSaveFilepath.GetString(), 0, &hszParticleLoader);

		UpdateAllComponents( NULL, PARTICLE_UPDATE_ALLVIEWS, NULL);
	}
	//"Save As..."	(hszSaveFilepath != m_hszEmitterFilepath)
	else
	{
		// CREATE NEW DIRECTORY
		TCHAR czNewDirectoryPath[_MAX_PATH];
		m_ParticleToolBox.BuildParticleDirectory(czNewDirectoryPath, hszNewEmitterSaveFilepath.GetString());
		m_ToolBox->MakeDirectory(czNewDirectoryPath);

		// BRUSHES
		// RENAME BRUSHES FIRST SO EMITTER CAN CORRECTLY LIST THEM IN ITS XML BRUSH LIST

		int numBrushes = GetNumBrushes();
		for (int i = 0; i < GetNumBrushes(); i++)
		{
			StdString szOldBrushPath(m_pParticleEmitter->GetBrush(i)->GetName()->GetString());

			TCHAR tpath[_MAX_PATH];
			m_ParticleToolBox.BuildParticleBrushPath(tpath, hszNewEmitterSaveFilepath.GetString(), szOldBrushPath );
			CHashString hszNewBrushPath(tpath);

			StdString szNewRelativeBrushPath(m_ToolBox->GetRelativePath(tpath));

			CHashString hszOldBrushFilepath(szOldBrushPath);
			CHashString hszNewBrushFilepath(szNewRelativeBrushPath);


			PARTICLEFILEPARAMS pfp;
			pfp.filename = &hszOldBrushFilepath;
			pfp.newfilename = &hszNewBrushFilepath;
			static DWORD msgHash_RenameParticleBrush = CHashString(_T("RenameParticleBrush")).GetUniqueID();
//			result = m_ToolBox->SendMessage(msgHash_RenameParticleBrush, sizeof(PARTICLEFILEPARAMS), &pfp, NULL, NULL);	
//			if (result == MSG_ERROR)
//				return false;
			
			if (!RenameParticleComponent(PARTICLE_BRUSH, szOldBrushPath, szNewRelativeBrushPath))
				return false;
	
			pfp.filename = &hszNewBrushFilepath;
			static DWORD msgHash_SaveBrushFile = CHashString(_T("SaveBrushFile")).GetUniqueID();
			result = m_ToolBox->SendMessage(msgHash_SaveBrushFile, sizeof(TCHAR *), (TCHAR *)tpath, 0, &hszParticleLoader);
			if (result == MSG_ERROR)
				return false;
		}

		//	MESSAGE TO THE PARTICLE MANAGER TO RENAME A FILE IN ITS MAP
//		PARTICLEFILEPARAMS pfp;
//		pfp.filename = &hszOldEmitterFilepath;
//		pfp.newfilename = &hszNewEmitterSaveFilepath;
		static DWORD msgHash_RenameParticleEmitter = CHashString(_T("RenameParticleEmitter")).GetUniqueID();
//		result = m_ToolBox->SendMessage(msgHash_RenameParticleEmitter, sizeof(PARTICLEFILEPARAMS), &pfp, NULL, NULL);	
//		if (result == MSG_ERROR)
//			return false;
		StdString szOldEmitterFilepath(hszOldEmitterFilepath.GetString());
		StdString szNewEmitterFilepath(hszNewEmitterSaveFilepath.GetString());

		if (!RenameParticleComponent(PARTICLE_EMITTER, szOldEmitterFilepath, szNewEmitterFilepath))
			return false;

		
		//	RENAME LOCAL COPIES OF THAT FILENAME/PATH
		m_hszEmitterFilepath = hszNewEmitterSaveFilepath;

		//	MESSAGE TO THE PARTICLE LOADERSAVER TO SAVE OUT THAT EMITTER FILE TO A SPECIFIC LOCATION. (PASSED FROM THIS FUNCTION)
		static DWORD msgHash_SaveFile = CHashString(_T("SaveFile")).GetUniqueID();
		result = m_ToolBox->SendMessage(msgHash_SaveFile, sizeof(TCHAR *), (void *)hszNewEmitterSaveFilepath.GetString(), 0, &hszParticleLoader);

		UpdateAllComponents( NULL, PARTICLE_UPDATE_ALLVIEWS, NULL);
	}

	return true;
}

void CParticleDoc::SetGraphData(CView * List, StdString szItem, StdString szItemParent)
{
	CREATE_PARTICLE_STRING_TABLE(ParticlePropertyStringMap);
	CREATE_EMITTER_STRING_TABLE(EmitterPropertyStringMap);
	m_bWorkingOnEmitter = false;
	m_iWorkingOnBrush = -1;
	m_eWorkingEmitterProperty = EMITTER_VARIABLE_MAX;
	m_eWorkingParticleProperty = PARTICLE_VARIABLE_MAX;

	// The item is the Emitter
	if (szItemParent == _T(""))
	{
		EditorUpdateOptionTree(m_hszEmitterFilepath.GetString(), UPDATE_PARTICLE_EMITTER);
		return;
	}
	// The item is a Brush or an Emitter Property
	else if (szItemParent == _T("Emitter"))
	{
		m_bWorkingOnEmitter = true;

		// See if it is a brush
		for (int i = 0; i < GetNumBrushes(); i++)
		{
			StdString szBrushName(m_pParticleEmitter->GetBrush(i)->GetName()->GetString());
			if (szItem == szBrushName)
			{
				EditorUpdateOptionTree(szBrushName, UPDATE_PARTICLE_BRUSH);
				m_iWorkingOnBrush = i;
				return;
			}
		}
		for (int i = 0; i < EMITTER_VARIABLE_MAX; i++)
		{
			if (szItem == EmitterPropertyStringMap[i])
			{
				m_eWorkingEmitterProperty = static_cast<PARTICLE_EMITTER_VARIABLE>(i);
				m_pParticleEmitter->GetKeyData(static_cast<PARTICLE_EMITTER_VARIABLE>(i), m_CurrentGraphData);
				m_pParticleEmitter->GetKeyData(static_cast<PARTICLE_EMITTER_VARIABLE>(i), m_LastGraphData);
				SetModifiedFlag(1);
				UpdateAllComponents(List, PARTICLE_UPDATE_GRAPHVIEW, NULL);
				break;
			}
		}
	}
	// The item is a Brush Property
	else
	{
		m_bWorkingOnEmitter = true;

		IParticleType *temp;
		int i;
		for (i = 0; i < GetNumBrushes(); i++)
		{
			temp = m_pParticleEmitter->GetBrush(i);
			StdString szTemp(temp->GetName()->GetString());
			if (szItemParent == szTemp)
			{
				m_iWorkingOnBrush = i;
				break;
			}
		}
		for (int j = 0; j < PARTICLE_VARIABLE_MAX; j++)
		{
			if (szItem == ParticlePropertyStringMap[j])
			{
				m_eWorkingParticleProperty = static_cast<PARTICLE_VARIABLE>(j);
				IParticleType*  pBrush = m_pParticleEmitter->GetBrush(m_iWorkingOnBrush);
				if (pBrush)
				{
					pBrush->GetKeyData( static_cast<PARTICLE_VARIABLE>(j), m_CurrentGraphData);
					pBrush->GetKeyData( static_cast<PARTICLE_VARIABLE>(j), m_LastGraphData);
				}
				SetModifiedFlag(1);
				UpdateAllComponents(List, PARTICLE_UPDATE_GRAPHVIEW, NULL);
				break;
			}
		}
		i = 0;
	}
}

void CParticleDoc::EditorUpdateOptionTree(const TCHAR * czItemName, int type)
{	
	POSITION pos = GetFirstViewPosition();
	CView * pView = GetNextView( pos );
	assert(pView);

	CParticleFrame* FrameParent = DYNAMIC_DOWNCAST(CParticleFrame, pView->GetParentFrame());
	assert(FrameParent);

//	TCHAR czEmitterFilename[_MAX_PATH];
//	ExtractFileName(czEmitterFilename, m_hszEmitterFilepath.GetString());

	StdString szTypeName;
	CHashString hszItemName(czItemName);
	if (UPDATE_PARTICLE_EMITTER == type)
		szTypeName = _T("CParticleEmitter");
	if (UPDATE_PARTICLE_BRUSH == type)
		szTypeName = _T("CParticleType");
	CHashString hszTypeName(szTypeName);

	CParticleEditorPropertyDock *pDock = SINGLETONINSTANCE(CParticleEditor)->GetPropertyDock();
	pDock->SetTreeUpdateSerializeFuncCallback(UpdateTreeSerializeCallback);
	pDock->RegisterObjectCreated(hszItemName);
	pDock->UpdateOptionTree(&hszItemName, &hszTypeName);
}


void CParticleDoc::UpdateTreeSerializeCallback(StdString name, StdString type, IArchive *ar)
{
	IToolBox * TempBox = EngineGetToolBox();
	DWORD result;

	SERIALIZEOBJECTPARAMS sop;
	sop.archive = ar;
	CHashString oName = name;
	sop.name = &oName;
	static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
	result = EngineGetToolBox()->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop);
	if (result != MSG_HANDLED)
		TempBox->Log(LOGWARNING, _T("Particle Editor: %s(%i): Failed to Serialize Particle Data on OptionTree Callback"), __FILE__, __LINE__);

	return;
}

void CParticleDoc::UpdateParticle(float lasteffectedkeytime)
{
	// Reset Particle Keyframe to last.
	if (m_bWorkingOnEmitter)
	{
		if (m_iWorkingOnBrush != -1)
		{
			// Valid Brush and Valid Brush Property
			if (m_eWorkingParticleProperty != PARTICLE_VARIABLE_MAX)
			{
				// Set Keyframe Data.
				m_pParticleEmitter->GetBrush(m_iWorkingOnBrush)->SetKeyData(m_eWorkingParticleProperty, m_CurrentGraphData);
				if (lasteffectedkeytime == 0.0f)
					RestartParticle();
				else
					SetParticleTime(lasteffectedkeytime);
			}
		}
		// Not working on a Brush or Brush Property
		// Try Emitter Property...
		else
		{
			if (m_eWorkingEmitterProperty != EMITTER_VARIABLE_MAX)
			{
				m_pParticleEmitter->SetKeyData(m_eWorkingEmitterProperty, m_CurrentGraphData);
				if (lasteffectedkeytime == 0.0f)
					RestartParticle();
				else
					SetParticleTime(lasteffectedkeytime);
			}
		}
	}
}

KEYFRAME_DATA_SET * CParticleDoc::GetKeyData()
{
	return &m_CurrentGraphData;
}

int CParticleDoc::GetNumBrushes()
{
	return m_pParticleEmitter->GetNumBrushes();
}

void CParticleDoc::CreateScene()
{
	CreateEEObject(NULL, &m_hszSceneType, m_hszSceneName, NULL);
}

void CParticleDoc::SetActiveScene()
{
	GETSETACTIVESCENE gsas;
	gsas.pSceneName = &m_hszSceneName;
	static DWORD msgHash_SetActiveScene = CHashString(_T("SetActiveScene")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetActiveScene, sizeof(gsas), &gsas);
}

void CParticleDoc::CreateParticleSystem()
{
	IArchive *pArchive = CreateMemoryArchive();
	if (pArchive == NULL)
	{
		return;
	}

	CFileVersionSetter lastVersionSetter;

	// filling archive for editor object entity
	Vec3 v3Zero;
	EulerAngle eaZero;
	Vec3 v3Scale(1.0f, 1.0f, 1.0f);
	pArchive->SetIsWriting(true);
	StdString szEntityType(_T("ParticleEditorObject"));
	pArchive->Write(szEntityType, _T("EntityType"));
	pArchive->Write(v3Zero, _T("Position"));
	pArchive->Write(eaZero, _T("Rotation"));
	pArchive->Write(v3Scale, _T("Scale"));
	pArchive->SetIsWriting(false);

	static CHashString hszEntity(_T("CEntity"));
	// Create Instance of CEntity
	if (CreateEEObject(&m_hszSceneName, &hszEntity, m_hszEntityName, pArchive))
	{
		StdString szParticleObjectName;
		CHashString hszParticleObjectName;

		// filling archive for particle render object
		StdString szEmitterRelativeFilepath = GetPathName();
		pArchive->SetIsWriting(true);
		pArchive->SeekTo(0);
		pArchive->Write(szEmitterRelativeFilepath, _T("FileName"));
		pArchive->Write(true , _T("Enabled"));
		pArchive->Write(30000.0f, _T("ClipDistance"));
		pArchive->Write(-1.0f, _T("RemaingLife"));
		pArchive->Write(true, _T("ScreenAligned"));
		pArchive->SetIsWriting(false);

		// Create Instance of Particle
		static CHashString hszCParticleRenderObject(_T("CParticleRenderObject"));
		CreateEEObject(&m_hszEntityName, &hszCParticleRenderObject, m_hszParticleName, pArchive);

		static DWORD msgHash_Enable = CHashString(_T("Enable")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_Enable, 0, 0, &m_hszParticleName, &hszCParticleRenderObject);

		static DWORD msgHash_InitParticles = CHashString(_T("InitParticles")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_InitParticles,0,NULL);
	}

	pArchive->Close();
}

void CParticleDoc::RemoveParticleSystem()
{
	DeleteEEObject(&m_hszParticleName);
	DeleteEEObject(&m_hszEntityName);
}

void CParticleDoc::SetParticleTime(float time)
{
	static DWORD msgHash_SetParticleTime = CHashString(_T("SetParticleTime")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetParticleTime, sizeof(time), (void *)&time, &m_hszParticleName, 0);
}

void CParticleDoc::RestartParticle()
{
	static DWORD msgHash_Reset = CHashString(_T("Reset")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_Reset, 0, 0, &m_hszParticleName, 0);
}