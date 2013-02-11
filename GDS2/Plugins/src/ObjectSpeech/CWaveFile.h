/*************************************************************
	CWaveFile.h - intarface to CWaveFile class 
			       it ecapsulates most functionality to deal
			       with wave files.
       version:	 1.1
	
       If you are going to use or change this class, please let me know by email.
	(please, explaine also the purpose of app where you want to add CWaveFile) 


	22.02.03	Alexander Beletsky(c) 
	albel@pochtamt.ru
****************************************************************/



#ifndef CWAVEFILE_H
#define CWAVEFILE_H

#include "filemap.h"

struct WAVEFORM
{
	WORD wFormatTag;
	WORD nChannels;
	DWORD nSamplesPerSec;
	DWORD nAvgBytesPerSec;
	WORD nBlockAlign;
	WORD wBitsPerSample;
};

struct RIFF 
{
	_TCHAR riffID[4];
	DWORD riffSIZE;
	_TCHAR riffFORMAT[4];
};

struct FMT
{
	_TCHAR fmtID[4];
	DWORD fmtSIZE;
	WAVEFORM fmtFORMAT;
};

struct DATA 
{
	_TCHAR dataID[4];
	DWORD dataSIZE;
};

typedef short	AudioWord;
typedef unsigned char	AudioByte;

//Exception classes for CWaveFile
namespace WaveErrors {
	class FileOperation {};
	class RiffDoesntMatch {};
	class WaveDoesntMatch {};
	class FmtDoesntMatch {};
	class DataDoesntMatch {};
}

class CWaveFile : protected CFileMap, public  CObject {
public:
	CWaveFile( );
    CWaveFile( LPCTSTR fileName );
	~CWaveFile() {}
	void Load( LPCTSTR fileName );
	WAVEFORM* GetWaveFormat() { return &pFMT->fmtFORMAT; }
	DATA* GetWaveData() { return pDATA; }
	LPVOID GetData() { return reinterpret_cast< LPVOID >( dataAddress ); }
	BOOL DrawData( CDC *pDC, CRect *pRect );
	DWORD GetDataSize( void );
protected:
	PBYTE dataAddress;
	RIFF* pRIFF;
	FMT* pFMT;
	DATA* pDATA;
private:
    BOOL CheckID(_TCHAR* idPar,_TCHAR A, _TCHAR B, _TCHAR C, _TCHAR D);
	void ReadWave();
	void ReadRIFF();
	void ReadFMT();
	void ReadDATA();
	void DrawByte( CDC *pDC );
	void DrawWord( CDC *pDC );

	bool m_bWaveLoaded;
};

#endif	//CWAVEFILE_H