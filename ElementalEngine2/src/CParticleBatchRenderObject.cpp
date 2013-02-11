///==========================================================================
/// \file	CParticleBatchRenderObject.hpp
/// \brief	Implementation of CParticleBatchRenderObject interface
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
#include "CParticleBatchRenderObject.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace ElementalEngine;

REGISTER_COMPONENT(CParticleBatchRenderObject);

/// \brief The Initialization for the Object
/// \return pointer to an IComponent
IComponent *CParticleBatchRenderObject::Create(int nArgs, va_list argptr)
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
			self = new CParticleBatchRenderObject(parentName, name, true);
		}
		else if (nArgs == 3)
		{
			bAddToHier = va_arg(argptr, bool);
			self = new CParticleBatchRenderObject(parentName, name, bAddToHier);
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
CParticleBatchRenderObject::CParticleBatchRenderObject(IHashString *parentName, IHashString *name, bool bAddToHier) : 
		CRenderObject<>( _T("CParticleManager"),_T("CParticleBatchRenderObject"), parentName, name )
{
	m_bEnabled = false;

	m_DynamicVertexBuffer = NULL;

	m_bAddToHierarchy = bAddToHier;	
	if (bAddToHier)
	{
		AddToHierarchy();
	}

	DeInit();
}

CParticleBatchRenderObject::~CParticleBatchRenderObject()
{
	RemoveBoundingObject();

	DeInit();
	if (m_bAddToHierarchy)
	{
		RemoveFromHierarchy();
	}
}

void CParticleBatchRenderObject::Init()
{
	//grab the renderer
	GETRENDERERINTERFACEPARAMS renderInterfaceParam;
	renderInterfaceParam.m_RendererInterface = NULL;
	static DWORD msgHash_GetRendererInterface = CHashString(_T("GetRendererInterface")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetRendererInterface, sizeof(GETRENDERERINTERFACEPARAMS), &renderInterfaceParam, NULL, NULL );
	m_IRenderer = renderInterfaceParam.m_RendererInterface;


	CHANNELDESCRIPTORLIST VertexFormatParticles;
	VertexFormatParticles.push_back( MESH_STREAM_VERTEX_POSITION3 ); // position
	VertexFormatParticles.push_back( MESH_STREAM_VERTEX_TEXTURECOORD1 ); // texcoord
	VertexFormatParticles.push_back( MESH_STREAM_VERTEX_NORMAL ); // position offset
	VertexFormatParticles.push_back( MESH_STREAM_VERTEX_TANGENT4 ); // rotation
	VertexFormatParticles.push_back( MESH_STREAM_VERTEX_COLORCRGBA ); // color

	ALLOCATERENDERBUFFERMSG msg;	
	UINT Max2DAllocation = MAX_PARTICLE_BUFFER;
	msg.ElementsToAllocate = Max2DAllocation;
	msg.SizeVertexFormat = sizeof( PARTICLE_VERTEX );
	msg.VertexFormat = VertexFormatParticles;
	msg.Dynamic = true;
	msg.m_AllowSharedBuffer = false;

	static DWORD msgHash_AllocateVertices = CHashString(_T("AllocateVertices")).GetUniqueID();
	DWORD res = EngineGetToolBox()->SendMessage(msgHash_AllocateVertices, sizeof( msg ), &msg, NULL, NULL );

	if( res == MSG_HANDLED )
	{
		m_DynamicVertexBufferAllocation.m_Offset = msg.m_ReturnOffset;
		m_DynamicVertexBufferAllocation.m_AllocationHandle = msg.m_ReturnHandle;
		m_DynamicVertexBufferAllocation.m_InterfaceHandle = msg.m_ReturnInterfaceHandle;
		m_DynamicVertexBufferAllocation.m_Size = Max2DAllocation;
		m_DynamicVertexBufferAllocation.m_Stride = msg.SizeVertexFormat;
		m_DynamicVertexBuffer = (IVertexBufferObject*)msg.m_InterfacePointer;
	}			
	else
	{	
		//return false;
	}

	UpdateGlobalObject();
}

/// \brief Update the render object( Render )
/// \return true or false
bool CParticleBatchRenderObject::Render( UINT pass, IEffect * override )
{
	if( !m_DynamicVertexBuffer )
		return true;

	vector< int > renderOffsets;

	int curoffset = m_DynamicVertexBufferAllocation.m_Offset;
	void * pDest = NULL;
	m_DynamicVertexBuffer->Lock( m_DynamicVertexBufferAllocation.m_Offset,
								m_DynamicVertexBufferAllocation.m_Size,
								(void**)&pDest,
								VBO_LOCK_DISCARD
								);

	// batch lines
	CParticleManager *pParticleManager = dynamic_cast<CParticleManager*>(m_Manager);
	PARTICLETYPETOPARTICLELINE* BatchMap = pParticleManager->GetParticleLineBatches();
	PARTICLETYPETOPARTICLELINE::iterator it = BatchMap->begin();
	while( it != BatchMap->end() )
	{
		const CParticleLineType* ParticleType = it->first;
		PARTICLELINESMAP::iterator ParticleIt = it->second.begin();

		while( ParticleIt != it->second.end() )
		{
			CParticleLineRenderObject* pObject = (CParticleLineRenderObject *)(ParticleIt->second);
			if( pObject->IsInScene() )
			{
				curoffset += pObject->UpdateBuffer( (char**)&pDest, curoffset, m_DynamicVertexBuffer );
			}
			++ParticleIt;
		}
		renderOffsets.push_back( curoffset );
		++it;
	}
	// batch ribbons
	PARTICLETYPETOPARTICLERIBBON* RibbonBatchMap = pParticleManager->GetParticleRibbonBatches();
	PARTICLETYPETOPARTICLERIBBON::iterator itRibbon = RibbonBatchMap->begin();
	while( itRibbon != RibbonBatchMap->end() )
	{
		const CParticleRibbonType* ParticleType = itRibbon->first;
		PARTICLERIBBONSMAP::iterator ParticleIt = itRibbon->second.begin();

		while( ParticleIt != itRibbon->second.end() )
		{
			CParticleRibbonRenderObject* pObject = (CParticleRibbonRenderObject *)(ParticleIt->second);
			if( pObject->IsInScene() )
			{
				curoffset += pObject->UpdateBuffer( (char**)&pDest, curoffset, m_DynamicVertexBuffer );
			}
			++ParticleIt;
		}
		renderOffsets.push_back( curoffset );
		++itRibbon;
	}

	m_DynamicVertexBuffer->Unlock();

	Matrix4x4 matworld;
	matworld.SetIdentity();
	m_IRenderer->SetMatrix( WORLD_MATRIX, (CONST FLOAT*)matworld.GetMatrix() );

	int i = 0;

	// render lines
	for (it = BatchMap->begin(); it != BatchMap->end();++it,i++)
	{
		const CParticleLineType* ParticleType = it->first;

		PARTICLELINESMAP::iterator ParticleIt = it->second.begin();
		int offset = i == 0 ? m_DynamicVertexBufferAllocation.m_Offset : renderOffsets[ i-1 ];
		int numElements = renderOffsets[ i ] - offset;
		numElements /= 3;
		// skip empty render offsets
		if (numElements <= 0)
			continue;

		m_IRenderer->SetMaterial( 0, ParticleType->GetMaterial() );
		if (ParticleType->GetAdditive())
			m_IRenderer->SetBlendMode( BLEND_ADDITIVE );
		else
			m_IRenderer->SetBlendMode( BLEND_NORMAL );

		IBaseTextureObject *pTexture = ParticleType->GetTexture();
		if( pTexture && pTexture->GetTextureType() == EE_TEXTUREANIMATED2D )
		{
			IAnimatedTextureObject *pAnimTex = (IAnimatedTextureObject *)pTexture;
			float u;
			ITextureObject *pAnimFrameTex;
			pAnimTex->GetFrame( 0, u,u,u,u, &pAnimFrameTex );
			pTexture = pAnimFrameTex;
		}

		m_IRenderer->SetTexture( 0, pTexture );
		m_IRenderer->RenderVertexBuffer( m_DynamicVertexBuffer, offset, numElements, DP_TRIANGLELIST );
	}

	// render ribbons
	for (itRibbon = RibbonBatchMap->begin(); itRibbon != RibbonBatchMap->end();++itRibbon,i++)
	{
		int offset = i == 0 ? m_DynamicVertexBufferAllocation.m_Offset : renderOffsets[ i-1 ];
		int numElements = renderOffsets[ i ] - offset;
		numElements = numElements - 2;
		if (numElements > 0)
		{
			const CParticleRibbonType* ParticleType = itRibbon->first;

			PARTICLERIBBONSMAP::iterator ParticleIt = itRibbon->second.begin();

			m_IRenderer->SetMaterial( 0, ParticleType->GetMaterial() );
			if (ParticleType->GetAdditive())
				m_IRenderer->SetBlendMode( BLEND_ADDITIVE );
			else
				m_IRenderer->SetBlendMode( BLEND_NORMAL );

			IBaseTextureObject *pTexture = ParticleType->GetTexture();
			if( pTexture && pTexture->GetTextureType() == EE_TEXTUREANIMATED2D )
			{
				IAnimatedTextureObject *pAnimTex = (IAnimatedTextureObject *)pTexture;
				float u;
				ITextureObject *pAnimFrameTex;
				pAnimTex->GetFrame( 0, u,u,u,u, &pAnimFrameTex );
				pTexture = pAnimFrameTex;
			}

			m_IRenderer->SetTexture( 0, pTexture );
			m_IRenderer->RenderVertexBuffer( m_DynamicVertexBuffer, offset, numElements, DP_TRIANGLESTRIP );
		}
	}
	m_IRenderer->SetBlendMode( BLEND_NORMAL );
	/*
	// batch emitter
	PARTICLETYPETOPARTICLEEMITTER* EmitterBatchMap = pParticleManager->GetParticleEmitterBatches();
	PARTICLETYPETOPARTICLEEMITTER::iterator itEmitter = EmitterBatchMap->begin();

//	vector< int > renderOffsets;
	if( m_DynamicVertexBuffer )
	{
		int curoffset = m_DynamicVertexBufferAllocation.m_Offset;
		void * pDest = NULL;
		m_DynamicVertexBuffer->Lock( m_DynamicVertexBufferAllocation.m_Offset,
									m_DynamicVertexBufferAllocation.m_Size,
									(void**)&pDest,
									VBO_LOCK_DISCARD
									);

		while( itEmitter != EmitterBatchMap->end() )
		{
			const CParticleEmitterType* ParticleType = itEmitter->first;
			PARTICLEEMITTERSMAP::iterator ParticleIt = itEmitter->second.begin();
			ParticleIt = itEmitter->second.begin();


			while( ParticleIt != itEmitter->second.end() )
			{
				CParticleEmitterObject* pObject = (CParticleEmitterObject *)(ParticleIt->second);
				if( pObject->IsInScene() )
				{
					curoffset += pObject->UpdateBuffer( (char**)&pDest, curoffset, m_DynamicVertexBuffer );
				}
				++ParticleIt;
			}
			renderOffsets.push_back( curoffset );
			++itEmitter;
		}
		m_DynamicVertexBuffer->Unlock();
	}
	itEmitter = EmitterBatchMap->begin();
	i = 0;
	for (; itEmitter != EmitterBatchMap->end();++itEmitter,i++)
	{
		const CParticleEmitterType* ParticleType = itEmitter->first;

		PARTICLEEMITTERSMAP::iterator ParticleIt = itEmitter->second.begin();
		if( m_DynamicVertexBuffer )
		{
			int offset = i == 0 ? m_DynamicVertexBufferAllocation.m_Offset : renderOffsets[ i-1 ];
			int numElements = renderOffsets[ i ] - offset;
			numElements /= 3;
			// skip empty render offsets
			if (numElements <= 0)
				continue;

			Matrix4x4 matworld;
			matworld.SetIdentity();
			m_IRenderer->SetMatrix( WORLD_MATRIX, (CONST FLOAT*)matworld.GetMatrix() );
			
			m_IRenderer->SetMaterial( 0, NULL );
			m_IRenderer->SetDepthTest( true );
			m_IRenderer->SetDepthWrite( false );	
			m_IRenderer->SetAlphaTest( false );
			if (ParticleType->GetAdditiveBlending())
				m_IRenderer->SetBlendMode( BLEND_ADDITIVE );
			else
				m_IRenderer->SetBlendMode( BLEND_NORMAL );
			m_IRenderer->SetRenderState( RENDERSTATE_ALPHABLENDENABLE, RENDERSTATEPARAM_TRUE );
			m_IRenderer->SetRenderState( RENDERSTATE_CULLMODE, RENDERSTATEPARAM_CULLNONE );

			ITextureObject *pTexture = (ITextureObject*)ParticleType->GetTexture();
			if( pTexture && pTexture->GetTextureType() == EE_TEXTUREANIMATED2D )
			{
				IAnimatedTextureObject *pAnimTex = (IAnimatedTextureObject *)pTexture;
				float u;
				pAnimTex->GetFrame( 0, u,u,u,u, &pTexture );
			}

			m_IRenderer->SetTexture( 0, pTexture );
			m_IRenderer->RenderVertexBuffer( m_DynamicVertexBuffer, offset, numElements, DP_TRIANGLELIST );

			m_IRenderer->SetBlendMode( BLEND_NORMAL );
			m_IRenderer->SetDepthTest( true );
			m_IRenderer->SetDepthWrite( true );
		}
	}
	*/

/*
	m_bInScene = true;		
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

void CParticleBatchRenderObject::GetBoundingSphere( Vec3 &position, float &radius )
{
	position.Set(0,0,0);
	radius = 0.0f;
}

bool CParticleBatchRenderObject::IsAlphaObject()
{	
	return true;
}

bool CParticleBatchRenderObject::IsLessThan( const IRenderObject * obj, DWORD sortMethod, int curRenderPass )
{	
	//order for particles does not matter
	return ( false );
}

void CParticleBatchRenderObject::DeInit()
{
	CRenderObject<>::DeInit();
}

void CParticleBatchRenderObject::Serialize(IArchive &ar)
{
	if (ar.IsReading())
	{
	}
	else
	{
	}
}

IHashString * CParticleBatchRenderObject::GetBaseSortClass()
{
	static CHashString CRO(_T("ParticleBatchRenderObject"));
	return &CRO;
}