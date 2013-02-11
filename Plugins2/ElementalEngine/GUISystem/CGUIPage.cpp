//==========================================================================
/// \file	CGUIPage.cpp
/// \brief	CGUIPage implementation
/// \date	2-22-2005
/// \author Kyle Swaim
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
#include "CGUIStatusBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT(CGUIPage);

REGISTER_MESSAGE_HANDLER(GetObjUnderCursor, OnGetObjectUnderCursor, CGUIPage);
REGISTER_MESSAGE_HANDLER(AddGUIElementToPage, OnAddGUIElementToPage, CGUIPage);
REGISTER_MESSAGE_HANDLER(RemoveGUIElementFromPage, OnRemoveGUIElementFromPage, CGUIPage);
REGISTER_MESSAGE_HANDLER(GetObjInsideRect, OnGetObjectInsideRect, CGUIPage);
REGISTER_MESSAGE_HANDLER(GetPageItemList, OnGetPageItemList, CGUIPage);

///
/// Constructor / Destructor
///

CGUIPage::CGUIPage(IHashString *parentName, IHashString *name) : 
	OBJECTTEMPLATE(CGUIManager, CGUIPage, IObject, parentName, name)
{
	m_iPageOffsetX = m_iPageOffsetY = 0;
	m_fWidthRatio = 1.f;
	m_fHeightRatio = 1.f;
	m_fZoomFactor = 1.f;
	m_bActive = true;
	m_pRenderTarget = NULL;
	m_bBatchRender = true;
	m_RenderContext = NULL;

	static CHashString hsGuiManager(_T("CGUIManager"));
	IComponent *guiManagerComponent = m_ToolBox->CreateComponent(&hsGuiManager, 0);
	if (guiManagerComponent == NULL)
	{
		m_ToolBox->Log(LOGERROR, _T("GUI Instance: Could not get gui manager component!\n"));
		return;
	}

	m_pManager = dynamic_cast<CGUIManager*>(guiManagerComponent);
	if (m_pManager == NULL)
	{
		m_ToolBox->Log(LOGERROR, _T("GUI Instance: Could not dynamic cast to gui manager!\n"));
		return;
	}

	m_fSaveVersion = 1.0f;
}

CGUIPage::~CGUIPage()
{
}

/// \brief The Initialization for the Object
/// \return pointer to an IComponent
IComponent *CGUIPage::Create(int nArgs, va_list argptr)
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
		self = new CGUIPage(parentName, name);
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}


IHashString* CGUIPage::GetComponentType()
{
	static CHashString guiPageCT(_T("CGUIPage"));
	return &guiPageCT;
}

bool CGUIPage::IsKindOf(IHashString *compType)
{
	return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
}

bool CGUIPage::Update()
{
	DWORD						sortMethod;
	ADDRENDERLISTPARAMS			addRenderListMessage;
	OBJECTLIST::iterator		olIter;
	IObject						*Object;
	
	for(olIter = m_ObjectList.begin(); olIter != m_ObjectList.end();
		olIter++)
	{
		Object  = dynamic_cast<IObject*>(*olIter);
		if(Object == NULL)
		{
			EngineGetToolBox()->SetErrorValue(ERR_NULL_POINTER);
			EngineGetToolBox()->Log(LOGERROR, _T("Could not cast object from the object page list to IObject*"));
			return false;
		}
		Object->Update(); //update each gui object before adding to render list
	}
	
	if(m_ObjectList.empty())
	{
		return false;
	}
	
	if(m_SortedMap.empty())
	{
		SortGUIElements();
	}
	
	// setup render list for render manager
	if (m_bBatchRender)
	{
		sortMethod = SORT_FIFO;//SORT_DISTANCE;
		if (m_pRenderTarget == NULL) // render to scene view (world) or to context, if enabled
		{
			addRenderListMessage.dwSortMethod = sortMethod;
			addRenderListMessage.objList = &m_ObjectList;
			addRenderListMessage.dwListPriorityKey = RENDERPRIORITY_FRONTBUFFER;
			addRenderListMessage.renderContext = m_RenderContext;
			addRenderListMessage.dwFuncCallback = CGUIManager::PreRenderWrap;
			addRenderListMessage.dwFuncPostRenderCallback = CGUIManager::PostRenderWrap;
			static DWORD msgHash_AddRenderList = CHashString(_T("AddRenderList")).GetUniqueID();
			EngineGetToolBox()->SendMessage(msgHash_AddRenderList, sizeof(addRenderListMessage), &addRenderListMessage, NULL, NULL);
		}		
		else // render to texture
		{
			RENDERLISTTOTEXTUREPARAMS params;
			params.dwSortMethod = sortMethod;
			params.objList = &m_ObjectList;
			params.RenderTexture = m_pRenderTarget;
			params.Camera = NULL;
			params.clearDepth = true;
			params.clearTarget = true;
			static DWORD msgHash_RenderListToTexture = CHashString(_T("RenderListToTexture")).GetUniqueID();
			EngineGetToolBox()->SendMessage(msgHash_RenderListToTexture, sizeof(params), &params, NULL, NULL);
		}
	}

	return true;
}


