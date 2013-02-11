///============================================================================
/// \file	CFontManager.cpp
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
#include "CFontManager.h"
#define DEFAULTFONTVERTSIZE 24
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//void InitFontImplementation();
//void DeInitFontImplementation();

REGISTER_COMPONENT_SINGLETON( CFontManager );

REGISTER_MESSAGE_HANDLER( CreateFont, OnCreateFont, CFontManager );
REGISTER_MESSAGE_HANDLER( DeleteFont, OnDeleteFont, CFontManager );
REGISTER_MESSAGE_HANDLER( GetFontPixWidth, OnGetFontPixWidth, CFontManager );
REGISTER_MESSAGE_HANDLER( RenderUpdate, OnRenderUpdate, CFontManager );
REGISTER_MESSAGE_HANDLER( SetFontImplementation, SetFontImplementation, CFontManager );
REGISTER_MESSAGE_HANDLER_PRIORITY( DestroyRenderer, OnDestroyRenderer, CFontManager, 0x70000000 );

REGISTER_MESSAGE_HANDLER( AddFontGlyphTexFilename, OnAddFontGlyphTexFilename, CFontManager );
REGISTER_MESSAGE_HANDLER( GetFontGlyphTexture, OnGetFontGlyphTexture, CFontManager );

/// \brief The Default Constructor for CFontManager
/// \return void
CFontManager::CFontManager() : 
	CManager(_T("CFontManager"), FONTPRIORITY )
{
	m_Dynamic2DVertexBuffer = NULL;
	m_FontVertexSize = DEFAULTFONTVERTSIZE;
	m_FontImplementation = NULL;
//	InitFontImplementation();
}


/// \brief	Function to get this component.  This is a singleton
/// \param	void
/// \return	IComponent*: This Manager
IComponent *CFontManager::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE( CFontManager );
}

/// \brief The Default Destructor for CFontManager
/// \return void

CFontManager::~CFontManager( )
{
	DeInit();
}

