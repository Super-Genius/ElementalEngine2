///============================================================================
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

#include "Stdafx.h"
#include "CDX9Font.h"
#include "PerformanceProfiler.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define TEXTURESIZE 512 //square
#define FONT_BITDEPTH 32
#define UV_BUFFER 3

// this freetype stuff is being moved from within CDX9Font so it can be called at the right time by CFontManager
void InitFontImplementation()
{
	int error = FT_Init_FreeType( &EE_CDX9Font::m_FontLibrary );
}

void DeInitFontImplementation()
{
	FACEMAP::iterator iter = EE_CDX9Font::m_NormFaceMap.begin();
	while (iter != EE_CDX9Font::m_NormFaceMap.end())
	{
		FT_Done_Face( iter->second.m_FTFace );
		iter->second.m_FTFace = NULL;
		iter++;
	}
	EE_CDX9Font::m_NormFaceMap.clear();

	iter = EE_CDX9Font::m_BoldFaceMap.begin();
	while (iter != EE_CDX9Font::m_BoldFaceMap.end())
	{
		FT_Done_Face( iter->second.m_FTFace );
		iter->second.m_FTFace = NULL;
		iter++;
	}
	EE_CDX9Font::m_BoldFaceMap.clear();

	iter = EE_CDX9Font::m_ItalicFaceMap.begin();
	while (iter != EE_CDX9Font::m_ItalicFaceMap.end())
	{
		FT_Done_Face( iter->second.m_FTFace );
		iter->second.m_FTFace = NULL;
		iter++;
	}
	EE_CDX9Font::m_ItalicFaceMap.clear();

	iter = EE_CDX9Font::m_BoldItalicFaceMap.begin();
	while (iter != EE_CDX9Font::m_BoldItalicFaceMap.end())
	{
		FT_Done_Face( iter->second.m_FTFace );
		iter->second.m_FTFace = NULL;
		iter++;
	}
	EE_CDX9Font::m_BoldItalicFaceMap.clear();

	FT_Done_FreeType( EE_CDX9Font::m_FontLibrary );
	EE_CDX9Font::m_FontLibrary = NULL;
}

//statics might need to move to a manager?
FT_Library						EE_CDX9Font::m_FontLibrary;
FACEMAP							EE_CDX9Font::m_NormFaceMap;
FACEMAP							EE_CDX9Font::m_BoldFaceMap;
FACEMAP							EE_CDX9Font::m_ItalicFaceMap;
FACEMAP							EE_CDX9Font::m_BoldItalicFaceMap;
IMAGEMAP						EE_CDX9Font::m_ImageMap;

void DrawToTexture( ITextureObject * tex,
				    FT_Bitmap *bitmap,
					int x,
					int y )
{
	INT pitch;
	unsigned char * pBits = (unsigned char *)tex->Lock( &pitch );
	if( pBits &&
		bitmap &&
		bitmap->pixel_mode == FT_PIXEL_MODE_GRAY )
	{
        unsigned char * pSrc = bitmap->buffer;
		INT srcpitch = bitmap->pitch;
		int sizex = bitmap->width;
		int sizey = bitmap->rows;
		//increment our dest pointer to x/y position
		for( int i = 0; i < y; i++ )
		{
			pBits += pitch;
		}
		pBits += x * (FONT_BITDEPTH>>3);
		//now we have our dest pointer were we want it to be, start copying
		for( int i = 0; i < sizey; i++ )
		{
			for( int w = 0; w < sizex; w++ )
			{
				short pixelsize = ( FONT_BITDEPTH >> 3 );
#ifdef XBOX
				pBits[0 + w*pixelsize ] = pSrc[ w ]; //alpha
				pBits[1 + w*pixelsize ] = 0xff;
				pBits[2 + w*pixelsize ] = 0xff;
				pBits[3 + w*pixelsize ] = 0xff;
#else
				pBits[0 + w*pixelsize ] = 0xff;
				pBits[1 + w*pixelsize ] = 0xff;
				pBits[2 + w*pixelsize ] = 0xff;
				pBits[3 + w*pixelsize ] = pSrc[ w ]; //alpha
#endif
			}
			//increment both pitches
			pSrc += srcpitch;
			pBits += pitch;
		}        
		tex->Unlock();
	}
}
EE_CDX9Font::EE_CDX9Font()
:
m_Face( NULL ),
m_VerticesWritten( 0 ),
m_IndicesWritten( 0 ),
m_VB( NULL ),
m_VBOffset( 0 ),
m_Text(""),
m_Delete( false ),
m_bVisible( true ),
m_IRenderer( NULL ),
m_LeadScale( 1.f ),
m_KernScale( 1.f ),
m_bUseKerning( true ),
m_Color( 0x00000000 ),
m_TotalVerticesWritten(0)
{
	m_CRect.x1 = -1000; // test condition to see if it was modified; used in render()
	xOff = 0;
	yOff = 0;
	memset( &m_RRect, 0, sizeof( m_RRect ) );
	m_Faces[0] = NULL;
	m_Faces[1] = NULL;
	m_Faces[2] = NULL;
	m_Faces[3] = NULL;
	m_NormVertexCount = 0;
	m_BoldVertexCount = 0;
	m_ItalicVertexCount = 0;
	m_BoldItalicVertexCount = 0;
	m_ImageVertexCount = 0;

	m_fWidthRatio = 1.f;
	m_fHeightRatio = 1.f;
	m_fZoomFactor = 1.f;
	m_iPageOffsetX = 0;
	m_iPageOffsetY = 0;

	m_TextWithTags = _T("");

	m_bWrap = false;

	m_hsNormFontFace = _T("");
	m_hsBoldFontFace = _T("");
	m_hsItalicFontFace = _T("");
	m_hsBoldItalicFontFace = _T("");

	m_bUseShadow = false;
	m_iShadowColor = 0xff000000;
	m_iShadowXScale = 0;
	m_iShadowYScale = 0;
	m_iShadowXOffset = 0;
	m_iShadowYOffset = 0;
}


bool EE_CDX9Font::SetFont( const TCHAR * filename, int height )
{
	m_hsNormFontFace = filename;
	m_PixHeight = height;
	return SetFontFace(NORMAL, filename, height);
}

bool EE_CDX9Font::SetFontBold( const TCHAR * filename, int height )
{
	m_hsBoldFontFace = filename;
	m_PixHeight = height;
	return SetFontFace(BOLD, filename, height);
}

bool EE_CDX9Font::SetFontItalic( const TCHAR * filename, int height )
{
	m_hsItalicFontFace = filename;
	m_PixHeight = height;
	return SetFontFace(ITALIC, filename, height);
}

bool EE_CDX9Font::SetFontBoldItalic( const TCHAR * filename, int height )
{
	m_hsBoldItalicFontFace = filename;
	m_PixHeight = height;
	return SetFontFace(BOLDITALIC, filename, height);
}

