///============================================================================
/// \file		BrushObject.cpp
/// \brief		Implementation of the CBrushObject Class
/// \date		07-07-2005
/// \author		Patrick Ghiocel
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

CBrushObject::CBrushObject( StdString wszName, StdString wszFileName, StdString wszOperator ) 
{
	m_pData = NULL;
	m_pBitmap = NULL;
	m_wszName = wszName;
	m_wszFileName = wszFileName;
	m_wszOperator = wszOperator;

	ITextureObject *TextureObjectInterface = LoadTexture( m_wszFileName );

	int iBrushSize = 32;
	void *pTextureData = NULL;
	int iColorDepth = 8;

	if (TextureObjectInterface)
	{
		// brush texture must be square
		if (TextureObjectInterface->GetHeight() == TextureObjectInterface->GetWidth())
		{
			iColorDepth = TextureObjectInterface->GetColorDepth();
			iBrushSize = TextureObjectInterface->GetWidth();
			pTextureData = TextureObjectInterface->Read();
		}
		else 
		{
			EngineGetToolBox()->Log( LOGWARNING, _T("%s(%d): Brush '%s' is not a square!\n"), __FILE__, __LINE__ , m_wszFileName );
		}	
	}

	int iDataSize = iBrushSize * iBrushSize;

	/////////////////////////////////////////////////////
	//Create Bitmap to show on GUI

	if ( TextureObjectInterface != NULL )
	{
		// the incomming sizes
		UINT width = TextureObjectInterface->GetWidth();
		UINT height = TextureObjectInterface->GetHeight();

		// this will be the final sizes
		UINT targetWidth = 20;
		UINT targetHeight = 20;

		// these will be about 1.6 (32/20) -> we go thru the target pixels, and find the 
		// matching source pixels to them.
		double scaleWidth = (double)width / (double)targetWidth;
		double scaleHeight = (double)height / (double)targetHeight;

		// the source buffer
		void* buffer;
		buffer = TextureObjectInterface->Read();

		// the target buffer
		size_t bufSize = targetWidth * targetHeight * 4;
		auto_ptr<UCHAR> tempBuffer(new UCHAR[bufSize]);

		// just to have a pointer of an other type to these buffers
		UCHAR* memPointer = (UCHAR*) tempBuffer.get();
		UCHAR* sourcePointer = (UCHAR*) buffer;

		// modify temp buffer to have RGB values 
		for ( unsigned int rowIterator = 0; rowIterator < targetWidth; rowIterator++)
		{
			for ( unsigned int columnIterator = 0; columnIterator < targetWidth; columnIterator++)
			{
				// this iterates thru all the pixels of the target bitmap
				UINT currentPixelOffset = ( rowIterator * targetWidth ) + columnIterator;

				// this point to the appropriate souce pixel to get the color from
				//UINT sourcePixelOffset = (UINT)floor( ( (double)rowIterator * (double)targetWidth * scaleWidth ) ) + (UINT)floor( (double)columnIterator  * scaleHeight );
				UINT sourcePixelOffset = (UINT)floor( ( (double)rowIterator * scaleWidth ) ) * width + (UINT)floor( (double)columnIterator  * scaleHeight );

				// this is the alpha value of the souce pixel, use that as target color for every target channel
				// pixel pointer is scaled; +3 will point to alpha channel???
				UCHAR sourceColor = *( sourcePointer + 4 * sourcePixelOffset + 3 );
				
				// we need to invert the colors, as black means transparent in the alpha
				// channel and we need white as background/transparent color
				// '/2' to make it lighter, it was too dark earlier
				UCHAR invetedSourceColor = 255 - sourceColor / 2;
				
				// copy source Alpha value to target BGR
				// lets go thru the 4 channels
				*( memPointer + 4 * currentPixelOffset ) = invetedSourceColor;    //BLUE
				*( memPointer + 4 * currentPixelOffset + 1) = invetedSourceColor;     //GREEN
				*( memPointer + 4 * currentPixelOffset + 2) = invetedSourceColor; //RED
				*( memPointer + 4 * currentPixelOffset + 3) = 0xFF;	//ALPHA
			}
		}

		// lets create a device independent image from our buffer data
		BITMAPINFO bmpi = {0};
		BYTE *pBits = NULL;

		// init the DIB info header
		bmpi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmpi.bmiHeader.biWidth = targetWidth;
		bmpi.bmiHeader.biHeight = targetHeight;
		bmpi.bmiHeader.biPlanes = 1;
		bmpi.bmiHeader.biBitCount = 32;			//we need an extra alpha channel (32bit = 4bytes)
		bmpi.bmiHeader.biCompression = BI_RGB;

		// this will be the current windows desktop device
		HDC myDC = CreateCompatibleDC(GetDC(NULL));

		// handle to our device independent image
		HBITMAP hBmp = CreateDIBSection(myDC, &bmpi, DIB_RGB_COLORS, (void**)&pBits, NULL, 0);
		
		//This will be the permanent bitmap of the brush. create it on the heap.
		m_pBitmap = new CBitmap();

		//Init our brush bitmap with the device independent bitmap handle.
		m_pBitmap->Attach( hBmp );

		//BUGGY WORKING VERSION: Fromhandle returns only a temporary object.
		//CBitmap* currentBitmapRGBA = CBitmap::FromHandle( hBmp ); 

		if ( m_pBitmap != NULL )
		{
			// Set our data in memory as bitmap of our CBitmap object. 
			UINT bytesWritten =	m_pBitmap->SetBitmapBits( bufSize , memPointer );

			// error checking 
			if ( bytesWritten == 0 )
			{
				EngineGetToolBox()->Log( LOGWARNING, _T("%s(%d): Error creating brush bitmap.\n"), __FILE__, __LINE__ );
			} 
		} 
		else
		{	
			EngineGetToolBox()->Log( LOGWARNING, _T("%s(%d): Cann't create device dependent bitmap for brush.\n"), __FILE__, __LINE__ );
		}
	}

	// create float data to give to EE
	m_iSize = iBrushSize;
	int iSkip = iColorDepth / 8;
	m_pData = new float[iDataSize];
	if (pTextureData)
	{
		if (iColorDepth == 32) // use alpha channel
		{
			for (int i=0; i < iDataSize; i++)
				m_pData[i] = (float)((unsigned char*)pTextureData)[i*4+3] / 255.0f;
		}
		else
		{
			for (int i=0; i < iDataSize; i++)
				m_pData[i] = (float)((unsigned char*)pTextureData)[i*iSkip] / 255.0f;
		}
	}
	else
	{
		for (int i=0; i < iDataSize; i++)
			m_pData[i] = 0.0f;
	}
}