bool CGUIPage::PageBuffer(const StdString &pageName)
{
	int result;
	StdString pageBufferName(_T("page_buffer_"));
		
	result = _tcsnicmp(pageName, pageBufferName, 12);
	
	if (result == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CGUIPage::Serialize(IArchive &ar)
{
	CGUIManager *manager;
	static CHashString managerName(_T("CGUIManager"));
	IObject *Object;
	StdString pageName;
	StdString guiName;
	StdString guiType;
	StdString type;
	int iNumGuiElements;
	OBJECTLIST::iterator olIter;

	if(ar.IsReading())
	{
		CHashString versionName;
		static DWORD msgHash_GetFileVersion = CHashString(_T("GetFileVersion")).GetUniqueID();
		DWORD retval = EngineGetToolBox()->SendMessage(msgHash_GetFileVersion, sizeof(IHashString), &versionName);

		static DWORD Vers2_0a = CHashString(_T("2.0a")).GetUniqueID();
		static DWORD Vers2_0b = CHashString(_T("2.0b")).GetUniqueID();
		static DWORD Vers2_0c = CHashString(_T("2.0c")).GetUniqueID();

		int verNum = 0;
		
		if (versionName.GetUniqueID() == Vers2_0a)
		{
			verNum = 3;
		}
		else if (versionName.GetUniqueID() == Vers2_0b)
		{
			verNum = 4;
		}
		else if (versionName.GetUniqueID() == Vers2_0c)
		{
			verNum = 5;
		}
		else 
		{
			m_ToolBox->Log(LOGERROR, _T("Bad GUI file version!"));
			assert(0);
			return;
		}

		float loadVersion = 0.0f;

		if (verNum >= 4)
		{
			ar.Read(loadVersion);
			if (loadVersion == 0.0f)
			{
				// initial serialize from editor; set to save version (up to date schema)
				loadVersion = m_fSaveVersion;
			}
		}

		ar.Read(iNumGuiElements);
		while(iNumGuiElements != 0)
		{
			ar.Read(guiName);
			ar.Read(guiType);

			CHashString szhGuiName(guiName.c_str());
			CHashString szhGuiType(guiType.c_str());

			// ask gui manager for the object to add
			manager = dynamic_cast<CGUIManager*>(EngineGetToolBox()->GetComponent( &managerName ));
			if (!manager)
			{
				EngineGetToolBox()->SetErrorValue(ERR_NULL_POINTER);
				EngineGetToolBox()->Log(LOGERROR, _T("could not cast IComponent from EngineToolBox to CGUIManager\n")); 
				return;
			}
			Object = manager->GetObject(&szhGuiName, &szhGuiType);
			if(Object == NULL)
			{
				CREATEOBJECTPARAMS cop;
				INITOBJECTPARAMS iop;
				cop.name = &szhGuiName;
				cop.typeName = &szhGuiType;
				cop.parentName = GetName();
				iop.name = &szhGuiName;

				static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
				m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop, NULL, NULL);
				static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
				m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &cop, NULL, NULL);
				Object = manager->GetObject(&szhGuiName, &szhGuiType);
			}
			m_ObjectList.push_back(Object);
			iNumGuiElements--;
		}

	}
	else //ar.IsWritting()
	{
		TCHAR strNum[50];
		int count = 1;
		StdString itemName;
		StdString itemType;

		//pageName = GetName()->GetString();
		//type = GetComponentType()->GetString();
		iNumGuiElements = m_ObjectList.size();

		ar.Write(m_fSaveVersion, _T("Version"));

		// start the page class
		//ar.StartClass(type); // <CGUIPage
		// write the page's name
		//ar.Write( pageName, _T("Name") );
		// write the number of gui elements 
		ar.Write( iNumGuiElements, _T("NumGuiElements") );
		
		for(olIter = m_ObjectList.begin(); olIter != m_ObjectList.end();
			olIter++)
		{

			_ltot(count, strNum, 10);
			itemName = StdString(_T("GuiName")) + strNum;
			itemType = StdString(_T("GuiType")) + strNum;

			guiName = (*olIter)->GetName()->GetString();
			ar.Write( guiName, itemName );
			guiType = (*olIter)->GetComponentType()->GetString();
			ar.Write( guiType, itemType );
			count++;
		}
		//ar.EndClass(); // </CGUIPage>
	}
}

