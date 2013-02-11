// SpeechDoc.cpp : implementation file
//

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSpeechDoc

IMPLEMENT_DYNCREATE(CSpeechDoc, CDocument)

CSpeechDoc::CSpeechDoc()
{
}

BOOL CSpeechDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CSpeechDoc::~CSpeechDoc()
{
}


BEGIN_MESSAGE_MAP(CSpeechDoc, CDocument)
	//{{AFX_MSG_MAP(CSpeechDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_3D_BUTTON, On3dButton)
	ON_UPDATE_COMMAND_UI(ID_3D_BUTTON, OnUpdate3dButton)
	ON_COMMAND(ID_Speech_TEST1, OnSpeechTest1)
	ON_UPDATE_COMMAND_UI(ID_Speech_TEST1, OnUpdateSpeechTest1)
	ON_COMMAND(ID_Speech_TEST2, OnSpeechTest2)
	ON_UPDATE_COMMAND_UI(ID_Speech_TEST2, OnUpdateSpeechTest2)
	ON_COMMAND(ID_Speech_TEST3, OnSpeechTest3)
	ON_UPDATE_COMMAND_UI(ID_Speech_TEST3, OnUpdateSpeechTest3)
	ON_COMMAND(ID_TEST_PAL_BUTTON_2, OnTestPalButton2)
	ON_UPDATE_COMMAND_UI(ID_TEST_PAL_BUTTON_2, OnUpdateTestPalButton2)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpeechDoc diagnostics

#ifdef _DEBUG
void CSpeechDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSpeechDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSpeechDoc serialization

void CSpeechDoc::Serialize(CArchive& ar)
{
	HRESULT             hr = S_OK;

	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		SetFileName( ar.m_strFileName );
	}
}

/////////////////////////////////////////////////////////////////////////////
// CSpeechDoc commands

//-----------------------------
// Function:	SetFileName
// Description:	Sets the Filename of the object
// Parameters:	CString * f
// Returns:		.
//-----------------------------
void CSpeechDoc::SetFileName(CString f)
{
	m_Filename = f;
	LPTSTR ptr = f.LockBuffer();
	m_cWave.Load( ptr );
	f.UnlockBuffer();
}

void CSpeechDoc::OnTestPalButton2() 
{
	//MessageBox(NULL,_T("Parsing Wav\n"), _T("Speech"), MB_OK);	
	//Parse();	
	MessageBox(NULL,_T("Saving Viseme\n"), _T("Speech"), MB_OK);	
	BuildVisemeFile();
}


void CSpeechDoc::OnUpdateTestPalButton2(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();
}

void CSpeechDoc::On3dButton() 
{
	MessageBox(NULL,_T("Speaking Wav\n"), _T("Speech"), MB_OK);	
    Speak();
}


void CSpeechDoc::OnUpdate3dButton(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();	
}

//-----------------------------
// Function:	Init_Speech
// Description:	Tests the Speak Engine
// Parameters:	.
// Returns:		.
//-----------------------------
void CSpeechDoc::Test_Speech( void )
{
	HRESULT hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void **)&m_pVoice);
    if( SUCCEEDED( hr ) )
    {
        hr = m_pVoice->Speak(L"Microsoft Speech 5.1 Successfully Loaded", 0, NULL);
        m_pVoice->Release();
        m_pVoice = NULL;
    }
	
}

//-----------------------------
// Function:	Speak_TXT
// Description:	Feeds a TXT file to the Speak
// Parameters:	.
// Returns:		.
//-----------------------------
void CSpeechDoc::Speak_TXT( void )
{
	USES_CONVERSION;	// To Convert CString to WCHAR *, call thid and macro A2W( )
	HRESULT hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void **)&m_pVoice);
	hr = m_pVoice->Speak( A2W(m_Filename), SPF_IS_FILENAME, NULL );
	m_pVoice->Release();
}

