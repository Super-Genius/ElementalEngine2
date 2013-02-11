///=====================================================================
/// \file	CMaterialOverride.cpp
/// \brief	Implementation for material overrides
/// \date	08/08/2007
/// \author	Nick Tourte
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
///=====================================================================

#include "stdafx.h"
#include "CMaterialOverride.h"

#define PIXELCONSTANTTYPE			_T("pixelconstant")
#define VERTEXCONSTANTTYPE			_T("vertexconstant")
#define TEXTURETYPE					_T("texture")
#define RENDERSTATETYPE				_T("renderstate")

CMaterialOverride::CMaterialOverride()
{
	// grab the render interface
	GETRENDERERINTERFACEPARAMS renderInterface;
	static DWORD msgHash_GetRendererInterface = CHashString(_T("GetRendererInterface")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_GetRendererInterface, sizeof(GETRENDERERINTERFACEPARAMS), &renderInterface);
	m_Renderer = renderInterface.m_RendererInterface;

	m_dwPixelConstantType = CHashString(PIXELCONSTANTTYPE).GetUniqueID();
	m_dwVertexConstantType = CHashString(VERTEXCONSTANTTYPE).GetUniqueID();
	m_dwTextureType = CHashString(TEXTURETYPE).GetUniqueID();
	m_dwRenderStateType = CHashString(RENDERSTATETYPE).GetUniqueID();

	m_bAlphaBlend = false;

	m_ToolBox = EngineGetToolBox();
}

CMaterialOverride::~CMaterialOverride()
{}

void CMaterialOverride::Apply(IRenderer *pRenderer, IEffect *pEffect)
{
	if (pEffect)
	{
		VEC4OVERRIDEMAP::iterator vertConstIter = m_VertexConstantOverrides.begin();
		for (; vertConstIter != m_VertexConstantOverrides.end(); ++vertConstIter)
		{
			CHashString var(m_ToolBox->GetHashString(vertConstIter->first));
			Vec4 value = vertConstIter->second;
			pEffect->SetVertexConstant(&var, value.x, value.y, value.z, value.w);		
		}

		VEC4OVERRIDEMAP::iterator pixConstIter = m_PixelConstantOverrides.begin();
		for (; pixConstIter != m_PixelConstantOverrides.end(); ++pixConstIter)
		{
			CHashString var(m_ToolBox->GetHashString(pixConstIter->first));
			Vec4 value = pixConstIter->second;
			pEffect->SetPixelConstant(&var, value.x, value.y, value.z, value.w);		
		}
	}

	if (pRenderer)
	{
		TEXTUREMAP::iterator texIter = m_TextureOverrides.begin();
		for (; texIter != m_TextureOverrides.end(); ++texIter)
		{
			UINT stage = texIter->first;
			IBaseTextureObject *tex = texIter->second;
			if (tex)
			{
				pRenderer->SetTexture(stage, tex);
			}
		}

		map< ENUMRENDERSTATE, UINT >::iterator rStateIter = m_RenderStateOverrides.begin();
		for (; rStateIter != m_RenderStateOverrides.end(); ++rStateIter)
		{
			pRenderer->PushRenderState(rStateIter->first, rStateIter->second);
		}
	}
}

void CMaterialOverride::UnApply(IRenderer *pRenderer, IEffect *pEffect)
{
	if (pRenderer)
	{
		map< ENUMRENDERSTATE, UINT >::iterator rStateIter = m_RenderStateOverrides.begin();
		for (; rStateIter != m_RenderStateOverrides.end(); ++rStateIter)
		{
			pRenderer->PopRenderState(rStateIter->first);
		}
	}
}

void CMaterialOverride::SetOverride(DWORD hsType, DWORD hsName, Vec4 vecValue)
{
	if (hsType == m_dwPixelConstantType)
	{
		m_PixelConstantOverrides[hsName] = vecValue;
	}
	else if (hsType == m_dwVertexConstantType)
	{
		m_VertexConstantOverrides[hsName] = vecValue;
	}
}

void CMaterialOverride::SetOverride(DWORD hsType, DWORD hsName, StdString szValue)
{
	if (hsType == m_dwTextureType)
	{
		AddTextureToMap(hsName, szValue);
	}
	else if (hsType == m_dwRenderStateType)
	{
		ENUMRENDERSTATE estate = m_Renderer->StringToRenderState( hsName );

		UINT ival;
		if (estate == RENDERSTATE_ALPHAREF)
		{
			ival = _tstoi( szValue );
		}
		else
		{
			ival = m_Renderer->StringToRenderStateParam(CHashString(szValue).GetUniqueID());
		}
		m_RenderStateOverrides[estate] = ival;

		if (estate == RENDERSTATE_ALPHABLENDENABLE)
		{
			if (ival == RENDERSTATEPARAM_TRUE)
			{
				m_bAlphaBlend = true;
			}
			else
			{
				m_bAlphaBlend = false;
			}
		}
	}
}

void CMaterialOverride::AddTextureToMap(DWORD hsNamedSpot, StdString szTexture)
{
	StdString spot = m_ToolBox->GetHashString(hsNamedSpot);
	// for now, let's say the name of the variable is just the stage number
	UINT stage = (UINT)_tstoi(spot);
	CHashString texName(szTexture);

	TEXTUREOBJECTPARAMS top;
	top.Name = &texName;
	static DWORD msgAddTexture = CHashString(_T("AddTexture")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgAddTexture, sizeof(TEXTUREOBJECTPARAMS), &top) == MSG_HANDLED)
	{
		if (top.TextureObjectInterface)
		{
			m_TextureOverrides[stage] = top.TextureObjectInterface;
		}
	}
}