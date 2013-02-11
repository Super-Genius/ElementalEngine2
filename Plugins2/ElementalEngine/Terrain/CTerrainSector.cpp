///==========================================================================
/// \file	CTerrainSector.cpp
/// \brief	Implementation of CTerrainSector for Terrain
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

bool SectorSpaceBrushBlitter( Vec3 position, float *pDataMap, int iDataMapSize, BRUSHPARAMS *pBrush, float fAvgValue=0 );
bool TerrainSectorBrushAreaInfo( Vec3 sectorPos, float *pDataMap, int iDataMapSize, TERRAINBRUSHAREAINFO &info );

REGISTER_COMPONENT(CTerrainSector);
REGISTER_MESSAGE_HANDLER(TerrainSectorDeform, OnTerrainSectorDeform, CTerrainSector);
REGISTER_MESSAGE_HANDLER(TerrainSectorSetData, OnTerrainSectorSetData, CTerrainSector);
REGISTER_MESSAGE_HANDLER(TerrainSectorGetData, OnTerrainSectorGetData, CTerrainSector);
REGISTER_MESSAGE_HANDLER(TerrainSectorGetPosition, OnTerrainSectorGetPosition, CTerrainSector );
REGISTER_MESSAGE_HANDLER(TerrainSectorGetBrushAreaInfo, OnTerrainSectorGetBrushAreaInfo, CTerrainSector );
REGISTER_MESSAGE_HANDLER(GetGlobalPosition, OnGetGlobalPosition, CTerrainSector );
REGISTER_MESSAGE_HANDLER(GetGlobalEuler, OnGetGlobalEuler, CTerrainSector );
REGISTER_MESSAGE_HANDLER(GetGlobalScale, OnGetGlobalScale, CTerrainSector );
REGISTER_MESSAGE_HANDLER(GetGlobalTransform, OnGetGlobalTransform, CTerrainSector );
REGISTER_MESSAGE_HANDLER(TerrainSetPath, OnTerrainSetPath, CTerrainSector );

REGISTER_MESSAGE_HANDLER(GetEntityType, OnGetEntityType, CTerrainSector);
REGISTER_MESSAGE_HANDLER(GetParentName, OnGetParentName, CTerrainSector); 
REGISTER_MESSAGE_HANDLER(IsEntityTypeOf, OnIsEntityTypeOf, CTerrainSector); 

IComponent *CTerrainSector::Create(int nArgs, va_list argptr)
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
			self = new CTerrainSector(parentName, name, true);
		}
		else if (nArgs == 3)
		{
			bAddToHier = va_arg(argptr, bool);
			self = new CTerrainSector(parentName, name, bAddToHier);
		}
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

CTerrainSector::CTerrainSector(IHashString *parentName, IHashString *name, bool bAddToHier) : 
	SCENEOBJECTTEMPLATE( CTerrainManager, CTerrainSector, ISceneObject, parentName, name)
{
	m_bAddToHierarchy = bAddToHier;
	if (bAddToHier)
	{
		AddToHierarchy();
	}

	m_pTerrainMgr = dynamic_cast<CTerrainManager*>(m_ToolBox->GetComponent(GetManagerName()));

	// Properties
	m_pHeightMap = NULL;
	m_iHeightMapSize = 0;
	m_iSectorX = 0;
	m_iSectorY = 0;
}

CTerrainSector::~CTerrainSector()
{
	if (m_bAddToHierarchy)
	{
		RemoveFromHierarchy();
	}
}

void CTerrainSector::Init()
{
	static CHashString chmoTypeName(_T("CHeightmapObject"));
	// ensure manager knows this sector exists and where this sector is
	m_pTerrainMgr->SetSectorExistance( GetName(), m_iSectorX, m_iSectorY, true );

	CHashString hszHeightmap( m_szHeightMap );
	HEIGHTMAPDATAPARAMS hmap;

	static DWORD msgHash_GetHeightmapData = CHashString(_T("GetHeightmapData")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_GetHeightmapData, sizeof(hmap), &hmap, &hszHeightmap, &chmoTypeName ) != MSG_HANDLED)
	{
		// if failed load heightmap file
		LOADFILEEXTPARAMS lfep;
		lfep.fileName = const_cast<TCHAR*>((const TCHAR*)m_szHeightMap);
		lfep.bInternalLoad = true;
		static DWORD msgHash_LoadFileByExtension = CHashString(_T("LoadFileByExtension")).GetUniqueID();
		DWORD retVal = m_ToolBox->SendMessage(msgHash_LoadFileByExtension, sizeof(LOADFILEEXTPARAMS), &lfep);

		if (retVal != MSG_HANDLED)
		{
			m_ToolBox->Log( LOGERROR, _T("%s(%i): CTerrainSector: Failed to load highmap data."),__FILE__, __LINE__ );	
			hmap.iWidth = hmap.iHeight = 0;
			hmap.pData = hmap.pathName = NULL;
		}
		else
		{
			//file loading handled, let's send a new message to the object handler
			static DWORD msgHash_GetHeightmapData = CHashString(_T("GetHeightmapData")).GetUniqueID();
			retVal = m_ToolBox->SendMessage(msgHash_GetHeightmapData, sizeof(hmap), &hmap, &hszHeightmap, &chmoTypeName );
			if (retVal != MSG_HANDLED)
			{
				m_ToolBox->Log( LOGERROR, _T("%s(%i): CTerrainSector: Failed to get highmap data."), __FILE__, __LINE__ );	
			}
		}
	}
	m_pHeightMap = (float*)hmap.pData;
	m_iHeightMapSize = hmap.iWidth; // its square
	
	UpdatePositionFromParent();

	NotifyHeightMapChange();
}

