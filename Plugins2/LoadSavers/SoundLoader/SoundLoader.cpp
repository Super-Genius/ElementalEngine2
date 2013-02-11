///============================================================================
/// \file		SoundLoader.cpp
/// \brief		Implementation file for Sound Loading
/// \date		10-05-2006
/// \author		Jonathan Burgess
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

REGISTER_COMPONENT_SINGLETON(CSoundLoader);
REGISTER_MESSAGE_HANDLER(LoadFile, OnLoadFile, CSoundLoader);
REGISTER_MESSAGE_HANDLER(SaveFile, OnSaveFile, CSoundLoader);

CHashString CSoundLoader::m_HashName(_T("CSoundLoader"));

LSRC_MESSAGE CSoundLoader::m_LoadSaveRegisterData = {
		&CSoundLoader::m_HashName,
		_T("MP3 Sound Files (*.mp3)\0" "*.mp3\0")
		_T("Ogg Vorbis Sound Files (*.ogg)\0" "*.ogg\0")
		_T("WAVE Sound Files (*.wav)\0" "*.wav\0")
		_T("Playlist Files (*.xmp)\0" "*.xmp\0")
		_T("\0\0"),
		_T("MP3 Sound Files (*.mp3)\0" "*.mp3\0")
		_T("Ogg Vorbis Sound Files (*.ogg)\0" "*.ogg\0")
		_T("WAVE Sound Files (*.wav)\0" "*.wav\0")
		_T("Playlist Files (*.xmp)\0" "*.xmp\0")
		_T("\0\0"),
		true, CSoundLoader::PrecacheCallback,
};

bool CSoundLoader::PrecacheCallback(IHashString *ext)
{
	static CHashString hsXmpExt(_T(".xmp"));
	if ((ext) && (ext->GetUniqueID() == hsXmpExt.GetUniqueID()))
	{
		return false;
	}
	else
	{
		return true;
	}
}

CSoundLoader::CSoundLoader() 
{
	// save for convenience
	m_ToolBox = EngineGetToolBox();
	DWORD check;

	static DWORD msgHash_RegisterLoadSaveComponent = CHashString(_T("RegisterLoadSaveComponent")).GetUniqueID();
	check = m_ToolBox->SendMessage(msgHash_RegisterLoadSaveComponent, sizeof(LSRC_MESSAGE),&m_LoadSaveRegisterData);

	if (!check)
	{
		// log error RegisterLoadSaveComponent failed!
	}
}

// create function for this world loader
IComponent *CSoundLoader::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE( CSoundLoader );
}

IHashString *CSoundLoader::GetComponentType()
{
	return &m_HashName;
}

bool CSoundLoader::IsKindOf(IHashString *compType)
{
	return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
}

DWORD CSoundLoader::OnLoadFile(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(TCHAR *));
	TCHAR *pFileName = (TCHAR *)params;
	
	StdString fileName = pFileName;

	fileName.tolower();

	DWORD retval = MSG_HANDLED_STOP;
	CHashString hszFileName(fileName);

	set<DWORD>::iterator fileIter = m_FilesLoaded.find(hszFileName.GetUniqueID());
	if (fileIter != m_FilesLoaded.end())
	{
		// this file has already been loaded (hopefully correctly)
		return retval;
	}

	StdString extension, currentToken;
	fileName.GetToken(_T("."), currentToken);

	while (_tcscmp(currentToken, _T("")) != 0)
	{
		extension = currentToken;
		fileName.GetToken(_T("."), currentToken);
	}
	
	static CHashString hszWav(_T("wav"));
	static CHashString hszOgg(_T("ogg"));
	static CHashString hszMP3(_T("mp3"));
	static CHashString hszXmp(_T("xmp"));

	CHashString hszExt(extension);
	DWORD extID = hszExt.GetUniqueID();

	if ((extID == hszWav.GetUniqueID()) ||
		(extID == hszOgg.GetUniqueID()) ||
		(extID == hszMP3.GetUniqueID()))
	{
		bool success = LoadSoundFile(fileName, &hszExt);
		if (!success)
		{
			m_ToolBox->Log(LOGWARNING, _T("Failed to load sound %s\n"), (const TCHAR*)fileName);
			retval = MSG_WARNING;
		}
		else
		{
			m_FilesLoaded.insert(hszFileName.GetUniqueID());
		}
	}

	else if (extID == hszXmp.GetUniqueID())
	{
		bool success = LoadPlaylistFile(fileName);
		if (!success)
		{
			m_ToolBox->Log(LOGWARNING, _T("Failed to load playlist %s\n"), (const TCHAR*)fileName);
			retval = MSG_WARNING;
		}
		else
		{
			m_FilesLoaded.insert(hszFileName.GetUniqueID());
		}
	}

	return retval;
}