bool EE_CDX9Font::SetFontFace(FONTTYPE type, const TCHAR* filename, int height)
{
	if (_tcscmp(filename, "") == 0)
		return false;

	if (height <= 0)
	{
		return false;
	}

	StdString szFilePath(filename);
	szFilePath.MakeSafeFileName();
#ifdef XBOX
	szFilePath = SetPathDrive( szFilePath, EngineGetToolBox()->GetDrive() );
#endif

	FACEMAP *mapToCheck;
	switch(type)
	{
		case NORMAL:
		{
			mapToCheck = &m_NormFaceMap;
			break;
		}
		case BOLD:
		{
			mapToCheck = &m_BoldFaceMap;
			break;
		}
		case ITALIC:
		{
			mapToCheck = &m_ItalicFaceMap;
			break;
		}
		case BOLDITALIC:
		{
			mapToCheck = &m_BoldItalicFaceMap;
			break;
		}
		default:
		{
			mapToCheck = NULL;
			break;
		}
	}
	StdString tempFName = filename;
	tempFName.tolower();
	if( !m_IRenderer )
	{
		GETRENDERERINTERFACEPARAMS renderInterfaceParam;
		renderInterfaceParam.m_RendererInterface = NULL;
		static DWORD msgHash_GetRendererInterface = CHashString(_T("GetRendererInterface")).GetUniqueID();
		EngineGetToolBox()->SendMessage(msgHash_GetRendererInterface, sizeof(GETRENDERERINTERFACEPARAMS), &renderInterfaceParam, NULL, NULL );
		m_IRenderer = renderInterfaceParam.m_RendererInterface;
	}

	if(( m_FontLibrary ) && 
		(mapToCheck))
	{
		const TCHAR *pSuffix = _T("");
		switch(type)
		{
			case BOLD:			pSuffix = "b"; break;
			case ITALIC:		pSuffix = "i"; break;
			case BOLDITALIC:	pSuffix = "bi"; break;
		}

		TCHAR buffer[5];
		_stprintf(buffer, _T("_%d"), height);
		tempFName += buffer;
		tempFName += pSuffix;
		CHashString hfile( tempFName );
		FACEMAP::iterator iter = mapToCheck->find( hfile.GetUniqueID() );
		if( iter == mapToCheck->end() )
		{
			CDX9FontFace newface;
			int error = FT_New_Face( m_FontLibrary,
									szFilePath,
									0,
									&newface.m_FTFace );

			if ( error == FT_Err_Unknown_File_Format )
			{
				//the font file could be opened and read, but it appears
				//that its font format is unsupported
			}
			else if ( error )
			{
				//another error code means that the font file could not
				//be opened or read, or simply that it is broken...
			}
			else
			{
				error = FT_Set_Pixel_Sizes(
						newface.m_FTFace,   // handle to face object 
						0,					// pixel_width           
						height );			// pixel_height          

				if( !error )
				{
					int pen_x = 0;
					int pen_y = 0;
					int num_chars = MAX_FONT_CHAR;
					
					int necessaryTexHeight = 0;

					FT_Face &face = newface.m_FTFace; //reference
					FT_GlyphSlot  slot = face->glyph;
					int max_y = m_PixHeight + UV_BUFFER;
					newface.m_HasKerning = (FT_HAS_KERNING( newface.m_FTFace ))?true:false;

					// may as well cache this stuff
					FT_Bitmap bitmaps[MAX_FONT_CHAR];

					// first pass determines necessary texture height
					for ( int n = 0; n < num_chars; n++ )
					{
						FT_UInt  glyph_index;
						// retrieve glyph index from character code 
						glyph_index = FT_Get_Char_Index( face, (char)n );
						
						//save off index to use for kerning between two glyphs using
						// FT_Get_Kerning
						if( newface.m_HasKerning )
						{
							for( int k = 0; k < num_chars; k++ )
							{											
								FT_Vector  delta;
								FT_UInt g2 = FT_Get_Char_Index( face, (char)k );
								FT_Get_Kerning( face, glyph_index, g2,
												FT_KERNING_DEFAULT, &delta );
								short kern = (short)(delta.x >> 6);//from points to pixels
								newface.m_Kerning[ n ][ k ] = kern;
							}
						}
						// load glyph image into the slot (erase previous one) 
						error = FT_Load_Glyph( face, glyph_index, FT_LOAD_DEFAULT );
						if ( error )
							continue;  // ignore errors 
		
						// convert to an anti-aliased bitmap
						error = FT_Render_Glyph( face->glyph, FT_RENDER_MODE_NORMAL );
						if ( error )
							continue;

						// cache bitmap
						bitmaps[n] = slot->bitmap;
						// deep copy buffer, since it's a single buffer reused (not unique per character)
						UINT bufSize = slot->bitmap.width * slot->bitmap.rows;
						unsigned char *buf = new unsigned char[bufSize];
						memcpy(buf, slot->bitmap.buffer, bufSize);
						bitmaps[n].buffer = buf;

						//save metrics:
						if( slot->bitmap.rows == max_y )
						{
							max_y = slot->bitmap.rows + UV_BUFFER;							
						}
						newface.m_Metrics[ n ] = slot->metrics;
						
						if (max_y+pen_y > necessaryTexHeight)
						{
							necessaryTexHeight = max_y+pen_y;
						}

						if( pen_x + slot->bitmap.width + UV_BUFFER > TEXTURESIZE )
						{
							//skip down
							pen_y += max_y;
							max_y = m_PixHeight+ UV_BUFFER;
							pen_x = 0;
						}

						// increment pen position
						pen_x += slot->bitmap.width + UV_BUFFER;
					}

					// NOW determine smallest power of two to fit tex (from 64 on)
					int texHeight = 64;
					while (texHeight < necessaryTexHeight)
					{
						texHeight = texHeight << 1;
					}

					//build and insert face
					newface.m_RefCount = 1;
					//create the texture
					// load the texture from the file
					CREATETEXTUREPARAMS tobj;
					tobj.bitDepth = FONT_BITDEPTH;
					tobj.Name = &hfile;
					tobj.sizeX = TEXTURESIZE;
					tobj.sizeY = texHeight;
					tobj.TextureObjectInterface = NULL;

					static DWORD msgHash_CreateTexture = CHashString(_T("CreateTexture")).GetUniqueID();
					DWORD res = EngineGetToolBox()->SendMessage(msgHash_CreateTexture, sizeof(tobj), &tobj );
					if( res == MSG_HANDLED )
					{
						// store the texture id in the user data of the map
						newface.m_Texture = (ITextureObject *)tobj.TextureObjectInterface;
					}else
					{
						return false;
					}

					// now we iterate again and actually write the glyphs
					pen_x = 0;
					pen_y = 0;
					for ( int n = 0; n < num_chars; n++ )
					{
						//save metrics:
						if( bitmaps[n].rows == max_y )
						{
							max_y = bitmaps[n].rows + UV_BUFFER;
						}
						
						if( pen_x + bitmaps[n].width + UV_BUFFER > TEXTURESIZE )
						{
							//skip down
							pen_y += max_y;
							max_y = m_PixHeight+ UV_BUFFER;
							pen_x = 0;
						}
						// now, draw to our target surface
						DrawToTexture( newface.m_Texture,
										&bitmaps[n],
										pen_x ,
										pen_y );
						//save bounds
						newface.m_CharCoords[ n ].x1 = (float)((double)(pen_x) / (double)TEXTURESIZE);
						newface.m_CharCoords[ n ].x2 = (float)((double)(pen_x + bitmaps[n].width )
														/ (double)TEXTURESIZE);
						newface.m_CharCoords[ n ].y1 = (float)((double)(pen_y  )/ (double)texHeight);
						newface.m_CharCoords[ n ].y2 = (float)((double)(pen_y + bitmaps[n].rows )
														/ (double)texHeight);

						// increment pen position
						pen_x += bitmaps[n].width + UV_BUFFER;

						// clear buffer
						delete [] bitmaps[n].buffer;
					}

					// TODO: may want to uncomment if we want 3d fonts with perspective...
					//newface.m_Texture->GenerateMips();

					//insert into map						
					pair<FACEMAP::iterator, bool> insResult = mapToCheck->insert(
						pair<DWORD, CDX9FontFace>(hfile.GetUniqueID(), newface));

					FACEMAP::iterator insElem = insResult.first;

					m_Faces[type] = &insElem->second;
				}
				return true;
			}
		}
		else
		{
			//face found, set it as the active face
			m_Faces[type] = &((*iter).second);
			m_Faces[type]->m_RefCount++;
		}
	}
	return false;
}