//-----------------------------
// Function:	Speak_WAV
// Description:	Feeds a WAV file to the Speak
// Parameters:	.
// Returns:		.
//-----------------------------
void CSpeechDoc::Speak_WAV( void )
{
	SPEVENT * temp;
	bool end;
	HRESULT hr = SPBindToFile( m_Filename , SPFM_OPEN_READONLY, &m_pStream );
	if( SUCCEEDED( hr ) )
    {
		hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void **)&m_pVoice);
		hr = m_pVoice->SetInterest( SPFEI(SPEI_VISEME) ,
									SPFEI(SPEI_VISEME) );
        hr = m_pVoice->SpeakStream( m_pStream, SPF_DEFAULT, NULL );

		end = false;
		do
		{		
			CSpEvent spEvent;
			if( S_OK == spEvent.GetFrom( m_pVoice ) )
			{
				temp = new SPEVENT;
				spEvent.CopyTo(temp);
				m_vEventList.push_back(temp);			
			}
			else
				end = true;
		} while( end == false );

        m_pVoice->Release();
        m_pVoice = NULL;
    }
	hr = m_pStream->Close();
    
	UpdateAllViews(NULL);

}

//-----------------------------
// Function:	Speak_XML
// Description:	Feeds an XML file to the Speak
// Parameters:	.
// Returns:		.
//-----------------------------
void CSpeechDoc::Speak_XML( void )
{
	USES_CONVERSION;	// To Convert CString to WCHAR *, call thid and macro A2W( )
	HRESULT hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void **)&m_pVoice);
	hr = m_pVoice->Speak( A2W(m_Filename), SPF_IS_XML | SPF_IS_FILENAME, NULL );
	m_pVoice->Release();
}

//-----------------------------
// Function:	Speak
// Description:	Feeds an object to the TTS Engine
// Parameters:	.
// Returns:		.
//-----------------------------
void CSpeechDoc::Speak( void )
{
	if( m_Filename.Find(".txt") != -1 )
		Speak_TXT();

	else if( m_Filename.Find(".wav") != -1 )
		Speak_WAV();
	
	else if( m_Filename.Find(".xml") != -1 )
		Speak_XML();

	else
		Speak_TXT();
}

//-----------------------------
// Function:	Parse_TXT
// Description:	Convertes a TXT to Text/Phonemes
// Parameters:	.
// Returns:		.
//-----------------------------
void CSpeechDoc::Parse_TXT( void )
{
	USES_CONVERSION;
	HRESULT hr = S_OK;
	CSpEvent Events;
	
	
	hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void **)&m_pVoice);
	hr = m_pVoice->SetInterest( SPFEI_ALL_EVENTS, SPFEI_ALL_EVENTS );
	hr = m_pVoice->Speak( A2W(m_Filename), SPF_IS_FILENAME, NULL );

	int c = 0;
	while( Events.GetFrom(m_pVoice) == S_OK )
	{
		//m_vEventList.push_back(temp);	
	}
    
	m_pVoice->Release();
    m_pVoice = NULL;
}

