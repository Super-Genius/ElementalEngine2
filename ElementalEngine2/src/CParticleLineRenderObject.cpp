///==========================================================================
/// \file	CParticleLineRenderObject.hpp
/// \brief	Implementation of CParticleLineRenderObject interface
/// \date	3/29/2007
/// \author Brian Bazyk
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
///==========================================================================

#include "StdAfx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace ElementalEngine;

REGISTER_COMPONENT(CParticleLineRenderObject);
REGISTER_MESSAGE_HANDLER( Disable, OnDisable, CParticleLineRenderObject );
REGISTER_MESSAGE_HANDLER( Enable, OnEnable, CParticleLineRenderObject );
REGISTER_MESSAGE_HANDLER( SetLength, OnSetLength, CParticleLineRenderObject );
REGISTER_MESSAGE_HANDLER( SetParticleLineSpeed, OnSetParticleLineSpeed, CParticleLineRenderObject );

/// \brief The Initialization for the Object
/// \return pointer to an IComponent
IComponent *CParticleLineRenderObject::Create(int nArgs, va_list argptr)
{
	IHashString *name;
	IHashString *parentName;
	IObject *self;
	bool bAddToHier;

	name = va_arg(argptr, IHashString *);
	parentName = va_arg(argptr, IHashString *);
	// create the object however you must create it
	try
	{
		// try and create an instance
		if (nArgs == 2)
		{
			self = new CParticleLineRenderObject(parentName, name, true);
		}
		else if (nArgs == 3)
		{
			bAddToHier = va_arg(argptr, bool);
			self = new CParticleLineRenderObject(parentName, name, bAddToHier);
		}
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

///
/// Constructor / Destructor
///
CParticleLineRenderObject::CParticleLineRenderObject(IHashString *parentName, IHashString *name, bool bAddToHier) : 
		CRenderObject<>( _T("CParticleManager"),_T("CParticleLineRenderObject"), parentName, name ),
		m_ccamTypeName(_T("CCamera")),
		m_szTypeTypeName(_T("CParticleLineType"))
{
	m_bEnabled = false;

	m_bAddToHierarchy = bAddToHier;	
	if (bAddToHier)
	{
		AddToHierarchy();
	}

	DeInit();
	m_bInScene = false;
	m_CurrentTick = 0;
	m_pType = NULL;
	m_Position.Set( 0, 0, 0 );
	m_LocalPos.Set( 0, 0, 0 );
	m_GlobalDir.Set( 0, 0, 1 );
	m_fLengthScale = 1.0f;
	m_fSpeed = 0;
}

CParticleLineRenderObject::~CParticleLineRenderObject()
{
	RemoveBoundingObject();

	DeInit();
	if (m_bAddToHierarchy)
	{
		RemoveFromHierarchy();
	}
}

void CParticleLineRenderObject::Init()
{
	//grab the renderer
	GETRENDERERINTERFACEPARAMS renderInterfaceParam;
	renderInterfaceParam.m_RendererInterface = NULL;
	static DWORD msgHash_GetRendererInterface = CHashString(_T("GetRendererInterface")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetRendererInterface, sizeof(GETRENDERERINTERFACEPARAMS), &renderInterfaceParam, NULL, NULL );
	m_IRenderer = renderInterfaceParam.m_RendererInterface;

	// get the type
	CParticleManager *pParticleManager = dynamic_cast<CParticleManager*>(m_Manager);
	m_pType = (CParticleLineType*)pParticleManager->LoadParticleType( &m_szTypeName, &m_szTypeTypeName );
	if (m_pType)
	{
		m_fSpeed = m_pType->GetSpeed();
		if (m_pType->GetMaxLife() > 0.0f)
			m_fLife = m_pType->GetMaxLife();
		else
			m_fLife = 0.0f;

		OnEnable(0,0);
		pParticleManager->AddParticleLine( this );
	}

	m_LastTickCount = m_CurrentTick;
	m_fTime = 0;
	m_bTransformIsDirty = true;
}

int CParticleLineRenderObject::UpdateBuffer( char ** pDest, int offset,IVertexBufferObject * vb )
{
	// reset to NULL in case this fails we don't try to render
	m_VB = NULL;
	m_VBOffset = 0;

	if( m_bInScene && m_bEnabled )
	{
		unsigned int iVertices = 12;

		// check if we have enough room in the vb
		if ((vb->GetSize() - offset) < iVertices)
			return 0;

		// get active camera position and view direction
		Vec3 vCameraPos(0,0,0);
		CONTEXTCAMVECTORPARAMS ccvp;
		ccvp.pRC = NULL;
		ccvp.vdata = &vCameraPos;
		static DWORD msgHash_GetActiveCameraPosition = CHashString(_T("GetActiveCameraPosition")).GetUniqueID();
		if (m_ToolBox->SendMessage(msgHash_GetActiveCameraPosition, sizeof(ccvp), &ccvp ) != MSG_HANDLED)
			return 0;

		CAMERAVECTORSPARAMS camvectors;
		Vec3 camvectorup(0,1,0);
		Vec3 camvectorright(1,0,0);
		Vec3 camvectorview(0,1,0);
		camvectors.vRightVector = &camvectorright;
		camvectors.vUpVector = &camvectorup;
		camvectors.vViewVector = &camvectorview;
		CHashString hszCameraName;	
		CONTEXTCAMPARAMS ccp;
		ccp.pRC = NULL;
		ccp.szCameraName = &hszCameraName;
		static DWORD msgHash_GetActiveCamera = CHashString(_T("GetActiveCamera")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GetActiveCamera, sizeof( CONTEXTCAMPARAMS ), &ccp, NULL, NULL );
		static DWORD msgHash_GetCameraVectors = CHashString(_T("GetCameraVectors")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GetCameraVectors, sizeof( CAMERAVECTORSPARAMS ), &camvectors, 
			ccp.szCameraName, &m_ccamTypeName);

		Vec3 vCameraView = camvectorview;

		PARTICLE_VERTEX *pVertexDest = (PARTICLE_VERTEX*)(*pDest);

		// find line points
		float fTotalLength = m_pType->GetLength() * m_fLengthScale;
		float fHeadLength = m_pType->GetHeadLength() * m_fLengthScale;
		float fTailLength = fTotalLength - fHeadLength;

		if (fTailLength < 0.0f)
			fTailLength = 0.0f;
		Vec3 vStart( m_Position );
		Vec3 vMiddle( vStart + m_GlobalDir * fHeadLength );
		Vec3 vEnd( vStart + m_GlobalDir * fTotalLength );

		// find up vector
		Vec3 vStartToCamera = vStart - vCameraPos;
		float fCamLength = vStartToCamera.Length();
		if (fCamLength < 0.001f) // too close to camera
		{
			vStartToCamera = vCameraView;
		}
		else
		{
			vStartToCamera /= fCamLength;
		}

		Vec3 vUp( vStartToCamera.Cross( m_GlobalDir ));
		float fUpLength = vUp.Length();
		if (fUpLength < 0.001f)
		{
			vUp.Set( 0,0,0 ); // no line
		}
		else
		{
			vUp /= fUpLength;
		}
		Vec3 vUpHead = vUp * m_pType->GetHeadWidth() * 0.5f;
		Vec3 vUpTail = vUp * m_pType->GetTailWidth() * 0.5f;
		vUp *= m_pType->GetMidWidth() * 0.5f;

		// adjust alpha
		float fRatio = 1.0f;
		if (m_pType->GetMaxLife() > 0.0f)
			fRatio = m_fLife / m_pType->GetMaxLife();
		float fAlpha = (fRatio * m_pType->GetAlpha());

		Vec3 vecHeadColor = m_pType->GetHeadColor();
		Vec3 vecTailColor = m_pType->GetTailColor();
		Vec3 vecMiddleColor;
		vecMiddleColor = vecHeadColor * (fHeadLength/fTotalLength) + vecTailColor * (fTailLength/fTotalLength);
		DWORD middleColor = m_IRenderer->GetColorARGB( fAlpha, vecMiddleColor.x, vecMiddleColor.y, vecMiddleColor.z );
		m_HeadColor = m_IRenderer->GetColorARGB( fAlpha, vecHeadColor.x, vecHeadColor.y, vecHeadColor.z );
		m_TailColor = m_IRenderer->GetColorARGB( fAlpha, vecTailColor.x, vecTailColor.y, vecTailColor.z );

		// find line points
		Vec3 vPoints[6];
		vPoints[0] = vStart + vUpHead;
		vPoints[1] = vStart - vUpHead;
		vPoints[2] = vMiddle + vUp;
		vPoints[3] = vMiddle - vUp;
		vPoints[4] = vEnd + vUpTail;
		vPoints[5] = vEnd - vUpTail;

		// set vertex data
		pVertexDest[0].Position[0] = vPoints[0].x;
		pVertexDest[0].Position[1] = vPoints[0].y;
		pVertexDest[0].Position[2] = vPoints[0].z;
		pVertexDest[0].Color = m_HeadColor;

		pVertexDest[1].Position[0] = vPoints[1].x;
		pVertexDest[1].Position[1] = vPoints[1].y;
		pVertexDest[1].Position[2] = vPoints[1].z;
		pVertexDest[1].Color = m_HeadColor;

		pVertexDest[2].Position[0] = vPoints[2].x;
		pVertexDest[2].Position[1] = vPoints[2].y;
		pVertexDest[2].Position[2] = vPoints[2].z;
		pVertexDest[2].Color = middleColor;

		pVertexDest[3].Position[0] = vPoints[1].x;
		pVertexDest[3].Position[1] = vPoints[1].y;
		pVertexDest[3].Position[2] = vPoints[1].z;
		pVertexDest[3].Color = m_HeadColor;

		pVertexDest[4].Position[0] = vPoints[3].x;
		pVertexDest[4].Position[1] = vPoints[3].y;
		pVertexDest[4].Position[2] = vPoints[3].z;
		pVertexDest[4].Color = middleColor;

		pVertexDest[5].Position[0] = vPoints[2].x;
		pVertexDest[5].Position[1] = vPoints[2].y;
		pVertexDest[5].Position[2] = vPoints[2].z;
		pVertexDest[5].Color = middleColor;

		pVertexDest[6].Position[0] = vPoints[2].x;
		pVertexDest[6].Position[1] = vPoints[2].y;
		pVertexDest[6].Position[2] = vPoints[2].z;
		pVertexDest[6].Color = middleColor;

		pVertexDest[7].Position[0] = vPoints[3].x;
		pVertexDest[7].Position[1] = vPoints[3].y;
		pVertexDest[7].Position[2] = vPoints[3].z;
		pVertexDest[7].Color = middleColor;

		pVertexDest[8].Position[0] = vPoints[4].x;
		pVertexDest[8].Position[1] = vPoints[4].y;
		pVertexDest[8].Position[2] = vPoints[4].z;
		pVertexDest[8].Color = m_TailColor;

		pVertexDest[9].Position[0] = vPoints[3].x;
		pVertexDest[9].Position[1] = vPoints[3].y;
		pVertexDest[9].Position[2] = vPoints[3].z;
		pVertexDest[9].Color = middleColor;

		pVertexDest[10].Position[0] = vPoints[5].x;
		pVertexDest[10].Position[1] = vPoints[5].y;
		pVertexDest[10].Position[2] = vPoints[5].z;
		pVertexDest[10].Color = m_TailColor;

		pVertexDest[11].Position[0] = vPoints[4].x;
		pVertexDest[11].Position[1] = vPoints[4].y;
		pVertexDest[11].Position[2] = vPoints[4].z;
		pVertexDest[11].Color = m_TailColor;

		//check animated textures:
		IAnimatedTextureObject *pAnimTex = NULL;
		IBaseTextureObject *pTexture = m_pType->GetTexture();
		if( pTexture && pTexture->GetTextureType() == EE_TEXTUREANIMATED2D )
			pAnimTex = dynamic_cast<IAnimatedTextureObject*>(pTexture);

		// update animated texture
		if( pAnimTex )
		{
			float fFPS = m_pType->GetTextureFPS();

			int iTextureFrame = (int)(m_fTime * fFPS);
			iTextureFrame %= pAnimTex->GetNumFrames();

			// find uv's
			float u1, u2, v1, v2;
			pAnimTex->GetFrame( iTextureFrame, v1, u1, v2, u2, NULL );

			u2 *= m_pType->GetTextureScale();
			float u12 = (u1+u2)*0.5f; // u12 = midpoint of u1,u2

			pVertexDest[0].TexCoord[0] = u1;
			pVertexDest[0].TexCoord[1] = v1;
			pVertexDest[1].TexCoord[0] = u1;
			pVertexDest[1].TexCoord[1] = v2;
			pVertexDest[2].TexCoord[0] = u12;
			pVertexDest[2].TexCoord[1] = v1;

			pVertexDest[3].TexCoord[0] = u1;
			pVertexDest[3].TexCoord[1] = v2;
			pVertexDest[4].TexCoord[0] = u12;
			pVertexDest[4].TexCoord[1] = v2;
			pVertexDest[5].TexCoord[0] = u12;
			pVertexDest[5].TexCoord[1] = v1;

			pVertexDest[6].TexCoord[0] = u12;
			pVertexDest[6].TexCoord[1] = v1;
			pVertexDest[7].TexCoord[0] = u12;
			pVertexDest[7].TexCoord[1] = v2;
			pVertexDest[8].TexCoord[0] = u2;
			pVertexDest[8].TexCoord[1] = v1;

			pVertexDest[9].TexCoord[0] = u12;
			pVertexDest[9].TexCoord[1] = v2;
			pVertexDest[10].TexCoord[0] = u2;
			pVertexDest[10].TexCoord[1] = v2;
			pVertexDest[11].TexCoord[0] = u2;
			pVertexDest[11].TexCoord[1] = v1;
		}
		else
		{
			// not using animated texture
			pVertexDest[0].TexCoord[0] = 0.f;
			pVertexDest[0].TexCoord[1] = 0.f;
			pVertexDest[1].TexCoord[0] = 0.f;
			pVertexDest[1].TexCoord[1] = 1.f;
			pVertexDest[2].TexCoord[0] = 0.5f;
			pVertexDest[2].TexCoord[1] = 0.f;

			pVertexDest[3].TexCoord[0] = 0.f;
			pVertexDest[3].TexCoord[1] = 1.f;
			pVertexDest[4].TexCoord[0] = 0.5f;
			pVertexDest[4].TexCoord[1] = 1.f;
			pVertexDest[5].TexCoord[0] = 0.5f;
			pVertexDest[5].TexCoord[1] = 0.f;

			pVertexDest[6].TexCoord[0] = 0.5f;
			pVertexDest[6].TexCoord[1] = 0.f;
			pVertexDest[7].TexCoord[0] = 0.5f;
			pVertexDest[7].TexCoord[1] = 1.f;
			pVertexDest[8].TexCoord[0] = 1.f;
			pVertexDest[8].TexCoord[1] = 0.f;

			pVertexDest[9].TexCoord[0] = 0.5f;
			pVertexDest[9].TexCoord[1] = 1.f;
			pVertexDest[10].TexCoord[0] = 1.f;
			pVertexDest[10].TexCoord[1] = 1.f;
			pVertexDest[11].TexCoord[0] = 1.f;
			pVertexDest[11].TexCoord[1] = 0.f;
		}

		// advance dest ptr
		(*pDest) = (char*)((PARTICLE_VERTEX*)(*pDest) + iVertices);

		m_bInScene = false;//reset	
		m_VB = vb;
		m_VBOffset = offset;
		return iVertices;
	}
	return 0;
}

bool CParticleLineRenderObject::Update()
{
	if( m_bTransformIsDirty )
	{
		Matrix4x4 parentTransform;
		parentTransform.SetIdentity();
		// Get the transform from the parent entity
		static DWORD msgHash_GetGlobalTransform = CHashString(_T("GetGlobalTransform")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GetGlobalTransform, sizeof(Matrix4x4), &parentTransform, GetParentName() );

		m_Position = parentTransform * m_LocalPos;
		parentTransform.SetTranslation( Vec3(0,0,0) );
		m_GlobalDir = parentTransform * m_pType->GetDirection();
 		m_bTransformIsDirty = false;

		UpdateBoundingObject();
	}

	m_CurrentTick = m_Timer->GetTime();

	if( m_bEnabled )
	{
		//count down life
		float TimeDeltaSeconds = (float)(m_CurrentTick - m_LastTickCount);
		m_fTime += TimeDeltaSeconds;
		m_TimeDelta = TimeDeltaSeconds;

		if (m_pType->GetMaxLife() > 0.0f)
		{
			m_fLife -= m_TimeDelta;
			if (m_fLife < 0.0f)
			{
				return false;
			}
		}

		// adjust pos
		if (m_fSpeed > 0.0f)
		{
			m_LocalPos += m_pType->GetDirection() * m_TimeDelta * m_fSpeed;
			SetDirtyTransformFlag();
		}
	}

	m_LastTickCount = m_CurrentTick;

	return true;
}

/// \brief Update the render object( Render )
/// \return true or false
bool CParticleLineRenderObject::Render( UINT pass, IEffect * override )
{
	static CHashString szAlphaBlend(_T("alphablend")); 
	static CHashString szTrue(_T("true")); 
	static CHashString cullmode(_T("cullmode"));
	static CHashString cullnone(_T("cullnone"));

	m_bInScene = true;
	/*
	if( m_VB )
	{
		Matrix4x4 matworld;
		matworld.SetIdentity();
		m_IRenderer->SetMatrix( WORLD_MATRIX, (CONST FLOAT*)matworld.GetMatrix() );
		
		m_IRenderer->SetMaterial( 0, NULL );
		m_IRenderer->SetDepthTest( true );
		m_IRenderer->SetDepthWrite( false );	
		m_IRenderer->SetAlphaTest( false );
		if (m_pType->GetAdditive())
			m_IRenderer->SetBlendMode( BLEND_ADDITIVE );
		else
			m_IRenderer->SetBlendMode( BLEND_NORMAL );
		m_IRenderer->SetRenderState( RENDERSTATE_ALPHABLENDENABLE, RENDERSTATEPARAM_TRUE );
		m_IRenderer->SetRenderState( RENDERSTATE_CULLMODE, RENDERSTATEPARAM_CULLNONE );

		ITextureObject *pTexture = m_pType->GetTexture();
		if( pTexture && pTexture->GetTextureType() == EE_TEXTUREANIMATED2D )
		{
			IAnimatedTextureObject *pAnimTex = (IAnimatedTextureObject *)pTexture;
			float u;
			pAnimTex->GetFrame( 0, u,u,u,u, &pTexture );
		}

		m_IRenderer->SetTexture( 0, pTexture );
		m_IRenderer->RenderVertexBuffer( m_VB, m_VBOffset, 4, DP_TRIANGLESTRIP );

		m_IRenderer->SetBlendMode( BLEND_NORMAL );
		m_IRenderer->SetDepthTest( true );
		m_IRenderer->SetDepthWrite( true );
	}
	*/
	return true;
}

void CParticleLineRenderObject::GetBoundingSphere( Vec3 &position, float &radius )
{
	float fRadius = 0.0f;
	Vec3 vCenter( 0, 0, 0 );
	if (m_pType)
	{
		fRadius = m_pType->GetLength() * m_fLengthScale * 0.5f;
		vCenter = m_Position + m_GlobalDir * fRadius;
	}
	position = vCenter;
	radius = fRadius;
}

bool CParticleLineRenderObject::IsAlphaObject()
{	
	return true;
}

bool CParticleLineRenderObject::IsLessThan( const IRenderObject * obj, DWORD sortMethod, int curRenderPass )
{	
	//order for particles does not matter
	return ( false );
}

void CParticleLineRenderObject::DeInit()
{
	CParticleManager *pParticleManager = dynamic_cast<CParticleManager*>(m_Manager);
	pParticleManager->RemoveParticleLine( this );
}

void CParticleLineRenderObject::Serialize(IArchive &ar)
{
	if (ar.IsReading())
	{
		StdString szTypeName;
		ar.Read( szTypeName, _T("TypeFileName") );

		m_szTypeName.Init( szTypeName );
	}
	else
	{
		ar.Write( m_szTypeName.GetString(), _T("TypeFileName") );
	}
}

IHashString * CParticleLineRenderObject::GetBaseSortClass()
{
	static CHashString CRO(_T("ParticleLineRenderObject"));
	return &CRO;
}

DWORD CParticleLineRenderObject::OnDisable(DWORD size, void *params)
{
	m_bEnabled = false;
	return MSG_HANDLED_PROCEED;
}

DWORD CParticleLineRenderObject::OnEnable(DWORD size, void *params)
{
	if (m_pType)
	{
		m_bEnabled = true;
		//Grab current time	
		m_CurrentTick = m_Timer->GetTime();
		m_LastTickCount = m_CurrentTick;
		// reset local position
		m_LocalPos.Set( 0, 0, 0 );
		return MSG_HANDLED_PROCEED;
	}
	else
		return MSG_ERROR;
}

DWORD CParticleLineRenderObject::OnSetLength( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(float) );
	float fTotalLength = *(float*)params;
	m_fLengthScale = fTotalLength / m_pType->GetLength();
	return MSG_HANDLED_STOP;
}

DWORD CParticleLineRenderObject::OnSetParticleLineSpeed( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(float) );
	m_fSpeed = *(float*)params;
	return MSG_HANDLED_STOP;
}

const CParticleLineType* CParticleLineRenderObject::GetParticleLineType( )
{
	return m_pType;
}
