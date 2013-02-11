///==========================================================================
/// \file	CGUIManager.cpp
/// \brief	Manager for CEntity objects
///			This file defines CGUIManager which manages the creation
///			and manipulation of CEntity objects. 
///			Inherits from CManager
/// \date	2-22-2005
/// \author	Kenneth Hurley
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

#include "databasedefines.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT_SINGLETON(CGUIManager);

TOLUA_API int tolua_GUIScript_open (lua_State* tolua_S);

REGISTER_MESSAGE_HANDLER(MouseMove, OnMouseMove, CGUIManager);
REGISTER_MESSAGE_HANDLER(LeftMousePress, OnLeftMousePress, CGUIManager);
REGISTER_MESSAGE_HANDLER(LeftMouseRelease, OnLeftMouseRelease, CGUIManager);
REGISTER_MESSAGE_HANDLER(KeyPress, OnKeyPress, CGUIManager);
REGISTER_MESSAGE_HANDLER(KeyRelease, OnKeyRelease, CGUIManager);
REGISTER_MESSAGE_HANDLER(GUIManagerSetActivePage, OnSetActivePage, CGUIManager);
REGISTER_MESSAGE_HANDLER(ChangeObjectName, OnChangeObjectName, CGUIManager);
REGISTER_MESSAGE_HANDLER(FindGUIObject, OnFindObject, CGUIManager);
REGISTER_MESSAGE_HANDLER(Start, OnStart, CGUIManager);
REGISTER_MESSAGE_HANDLER(Stop, OnStop, CGUIManager);
REGISTER_MESSAGE_HANDLER(ClearGUIObjects, OnClearGUIObjects, CGUIManager);
REGISTER_MESSAGE_HANDLER(SetWindowSize, OnSetWindowSize, CGUIManager);
REGISTER_MESSAGE_HANDLER(SetGUIEditorMode, OnSetGUIEditorMode, CGUIManager);
REGISTER_MESSAGE_HANDLER(GetGUIMousePosition, OnGetGUIMousePosition, CGUIManager);
REGISTER_MESSAGE_HANDLER(GetZoomFactor, OnGetZoomFactor, CGUIManager);
REGISTER_MESSAGE_HANDLER(SetZoomFactor, OnSetZoomFactor, CGUIManager);
REGISTER_MESSAGE_HANDLER(SetXScale, OnSetXScale, CGUIManager);
REGISTER_MESSAGE_HANDLER(SetYScale, OnSetYScale, CGUIManager);
REGISTER_MESSAGE_HANDLER(SetActiveMode, OnSetActiveMode, CGUIManager);
REGISTER_MESSAGE_HANDLER(SetGUIRenderToTexture, OnSetGUIRenderToTexture, CGUIManager);
REGISTER_MESSAGE_HANDLER(GetGUIRenderObjects, OnGetGUIRenderObjects, CGUIManager);
REGISTER_MESSAGE_HANDLER(DisableBatchRendering, OnDisableBatchRendering, CGUIManager);
REGISTER_MESSAGE_HANDLER(RenderPageToContext, OnRenderPageToContext, CGUIManager);
REGISTER_MESSAGE_HANDLER(GUIManagerIncrementZoom, OnIncrementZoom, CGUIManager);
REGISTER_MESSAGE_HANDLER(GUIManagerDecrementZoom, OnDecrementZoom, CGUIManager);
REGISTER_MESSAGE_HANDLER(GUIManagerSetPageOffset, OnSetPageOffset, CGUIManager);
REGISTER_MESSAGE_HANDLER(SetGUIWorkingRes, OnSetGUIWorkingRes, CGUIManager);
REGISTER_MESSAGE_HANDLER(CheckGUISystemEmpty, OnCheckGUISystemEmpty, CGUIManager);
REGISTER_MESSAGE_HANDLER(LoadingUpdate, OnLoadingUpdate, CGUIManager);
REGISTER_MESSAGE_HANDLER(QueueGUIStateEvent, OnQueueGUIStateEvent, CGUIManager);
REGISTER_MESSAGE_HANDLER(SetGUIStateObject, OnSetGUIStateObject, CGUIManager);
REGISTER_MESSAGE_HANDLER_PRIORITY(RenderUpdate, OnRenderUpdate, CGUIManager, 0x0000ffff);
REGISTER_MESSAGE_HANDLER(GetGUIScreenSize, OnGetGUIScreenSize, CGUIManager);
REGISTER_MESSAGE_HANDLER(SetGUIFontGlyphDB, OnSetGUIFontGlyphDB, CGUIManager);
REGISTER_MESSAGE_HANDLER(AddGUILanguage, OnAddGUILanguage, CGUIManager);
REGISTER_MESSAGE_HANDLER(RemoveGUILanguage, OnRemoveGUILanguage, CGUIManager);
REGISTER_MESSAGE_HANDLER(SetActiveGUILanguage, OnSetActiveGUILanguage, CGUIManager);
REGISTER_MESSAGE_HANDLER(GetGUILanguages, OnGetGUILanguages, CGUIManager);

// keybinding
REGISTER_MESSAGE_HANDLER(GUIKeyLeft, OnKeyLeft, CGUIManager);
REGISTER_MESSAGE_HANDLER(GUIKeyRight, OnKeyRight, CGUIManager);
REGISTER_MESSAGE_HANDLER(GUIKeyHome, OnKeyHome, CGUIManager);
REGISTER_MESSAGE_HANDLER(GUIKeyEnd, OnKeyEnd, CGUIManager);
REGISTER_MESSAGE_HANDLER(GUIKeyDel, OnKeyDel, CGUIManager);
REGISTER_MESSAGE_HANDLER(GUIKeyBackSpace, OnKeyBackSpace, CGUIManager);
REGISTER_MESSAGE_HANDLER(GUIKeyEnter, OnKeyEnter, CGUIManager);

///
/// Constructor / Destructor
///

