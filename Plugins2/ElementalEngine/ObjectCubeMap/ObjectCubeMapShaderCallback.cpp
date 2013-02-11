///============================================================================
/// \file		ObjectCubeMapShaderCallback.cpp
/// \brief		Implementation of Object Cube Map Shader Callback class
/// \date		10-04-2005
/// \author		Josh Lulewicz
/// \note		Copyright (c) 2005 Signature Devices, Inc.
///============================================================================

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//REGISTER_COMPONENT(CObjectCubeMapShaderCallback);

CObjectCubeMapShaderCallback::CObjectCubeMapShaderCallback()
{
	// grab the tool box
	m_ToolBox = EngineGetToolBox();
	// init member variables
	m_ShaderCallbackName = NULL;
	m_fFadeDistance = 1.0f / 200.0f;

	// grab the render interface
	GETRENDERERINTERFACEPARAMS renderInterface;

	m_ToolBox->SendMessage(_T("GetRendererInterface"), 
						   sizeof(GETRENDERERINTERFACEPARAMS), 
						   &renderInterface);

	m_Renderer = renderInterface.m_RendererInterface;

	m_ShaderCallbackName = new CHashString(_T("ObjectCubeMapUpdate"));

	// register the shader callback
	ADDSHADERCALLBACKMESSAGE ascb;

	ascb.m_ShaderCallback = this;

	m_ToolBox->SendMessage(_T("SetShaderCallBack"), sizeof(IShaderCallback*), &ascb);
}

CObjectCubeMapShaderCallback::~CObjectCubeMapShaderCallback()
{
	// register the shader callback
	ADDSHADERCALLBACKMESSAGE ascb;

	ascb.m_ShaderCallback = this;

	m_ToolBox->SendMessage(_T("RemoveShaderCallBack"), sizeof(IShaderCallback*), &ascb);

	if (m_ShaderCallbackName)
	{
		delete m_ShaderCallbackName;
		m_ShaderCallbackName = NULL;
	}
}

void CObjectCubeMapShaderCallback::ShaderCallback(IShader *shader, float time)
{
	TIMEOFDAYDESC tod;
	Vec3 sunDir;

	tod.vecSunDir = &sunDir;
	if (EngineGetToolBox()->SendMessage( _T("GetTimeOfDayDesc"), sizeof(tod), &tod, NULL, &CHashString(_T("CTimeOfDayObject")) ) != MSG_HANDLED)
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

	shader->SetVertexShaderConstant(48, dir[0], dir[1], dir[2], dir[3]);

    // register c[49] : track sun direction in eyespace (models)
    float eyemat[16]; // = Vision::World.GetGLWorldMatrix();
	m_Renderer->GetMatrix(VIEW_MATRIX, eyemat);
    float eyedir[4]; // = &shader->vprog_custom_param[1].param[0];
    eyedir[0] = dir[0]*eyemat[0] + dir[1]*eyemat[4] + dir[2]*eyemat[8];
    eyedir[1] = dir[0]*eyemat[1] + dir[1]*eyemat[5] + dir[2]*eyemat[9];
    eyedir[2] = dir[0]*eyemat[2] + dir[1]*eyemat[6] + dir[2]*eyemat[10];
    eyedir[3] = dir[3]; // y-lookup coordinate

	shader->SetVertexShaderConstant(49, eyedir[0], eyedir[1], eyedir[2], eyedir[3]);

	// register c[50]
    float flags[2]; // = &shader->vprog_custom_param[2].param[0];
	flags[0] = (tod.bFog) ? 1.0f : 0.0f;      // fog
	flags[1] = (tod.bLighting) ? 1.0f : 0.0f; // lighting

	shader->SetVertexShaderConstant(50, flags[0], flags[1], 0, 0);

	// set fade distance
	shader->SetVertexShaderConstant(51, m_fFadeDistance, 0, 0, 0);

	// set stop drawing distance
	//shader->SetClipDistance(200.0f);
}

IHashString* CObjectCubeMapShaderCallback::GetCallbackName()
{
	return m_ShaderCallbackName;
}