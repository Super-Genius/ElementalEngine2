///==========================================================================
/// \file	CTerrainConfig.cpp
/// \brief	Implementation of CTerrainConfig for Terrain
/// \date	03-24-2005
/// \author	Brian Bazyk
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

REGISTER_COMPONENT(CTerrainConfig);

IComponent *CTerrainConfig::Create(int nArgs, va_list argptr)
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
			self = new CTerrainConfig(parentName, name, true);
		}
		else if (nArgs == 3)
		{
			bAddToHier = va_arg(argptr, bool);
			self = new CTerrainConfig(parentName, name, bAddToHier);
		}
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

CTerrainConfig::CTerrainConfig(IHashString *parentName, IHashString *name, bool bAddToHier) : 
	m_HashName(_T("CTerrainConfig")), 
	OBJECTTEMPLATE(CTerrainManager, CTerrainConfig, IObject, parentName, name)
{
	m_bAddToHierarchy = bAddToHier;
	if (bAddToHier)
	{
		AddToHierarchy();
	}
	SetDefaults();
}


CTerrainConfig::~CTerrainConfig()
{
	DeInit();
	if (m_bAddToHierarchy)
	{
		RemoveFromHierarchy();
	}
}

void CTerrainConfig::DeInit()
{
	CObjectTemplate<IObject>::DeInit();
}

void CTerrainConfig::SetDefaults()
{
	// resolution of a sector (grid resolution of squares, not points!)
	m_iSectorResolution = 8;
	// Size of a sector in world space
	m_fSectorSize = 1.0f;
	// Terrain Z scale in world space
	m_fHeightMapZScale = 1.0f;
	// LOD Distance
	m_fLODDistance = 150;
}

void CTerrainConfig::Serialize( IArchive &ar )
{
	if (ar.IsReading())
	{
		SetDefaults();

		// Get the data's version number
		CHashString hszVersion = _T("");

		float version;
		static DWORD msgHash_GetFileVersion = CHashString(_T("GetFileVersion")).GetUniqueID();
		DWORD retval = m_ToolBox->SendMessage(msgHash_GetFileVersion, sizeof(IHashString), &hszVersion);
		if (retval != MSG_HANDLED)
		{
			m_ToolBox->Log(LOGERROR, _T("CTerrainConfig Serialize: Could not get file version!"));
			assert(0);
			return;
		}

		float atofVersion = (float)_tstof(hszVersion.GetString());

		version = atofVersion;

		StdString dummy;
		ar.Read( dummy, _T("TerrainShaderLibrary") );
		ar.Read( dummy, _T("TerrainShaderEffect") );
		ar.Read( m_iSectorResolution, _T("SectorResolution") );
		ar.Read( m_fSectorSize, _T("SectorSize") );
		ar.Read( m_fHeightMapZScale, _T("ZScale") );
		ar.Read( m_fLODDistance, _T("LODDistance") );

		// broadcast terrain config change
		static DWORD msgHash_SetTerrainConfig = CHashString(_T("SetTerrainConfig")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_SetTerrainConfig, sizeof(CTerrainConfig*), this );
	}
	else if (!ar.IsReading())
	{
		ar.Write( _T(""), _T("TerrainShaderLibrary") );
		ar.Write( _T(""), _T("TerrainShaderEffect") );
		ar.Write( m_iSectorResolution, _T("SectorResolution") );
		ar.Write( m_fSectorSize, _T("SectorSize") );
		ar.Write( m_fHeightMapZScale, _T("ZScale") );
		ar.Write( m_fLODDistance, _T("LODDistance") );
	}
}

IHashString *CTerrainConfig::GetComponentType()
{
	return &m_HashName;
}

bool CTerrainConfig::IsKindOf(IHashString *compType)
{
	return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
}

int CTerrainConfig::GetSectorResolution()
{
	return m_iSectorResolution;
}

float CTerrainConfig::GetSectorSize()
{
	return m_fSectorSize;
}

float CTerrainConfig::GetHeightMapZScale()
{
	return m_fHeightMapZScale;
}

float CTerrainConfig::GetLODDistance()
{
	return m_fLODDistance;
}