bool CGUIPage::InsideRect(int checkX, int checkY, int x1, int y1, int x2, int y2)
{
	int top, bottom, left, right;

	if(x1 < x2)
	{
		left = x1;
		right = x2;
	}
	else
	{
		left = x2;
		right = x1;
	}

	if (y1 < y2)
	{
		top = y1;
		bottom = y2;
	}	
	else
	{
		top = y2;
		bottom = y1;
	}

	if( left < checkX && top < checkY
		&& right > checkX && bottom > checkY )
	{
		return true;
	}
	else
	{
		return false;
	}
}

DWORD CGUIPage::OnGetObjectInsideRect(DWORD size, void *params)
{
	DWORD retVal;

	OBJECTLIST::iterator olIter;
	IGUIElement *object;
	OBJINRECT *data;
	
	VERIFY_MESSAGE_SIZE(size, sizeof(OBJINRECT));
	data = (OBJINRECT*)params;

	int x1 = data->x1;
	int x2 = data->x2;
	int y1 = data->y1;
	int y2 = data->y2;

	POSITIONDATA pd;
	IHashString *guiName;

	for(olIter = m_ObjectList.begin();
		olIter != m_ObjectList.end();
		olIter++)
	{
		object = dynamic_cast<IGUIElement*>(*olIter);
		if(!object)
		{	
			m_ToolBox->SetErrorValue(ERR_NULL_POINTER);
			m_ToolBox->Log(LOGERROR, _T("Could not grab IGUIElement from list of gui elements"));
			return MSG_ERROR;
		}
		
		guiName = object->GetName();

		static DWORD msgHash_GetPosition = CHashString(_T("GetPosition")).GetUniqueID();
		retVal = m_ToolBox->SendMessage(msgHash_GetPosition, sizeof(POSITIONDATA), &pd, guiName);
		if(retVal != MSG_HANDLED)
		{
			m_ToolBox->Log(LOGERROR, _T("Could not send GetPosition message to %s\n"), guiName->GetString());
			return MSG_ERROR;
		}

		if( InsideRect((int)pd.m_fXPos, (int)pd.m_fYPos, 
			data->x1, data->y1, data->x2, data->y2) )
		{
			data->list.push_back(object);
		}
	}
	if (data->list.empty())
	{
		return MSG_HANDLED_PROCEED;
	}
	else
	{
		return MSG_HANDLED_STOP;
	}
}