// Initializes the manager
void CFontManager::InitFonts()
{
	//Initialize 2D vertex buffer
	//create new allocation
	CHANNELDESCRIPTORLIST VertexFormatFonts;
	VertexFormatFonts.push_back( MESH_STREAM_VERTEX_POSITION3 );
	VertexFormatFonts.push_back( MESH_STREAM_VERTEX_TEXTURECOORD1 );
	VertexFormatFonts.push_back( MESH_STREAM_VERTEX_COLORCRGBA );

	ALLOCATERENDERBUFFERMSG msg;	
	UINT Max2DAllocation = MAX_FONT_BUFFER;
	msg.ElementsToAllocate = Max2DAllocation;
	msg.SizeVertexFormat = m_FontVertexSize;
	msg.VertexFormat = VertexFormatFonts;
	msg.Dynamic = true;
	msg.m_AllowSharedBuffer = false;

	static DWORD msgHash_AllocateVertices = CHashString(_T("AllocateVertices")).GetUniqueID();
	DWORD res = EngineGetToolBox()->SendMessage(msgHash_AllocateVertices, sizeof( msg ), &msg, NULL, NULL );

	if( res == MSG_HANDLED )
	{
		m_Dynamic2DVertexBufferAllocation.m_Offset = msg.m_ReturnOffset;
		m_Dynamic2DVertexBufferAllocation.m_AllocationHandle = msg.m_ReturnHandle;
		m_Dynamic2DVertexBufferAllocation.m_InterfaceHandle = msg.m_ReturnInterfaceHandle;
		m_Dynamic2DVertexBufferAllocation.m_Size = Max2DAllocation;
		m_Dynamic2DVertexBufferAllocation.m_Stride = msg.SizeVertexFormat;
		m_Dynamic2DVertexBuffer = (IVertexBufferObject*)msg.m_InterfacePointer;
	}		

	ALLOCATERENDERBUFFERMSG indexMsg;
	indexMsg.ElementsToAllocate = Max2DAllocation;

	static DWORD msgHash_AllocateIndices = CHashString(_T("AllocateIndices")).GetUniqueID();
	res = m_ToolBox->SendMessage(msgHash_AllocateIndices, sizeof( indexMsg ), &indexMsg, NULL, NULL );
	if( res == MSG_HANDLED )
	{
		m_Dynamic2DIndexBufferAllocation.m_Offset = indexMsg.m_ReturnOffset;
		m_Dynamic2DIndexBufferAllocation.m_AllocationHandle = indexMsg.m_ReturnHandle;
		m_Dynamic2DIndexBufferAllocation.m_InterfaceHandle = indexMsg.m_ReturnInterfaceHandle;
		m_Dynamic2DIndexBufferAllocation.m_Size = Max2DAllocation;
		m_Dynamic2DIndexBufferAllocation.m_Stride = sizeof(short);
		m_Dynamic2DIndexBuffer = (IIndexBuffer*)indexMsg.m_InterfacePointer;		
	}	

	else
	{	
		//return false;
	}
}
/// \brief	Update function for the CFontManager
///			Renders immediately, the queued list of renderobjects
/// \return	void
void CFontManager::Update(  DWORD tickCount  )
{	
	set<IFontObject*>::iterator objIter;

	//Delete all released fonts:	
	if( m_FontObjectList.size() > 0 )
	{		
		//call font object's update function
		// find name in obj list
		IFontObject * pObject;
		for( objIter = m_FontObjectList.begin();objIter != m_FontObjectList.end(); )
		{
			pObject = (*objIter);
			if( !pObject->Update() )
			{
				objIter = m_FontObjectList.erase( objIter );
			}
			else
			{
				++objIter;
			}
		}
		//reloop through available systems and lock for render later on
		if( m_Dynamic2DVertexBuffer )
		{
			void * pDest = NULL;
			int curoffset = m_Dynamic2DVertexBufferAllocation.m_Offset;
			
			m_Dynamic2DVertexBuffer->Lock( m_Dynamic2DVertexBufferAllocation.m_Offset,
											m_Dynamic2DVertexBufferAllocation.m_Size,
											(void**)&pDest,
											VBO_LOCK_DISCARD
											);
			for( objIter = m_FontObjectList.begin();objIter != m_FontObjectList.end(); ++objIter )
			{
				pObject = (*objIter);
				curoffset += pObject->UpdateBuffer( (char**)&pDest, curoffset, m_Dynamic2DVertexBuffer );
			}
			m_Dynamic2DVertexBuffer->Unlock();//Don't really have to unlock I don't think since lock is full featured
		}		

		if( m_Dynamic2DIndexBuffer )
		{
			void * pDest = NULL;
			int curoffset = m_Dynamic2DIndexBufferAllocation.m_Offset;
			
			m_Dynamic2DIndexBuffer->Lock( m_Dynamic2DIndexBufferAllocation.m_Offset,
											m_Dynamic2DIndexBufferAllocation.m_Size,
											(void**)&pDest
											);
			for( objIter = m_FontObjectList.begin();objIter != m_FontObjectList.end(); ++objIter )
			{
				pObject = (*objIter);
				curoffset += pObject->UpdateIndexBuffer( (char**)&pDest, curoffset, m_Dynamic2DIndexBuffer );
			}
			m_Dynamic2DIndexBuffer->Unlock();//Don't really have to unlock I don't think since lock is full featured
		}	
	}
}

/// \brief	DeInitializes the CFontManager
/// \return	bool: Success
bool CFontManager::DeInit( void )
{
	if (m_FontObjectList.size() > 0)
	{
		set<IFontObject*>::iterator fontIter = m_FontObjectList.begin();
		while (fontIter != m_FontObjectList.end())
		{
			IFontObject *pFontObject = *fontIter;
			delete pFontObject;
			fontIter++;
		}
		m_FontObjectList.clear();
	}

	//ClearGlyphTextures();

//	DeInitFontImplementation();
	return CManager::DeInit();
}


DWORD CFontManager::OnCreateFont( DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE( size, sizeof( CREATEFONTINTERFACE ) );
	CREATEFONTINTERFACE * msg = (CREATEFONTINTERFACE*)params;

	msg->m_FontInterface = NULL;
	if( m_FontImplementation )
	{
		msg->m_FontInterface = m_FontImplementation->CreateFont();;
	}
	if( msg->m_FontInterface == NULL )
	{
		m_ToolBox->Log( LOGERROR, _T("Could not create font object\n") );
		return MSG_ERROR;
	}
	//add to list
	m_FontObjectList.insert( (IFontObject*)(msg->m_FontInterface) );

	//check VB
	if( !m_Dynamic2DVertexBuffer )
	{
		InitFonts();
	}

	return MSG_HANDLED_STOP;
}

