///============================================================================
/// \file		TextureComponent.cpp
/// \brief		Header file for Texture Manager to hook up to Elemental Engine
/// \date		04-22-2005
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
#include "TextureComponent.hpp"

// registers ourselves with Elemental Engine, so we get messages
// it's not really a singleton to us, but to EE it is.
REGISTER_COMPONENT_SINGLETON( CTextureComponent );

// the messages we receive
REGISTER_MESSAGE_HANDLER(CreateTextureThumbnail, OnCreateTextureThumbnail, CTextureComponent);
REGISTER_MESSAGE_HANDLER(WindowsCmdMsg, OnCmdMsg, CTextureComponent);
REGISTER_MESSAGE_HANDLER(WindowsMainCreate, OnCreate, CTextureComponent);
REGISTER_MESSAGE_HANDLER(FileLoaded_dds, OnLoadedTexture, CTextureComponent);
REGISTER_MESSAGE_HANDLER(FileLoaded_tga, OnLoadedTexture, CTextureComponent);
REGISTER_MESSAGE_HANDLER(FileLoaded_bmp, OnLoadedTexture, CTextureComponent);
REGISTER_MESSAGE_HANDLER(FileLoaded_png, OnLoadedTexture, CTextureComponent);
REGISTER_MESSAGE_HANDLER(GlobalUpdateTick, OnGlobalUpdateTick, CTextureComponent);
REGISTER_MESSAGE_HANDLER(CreateThumbnail_dds, OnCreateTextureThumbnail, CTextureComponent);
REGISTER_MESSAGE_HANDLER(CreateThumbnail_tga, OnCreateTextureThumbnail, CTextureComponent);
REGISTER_MESSAGE_HANDLER(CreateThumbnail_bmp, OnCreateTextureThumbnail, CTextureComponent);
REGISTER_MESSAGE_HANDLER(CreateThumbnail_png, OnCreateTextureThumbnail, CTextureComponent);

CTextureComponent::CTextureComponent()
{
	m_ToolBox = EngineGetToolBox();
	
	m_Parent = NULL;
}

CTextureComponent::~CTextureComponent()
{

}

// we don't want elemental or anybody else creating us.
IComponent *CTextureComponent::Create( int nArgs, va_list argptr )
{
	return SINGLETONINSTANCE(CTextureComponent);
}

// serialize function for this object
void CTextureComponent::Serialize(IArchive &ar)
{
}

// get component name for this object
IHashString *CTextureComponent::GetComponentType()
{
	static CHashString compName(_T("CTextureComponent"));
	return &compName;
}

bool CTextureComponent::IsKindOf(IHashString *compType)
{
	return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
}

// constructor for Example component (interface to Elemental Engine)
void CTextureComponent::SetParent(CTextureEditor *parent)
{
	m_Parent = parent;
}


// message to handle OnCmdMsgs from main window
DWORD CTextureComponent::OnCmdMsg(DWORD size, void *param)
{
	CMDMESSAGE *pCmdMsg;
	VERIFY_MESSAGE_SIZE(size, sizeof(CMDMESSAGE));
	pCmdMsg = (CMDMESSAGE *)param;

	// forward to parent.
	if (m_Parent->OnCmdMsg(pCmdMsg->m_nID, pCmdMsg->m_nCode, pCmdMsg->m_pExtra,
		pCmdMsg->m_pHandlerInfo))
	{
		pCmdMsg->retVal = true;
		return MSG_HANDLED_PROCEED;
	}

	// it wasn't handled by parent, so return that
	return MSG_NOT_HANDLED;
}

// message to handle WindowsCreate function
DWORD CTextureComponent::OnCreate(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(CMDIFrameWnd *));
	m_Parent->OnCreate((CMDIFrameWnd *)param);

	return MSG_HANDLED_PROCEED;
}

DWORD CTextureComponent::OnCreateTextureThumbnail(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(CREATETHUMBNAILPARAMS));
	CREATETHUMBNAILPARAMS * ctp = (CREATETHUMBNAILPARAMS *)param;

	// this will be the source picture
	auto_ptr<CTextureDoc> lDoc(dynamic_cast<CTextureDoc *>(CTextureDoc::CreateObject()));
	lDoc->TargetOpenDocument(ctp->lpszPathName, true);
	
	// temp object for the original image to get its parameters
	BITMAP bitmap;

	// get the bitmap and its parameters into the temp object
	lDoc->m_ThumbNail.GetBitmap(&bitmap);

	// let's create a buffer for the thumbnail
	size_t bufSize = bitmap.bmWidth * bitmap.bmHeight * bitmap.bmBitsPixel/8;
	auto_ptr<char> tempBuffer(new char[bufSize]);

	// get the source image into the temp object 
	lDoc->m_ThumbNail.GetBitmapBits(bufSize, tempBuffer.get());

	//Let's create the new thumbnail object that will be returned
	ctp->pThumbnail = new CBitmap();
	
	ctp->pThumbnail->CreateBitmap(bitmap.bmWidth, bitmap.bmHeight, bitmap.bmPlanes, bitmap.bmBitsPixel, tempBuffer.get()); 
	return MSG_HANDLED_PROCEED;
}

DWORD CTextureComponent::OnLoadedTexture(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(LPTSTR));
	LPTSTR name = (LPTSTR)param;
	m_Parent->OpenTexture( name );
	return MSG_HANDLED_PROCEED;
}

DWORD CTextureComponent::OnGlobalUpdateTick(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(GLOBALTICKPARAMS));

//	m_Parent->m_TextureEditorCmdTarget.OnToolsAppertureGenerator();

	return MSG_HANDLED_PROCEED;
}