int EE_CDX9Font::GetFontPixWidth( const TCHAR * str )
{
	if( m_Faces[NORMAL]  )
	{
		if( m_Faces[NORMAL]->m_HasKerning && m_bUseKerning)
		{
			int strlength = _tcslen( str );
			float length = 0;
			// for each glyph image, compute its bounding box,     
			// translate it, and grow the string bbox           
			for ( int n = 0; n < strlength-1; n++ )
			{
				TCHAR c, c1;
				c = str[n];
				c1 = str[n+1];
				if( c >= MAX_FONT_CHAR ||
					c1 >= MAX_FONT_CHAR )
				{
					return 0;//out of bounds
				}
				if (c < 0)
				{
					continue;
				}
				if (c1 < 0)
				{
					continue;
				}
				short delta = m_Faces[NORMAL]->m_Kerning[ c ][ c1 ];
				length += m_KernScale*(float)delta;

			}
			return (int)length;

		}else
		{
			int strlength = _tcslen( str );
			float length = 0;
			// for each glyph image, compute its bounding box,     
			// translate it, and grow the string bbox           
			for ( int n = 0; n < strlength; n++ )
			{
				TCHAR c = str[n];
				if( c >= MAX_FONT_CHAR )
				{
					return 0;//out of bounds
				}
				if (c < 0)
				{
					continue;
				}
				short delta = (short)(m_Faces[NORMAL]->m_Metrics[ str[ n ] ].horiAdvance>>6);
				length += m_KernScale*(float)delta;

			}
			return (int)length;
		}
	}
	return 0;
}

int EE_CDX9Font::GetFontPixWidth( CHashString fontFace, UINT fontsize, const TCHAR * str )
{
	StdString tempFName = fontFace.GetString();
	tempFName.tolower();
	TCHAR buf[5];
	_stprintf(buf, _T("_%d"), fontsize);
	tempFName += buf;
	fontFace = tempFName;

	FACEMAP::iterator normIter = m_NormFaceMap.find(fontFace.GetUniqueID());
	FACEMAP::iterator boldIter = m_BoldFaceMap.find(fontFace.GetUniqueID());
	FACEMAP::iterator italicIter = m_ItalicFaceMap.find(fontFace.GetUniqueID());
	FACEMAP::iterator boldItalicIter = m_BoldItalicFaceMap.find(fontFace.GetUniqueID());

	if ((normIter == m_NormFaceMap.end()) &&
		(boldIter == m_BoldFaceMap.end()) &&
		(italicIter == m_ItalicFaceMap.end()) &&
		(boldItalicIter == m_BoldItalicFaceMap.end()))
	{
		return 0;
	}

	FONTTYPE mapSwitch;
	if (normIter != m_NormFaceMap.end())
	{
		mapSwitch = NORMAL;
	}
	else if(boldIter != m_BoldFaceMap.end())
	{
		mapSwitch = BOLD;
	}
	else if (italicIter == m_ItalicFaceMap.end())
	{
		mapSwitch = ITALIC;
	}
	else if(boldItalicIter == m_BoldItalicFaceMap.end())
	{
		mapSwitch = BOLDITALIC;
	}

	int strlength = _tcslen( str );
	float length = 0;
	// for each glyph image, compute its bounding box,     
	// translate it, and grow the string bbox           
	for ( int n = 0; n < strlength; n++ )
	{
		TCHAR c = str[n];
		if( c >= MAX_FONT_CHAR )
		{
			return 0;//out of bounds
		}
		if (c < 0)
		{
			continue;
		}
		short delta;
		switch(mapSwitch)
		{
			case NORMAL:
			{
				delta = (short)normIter->second.m_Metrics[ str[ n ] ].horiAdvance>>6;
				break;
			}
			case BOLD:
			{
				delta = (short)boldIter->second.m_Metrics[ str[ n ] ].horiAdvance>>6;
				break;
			}
			case ITALIC:
			{
				delta = (short)italicIter->second.m_Metrics[ str[ n ] ].horiAdvance>>6;
				break;
			}
			case BOLDITALIC:
			{
				delta = (short)boldItalicIter->second.m_Metrics[ str[ n ] ].horiAdvance>>6;
				break;
			}
			default:
			{
				delta = 0;
			}
		}
		length += /*m_KernScale* */(float)delta;

	}
	return (int)length;
}

void EE_CDX9Font::SetFontPixHeight( int height )
{
	if( m_Faces[NORMAL] )
	{
		SetFont( m_hsNormFontFace.GetString(), height );
	}
}


bool EE_CDX9Font::Update()
{
	//deferred rendering for fonts?
	//create vertices
	if( m_bVisible &&
		m_Faces[NORMAL] )
	{
		m_VerticesWritten = 0;
		m_IndicesWritten = 0;
		m_NormVertexCount = 0;
		m_BoldVertexCount = 0;
		m_ItalicVertexCount = 0;
		m_BoldItalicVertexCount = 0;
		m_ImageVertexCount = 0;
		m_ImageVec.clear();			

		UpdateBuffers();
	}
	return (!m_Delete);
}

