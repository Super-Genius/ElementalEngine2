///============================================================================
/// \file		TimeOfDayShaderCallback.cpp
/// \brief		Implementation for CTimeOfDayShaderCallback class
/// \date		10-31-2005
/// \author		Josh Lulewicz
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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CTimeOfDayShaderCallback::CTimeOfDayShaderCallback()
{
	// grab the tool box
	m_ToolBox = EngineGetToolBox();
	// init member variables
	m_ShaderCallbackName = NULL;

	// grab the render interface
	GETRENDERERINTERFACEPARAMS renderInterface;

	static DWORD msgHash_GetRendererInterface = CHashString(_T("GetRendererInterface")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetRendererInterface, 
						   sizeof(GETRENDERERINTERFACEPARAMS), 
						   &renderInterface);

	m_Renderer = renderInterface.m_RendererInterface;

	m_ShaderCallbackName = new CHashString(_T("TimeOfDayShaderUpdate"));

	// register the shader callback
	ADDSHADERCALLBACKMESSAGE ascb;

	ascb.m_ShaderCallback = this;

	static DWORD msgHash_SetShaderCallBack = CHashString(_T("SetShaderCallBack")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetShaderCallBack, sizeof(IShaderCallback*), &ascb);
}

CTimeOfDayShaderCallback::~CTimeOfDayShaderCallback()
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

void CTimeOfDayShaderCallback::ShaderCallback(IShader *shader, float time)
{
	TIMEOFDAYDESC tod;
	Vec3 sunDir;

	tod.vecSunDir = &sunDir;
	static DWORD msgHash_GetTimeOfDayDesc = CHashString(_T("GetTimeOfDayDesc")).GetUniqueID();
	static CHashString hsTimeOfDayType(_T("CTimeOfDayObject"));
	if (EngineGetToolBox()->SendMessage(msgHash_GetTimeOfDayDesc, sizeof(tod), &tod, NULL, &hsTimeOfDayType ) != MSG_HANDLED)
	{
		// set default tod values
		sunDir.Set( -0.25f, -0.15f, 1.0f );
		tod.fTime = 0.5f;
		tod.bFog = false;
		tod.bLighting = false;
	}

	sunDir.Normalize();

	float dir[4]; // = &shader->vprog_custom_param[0].param[0]; 
    dir[0] = sunDir.x;
    dir[1] = sunDir.y;
    dir[2] = sunDir.z;
	dir[3] = tod.fTime; // y-lookup coordinate

	shader->SetConstant(48, dir[0], dir[1], dir[2], dir[3]);

    // register c[49] : track sun direction in eyespace (models)
    float eyemat[16]; // = Vision::World.GetGLWorldMatrix();
	m_Renderer->GetMatrix(VIEW_MATRIX, eyemat);
    float eyedir[4]; // = &shader->vprog_custom_param[1].param[0];
    eyedir[0] = dir[0]*eyemat[0] + dir[1]*eyemat[4] + dir[2]*eyemat[8];
    eyedir[1] = dir[0]*eyemat[1] + dir[1]*eyemat[5] + dir[2]*eyemat[9];
    eyedir[2] = dir[0]*eyemat[2] + dir[1]*eyemat[6] + dir[2]*eyemat[10];
    eyedir[3] = dir[3]; // y-lookup coordinate

	shader->SetConstant(49, eyedir[0], eyedir[1], eyedir[2], eyedir[3]);

	// register c[50]
    float flags[2]; // = &shader->vprog_custom_param[2].param[0];
	flags[0] = (tod.bFog) ? 1.0f : 0.0f;      // fog
	flags[1] = (tod.bLighting) ? 1.0f : 0.0f; // lighting

	shader->SetConstant(50, flags[0], flags[1], 0, 0);
}

IHashString* CTimeOfDayShaderCallback::GetCallbackName()
{
	return m_ShaderCallbackName;
}