///============================================================================
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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT(CWorldEventRenderObject);

IComponent *CWorldEventRenderObject::Create(int nArgs, va_list argptr)
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
		self = new CWorldEventRenderObject(parentName, name);
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

CWorldEventRenderObject::CWorldEventRenderObject(IHashString *parentName, IHashString *name) : 
	SCENEOBJECTTEMPLATE( CRenderManager, CWorldEventRenderObject, IRenderObject, parentName, name)
{
	m_pEditor = SINGLETONINSTANCE(CWorldEventEditor);
	m_IRenderer = NULL;
}

CWorldEventRenderObject::~CWorldEventRenderObject()
{
	DeInit();
}

void CWorldEventRenderObject::Init()
{
	GETRENDERERINTERFACEPARAMS renderInterfaceParam;
	static DWORD msgHash_GetRendererInterface = CHashString(_T("GetRendererInterface")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_GetRendererInterface, sizeof(GETRENDERERINTERFACEPARAMS), &renderInterfaceParam ) == MSG_HANDLED)
		m_IRenderer = renderInterfaceParam.m_RendererInterface;

	SCENEOBJECTPARAMS ubbp;
	ubbp.pObject = this;
	ubbp.sortID = RENDEROBJECT3D;
	static DWORD msgHash_UpdateBoundingBox = CHashString("UpdateBoundingBox").GetUniqueID();
	m_ToolBox->SendMessage(msgHash_UpdateBoundingBox, sizeof(SCENEOBJECTPARAMS), &ubbp );
}

bool CWorldEventRenderObject::Update()
{
	return true;
}

void CWorldEventRenderObject::DeInit()
{
	SCENEOBJECTPARAMS ubbp;
	ubbp.pObject = this;
	ubbp.sortID = RENDEROBJECT3D;
	static DWORD msgHash_RemoveBoundingBoxedObject = CHashString("RemoveBoundingBoxedObject").GetUniqueID();
	m_ToolBox->SendMessage(msgHash_RemoveBoundingBoxedObject, sizeof( SCENEOBJECTPARAMS), &ubbp );
}

bool CWorldEventRenderObject::Render( UINT pass, IEffect * override )
{
	static DWORD msgHash_GetWorldEventInfo = CHashString(_T("GetWorldEventInfo")).GetUniqueID();
	static DWORD msgHash_GetGlobalPosition = CHashString(_T("GetGlobalPosition")).GetUniqueID();
	static DWORD msgHash_QueryWorldEventTriggers = CHashString(_T("QueryWorldEventTriggers")).GetUniqueID();
	CHashString hszStateObjectType(_T("CQHStateObject"));

	unsigned char lineColorSelected[] = { 255, 255, 255 };
	unsigned char lineColorUnselected[] = { 180, 180, 180 };
	unsigned char *lineColor = lineColorUnselected;

	if( (pass == 0) && (m_IRenderer) )
	{
		// find all the world trigger events
		WORLDEVENTTRIGGERMAP WorldEventTriggerMap;
		WORLDEVENTTRIGGERMAP::iterator itrMap;

		QUERYWORLDEVENTTRIGGERSPARAMS qwetp;
		qwetp.pWorldEventTriggerMap = &WorldEventTriggerMap;
		m_ToolBox->SendMessage(msgHash_QueryWorldEventTriggers, sizeof(qwetp), &qwetp);

		for (itrMap = WorldEventTriggerMap.begin(); itrMap != WorldEventTriggerMap.end(); itrMap++)
		{
			// get the name of the world event trigger
			const TCHAR *pWorldEventTriggerName = m_ToolBox->GetHashString( itrMap->first );
			// get the info about this world event trigger
			WORLDEVENTINFOPARAMS worldEventTriggerInfo;
			m_ToolBox->SendMessage(msgHash_GetWorldEventInfo, sizeof(worldEventTriggerInfo), &worldEventTriggerInfo, &CHashString(pWorldEventTriggerName) );

			if (!worldEventTriggerInfo.bUseEntityName)
				continue;

			// get position of entity associated with trigger
			Vec3 srcEntityPos;
			if (m_ToolBox->SendMessage(msgHash_GetGlobalPosition, sizeof(srcEntityPos), &srcEntityPos, &worldEventTriggerInfo.hszEntityName ) != MSG_HANDLED)
				continue;

			// find out if src entity is the currently selected object
			bool bCurrentlySelected = (*m_pEditor->GetSelectedObject() == worldEventTriggerInfo.hszEntityName);
			if (bCurrentlySelected)
				lineColor = lineColorSelected;
			else
				lineColor = lineColorUnselected;

			// get list of world events associated with this world event trigger
			WORLDEVENTLIST::iterator itrList;
			for (itrList = itrMap->second.begin(); itrList != itrMap->second.end(); itrList++)
			{
				const TCHAR *pWorldEventName = m_ToolBox->GetHashString( *itrList );
				CHashString hszWorldEventName(pWorldEventName);

				// get the name of the event that this world event trigger fires
				WORLDEVENTINFOPARAMS worldEventInfo;
				m_ToolBox->SendMessage(msgHash_GetWorldEventInfo, sizeof(worldEventInfo), &worldEventInfo, &hszWorldEventName );

				if (!worldEventInfo.bUseEntityName)
					continue;

				// get position of entity associated with target event
				Vec3 dstEntityPos;
				if (m_ToolBox->SendMessage(msgHash_GetGlobalPosition, sizeof(dstEntityPos), &dstEntityPos, &worldEventInfo.hszEntityName ) != MSG_HANDLED)
					continue;

				// draw line from trigger entity to target entity
				m_IRenderer->Draw3DLine( srcEntityPos.x, srcEntityPos.y, srcEntityPos.z, 
					dstEntityPos.x, dstEntityPos.y, dstEntityPos.z, 
					lineColor[0], lineColor[1], lineColor[2], 1.0f );
			}
		}
	}
	return true;
}

bool CWorldEventRenderObject::IsLessThan( const IRenderObject * obj, DWORD sortMethod, int curRenderPass )
{
	return true;
}

DWORD CWorldEventRenderObject::GetRenderPriority()
{
    return 0;
}

UINT CWorldEventRenderObject::GetNumPass()
{
	return 1;
}

void CWorldEventRenderObject::GetBoundingSphere( Vec3 &position, float &radius )
{
	position.Set( 0,0,0 );
	radius = 999999.0f;
}

IHashString * CWorldEventRenderObject::GetBaseSortClass()
{
	return GetComponentType();
}

bool CWorldEventRenderObject::IsAlphaObject()
{
	return false;
}

IMeshObject* CWorldEventRenderObject::GetMeshObject()
{
	return NULL;
}