void CTerrainSector::DeInit()
{
	// ensure manager knows this sector does not exist
	m_pTerrainMgr->SetSectorExistance( GetName(), m_iSectorX, m_iSectorY, false );

	// delete heightmap object
	CHashString hszHeightmap( m_szHeightMap );
	DELETEOBJECTPARAMS dop;
	dop.name = &hszHeightmap;
	static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof(dop), &dop );

	m_pHeightMap = NULL;
	m_iHeightMapSize = 0;
}

void CTerrainSector::Serialize(IArchive &ar)
{
	if (ar.IsReading())
	{
		ar.Read( m_iSectorX );
		ar.Read( m_iSectorY );
		ar.Read( m_szHeightMap );
	}
	else
	{
		ar.Write( m_iSectorX, _T("SectorX"));
		ar.Write( m_iSectorY, _T("SectorY"));
		ar.Write( m_szHeightMap, _T("HeightMap") );
	}
}

bool CTerrainSector::Update()
{
	if (m_bTransformIsDirty)
	{
		UpdatePositionFromParent();
		m_bTransformIsDirty = false;
	}
	return true;
}

bool CTerrainSector::Deform( BRUSHPARAMS brush, float fAvgHeight )
{
	if (!m_pHeightMap)
		return false;

	if (SectorSpaceBrushBlitter( m_Position, m_pHeightMap, m_iHeightMapSize, &brush, fAvgHeight ))
	{
		// update children only if the blitter modified something
		NotifyHeightMapChange();
	}
	return true;
}

DWORD CTerrainSector::OnTerrainSectorDeform(DWORD size, void *in_params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(BRUSHPARAMS));
	BRUSHPARAMS *pParams = (BRUSHPARAMS*)in_params;

	if (!Deform( *pParams, 0 ))
		return MSG_ERROR;
	return MSG_HANDLED_STOP;
}

DWORD CTerrainSector::OnTerrainSectorGetBrushAreaInfo(DWORD size, void *in_params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(TERRAINBRUSHAREAINFO));
	TERRAINBRUSHAREAINFO *pParams = (TERRAINBRUSHAREAINFO*)in_params;

	if (!m_pHeightMap)
		return MSG_ERROR;
	if (!TerrainSectorBrushAreaInfo( m_Position, m_pHeightMap, m_iHeightMapSize, *pParams ))
		return MSG_ERROR;

	return MSG_HANDLED_STOP;
}

DWORD CTerrainSector::OnTerrainSectorSetData(DWORD size, void *in_params)
{
	VERIFY_MESSAGE_SIZE( size, sizeof(HEIGHTMAPDATAPARAMS) );
	return MSG_HANDLED_PROCEED;
}

DWORD CTerrainSector::OnTerrainSectorGetData(DWORD size, void *in_params)
{
	VERIFY_MESSAGE_SIZE( size, sizeof(HEIGHTMAPDATAPARAMS) );
	HEIGHTMAPDATAPARAMS *pParams = (HEIGHTMAPDATAPARAMS*)in_params;

	pParams->iWidth = pParams->iHeight = m_iHeightMapSize;
	pParams->pData = m_pHeightMap;
	pParams->pathName = &m_szHeightMap;

	return MSG_HANDLED_STOP;
}

DWORD CTerrainSector::OnTerrainSectorGetPosition(DWORD size, void *in_params)
{
	VERIFY_MESSAGE_SIZE( size, sizeof( TERRAINSECTORPOSITIONPARAMS ) );
	TERRAINSECTORPOSITIONPARAMS * pParams = (TERRAINSECTORPOSITIONPARAMS*)in_params;

	float fSectorSize = m_pTerrainMgr->GetMetrics().fSectorSize;

	pParams->SectorX = m_iSectorX;
	pParams->SectorY = m_iSectorY;
	pParams->SectorCorner = m_Position;
	pParams->SectorCenter = m_Position + Vec3( fSectorSize * 0.5f, 0.0, fSectorSize * 0.5f );
	pParams->fSectorSize = fSectorSize;

	return MSG_HANDLED_STOP;
}