CGUIManager::CGUIManager() : 
	CManager(_T("CGUIManager"), GUIPRIORITY, GUIPRIORITY)
{
	m_ActivePage = NULL;

	m_IsRunning = true;
	
	m_pFocusItem = NULL;
	m_iScreenOffsetX = m_iScreenOffsetY = 0;
	m_iScreenSizeX = m_iScreenSizeY = 0;

	m_iResX = m_iResY = 0;

	m_bEditorMode = false;
	m_bSpacePressed = false;
	m_bCtrlPressed = false;
	m_bShiftPressed = false;
	m_bLeftMousePressed = false;

	m_fZoomTable[0] = 0.125f;
	m_fZoomTable[1] = 0.25f;
	m_fZoomTable[2] = 0.333f;
	m_fZoomTable[3] = 0.5f;
	m_fZoomTable[4] = 0.666f;
	m_fZoomTable[5] = 1.f;
	m_fZoomTable[6] = 2.f;
	m_fZoomTable[7] = 3.f;
	m_fZoomTable[8] = 4.f;
	m_fZoomTable[9] = 5.f;

	m_fScaleFactorX = 1.f;
	m_fScaleFactorY = 1.f;
	m_fZoomFactor = 1.f;

	m_iZoomIndex = 5;

	m_iPageOffsetX = m_iPageOffsetY = 0;

	m_iMouseLastPosX = m_iMouseLastPosY = 0;	

	m_iBGColor = 0; 

	m_RenderContext = NULL;
	m_bActiveMode = true;

	static DWORD msgHash_GetMasterScriptState = CHashString(_T( "GetMasterScriptState" )).GetUniqueID();
	DWORD amessageHandle = m_ToolBox->SendMessage(msgHash_GetMasterScriptState, sizeof( lua_State * ), &m_LuaState );
	if( amessageHandle != MSG_HANDLED)
	{
		StdString error;
		error = _T("Error missing Master Script State Data\n");

		// log error
		EngineGetToolBox()->SetErrorValue(WARN_INVALID_OPERATION);
		EngineGetToolBox()->Log(LOGWARNING, error);
    }
	else
	{
		// register our additional structures/handlers with LUA master
		if (m_LuaState)
			tolua_GUIScript_open(m_LuaState);
	}
	
	m_bTickStateWrapper = true;		
	
	GETRENDERERINTERFACEPARAMS renderInterface;

	static DWORD msgHash_GetRendererInterface = CHashString(_T("GetRendererInterface")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_GetRendererInterface, 
					sizeof(GETRENDERERINTERFACEPARAMS), 
					&renderInterface, 
					NULL, 
					NULL);

	m_Renderer = renderInterface.m_RendererInterface;

	m_hsTextGlyphDBFile = _T("");
}

IComponent *CGUIManager::Create(int nArgs, va_list argptr)
{
	// Return the singleton instance of this class
	return SINGLETONINSTANCE( CGUIManager );
}

CGUIManager::~CGUIManager()
{
	DeInit();
}

///
/// Inherited methods
///

void CGUIManager::Update(DWORD tickCount)
{
//	PERFORMANCE_PROFILER_AUTO(GUIManager_update);

	if (!m_VertexBuffer)
	{
		CreateVertexBuffer();
	}

	if(m_IsRunning == true)
	{
		for (UINT i=0; i<m_Animations.size(); i++)
		{
			m_Animations[i]->Update();	
		}

		if(m_ActivePage != NULL )
		{
			m_ActivePage->Update();
		}
	}
	// fire event from queue, if any
	if (m_bTickStateWrapper)
	{
		m_StateWrapper.Update();
	}	

	if (m_VertexBuffer)
	{
		UINT curOffset = m_VertexBufferAllocation.m_Offset;
		void *pDest = NULL;
		m_VertexBuffer->Lock(m_VertexBufferAllocation.m_Offset, m_VertexBufferAllocation.m_Size, 
			(void **)&pDest, VBO_LOCK_DISCARD);

		if(m_ActivePage != NULL )
		{
			UINT written = m_ActivePage->UpdatePageItemVertexBuffer(&pDest, curOffset, m_VertexBuffer);
			curOffset += written;
		}

		m_VertexBuffer->Unlock();
	}
}

bool CGUIManager::DeInit()
{	
	// clear everything out
	OnClearGUIObjects(0, NULL);
	return CManager::DeInit();
}

bool CGUIManager::AddObject( IObject *Object, IHashString *group)
{
	bool retVal;

	assert(Object);
	IHashString *compType = Object->GetComponentType();
	assert(compType);
	if (_tcscmp(compType->GetString(), _T("CGUIAnimation")) == 0)
	{
		CGUIAnimation *anim = (CGUIAnimation*)Object;
		assert(anim);
		m_Animations.push_back(anim);
		return true;
	}

	IObject *srcObject;	
	CHashString oName = Object->GetName();
	GUIMAP::iterator gmmStartIter = m_GUIObjects.lower_bound(oName.GetUniqueID());
	GUIMAP::iterator gmmEndIter = m_GUIObjects.upper_bound(oName.GetUniqueID());

	//Get a range of objects w/ the same name as Object
	for(; gmmStartIter != gmmEndIter; gmmStartIter++)
	{
		srcObject = gmmStartIter->second;
		//check if adding this Object's component type is already in this range
		if(srcObject->GetComponentType()->GetUniqueID() == Object->GetComponentType()->GetUniqueID())
		{
			//cannot add objects w/ same name and same component type
			return false;
		}
	}
	// let the base manager handle the list<> of these
	retVal = CManager::AddObject(Object, group);

	m_GUIObjects[oName.GetUniqueID()] = Object;
	return retVal;
}

IObject *CGUIManager::GetObject(IHashString *guiName, IHashString *CompType)
{
	IObject *foundObject;
	IHashString *foundType;
	GUIMAP::iterator gmmStartIter = m_GUIObjects.lower_bound(guiName->GetUniqueID());
	GUIMAP::iterator gmmEndIter = m_GUIObjects.upper_bound(guiName->GetUniqueID());

	//must pass a component type
	if(CompType == NULL)
	{
		return NULL;
	}

	for(; gmmStartIter != gmmEndIter; gmmStartIter++)
	{
		foundObject = gmmStartIter->second;
		foundType = foundObject->GetComponentType();
		
		//check component type
		if(CompType->GetUniqueID() == foundType->GetUniqueID())
		{
			return foundObject;
		}
	}
	//else could not find Object 
	return NULL;
}


// Mouse movement messages
DWORD CGUIManager::OnMouseMove(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(MOUSECOORDS));
	DWORD retVal = MSG_NOT_HANDLED;
	if (m_GUIObjects.size() == 0)
	{
		return retVal;
	}

	// make local copy of params
	MOUSECOORDS guiMouse = *(MOUSECOORDS*)params;

	m_iMouseLastPosX = guiMouse.m_lXPos;
	m_iMouseLastPosY = guiMouse.m_lYPos;

	float fViewportToGuiX=1.f, fViewportToGuiY=1.f;

	OnGetGUIMousePosition(sizeof(MOUSECOORDS), &guiMouse);

	if(m_ActivePage != NULL)
	{
		retVal = m_ActivePage->OnMouseMove(size, &guiMouse);
	}

	return MSG_HANDLED_PROCEED;
}

// left mouse button down message
DWORD CGUIManager::OnLeftMousePress(DWORD size, void *params)
{
	DWORD retVal;
	if(m_ActivePage)
	{
		retVal = m_ActivePage->OnLeftMousePress(size, params);
		if(retVal == MSG_NOT_HANDLED)
		{
			// remove any focus
			SetFocus(NULL);
		}
		return MSG_HANDLED_PROCEED;
	}
	else
	{
		return MSG_NOT_HANDLED;
	}
}

