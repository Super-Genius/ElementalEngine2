///==========================================================================
/// \file	CParticleRibbonRenderObject.hpp
/// \brief	Implementation of CParticleRibbonRenderObject interface
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
#include "CParticleRibbonRenderObject.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace ElementalEngine;

REGISTER_COMPONENT(CParticleRibbonRenderObject);
REGISTER_MESSAGE_HANDLER( Disable, OnDisable, CParticleRibbonRenderObject );
REGISTER_MESSAGE_HANDLER( Enable, OnEnable, CParticleRibbonRenderObject );
REGISTER_MESSAGE_HANDLER( SetColor, OnSetColor, CParticleRibbonRenderObject );

/// \brief The Initialization for the Object
/// \return pointer to an IComponent
IComponent *CParticleRibbonRenderObject::Create(int nArgs, va_list argptr)
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
			self = new CParticleRibbonRenderObject(parentName, name, true);
		}
		else if (nArgs == 3)
		{
			bAddToHier = va_arg(argptr, bool);
			self = new CParticleRibbonRenderObject(parentName, name, bAddToHier);
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
CParticleRibbonRenderObject::CParticleRibbonRenderObject(IHashString *parentName, IHashString *name, bool bAddToHier) : 
		CRenderObject<>( _T("CParticleManager"),_T("CParticleRibbonRenderObject"), parentName, name ),
		m_szTypeTypeName(_T("CParticleRibbonType"))
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
	m_bMotionOriented = false;
}

CParticleRibbonRenderObject::~CParticleRibbonRenderObject()
{
	RemoveBoundingObject();

	DeInit();
	if (m_bAddToHierarchy)
	{
		RemoveFromHierarchy();
	}
}

void CParticleRibbonRenderObject::Init()
{
	//grab the renderer
	GETRENDERERINTERFACEPARAMS renderInterfaceParam;
	renderInterfaceParam.m_RendererInterface = NULL;
	static DWORD msgHash_GetRendererInterface = CHashString(_T("GetRendererInterface")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetRendererInterface, sizeof(GETRENDERERINTERFACEPARAMS), &renderInterfaceParam, NULL, NULL );
	m_IRenderer = renderInterfaceParam.m_RendererInterface;

	// get the type
	CParticleManager *pParticleManager = dynamic_cast<CParticleManager*>(m_Manager);
	m_pType = (CParticleRibbonType*)pParticleManager->LoadParticleType( &m_szTypeName, &m_szTypeTypeName );
	if (m_pType)
	{
		m_OldLocalPos.Set( 0, 0, 0 );
		// create first segment
		RIBBONSEGMENT seg;
		seg.pos.Set( 0, 0, 0 );
		seg.len = 0;
		m_SegmentList.push_back( seg );
	}

	m_LastTickCount = m_CurrentTick;
	m_fTime = 0;
	m_bTransformIsDirty = true;

	OnEnable(0,0);
	SetColor( 1.0f, 1.0f, 1.0f );

	pParticleManager->AddParticleRibbon( this );
}