DWORD CGUIPage::OnGetObjectUnderCursor(DWORD size, void *params)
{
	OBJECTLIST::iterator olIter;
	IGUIElement *lesserGE = NULL;
	IGUIElement *newGE;
	OBJUNDERCURSOR *data;
	
	VERIFY_MESSAGE_SIZE(size, sizeof(OBJUNDERCURSOR));
	data = (OBJUNDERCURSOR*)params;

	float fCursorPosX = data->m_fPosX;
	float fCursorPosY = data->m_fPosY;

	data->m_Name = NULL;
	data->m_CompType = NULL;

	for(olIter = m_ObjectList.begin();
		olIter != m_ObjectList.end();
		olIter++)
	{
		newGE = dynamic_cast<IGUIElement*>(*olIter);
		if(!newGE)
		{	
			EngineGetToolBox()->SetErrorValue(ERR_NULL_POINTER);
			EngineGetToolBox()->Log(LOGERROR, _T("Could not grab IGUIElement from list of gui elements"));
			return MSG_ERROR;
		}
		if(newGE->IsCursorOver(fCursorPosX, fCursorPosY))
		{
			if(lesserGE==NULL)
			{	
				lesserGE = newGE;
			}
			else if( lesserGE->GetZOrder() > newGE->GetZOrder() )
			{
					lesserGE = newGE;
			}
		}
	}
	if(lesserGE)
	{
		data->m_Name = lesserGE->GetName();
		data->m_CompType = lesserGE->GetComponentType();
	}

	if(data->m_Name == NULL)
	{	
		return MSG_HANDLED_PROCEED;
	}
	else
	{
		return MSG_HANDLED_STOP;
	}

}

// add a gui element to the page list
DWORD CGUIPage::OnAddGUIElementToPage(DWORD size, void *params)
{
	OBJECTLIST::iterator olIter;
	GUIPAGEMESSAGE *gpm;
	IObject *guiObject;
	static CHashString managerName(_T("CGUIManager"));
	CGUIManager *manager;

	// make sure message is valid
	VERIFY_MESSAGE_SIZE(size, sizeof(GUIPAGEMESSAGE));

	// downcast parameters to struct we expect
	gpm = (GUIPAGEMESSAGE *)params;

	// ask gui manager for the object to add
	manager = dynamic_cast<CGUIManager*>(EngineGetToolBox()->GetComponent( &managerName ));
	if (!manager)
	{
		EngineGetToolBox()->SetErrorValue(ERR_NULL_POINTER);
		EngineGetToolBox()->Log(LOGERROR, _T("could not cast IComponent from EngineToolBox to CGUIManager\n")); 
		return MSG_ERROR;
	}
	guiObject = manager->GetObject(gpm->name, gpm->compType);
	if(guiObject == NULL)
	{
		EngineGetToolBox()->SetErrorValue(ERR_NULL_POINTER);
		EngineGetToolBox()->Log(LOGERROR, _T("could not get the gui object from the gui manager: %s object not found\n"), gpm->name);
		return MSG_ERROR;
	}

	// check if object is already on list
	for(olIter = m_ObjectList.begin(); olIter != m_ObjectList.end(); olIter++)
	{
		if ( (*olIter)->GetName()->GetUniqueID() == guiObject->GetName()->GetUniqueID() )
		{
			// object is already on page
			return MSG_NOT_HANDLED;
		}
	}
	// now add object to list
	m_ObjectList.push_back(guiObject);
	m_SortedMap.clear();
	IGUIElement *guiElement = dynamic_cast<IGUIElement*>(guiObject);
	guiElement->SetWidthRatio(m_fWidthRatio);
	guiElement->SetHeightRatio(m_fHeightRatio);
	guiElement->SetZoomFactor(m_fZoomFactor);
	guiElement->SetPageOffset(m_iPageOffsetX, m_iPageOffsetY);
	return MSG_HANDLED_STOP;
}

