///============================================================================
/// \file		CoordinateToolShaderCallback.cpp
/// \brief		Implementation for CCoordinateToolShaderCallback class
/// \date		10-31-2005
/// \author		Josh Lulewicz
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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CCoordinateToolShaderCallback::CCoordinateToolShaderCallback()
{
	// grab the tool box
	m_ToolBox = EngineGetToolBox();
	// init member variables
	m_ShaderCallbackName = NULL;

	// grab the render interface
	GETRENDERERINTERFACEPARAMS renderInterface;

	DWORD msgHash_GetRendererInterface = CHashString(_T("GetRendererInterface")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetRendererInterface, 
						   sizeof(GETRENDERERINTERFACEPARAMS), 
						   &renderInterface);

	m_Renderer = renderInterface.m_RendererInterface;

	m_ShaderCallbackName = new CHashString(_T("CoordinateToolShaderUpdate"));

	// register the shader callback
	ADDSHADERCALLBACKMESSAGE ascb;

	ascb.m_ShaderCallback = this;

	static DWORD msgHash_SetShaderCallBack = CHashString(_T("SetShaderCallBack")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetShaderCallBack, sizeof(IShaderCallback*), &ascb);
}

CCoordinateToolShaderCallback::~CCoordinateToolShaderCallback()
{
	// register the shader callback
	ADDSHADERCALLBACKMESSAGE ascb;

	ascb.m_ShaderCallback = this;

	static DWORD msgHash_RemoveShaderCallBack = CHashString(_T("RemoveShaderCallBack")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_RemoveShaderCallBack, sizeof(IShaderCallback*), &ascb);

	if (m_ShaderCallbackName)
	{
		delete m_ShaderCallbackName;
		m_ShaderCallbackName = NULL;
	}
}

void CCoordinateToolShaderCallback::ShaderCallback(IShader *shader, float time)
{

}

IHashString* CCoordinateToolShaderCallback::GetCallbackName()
{
	return m_ShaderCallbackName;
}