///============================================================================
/// \file	CAnimatedTexture.cpp
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
#include "CAnimatedTexture.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT(CAnimatedTextureObject);

///
/// Constructor / Destructor
///

CAnimatedTextureObject::CAnimatedTextureObject(IHashString *parentName, IHashString *name) : 
	OBJECTTEMPLATE( CTextureManager, CAnimatedTextureObject, IAnimatedTextureObject, parentName, name )
{

	DeInit();
	
	// grab the render interface
	GETRENDERERINTERFACEPARAMS renderInterface;
	static DWORD msgHash_GetRendererInterface = CHashString(_T("GetRendererInterface")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_GetRendererInterface, sizeof(GETRENDERERINTERFACEPARAMS), &renderInterface);
	m_Renderer = renderInterface.m_RendererInterface;
}

CAnimatedTextureObject::~CAnimatedTextureObject()
{
	DeInit();
}

void CAnimatedTextureObject::Serialize(IArchive &ar)
{
	//create
	if (ar.IsReading())
	{		
		//load data
		//format should be number of frames and textures associated with frames. a
		ar.Read( m_NumFrames, "NumFrames" );
		ar.Read( m_fFrameRate, "FrameRate" );
		ar.Read( m_NumTextures, "NumTextures" );
		ar.Read( m_Width, "FrameWidth" );
		ar.Read( m_Height, "FrameHeight" );
		for( UINT i = 0; i < m_NumTextures; i++ )
		{
			//read texture name
			StdString filename;
			ar.Read( filename, "TextureFile" );
			m_TextureFilenames.push_back( filename );
		}
		BuildUVs();
	}
	else
	{
		//output
		//format should be number of frames and textures associated with frames. a
		ar.Write( m_NumFrames, "NumFrames" );
		ar.Write( m_fFrameRate, "FrameRate" );
		ar.Write( m_NumTextures, "NumTextures" );
		ar.Write( m_Width, "FrameWidth" );
		ar.Write( m_Height, "FrameHeight" );
		for( UINT i = 0; i < m_NumTextures; i++ )
		{
			//read texture name
			ar.Write( m_TextureFilenames[ i ], "TextureFile" );		
		}
	}
}