// remove GUI Element from page list
DWORD CGUIPage::OnRemoveGUIElementFromPage(DWORD size, void *params)
{
	OBJECTLIST::iterator olIter;
	IObject	*remObj;
	static CHashString managerName(_T("CGUIManager"));
	CGUIManager *manager;
	GUIPAGEMESSAGE *gpm;

	VERIFY_MESSAGE_SIZE(size, sizeof(GUIPAGEMESSAGE));	
	
	gpm = (GUIPAGEMESSAGE*)params;
	gpm->wasFoundInPage = false;

	//ask gui manager for iobject to remove
	manager = dynamic_cast<CGUIManager*>(EngineGetToolBox()->GetComponent(&managerName));

	if (!manager)
	{
		EngineGetToolBox()->SetErrorValue(ERR_NULL_POINTER);
		EngineGetToolBox()->Log(LOGERROR, _T("could not cast IComponent from EngineToolBox to CGUIManager\n")); 
		return MSG_ERROR;
	}
	remObj = manager->GetObject(gpm->name, gpm->compType);
	if(remObj == NULL)
	{
		return MSG_NOT_HANDLED;
	}
	
	for(olIter = m_ObjectList.begin(); olIter != m_ObjectList.end(); olIter++)
	{
		if(remObj == *olIter)
		{
			//object found on list...remove
			m_ObjectList.erase(olIter);
			gpm->wasFoundInPage = true;
			break;	// no reason to keep searching, b/c dup gui elements can't exist on same page
		}
	}	
	return MSG_HANDLED_PROCEED;
}

DWORD CGUIPage::OnMouseMove(DWORD size, void *params)
{
	DWORD retVal;
	if (m_bActive)
	{
		OBJECTLIST::iterator olIter = m_ObjectList.begin();
		IGUIElement *guiElement;
		for(; olIter != m_ObjectList.end(); olIter++)
		{
			guiElement = dynamic_cast<IGUIElement*> (*olIter);
			if(!guiElement)
			{	
				EngineGetToolBox()->SetErrorValue(ERR_NULL_POINTER);
				EngineGetToolBox()->Log(LOGERROR, _T("Could not grab IGUIElement from list of gui elements"));
				return MSG_ERROR;
			}
			retVal = guiElement->OnMousePosition(size, params);
			if(retVal == MSG_HANDLED_STOP || retVal == MSG_ERROR)
			{
				return retVal;
			}
		}
	}
	return MSG_HANDLED_PROCEED;
}

// b/c the objects are sorted from back to front(since that is 
// their draw order) the objects must be traversed in the reverse
// order for LeftMousePress, LeftMouseRelease, KeyPress

DWORD CGUIPage::OnLeftMousePress(DWORD size, void *params)
{
	DWORD retVal = MSG_NOT_HANDLED;
	if (m_bActive)
	{
		int counter = 0;
		OBJECTLIST::reverse_iterator olIter = m_ObjectList.rbegin();
		IGUIElement *guiElement;

		while(counter != m_ObjectList.size())
		{
			guiElement = dynamic_cast<IGUIElement*> (*olIter);
			if(!guiElement)
			{	
				EngineGetToolBox()->SetErrorValue(ERR_NULL_POINTER);
				EngineGetToolBox()->Log(LOGERROR, _T("Could not grab IGUIElement from list of gui elements"));
				return MSG_ERROR;
			}
			retVal = guiElement->OnLeftMousePress(size, params);
			if(retVal == MSG_HANDLED_STOP || retVal == MSG_ERROR)
			{
				return retVal;
			}
			olIter++;
			counter++;
		}
	}
	return retVal;
}

DWORD CGUIPage::OnLeftMouseRelease(DWORD size, void *params)
{
	DWORD retVal = MSG_NOT_HANDLED;
	if (m_bActive)
	{
		int counter = 0;
		OBJECTLIST::reverse_iterator olIter = m_ObjectList.rbegin();
		IGUIElement *guiElement;

		while(counter != m_ObjectList.size())
		{
			guiElement = dynamic_cast<IGUIElement*> (*olIter);
			if(!guiElement)
			{	
				EngineGetToolBox()->SetErrorValue(ERR_NULL_POINTER);
				EngineGetToolBox()->Log(LOGERROR, _T("Could not grab IGUIElement from list of gui elements"));
				return MSG_ERROR;
			}
			retVal = guiElement->OnLeftMouseRelease(size, params);
			if(retVal == MSG_HANDLED_STOP || retVal == MSG_ERROR)
			{
				return retVal;
			}
			olIter++;
			counter++;
		}
	}
	return retVal;
}