//-----------------------------
// Function:	Parse_WAV
// Description:	Convertes a WAV to Text/Phonemes
// Parameters:	.
// Returns:		.
//-----------------------------
void CSpeechDoc::Parse_WAV( void )
{
	ISpVoice * pVoice = NULL;
	CSpEvent Events;
	HRESULT hr = S_OK;
	ISpStream * cpInputStream;
	ISpRecognizer * cpRecognizer;
	ISpRecoContext * cpRecoContext;
	ISpRecoGrammar * cpRecoGrammar;

    CSpEvent        event;  // helper class in sphelper.h for events that releases any 
                            // allocated memory in it's destructor - SAFER than SPEVENT
    int i = 0;
	int bExit = 0;
	CSpStreamFormat sInputFormat;

	CoInitialize(NULL);

    hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void **)&pVoice);
	if( FAILED( hr ) )
	{
		printf( "CoCreateInstance (CLSID_SpVoice): Failed\n\n" );
	}

	hr = CoCreateInstance( CLSID_SpStream, NULL, CLSCTX_ALL, IID_ISpStream, (void **)&cpInputStream );
	if( FAILED( hr ) )
	{
		printf( "CoCreateInstance (CLSID_SpStream): Failed\n\n");
	}

	hr = sInputFormat.AssignFormat(SPSF_22kHz16BitStereo);
	if( FAILED( hr ) )
	{
		printf(" Assign Format: Failed\n\n");
	}

	hr = cpInputStream->BindToFile(L"test.wav", SPFM_OPEN_READONLY, &sInputFormat.FormatId(), 
											sInputFormat.WaveFormatExPtr(), SPFEI_ALL_EVENTS);
	if( FAILED( hr ) )
	{
		printf(" Bind To File: Failed\n\n");
	}

	hr = CoCreateInstance(CLSID_SpInprocRecognizer, NULL, CLSCTX_ALL, IID_ISpRecognizer, (void **)&cpRecognizer );
	if( FAILED( hr ) )
	{
		printf(" CoCreateInstance (Recognizer): Failed\n\n");
	}

	hr = cpRecognizer->SetInput(cpInputStream, TRUE);
	if( FAILED( hr ) )
	{
		printf(" Recognizer Set Input: Failed\n\n");
	}

    hr = cpRecognizer->CreateRecoContext(&cpRecoContext);
	if( FAILED( hr ) )
	{
		printf(" Recognizer Create Context: Failed\n\n");
	}

	hr = cpRecoContext->CreateGrammar(NULL, &cpRecoGrammar);
	if( FAILED( hr ) )
	{
		printf(" Recognizer Create Grammar: Failed\n\n");
	}

	hr = cpRecoGrammar->LoadDictation(NULL,SPLO_STATIC);
	if( FAILED( hr ) )
	{
		printf(" Grammar LoadDictation: Failed\n\n");
	}

	hr = cpRecoContext->SetInterest( SPFEI_ALL_SR_EVENTS, SPFEI_ALL_SR_EVENTS);
	if( FAILED( hr ) )
	{
		printf(" Context Set Interest: Failed\n\n");
	}


	hr = cpRecoGrammar->SetDictationState(SPRS_ACTIVE);
	if( FAILED( hr ) )
	{
        printf(" Grammar Set Dictation: Failed\n\n");
	}

	BOOL fEndStreamReached = FALSE;
	SPEVENT  * temp;
	
	while (!fEndStreamReached && (S_OK == cpRecoContext->WaitForNotifyEvent(INFINITE)))
	{
		CSpEvent spEvent;

		// pull all queued events from the reco context's event queue
		while (!fEndStreamReached && S_OK == spEvent.GetFrom((cpRecoContext)))
		{
			temp = new SPEVENT;
			spEvent.CopyTo(temp);
			m_vEventList.push_back(temp);	
			// Check event type
			switch (spEvent.eEventId)
			{
				// end of the wav file was reached by the speech recognition engine
				case SPEI_END_SR_STREAM:
					//printf( "End input stream\n" );
					fEndStreamReached = TRUE;
					hr = cpRecoGrammar->SetDictationState(SPRS_INACTIVE);
					break;
				
				default:
					break;
			}
			// clear any event data/object references
		}// END event pulling loop - break on empty event queue OR end stream
	// END event polling loop - break on event timeout OR end stream
	}

	
	pVoice->Release();
	pVoice = NULL;
	CoUninitialize();
}

