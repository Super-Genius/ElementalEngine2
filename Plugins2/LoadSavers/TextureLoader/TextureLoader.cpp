///============================================================================
/// \file		TextureLoader.cpp
/// \brief		Implementation file for texture Loader
/// \date		05-14-2004
/// \author		Kenneth Hurley
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

REGISTER_COMPONENT_SINGLETON(CTextureLoader);
REGISTER_MESSAGE_HANDLER(LoadFile, OnLoadTextureFile, CTextureLoader);
REGISTER_MESSAGE_HANDLER(SaveFile, OnSaveTextureFile, CTextureLoader);
REGISTER_MESSAGE_HANDLER(UnloadFile, OnUnloadTextureFile, CTextureLoader);
REGISTER_MESSAGE_HANDLER(SetSaveAsFormat, OnSetSaveAsFormat, CTextureLoader);

CHashString CTextureLoader::m_HashName(_T("CTextureLoader"));

LSRC_MESSAGE CTextureLoader::m_LoadSaveRegisterData = {
		&CTextureLoader::m_HashName,
#if 0	// ONCE THE LOADER IS DONE.
		"DevIL Files (*.oil)\0" "*.oil\0"
		"Graphics Interchange Format (*.gif)\0" "*.gif\0"
		"Half-Life Model Files (*.mdl)\0" "*.mdl\0"
		"Homeworld Image Files (*.lif)\0" "*.lif\0"
#endif
		"Jpeg Files (*.jpe, *.jpg, *.jpeg)\0" "*.jpe;*.jpg;*.jpeg\0"
#if 0
		"Microsoft Bitmap Files (*.bmp)\0" "*.bmp\0"
		"Microsoft Icon Files (*.ico)\0" "*.ico\0"
		"Portable AnyMap Files (*.pbm, *.pgm, *.pnm, *.ppm)\0" "*.pbm;*.pgm;*.pnm;*.ppm\0"
		"Portable Network Graphics Files (*.png)\0" "*.png\0"
		"Sgi Files (*.sgi)\0" "*.bw;*.rgb;*.rgba;*.sgi\0"
#endif
		"Targa Files (*.tga)\0" "*.tga\0"
		"DirectX Image Files (*.dds)\0" "*.dds\0"
#if 0
		"Tiff Files (*.tif)\0" "*.tif;*.tiff\0"
		"Quake Wal Files (*.wal)\0" "*.wal\0"
		"ZSoft Pcx Files (*.pcx)\0" "*.pcx\0"
#endif
		"\0\0",	
		// save types
		"C-Style Header (*.h)\0" "*.h\0"
		"DevIL Files (*.oil)\0" "*.oil\0"
		"Direct X Files (*.dds)\0" "*.dds\0"
		"Jpeg Files (*.jpe, *.jpg, *.jpeg)\0" "*.jpe;*.jpg;*.jpeg\0"
		"Microsoft Bitmap Files (*.bmp)\0" "*.bmp\0"
		"Portable AnyMap Files (*.pbm, *.pgm, *.ppm)\0" "*.pbm;*.pgm;*.ppm\0"
		"Portable Network Graphics Files (*.png)\0" "*.png\0"
		"Sgi Files (*.sgi)\0" "*.bw;*.rgb;*.rgba;*.sgi\0"
		"Targa Files (*.tga)\0" "*.tga\0"
		"ZSoft Pcx Files (*.pcx)\0" "*.pcx\0"
		"\0\0",
		true, NULL,
};

CTextureLoader::CTextureLoader() 
{
	// save for convenience
	m_ToolBox = EngineGetToolBox();
	// Initialze the image library
	ilInit();
	iluInit();
	ilutInit();
	static DWORD msgHash_RegisterLoadSaveComponent = CHashString(_T("RegisterLoadSaveComponent")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_RegisterLoadSaveComponent, sizeof(LSRC_MESSAGE), &m_LoadSaveRegisterData);

	m_TextureMap.clear();
}

CTextureLoader::~CTextureLoader()
{
	ilShutDown();
}

// create function for this world loader
IComponent *CTextureLoader::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE( CTextureLoader );
}

IHashString *CTextureLoader::GetComponentType()
{
	return &m_HashName;
}

bool CTextureLoader::IsKindOf(IHashString *compType)
{
	return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
}

