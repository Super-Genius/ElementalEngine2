#include "stdafx.h"
#include "CWaveFile.h"

CWaveFile::CWaveFile( )
{
	m_bWaveLoaded = false;
}



CWaveFile::CWaveFile( LPCTSTR fileName ) 
{
	if( !Open( fileName, FALSE) ) {
		throw WaveErrors::FileOperation();
	}
	m_bWaveLoaded = true;
	//Get base address of mapped file
	dataAddress = Base();
	ReadWave();
	
}

void CWaveFile::Load( LPCTSTR fileName )
{
	if( !Open( fileName, FALSE) ) 
	{
		throw WaveErrors::FileOperation();
	}
	m_bWaveLoaded = true;

	dataAddress = Base();
	ReadWave();
}


void CWaveFile::ReadWave()
{
	ReadRIFF();
	//Move to next block
        dataAddress += sizeof( *pRIFF );
	ReadFMT();
	//Move to next block
	dataAddress += sizeof( *pFMT );
	ReadDATA();
	dataAddress += sizeof( *pDATA );
	//Wave have beed read!
}

inline void CWaveFile::ReadRIFF()
{
        pRIFF = reinterpret_cast< RIFF* >( dataAddress );
        if( !CheckID( pRIFF->riffID, 'R', 'I', 'F', 'F') ) {
		throw WaveErrors::RiffDoesntMatch();
        }
	if( !CheckID( pRIFF->riffFORMAT, 'W', 'A', 'V', 'E') ) {
		throw WaveErrors::WaveDoesntMatch();
	}
}

inline void CWaveFile::ReadFMT()
{
	pFMT = reinterpret_cast< FMT* >( dataAddress );
	if( !CheckID( pFMT->fmtID, 'f', 'm', 't', ' ') ) {
		throw WaveErrors::FmtDoesntMatch();
	}
}

inline void CWaveFile::ReadDATA()
{
	try {

		pDATA = reinterpret_cast< DATA* >( dataAddress );
		if( !CheckID( pDATA->dataID, 'd', 'a', 't', 'a') ) {
			throw WaveErrors::DataDoesntMatch();
		}

	}catch( WaveErrors::DataDoesntMatch & ) {
		//something stange! In Microsoft wave files DATA identifier 
		//can be offseted (maybe because of address aligment)
		//Start to looking DATA_ID "manualy" ;)
		PBYTE b = Base();
		BOOL foundData = FALSE;
		while(  (dataAddress - b) !=  dwSize ) {
			if( *dataAddress == 'd' ) {
				//It can be DATA_ID, check it!
				pDATA = reinterpret_cast< DATA * >( dataAddress );
				if( CheckID( pDATA->dataID, 'd','a','t','a' ) ) {
					//DATA_ID was founded
					foundData = TRUE;
					break;
				}
			}
			dataAddress++;
		}
		if( !foundData ) {
			//This file maybe corrupted
			throw WaveErrors::DataDoesntMatch();
		}
	}
}

BOOL CWaveFile::DrawData(CDC *pDC, CRect *pRect )
{
	pDC->SaveDC();
	CPen pen(PS_SOLID, 1, RGB(255,255,255) );
	CSize * pNewSize = &pRect->Size();

	pDC->SelectObject(&pen);
	//CBrush brush( RGB( 150, 200, 255 ) );
	//FillRect( *pDC, pRect, brush );

	pDC->SetMapMode(MM_ANISOTROPIC);
	
	pDC->SetWindowExt( 
		pFMT->fmtFORMAT.wBitsPerSample == 8 ? pDATA->dataSIZE : pDATA->dataSIZE / 2, 
		pFMT->fmtFORMAT.wBitsPerSample == 8 ? 300 : 80000);
	
	pDC->SetViewportOrg( pNewSize->cx - pRect->right, pNewSize->cy/2);
	pDC->SetViewportExt( pNewSize->cx, pNewSize->cy );
	pDC->MoveTo( pRect->left, pRect->bottom/2 );

	if( pFMT->fmtFORMAT.wBitsPerSample == 8 ) 
		DrawByte( pDC );
	else
		DrawWord( pDC );

	pDC->RestoreDC(-1);
	return TRUE;
}

void CWaveFile::DrawByte( CDC *pDC )
{
	const AudioByte *buffer = reinterpret_cast< AudioByte* >( dataAddress  );
	for( DWORD p = 0; p < pDATA->dataSIZE; p++ ) {
		pDC->MoveTo(p , 0 );
		pDC->LineTo(p,buffer[ p ] - 128);
	}
}

void CWaveFile::DrawWord( CDC *pDC )
{
	const AudioWord *buffer = reinterpret_cast< AudioWord* >( dataAddress  );
	for( DWORD p = 0; p < pDATA->dataSIZE / 2; p++ ) {
		pDC->MoveTo(p,  0);
		pDC->LineTo(p, buffer[ p ] );
	}
}
//Checking identifiers of headers  ( FMT, DATA, WAVE )
inline BOOL CWaveFile::CheckID(_TCHAR *idPar, _TCHAR A, _TCHAR B, _TCHAR C, _TCHAR D)
{
	return(
		(idPar[0] == A) &&
		(idPar[1] == B) &&
		(idPar[2] == C) &&
		(idPar[3] == D)
		);
}

DWORD CWaveFile::GetDataSize( void )
{
	return pDATA->dataSIZE;
}

