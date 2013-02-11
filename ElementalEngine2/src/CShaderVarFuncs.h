///============================================================================
/// \file	CShaderVarFuncs.h
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

#ifndef CSHADERVARFUNCS_H
#define CSHADERVARFUNCS_H
//Variable manipulator classes
#include "Stdafx.h"
#include "math.h"
#include "CCameraManager.h"
#include "CRenderManager.h"

class CSinCosTanTime : public IShaderVarFunc
{
public:
	virtual int GetValue( Vec4 *pData )
	{
		Vec4 &v = *pData;
		IToolBox * toolbox = EngineGetToolBox();
		if( toolbox )
		{
			ITimer* timer = toolbox->GetTimer();
			v.w = timer->GetTime();
			v.x = sin( v.w );
			v.y = cos( v.w );
			v.z = tan( v.w );
			return true;
		}
		return false;
	}
	virtual IHashString * GetName()
	{
		static CHashString cname( _T("SinCosTanTime") );
		return &cname;
	}
};

class CShaderVarFuncEyePos : public IShaderVarFunc
{
public:
	CShaderVarFuncEyePos()
	{
		m_ToolBox = EngineGetToolBox();
		GETRENDERERINTERFACEPARAMS renderInterface;
		renderInterface.m_RendererInterface = NULL;
		static DWORD msgHash_GetRendererInterface = CHashString(_T("GetRendererInterface")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GetRendererInterface, sizeof(GETRENDERERINTERFACEPARAMS), &renderInterface );
		m_Renderer = renderInterface.m_RendererInterface;
	}
	virtual int GetValue( Vec4 *pData )
	{
		Vec4 &v = *pData;
		if (m_Renderer)
		{
			static Matrix4x4 matView, matViewInv;
			m_Renderer->GetMatrix( VIEW_MATRIX, matView.GetMatrix() );
			matView.GetInverse( matViewInv );
			v.x = matViewInv.m[12];
			v.y = matViewInv.m[13];
			v.z = matViewInv.m[14];
			v.w = matViewInv.m[15];
			return 1;
		}
		return 0;
	}
	virtual IHashString * GetName()
	{
		static CHashString cname( _T("EyePos") );
		return &cname;
	}

	IToolBox *m_ToolBox;
	IRenderer *m_Renderer;
};

class CShaderVarFuncZMinMax : public IShaderVarFunc
{
public:
	CShaderVarFuncZMinMax()
	{
		m_ToolBox = EngineGetToolBox();
		m_CameraMgr = SINGLETONINSTANCE( CCameraManager );
		GETRENDERERINTERFACEPARAMS renderInterface;
		renderInterface.m_RendererInterface = NULL;
		static DWORD msgHash_GetRendererInterface = CHashString(_T("GetRendererInterface")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GetRendererInterface, sizeof(GETRENDERERINTERFACEPARAMS), &renderInterface );
		m_Renderer = renderInterface.m_RendererInterface;
	}
	virtual int GetValue( Vec4 *pData )
	{
		Vec4 &v = *pData;
		Vec3 MinMax;
		if( m_CameraMgr->OnGetActiveCameraZMinMax( sizeof( Vec3 ), &MinMax ) == MSG_HANDLED )
		{
			v.x = MinMax.x;
			v.y = MinMax.y;
		}
		else
		{
			v.x = 0.0;
			v.y = 0.0;
		}
		int width, height;
		m_Renderer->GetBackBufferDimensions( width, height );
		v.z = (float)width;
		v.w = (float)height;
		return 1;
	}
	virtual IHashString * GetName()
	{
		static CHashString cname( _T("ZMinMax") );
		return &cname;
	}

	IToolBox *m_ToolBox;
	CCameraManager *m_CameraMgr;
	IRenderer *m_Renderer;
};

class CSceneDepthTextureCallback : public ITextureCallback
{
public:
	virtual IBaseTextureObject* GetTexture()
	{
		TEXTUREOBJECTPARAMS top;
		static DWORD msgHash_GetDepthTargetAsTexture = CHashString(_T("GetDepthTargetAsTexture")).GetUniqueID();
		if (EngineGetToolBox()->SendMessage(msgHash_GetDepthTargetAsTexture, sizeof(top), &top) == MSG_HANDLED)
		{
			return top.TextureObjectInterface;
		}
		return NULL;
	}
	virtual IHashString * GetName()
	{
		static CHashString cname( _T("SceneDepthTextureCallback") );
		return &cname;
	}
};



class CShaderVarFuncFogColor : public IShaderVarFunc
{
public:
	CShaderVarFuncFogColor()
	{
		m_RenderMgr = SINGLETONINSTANCE( CRenderManager );
	}

	virtual int GetValue( Vec4 *pData )
	{
		Vec4 &v = *pData;
		Vec3 color;
		if( m_RenderMgr )
		{
			m_RenderMgr->GetFogColor( color );
			v.x = color.x;
			v.y = color.y;
			v.z = color.z;
			v.w = 0.f;
			return 1;
		}
		return 0;
	}
	virtual IHashString * GetName()
	{
		static CHashString cname( _T("FogColor") );
		return &cname;
	}

	CRenderManager *m_RenderMgr;
};


class CShaderVarFuncFogDistance: public IShaderVarFunc
{
public:
	virtual int GetValue( Vec4 *pData )
	{
		Vec4 &v = *pData;

		IToolBox * toolbox = EngineGetToolBox();
		if( toolbox )
		{
			CRenderManager * crend = SINGLETONINSTANCE( CRenderManager );
			Vec3 color;
			if( crend )
			{
				crend->GetFogStartDistance( v.x, v.y );
				v.z = 0.f;
				v.w = 0.f;
				return 1;
			}
		}
		return 0;
	}
	virtual IHashString * GetName()
	{
		static CHashString cname( _T("FogDistance") );
		return &cname;
	}
};

class CShaderVarFuncModelToWorld : public IShaderVarFunc
{
public:
	CShaderVarFuncModelToWorld()
	{
		m_ToolBox = EngineGetToolBox();
		GETRENDERERINTERFACEPARAMS renderInterface;
		renderInterface.m_RendererInterface = NULL;
		static DWORD msgHash_GetRendererInterface = CHashString(_T("GetRendererInterface")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GetRendererInterface, sizeof(GETRENDERERINTERFACEPARAMS), &renderInterface );
		m_Renderer = renderInterface.m_RendererInterface;
	}
	virtual int GetValue( Vec4 *pData )
	{
		if (m_Renderer && pData)
		{
			static Matrix4x4 matWorld;
			m_Renderer->GetMatrix( WORLD_MATRIX, matWorld.GetMatrix() );

			pData[0].Set( matWorld.GetRow(0) );
			pData[1].Set( matWorld.GetRow(1) );
			pData[2].Set( matWorld.GetRow(2) );
		}
		return 3;
	}
	virtual IHashString * GetName()
	{
		static CHashString cname( _T("ModelToWorld") );
		return &cname;
	}

	IToolBox *m_ToolBox;
	IRenderer *m_Renderer;
};

#endif