int CParticleRibbonRenderObject::UpdateBuffer( char ** pDest, int offset,IVertexBufferObject * vb )
{
	UpdateSegments();

	if (m_SegmentList.size() == 0)
		return 0;

	if( m_bInScene && m_bEnabled )
	{
		size_t iVertices = (m_SegmentList.size()+1) * 2 + 2;//2 for degenerate end

		// check if we have enough room in the vb
		if ((vb->GetSize() - offset) < iVertices)
			return 0;

		// TODO: move animated texture update to type instead of doing this for each object. Store the uv's in the
		// type so the render objects can use them for their uv's.
		// Do the same for particle lines.
		//check animated textures:
		float v1=0, u1=0, v2=1, u2=1;
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
			pAnimTex->GetFrame( iTextureFrame, v1, u1, v2, u2, NULL );
		}

		PARTICLE_VERTEX *pVertexDest = (PARTICLE_VERTEX*)(*pDest);
		float fLength = 0;
		SEGMENTLIST::iterator itSeg = m_SegmentList.begin();
		bool start = true;
		while (itSeg != m_SegmentList.end())
		{
			RIBBONSEGMENT *pSeg = &(*itSeg);
			float fU = (fLength / m_pType->GetRibbonLength()) * m_pType->GetTextureScale();

			Vec3 vAbove, vBelow, vAboveNext, vBelowNext;
			vAbove = pSeg->pos + pSeg->up * m_fAboveDist;
			vBelow = pSeg->pos - pSeg->up * m_fBelowDist;

			if( start )
			{
				start = false;
				pVertexDest[0].Color = m_VertexColor;
				pVertexDest[0].Position = vAbove;
				pVertexDest[0].TexCoord[0] = fU;
				pVertexDest[0].TexCoord[1] = v1;
				pVertexDest++;
			}

			pVertexDest[0].Color = m_VertexColor;
			pVertexDest[0].Position = vAbove;
			pVertexDest[0].TexCoord[0] = fU;
			pVertexDest[0].TexCoord[1] = v1;
			
			pVertexDest[1].Color = m_VertexColor;
			pVertexDest[1].Position = vBelow;
			pVertexDest[1].TexCoord[0] = fU;
			pVertexDest[1].TexCoord[1] = v2;

			fLength += pSeg->len;

			itSeg++;
			pVertexDest += 2;
		}

		itSeg--;
		RIBBONSEGMENT *pSeg = &(*itSeg);
		float fU = 1.0f * m_pType->GetTextureScale();

		Vec3 vAbove, vBelow;
		vAbove = pSeg->pos + pSeg->up * m_fAboveDist + (pSeg->back * (pSeg->len/pSeg->len0));
		vBelow = pSeg->pos - pSeg->up * m_fBelowDist + (pSeg->back * (pSeg->len/pSeg->len0));

		pVertexDest[0].Color = m_VertexColor;
		pVertexDest[0].Position = vAbove;
		pVertexDest[0].TexCoord[0] = fU;
		pVertexDest[0].TexCoord[1] = v1;

		pVertexDest[1].Color = m_VertexColor;
		pVertexDest[1].Position = vBelow;
		pVertexDest[1].TexCoord[0] = fU;
		pVertexDest[1].TexCoord[1] = v2;

		//create degenerate end
		pVertexDest[2].Color = m_VertexColor;
		pVertexDest[2].Position = vBelow;
		pVertexDest[2].TexCoord[0] = fU;
		pVertexDest[2].TexCoord[1] = v2;

		// advance dest ptr
		pVertexDest += 3;
		(*pDest) = (char*)(pVertexDest);

		m_bInScene = false;//reset	
		// probably should use size_t for this?
		return (int)iVertices;
	}
	return 0;
}

bool CParticleRibbonRenderObject::Update()
{
	if( m_bTransformIsDirty )
	{
		m_GlobalTransform.SetIdentity();
		// Get the transform from the parent entity
		static DWORD msgHash_GetGlobalTransform = CHashString(_T("GetGlobalTransform")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GetGlobalTransform, sizeof(Matrix4x4), &m_GlobalTransform, GetParentName() );

		m_Position = m_GlobalTransform.GetTranslation();
 		m_bTransformIsDirty = false;

		static DWORD msgHash_GetLocalTransform = CHashString(_T("GetLocalTransform")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GetLocalTransform, sizeof(Matrix4x4), &m_LocalTransform, GetParentName() );

		UpdateBoundingObject();
	}

	m_CurrentTick = m_Timer->GetTime();

	if( m_bEnabled )
	{
		float TimeDeltaSeconds = (float)(m_CurrentTick - m_LastTickCount);
		m_fTime += TimeDeltaSeconds;
		m_TimeDelta = TimeDeltaSeconds;
	}

	m_LastTickCount = m_CurrentTick;

	return true;
}

/// \brief Update the render object( Render )
/// \return true or false
bool CParticleRibbonRenderObject::Render( UINT pass, IEffect * override )
{
	m_bInScene = true;
	return true;
}

void CParticleRibbonRenderObject::GetBoundingSphere( Vec3 &position, float &radius )
{
	float fRadius = 0.0f;
	Vec3 vCenter( 0, 0, 0 );
	if (m_pType)
	{
		fRadius = m_pType->GetRibbonLength();
		vCenter = m_Position;
	}
	position = vCenter;
	radius = fRadius;
}

bool CParticleRibbonRenderObject::IsAlphaObject()
{	
	return true;
}

bool CParticleRibbonRenderObject::IsLessThan( const IRenderObject * obj, DWORD sortMethod, int curRenderPass )
{	
	//order for particles does not matter
	return ( false );
}

void CParticleRibbonRenderObject::DeInit()
{
	CParticleManager *pParticleManager = dynamic_cast<CParticleManager*>(m_Manager);
	pParticleManager->RemoveParticleRibbon( this );
}

void CParticleRibbonRenderObject::Serialize(IArchive &ar)
{
	if (ar.IsReading())
	{
		StdString szTypeName, szParentBoneName;
		ar.Read( szTypeName, _T("TypeFileName") );
		ar.Read( szParentBoneName, _T("ParentBoneName") );
		ar.Read( m_bMotionOriented, _T("OrientToVelocity") );

		m_szTypeName.Init( szTypeName );
		m_szParentBoneName.Init( szParentBoneName );
	}
	else
	{
		ar.Write( m_szTypeName.GetString(), _T("TypeFileName") );
		ar.Write( m_szParentBoneName.GetString(), _T("ParentBoneName") );
		ar.Write( m_bMotionOriented, _T("OrientToVelocity") );
	}
}