DWORD CGUIPage::OnKeyPress(DWORD size, void *params)
{
	if (m_bActive)
	{
		int counter = 0;
		OBJECTLIST::reverse_iterator olIter = m_ObjectList.rbegin();
		IGUIElement *guiElement;

		while(counter != m_ObjectList.size())
		{
			guiElement = dynamic_cast<IGUIElement*> (*olIter);
			if(!guiElement)
			{	
				EngineGetToolBox()->SetErrorValue(ERR_NULL_POINTER);
				EngineGetToolBox()->Log(LOGERROR, _T("Could not grab IGUIElement from list of gui elements"));
				return MSG_ERROR;
			}
			DWORD retVal = guiElement->OnKeyPress(size, params);
			if(retVal == MSG_HANDLED_STOP || retVal == MSG_ERROR)
			{
				return retVal;
			}
			olIter++;
			counter++;
		}
	}
	return MSG_HANDLED_PROCEED;	
}

void CGUIPage::SortGUIElements()
{
	IGUIElement *guiElement;
	OBJECTLIST::iterator olIter = m_ObjectList.begin();
	
	// clear sorted map
	m_SortedMap.clear();

	while(olIter != m_ObjectList.end())
	{
		guiElement = dynamic_cast<IGUIElement*>((*olIter));
		if(guiElement == NULL)
		{
			m_ToolBox->SetErrorValue(ERR_NULL_POINTER);
			m_ToolBox->Log(LOGERROR, _T("Could not cast to IGUIElement: %s"), (*olIter)->GetName());
			continue;
		}
		// insert gui element into sorted map
		m_SortedMap.insert(make_pair(guiElement->GetZOrder(), guiElement));		

		IHashString *type = guiElement->GetComponentType();		
		if (type)
		{
			static DWORD dwGroup = CHashString(_T("CGUIGroup")).GetUniqueID();
			if (type->GetUniqueID() == dwGroup)
			{
				CGUIGroup *group = dynamic_cast<CGUIGroup*>(guiElement);
				if (group)
				{
					group->SortElements();
				}
			}
		}

		olIter++;
	}
	
	// now clear object list
	m_ObjectList.clear();

	SORTEDGUIMAP::iterator sgmIter = m_SortedMap.begin();
	while(sgmIter != m_SortedMap.end())
	{
		// added the gui elements into the list in sorted order
		m_ObjectList.push_back(sgmIter->second);	
		sgmIter++;
	}
}

/// Sets the width ratio value for this page and all the underlying gui elements
/// \param value - value to set ratio to
void CGUIPage::SetWidthRatio(float value)
{
	m_fWidthRatio = value;

	IGUIElement *guiElement;
	OBJECTLIST::iterator olIter = m_ObjectList.begin();
	
	while(olIter != m_ObjectList.end())
	{
		guiElement = dynamic_cast<IGUIElement*>((*olIter));
		if(guiElement == NULL)
		{
			m_ToolBox->SetErrorValue(ERR_NULL_POINTER);
			m_ToolBox->Log(LOGERROR, _T("Could not cast to IGUIElement: %s"), (*olIter)->GetName());
		}
		
		guiElement->SetWidthRatio(value);
		olIter++;
	}
}

/// Sets the height ratio value for this page and all the underlying gui elements
/// \param value - value to set ratio to
void CGUIPage::SetHeightRatio(float value)
{
	m_fHeightRatio = value;

	IGUIElement *guiElement;
	OBJECTLIST::iterator olIter = m_ObjectList.begin();
	
	while(olIter != m_ObjectList.end())
	{
		guiElement = dynamic_cast<IGUIElement*>((*olIter));
		if(guiElement == NULL)
		{
			m_ToolBox->SetErrorValue(ERR_NULL_POINTER);
			m_ToolBox->Log(LOGERROR, _T("Could not cast to IGUIElement: %s"), (*olIter)->GetName());
		}
		
		guiElement->SetHeightRatio(value);
		olIter++;
	}
}