DWORD CGUIManager::OnLeftMousePressAlt(DWORD size, void *params)
{
	if (m_ActivePage && m_bEditorMode)
	{
		if (!m_bLeftMousePressed)
		{
			m_bLeftMousePressed = true;
			if (m_bSpacePressed && !m_bCtrlPressed)
			{
				DecrementZoom();				
			}
		}
	}
	return MSG_HANDLED_PROCEED;
}

DWORD CGUIManager::OnLeftMouseRelease(DWORD size, void *params)
{
	m_bLeftMousePressed = false;
	if(m_ActivePage)
	{
		m_ActivePage->OnLeftMouseRelease(size, params);		
	}
	return MSG_HANDLED_PROCEED;	
}

DWORD CGUIManager::OnKeyPress(DWORD size, void *params)
{
	if(m_ActivePage)
	{
		m_ActivePage->OnKeyPress(size, params);
	}
	return MSG_HANDLED_PROCEED;	
}

DWORD CGUIManager::OnKeyRelease(DWORD size, void *params)
{
	return MSG_HANDLED_PROCEED;
}

// message to set active page for GUI Manager
DWORD CGUIManager::OnSetActivePage(DWORD size, void *params)
{	
	static CHashString	pageType(_T("CGUIPage"));
	IHashString *pageName;
	CGUIPage *  pageObject;
	DWORD retVal = MSG_ERROR;
	
	// make sure message is valid
	VERIFY_MESSAGE_SIZE(size, sizeof(IHashString));

	if(params == NULL)
	{
		m_ActivePage = NULL;
		return MSG_HANDLED_PROCEED;
	}

	pageName = (IHashString*)params;
	
	pageObject = dynamic_cast<CGUIPage*>(GetObject(pageName, &pageType));

	if(pageObject == NULL)
	{
  		EngineGetToolBox()->SetErrorValue(WARN_PAGE_NOT_FOUND);
		EngineGetToolBox()->Log(LOGWARNING, _T("Could not get page %s\n"), 
			pageName->GetString());
		return retVal;
	}
	else
	{
		// set the activepage
		m_ActivePage = pageObject;
		m_ActivePage->SetRenderContext(m_RenderContext);
		
		return MSG_HANDLED_PROCEED;
	}	
}

DWORD CGUIManager::OnChangeObjectName(DWORD size, void *params)
{
	CHANGEOBJNAME *data;
	IObject *Object;
	IHashString *oldName;
	IHashString *newName;
	IHashString *compType;
	IHashString *checkCompType;
	VERIFY_MESSAGE_SIZE(size, sizeof(CHANGEOBJNAME));
	
	data = (CHANGEOBJNAME*)params;
	oldName = data->oldName;
	newName = data->newName;
	compType = data->compType;

	GUIMAP::iterator gmmStartIter;
	GUIMAP::iterator gmmEndIter; 
	//range of objects w/ the same name

	gmmStartIter = m_GUIObjects.lower_bound(data->newName->GetUniqueID());
	gmmEndIter = m_GUIObjects.upper_bound(data->newName->GetUniqueID());
	
	if(gmmStartIter != gmmEndIter)
	{
		for(; gmmStartIter != gmmEndIter; gmmStartIter++)
		{
			Object = gmmStartIter->second;
			checkCompType = Object->GetComponentType();
			if(checkCompType->GetUniqueID() == compType->GetUniqueID())
			{
				EngineGetToolBox()->SetErrorValue(WARN_NAME_ALREADY_EXISTS);		
				EngineGetToolBox()->Log(LOGWARNING, _T("cannot rename to: %s, componente type exists w/ this name\n"), 
					data->oldName->GetString());
				return MSG_NOT_HANDLED;
			}
		}
	}
	//range of objects w/ the same name
	gmmStartIter = m_GUIObjects.lower_bound(oldName->GetUniqueID());
	gmmEndIter = m_GUIObjects.upper_bound(oldName->GetUniqueID());
	
	if(gmmStartIter == gmmEndIter)
	{
	//	EngineGetToolBox()->SetErrorValue(WARN_NAME_NOT_FOUND);		
	//	EngineGetToolBox()->Log(LOGWARNING, _T("original name:%s not found\n"), oldName->GetString());
		
		// object is not a gui object
		return MSG_NOT_HANDLED;		
	}

	//done w/ error checking
	for(; gmmStartIter != gmmEndIter; gmmStartIter++)
	{
		Object = gmmStartIter->second;
		checkCompType = Object->GetComponentType();
		if(checkCompType->GetUniqueID() == compType->GetUniqueID() || 
			Object->IsKindOf(compType) )
		{
			//found a matching component type...safe to rename
			//remove name from map
			m_GUIObjects.erase(gmmStartIter);
			
			//add object back into map
			m_GUIObjects[newName->GetUniqueID()] = Object;
			return MSG_HANDLED_PROCEED;
		}
	}	
	EngineGetToolBox()->SetErrorValue(WARN_NAME_NOT_FOUND);		
	EngineGetToolBox()->Log(LOGWARNING, _T("original name:%s and component type not found\n"), 
		oldName->GetString());
	return MSG_NOT_HANDLED;
}	
	
DWORD CGUIManager::OnFindObject(DWORD size, void *params)
{

	FINDGUIOBJECT *fgo;
	VERIFY_MESSAGE_SIZE(size, sizeof(FINDGUIOBJECT));
	
	fgo = (FINDGUIOBJECT*)params;

	fgo->object = NULL;
	
	GUIMAP::iterator gmmStartIter = m_GUIObjects.lower_bound(fgo->name->GetUniqueID());
	GUIMAP::iterator gmmEndIter = m_GUIObjects.upper_bound(fgo->name->GetUniqueID());

	for(; gmmStartIter != gmmEndIter; gmmStartIter++)
	{
		fgo->type = gmmStartIter->second->GetComponentType();
		fgo->bFound = true;
		fgo->object = gmmStartIter->second;
		return MSG_HANDLED_STOP;
	}
	//Object w/ name not found
	fgo->bFound = false;
	return MSG_HANDLED_STOP;
}

/// \brief	Start running the physics simulation
DWORD CGUIManager::OnStart(DWORD size, void *params)
{
	m_IsRunning = true;

	return MSG_HANDLED_PROCEED;
}

/// \brief	Stop running the physics simulation
DWORD CGUIManager::OnStop(DWORD size, void *params)
{
	m_IsRunning = false;

	return MSG_HANDLED_PROCEED;
}

