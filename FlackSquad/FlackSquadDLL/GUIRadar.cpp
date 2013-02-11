///==========================================================================
/// \file	GUIRadar.cpp
/// \brief	Radar for FlackSquad
/// \date	01/10/07
/// \author Nick Tourte
///==========================================================================

#include "StdAfx.h"
#include "GUIRadar.h"
#include "entityquerydefs.h"
#include "luaobjdistquerylist.h"
#include "gui\guidefines.h"
#include "performanceprofiler.h"
#include "CRenderObject.h"

// for sqrt... we don't have a sqrt func...
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CHashString CGUIRadar::radarCT = _T("CGUIRadar");

REGISTER_COMPONENT(CGUIRadar);

REGISTER_MESSAGE_HANDLER(SetPosition, OnSetPosition, CGUIRadar);
REGISTER_MESSAGE_HANDLER(SetSize, OnSetSize, CGUIRadar);
REGISTER_MESSAGE_HANDLER(GetPosition, OnGetPosition, CGUIRadar);
REGISTER_MESSAGE_HANDLER(GetSize, OnGetSize, CGUIRadar);

REGISTER_MESSAGE_HANDLER(SetTurretPosition, OnSetTurretPosition, CGUIRadar);
REGISTER_MESSAGE_HANDLER(SetTurretBearing, OnSetTurretBearing, CGUIRadar);
REGISTER_MESSAGE_HANDLER(SetRadarRadius, OnSetRadarRadius, CGUIRadar);
REGISTER_MESSAGE_HANDLER(CheckNameInRadarRange, OnCheckNameInRadarRange, CGUIRadar);
//REGISTER_MESSAGE_HANDLER(FrameRateCountMessage, OnGetFrameRate, CGUIRadar);

REGISTER_MESSAGE_HANDLER( GetFrameRate, OnGetFrameRate, CGUIRadar ); 


CGUIRadar::CGUIRadar(IHashString *parentName, IHashString *name):
	OBJECTTEMPLATE(CGUIManager, CGUIRadar, IGUIReference, parentName, name)
{
	m_fSizeX = 0.0f;
	m_fSizeY = 0.0f;
	m_fPosX = 0.0f;
	m_fPosY = 0.0f;

	m_fTurretBearing = 0.0f;
	m_fRadarRadius = 8000.0f; // default

	GETRENDERERINTERFACEPARAMS renderInterface;

	static DWORD msgGetRendererInterface = CHashString( _T("GetRendererInterface") ).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgGetRendererInterface, sizeof(GETRENDERERINTERFACEPARAMS), &renderInterface, NULL, NULL);

	m_Renderer = renderInterface.m_RendererInterface;
	m_pBlipTexture = NULL;
	m_fPreviousBlipUpdateTime = 0;
	m_fPreviousBlipPosUpdateTime = 0;

}

CGUIRadar::~CGUIRadar()
{}

