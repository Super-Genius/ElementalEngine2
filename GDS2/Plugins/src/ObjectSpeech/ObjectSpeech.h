#if !defined(AFX_ObjectSpeech_H__80A18743_A0D3_4593_AADE_3EB5BA220CC5__INCLUDED_)
#define AFX_ObjectSpeech_H__80A18743_A0D3_4593_AADE_3EB5BA220CC5__INCLUDED_

// ObjectSpeech.cpp : implementation file
//

#include "stdafx.h"
#include <sapi.h>           // SAPI
#include <sphelper.h>
#include <spuihelp.h>
//#include <wchar.h>
//#include <objbase.h>
#include <vector>

#define _ATL_APARTMENT_THREADED
#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override something, 
//but do not change the name of _Module
//extern CComModule _Module;
#include <atlcom.h>

#include "CWaveFile.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ObjectSpeech.h : header file
//
#undef AFX_DATA
#define AFX_DATA AFX_DATA_IMPORT

/////////////////////////////////////////////////////////////////////////////
// CObjectSpeech document
class CObjectSpeech : public CObjectDLLInterface
{
// attributes
public:
	UINT m_PageIndex;
	CProgramInfo *m_ProgramInfo;
    
// implementation
public:
	CObjectSpeech(CProgramInfo *ProgramInfo);
	~CObjectSpeech();
/*
	void Test_Speech( void );

	void Speak( void );
    void Speak_TXT( void );
	void Speak_WAV( void );
	void Speak_XML( void );

	void Parse( void );
	void Parse_TXT( void );
	void Parse_WAV( void );
	void Parse_XML( void );

	CString GrabEventString( int );
*/	
	LPTSTR GetLoadStrings();
	LPTSTR GetSaveStrings();

	DWORD Load(LPTSTR filename);
	DWORD Save(LPTSTR filename);

/*	void SetFileName(CString f);

	void DrawWave( CDC *pDC, CRect *pRect );
	void DrawEvents( CDC *pDC, CRect *pRect );
*/
	void Edit();

	void ChangeProperties(DWORD itemID);
	DWORD GetProperties(DWORD itemID);
    virtual CString *GetName();
    virtual BOOL Initialize();
	
//	ISpStream * m_pStream;
//	ISpVoice * m_pVoice;
//  ISpAudio * m_pOutAudio;
//	ISpRecognizer * m_pRecognizer;
//	ISpRecoContext * m_pContext;
//	ISpRecoGrammar * m_pGrammar;

//	vector<SPEVENT *> m_vEventList;

	
// attributes
protected:
	CMultiDocTemplate *m_DocTemplate;
//	long				m_Rate;
//	USHORT				m_Volume;
//	CString			m_Filename;
//	CWaveFile m_cWave;
	
};

extern CObjectSpeech *gObjectSpeech;
extern AFX_EXTENSION_MODULE ObjectSpeechDLL;

#undef AFX_DATA
#define AFX_DATA

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ObjectSpeech_H__80A18743_A0D3_4593_AADE_3EB5BA220CC5__INCLUDED_)