/// tells the gui manager what size the window is so it can adjust the mouse position 
/// accordingly
/// \param size = sizeof SETWINDRECTPARAMS
/// \param param = SETWINDRECTPARAMS, window rect information
/// \return MSG_HANDLED_STOP
DWORD CGUIManager::OnSetWindowSize(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(VIEWPORTPARAMS));
	VIEWPORTPARAMS *rect = (VIEWPORTPARAMS*) param;

	m_iScreenOffsetX = rect->offsetx;
	m_iScreenOffsetY = rect->offsety;
	m_iScreenSizeX = rect->width;
	m_iScreenSizeY = rect->height;

	VIEWPORTPARAMS vidmsg;
	memset( &vidmsg, 0, sizeof(vidmsg) );
	static DWORD msgHash_GetVideoSize = CHashString(_T("GetVideoSize")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_GetVideoSize, sizeof(vidmsg), &vidmsg ) == MSG_HANDLED)
	{
		if (m_iScreenSizeX != 0)
		{
			m_fScaleFactorX = (float)vidmsg.width / (float)m_iScreenSizeX;
		}
		if (m_iScreenSizeY != 0)
		{
			m_fScaleFactorY = (float)vidmsg.height / (float)m_iScreenSizeY;
		}
		if (m_ActivePage)
		{
			m_ActivePage->SetWidthRatio(m_fScaleFactorX);
			m_ActivePage->SetHeightRatio(m_fScaleFactorY);
		}
	}
	
	return MSG_HANDLED_STOP;
}
		
// Serialize this object to the archive.
void CGUIManager::Serialize(IArchive &ar)
{
	GUIMAP::iterator gmmIter;
	IObject *Object;
	StdString name;
	StdString compType;

	if(ar.IsReading())
	{

	}
	else 
	{
		// main class
		StdString version = _T("2.0c"); // febraury 2008 serialization
		ar.StartClass(_T("CGUI_"));
		ar.Write(version, _T("Vers"));

		// september 2006 editor info
		ar.Write(m_iResX, _T("XRes"));
		ar.Write(m_iResY, _T("YRes"));

		ar.Write(m_hsTextGlyphDBFile.GetString(), _T("FontGlyphDBFile"));

		// write languages out here
		static float fSaveLangVersion = 1.0f;
		LANGUAGEFILEMAP::iterator langMapIter = m_mLanguages.begin();
		for (; langMapIter != m_mLanguages.end(); langMapIter++)
		{
			StdString szName = m_ToolBox->GetHashString(langMapIter->first);
			
			ar.StartClass(_T("Language"));
			ar.Write(fSaveLangVersion, _T("Version"));			
			ar.Write(szName, _T("Name"));
			ar.Write(langMapIter->second, _T("File"));
			ar.EndClass();
		}

		for(gmmIter = m_GUIObjects.begin(); gmmIter != m_GUIObjects.end();
			gmmIter++)
		{
			Object = gmmIter->second;
			if(Object != NULL)
			{
				// try to cast as a gui element... 
				IGUIElement *guiObj = dynamic_cast<IGUIElement*>(Object);
				if (guiObj) // if successful
				{
					// this check is ultimately for the elements in the editor, so they don't get
					// serialized out
					if (guiObj->ToSerializeOut()) // check to see if we need to write it out
					{
						compType = Object->GetComponentType()->GetString();
						name = Object->GetName()->GetString();
						ar.StartClass(compType); 
						ar.Write(name, _T("name"));

						Object->Serialize(ar);

						ar.EndClass();
					}
				}		
				else // if not successful, object is probably a guipage, go ahead and serialize
				{
					compType = Object->GetComponentType()->GetString();
					name = Object->GetName()->GetString();
					ar.StartClass(compType); 
					ar.Write(name, _T("name"));

					Object->Serialize(ar);

					ar.EndClass();
				}
			}
			else
			{
				EngineGetToolBox()->SetErrorValue(ERR_NULL_POINTER);
				EngineGetToolBox()->Log(LOGWARNING, _T("CGUImanager could not serialize it's gui elements\n"));
				return;
			}
		}


		for (UINT i=0; i<m_Animations.size(); i++)
		{
			ar.StartClass(_T("CGUIAnimation"));
			ar.Write(m_Animations[i]->GetName()->GetString(), _T("name"));
			m_Animations[i]->Serialize(ar);
			ar.EndClass();
		}
		ar.EndClass();
	}
}

bool CGUIManager::DeleteObject(IObject *Object)
{
	return RemoveObject(Object->GetName(), Object->GetComponentType());
}

bool CGUIManager::RemoveObject(IHashString *guiName, IHashString *guiType)
{
	if (_tcscmp(guiType->GetString(), _T("CGUIAnimation")) == 0)
	{
		for (UINT i=0; i<m_Animations.size(); i++)
		{
			if (m_Animations[i]->GetName()->GetUniqueID() == guiName->GetUniqueID())
			{
				CManager::DeleteObject(dynamic_cast<IObject*>(m_Animations[i]));
				m_Animations.erase(m_Animations.begin() + i);				
				return true;
			}
		}
	}
    
	GUIPAGEMESSAGE gmsgPage;
	static CHashString group(_T("CGUIGroup"));
	static CHashString page(_T("CGUIPage"));
	IHashString *compType;
	GUIMAP::iterator gmmStartIter = m_GUIObjects.lower_bound(guiName->GetUniqueID());
	GUIMAP::iterator gmmEndIter = m_GUIObjects.upper_bound(guiName->GetUniqueID());

	// if object is on a page, remove it from the page
	if( guiType->GetUniqueID() != page.GetUniqueID() )
	{
		gmsgPage.name = guiName;
		gmsgPage.compType = guiType;
		// remove gui element from any pages it could be in
		static DWORD msgHash_RemoveGUIElementFromPage = CHashString(_T("RemoveGUIElementFromPage")).GetUniqueID();
		EngineGetToolBox()->SendMessage(msgHash_RemoveGUIElementFromPage, sizeof(GUIPAGEMESSAGE), &gmsgPage, NULL, &page);

	}
	// if object is in a group, remove it from the group
	if ( guiType->GetUniqueID() != group.GetUniqueID() )
	{
		// remove gui element from any groups it could be in
		static DWORD msgHash_RemoveGUIElement = CHashString(_T("RemoveGUIElement")).GetUniqueID();
		EngineGetToolBox()->SendMessage(msgHash_RemoveGUIElement, sizeof(IHashString*), guiName, NULL, &group);
	}

	// remove object from the m_GUIObjects map
	for(; gmmStartIter != gmmEndIter; gmmStartIter++)
	{
		compType = gmmStartIter->second->GetComponentType();
		if(compType->GetUniqueID() == guiType->GetUniqueID())
		{			
			// first check...if object is a page and is active, disable 
			// active page
			if(gmmStartIter->second == m_ActivePage)
			{
				m_ActivePage = NULL;
			}

			// final check...if object had focus, set focus null
			if(gmmStartIter->second == m_pFocusItem)
			{
				m_pFocusItem = NULL;
			}
			
			// call CManager to delete this object from it's list
			CManager::DeleteObject(gmmStartIter->second);
			m_GUIObjects.erase(gmmStartIter);	
			break;
		}
	}
	return true;
}