IComponent *CGUIRadar::Create(int nArgs, va_list argptr)
{
	IHashString *name;
	IHashString *parentName;
	IObject *self;

	name = va_arg(argptr, IHashString *);
	parentName = va_arg(argptr, IHashString *);
	// create the object however you must create it
	try
	{
		// try and create an instance
		self = new CGUIRadar(parentName, name);
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

void CGUIRadar::Serialize(IArchive &ar)
{
	if (ar.IsReading())
	{
		ar.Read(m_fRadarRadius, _T("RadarRadius"));
	}
	else
	{
		ar.Write(m_fRadarRadius, _T("RadarRadius"));
	}
}

bool CGUIRadar::Update()
{
	PERFORMANCE_PROFILER_AUTO(GUIRadar_Update_Query);
	
	static CHashString hszEnemy(_T("enemy"));
	static CHashString hszEnemyBoss(_T("enemyBoss"));
	DWORD retval;

	// run radar at reduced framerate
	float fTimeDelta;
	const float fBlipFrametime = 1.0f; // 1Hz
	const float fBlipPosFrametime = 1.0f / 10.0f; // 10Hz

	// reduced framerate blip update
	fTimeDelta = m_Timer->GetTime() - m_fPreviousBlipUpdateTime;
	if (fTimeDelta > fBlipFrametime)
	{
		GETOBJECTTYPEQUERYPARAMS gotqp;
		OBJECTTYPEQUERYLIST queryList;
		gotqp.type = &hszEnemy;
		gotqp.pObjectsList = &queryList;

		m_mEntityTypes.clear();
		
		CREATEARCHIVE ca;
		static CHashString memType(_T("Memory"));

		ca.mode = STREAM_MODE_WRITE | STREAM_MODE_READ;
		ca.streamData = NULL;
		ca.streamSize = 0;
		ca.streamType = &memType;

		static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca);
		
		if (ca.archive == NULL)
		{
			m_ToolBox->Log(LOGWARNING, _T("CGUIRadar: could not create archive!\n"));
			return false;
		}

		static DWORD msgSubmitQuery = CHashString( _T("SubmitTypeQuery") ).GetUniqueID();
		retval = m_ToolBox->SendMessage(msgSubmitQuery, sizeof(GETOBJECTTYPEQUERYPARAMS), &gotqp);
		if (retval == MSG_HANDLED)
		{
			for (OBJECTTYPEQUERYLIST::iterator iter = queryList.begin(); iter != queryList.end(); iter++)
			{
				IHashString *currentName = (*iter);
				if (currentName)
				{
					IArchive *archive = ca.archive;
					archive->SetIsWriting(true);
					archive->SeekTo(0);						
					
					DATABASEATTRIBUTEPARAMS dap;
					static CHashString hsIsAliveAttribute(_T("IsAlive"));
					dap.m_AttributeName = &hsIsAliveAttribute;
					dap.m_AttributeArchive = archive;
					static DWORD msgGetAttribute = CHashString(_T("GetAttribute")).GetUniqueID();
					m_ToolBox->SendMessage(msgGetAttribute, sizeof(DATABASEATTRIBUTEPARAMS), &dap, currentName);

					bool bIsAlive = false;

					archive->SetIsWriting(false);
					archive->SeekTo(0);						
					archive->Read(bIsAlive);
					if (bIsAlive)
					{
						m_mEntityTypes.insert(pair<DWORD, CHashString>(currentName->GetUniqueID(), hszEnemy));
					}
				}
			}
		}
		ca.archive->Close();

		m_fPreviousBlipUpdateTime = m_Timer->GetTime();
	}

	// reduced framerate blip update
	fTimeDelta = m_Timer->GetTime() - m_fPreviousBlipPosUpdateTime;
	if (fTimeDelta > fBlipPosFrametime)
	{
		m_vEnemyPos.clear();
		m_vEnemyBossPos.clear();

		ETYPEMAP::iterator typeIter;
		typeIter = m_mEntityTypes.begin();
		while (typeIter != m_mEntityTypes.end())
		{
			CHashString name;
			name = m_ToolBox->GetHashString(typeIter->first);
			Vec3 pos;

			static DWORD msgGetGlobalPosition = CHashString( _T("GetGlobalPosition") ).GetUniqueID();
			retval = m_ToolBox->SendMessage(msgGetGlobalPosition, sizeof(Vec3), &pos, &name);

			if (retval == MSG_HANDLED)
			{
				float x, z;
				x = m_TurretPos.x - pos.x;
				z = m_TurretPos.z - pos.z;

				x /= m_fRadarRadius;
				z /= m_fRadarRadius;

				if ((abs(x) <= 1.0f) && (abs(z) <= 1.0f))
				{
					float dist = (float)sqrt((x*x) + (z*z));
					if (dist <= 1.0f)
					{
						if (typeIter->second.GetUniqueID() == hszEnemy.GetUniqueID())
						{
							ENEMYPOS ep;
							ep.x = x;
							ep.y = z;
							m_vEnemyPos.push_back(ep);
						}
						else if (typeIter->second.GetUniqueID() == hszEnemyBoss.GetUniqueID())
						{
							ENEMYPOS ep;
							ep.x = x;
							ep.y = z;
							m_vEnemyBossPos.push_back(ep);
						}
					}
				}
			}
			typeIter++;
		}

		m_fPreviousBlipPosUpdateTime = m_Timer->GetTime();
	}
	return true;
}

void CGUIRadar::GetRenderParams(vector<GUIVERTEXINFO> &vertexData, float x, float y, float width, float height, 
								DWORD state, float angle, DWORD color)
{
	if (!m_pBlipTexture)
	{
		m_pBlipTexture = CRenderObject<>::LoadTexture(_T("Textures/white.dds"));
	}

	UINT i;
	for (i=0; i<m_vEnemyPos.size(); i++)
	{
		MathUtil math;
		Vec3 pos;
		pos.x = m_vEnemyPos[i].x * (width / 2.0f);
		pos.y = 0.0f;
		pos.z = -m_vEnemyPos[i].y * (width / 2.0f);
		Matrix3x3 mat;
		EulerAngle euler;
		euler.Set(0.0f, 0.0f, angle+180.0f);
		mat.SetRotation(euler);
		pos *= mat;

//		m_Renderer->Draw2DQuad(pos.x + (xPos + (width / 2.0f)), pos.y + (yPos + (height / 2.0f)), 4.0f, 4.0f, NULL, 0xaf22eb15, 45.0f);
		GUIVERTEXINFO entry;
		LoadEntry(entry, pos.x + (x + (width / 2.0f)), pos.z + (y + (height / 2.0f)), 0xaf22eb15);
		vertexData.push_back(entry);
	}	

	for (i=0; i<m_vEnemyBossPos.size(); i++)
	{
		MathUtil math;
		Vec3 pos;
		pos.x = m_vEnemyBossPos[i].x * (width / 2.0f);
		pos.y = 0.0f;
		pos.z = -m_vEnemyBossPos[i].y * (width / 2.0f);
		Matrix3x3 mat;
		EulerAngle euler;
		euler.Set(0.0f, 0.0f, angle+180.0f);
		mat.SetRotation(euler);
		pos *= mat;

//		m_Renderer->Draw2DQuad(pos.x + (xPos + (width / 2.0f)), pos.y + (yPos + (height / 2.0f)), 5.0f, 5.0f, NULL, 0xef22fb15, 45.0f);
		GUIVERTEXINFO entry;
		LoadEntry(entry, pos.x + (x + (width / 2.0f)), pos.z + (y + (height / 2.0f)), 0xef22fb15);
		vertexData.push_back(entry);
	}
}

void CGUIRadar::LoadEntry(GUIVERTEXINFO &entry, float x, float y, DWORD color)
{
	float fBlipHalfSize = 4.0f;
	entry.iNumPrims = 2;
	
	// NOTE: winding is now cw
	entry.VertexData[0].pos[0] = x-fBlipHalfSize;
	entry.VertexData[0].pos[1] = y;
	entry.VertexData[0].pos[2] = 0.0f;
	entry.VertexData[0].u = 0.0f;
	entry.VertexData[0].v = 0.0f;
	entry.VertexData[0].color = color;

	entry.VertexData[1].pos[0] = x;
	entry.VertexData[1].pos[1] = y-fBlipHalfSize;
	entry.VertexData[1].pos[2] = 0.0f;
	entry.VertexData[1].u = 0.0f;
	entry.VertexData[1].v = 0.0f;
	entry.VertexData[1].color = color;

	entry.VertexData[2].pos[0] = x;
	entry.VertexData[2].pos[1] = y+fBlipHalfSize;
	entry.VertexData[2].pos[2] = 0.0f;
	entry.VertexData[2].u = 0.0f;
	entry.VertexData[2].v = 0.0f;
	entry.VertexData[2].color = color;

	entry.VertexData[3].pos[0] = x;
	entry.VertexData[3].pos[1] = y-fBlipHalfSize;
	entry.VertexData[3].pos[2] = 0.0f;
	entry.VertexData[3].u = 0.0f;
	entry.VertexData[3].v = 0.0f;
	entry.VertexData[3].color = color;

	entry.VertexData[4].pos[0] = x+fBlipHalfSize;
	entry.VertexData[4].pos[1] = y;
	entry.VertexData[4].pos[2] = 0.0f;
	entry.VertexData[4].u = 0.0f;
	entry.VertexData[4].v = 0.0f;
	entry.VertexData[4].color = color;

	entry.VertexData[5].pos[0] = x;
	entry.VertexData[5].pos[1] = y+fBlipHalfSize;
	entry.VertexData[5].pos[2] = 0.0f;
	entry.VertexData[5].u = 0.0f;
	entry.VertexData[5].v = 0.0f;
	entry.VertexData[5].color = color;

	entry.pTexture = m_pBlipTexture;
}

DWORD CGUIRadar::OnSetPosition(DWORD msgSize, void *param)
{
	VERIFY_MESSAGE_SIZE(msgSize, sizeof(POSITIONDATA));
	POSITIONDATA pd = *(POSITIONDATA*) param;

	m_fPosX = pd.m_fXPos;
	m_fPosY = pd.m_fYPos;

	return MSG_HANDLED_STOP;
}

DWORD CGUIRadar::OnGetPosition(DWORD msgSize, void *param)
{
	VERIFY_MESSAGE_SIZE(msgSize, sizeof(POSITIONDATA));
	POSITIONDATA *pd = (POSITIONDATA*) param;

	pd->m_fXPos = m_fPosX;
	pd->m_fYPos = m_fPosY;
	
	return MSG_HANDLED_STOP;
}

DWORD CGUIRadar::OnSetSize(DWORD msgSize, void *param)
{
	VERIFY_MESSAGE_SIZE(msgSize, sizeof(SIZEDATA));
	SIZEDATA sd = *(SIZEDATA*) param;
	
	m_fSizeX = sd.m_fXSize;
	m_fSizeY = sd.m_fYSize;

	return MSG_HANDLED_STOP;
}

DWORD CGUIRadar::OnGetSize(DWORD msgSize, void *param)
{
	VERIFY_MESSAGE_SIZE(msgSize, sizeof(SIZEDATA));
	SIZEDATA *sd = (SIZEDATA*) param;
	
	sd->m_fXSize = m_fSizeX;
	sd->m_fYSize = m_fSizeY;

	return MSG_HANDLED_STOP;
}

DWORD CGUIRadar::OnSetTurretPosition(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(Vec3));
	Vec3 pos = *(Vec3*) param;
	
	m_TurretPos.Set(pos);

	return MSG_HANDLED_STOP;
}