DWORD CTerrainSector::OnGetGlobalPosition(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(Vec3));
	Vec3 *pos = (Vec3*)param;

	pos->Set(m_Position);

	return MSG_HANDLED_STOP;
}
	
DWORD CTerrainSector::OnGetGlobalEuler(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(EulerAngle));
	EulerAngle *euler = (EulerAngle*)param;

	euler->Set(m_Orientation);

	return MSG_HANDLED_STOP;
}
	
DWORD CTerrainSector::OnGetGlobalScale(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(Vec3));
	Vec3 *scale = (Vec3*)param;

	scale->Set(m_Scale);

	return MSG_HANDLED_STOP;
}

DWORD CTerrainSector::OnGetGlobalTransform(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(Matrix4x4));
	Matrix4x4 *xform = (Matrix4x4*)param;

	xform->SetFrom4x4(m_Transform.GetMatrix());

	return MSG_HANDLED_STOP;
}

DWORD CTerrainSector::OnTerrainSetPath( DWORD size, void *param )
{
	VERIFY_MESSAGE_SIZE( size, sizeof( IHashString ) );
	IHashString *pNewPath = (IHashString *)param;

	TCHAR szHeightmap[MAX_PATH];
	_stprintf( szHeightmap, _T("%s%s.hmp"), pNewPath->GetString(), GetName()->GetString() );

	// path already correct for height map
	if (_tcsicmp(m_szHeightMap, szHeightmap) == 0)
		return MSG_HANDLED_PROCEED;

	// rename heightmap object
	CHashString hszOldHeightMap(m_szHeightMap);
	CHashString hszNewHeightMap(szHeightmap);
	CHANGEOBJNAME con;
	con.oldName = &hszOldHeightMap;
	con.newName = &hszNewHeightMap;
	static DWORD msgHash_RenameObject = CHashString(_T("RenameObject")).GetUniqueID();
	DWORD retVal = m_ToolBox->SendMessage(msgHash_RenameObject, sizeof(con), &con);
	if (retVal != MSG_HANDLED)
	{
		m_ToolBox->Log( LOGERROR, _T("Failed to rename heightmap object.") );
	}

	m_szHeightMap = szHeightmap;

	return MSG_HANDLED_PROCEED;
}

DWORD CTerrainSector::OnGetEntityType(DWORD size, void *param)
{
	static DWORD msg_GetEntityType = CHashString(_T("GetEntityType")).GetUniqueID();
	return m_ToolBox->SendMessage( msg_GetEntityType, size, param, GetParentName() );
}

DWORD CTerrainSector::OnGetParentName(DWORD size, void *param)
{
	static DWORD msg_GetParentName = CHashString(_T("GetParentName")).GetUniqueID();
	return m_ToolBox->SendMessage( msg_GetParentName, size, param, GetParentName() );
}

DWORD CTerrainSector::OnIsEntityTypeOf(DWORD size, void *param)
{
	static DWORD msg_IsEntityTypeOf = CHashString(_T("IsEntityTypeOf")).GetUniqueID();
	return m_ToolBox->SendMessage( msg_IsEntityTypeOf, size, param, GetParentName() );
}

void CTerrainSector::UpdatePositionFromParent()
{
	static DWORD msgHash_GetGlobalPosition = CHashString(_T("GetGlobalPosition")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetGlobalPosition, sizeof(Vec3), &m_Position, GetParentName());
	static DWORD msgHash_GetGlobalEuler = CHashString(_T("GetGlobalEuler")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetGlobalEuler, sizeof(EulerAngle), &m_Orientation, GetParentName());
	static DWORD msgHash_GetGlobalScale = CHashString(_T("GetGlobalScale")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetGlobalScale, sizeof(Vec3), &m_Scale, GetParentName());
	static DWORD msgHash_GetGlobalTransform = CHashString(_T("GetGlobalTransform")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetGlobalTransform, sizeof(Matrix4x4), &m_Transform, GetParentName());	

	float fSectorSize = m_pTerrainMgr->GetMetrics().fSectorSize;
	m_Position += Vec3( (float)m_iSectorX * fSectorSize, 0.0f, (float)m_iSectorY * fSectorSize );

	m_Transform.SetTranslation(m_Position);
}

///	\brief	send TerrainSectorSetData to EE
void CTerrainSector::NotifyHeightMapChange()
{
	HEIGHTMAPDATAPARAMS sethmap;
	sethmap.pData = m_pHeightMap;
	sethmap.iWidth = sethmap.iHeight = m_iHeightMapSize;
	static DWORD msgHash_TerrainSectorSetData = CHashString(_T("TerrainSectorSetData")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_TerrainSectorSetData, sizeof(sethmap), &sethmap, GetName());
}