// hierarchy pump message function
DWORD CGUIManager::PumpHandler(DWORD size, void *data, 
  		IHashString *name, IHashString *compType, MSGFUNCTION msgFunction)
{
	IObject *destObject;
	IHashString *destTypeName;		// the type of the destination
	DWORD retVal = MSG_NOT_HANDLED;
	bool bMsgHandled = false;

	GUIMAP *currentGUIMap = &m_GUIObjects;
	if( name == NULL )
	{
		GUIMAP::iterator gmmIter = currentGUIMap->begin();
		for(; retVal != MSG_HANDLED_STOP && gmmIter != currentGUIMap->end(); gmmIter++)
		{
			// grab the destination object
  			destObject = gmmIter->second;
  			// and it's HashString type 
  			destTypeName = destObject->GetComponentType();
  			// only pump messages to our component types that are known.
			if ( (compType == NULL) || (destObject->IsKindOf(compType)) || 
				(compType->GetUniqueID() == destTypeName->GetUniqueID()) )
  			{
	  			// pump the message to the objects message handler
  				retVal = (destObject->*msgFunction)(size, data);
			}
		}
	}
	else
	{
		GUIMAP::iterator findGUIObject = currentGUIMap->find( name->GetUniqueID() );
		if( findGUIObject != currentGUIMap->end() )
		{
			// grab the destination object
  			destObject = findGUIObject->second;
			if (destObject->IsKindOf(compType))
			{
	  			// pump the message to the objects message handler
  				retVal = (destObject->*msgFunction)(size, data);
			}
		}
	}

	// now check to see if message is for an animation
	if (_tcscmp(compType->GetString(), _T("CGUIAnimation")) == 0)
	{
		for (UINT i=0; i<m_Animations.size(); i++)
		{
			if (m_Animations[i]->GetName()->GetUniqueID() == name->GetUniqueID())
			{
				retVal = (m_Animations[i]->*msgFunction)(size, data);
				return retVal;
			}
		}
	}
	return retVal;
}

DWORD CGUIManager::OnKeyLeft(DWORD msgSize, void *param)
{
	static CHashString guiElement(_T("CGUIElement"));

	static DWORD msgHash_GUIKeyLeft = CHashString(_T("GUIKeyLeft")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_GUIKeyLeft, msgSize, param, NULL, &guiElement);
	return MSG_HANDLED_PROCEED;	
}

DWORD CGUIManager::OnKeyRight(DWORD msgSize, void *param)
{
	static CHashString guiElement(_T("CGUIElement"));

	static DWORD msgHash_GUIKeyRight = CHashString(_T("GUIKeyRight")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_GUIKeyRight, msgSize, param, NULL, &guiElement);
	return MSG_HANDLED_PROCEED;	
}

DWORD CGUIManager::OnKeyHome(DWORD msgSize, void *param)
{
	static CHashString guiElement(_T("CGUIElement"));

	static DWORD msgHash_GUIKeyHome = CHashString(_T("GUIKeyHome")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_GUIKeyHome, msgSize, param, NULL, &guiElement);
	return MSG_HANDLED_PROCEED;	
}

DWORD CGUIManager::OnKeyEnd(DWORD msgSize, void *param)
{
	static CHashString guiElement(_T("CGUIElement"));

	static DWORD msgHash_GUIKeyEnd = CHashString(_T("GUIKeyEnd")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_GUIKeyEnd, msgSize, param, NULL, &guiElement);
	return MSG_HANDLED_PROCEED;	
}

DWORD CGUIManager::OnKeyDel(DWORD msgSize, void *param)
{
	static CHashString guiElement(_T("CGUIElement"));

	static DWORD msgHash_GUIKeyDel = CHashString(_T("GUIKeyDel")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_GUIKeyDel, msgSize, param, NULL, &guiElement);
	return MSG_HANDLED_PROCEED;	
}

DWORD CGUIManager::OnKeyBackSpace(DWORD msgSize, void *param)
{
	static CHashString guiElement(_T("CGUIElement"));

	static DWORD msgHash_GUIKeyBackSpace = CHashString(_T("GUIKeyBackSpace")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_GUIKeyBackSpace, msgSize, param, NULL, &guiElement);
	return MSG_HANDLED_PROCEED;	
}

DWORD CGUIManager::OnKeyEnter(DWORD msgSize, void *param)
{
	static CHashString guiElement(_T("CGUIElement"));

	static DWORD msgHash_GUIKeyEnter = CHashString(_T("GUIKeyEnter")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_GUIKeyEnter, msgSize, param, NULL, &guiElement);
	return MSG_HANDLED_PROCEED;	
}

void CGUIManager::SortActivePage()
{
	if(m_ActivePage != NULL)
	{
		m_ActivePage->SortGUIElements();
	}
}

DWORD CGUIManager::OnClearGUIObjects(DWORD size, void* data)
{
	DELETEOBJECTPARAMS dop;
	GUIMAP::iterator gmmIter = m_GUIObjects.begin();

	while( m_GUIObjects.size() > 0 )
	{
		CHashString name = m_ToolBox->GetHashString(gmmIter->first);
		dop.name = &name;
		static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof(DELETEOBJECTPARAMS), &dop, NULL, NULL);

		gmmIter = m_GUIObjects.begin();
	}
	
	// set all member variablse to null
	m_ActivePage = NULL;
	m_pFocusItem = NULL;

	vector<CGUIAnimation*>::iterator animIter = m_Animations.begin();
	while (animIter != m_Animations.end())
	{
		dop.name = (*animIter)->GetName();
		static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof(DELETEOBJECTPARAMS), &dop, NULL, NULL);
		animIter = m_Animations.begin();
	}

	m_Animations.clear();
	m_mLanguages.clear();

	return MSG_HANDLED_PROCEED;
}

void CGUIManager::SetFocus(IObject *object)
{
	if(object != NULL)
	{
		if( object != m_pFocusItem )
		{
			// remove focus on the old item
			if(m_pFocusItem)
			{
				static DWORD msgHash_RemoveFocus = CHashString(_T("RemoveFocus")).GetUniqueID();
				m_ToolBox->SendMessage(msgHash_RemoveFocus, 0, NULL, m_pFocusItem->GetName());
			}

			// set the new item with focus
			m_pFocusItem = object;
		}
	}
	else
	{
		if(m_pFocusItem)
		{
			static DWORD msgHash_RemoveFocus = CHashString(_T("RemoveFocus")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_RemoveFocus, 0, NULL, m_pFocusItem->GetName());
		}
		// clear the focus item
		m_pFocusItem = NULL;
	}
}