bool IsTextureCompressed( ILinfo &info, CHashString &outformat, ILenum &format )
{
	format = ilGetInteger( IL_DXTC_DATA_FORMAT );
	switch( format )
	{
	case IL_DXT1:
		outformat.Init( "DXT1");
		return true;
	case IL_DXT2:
		outformat.Init( "DXT2");
		return true;
	case IL_DXT3:
		outformat.Init( "DXT3");
		return true;
	case IL_DXT4:
		outformat.Init( "DXT4");
		return true;
	case IL_DXT5:
		outformat.Init( "DXT5");
		return true;
	case IL_DXT_NO_COMP:
		break;
	};
	return false;
}

ILenum CTextureLoader::GetILFormat( IHashString * hash )
{
	if( hash )
	{
		static CHashString dxt5(_T("DXT5"));
		static CHashString dxt3(_T("DXT3"));
		static CHashString dxt1(_T("DXT1"));
		DWORD dHash = hash->GetUniqueID();
		if( dHash == dxt5.GetUniqueID() )
		{
			ilSetInteger( IL_DXTC_FORMAT, IL_DXT5 );
			return IL_DXT5;
		}
		else if( dHash == dxt3.GetUniqueID() )
		{
			ilSetInteger( IL_DXTC_FORMAT, IL_DXT3 );
			return IL_DXT3;
		}
		else if( dHash == dxt1.GetUniqueID() )
		{
			ilSetInteger( IL_DXTC_FORMAT, IL_DXT1 );
			return IL_DXT1;
		}
	}
	return IL_RGBA;
}
	
DWORD CTextureLoader::OnSetSaveAsFormat( DWORD size, void * params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof( IHashString ) );
	IHashString * hash = (IHashString*)params;
	if( hash )
	{
		if( _tcslen( hash->GetString() ) > 0 )
		{
			m_SaveAsFormat.Init( hash->GetString() );
		}
	}
	return MSG_HANDLED_PROCEED;
}