/// Sets the zoom for this page and all underlying gui elements
/// \param value - value to set zoom to
void CGUIPage::SetZoomFactor(float value)
{	
	m_fZoomFactor = value;

	IGUIElement *guiElement;
	OBJECTLIST::iterator olIter = m_ObjectList.begin();
	
	while(olIter != m_ObjectList.end())
	{
		guiElement = dynamic_cast<IGUIElement*>((*olIter));
		if(guiElement == NULL)
		{
			m_ToolBox->SetErrorValue(ERR_NULL_POINTER);
			m_ToolBox->Log(LOGERROR, _T("Could not cast to IGUIElement: %s"), (*olIter)->GetName());
		}
		
		guiElement->SetZoomFactor(value);
		olIter++;
	}
}

/// Sets the page offset to render elements at based on displaced positioning
/// \param xOff - x offset 
/// \param yOff - y offset 
void CGUIPage::SetPageOffset(int xOff, int yOff)
{
	m_iPageOffsetX = xOff;
	m_iPageOffsetY = yOff;

	IGUIElement *guiElement;
	OBJECTLIST::iterator olIter = m_ObjectList.begin();
	while(olIter != m_ObjectList.end())
	{
		guiElement = dynamic_cast<IGUIElement*>((*olIter));
		if(guiElement == NULL)
		{
			m_ToolBox->SetErrorValue(ERR_NULL_POINTER);
			m_ToolBox->Log(LOGERROR, _T("Could not cast to IGUIElement: %s"), (*olIter)->GetName());
		}
		/*
		POSITIONDATA pos;
		IHashString *guiName;
		guiName = guiElement->GetName();
		static DWORD msgHash_GetPosition = CHashString(_T("GetPosition")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GetPosition, sizeof(POSITIONDATA), &pos, guiName, NULL);
		pos.m_fXPos += (float) xOff;
		pos.m_fYPos += (float) yOff;
		static DWORD msgHash_SetPosition = CHashString(_T("SetPosition")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_SetPosition, sizeof(POSITIONDATA), &pos, guiName, NULL);
		*/		
		guiElement->SetPageOffset(m_iPageOffsetX, m_iPageOffsetY);
		olIter++;
	}
}

void CGUIPage::SetTextureToRenderTo(ITextureObject *tex)
{
	m_pRenderTarget = tex;
}

DWORD CGUIPage::OnGetPageItemList(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(GUIELEMLIST));
	GUIELEMLIST *gel = (GUIELEMLIST*) param;
	
	IGUIElement *guiElement;
	OBJECTLIST::iterator olIter = m_ObjectList.begin();
	while(olIter != m_ObjectList.end())
	{
		guiElement = dynamic_cast<IGUIElement*>((*olIter));
		if(guiElement == NULL)
		{
			m_ToolBox->SetErrorValue(ERR_NULL_POINTER);
			m_ToolBox->Log(LOGERROR, _T("Could not cast to IGUIElement: %s"), (*olIter)->GetName());
		}
		gel->m_vNames.push_back(guiElement->GetName()->GetString());
		gel->m_vTypes.push_back(guiElement->GetComponentType()->GetString());
		olIter++;
	}
	return MSG_HANDLED_STOP;
}

UINT CGUIPage::UpdatePageItemVertexBuffer(void **pDest, UINT offset, IVertexBufferObject *vb)
{
	UINT writtenVerts = 0, curOffset = offset;
	
	OBJECTLIST::iterator olIter = m_ObjectList.begin();
	IGUIElement *guiElement;
	for(; olIter != m_ObjectList.end(); olIter++)
	{
		guiElement = dynamic_cast<IGUIElement*> (*olIter);
		if(!guiElement)
		{	
			EngineGetToolBox()->SetErrorValue(ERR_NULL_POINTER);
			EngineGetToolBox()->Log(LOGERROR, _T("Could not grab IGUIElement from list of gui elements"));
			return MSG_ERROR;
		}
		UINT tempWritten = guiElement->UpdateBuffer(pDest, curOffset, vb);
		writtenVerts += tempWritten;
		curOffset += tempWritten;
	}		
	return writtenVerts;
}