void EE_CDX9Font::UpdateBuffers()
{
	float x = 0.f;
	float y = (float)m_PixHeight;
	for( int line = 0; line < (int)m_Lines.size(); line++ )
	{
		if (m_CRect.x1 == -1000)
		{
			if (y-m_PixHeight > m_RRect.y2 - m_RRect.y1)
			{
				y += m_LeadScale*(float)m_PixHeight;
				x = 0.f;//m_RRect.x1 + xOff;
				continue;
			}
		}
		else
		{
			if (y-m_PixHeight > m_CRect.y2 - m_CRect.y1)
			{
				y += m_LeadScale*(float)m_PixHeight;
				x = 0.f; //m_RRect.x1 + xOff;
				continue;
			}
		}

		size_t iLineSize = m_Lines[line].size();
		for( unsigned int i = 0; i < iLineSize; i++ )
		{
			tagString &lineString = m_Lines[line];
			taggableChar &lineChar = lineString[i];

			if (lineChar.imageID == 0)
			{
				TCHAR c = lineChar.data;
				FONTTYPE charFType;
				if ((lineChar.bold) && (lineChar.italic))
				{
					if (m_Faces[BOLDITALIC])
					{
						charFType = BOLDITALIC;
					}
					else
					{
						charFType = NORMAL;
					}
				}
				else if (lineChar.bold)
				{
					if (m_Faces[BOLD])
					{
						charFType = BOLD;
					}
					else
					{
						charFType = NORMAL;
					}
				}
				else if (lineChar.italic)
				{
					if (m_Faces[ITALIC])
					{
						charFType = ITALIC;
					}
					else
					{
						charFType = NORMAL;
					}
				}
				else
				{
					charFType = NORMAL;
				}
				
				DWORD currentColor = m_Lines[line][i].color;

				if (c < 0) // hey, it happens...
				{
					c = ' ';
				}
				int index = m_VerticesWritten;
				float starty = y - (float)(m_Faces[charFType]->m_Metrics[c].horiBearingY>>6);
				float startx = x + (float)(m_Faces[charFType]->m_Metrics[c].horiBearingX>>6);
				float stopx = startx + (float)(m_Faces[charFType]->m_Metrics[c].width>>6);
				float stopy = starty + (float)(m_Faces[charFType]->m_Metrics[c].height>>6);

				// don't write verts for a space
				if (c != ' ')
				{
					// NOTE: winding is now cw
					m_VBBuffer[0 + index].pos[ 0 ] = startx-0.5f;
					m_VBBuffer[0 + index].pos[ 1 ] = starty-0.5f;
					m_VBBuffer[0 + index].pos[ 2 ] = 0;
					m_VBBuffer[0 + index].u = m_Faces[charFType]->m_CharCoords[ c ].x1;
					m_VBBuffer[0 + index].v = m_Faces[charFType]->m_CharCoords[ c ].y1;
					m_VBBuffer[0 + index].color = currentColor;
					
					m_VBBuffer[1 + index].pos[ 0 ] = stopx-0.5f;
					m_VBBuffer[1 + index].pos[ 1 ] = starty-0.5f;
					m_VBBuffer[1 + index].pos[ 2 ] = 0;
					m_VBBuffer[1 + index].u = m_Faces[charFType]->m_CharCoords[ c ].x2;
					m_VBBuffer[1 + index].v = m_Faces[charFType]->m_CharCoords[ c ].y1;
					m_VBBuffer[1 + index].color = currentColor;
					
					m_VBBuffer[2 + index].pos[ 0 ] = startx-0.5f;
					m_VBBuffer[2 + index].pos[ 1 ] = stopy-0.5f;
					m_VBBuffer[2 + index].pos[ 2 ] = 0;
					m_VBBuffer[2 + index].u = m_Faces[charFType]->m_CharCoords[ c ].x1;
					m_VBBuffer[2 + index].v = m_Faces[charFType]->m_CharCoords[ c ].y2;
					m_VBBuffer[2 + index].color = currentColor;
					
					m_VBBuffer[3 + index].pos[ 0 ] = stopx-0.5f;
					m_VBBuffer[3 + index].pos[ 1 ] = stopy-0.5f;
					m_VBBuffer[3 + index].pos[ 2 ] = 0;
					m_VBBuffer[3 + index].u = m_Faces[charFType]->m_CharCoords[ c ].x2;
					m_VBBuffer[3 + index].v = m_Faces[charFType]->m_CharCoords[ c ].y2;
					m_VBBuffer[3 + index].color = currentColor;
				
					if (m_bUseShadow)
					{
						m_VBBuffer[0 + 4 + index] = m_VBBuffer[0 + index];
						m_VBBuffer[1 + 4 + index] = m_VBBuffer[1 + index];
						m_VBBuffer[2 + 4 + index] = m_VBBuffer[2 + index];
						m_VBBuffer[3 + 4 + index] = m_VBBuffer[3 + index];

						index += 4;

						m_VBBuffer[0 + index].color = m_iShadowColor;
						m_VBBuffer[1 + index].color = m_iShadowColor;
						m_VBBuffer[2 + index].color = m_iShadowColor;
						m_VBBuffer[3 + index].color = m_iShadowColor;

						m_VBBuffer[0 + index].pos[ 0 ] += (float)(m_iShadowXOffset - m_iShadowXScale);
						m_VBBuffer[0 + index].pos[ 1 ] += (float)(m_iShadowYOffset - m_iShadowYScale);

						m_VBBuffer[1 + index].pos[ 0 ] += (float)(m_iShadowXOffset + m_iShadowXScale);
						m_VBBuffer[1 + index].pos[ 1 ] += (float)(m_iShadowYOffset - m_iShadowYScale);
						
						m_VBBuffer[2 + index].pos[ 0 ] += (float)(m_iShadowXOffset - m_iShadowXScale);
						m_VBBuffer[2 + index].pos[ 1 ] += (float)(m_iShadowYOffset + m_iShadowYScale);
						
						m_VBBuffer[3 + index].pos[ 0 ] += (float)(m_iShadowXOffset + m_iShadowXScale);
						m_VBBuffer[3 + index].pos[ 1 ] += (float)(m_iShadowYOffset + m_iShadowYScale);
					}

					unsigned int ibIndex=0;
					if (charFType == NORMAL)
					{
						ibIndex = m_NormVertexCount;
					}
					else if (charFType == BOLD)
					{
						ibIndex = m_BoldVertexCount;
					}
					else if (charFType == ITALIC)
					{
						ibIndex = m_ItalicVertexCount;
					}
					else if (charFType == BOLDITALIC)
					{
						ibIndex = m_BoldItalicVertexCount;
					}
					
					if (m_bUseShadow)
					{
						m_IBBuffer[charFType][0 + ibIndex] = 0 + 4 + m_VerticesWritten;
						m_IBBuffer[charFType][1 + ibIndex] = 1 + 4 + m_VerticesWritten;
						m_IBBuffer[charFType][2 + ibIndex] = 2 + 4 + m_VerticesWritten;
						m_IBBuffer[charFType][3 + ibIndex] = 3 + 4 + m_VerticesWritten;
						m_IBBuffer[charFType][4 + ibIndex] = 2 + 4 + m_VerticesWritten;
						m_IBBuffer[charFType][5 + ibIndex] = 1 + 4 + m_VerticesWritten;

						m_IBBuffer[charFType][0 + 6 + ibIndex] = 0 + m_VerticesWritten;
						m_IBBuffer[charFType][1 + 6 + ibIndex] = 1 + m_VerticesWritten;
						m_IBBuffer[charFType][2 + 6 + ibIndex] = 2 + m_VerticesWritten;
						m_IBBuffer[charFType][3 + 6 + ibIndex] = 3 + m_VerticesWritten;
						m_IBBuffer[charFType][4 + 6 + ibIndex] = 2 + m_VerticesWritten;
						m_IBBuffer[charFType][5 + 6 + ibIndex] = 1 + m_VerticesWritten;

						if (charFType == NORMAL)
						{
							m_NormVertexCount += 12;
						}
						else if (charFType == BOLD) 
						{
							m_BoldVertexCount += 12;
						}
						else if (charFType == ITALIC)
						{
							m_ItalicVertexCount += 12;
						}
						else if (charFType == BOLDITALIC)
						{
							m_BoldItalicVertexCount += 12;
						}

						m_VerticesWritten += 8;
						m_IndicesWritten += 12;
					}
					else
					{
						m_IBBuffer[charFType][0 + ibIndex] = 0 + index;
						m_IBBuffer[charFType][1 + ibIndex] = 1 + index;
						m_IBBuffer[charFType][2 + ibIndex] = 2 + index;
						m_IBBuffer[charFType][3 + ibIndex] = 3 + index;
						m_IBBuffer[charFType][4 + ibIndex] = 2 + index;
						m_IBBuffer[charFType][5 + ibIndex] = 1 + index;

						if (charFType == NORMAL)
						{
							m_NormVertexCount+=6;
						}
						else if (charFType == BOLD) 
						{
							m_BoldVertexCount+=6;
						}
						else if (charFType == ITALIC)
						{
							m_ItalicVertexCount+=6;
						}
						else if (charFType == BOLDITALIC)
						{
							m_BoldItalicVertexCount+=6;
						}

						m_VerticesWritten += 4;
						m_IndicesWritten += 6;
					}
				}

				//now shift x by kerning
				if( m_Faces[charFType]->m_HasKerning && m_bUseKerning)
				{					
					if( i < iLineSize - 1 )
					{
						TCHAR cnext = (TCHAR)m_Lines[line][ i + 1].data;
						x += m_KernScale*(float)(m_Faces[charFType]->m_Kerning[ c ][ cnext ]);
					}				
				}else
				{
					x += m_KernScale*(float)(m_Faces[charFType]->m_Metrics[c].horiAdvance>>6);					
				}				
			}
			else
			{
				IBaseTextureObject *tex = m_ImageMap[lineChar.imageID];
				int index = m_VerticesWritten;
				float starty = y - m_PixHeight;
				float startx = x;
				float width = (float)m_PixHeight;
				
				if (tex)
				{
					UINT texWidth = tex->GetWidth();
					UINT texHeight = tex->GetHeight();
					float ratio = (float)texWidth / (float)texHeight;
					width = (float)m_PixHeight * ratio;
				}

				m_ImageVec.push_back(tex);

				float stopx = startx + width; 
				float stopy = y;

				DWORD currentColor = lineChar.color;
				currentColor = (currentColor & 0xff000000) + 0xffffff;
				
				// NOTE: winding is now cw
				m_VBBuffer[0 + index].pos[ 0 ] = startx-0.5f;
				m_VBBuffer[0 + index].pos[ 1 ] = starty-0.5f;
				m_VBBuffer[0 + index].pos[ 2 ] = 0;
				m_VBBuffer[0 + index].u = 0.0f;
				m_VBBuffer[0 + index].v = 0.0f;
				m_VBBuffer[0 + index].color = currentColor;
				
				m_VBBuffer[1 + index].pos[ 0 ] = stopx-0.5f;
				m_VBBuffer[1 + index].pos[ 1 ] = starty-0.5f;
				m_VBBuffer[1 + index].pos[ 2 ] = 0;
				m_VBBuffer[1 + index].u = 1.0f;
				m_VBBuffer[1 + index].v = 0.0f;
				m_VBBuffer[1 + index].color = currentColor;
				
				m_VBBuffer[2 + index].pos[ 0 ] = startx-0.5f;
				m_VBBuffer[2 + index].pos[ 1 ] = stopy-0.5f;
				m_VBBuffer[2 + index].pos[ 2 ] = 0;
				m_VBBuffer[2 + index].u = 0.0f;
				m_VBBuffer[2 + index].v = 1.0f;
				m_VBBuffer[2 + index].color = currentColor;
				
				m_VBBuffer[3 + index] = m_VBBuffer[1 + index];
				m_VBBuffer[3 + index].color = currentColor;
				m_VBBuffer[5 + index] = m_VBBuffer[2 + index];
				m_VBBuffer[5 + index].color = currentColor;
				m_VBBuffer[4 + index].pos[ 0 ] = stopx-0.5f;
				m_VBBuffer[4 + index].pos[ 1 ] = stopy-0.5f;
				m_VBBuffer[4 + index].pos[ 2 ] = 0;
				m_VBBuffer[4 + index].u = 1.0f;
				m_VBBuffer[4 + index].v = 1.0f;
				m_VBBuffer[4 + index].color = currentColor;
				
				m_IBBuffer[IMAGE][0 + m_ImageVertexCount] = 0 + index;
				m_IBBuffer[IMAGE][1 + m_ImageVertexCount] = 1 + index;
				m_IBBuffer[IMAGE][2 + m_ImageVertexCount] = 2 + index;
				m_IBBuffer[IMAGE][3 + m_ImageVertexCount] = 3 + index;
				m_IBBuffer[IMAGE][4 + m_ImageVertexCount] = 4 + index;
				m_IBBuffer[IMAGE][5 + m_ImageVertexCount] = 5 + index;

				m_ImageVertexCount += 6;

				x += width;
				m_VerticesWritten += 6;
				m_IndicesWritten += 6;
			}
		}
		//increment y pos
		y += m_LeadScale*(float)m_PixHeight;
		x = 0.f;
	}
}

