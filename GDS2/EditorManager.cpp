///============================================================================
/// \file	EditorManager.cpp
/// \brief	Registers editor plugins
/// \date	5-19-2005
/// \author	Kyle Swaim
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
#include "EditorManager.h"
#include "Vec3.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT_SINGLETON( CEditorManager );

REGISTER_MESSAGE_HANDLER( RegisterEditor, OnRegisterEditor, CEditorManager );
REGISTER_MESSAGE_HANDLER( RegisterTool, OnRegisterTool, CEditorManager );
REGISTER_MESSAGE_HANDLER( LeftClickObject, OnLeftClickObject, CEditorManager );
REGISTER_MESSAGE_HANDLER( LeftClickView, OnLeftClickView, CEditorManager );
REGISTER_MESSAGE_HANDLER( DblClick, OnDblClick, CEditorManager );
REGISTER_MESSAGE_HANDLER( GetLinkableObjects, OnGetLinkableObjects, CEditorManager );
REGISTER_MESSAGE_HANDLER( SetPickingGroups, OnSetPickingGroups, CEditorManager );

CEditorManager::CEditorManager() : m_HashName( _T("CEditorManager") )
{
	m_EditorMap.clear();
	m_ToolMap.clear();
	m_LinkMap.clear();
	m_ToolBox = EngineGetToolBox();

	m_iCurrentCollisionGroup = ALL;

	// Set the control group not to collide with objects or terrain


	LoadLinkMap( StdString(theApp.GetAppPath() + "\\objectproperties\\LinkMap.xml") );
}

CEditorManager::~CEditorManager()
{
	m_EditorMap.clear();
	m_ToolMap.clear();
	m_LinkMap.clear();
}

/// \brief	return a string of the component type
/// \return a reference to this class's textual classname
IHashString *CEditorManager::GetComponentType()
{
	return &m_HashName;
}

/// \brief	Checks if this is similar to it's base class's component
/// \param	compType - the type to compare this against
/// \return	true if it is simlar, else false
bool CEditorManager::IsKindOf(IHashString *compType)
{
	return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
}

/// \brief	Get a singleton instance of this class
/// \return	a singleton instance of this class
IComponent *CEditorManager::Create( int nArgs, va_list argptr )
{
	return SINGLETONINSTANCE( CEditorManager );
}

/// \brief	Look to see if a link is valid
/// \param	src the object linking to another lobject
/// \param	dst the object being linked to
/// \return	true if valid, false if not valid
bool CEditorManager::IsLinkValid(DWORD src, DWORD dst)
{
	LINKMAP::iterator iter;

	for( iter = m_LinkMap.begin(); iter != m_LinkMap.end( ); iter++ )
	{
		if( (iter->first == src) && (iter->second == dst) )
		{
			return true;
		}
	}

	return false;
}

/// \brief	Register an object with an editor
/// \param	size - the size of the message
/// \param	params - the message
DWORD CEditorManager::OnRegisterEditor( DWORD size, void *params )
{
	REGISTER_EDITOR_MESSAGE *regMess;
	EDITABLELIST::iterator elIter;
	EDITABLELIST *el;

	VERIFY_MESSAGE_SIZE( size, sizeof(REGISTER_EDITOR_MESSAGE) );
	regMess = (REGISTER_EDITOR_MESSAGE *)params;

	el = regMess->objectNames;
	// loop through component types and save off the editor component type
	for(elIter = el->begin(); elIter != el->end(); elIter++)
	{
		// insert editor into multimap
		m_EditorMap.insert(LINKPAIR((*elIter)->GetUniqueID(), regMess->editorName->GetUniqueID()));
	}

	return MSG_HANDLED_PROCEED;
}

DWORD CEditorManager::OnRegisterTool( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(REGISTER_TOOL_MESSAGE) );
	REGISTER_TOOL_MESSAGE *regMess = (REGISTER_TOOL_MESSAGE *)params;
	
	PICKABLELIST::iterator iter;
	for (iter = regMess->objectNames->begin(); iter != regMess->objectNames->end(); ++iter)
	{
		m_ToolMap.insert(TOOLMAP::value_type((*iter)->GetUniqueID(), regMess->toolName->GetUniqueID()));
	}

	return MSG_HANDLED_PROCEED;
}