DWORD CSoundLoader::OnSaveFile(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(TCHAR *));
	TCHAR *pFileName = (TCHAR *)params;

	DWORD retval = MSG_HANDLED_STOP;

	return retval;
}

bool CSoundLoader::LoadSoundFile(const TCHAR* fileName, IHashString *extension)
{
	bool ret = false;

	// create a file stream and send that to the manager to decide to load it into ram or keep the
	// stream
	CREATESTREAM cs;
	cs.streamData =	(void*)fileName;
	cs.mode = STREAM_MODE_READ | STREAM_MODE_BINARY;
	
	static DWORD msgHash_CreateStream_File = CHashString(_T("CreateStream_File")).GetUniqueID();
	DWORD msgRet = m_ToolBox->SendMessage(msgHash_CreateStream_File, sizeof(CREATESTREAM), &cs);
	if (msgRet != MSG_HANDLED)
	{
		m_ToolBox->Log(LOGWARNING, _T("Sound loader: could not create file stream!\n"));
		return false;
	}

	if (cs.openStream == NULL)
	{
		m_ToolBox->Log(LOGWARNING, _T("Sound loader: stream returned NULL!\n"));
		return false;
	}

	// send stream to manager
	ADDSOUNDOBJECTPARAMS sp;
	CHashString zFileName(fileName);

	sp.FileName = &zFileName;
	sp.fileType = extension;
	sp.fileStream = cs.openStream;

	static DWORD msgHash_AddSound = CHashString(_T("AddSound")).GetUniqueID();
	msgRet = m_ToolBox->SendMessage(msgHash_AddSound, sizeof(ADDSOUNDOBJECTPARAMS), &sp);
	if (msgRet == MSG_HANDLED)
	{
		ret = true;
	}	

	return ret;
}

bool CSoundLoader::LoadPlaylistFile(const TCHAR* fileName)
{
	CHashString hszFileName(fileName);

	CHashString streamType(_T("File"));
	CREATEARCHIVE ca;
	
	ca.streamData = (void *)fileName;
	ca.mode = STREAM_MODE_READ;
	ca.streamType = &streamType;
	
	static DWORD msgHash_CreateXMLArchive = CHashString(_T("CreateXMLArchive")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_CreateXMLArchive, sizeof(CREATEARCHIVE), &ca) != MSG_HANDLED)
	{
		m_ToolBox->Log(LOGWARNING, _T("Could not create archive for playlist %s\n"), (const TCHAR*)fileName);
		return false;
	}

	LOADPLAYLISTPARAMS lpp;
	lpp.fileName = &hszFileName;
	lpp.ar = ca.archive;

	static DWORD msgHash_LoadMusicPlaylist = CHashString(_T("LoadMusicPlaylist")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_LoadMusicPlaylist, sizeof(LOADPLAYLISTPARAMS), &lpp) != MSG_HANDLED)
	{
		m_ToolBox->Log(LOGWARNING, _T("Could not send playlist data to sound manager\n"));
		return false;
	}
	ca.archive->Close();

	return true;
}
	
bool CSoundLoader::SavePlaylistFile(const TCHAR* fileName)
{
	return false;
}
