///============================================================================
/// \file	CParticleLineType.cpp
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

REGISTER_COMPONENT( CParticleLineType );

IComponent *CParticleLineType::Create(int nArgs, va_list argptr)
{
	IObject *self;
	IHashString *name = va_arg(argptr, IHashString *);
	IHashString *parentName = va_arg(argptr, IHashString *);
	try
	{
		// try and create an instance
		self = new CParticleLineType(parentName, name);
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

CParticleLineType::CParticleLineType(IHashString *parentName, IHashString *name) : 
	OBJECTTEMPLATE(CParticleManager, CParticleLineType, IObject, parentName, name)
{
	m_pMaterial = NULL;
	m_pTexture = NULL;
	m_pParticleManager = (CParticleManager*)m_Manager;
}

CParticleLineType::~CParticleLineType()
{
	DeInit();
}

void CParticleLineType::Init()
{
	m_pMaterial = CRenderObject<>::LoadMaterial( &m_szMaterialName );
	if (m_pMaterial)
		m_pTexture = m_pMaterial->GetTexture( 0, 0 );
}

void CParticleLineType ::DeInit()
{
}

void CParticleLineType::Serialize( IArchive &ar )
{
	if (ar.IsReading())
	{
		int iVersion;
		ar.Read( iVersion, _T("Version") );

		StdString szMaterialName;
		ar.Read( szMaterialName, _T("Material") );
		ar.Read( m_fTextureFPS, _T("TextureFPS") );
		ar.Read( m_fTextureScale, _T("TextureScale") );
		ar.Read( m_HeadColor, _T("HeadColor") );
		ar.Read( m_TailColor, _T("TailColor") );
		ar.Read( m_fMaxLife, _T("MaxLife") );
		ar.Read( m_fAlpha, _T("Alpha") );
		ar.Read( m_bAdditive, _T("Additive") );
		ar.Read( m_fLength, _T("Length") );
		ar.Read( m_fHeadLength, _T("HeadLength") );
		ar.Read( m_Direction, _T("Direction") );
		ar.Read( m_fSpeed, _T("Speed") );
		ar.Read( m_fMidWidth, _T("Width") );
		ar.Read( m_fHeadWidth, _T("HeadWidth") );
		ar.Read( m_fTailWidth, _T("TailWidth") );

		m_szMaterialName.Init( szMaterialName );
	}
	else
	{
		ar.Write( m_szMaterialName.GetString(), _T("Material") );
		ar.Write( m_fTextureFPS, _T("TextureFPS") );
		ar.Write( m_fTextureScale, _T("TextureScale") );
		ar.Write( m_HeadColor, _T("HeadColor") );
		ar.Write( m_TailColor, _T("TailColor") );
		ar.Write( m_fMaxLife, _T("MaxLife") );
		ar.Write( m_fAlpha, _T("Alpha") );
		ar.Write( m_bAdditive, _T("Additive") );
		ar.Write( m_fLength, _T("Length") );
		ar.Write( m_fHeadLength, _T("HeadLength") );
		ar.Write( m_Direction, _T("Direction") );
		ar.Write( m_fSpeed, _T("Speed") );
		ar.Write( m_fMidWidth, _T("Width") );
		ar.Write( m_fHeadWidth, _T("HeadWidth") );
		ar.Write( m_fTailWidth, _T("TailWidth") );
	}
}