// send a message to all the registered editors of this component type
DWORD CEditorManager::EnumEditorsMessage(IHashString *compType, DWORD msg, DWORD size, void *params)
{
	EDITORMAP::iterator emIter;
	EDITORMAP::iterator emEndIter;
	DWORD retVal;

	// default that we didn't do anything with it
	retVal = MSG_NOT_HANDLED;

	if( compType )
	{
		emEndIter = m_EditorMap.upper_bound(compType->GetUniqueID());
		for(emIter = m_EditorMap.lower_bound(compType->GetUniqueID()); 
				emIter != emEndIter; ++emIter)
		{
			const TCHAR *editorName = m_ToolBox->GetHashString(emIter->second);
			if(editorName != NULL)
			{
				CHashString hashEditorName(editorName);
				// send the message to the specific object
				retVal = m_ToolBox->SendMessage(msg, size, params, NULL, &hashEditorName);
				if((retVal != MSG_HANDLED) && (retVal != MSG_NOT_HANDLED))
					break;
			}
			retVal = MSG_HANDLED_PROCEED;
		}
	}

	return retVal;
}

DWORD CEditorManager::EnumToolsMessage(IHashString *objectCompType, DWORD msg, DWORD size, void *params)
{
	if (objectCompType)
	{
		TOOLMAP::iterator startIter = m_ToolMap.lower_bound(objectCompType->GetUniqueID());
		if (startIter != m_ToolMap.end())
		{
            // go on only if some tool registered for such pickable object
			TOOLMAP::iterator endIter = m_ToolMap.upper_bound(objectCompType->GetUniqueID());
			DWORD retVal = MSG_NOT_HANDLED;
			for (TOOLMAP::iterator iter = startIter; iter != endIter; ++iter)
			{
				const TCHAR *toolName = m_ToolBox->GetHashString(iter->second);
				if (toolName != NULL)
				{
					CHashString hashToolName(toolName);
					DWORD result = m_ToolBox->SendMessage(msg, size, params, NULL, &hashToolName);
					if (result == MSG_HANDLED)
					{
						retVal = MSG_HANDLED;
					}
					else if (result != MSG_NOT_HANDLED)
					{
						retVal = result;
						break;
					}
				}
			}
			return retVal;
		}
	}
	return MSG_NOT_HANDLED;
}

DWORD CEditorManager::OnLeftClickObject( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(MOUSECLICKPARAMS) );
	MOUSECLICKPARAMS *message = (MOUSECLICKPARAMS *)params;

	CLICKOBJECTPARAMS LClickObjParams;
	Vec3 hitPos;
	// given an object explicitly (like from hierarchy) don't do a raycast
	LClickObjParams.bIntersectionValid = false;
	LClickObjParams.object = message->object;
	LClickObjParams.compType = message->compType;

	// broadcast click object messages to registered editors
	static DWORD msgHash_LeftClickEditorObject = CHashString(_T("LeftClickEditorObject")).GetUniqueID();
	return EnumEditorsMessage( LClickObjParams.compType, msgHash_LeftClickEditorObject, sizeof(LClickObjParams), &LClickObjParams );
}