EE_CDX9Font::~EE_CDX9Font()
{
	for(int i = 0; i < 4; i++ )
	{
		if( m_Faces[i] )
		{
			m_Faces[i]->m_RefCount--;
			if( m_Faces[i]->m_RefCount == 0 )
			{
				/// Remove Face
				// Face already removed?!?!
				
				/// Remove Texture
				TEXTUREOBJECTPARAMS tobj;
				tobj.bLoad = false;
				tobj.Name = NULL;
				tobj.TextureObjectInterface = m_Faces[i]->m_Texture;
				static DWORD msgHash_RemoveTexture = CHashString(_T("RemoveTexture")).GetUniqueID();
				EngineGetToolBox()->SendMessage(msgHash_RemoveTexture, sizeof(tobj), &tobj );
				m_Faces[i]->m_Texture = NULL;
			}
		}
	}
}

/// returns offset
int EE_CDX9Font::UpdateBuffer( char ** pDest, int offset,IVertexBufferObject * vb )
{
	if( m_bVisible &&
		m_Faces[NORMAL] )
	{
		m_VB = vb;
		m_VBOffset = offset;
		memcpy( (*pDest), m_VBBuffer, m_VerticesWritten*sizeof( FONT_VERTEX ) );
		(*pDest) += m_VerticesWritten*sizeof( FONT_VERTEX );
		return m_VerticesWritten;
	}
	return 0;
}

int EE_CDX9Font::UpdateIndexBuffer( char ** pDest, int offset,IIndexBuffer * ib )
{
	if( m_bVisible)
	{
		m_NormIBOffset = offset;
		m_BoldIBOffset = m_NormIBOffset + m_NormVertexCount;
		m_ItalicIBOffset = m_BoldIBOffset + m_BoldVertexCount;
		m_BoldItalicIBOffset = m_ItalicIBOffset + m_ItalicVertexCount;
		m_ImageIBOffset = m_BoldItalicIBOffset + m_BoldItalicVertexCount;
		if (m_Faces[NORMAL])
		{
			m_NormalIB = ib;			
			memcpy( (*pDest), m_IBBuffer[NORMAL], m_NormVertexCount*sizeof( short ) );
			(*pDest) += m_NormVertexCount*sizeof( short );
		}	
		if (m_Faces[BOLD])
		{
			m_BoldIB = ib;
			memcpy( (*pDest), m_IBBuffer[BOLD], m_BoldVertexCount*sizeof( short ) );
			(*pDest) += m_BoldVertexCount*sizeof( short );
		}
		if (m_Faces[ITALIC])
		{
			m_ItalicIB = ib;			
			memcpy( (*pDest), m_IBBuffer[ITALIC], m_ItalicVertexCount*sizeof( short ) );
			(*pDest) += m_ItalicVertexCount*sizeof( short );
		}
		if (m_Faces[BOLDITALIC])
		{
			m_BoldItalicIB = ib;			
			memcpy( (*pDest), m_IBBuffer[BOLDITALIC], m_BoldItalicVertexCount*sizeof( short ) );
			(*pDest) += m_BoldItalicVertexCount*sizeof( short );
		}
		if (m_ImageVertexCount > 0)
		{
			m_ImageIB = ib;			
			memcpy( (*pDest), m_IBBuffer[IMAGE], m_ImageVertexCount*sizeof( short ) );
			(*pDest) += m_ImageVertexCount*sizeof( short );
		}
		m_TotalVerticesWritten = m_IndicesWritten + offset;
		return m_IndicesWritten;
	}
	return 0;
}