// load a file if we recognize the type.
DWORD CTextureLoader::OnLoadTextureFile(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(TCHAR *));
	TCHAR *pFileName = (TCHAR *)params;

	StdString szFileName(pFileName);
	szFileName.MakeSafeFileName();
	CHashString hszFileName( szFileName );

	// if texture is already loaded, increment it's reference count and return
	TEXTUREOBJECTPARAMS top;
	top.Name = &hszFileName;
	static DWORD msgHash_GetTexture = CHashString(_T("GetTexture")).GetUniqueID();
	if (EngineGetToolBox()->SendMessage(msgHash_GetTexture, sizeof(top), &top ) == MSG_HANDLED)
	{
		IBaseTextureObject *pTexture = top.TextureObjectInterface;
		if (pTexture && pTexture->IsDataLoaded())
		{
			pTexture->IncrementRefCount();
			return MSG_HANDLED_STOP;
		}
	}

	DWORD msgRetVal = MSG_HANDLED_STOP;
	ILboolean result;
	ILuint	 theImage;
	ILubyte* theImageData = NULL;
	ILinfo  theImageInfo;	
	if( _tcsstr( szFileName, ".tga" ) )
	{
		ilEnable( IL_ORIGIN_SET );
		ilRegisterOrigin( IL_ORIGIN_UPPER_LEFT  );
		ilOriginFunc( IL_ORIGIN_UPPER_LEFT );
	}
	ilSetInteger( IL_KEEP_DXTC_DATA, IL_TRUE );
	ilSetInteger( IL_ONLY_DXTC_DATA, IL_TRUE );//don't keep uncompressed format
	ilGenImages(1, &theImage);
	ilBindImage(theImage);

	result = ilLoadImage( (const ILstring)szFileName.c_str() );
	if( !result )
	{
		return MSG_ERROR;
	}	
	iluGetImageInfo(&theImageInfo);

	BYTE *pImageData = NULL;

	// create the texture
	CREATETEXTUREPARAMS ctp;
	CHashString hformat;
	bool bCompressedTexture = false;
	ILenum ddsformat = 0;
	//-1 because I think this is returning numlevels sometimes
	int numMips = ilGetInteger( IL_NUM_MIPMAPS );
	if( numMips < 0 ){numMips = 0;}
	if( _tcsstr( szFileName, ".dds" ) )
	{
		bCompressedTexture = IsTextureCompressed( theImageInfo, hformat, ddsformat );
		if( bCompressedTexture )
		{
			ctp.Format = &hformat;		
		}
	}

	static CHashString hszFormatA32B32G32R32F(_T("A32B32G32R32F"));

	ctp.Name = &hszFileName;
	ctp.sizeX = theImageInfo.Width;
	ctp.sizeY = theImageInfo.Height;
	ctp.TextureObjectInterface = NULL;
	ctp.numMips = numMips;
	if (theImageInfo.Type == IL_FLOAT && theImageInfo.Bpp == 4)
	{
		ctp.Format = &hszFormatA32B32G32R32F;
		ctp.bitDepth = 128;
	}
	else
	{
		ctp.bitDepth = theImageInfo.Bpp * 8;
	}

	static DWORD msgHash_CreateTexture = CHashString(_T("CreateTexture")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_CreateTexture, sizeof(ctp), &ctp );

	// write the image data to the texture
	ITextureObject *pTexture = dynamic_cast<ITextureObject*>( ctp.TextureObjectInterface );

	if (pTexture)
	{
		//Save to the map of Textures for unloading
		m_TextureMap[hszFileName.GetUniqueID()] = pTexture->GetName()->GetUniqueID();
		pTexture->SetTextureName( &hszFileName );

		if( bCompressedTexture )
		{
			int bsize = ilGetDXTCData( NULL, 0, ddsformat );
			if( bsize > 0 )
			{
				
				ilSetInteger( IL_ACCESS_DXTC_INTERNAL , IL_TRUE );
				ilGetDXTCData( &pImageData, bsize, ddsformat );
				ilSetInteger( IL_ACCESS_DXTC_INTERNAL , IL_FALSE );
				theImageData = pImageData;
			}else
			{
				//no more mip maps possible, dxt stores textures in at least 4x4 size
				//any further and it can't use them
				msgRetVal = MSG_ERROR;
			}
		}
		else if (theImageInfo.Bpp != 1 && theImageInfo.Type != IL_FLOAT)
		{
			// we need to format the data into something the renderer can use
			SAFE_DELETE_ARRAY( pImageData );
			pImageData = new BYTE[theImageInfo.Width*theImageInfo.Height*4];

			ILenum format;
			format = IL_BGRA;

			ilCopyPixels( 0,0,0, theImageInfo.Width, theImageInfo.Height, 1, format, IL_UNSIGNED_BYTE, pImageData );
			theImageInfo.Bpp = 4;
			theImageData = pImageData;
		}
		else
		{
			// we can use the data directly
			theImageData = ilGetData();
		}
		if (msgRetVal != MSG_ERROR &&
			!pTexture->Write( theImageData, 0 , ctp.Format))
		{
			msgRetVal = MSG_ERROR;
		}

		//Check for mipmaps, if no mipmaps, just do top level surface
		if( numMips != 0 ) //Has mipmaps
		{
			for( int miplevel = 1; miplevel < numMips; miplevel++ )
			{
				ilBindImage( theImage );
				//Bind each mip level and save them to the texture
				if( ilActiveMipmap( miplevel ) != IL_FALSE )
				{
					iluGetImageInfo(&theImageInfo);
					if( bCompressedTexture )
					{
						// DXT does not support mips with a dimension less than it's encoding block size of 4x4
						if (theImageInfo.Width < 4 || theImageInfo.Height < 4)
							continue;

						int bsize = ilGetDXTCData( NULL, 0, ddsformat );
						
						if( bsize > 0 )
						{
							ilSetInteger( IL_ACCESS_DXTC_INTERNAL , IL_TRUE );
							ilGetDXTCData( &pImageData, bsize, ddsformat );
							ilSetInteger( IL_ACCESS_DXTC_INTERNAL , IL_FALSE );
							theImageData = pImageData;
						}else
						{
							//no more mip maps possible, dxt stores textures in at least 4x4 size
							//any further and it can't use them
							break;
						}
					}
					else 
						if (theImageInfo.Bpp != 1 && theImageInfo.Type != IL_FLOAT)
					{
						// we need to format the data into something the renderer can use
						SAFE_DELETE_ARRAY( pImageData );
						pImageData = new BYTE[theImageInfo.Width*theImageInfo.Height*4];

						ILenum format;
						format = IL_BGRA;

						ilCopyPixels( 0,0,0, theImageInfo.Width, theImageInfo.Height, 1, format, IL_UNSIGNED_BYTE, pImageData );
						theImageInfo.Bpp = 4;
						theImageData = pImageData;
					}
					else
					{
						// we can use the data directly
						theImageData = ilGetData();
					}
					if(!pTexture->Write( theImageData, miplevel, ctp.Format ))
					{
						msgRetVal = MSG_ERROR;
						break;
					}
				}
			}
		}
	}
	else
		msgRetVal = MSG_ERROR;

	ilDeleteImages(1, &theImage);
	if( !bCompressedTexture )
	{
		SAFE_DELETE_ARRAY( pImageData );
	}

	return msgRetVal;
}