/// \brief	Route the LeftClickView message to the object
/// \param	size - the size of the message
/// \param	params - the message
/// \return	return message
DWORD CEditorManager::OnLeftClickView( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(MOUSECLICKPARAMS) );
	MOUSECLICKPARAMS *message = (MOUSECLICKPARAMS *)params;

	Vec3 hitPos;
	// do raycast, fill appropriate message parameters
	DoMouseRaycast( 0, message, &hitPos );
	message->hitPosition = &hitPos;

	static DWORD msgHash_LeftClickTool = CHashString(_T("LeftClickTool")).GetUniqueID();
	DWORD result = EnumToolsMessage(message->pickableCompType, msgHash_LeftClickTool, sizeof(MOUSECLICKPARAMS), message);
	if (result == MSG_HANDLED)
	{
		// Some tool has processed the message, so it was not editable object
		// hit. Therefore LeftClickObject must not be sent.
		return MSG_HANDLED_PROCEED;
	}
	if (result != MSG_NOT_HANDLED)
	{
		// Error appeared during message processing by tool.
		return result;
	}

	// broad cast that any object was clicked
	static DWORD msgHash_LeftClickObject = CHashString(_T("LeftClickObject")).GetUniqueID();
	return m_ToolBox->SendMessage(msgHash_LeftClickObject, sizeof(MOUSECLICKPARAMS), message);

	message->hitPosition = NULL;
}

/// \brief	Route the OnClick message to the specific manager/object
/// \param	size - the size of the message
/// \param	params - the message
/// \return	return message
DWORD CEditorManager::OnDblClick( DWORD size, void *params )
{
	CLICKOBJECTPARAMS *message;

	VERIFY_MESSAGE_SIZE( size, sizeof(CLICKOBJECTPARAMS) );
	message = (CLICKOBJECTPARAMS *)params;

	static DWORD msgHash_DblClickObject = CHashString(_T("DblClickObject")).GetUniqueID();
	return EnumEditorsMessage( message->compType, msgHash_DblClickObject, size, params);
}

/// \brief	Given an Object type return the list of objects that
///			it can be linked to. If no object is given default
///			to returning objects that can be linked to the world
///			object.
/// \param	size - the size of the message
/// \param	params - the message
/// \return	return message
DWORD CEditorManager::OnGetLinkableObjects( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(LINKABLEOBJECTS) );
	LINKABLEOBJECTS *message = (LINKABLEOBJECTS*)params;
	list<DWORD> *linkableObjects;

	LINKMAP::iterator iter;
	LINKMAP::iterator endIter;

	if (message->key == NULL)
	{
		m_ToolBox->Log(LOGERROR, _T("Error: OnGetLinkableObject key cannot be NULL\n"));
		return MSG_ERROR;
	}

	linkableObjects = message->linkableObjects;
	if (linkableObjects == NULL)
	{
		m_ToolBox->Log(LOGERROR, _T("Error: OnGetLinkableObject linkable objects cannot be NULL\n"));
		return MSG_ERROR;
	}

	IHashString *tKey = message->key;
	endIter = m_LinkMap.upper_bound(tKey->GetUniqueID());
	for(iter = m_LinkMap.lower_bound(tKey->GetUniqueID()); iter != endIter; ++iter)
	{
		DWORD objID = iter->second;
		if(objID != NULL)
		{
			// Add it to the list
			linkableObjects->push_back(objID);
		}
	}

	return MSG_HANDLED_STOP;
}

DWORD CEditorManager::OnSetPickingGroups( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(COLLISIONGROUP) );
	COLLISIONGROUP *message = (COLLISIONGROUP*)params;

	m_iCurrentCollisionGroup = *message;

	return MSG_HANDLED_STOP;
}

/// \brief	Populate the LINKMAP from an XML file
/// \param	fileName - The name of the XML file to load
void CEditorManager::LoadLinkMap( StdString &fileName )
{
	CREATEARCHIVE ca;
	IXMLArchive *ar;
	IToolBox *toolBox = EngineGetToolBox();

	StdString elementName;
	StdString source;
	StdString dest;

	CHashString streamType(_T("File"));

	ca.streamData = (void*)(fileName.c_str());
	ca.mode = STREAM_MODE_READ;
	ca.streamType = &streamType;

	// Try opening the archive
	static DWORD msgHash_CreateXMLArchive = CHashString(_T("CreateXMLArchive")).GetUniqueID();
	if (toolBox->SendMessage(msgHash_CreateXMLArchive, sizeof(CREATEARCHIVE), &ca) != MSG_HANDLED) 
	{ 
		std::string szText; 
		szText = "Failed to load xml format file: "; 
		szText += fileName; 
		::MessageBox(NULL, szText.c_str(), "Warning!", MB_OK|MB_ICONEXCLAMATION); 
	}

	// Start reading from the archive
	ar = dynamic_cast<IXMLArchive *>(ca.archive);

	// skip the header info
	ar->GetNode(elementName);

	while (ar->GetNode(elementName))
	{
		if (elementName == _T("Link"))
		{
			ar->Read(source);
			ar->Read(dest);
			CHashString hashSrc(source);
			CHashString hashDest(dest);
			m_LinkMap.insert( LINKPAIR(hashDest.GetUniqueID(), hashSrc.GetUniqueID() ) );
		}
	}

	ar->Close();
}

