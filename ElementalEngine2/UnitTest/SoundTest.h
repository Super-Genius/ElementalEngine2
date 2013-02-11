#ifndef _C_SOUNDTEST_H_
#define _C_SOUNDTEST_H_

class CSoundTest : public CTestBase
{
public:
	CSoundTest();	
	~CSoundTest(){}

	// tests
	bool TestFileExists(TCHAR *fileName, GETSOUNDFILEINFOPARAMS *gsfip);
	bool Test2DFilePlaybackNoLoop(TCHAR *fileName);
	bool Test2DFilePlaybackLoopStart(TCHAR *fileName);
	bool Test2DFilePlaybackLoopStop();
	bool Test3DFilePlaybackStatic(TCHAR *fileName, Vec3 pos);
	bool Test3DSoundSpawner(TCHAR *spawnerFile);
	bool Test3DStopLoopingSpawnedSound();
	void Update3DSoundPositionLinear();
	void Update3DSoundPositionRadial();
	bool TestPlaylistLoad(TCHAR *fileName);
	bool TestPlaylistPlay(TCHAR *fileName);
	
	// testing state
	UINT GetTestState() { return m_iTestState; }
	void IncrementTestState() { m_iTestState++; }
	void SetFailureState() { m_iTestState = 0xffffffff; }
	void ResetTestState() { m_iTestState = 0; }

	// tick stuffs
	void SetTimeToCheck(float val) { m_fTimeCheck = val; }
	bool TickTime();

private:
	UINT m_iTestState;	
	float m_fTimeCheck;
	float m_fElapsedTime;
	CHashString m_hsCurrentPlayingObjName;
};

#endif //_C_SOUNDTEST_H_