CBrushObject::~CBrushObject()
{
}

void CBrushObject::Destroy()
{
	if( m_pData != NULL )
	{
		delete[] m_pData;
	}
	m_pData = NULL;

	if( m_pBitmap != NULL )
	{
		delete m_pBitmap;
	}
	m_pBitmap = NULL;
}

ITextureObject* CBrushObject::LoadTexture( LPCTSTR path )
{
	LOADFILEEXTPARAMS lfep;
	CHashString filename(path);
	lfep.fileName = (LPTSTR)filename.GetString();
	lfep.bInternalLoad = true;
	static DWORD msgHash_LoadFileByExtension = CHashString(_T("LoadFileByExtension")).GetUniqueID();
	if (EngineGetToolBox()->SendMessage(msgHash_LoadFileByExtension, sizeof(lfep), &lfep) == MSG_HANDLED)
	{
		TEXTUREOBJECTPARAMS top;
		top.Name = &filename;
		static DWORD msgHash_GetTexture = CHashString(_T("GetTexture")).GetUniqueID();
		if (EngineGetToolBox()->SendMessage(msgHash_GetTexture, sizeof(top), &top) == MSG_HANDLED)
		{
			return dynamic_cast<ITextureObject*>(top.TextureObjectInterface);
		}
	}
	return NULL;
}
