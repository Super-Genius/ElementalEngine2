///============================================================================
/// \file		TextureEditorCmdTarget.h
/// \brief		Implementation of Command Target for Texture Editor
/// \date		08-27-2007
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
#include "TextureEditor.h"
#include "TextureEditorCmdTarget.h"

IMPLEMENT_DYNAMIC(CTextureEditorCmdTarget, CCmdTarget)
CTextureEditorCmdTarget::CTextureEditorCmdTarget()
{
	m_pEditor = SINGLETONINSTANCE(CTextureEditor);
}

CTextureEditorCmdTarget::~CTextureEditorCmdTarget()
{
}


BEGIN_MESSAGE_MAP(CTextureEditorCmdTarget, CCmdTarget)
	ON_COMMAND(ID_TEXTEDIT_APPERTURE, OnToolsAppertureGenerator)
	ON_UPDATE_COMMAND_UI(ID_TEXTEDIT_APPERTURE, OnUpdateAppertureGenerator)
END_MESSAGE_MAP()


void CTextureEditorCmdTarget::OnToolsAppertureGenerator()
{
	CMDIFrameWnd *mainFrame = DYNAMIC_DOWNCAST(CMDIFrameWnd, AfxGetMainWnd());
	CMDIChildWnd *childFrame = mainFrame->MDIGetActive();
	CTextureDoc *pTextureDoc = DYNAMIC_DOWNCAST(CTextureDoc, childFrame->GetActiveDocument());
	if (!pTextureDoc)
		return;
	IBaseTextureObject *pDocTexture = pTextureDoc->m_pTextureObject;
	if (!pDocTexture)
		return;

	CHashString hszName(_T("AppertureGenObject"));
	CHashString hszType(_T("CAppertureGenRenderObject"));

	// create a CAppertureGenRenderObject
	CREATEOBJECTPARAMS cop;
	cop.name = &hszName;
	cop.parentName = NULL;
	cop.typeName = &hszType;
	static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
	if (EngineGetToolBox()->SendMessage(msgHash_CreateObject, sizeof(cop), &cop ) != MSG_HANDLED)
	{
		// log error
		return;
	}
	INITOBJECTPARAMS iop;
	iop.name = &hszName;
	static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
	if (EngineGetToolBox()->SendMessage(msgHash_InitObject, sizeof(iop), &iop ) != MSG_HANDLED)
	{
		// log error
		return;
	}

	TCHAR newPath[_MAX_PATH];
	TCHAR newName[_MAX_FNAME];
	_tsplitpath( pDocTexture->GetName()->GetString(), NULL, newPath, newName, NULL);
	StdString szDestTextureName( newPath );
	szDestTextureName += newName;
	szDestTextureName += _T("_aperture.tga");
	CHashString hszDestTextureName( szDestTextureName );

	GENERATEAPPERTURETEXTUREPARAMS gatp;
	gatp.pSourceTexture = pDocTexture;
	gatp.pDestTextureName = &hszDestTextureName;
	gatp.iDestTextureSize = 512;
	gatp.fOffset = 0.05f;
	gatp.fFlatten = 3.5f;
	static DWORD msgHash_GenerateAppertureTexture = CHashString(_T("GenerateAppertureTexture")).GetUniqueID();
	if (EngineGetToolBox()->SendMessage(msgHash_GenerateAppertureTexture, sizeof(gatp), &gatp, &hszName ) != MSG_HANDLED)
	{
		// log error
	}

	// destroy CAppertureGenRenderObject
	DELETEOBJECTPARAMS dop;
	dop.name = cop.name;
	static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_DeleteObject, sizeof(dop), &dop);
}

void CTextureEditorCmdTarget::OnUpdateAppertureGenerator(CCmdUI *pCmdUI)
{

}