void CEditorManager::DoMouseRaycast( int iButton, MOUSECLICKPARAMS *inout_mouseParams, Vec3 *out_pHitPos )
{
	RAYFROMMOUSEPARAMS rayfrommouse;
	rayfrommouse.mouseX = inout_mouseParams->point->x;
	rayfrommouse.mouseY = inout_mouseParams->point->y;
	Vec3 rayPos, rayDir;
	rayfrommouse.rayPos = &rayPos;
	rayfrommouse.rayDir = &rayDir;
	static DWORD msgHash_GetRayFromMouseCoords = CHashString(_T("GetRayFromMouseCoords")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_GetRayFromMouseCoords, sizeof(rayfrommouse), &rayfrommouse ) == MSG_HANDLED)
	{
		RAYCOLLISIONLIST listCollisions;
		RAYCASTLISTPARAMS raycast;
//		RAYCOLLISION listCollisions;
//		RAYCASTPARAMS raycast;
		raycast.rayPos = rayfrommouse.rayPos;
		raycast.rayDir = rayfrommouse.rayDir;
		raycast.listRayCollisions = &listCollisions;
		//raycast.collisionGroup = m_iCurrentCollisionGroup;
		raycast.group = m_iCurrentCollisionGroup;
		static DWORD msgHash_RaycastList = CHashString(_T("RaycastList")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_RaycastList, sizeof(raycast), &raycast );

		RAYCOLLISIONLIST::iterator nearestPickable = listCollisions.end();
		RAYCOLLISIONLIST::iterator nearest = listCollisions.end();
		for (RAYCOLLISIONLIST::iterator iter = listCollisions.begin(); iter != listCollisions.end(); ++iter)
		{
			if (m_ToolMap.find(iter->objectType->GetUniqueID()) == m_ToolMap.end())
			{
				// No tool registered for this pickable object.
				if ((nearest == listCollisions.end()) || (iter->distance < nearest->distance))
				{
					nearest = iter;
				}
			}
			else
			{
				// There is tool registered for this pickable object.
				if ((nearestPickable == listCollisions.end()) || (iter->distance < nearestPickable->distance))
				{
					nearestPickable = iter;
				}
			}
		}

		if (nearestPickable != listCollisions.end())
		{
			// Found tool registered for pickable object.
			// Such pickable object has higher priority, using it!
			nearest = nearestPickable;
		}

		if (nearest != listCollisions.end())
		{
			GETCOMPONENTTYPEPARAMS comptypemsg;
			comptypemsg.name = nearest->parentobject;

			static DWORD msgHash_GetComponentType = CHashString(_T("GetComponentType")).GetUniqueID();
			if (m_ToolBox->SendMessage(msgHash_GetComponentType, sizeof(comptypemsg), &comptypemsg ) != MSG_HANDLED)
			{
				//error?
			}

			out_pHitPos->Set(nearest->hitPoint[0], nearest->hitPoint[1], nearest->hitPoint[2]) ;
			inout_mouseParams->object = nearest->parentobject;
			inout_mouseParams->compType = comptypemsg.componenttype;
			inout_mouseParams->pickableObject = nearest->object;
			inout_mouseParams->pickableCompType = nearest->objectType;
		}
	}
}