/// tells the gui manager that we're running in the editor
/// \param size = sizeof bool
/// \param param = bool, true if running from editor
/// \return MSG_HANDLED_STOP
DWORD CGUIManager::OnSetGUIEditorMode(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(bool));
	m_bEditorMode = *(bool*)param;
	return MSG_HANDLED_STOP;
}

DWORD CGUIManager::OnGetGUIMousePosition(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(MOUSECOORDS));
	MOUSECOORDS *out = (MOUSECOORDS*) params;	
	out->m_lXPos = (long)(((float)(m_iMouseLastPosX - m_iPageOffsetX)) / m_fZoomFactor);
	out->m_lYPos = (long)(((float)(m_iMouseLastPosY - m_iPageOffsetY)) / m_fZoomFactor);
	return MSG_HANDLED_STOP;
}

DWORD CGUIManager::OnGetZoomFactor(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(float));
	float *zoom = (float*) params;
	*zoom = m_fZoomFactor;
	return MSG_HANDLED_STOP;
}

void CGUIManager::IncrementZoom()
{
	for (UINT i=0; i<9; i++)
	{
		if (m_fZoomFactor < m_fZoomTable[i+1])
		{
			m_fZoomFactor = m_fZoomTable[i+1];
			break;
		}
	}	
}

void CGUIManager::DecrementZoom()
{
	for (UINT i=9; i>0; i--)
	{
		if (m_fZoomFactor > m_fZoomTable[i-1])
		{
			m_fZoomFactor = m_fZoomTable[i-1];
			break;
		}
	}
}

DWORD CGUIManager::OnSetActiveMode(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(bool));
	bool active = *(bool*)param;
	if (m_ActivePage)
	{
		m_ActivePage->SetActive(active);
	}
	m_bActiveMode = active;
	return MSG_HANDLED_STOP;
}

DWORD CGUIManager::OnSetBGColor(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(DWORD));
	return MSG_HANDLED_STOP;
}

Vec3 CGUIManager::GetScreenCornerGUIPos()
{
	Vec3 pos;
	if ((m_iScreenSizeX > 0) && (m_iScreenSizeY > 0))
	{
		pos.x = (float)m_iScreenSizeX / m_fZoomFactor;
		pos.y = (float)m_iScreenSizeY / m_fZoomFactor;
	}
	return pos;
}

void CGUIManager::CenterPage()
{
	Vec3 adjust;
	adjust.x = (((float)m_iScreenSizeX / 2.f) - ((float)m_iResX* m_fZoomFactor / 2.f));
	adjust.y = (((float)m_iScreenSizeY / 2.f) - ((float)m_iResY* m_fZoomFactor / 2.f));
	adjust.x *= m_fScaleFactorX;
	adjust.y *= m_fScaleFactorY;
	adjust.x += (float)m_iPageOffsetX;
	adjust.y += (float)m_iPageOffsetY;
	m_iPageOffsetX += (int)adjust.x;
	m_iPageOffsetY += (int)adjust.y;
}

DWORD CGUIManager::OnSetGUIRenderToTexture(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(ITextureObject));
	ITextureObject *tex = (ITextureObject*) param;
	if (m_ActivePage)
	{
		m_ActivePage->SetTextureToRenderTo(tex);
	}
	return MSG_HANDLED_STOP;
}

DWORD CGUIManager::OnGetGUIRenderObjects(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(OBJECTLIST));
	OBJECTLIST *outList = (OBJECTLIST*) param;
	if (m_ActivePage)
	{
		*outList = m_ActivePage->GetRenderObjects();
	}
	return MSG_HANDLED_STOP;
}

DWORD CGUIManager::OnDisableBatchRendering(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(bool));
	bool disable = *(bool*) param;
	if (m_ActivePage)
	{
		m_ActivePage->DisableRendering(disable);
	}
	return MSG_HANDLED_STOP;
}

DWORD CGUIManager::OnSetXScale(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(float));
	float xScale = *(float*) params;
	m_fScaleFactorX = xScale;
	if (m_ActivePage)
	{
		m_ActivePage->SetWidthRatio(xScale);
	}
	return MSG_HANDLED_STOP;
}

DWORD CGUIManager::OnSetYScale(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(float));
	float yScale = *(float*) params;
	m_fScaleFactorY = yScale;
	if (m_ActivePage)
	{
		m_ActivePage->SetHeightRatio(yScale);
	}
	return MSG_HANDLED_STOP;
}

DWORD CGUIManager::OnRenderPageToContext(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(IRenderContext));
	IRenderContext *rC = (IRenderContext*)param;
	m_RenderContext = rC;
	if (m_ActivePage)
	{
		m_ActivePage->SetRenderContext(rC);
	}
	return MSG_HANDLED_STOP;
}

DWORD CGUIManager::OnIncrementZoom(DWORD size, void *param)
{
	IncrementZoom();	
	return MSG_HANDLED_STOP;
}

DWORD CGUIManager::OnDecrementZoom(DWORD size, void *param)
{
	DecrementZoom();	
	return MSG_HANDLED_STOP;
}

DWORD CGUIManager::OnSetPageOffset(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(POSITIONDATA));
	POSITIONDATA pos = *(POSITIONDATA*) param;	
	
	m_iPageOffsetX = (int)pos.m_fXPos;
	m_iPageOffsetY = (int)pos.m_fYPos;
	return MSG_HANDLED_STOP;
}

DWORD CGUIManager::OnSetZoomFactor(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(float));
	float zf = *(float*) params;
	m_fZoomFactor = zf;	
	return MSG_HANDLED_STOP;
}

DWORD CGUIManager::OnSetGUIWorkingRes(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(GUIWORKINGRES));
	GUIWORKINGRES gwr = *(GUIWORKINGRES*)param;
	m_iResX = gwr.iSizeX;
	m_iResY = gwr.iSizeY;
	return MSG_HANDLED_STOP;
}

DWORD CGUIManager::OnCheckGUISystemEmpty(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(bool));
	bool *empty = (bool*) param;
	if (m_GUIObjects.size() == 0)
	{
		*empty = true;
	}
	else
	{
		*empty = false;
	}
	return MSG_HANDLED_STOP;
}