void EE_CDX9Font::Render()
{
	if( m_bVisible &&
		m_Faces[NORMAL] &&
		m_IndicesWritten > 0 &&
		m_VB )
	{
		m_IRenderer->PushMatrix( WORLD_MATRIX );

		Matrix4x4 matWorld;
		m_IRenderer->GetMatrix(WORLD_MATRIX, matWorld.GetMatrix());
		Vec3 offset(((m_RRect.x1 + xOff)), ((m_RRect.y1 + yOff)), 0.0f);
		Vec3 original = matWorld.GetTranslation();
		Vec3 scale = matWorld.GetScale();
		original += offset*scale;
		matWorld.SetTranslation(original);
		m_IRenderer->SetMatrix(WORLD_MATRIX, (const float*)matWorld.GetMatrix());

		m_IRenderer->SetEffect( 0, NULL );
		m_IRenderer->SetDepthWrite( false );
		m_IRenderer->SetDepthTest( false );

		m_IRenderer->SetTexture( 0, (m_Faces[NORMAL]->m_Texture) );
		m_IRenderer->RenderIndexBuffer(m_NormalIB, m_VB, m_VBOffset, m_TotalVerticesWritten, m_NormIBOffset, 
			m_NormVertexCount/3);
		// if there's a bold font loaded
		if (m_Faces[BOLD])
		{
			m_IRenderer->SetTexture( 0, (m_Faces[BOLD]->m_Texture) );
			m_IRenderer->RenderIndexBuffer(m_BoldIB, m_VB, m_VBOffset, m_TotalVerticesWritten, m_BoldIBOffset, 
				m_BoldVertexCount/3);
		}
		if (m_Faces[ITALIC])
		{
			m_IRenderer->SetTexture( 0, (m_Faces[ITALIC]->m_Texture) );
			m_IRenderer->RenderIndexBuffer(m_ItalicIB, m_VB, m_VBOffset, m_TotalVerticesWritten, m_ItalicIBOffset, 
				m_ItalicVertexCount/3);
		}
		if (m_Faces[BOLDITALIC])
		{
			m_IRenderer->SetTexture( 0, (m_Faces[BOLDITALIC]->m_Texture) );
			m_IRenderer->RenderIndexBuffer(m_BoldItalicIB, m_VB, m_VBOffset, m_TotalVerticesWritten, m_BoldItalicIBOffset, 
				m_BoldItalicVertexCount/3);
		}

		if (m_ImageVec.size() > 0)
		{
			for (UINT i=0; i<m_ImageVec.size(); i++)
			{
				m_IRenderer->SetTexture( 0, m_ImageVec[i] );
				m_IRenderer->RenderIndexBuffer(m_ImageIB, m_VB, m_VBOffset, m_TotalVerticesWritten, m_ImageIBOffset+(i*6), 2);
			}
		}

		m_IRenderer->SetDepthWrite( true );	
		m_IRenderer->SetDepthTest( true );
		m_IRenderer->PopMatrix( WORLD_MATRIX );
	}
}