DWORD CGUIRadar::OnSetTurretBearing(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(float));
	float angle = *(float*) param;

	m_fTurretBearing = angle;

	return MSG_HANDLED_STOP;
}

DWORD CGUIRadar::OnSetRadarRadius(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(float));
	float radius = *(float*) param;

	m_fRadarRadius = radius;
	
	return MSG_HANDLED_STOP;
}

DWORD CGUIRadar::OnCheckNameInRadarRange(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(GUIQUERYBYNAME));
	GUIQUERYBYNAME *gqbn = (GUIQUERYBYNAME*) param;
	
	gqbn->bFound = false;
	if (gqbn->name)
	{
		ETYPEMAP::iterator mapIter = m_mEntityTypes.find(gqbn->name->GetUniqueID());
		if (mapIter != m_mEntityTypes.end())
		{
			gqbn->bFound = true;
		}
	}

	return MSG_HANDLED_STOP;
}


DWORD CGUIRadar::OnGetFrameRate(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(float));
	float FPS; 

	//////////////////////////////////////////////////////////////////
	// The GetRenderProfileInfo function is not correctly implemented in
	// DX9Renderer.cpp yet. use m_Timer instead.
/*	RENDERERPROFILEINFO info;
	RENDERERPROFILEINFO* p_info = &info;
	m_Renderer->GetFrameProfileInfo( p_info );
	if ( p_info->FrameRate ) 
	{
		FPS = p_info->FrameRate;
	}
*/

	// Getting FPS from the timer
	FPS  = m_Timer->GetFPS();

	//Debug Log 
	//m_ToolBox->Log( LOGWARNING, _T( "CGUIFPSDisplay - FrameRate: %f.\n" ), FPS );

	// return the FPS as output parameter
	float* ret = (float*)param;
	*ret = FPS;

	return MSG_HANDLED_STOP;
}