//-----------------------------
// Function:	Parse_XML
// Description:	Convertes a XML to Text/Phonemes
// Parameters:	.
// Returns:		.
//-----------------------------
void CSpeechDoc::Parse_XML( void )
{
	USES_CONVERSION;
	HRESULT hr = S_OK;
	CSpEvent Events;

	hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void **)&m_pVoice);
	
	hr = m_pVoice->SetInterest( SPFEI_ALL_EVENTS, SPFEI_ALL_EVENTS );
	hr = m_pVoice->Speak( A2W(m_Filename), SPF_IS_FILENAME | SPF_IS_XML, NULL );

	int c = 0;
	while( Events.GetFrom(m_pVoice) == S_OK )
	{
//		m_vEventList.push_back(temp);	
	}
    
	m_pVoice->Release();
    m_pVoice = NULL;
}

//-----------------------------
// Function:	Parse
// Description:	Convertes an Object to Text/Phonemes
// Parameters:	.
// Returns:		.
//-----------------------------
void CSpeechDoc::Parse( void )
{
//	if( m_Filename.Find(".txt") != -1 )
//		Parse_TXT();
//
	if( m_Filename.Find(".wav") != -1 )
		Parse_WAV();
//	
//	else if( m_Filename.Find(".xml") != -1 )
//		Parse_XML();
//
//	else
//		Parse_TXT();
}

//-----------------------------
// Function:	GrabEventString
// Description:	Translates Event ID into CString
// Parameters:	SPEVENTENUM EventId = ID of the eEventId
// Returns:		CString = the corresponding CString to the event
//-----------------------------
CString CSpeechDoc::GrabEventString( SPEVENTENUM EventId )
{
	switch( EventId )
	{
		case 0:
			return "UNDEFINED";

		case 1:
			return "START_INPUT_STREAM	";

		case 2:
			return "END_INPUT_STREAM";

		case 3:
			return "VOICE_CHANGE";

		case 4:
			return "SPEI_TTS_BOOKMARK";

		case 5:
			return "WORD_BOUNDARY";

		case 6:
			return "PHONEME";

		case 7:
			return "SENTENCE_BOUNDARY";

		case 8:
			return "VISEME";

		case 9:
			return "TTS_AUDIO_LEVEL";

		case 15:
			return "TTS_PRIVATE";

		case 30:
			return "RESERVED1";

		case 33:
			return "RESERVED2";

		case 34:
			return "END_SR_STREAM";

		case 35:
			return "SOUND_START";

		case 36:
			return "SOUND_END";

		case 37:
			return "PHRASE_START";

		case 38:
			return "RECOGNITION";

		case 39:
			return "HYPOTHESIS";

		case 40:
			return "SR_BOOKMARK";

		case 41:
			return "PROPERTY_NUM_CHANGE";

		case 42:
			return "PROPERTY_STRING_CHANGE";

		case 43:
			return "FALSE_RECOGNITION";

		case 44:
			return "INTERFERENCE";

		case 45:
			return "REQUEST_UI";

		case 46:
			return "RECO_STATE_CHANGE";

		case 47:
			return "ADAPTATION";

		case 48:
			return "START_SR_STREAM";

		case 49:
			return "RECO_OTHER_CONTEXT";

		case 50:
			return "SR_AUDIO_LEVEL";

		case 52:
			return "SR_PRIVATE";

		case 63:
			return "RESERVED3";

       default:
			return "UNDEFINED";
	}
}