/// \brief The Initialization for the Object
/// \return pointer to an IComponent
IComponent *CAnimatedTextureObject::Create(int nArgs, va_list argptr)
{
	IHashString *name;
	IHashString *parentName;
	IObject *self;

	name = va_arg(argptr, IHashString *);
	parentName = va_arg(argptr, IHashString *);
	// create the object however you must create it
	try
	{
		// try and create an instance
		self = (IObject*)new CAnimatedTextureObject(parentName, name);
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

/// \brief Loads a texture
/// \param file = the filename
bool CAnimatedTextureObject::LoadFromFile( const TCHAR * file )
{
	DeInit();

	DWORD retVal;
	IArchive *Archive = NULL;
	CHashString streamType(_T("File"));
	
	CREATEARCHIVE ca;
	ca.streamData = (void *)file;
	ca.mode = STREAM_MODE_READ;
	ca.streamType = &streamType;
	// call the Archive factory to create an XML archive
	static DWORD msgHash_CreateXMLArchive = CHashString(_T("CreateXMLArchive")).GetUniqueID();
	if (retVal = m_ToolBox->SendMessage(msgHash_CreateXMLArchive, sizeof(CREATEARCHIVE), &ca) != MSG_HANDLED)
	{
		return false;
	}
	Archive = ca.archive;
	Serialize( *Archive );
	Archive->Close();

	return false;
}


UINT CAnimatedTextureObject::GetNumFrames()
{
	return m_NumFrames;
}

void CAnimatedTextureObject::GetFrame( UINT frame, float &u1, float &v1, float &u2, float &v2, ITextureObject **out )
{
	if( frame >= 0 && frame < m_FrameUVs.size() )
	{
		u1 = m_FrameUVs[ frame ].u1;
		u2 = m_FrameUVs[ frame ].u2;
		v1 = m_FrameUVs[ frame ].v1;
		v2 = m_FrameUVs[ frame ].v2;
		if( out )
		{
			UINT index = m_FrameUVs[ frame ].m_TextureIndex;
			*out = m_Texture[ index ];
		}
	}
}

UINT CAnimatedTextureObject::GetHeight()
{
	return m_Height;
}

UINT CAnimatedTextureObject::GetWidth()
{
	return m_Width;
}

UINT CAnimatedTextureObject::GetColorDepth()
{
	return m_ColorDepth;
}

bool CAnimatedTextureObject::IsRenderTarget()
{
	return false;
}

void* CAnimatedTextureObject::Read(UINT frame)
{
	return NULL;
}

bool CAnimatedTextureObject::Write( UINT frame, void *p )
{
	
	return false;
}

void * CAnimatedTextureObject::Lock( UINT frame, int * pitch )
{
	return NULL;
}

void CAnimatedTextureObject::Unlock()
{

}

///
/// Init / Update / DeInit
///
void CAnimatedTextureObject::Init()
{
	m_Filename = "";
	m_RefCount = 1;
	m_NumFrames = 0;
	m_NumTextures = 0;
	
	m_Height = 0;
	m_Width = 0;
	m_ColorDepth = 0;
}

void CAnimatedTextureObject::DeInit()
{	
	m_Texture.clear();
	m_FrameUVs.clear();

	m_Filename = "";
	m_RefCount = 1;
	m_NumTextures = 0;
	m_NumFrames = 0;

	m_Height = 0;
	m_Width = 0;
	m_ColorDepth = 0;
}

unsigned int CAnimatedTextureObject::GetReferenceCount()
{
	return m_RefCount;
}

IHashString *CAnimatedTextureObject::GetTextureName()
{
	return &m_Filename;

}

void CAnimatedTextureObject::SetTextureName(IHashString *name)
{
	m_Filename = name;

}

void CAnimatedTextureObject::BuildUVs()
{
	if( m_Texture.size() > 0 )
	{
		for( UINT i = 0; i < m_Texture.size(); i++ )
		{
			m_Texture[ i ]->DecrementRefCount();
		}
		m_Texture.clear();
	}

	TEXTUREOBJECTPARAMS tobj;		
	UINT numtex =  m_NumTextures;
	m_Texture.resize( numtex );
	m_FrameUVs.clear();
	for( UINT i = 0; i < numtex; i++ )
	{
		//load the texture
		CHashString filename( m_TextureFilenames[ i ] );
		tobj.bLoad = true;
		tobj.Name = &filename;
		tobj.TextureObjectInterface = NULL;
		static DWORD msgHash_AddTexture = CHashString(_T("AddTexture")).GetUniqueID();
		DWORD res = EngineGetToolBox()->SendMessage(msgHash_AddTexture, sizeof(tobj), &tobj );
		if( res == MSG_HANDLED )
		{
			// store the texture id in the user data of the map
			m_Texture[ i ] = (ITextureObject *)tobj.TextureObjectInterface;
		}else
		{
			m_Texture[ i ] = NULL;
		}
		if( m_Texture[ i ] )
		{			
			//we go through the frames and build UVs based on our height and the current texture's height
			float texwidth = (float)m_Texture[ i ]->GetWidth();
			float texheight = (float)m_Texture[ i ]->GetHeight();
			//divide for uvs
			int numcols = ((int)texwidth)/m_Width;
			int numrows = ((int)texheight)/m_Height;
			float incrementV = 1.f/(float)numcols;
			float incrementU = 1.f/(float)numrows;
			for( int cols = 0; cols < numcols; cols++ )
			{
                for( int rows = 0; rows < numrows; rows++ )
				{
					UVRect r;
					r.v1 = ((float)cols)*incrementV;
					r.v2 = ((float)cols)*incrementV + incrementV;
					r.u1 = ((float)rows)*incrementU;
					r.u2 = ((float)rows)*incrementU + incrementU;
					r.m_TextureIndex = i;
					m_FrameUVs.push_back( r );
				}
			}
		}
	}
}