void EE_CDX9Font::SetText( const TCHAR * txt, char delimators[], int numtokendelim )
{
	PERFORMANCE_PROFILER_START(DX9Font_ParseTags);
	m_TextWithTags = txt;
	unsigned int k;
	bool isBold = false;
	bool isItalic = false;
	bool isUnderline = false;
	bool isStrikethru = false;
	vector<taggableChar> firstPassChars;
	firstPassChars.clear();
	vector<charColor> colorsRead;
	colorsRead.clear();
	charColor currentColorRead;
	unsigned int currentTextIndex = 0;

	static CHashString boldHash(_T("Bold"));
	static CHashString italicHash(_T("Italic"));
	static CHashString underlineHash(_T("Underline"));
	static CHashString strikethruHash(_T("StrikeThru"));
	static CHashString fcolorHash(_T("FColor"));
	static CHashString brHash(_T("BR"));
	static CHashString imgHash(_T("IMG"));

	// parse string for tags first
	for (k=0; k<_tcslen(txt); k++)
	{
		if (txt[k] == '[')
		{
			k++;
			if (txt[k] == '/') // end block
			{
				k++;
				StdString tempRead = "";
				
				while ((txt[k] != ']') && (txt[k] != '='))
				{
					tempRead += txt[k];
					k++;
				}

				CHashString tempHash(tempRead);
				if (tempHash.GetUniqueID() == boldHash.GetUniqueID())
				{
					isBold = false;
				}
				else if (tempHash.GetUniqueID() == italicHash.GetUniqueID())
				{
					isItalic = false;
				}
				else if (tempHash.GetUniqueID() == underlineHash.GetUniqueID())
				{
					isUnderline = false;
				}
				else if (tempHash.GetUniqueID() == strikethruHash.GetUniqueID())
				{
					isStrikethru = false;
				}
				else if (tempHash.GetUniqueID() == fcolorHash.GetUniqueID())
				{
					if (txt[k] == '=')
					{
						k++;
						unsigned int tempR, tempG, tempB, tempA;
						string sR, sG, sB, sA;
						
						while (txt[k] != ',') // read R component
						{
							sR += txt[k];
							k++;
						}
						tempR = (unsigned int)atoi(sR.c_str());
						k++;

						while (txt[k] != ',') // read G component
						{
							sG += txt[k];
							k++;
						}
						tempG = (unsigned int)atoi(sG.c_str());
						k++;

						while (txt[k] != ',') // read B component
						{
							sB += txt[k];
							k++;
						}
						tempB = (unsigned int)atoi(sB.c_str());
						k++;

						while (txt[k] != ']') // read A component
						{
							sA += txt[k];
							k++;
						}
						tempA = (unsigned int)atoi(sB.c_str());

						currentColorRead.endIndex = currentTextIndex;

						// don't bother pushing back if there's no characters between tags
						if (currentColorRead.endIndex != currentColorRead.startIndex)
						{
							currentColorRead.endA = tempA;
							currentColorRead.endR = tempR;
							currentColorRead.endG = tempG;
							currentColorRead.endB = tempB;
							currentColorRead.aFactor = ((currentColorRead.endA - currentColorRead.startA) / (currentColorRead.endIndex - currentColorRead.startIndex));
							currentColorRead.rFactor = ((currentColorRead.endR - currentColorRead.startR) / (currentColorRead.endIndex - currentColorRead.startIndex));
							currentColorRead.gFactor = ((currentColorRead.endG - currentColorRead.startG) / (currentColorRead.endIndex - currentColorRead.startIndex));
							currentColorRead.bFactor = ((currentColorRead.endB - currentColorRead.startB) / (currentColorRead.endIndex - currentColorRead.startIndex));
							colorsRead.push_back(currentColorRead);
						}
					}
					else
					{
						currentColorRead.endIndex = currentTextIndex;
						// don't bother pushing back if there's no characters between tags
						if (currentColorRead.endIndex != currentColorRead.startIndex)
						{
							currentColorRead.endR = currentColorRead.startR;
							currentColorRead.endG = currentColorRead.startG;
							currentColorRead.endB = currentColorRead.startB;
							currentColorRead.endA = currentColorRead.startA;							
							currentColorRead.aFactor = ((currentColorRead.endA - currentColorRead.startA) / (currentColorRead.endIndex - currentColorRead.startIndex));
							currentColorRead.rFactor = ((currentColorRead.endR - currentColorRead.startR) / (currentColorRead.endIndex - currentColorRead.startIndex));
							currentColorRead.gFactor = ((currentColorRead.endG - currentColorRead.startG) / (currentColorRead.endIndex - currentColorRead.startIndex));
							currentColorRead.bFactor = ((currentColorRead.endB - currentColorRead.startB) / (currentColorRead.endIndex - currentColorRead.startIndex));
							colorsRead.push_back(currentColorRead);
						}
					}
				}				
			}
			else // open block
			{
				StdString tempRead = "";
				
				while ((txt[k] != ']') && (txt[k] != '='))
				{
					tempRead += txt[k];
					k++;
				}
				CHashString tempHash(tempRead);
				if (tempHash.GetUniqueID() == boldHash.GetUniqueID())
				{
					isBold = true;
				}
				else if (tempHash.GetUniqueID() == italicHash.GetUniqueID())
				{
					isItalic = true;
				}
				else if (tempHash.GetUniqueID() == underlineHash.GetUniqueID())
				{
					isUnderline = true;
				}
				else if (tempHash.GetUniqueID() == strikethruHash.GetUniqueID())
				{
					isStrikethru = true;
				}
				else if (tempHash.GetUniqueID() == fcolorHash.GetUniqueID())
				{
					k++;
					unsigned int tempR, tempG, tempB, tempA;
					string sR, sG, sB, sA;
					
					while (txt[k] != ',') // read R component
					{
						sR += txt[k];
						k++;
					}
					tempR = (unsigned int)atoi(sR.c_str());
					k++;

					while (txt[k] != ',') // read G component
					{
						sG += txt[k];
						k++;
					}
					tempG = (unsigned int)atoi(sG.c_str());
					k++;

					while (txt[k] != ',') // read B component
					{
						sB += txt[k];
						k++;
					}
					tempB = (unsigned int)atoi(sB.c_str());
					k++;

					while (txt[k] != ']') // read A component
					{
						sA += txt[k];
						k++;
					}
					tempA = (unsigned int)atoi(sA.c_str());
										
					currentColorRead.startIndex = currentTextIndex;		
					currentColorRead.startA = tempA;
					currentColorRead.startR = tempR;
					currentColorRead.startG = tempG;
					currentColorRead.startB = tempB;
				}
				else if (tempHash.GetUniqueID() == brHash.GetUniqueID())
				{
					taggableChar tempChar;
					tempChar.data = '\n';
					firstPassChars.push_back(tempChar);
					currentTextIndex++;
				}
				else if (tempHash.GetUniqueID() == imgHash.GetUniqueID())
				{
					k++;
					StdString imgName;
					while (txt[k] != ']') // read A component
					{
						imgName += txt[k];
						k++;
					}
					CHashString imgHash(imgName);
					if (m_ImageMap[imgHash.GetUniqueID()] == NULL)
					{
						// TODO: load image via font manager
						IBaseTextureObject *tex=NULL;

						GETFONTGLYPHTEXTUREPARAMS gfgtp;
						gfgtp.hsKey = &imgHash;
						static DWORD msgGetFontGlyphTexture = CHashString(_T("GetFontGlyphTexture")).GetUniqueID();
						EngineGetToolBox()->SendMessage(msgGetFontGlyphTexture, sizeof(GETFONTGLYPHTEXTUREPARAMS), &gfgtp);
						tex = gfgtp.pTexture;

						if (tex == NULL)
						{
							// load missing texture
							static CHashString hsMissingTexName(_T("basedata\\editor\\textures\\missing.tga"));
							TEXTUREOBJECTPARAMS tobj;
							tobj.bLoad = true;
							tobj.Name = &hsMissingTexName;
							tobj.TextureObjectInterface = NULL;
							static DWORD msgHash_AddTexture = CHashString(_T("AddTexture")).GetUniqueID();
							EngineGetToolBox()->SendMessage(msgHash_AddTexture, sizeof(tobj), &tobj );
							tex = tobj.TextureObjectInterface;
						}

						m_ImageMap[imgHash.GetUniqueID()] = tex;
					}
                    
					// insert a 'character' here that will be interpreted later as an image
					taggableChar tempReadChar;
					tempReadChar.imageID = imgHash.GetUniqueID();
					firstPassChars.push_back(tempReadChar);
					currentTextIndex++;
				}				
			}
		}
		else
		{
			taggableChar tempReadChar;
			tempReadChar.data = txt[k];
			tempReadChar.bold = isBold;
			tempReadChar.italic = isItalic;
			tempReadChar.underline = isUnderline;
			tempReadChar.strikethru = isStrikethru;
			firstPassChars.push_back(tempReadChar);
			currentTextIndex++;
		}
	}

	if (firstPassChars.size() == 0)
	{
		m_Lines.clear();
		m_Text = "";
		return; // may as well exit out, there's no characters to set, just empty tags
	}

	PERFORMANCE_PROFILER_STOP(DX9Font_ParseTags);

	PERFORMANCE_PROFILER_START(DX9Font_ParseColor);
	
	// now we go thru again and set every individual character's color
	int rFactor, gFactor, bFactor, aFactor;
	UINT startR, startG, startB, startA, endR, endG,
		endB, endA, tempR, tempG, tempB, tempA, charInd;
	int colorsReadIndex = 0;

	DWORD tempColor = m_Color;
	rFactor = gFactor = bFactor = aFactor = 0;
	charInd = k;
	
	// default to black...
	startR = startG = startB = 0; 
	startA = 255;
	UINT check = colorsRead.size();
	for (k=0; k<firstPassChars.size(); k++) 
	{
		if (colorsRead.size() > 0)
		{
			if (colorsReadIndex >= 0)
			{
				if ((colorsRead[colorsReadIndex].startIndex <= k) && (colorsRead[colorsReadIndex].endIndex > k))
				{
					startR = colorsRead[colorsReadIndex].startR;
					startG = colorsRead[colorsReadIndex].startG;
					startB = colorsRead[colorsReadIndex].startB;
					startA = colorsRead[colorsReadIndex].startA;
					endR = colorsRead[colorsReadIndex].endR;
					endB = colorsRead[colorsReadIndex].endG;
					endG = colorsRead[colorsReadIndex].endB;
					endA = colorsRead[colorsReadIndex].endA;
					rFactor = colorsRead[colorsReadIndex].rFactor;
					gFactor = colorsRead[colorsReadIndex].gFactor;
					bFactor = colorsRead[colorsReadIndex].bFactor;
					aFactor = colorsRead[colorsReadIndex].aFactor;
					colorsReadIndex++;
					if (colorsReadIndex == colorsRead.size())
					{
						colorsReadIndex = -1;
					}					
				}
			}
		}

		tempA = (unsigned int)(startA + (aFactor * (k - charInd)));
		if (tempA > 255) tempA = 255;
		tempColor = tempA;
		tempColor = tempColor << 8;
		
		tempR = (unsigned int)(startR + (rFactor * (k - charInd)));
		if (tempR > 255) tempR = 255;
		tempColor += tempR;
		tempColor = tempColor << 8;

		tempG = (unsigned int)(startG + (gFactor * (k - charInd)));
		if (tempG > 255) tempG = 255;
		tempColor += tempG;
		tempColor = tempColor << 8;

		tempB = (unsigned int)(startB + (bFactor * (k - charInd)));
		if (tempB > 255) tempB = 255;
		tempColor += tempB;
				
		firstPassChars[k].color = tempColor;
	}

	PERFORMANCE_PROFILER_STOP(DX9Font_ParseColor);

	PERFORMANCE_PROFILER_START(DX9Font_SetParagraph);
	
	static TCHAR defaultdelimator[] = {' '};

	if (m_bWrap)
	{
		// OPTIMIZE!!! //
		if(	m_Faces[NORMAL] )
		{
			if( txt  == "" )
				return;
			m_Lines.clear();
			m_Text = "";
			for (k=0; k<firstPassChars.size(); k++)
			{
				m_Text += firstPassChars[k].data;
			}			
			int width = (int)(m_RRect.x2 - m_RRect.x1);
			string temp;		
			string accum;
			vector<taggableChar> tempChars;
			int curpixlength = 0;
			if( delimators == NULL )
			{
				delimators = defaultdelimator;
				numtokendelim = 1;
			}
			m_delimators = delimators;
			m_numdelimators = numtokendelim;
			bool newline = false;
			int i = 0;
			int starti = 0;
			while(i < (int)m_Text.length())
			{
				//find next token
				//check for delimators, tokens 
				bool check;
				do
				{
					check = false;
					for( int dcheck = 0; dcheck < numtokendelim; dcheck++ )
					{
						if( m_Text[ i ] == '\n' )
						{
							newline = true;
							check = true;
							i--;
							break;
						}
						if( m_Text[ i ] == delimators[ dcheck ] )
						{
							check = true;
							break;
						}
					}
					if( i == m_Text.length() )
					{
						break;
					}
					i++;
				}while( !check && newline == false);
				//we have a token, check to see if it fits
				temp = m_Text.substr(starti,i-starti); // 0 to i
				int substrpixellength = GetFontPixWidth((TCHAR*)temp.c_str());
				//check if it fit, add to accum if it does
				if( newline )
				{
					for (k=0; k<temp.length(); k++)
					{
						tempChars.push_back(firstPassChars[k+starti]);
					}
					m_Lines.push_back( tempChars );				
					accum = "";
					tempChars.clear();
					curpixlength = 0;
					newline = false; //reset newlines	
					i++;//skip newline character
				}else if( substrpixellength + curpixlength <= width )
				{
					curpixlength += substrpixellength;//create new length
					accum += temp; //concat string
					for (k=0; k<temp.length(); k++)
					{
						tempChars.push_back(firstPassChars[k+starti]);
					}
				}else
				{
					//write out current accum to line
					for (k=0; k<temp.length(); k++)
					{
						tempChars.push_back(firstPassChars[k+starti]);
					}
					m_Lines.push_back( tempChars );
					accum = temp;
					tempChars.clear();
					curpixlength = substrpixellength;
				}
				//now grab any whitespace or delimator space
				check = true; 				
				starti = i;
			}
			if( accum.length() > 0 )
			{
				m_Lines.push_back( tempChars );
			}
		}//if initialized and mface	
	}
	// not wrapping
	else
	{
		m_Lines.clear();
		m_Text = "";
		vector<taggableChar> tempChars;
		UINT size = firstPassChars.size();
		tempChars.reserve(size); // time saver
		for (k=0; k<firstPassChars.size(); k++)
		{
			m_Text += firstPassChars[k].data;
            if (firstPassChars[k].data == '\n')
			{
				m_Lines.push_back(tempChars);
				tempChars.clear();
				tempChars.reserve(size); // time saver
			}
			else
			{
				tempChars.push_back(firstPassChars[k]);
			}
		}
		if (tempChars.size() > 0)
		{
			m_Lines.push_back(tempChars);
		}
	}

	PERFORMANCE_PROFILER_STOP(DX9Font_SetParagraph);

	// TODO: set up the font texture with only the characters that we need

//	SetFontFace(NORMAL, m_hsNormFontFace.GetString(), m_PixHeight);
//	SetFontFace(BOLD, m_hsBoldFontFace.GetString(), m_PixHeight);
//	SetFontFace(ITALIC, m_hsItalicFontFace.GetString(), m_PixHeight);
//	SetFontFace(BOLDITALIC, m_hsBoldItalicFontFace.GetString(), m_PixHeight);	
}