DWORD CGUIManager::OnLoadingUpdate(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(LOADINGUPDATEPARAMS));
	LOADINGUPDATEPARAMS pbp = *(LOADINGUPDATEPARAMS*) param;

	static CHashString hsBarType(_T("CGUIStatusBar"));
	IDTOOBJECTMAP *barMap = GetObjectMap(&hsBarType);
	if (barMap)
	{
		for (IDTOOBJECTMAP::iterator barIter=barMap->begin(); barIter!=barMap->end(); barIter++)
		{
			CGUIStatusBar *pBar = dynamic_cast<CGUIStatusBar*>(barIter->second);
			if (pBar)
			{
				pBar->OnLoadingUpdate(size, param);
			}
		}
	}

	GLOBALTICKPARAMS gtp;
	gtp.tickCount = 0;
	static DWORD msgHash_RenderUpdate = CHashString(_T("RenderUpdate")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_RenderUpdate, 0, NULL);
	return MSG_HANDLED_PROCEED;
}

DWORD CGUIManager::OnQueueGUIStateEvent(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(GUIEVENT));
	GUIEVENT event = *(GUIEVENT*) param;
	m_StateWrapper.AddToQueue(event);
	return MSG_HANDLED_STOP;
}

DWORD CGUIManager::OnSetGUIStateObject(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(IHashString));
	IHashString *stateName = (IHashString*)param;
	m_StateWrapper.SetStateObjectName(stateName);
	return MSG_HANDLED_STOP;
}

DWORD CGUIManager::OnRenderUpdate(DWORD size, void *param)
{
	// disable ticking of state wrapper when we just want to render
	m_bTickStateWrapper = false;
	Update(0);
	m_bTickStateWrapper = true;
	return MSG_HANDLED_PROCEED;
}

void CGUIManager::CreateVertexBuffer()
{
	//create new allocation
	CHANNELDESCRIPTORLIST VertexFormatFonts;
	VertexFormatFonts.push_back( MESH_STREAM_VERTEX_POSITION3 );
	VertexFormatFonts.push_back( MESH_STREAM_VERTEX_TEXTURECOORD1 );
	VertexFormatFonts.push_back( MESH_STREAM_VERTEX_COLORCRGBA );

	ALLOCATERENDERBUFFERMSG msg;	
	UINT Max2DAllocation = MAXVERTEXBUFFER;
	msg.ElementsToAllocate = Max2DAllocation;
	msg.SizeVertexFormat = GUIVERTEXSIZE;
	msg.VertexFormat = VertexFormatFonts;
	msg.Dynamic = true;
	msg.m_AllowSharedBuffer = false;

	static DWORD msgHash_AllocateVertices = CHashString(_T("AllocateVertices")).GetUniqueID();
	DWORD res = EngineGetToolBox()->SendMessage(msgHash_AllocateVertices, sizeof( msg ), &msg, NULL, NULL );

	if( res == MSG_HANDLED )
	{
		m_VertexBufferAllocation.m_Offset = msg.m_ReturnOffset;
		m_VertexBufferAllocation.m_AllocationHandle = msg.m_ReturnHandle;
		m_VertexBufferAllocation.m_InterfaceHandle = msg.m_ReturnInterfaceHandle;
		m_VertexBufferAllocation.m_Size = Max2DAllocation;
		m_VertexBufferAllocation.m_Stride = msg.SizeVertexFormat;
		m_VertexBuffer = (IVertexBufferObject*)msg.m_InterfacePointer;
	}	
}

void CGUIManager::PreRenderWrap()
{
	CGUIManager *manager = SINGLETONINSTANCE(CGUIManager);
	if (manager == NULL)
	{
		return;
	}

	manager->PreRender();
}

void CGUIManager::PostRenderWrap()
{
	CGUIManager *manager = SINGLETONINSTANCE(CGUIManager);
	if (manager == NULL)
	{
		return;
	}

	manager->PostRender();
}

void CGUIManager::PreRender()
{
	if (m_Renderer)
	{
		m_Renderer->SetRenderState(RENDERSTATE_ALPHABLENDENABLE, RENDERSTATEPARAM_TRUE);
		m_Renderer->SetRenderState(RENDERSTATE_ALPHATESTENABLE, RENDERSTATEPARAM_FALSE);
		m_Renderer->SetRenderState(RENDERSTATE_CULLMODE, RENDERSTATEPARAM_CULLREGULAR);
		m_Renderer->SetSamplerState(0, SAMPLERSTATE_MIPFILTER, TEXTURESTAGE_TEXF_POINT);
		m_Renderer->SetSamplerState(0, SAMPLERSTATE_MAGFILTER, TEXTURESTAGE_TEXF_LINEAR);
		m_Renderer->SetSamplerState(0, SAMPLERSTATE_MINFILTER, TEXTURESTAGE_TEXF_LINEAR);

		m_Renderer->SetDepthTest(false);

		m_Renderer->PushMatrix( PROJECTION_MATRIX );
		m_Renderer->PushMatrix( WORLD_MATRIX );
		m_Renderer->PushMatrix( VIEW_MATRIX );

		int width, height;
		GetScreenSize(&width, &height);
		m_Renderer->SetOrtho2DScreenSize(0, (float)width, 0, (float)height);

		Matrix4x4 mat;
		Vec3 trans((float)m_iPageOffsetX, (float)m_iPageOffsetY, 0.0f);
		mat.SetTranslation(trans);
		Vec3 scale(m_fZoomFactor, m_fZoomFactor, m_fZoomFactor);
		mat.SetScale(scale);
		m_Renderer->SetMatrix(WORLD_MATRIX, mat.GetMatrix());
		
	}
}

void CGUIManager::PostRender()
{
	if (m_Renderer)
	{
		m_Renderer->SetDepthTest(true);

		m_Renderer->PopMatrix( PROJECTION_MATRIX );
		m_Renderer->PopMatrix( WORLD_MATRIX );
		m_Renderer->PopMatrix( VIEW_MATRIX );
	}
}

DWORD CGUIManager::OnGetGUIScreenSize(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(VIEWPORTPARAMS));
	VIEWPORTPARAMS *vpp = (VIEWPORTPARAMS*)param;

	if (vpp == NULL)
	{
		return MSG_ERROR;
	}

	GetScreenSize(&vpp->width, &vpp->height);
	return MSG_HANDLED_STOP;
}