// Save a file if we recognize the type.
DWORD CTextureLoader::OnSaveTextureFile(DWORD size, void *params)
{
	// Check the message to verify the size
	VERIFY_MESSAGE_SIZE(size, sizeof(TCHAR *));
	const TCHAR *pFileName = (const TCHAR *)params;

	StdString szFileName(pFileName);
	szFileName.MakeSafeFileName();
	
	CHashString fileName(szFileName);
	ITextureObject *texObj;
	TEXTUREOBJECTPARAMS top;
	DWORD retVal;
	ILuint	theImage;

	top.Name = &fileName;
	static DWORD msgHash_GetTexture = CHashString(_T("GetTexture")).GetUniqueID();
	retVal = m_ToolBox->SendMessage(msgHash_GetTexture, sizeof(TEXTUREOBJECTPARAMS), &top);
	if (retVal != MSG_HANDLED)
	{
		m_ToolBox->SetErrorValue(ERR_NAME_NOT_FOUND);
		m_ToolBox->Log(LOGERROR, _T("Cannot Find Texture Named %s\n"), fileName);
		return MSG_ERROR;
	}

	texObj = dynamic_cast<ITextureObject*>( top.TextureObjectInterface );

	/// hmmm, the texture wasn't found?
	if (texObj == NULL)
	{
		m_ToolBox->SetErrorValue(ERR_NULL_POINTER);
		m_ToolBox->Log(LOGERROR, _T("Cannot Find Texture Interface for %s\n"), fileName);
		return MSG_ERROR;
	}

	ILenum format;	
	bool isTarga =  ( _tcsstr( szFileName, ".tga" ) != NULL );

	switch (texObj->GetColorDepth())
	{
		// could be paletted?
		case 8:
			format = IL_LUMINANCE;
			break;
		case 24:
			if( !isTarga )
			{
				format = IL_RGB;
			}else
			{
				format = IL_BGR;
			}
			break;
		case 32:
			if( !isTarga )
			{
				format = IL_RGBA;
			}else
			{
				format = IL_BGRA;
			}
			break;
	}
	// allow for overwrites of file
	ilEnable(IL_FILE_OVERWRITE);
	ilGenImages(1, &theImage);
	ilBindImage(theImage);
	

	ilTexImage(texObj->GetWidth(), texObj->GetHeight(), 
			1, texObj->GetColorDepth()>>3, format, IL_UNSIGNED_BYTE,
			texObj->Read());
	
	if( isTarga )
	{
		ilEnable( IL_ORIGIN_SET );
		ilRegisterOrigin( IL_ORIGIN_UPPER_LEFT  );
		ilOriginFunc( IL_ORIGIN_UPPER_LEFT );
	}else
	{
		ilEnable( IL_ORIGIN_SET );
		ilRegisterOrigin( IL_ORIGIN_LOWER_LEFT  );
		ilOriginFunc( IL_ORIGIN_LOWER_LEFT );
	}
	
	//custom format	
	const TCHAR * customFormat = m_SaveAsFormat.GetString();
	if( customFormat &&
		_tcslen( customFormat ) > 0 )
	{
		GetILFormat( &m_SaveAsFormat );
		//not sure which one is default dds
		ilEnable( IL_ORIGIN_SET );
		ilRegisterOrigin( IL_ORIGIN_UPPER_LEFT  );
		ilOriginFunc( IL_ORIGIN_UPPER_LEFT );
		
	}
	if (!ilSaveImage((const ILstring)szFileName.c_str()))
	{
		return MSG_ERROR;
	}

	ilDeleteImages(1, &theImage);
	m_SaveAsFormat.Init( "" );

	return MSG_HANDLED_STOP;
}

DWORD CTextureLoader::OnUnloadTextureFile(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(TCHAR *));
	CHashString fileName((const TCHAR *)params);
	DWORD result;

	TEXTURETOOBJECTMAP::iterator itr = m_TextureMap.find( fileName.GetUniqueID() );

	if( itr != m_TextureMap.end() )
	{
		CHashString tmpObjectName( m_ToolBox->GetHashString( itr->second ) );
		UNLOADOBJECTPARAMS uop;
		uop.name = &tmpObjectName;
		static DWORD msgHash_UnloadObject = CHashString(_T("UnloadObject")).GetUniqueID();
		result = m_ToolBox->SendMessage(msgHash_UnloadObject, sizeof(uop), &uop );
		m_TextureMap.erase( itr );
		return result;
	}
	else
	{
		return MSG_ERROR;
	}
}
