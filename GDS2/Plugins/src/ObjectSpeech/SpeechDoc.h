#if !defined(AFX_SpeechDOC_H__80A18743_A0D3_4593_AADE_3EB5BA220CC5__INCLUDED_)
#define AFX_SpeechDOC_H__80A18743_A0D3_4593_AADE_3EB5BA220CC5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SpeechDoc.h : header file
//

#include <sapi.h>           // SAPI
#include <sphelper.h>
#include <spuihelp.h>
#include <vector>
#include "CWaveFile.h"

/////////////////////////////////////////////////////////////////////////////
// CSpeechDoc document


class CSpeechDoc : public CDocument
{
protected:
	
	CSpeechDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CSpeechDoc)

// Attributes
public:

// Operations
public:

	void Test_Speech( void );

	void Speak( void );
    void Speak_TXT( void );
	void Speak_WAV( void );
	void Speak_XML( void );

	void Parse( void );
	void Parse_TXT( void );
	void Parse_WAV( void );
	void Parse_XML( void );

	CString GrabEventString( SPEVENTENUM );
	CString GrabVisemeString( SPVISEMES );

	void BuildVisemeFile( void );
	
	LPTSTR GetLoadStrings();
	LPTSTR GetSaveStrings();

	DWORD Load(LPTSTR filename);
	DWORD Save(LPTSTR filename);

	void SetFileName(CString f);

	//void Edit();

	CString m_Filename;
	CWaveFile m_cWave;
	ISpStream * m_pStream;
	ISpVoice * m_pVoice;
	vector<SPEVENT *> m_vEventList;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpeechDoc)
	public:
	
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	protected:
	
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	
	virtual ~CSpeechDoc();
#ifdef _DEBUG
	
	virtual void AssertValid() const;
	
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CSpeechDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSpeechTest1();
	afx_msg void OnUpdateSpeechTest1(CCmdUI *pCmdUI);
	afx_msg void OnSpeechTest2();
	afx_msg void OnUpdateSpeechTest2(CCmdUI *pCmdUI);
	afx_msg void OnSpeechTest3();
	afx_msg void OnUpdateSpeechTest3(CCmdUI *pCmdUI);
	afx_msg void OnTestPalButton2();
	afx_msg void OnUpdateTestPalButton2(CCmdUI* pCmdUI);
	afx_msg void On3dButton();
	afx_msg void OnUpdate3dButton(CCmdUI* pCmdUI);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SpeechDOC_H__80A18743_A0D3_4593_AADE_3EB5BA220CC5__INCLUDED_)