DWORD CFontManager::OnDeleteFont(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE( size, sizeof( CREATEFONTINTERFACE ) );
	CREATEFONTINTERFACE msg = *(CREATEFONTINTERFACE*)param;
	
	set<IFontObject*>::iterator fontIter;

	fontIter = m_FontObjectList.find(msg.m_FontInterface);
	if (fontIter == m_FontObjectList.end())
	{
		return MSG_NOT_HANDLED;
	}
	
	IFontObject *obj = (*fontIter);
	delete obj;
	m_FontObjectList.erase(fontIter);

	return MSG_HANDLED_STOP;
}

DWORD CFontManager::OnGetFontPixWidth(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(FONTPIXWIDTHQUERY));
	FONTPIXWIDTHQUERY *fpwq = (FONTPIXWIDTHQUERY*) param;
	//TODO: fpwq->iWidth = EE_CDX9Font::GetFontPixWidth(CHashString(fpwq->szFilename), fpwq->iFontsize, fpwq->szText);
	if (m_FontImplementation)
	{
		fpwq->iWidth = m_FontImplementation->GetFontPixWidth(fpwq->szFilename, fpwq->iFontsize, fpwq->szText);
	}
	return MSG_HANDLED_STOP;	
}

DWORD CFontManager::OnRenderUpdate(DWORD size, void *param)
{
	Update(0);
	return MSG_HANDLED_PROCEED;
}

DWORD CFontManager::SetFontImplementation( DWORD size, void * params )
{
	VERIFY_MESSAGE_SIZE(size, sizeof(FONTIMPLEMENTATIONMSG));
	FONTIMPLEMENTATIONMSG * p = (FONTIMPLEMENTATIONMSG*)params;
	if( p && p->impl )
	{
		m_FontImplementation = p->impl;
	}
	return MSG_HANDLED_STOP;
}

DWORD CFontManager::OnDestroyRenderer(DWORD size, void *param)
{
	if (m_FontObjectList.size() > 0)
	{
		set<IFontObject*>::iterator fontIter = m_FontObjectList.begin();
		while (fontIter != m_FontObjectList.end())
		{
			IFontObject *pFontObject = *fontIter;
			delete pFontObject;
			fontIter++;
		}
		m_FontObjectList.clear();
	}
	return MSG_HANDLED_PROCEED;
}

void CFontManager::ClearGlyphTextures()
{
	GLYPHTEXTUREMAP::iterator mapIter = m_FontGlyphTextureMap.begin();
	TEXTUREOBJECTPARAMS top;
	for (; mapIter != m_FontGlyphTextureMap.end(); mapIter++)
	{
		top.TextureObjectInterface = mapIter->second;
		static DWORD msgRemoveTexture = CHashString(_T("RemoveTexture")).GetUniqueID();
		m_ToolBox->SendMessage(msgRemoveTexture, sizeof(TEXTUREOBJECTPARAMS), &top);
	}

	m_FontGlyphTextureMap.clear();
}

DWORD CFontManager::OnAddFontGlyphTexFilename(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(ADDFONTGLYPHFILENAMEPARAMS));
	ADDFONTGLYPHFILENAMEPARAMS afgfp = *(ADDFONTGLYPHFILENAMEPARAMS*)param;

	if ((afgfp.hsFile) && (afgfp.hsKey))
	{
		TEXTUREOBJECTPARAMS top;
		top.bLoad = true;
		top.Name = afgfp.hsFile;
		static DWORD msgAddTexture = CHashString(_T("AddTexture")).GetUniqueID();
		m_ToolBox->SendMessage(msgAddTexture, sizeof(TEXTUREOBJECTPARAMS), &top);
		IBaseTextureObject *tex = top.TextureObjectInterface;

		if (tex)
		{
			m_FontGlyphTextureMap[afgfp.hsKey->GetUniqueID()] = tex;
		}
	}

	return MSG_HANDLED_PROCEED;
}

DWORD CFontManager::OnGetFontGlyphTexture(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(GETFONTGLYPHTEXTUREPARAMS));
	GETFONTGLYPHTEXTUREPARAMS *gfgtp = (GETFONTGLYPHTEXTUREPARAMS*)param;

	if ((gfgtp) && (gfgtp->hsKey))
	{
		gfgtp->pTexture = m_FontGlyphTextureMap[gfgtp->hsKey->GetUniqueID()];
	}
	return MSG_HANDLED_STOP;
}