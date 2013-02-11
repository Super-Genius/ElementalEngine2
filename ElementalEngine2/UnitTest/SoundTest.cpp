#include "stdafx.h"
#include <math.h> // d'oh!

CSoundTest::CSoundTest()
{
	Initialize();
	m_iTestState = 0;
	m_fTimeCheck = 0.0f;
	m_fElapsedTime = 0.0f;
	m_hsCurrentPlayingObjName = _T("");	
}

bool CSoundTest::TestFileExists(TCHAR *fileName, GETSOUNDFILEINFOPARAMS *gsfip)
{
	bool ret = false;

	DWORD msgRet = LoadFile(fileName);
	if (msgRet == MSG_HANDLED)
	{
		CHashString hsFileName(fileName);
		gsfip->fileName = &hsFileName;
		static DWORD msgHash_GetSoundFileInfo = CHashString(_T("GetSoundFileInfo")).GetUniqueID();
		DWORD msgRet = m_ToolBox->SendMessage(msgHash_GetSoundFileInfo, sizeof(GETSOUNDFILEINFOPARAMS), gsfip);
		if (msgRet == MSG_HANDLED)
		{
			ret = true;	
		}
	}

	return ret;
}

bool CSoundTest::Test2DFilePlaybackNoLoop(TCHAR *fileName)
{
	bool ret = false;

	PLAYSOUNDPARAMS psp;
	CHashString hsFileName(fileName);
	
	psp.fileName = &hsFileName;
	psp.looping = false;
	psp.autoRemove = true;
	psp.volume = 1.0;
	psp.is3DSound = false;
	
	static DWORD msgHash_PlaySound = CHashString(_T("PlaySound")).GetUniqueID();
	DWORD msgRet = m_ToolBox->SendMessage(msgHash_PlaySound, sizeof(PLAYSOUNDPARAMS), &psp);
	if (msgRet == MSG_HANDLED)
	{
		ret = true;
	}

	return ret;
}

bool CSoundTest::Test2DFilePlaybackLoopStart(TCHAR *fileName)
{
	bool ret = false;

	PLAYSOUNDPARAMS psp;
	CHashString hsFileName(fileName);
		
	psp.fileName = &hsFileName;
	psp.looping = true;
	psp.autoRemove = true;
	psp.volume = 1.0;
	psp.is3DSound = false;
	psp.objectName = &m_hsCurrentPlayingObjName;
	
	static DWORD msgHash_PlaySound = CHashString(_T("PlaySound")).GetUniqueID();
	DWORD msgRet = m_ToolBox->SendMessage(msgHash_PlaySound, sizeof(PLAYSOUNDPARAMS), &psp);
	if (msgRet == MSG_HANDLED)
	{
		ret = true;
	}

	return ret;
}

bool CSoundTest::Test2DFilePlaybackLoopStop()
{
	bool ret = false;
	if (_tcscmp(m_hsCurrentPlayingObjName.GetString(), _T("")) != 0)
	{
		STOPSOUNDPARAMS ssp;
		ssp.soundObjectName = &m_hsCurrentPlayingObjName;
		static DWORD msgHash_StopSound = CHashString(_T("StopSound")).GetUniqueID();
		DWORD msgRet = m_ToolBox->SendMessage(msgHash_StopSound, sizeof(STOPSOUNDPARAMS), &ssp);
		if (msgRet == MSG_HANDLED)
		{
			ret = true;
			m_hsCurrentPlayingObjName = _T("");
		}
	}
	return ret;
}

bool CSoundTest::Test3DFilePlaybackStatic(TCHAR *fileName, Vec3 pos)
{
	bool ret = false;
	PLAYSOUNDPARAMS psp;
	CHashString hsFileName(fileName);
		
	psp.fileName = &hsFileName;
	psp.looping = false;
	psp.autoRemove = true;
	psp.volume = 1.0;
	psp.is3DSound = true;
	psp.objectName = &m_hsCurrentPlayingObjName;
	psp.soundPosition = &pos;
	
	static DWORD msgHash_PlaySound = CHashString(_T("PlaySound")).GetUniqueID();
	DWORD msgRet = m_ToolBox->SendMessage(msgHash_PlaySound, sizeof(PLAYSOUNDPARAMS), &psp);
	if (msgRet == MSG_HANDLED)
	{
		ret = true;
	}

	return ret;
}

bool CSoundTest::Test3DSoundSpawner(TCHAR *spawnerFile)
{
	DWORD retval = LoadFile(spawnerFile);
	if (retval != MSG_HANDLED)
	{
		return false;
	}
	static DWORD msgHash_GetSpawnedName = CHashString(_T("GetSpawnedName")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetSpawnedName, sizeof(IHashString*), &m_hsCurrentPlayingObjName);
	return true;
}

bool CSoundTest::Test3DStopLoopingSpawnedSound()
{
	static DWORD msgHash_StopSoundObject = CHashString(_T("StopSoundObject")).GetUniqueID();
	DWORD retval = m_ToolBox->SendMessage(msgHash_StopSoundObject, 0, NULL, &m_hsCurrentPlayingObjName);
	if (retval != MSG_HANDLED)
	{
		return false;
	}
	return true;
}

void CSoundTest::Update3DSoundPositionLinear()
{
	float yPos = cos(m_fElapsedTime) * 10.0f;	
	Vec3 pos(1.0f, yPos, 0.0f);
	static DWORD msgHash_SetGlobalPosition = CHashString(_T("SetGlobalPosition")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetGlobalPosition, sizeof(Vec3), &pos, &m_hsCurrentPlayingObjName);
}

void CSoundTest::Update3DSoundPositionRadial()
{
	float xPos = sin(m_fElapsedTime) * 10.0f;
	float yPos = cos(m_fElapsedTime) * 10.0f;		
	Vec3 pos(xPos, yPos, 0.0f);
	static DWORD msgHash_SetGlobalPosition = CHashString(_T("SetGlobalPosition")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetGlobalPosition, sizeof(Vec3), &pos, &m_hsCurrentPlayingObjName);
}

bool CSoundTest::TickTime()
{
	//float deltaT;
	//static DWORD msgHash_GetTimeDelta = CHashString(_T("GetTimeDelta")).GetUniqueID();
	//m_ToolBox->SendMessage(msgHash_GetTimeDelta, sizeof(float), &deltaT);

	//m_fElapsedTime += deltaT;
	ITimer* timer = m_ToolBox->GetTimer();
	m_fElapsedTime += timer->GetTimeDelta();
	if (m_fElapsedTime >= m_fTimeCheck)
	{
		m_fElapsedTime = 0.0f;
		return true;
	}
	return false;
}

bool CSoundTest::TestPlaylistLoad(TCHAR *fileName)
{
	bool ret = false;

	DWORD msgRet = LoadFile(fileName);
	if (msgRet == MSG_HANDLED)
	{
		ret = true;
	}

	return ret;
}

bool CSoundTest::TestPlaylistPlay(TCHAR *fileName)
{
	bool ret = false;

	CHashString hsFileName(fileName);
	static DWORD msgHash_RunPlaylist = CHashString(_T("RunPlaylist")).GetUniqueID();
	DWORD msgRet = m_ToolBox->SendMessage(msgHash_RunPlaylist, sizeof(IHashString*), &hsFileName);
	if (msgRet == MSG_HANDLED)
	{
		ret = true;
	}

	return ret;
}