DWORD CGUIManager::OnSetGUIFontGlyphDB(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(IHashString*));

	IHashString *inFile = (IHashString*)param;
	if (inFile)
	{
		m_hsTextGlyphDBFile.Init(inFile->GetString());		

		// now load new db
		TCHAR *fileName = _tcsdup(m_hsTextGlyphDBFile.GetString());
		LOADFILEEXTPARAMS lfep;
		lfep.fileName = fileName;
		lfep.bInternalLoad = true;

		static DWORD msgLoadFileByExtension = CHashString(_T("LoadFileByExtension")).GetUniqueID();
		DWORD retval = m_ToolBox->SendMessage(msgLoadFileByExtension, sizeof(LOADFILEEXTPARAMS), &lfep);
		free( fileName );
		if (retval == MSG_HANDLED)
		{
			CHashString hsGlyphDBObject;
			DATABASEINFO dbi;
			dbi.m_FileName = &m_hsTextGlyphDBFile;
			static DWORD msgGetDatabase = CHashString(_T("GetDatabase")).GetUniqueID();
			retval = m_ToolBox->SendMessage(msgGetDatabase, sizeof(DATABASEINFO), &dbi);
			if (retval == MSG_HANDLED)
			{
				hsGlyphDBObject.Init(dbi.m_DatabaseName->GetString());
				DATABASEATTRIBUTEPARAMS dap;
				CREATEARCHIVE ca;
				static CHashString memType(_T("Memory"));

				ca.mode = STREAM_MODE_WRITE | STREAM_MODE_READ;
				ca.streamData = NULL;
				ca.streamSize = 0;
				ca.streamType = &memType;
				static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
				m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca);

				IArchive *attribArchive = ca.archive;
				if (attribArchive)
				{
					dap.m_AttributeArchive = attribArchive;

					static DWORD msgGetAttribute = CHashString(_T("GetAttribute")).GetUniqueID();
					static CHashString hsKeyString(_T("KeyString"));
					static CHashString hsFileName(_T("TextureFileName"));					

					static CHashString hsDBType(_T("CDefinedDatabase"));
					for (UINT i=0; i<(UINT)dbi.m_nItems; i++)
					{
						// get key string
						attribArchive->SeekTo(0, SEEK_SET);
						dap.m_Index = i;
						dap.m_AttributeName = &hsKeyString;
						retval = m_ToolBox->SendMessage(msgGetAttribute, sizeof(DATABASEATTRIBUTEPARAMS), &dap, &hsGlyphDBObject, &hsDBType);
						if (retval != MSG_HANDLED)
						{
							m_ToolBox->Log(LOGERROR, _T("GUI manager: could not get key attribute for row %d; glyph not fully loaded!\n"), i);
							attribArchive->Close();
							return MSG_ERROR;
						}

						CHashString hsKey;
						StdString szKey;
						attribArchive->SetIsWriting(false);
						attribArchive->Read(szKey);
						hsKey = szKey;

						// get filename
						attribArchive->SeekTo(0, SEEK_SET);
						dap.m_AttributeName = &hsFileName;
						retval = m_ToolBox->SendMessage(msgGetAttribute, sizeof(DATABASEATTRIBUTEPARAMS), &dap, &hsGlyphDBObject, &hsDBType);
						if (retval != MSG_HANDLED)
						{
							m_ToolBox->Log(LOGERROR, _T("GUI manager: could not get file attribute for row %d; glyph not fully loaded!\n"), i);
							attribArchive->Close();
							return MSG_ERROR;
						}

						CHashString hsFile;
						StdString szFile;
						attribArchive->SetIsWriting(false);
						attribArchive->Read(szFile);
						szFile.tolower();
						hsFile = szFile;
						
						// add the glyph
                        ADDFONTGLYPHFILENAMEPARAMS afgfp;
						afgfp.hsFile = &hsFile;
						afgfp.hsKey = &hsKey;
						static DWORD msgAddFontGlyphTexFilename = CHashString(_T("AddFontGlyphTexFilename")).GetUniqueID();
						m_ToolBox->SendMessage(msgAddFontGlyphTexFilename, sizeof(ADDFONTGLYPHFILENAMEPARAMS), &afgfp);
					}
					attribArchive->Close();
				}
			}
		}
	}

	return MSG_HANDLED_STOP;
}

DWORD CGUIManager::OnAddGUILanguage(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(ADDGUILANGUAGEPARAMS));

	ADDGUILANGUAGEPARAMS aglp = *(ADDGUILANGUAGEPARAMS*)param;
	StdString langFile = m_ToolBox->GetHashString(aglp.dwHashLangFile);
	m_mLanguages[aglp.dwHashLangName] = langFile;

	return MSG_HANDLED_PROCEED;
}

DWORD CGUIManager::OnRemoveGUILanguage(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(IHashString*));
	IHashString* name = (IHashString*)param;
	
	if (name)
	{
		LANGUAGEFILEMAP::iterator mapIter = m_mLanguages.find(name->GetUniqueID());
		if (mapIter != m_mLanguages.end())
		{
			m_mLanguages.erase(mapIter);
		}
		else
		{
			m_ToolBox->Log(LOGWARNING, _T("%s %d: language %s not found in gui list\n"), 
				__FILE__, __LINE__, name->GetString());
		}
	}
	return MSG_HANDLED_PROCEED;
}

DWORD CGUIManager::OnSetActiveGUILanguage(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(IHashString*));
	IHashString *lang = (IHashString*)param;

	if (lang != NULL)
	{
		LANGUAGEFILEMAP::iterator mapIter = m_mLanguages.find(lang->GetUniqueID());
		if (mapIter != m_mLanguages.end())
		{
			LOADFILEEXTPARAMS lfep;
			lfep.bInternalLoad = true;
			lfep.fileName = (TCHAR*)mapIter->second.c_str();

			// loading file sets active language in language plugin
			static DWORD msgLoadFileByExtension = CHashString(_T("LoadFileByExtension")).GetUniqueID();
			m_ToolBox->SendMessage(msgLoadFileByExtension, sizeof(LOADFILEEXTPARAMS), &lfep);

			FlagObjectsUpdateLanguageEntry();

			return MSG_HANDLED_PROCEED;
		}
		else
		{
			m_ToolBox->Log(LOGWARNING, _T("%s %d: language %s not found!\n"),
				__FILE__, __LINE__, lang->GetString());
		}
	}
	else // lang == NULL
	{
		static DWORD msgClearLanguage = CHashString(_T("ClearLanguage")).GetUniqueID();
		m_ToolBox->SendMessage(msgClearLanguage, 0, NULL);
		return MSG_HANDLED_PROCEED;
	}

	return MSG_NOT_HANDLED;
}

DWORD CGUIManager::OnGetGUILanguages(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(LANGUAGEFILEMAP));
	LANGUAGEFILEMAP &map = *(LANGUAGEFILEMAP*)param;
	map = m_mLanguages;
	return MSG_HANDLED_STOP;
}

void CGUIManager::FlagObjectsUpdateLanguageEntry()
{
	GUIMAP::iterator mapIter = m_GUIObjects.begin();
	for (; mapIter != m_GUIObjects.end(); mapIter++)
	{
		IObject *obj = mapIter->second;
		if (obj)
		{
			// try to cast to IGUIReference, if it is one
			IGUIReference *ref = dynamic_cast<IGUIReference*>(obj);
			if (ref)
			{
				ref->FlagLanguageEntryChanged();
			}
		}
	}
}