///============================================================================
/// \file	CParticleRibbonType.cpp
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

REGISTER_COMPONENT( CParticleRibbonType );

IComponent *CParticleRibbonType::Create(int nArgs, va_list argptr)
{
	IObject *self;
	IHashString *name = va_arg(argptr, IHashString *);
	IHashString *parentName = va_arg(argptr, IHashString *);
	try
	{
		// try and create an instance
		self = new CParticleRibbonType(parentName, name);
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

CParticleRibbonType::CParticleRibbonType(IHashString *parentName, IHashString *name) : 
	OBJECTTEMPLATE(CParticleManager, CParticleRibbonType, IObject, parentName, name)
{
	m_pMaterial = NULL;
	m_pTexture = NULL;
	m_pParticleManager = (CParticleManager*)m_Manager;
}

CParticleRibbonType::~CParticleRibbonType()
{
	DeInit();
}

void CParticleRibbonType::Init()
{
	m_pMaterial = CRenderObject<>::LoadMaterial( &m_szMaterialName );
	if (m_pMaterial)
		m_pTexture = m_pMaterial->GetTexture( 0, 0 );

	m_fRibbonLength = (float)m_iSegmentCount * m_fSegmentLength;
}

void CParticleRibbonType ::DeInit()
{
}

void CParticleRibbonType::Serialize( IArchive &ar )
{
	if (ar.IsReading())
	{
		int iVersion;
		ar.Read( iVersion, _T("Version") );

		StdString szMaterialName;
		ar.Read( szMaterialName, _T("Material") );
		ar.Read( m_fTextureFPS, _T("TextureFPS") );
		ar.Read( m_fTextureScale, _T("TextureScale") );
		ar.Read( m_iSegmentCount, _T("SegmentCount") );
		ar.Read( m_fSegmentLength, _T("SegmentLength") );
		ar.Read( m_fSegmentWidth, _T("SegmentWidth") );
		ar.Read( m_fSegmentLife, _T("SegmentLife") );
		ar.Read( m_bAdditive, _T("Additive") );

		m_szMaterialName.Init( szMaterialName );
	}
	else
	{
		ar.Write( m_szMaterialName.GetString(), _T("Material") );
		ar.Write( m_fTextureFPS, _T("TextureFPS") );
		ar.Write( m_fTextureScale, _T("TextureScale") );
		ar.Write( m_iSegmentCount, _T("SegmentCount") );
		ar.Write( m_fSegmentLength, _T("SegmentLength") );
		ar.Write( m_fSegmentWidth, _T("SegmentWidth") );
		ar.Write( m_fSegmentLife, _T("SegmentLife") );
		ar.Write( m_bAdditive, _T("Additive") );
	}
}