int EE_CDX9Font::GetCharNum(const TCHAR * txt, char delimators[], int numtokendelim, float width)
{
	static TCHAR defaultdelimator[] = {' '};
	if(	m_Faces[NORMAL] )
	{
		if( txt  == "" )
			return 0;
//		m_Lines.clear();
		m_Text = txt;
//		int width = (int)(m_RRect.x2 - m_RRect.x1);
		string temp;		
		string accum;
		int curpixlength = 0;
		if( delimators == NULL )
		{
			delimators = defaultdelimator;
			numtokendelim = 1;
		}
		bool newline = false;
		int i = 0;
		int starti = 0;
		while(i < (int)m_Text.length())
		{
			//find next token
			//check for delimators, tokens 
			bool check;
			do
			{
				check = false;
				for( int dcheck = 0; dcheck < numtokendelim; dcheck++ )
				{
					if( m_Text[ i ] == '\n' )
					{
						newline = true;
					}
					if( m_Text[ i ] == delimators[ dcheck ] )
					{
						check = true;
						break;
					}
				}
				if( i == m_Text.length() )
				{
					break;
				}
				i++;
			}while( !check );
			//we have a token, check to see if it fits
			temp = m_Text.substr(starti,i-starti); // 0 to i
			int substrpixellength = GetFontPixWidth((TCHAR*)temp.c_str());
			//check if it fit, add to accum if it does
			if( substrpixellength + curpixlength <= width ||
				newline == true)
			{
				curpixlength += substrpixellength;//create new length
				accum += temp; //concat string
				if (newline) return accum.length();
			}else
			{
				//write out current accum to line
//				m_Lines.push_back( accum );
				accum = temp;
				return accum.length();				
				curpixlength = substrpixellength;
				newline = false; //reset newlines
			}
			//now grab any whitespace or delimator space
			check = true; 
			while( i < (int)m_Text.length() && check )
			{
				check = false;
				for( int dcheck = 0; dcheck < numtokendelim; dcheck++ )
				{
					if( m_Text[ i ] == '\n' )
					{
						newline = true;
					}
					if( m_Text[ i ] == delimators[ dcheck ] )
					{
						check = true;
						i++;
						break;
					}
				}
			}
			starti = i;
		}
		if( accum.length() > 0 )
		{
//			m_Lines.push_back( accum );
			return accum.length();
		}
	}//if initialized and mface
	return 0;
}

int EE_CDX9Font::GetNumLines()
{
	return m_Lines.size();
}