//-----------------------------
// Function:	GrabVisemeString
// Description:	Translates Viseme ID into CString
// Parameters:	SPVISEMES VisemeId = ID of the Viseme
// Returns:		CString = the corresponding CString to the event
//-----------------------------
CString CSpeechDoc::GrabVisemeString( SPVISEMES VisemeId )
{
	switch( VisemeId )
	{
		case SP_VISEME_0:
			return " 0: silence";
			break;

		case SP_VISEME_1:
			return " 1: ae, ax, ah";
			break; 

		case SP_VISEME_2:
			return " 2: aa";
			break;

		case SP_VISEME_3:
			return " 3: ao";
			break;

		case SP_VISEME_4:
			return " 4: ey, eh, uh";
			break;

		case SP_VISEME_5:
			return " 5: er";
			break;

		case SP_VISEME_6:
			return " 6: y, iy, ih, ix";
			break;

		case SP_VISEME_7:
			return " 7: w, uw";
			break;

		case SP_VISEME_8:
			return " 8: ow";
			break;

		case SP_VISEME_9:
			return " 9: aw";
			break;

		case SP_VISEME_10:
			return "10: oy";
			break;

		case SP_VISEME_11:
			return "11: ay";
			break;

		case SP_VISEME_12:
			return "12: h";
			break;

		case SP_VISEME_13:
			return "13: r";
			break;

		case SP_VISEME_14:
			return "14: l";
			break;

		case SP_VISEME_15:
			return "15: s, z";
			break;

		case SP_VISEME_16:
			return "16: sh, ch, jh, zh";
			break;

		case SP_VISEME_17:
			return "17: th, dh";
			break;

		case SP_VISEME_18:
			return "18: f, v";
			break;
		
		case SP_VISEME_19:
			return "19: d, t, n";
			break;

		case SP_VISEME_20:
			return "20: k, g, ng";
			break;

		case SP_VISEME_21:
			return "21: p, b, m";
			break;

		default:
			return "UNDEFINED";		// should never get here
	}
}

void CSpeechDoc::OnSpeechTest1()
{
	MessageBox(NULL,_T("Test Menu Item 1 was selected in CSpeechDoc.\n"), _T("Speech"), MB_OK);	

}

void CSpeechDoc::OnUpdateSpeechTest1(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}

void CSpeechDoc::OnSpeechTest2()
{
	// TODO: Add your command handler code here
}

void CSpeechDoc::OnUpdateSpeechTest2(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}

void CSpeechDoc::OnSpeechTest3()
{
	// TODO: Add your command handler code here
}

void CSpeechDoc::OnUpdateSpeechTest3(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}

//-----------------------------
// Function:	BuildVisemeFile
// Description:	Builds a File Containing all the Visemes 
// Parameters:	.
// Returns:		.
//-----------------------------
void CSpeechDoc::BuildVisemeFile( void )
{
	HANDLE hFile;
	CString Buffer = "\0";
	CString Temp = "\0";
	BOOL Check;
	DWORD i;
	int Time;
	DWORD dwNumWritten;

	CString Filename = m_Filename;
	Filename.Replace(".wav", ".vsf");

	hFile = CreateFile( Filename, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS,
					    FILE_ATTRIBUTE_NORMAL, NULL );

	Buffer = m_Filename;
	Buffer += "\n";
	Check = WriteFile(hFile, Buffer, Buffer.GetLength()*sizeof(char), &dwNumWritten, NULL);
	Buffer.Empty(); 

	for( i = 0; i < m_vEventList.size(); i++ )
	{
		Buffer = GrabEventString(m_vEventList[i]->eEventId);
		Buffer += ": ";
		Buffer += GrabVisemeString((SPVISEMES)LOWORD(m_vEventList[i]->lParam));
		Buffer += "\n";
		
		Buffer += "   Duration: ";
		Time = HIWORD(m_vEventList[i]->wParam);
		Temp.Format("%i", Time);
		Buffer += Temp;
		Buffer += "ms\n";

		Buffer += "   Stressed: ";
		if( (SPVFEATURE)HIWORD(m_vEventList[i]->lParam) >= SPVFEATURE_STRESSED )
			Buffer += "true\n";
		else
			Buffer += "false\n";

		Buffer += "   Emphasize: ";
		if( (SPVFEATURE)HIWORD(m_vEventList[i]->lParam) >= SPVFEATURE_EMPHASIS )
			Buffer += "true\n";
		else
			Buffer += "false\n";

		Buffer += "\n";		
		Check = WriteFile(hFile, Buffer, Buffer.GetLength()*sizeof(char), &dwNumWritten, NULL);
		Buffer.Empty();
		Temp.Empty();
	}

	Check = CloseHandle(hFile);
}