IHashString * CParticleRibbonRenderObject::GetBaseSortClass()
{
	static CHashString CRO(_T("ParticleRibbonRenderObject"));
	return &CRO;
}

DWORD CParticleRibbonRenderObject::OnDisable(DWORD size, void *params)
{
	m_bEnabled = false;
	return MSG_HANDLED_STOP;
}

DWORD CParticleRibbonRenderObject::OnEnable(DWORD size, void *params)
{
	m_bEnabled = true;
	//Grab current time	
	m_CurrentTick = m_Timer->GetTime();
	m_LastTickCount = m_CurrentTick;
	return MSG_HANDLED_STOP;
}

DWORD CParticleRibbonRenderObject::OnSetColor(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE( size, sizeof(Vec4) );
	Vec4 *msg = (Vec4*)params;
	SetColor( msg->x, msg->y, msg->z, msg->w );
	return MSG_HANDLED_STOP;
}

const CParticleRibbonType* CParticleRibbonRenderObject::GetParticleRibbonType( )
{
	return m_pType;
}

void CParticleRibbonRenderObject::UpdateSegments()
{
	if (m_SegmentList.size() == 0)
		return;

	if (*m_szParentBoneName.GetString() != '\0')
	{
		Vec3 parentBonePos;
		EulerAngle parentBoneRot;
		BONEINFO bi;
		bi.bonename = &m_szParentBoneName;
		bi.m_Position = &parentBonePos;
		bi.m_Rotation = &parentBoneRot;
		bi.m_Matrix = &m_GlobalTransform;
		// Get the transform from the parent bone
		static DWORD msgHash_GetBoneState = CHashString(_T("GetBoneState")).GetUniqueID();
		if (m_ToolBox->SendMessage(msgHash_GetBoneState, sizeof(BONEINFO), &bi, GetParentName() ) != MSG_HANDLED)
		{
			m_ToolBox->Log(LOGWARNING, _T("%s failed to GetBoneState from bone %s on %s.\n"), GetName()->GetString(), m_szParentBoneName.GetString(), GetParentName()->GetString());
			return;
		}
	}

	Vec3 ntpos = m_GlobalTransform.GetTranslation();
	Vec3 ntup;

	if (m_bMotionOriented)
	{
		Vec3 dispVec(ntpos);
		dispVec -= m_OldLocalPos;
		dispVec.Normalize();
		MathUtil math;	
		Matrix3x3 mat;	
		math.MatrixLookAt(Vec3(0, 0, 0), dispVec, mat);

		Matrix3x3 local(m_LocalTransform.Get3x3());
		ntup = mat * local * Vec3(0,1,0);
	}
	else
	{
		ntup = m_GlobalTransform * Vec3(0,1,0);
		ntup -= ntpos;
	}

	ntup.Normalize();
	float dlen = (ntpos-m_OldLocalPos).Length();

	// move first segment
	RIBBONSEGMENT &firstseg = *m_SegmentList.begin();
	if (firstseg.len > m_pType->GetSegmentLength())
	{
		// add new segment
		firstseg.back = (m_OldLocalPos-ntpos);
		firstseg.back.Normalize();
		firstseg.len0 = firstseg.len;
		RIBBONSEGMENT newseg;
		newseg.pos = ntpos;
		newseg.up = ntup;
		newseg.len = dlen;
		newseg.life = m_pType->GetSegmentLife() + m_fTime;
		m_SegmentList.push_front(newseg);
	}
	else
	{
		firstseg.up = ntup;
		firstseg.pos = ntpos;
		firstseg.len += dlen;
	}

	// kill stuff from the end
	float l = 0;
	bool erasemode = false;
	for (SEGMENTLIST::iterator it = m_SegmentList.begin(); it != m_SegmentList.end();)
	{
		if (!erasemode)
		{
			if ((m_pType->GetSegmentLife() > 0) && (m_SegmentList.size() > 1) && (it->life < m_fTime))
			{
				erasemode = true;
				continue;
			}

			l += it->len;
			if (l > m_pType->GetRibbonLength())
			{
				it->len = l - m_pType->GetRibbonLength();
				erasemode = true;
			}
			++it;
		}
		else
		{
			it = m_SegmentList.erase(it);
		}
	}

	m_OldLocalPos = ntpos;

	m_fAboveDist = m_pType->GetSegmentWidth() * 0.5f;
	m_fBelowDist = m_pType->GetSegmentWidth() * 0.5f;
}
