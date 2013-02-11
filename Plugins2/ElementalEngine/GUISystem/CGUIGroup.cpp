///============================================================================
/// \file		CGUIGroup.cpp
/// \brief		Header file for GUI Element groups concrete class
/// \date		04-29-2005
/// \author		Kenneth Hurley
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

#include "stdafx.h"
#include "CGUIGroup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT(CGUIGroup);
REGISTER_MESSAGE_HANDLER(AddGUIElement, OnAddGUIElement, CGUIGroup);
REGISTER_MESSAGE_HANDLER(RemoveGUIElement, OnRemoveGUIElement, CGUIGroup);
REGISTER_MESSAGE_HANDLER(GetObjUnderCursor, OnGetObjectUnderCursor, CGUIGroup);
REGISTER_MESSAGE_HANDLER(GetObjInsideRect, OnGetObjectInsideRect, CGUIGroup);
REGISTER_MESSAGE_HANDLER(ShowGroup, OnShow, CGUIGroup);
REGISTER_MESSAGE_HANDLER(HideGroup, OnHide, CGUIGroup);
REGISTER_MESSAGE_HANDLER(GetIsGroupVisible, OnGetIsGroupVisible, CGUIGroup);
REGISTER_MESSAGE_HANDLER(GetGroupItemList, OnGetGroupItemList, CGUIGroup);

REGISTER_MESSAGE_HANDLER(SetPosition, OnSetPosition, CGUIGroup);
REGISTER_MESSAGE_HANDLER(SetSize, OnSetSize, CGUIGroup);
REGISTER_MESSAGE_HANDLER(GetPosition, OnGetPosition, CGUIGroup);
REGISTER_MESSAGE_HANDLER(GetSize, OnGetSize, CGUIGroup);
REGISTER_MESSAGE_HANDLER(GetZOrder, OnGetZOrder, CGUIGroup);
REGISTER_MESSAGE_HANDLER(SetZOrder, OnSetZOrder, CGUIGroup);
REGISTER_MESSAGE_HANDLER(SetAttachOffset, OnSetAttachOffset, CGUIGroup);


CGUIGroup::CGUIGroup(IHashString *parentName, IHashString *name) :
	CGUIElement(parentName, name)
{
	m_bEnableGroup = true;
	m_bShowGroup = true;
	m_fWidthRatio = 1.f;
	m_fHeightRatio = 1.f;
	m_fZoomFactor = 1.f;
	m_iPageOffsetX = 0;
	m_iPageOffsetY = 0;
	m_fSaveVersion = 1.0f;
	m_fAttachOffsetX = 0.0f;
	m_fAttachOffsetY = 0.0f;

}

CGUIGroup::~CGUIGroup()
{
}

IComponent *CGUIGroup::Create(int nArgs, va_list argptr)
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
		self = new CGUIGroup(parentName, name);
	}
	catch(...)
	{
		return NULL;
	}
	return self;
}

bool CGUIGroup::Update()
{
	if (m_bTransformIsDirty)
	{
		Vec3 v3AttachPos;

		Matrix4x4 attachMat;
		if (m_pBaseObject)
		{
			m_pBaseObject->GetAttachTransform(&m_hsAttachNodeName, attachMat);
		}

		v3AttachPos = attachMat.GetTranslation();

		WORLDTOSCREENCOORDPARAMS wtscp;
		wtscp.worldXYZ = &v3AttachPos;
		static DWORD msgHash_TransformWorldToScreen = CHashString(_T("TransformWorldToScreen")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_TransformWorldToScreen, sizeof( WORLDTOSCREENCOORDPARAMS ), &wtscp, NULL, NULL );

		/// upper left corner Y coord pos of gui element
		POSITIONDATA pd;
		pd.m_fXPos = (float)wtscp.screenX + m_fAttachOffsetX;
		pd.m_fYPos = (float)wtscp.screenY + m_fAttachOffsetY;
		pd.m_fZPos = 0;
			
		// OnSetPosition also updates child objects.
		OnSetPosition(sizeof(POSITIONDATA), &pd);

		m_bTransformIsDirty = false;
	}

	GUIELEMENTMAP::iterator gemIter;
	//IRenderObject *curObject;
	IObject *curObject;

	for (gemIter = m_SubElements.begin(); gemIter != m_SubElements.end(); gemIter++)
	{
		curObject = gemIter->second;
		curObject->Update();
	}
	return true;
}

bool CGUIGroup::Render(UINT pass, IEffect * override)
{
	GUIELEMENTMAP::iterator gemIter;
	GUIELEMENTMAP sortedList;
	IGUIElement	*guiElement;
	IRenderObject *curObject;
	
	if(!m_bShowGroup)
	{
		// do not render
		return true;
	}

	//this will resort the list of subguielements
	for (gemIter = m_SubElements.begin(); gemIter != m_SubElements.end(); gemIter++)
	{
		guiElement = dynamic_cast<IGUIElement*>(gemIter->second);
		if(guiElement == NULL)
		{
			EngineGetToolBox()->SetErrorValue(ERR_NULL_POINTER);
			EngineGetToolBox()->Log(LOGERROR, _T("could not cast IObject to IGUIElement"));
			return false;
		}
		int key = guiElement->GetZOrder();
		sortedList.insert(GEMPAIR(key, guiElement));
	}

	for (gemIter = sortedList.begin(); gemIter != sortedList.end(); gemIter++)
	{
		curObject = (IRenderObject*)gemIter->second;
		curObject->Render(pass);
	}

	m_SubElements = sortedList;

	return true;
}

void CGUIGroup::RenderExternal(float xPos, float yPos, float width, float height, DWORD state, DWORD color, float angle)
{
	//add
}

IHashString *CGUIGroup::GetComponentType()
{
	static CHashString CGUIGroupCT(_T("CGUIGroup"));
	return &CGUIGroupCT;
}

bool CGUIGroup::IsKindOf(IHashString *compType)
{
	if( CGUIElement::IsKindOf(compType) ) return true;
	return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
}
void CGUIGroup::Serialize(IArchive &ar)
{
	CGUIManager *manager;
	static CHashString managerName(_T("CGUIManager"));
	IObject * Object;
	GUIELEMENTMAP::iterator gemIter;
	StdString guiName;
	StdString guiType;
	int iNumGuiElements = 0;

	if (ar.IsReading())
	{
		CHashString versionName;
		static DWORD msgHash_GetFileVersion = CHashString(_T("GetFileVersion")).GetUniqueID();
		DWORD retval = EngineGetToolBox()->SendMessage(msgHash_GetFileVersion, sizeof(IHashString), &versionName);

		static DWORD vers2_0a = CHashString(_T("2.0a")).GetUniqueID();
		static DWORD vers2_0b = CHashString(_T("2.0b")).GetUniqueID();
		static DWORD vers2_0c = CHashString(_T("2.0c")).GetUniqueID();

		int verNum = 0;
		
		if (versionName.GetUniqueID() == vers2_0a)
		{
			verNum = 3;
		}
		else if (versionName.GetUniqueID() == vers2_0b)
		{
			verNum = 4;
		}
		else if (versionName.GetUniqueID() == vers2_0c)
		{
			verNum = 5;
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

		CGUIElement::Serialize(ar);

		ar.Read(iNumGuiElements);

		if (iNumGuiElements == 0 )
		{
			// this means we are being serialized from the gui editor
			POSITIONDATA pd;
			SIZEDATA sd;
			pd.m_fXPos = m_fPosX;
			pd.m_fYPos = m_fPosY;
			pd.m_fZPos = 0;
			sd.m_fXSize = m_fSizeX;
			sd.m_fYSize = m_fSizeY;
			sd.m_fZSize = 0;
			
			// update the groups sub gui elements too
			OnSetPosition(sizeof(POSITIONDATA), &pd);
			OnSetSize(sizeof(SIZEDATA), &sd);
		}

		if(m_SubElements.size() != 0)
		{
			// do not continue to add groups
			return;
		}

		while(iNumGuiElements != 0)
		{
			ar.Read(guiName);
			ar.Read(guiType);

			CHashString szhGuiName(guiName.c_str());
			CHashString szhGuiType(guiType.c_str());

			manager = dynamic_cast<CGUIManager*>(EngineGetToolBox()->GetComponent( &managerName ));

			if(manager == NULL)
			{
				EngineGetToolBox()->SetErrorValue(ERR_NULL_POINTER);
				EngineGetToolBox()->Log(LOGERROR, _T("could not cast IComponent from toolbox to CGUIManager"));
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

			// insert gui element into list
			m_SubElements.insert(GEMPAIR(m_iZOrder, Object));

			iNumGuiElements--;
		}
	}
	else //else ar.Writing()
	{
		IObject *Object;
		IHashString *szhGuiName;
		IHashString *szhGuiType;
		TCHAR strNum[50];
		int count = 1;
		StdString itemName;
		StdString itemType;

		iNumGuiElements = m_SubElements.size();

		ar.Write(m_fSaveVersion, _T("Version"));

		CGUIElement::Serialize(ar);

		// write the number of gui elements
		ar.Write( iNumGuiElements, _T("NumGuiElements") );

		// iterate through all gui elements in group
		for(gemIter = m_SubElements.begin(); 
			gemIter != m_SubElements.end();
			gemIter++)
		{	
			_ltot(count, strNum, 10);
			itemName = StdString(_T("GuiName")) + strNum;
			itemType = StdString(_T("GuiType")) + strNum;

			//guiName = gemIter->second->GetName()->GetString();
			//guiType = gemIter->second->GetComponentType()->GetString();
			Object = gemIter->second;
			szhGuiName = Object->GetName();
			szhGuiType = Object->GetComponentType();

			ar.Write( szhGuiName->GetString(), itemName );
			ar.Write( szhGuiType->GetString(), itemType );
			count++;
		}	
	}
}

bool CGUIGroup::IsCursorOver(float fCursorPosX, float fCursorPosY)
{	
	if (m_bShowGroup)
	{
		IGUIElement* guiElement;
		GUIELEMENTMAP::iterator gemIter;

		for(gemIter = m_SubElements.begin(); 
			gemIter != m_SubElements.end();
			gemIter++)
		{	
			// if any of the sub elements are selected, return true
			guiElement = dynamic_cast<IGUIElement *>(gemIter->second);

			if (guiElement == NULL)
			{
				EngineGetToolBox()->SetErrorValue(ERR_NULL_POINTER);	
				EngineGetToolBox()->Log(LOGERROR, _T("could not cast IObject to a CGUIElement"));
				return false;
			}

			if(guiElement->IsCursorOver(fCursorPosX, fCursorPosY))
			{
				return true;
			}
		}

		// now return if we are over us
		return CGUIElement::IsCursorOver(fCursorPosX, fCursorPosY);
	}
	else
	{
		return false; // not visible, don't select group!
	}
}

void CGUIGroup::SetInitialPosition()
{	
	POSITIONDATA pd;
	IGUIElement *guiElement;
	IHashString* guiType;
	GUIELEMENTMAP::iterator gemIter = m_SubElements.begin();
	float fLessPosX, fLessPosY;
	float fPosX, fPosY;
	
	if ( m_SubElements.empty() )
	{
		return;
	}

	for(; gemIter != m_SubElements.end(); gemIter++)
	{
		guiElement = dynamic_cast<IGUIElement*>(gemIter->second);
		guiType = ((IObject*)gemIter->second)->GetComponentType();
		if (guiElement == NULL)
		{
			EngineGetToolBox()->SetErrorValue(ERR_NULL_POINTER);
			EngineGetToolBox()->Log(LOGERROR, _T("could not cast IObject to a CGUIElement"));
			return;
		}
		static DWORD msgHash_GetPosition = CHashString(_T("GetPosition")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GetPosition, sizeof(POSITIONDATA), &pd, guiElement->GetName(), guiType);
		fPosX = pd.m_fXPos;
		fPosY = pd.m_fYPos;

		if (gemIter == m_SubElements.begin())
		{
			fLessPosX = fPosX;
			fLessPosY = fPosY;
		}
		else
		{
			fLessPosX = (fPosX < fLessPosX) ? fPosX : fLessPosX;
			fLessPosY = (fPosY < fLessPosY) ? fPosY : fLessPosY;
		}
	}

	// set the new position
	pd.m_fXPos = fLessPosX;
	pd.m_fYPos = fLessPosY;
	CGUIElement::OnSetPosition(sizeof(POSITIONDATA), &pd);
}

void CGUIGroup::SetInitialSize()
{
	IHashString* guiName;
	IHashString* guiType;
	GUIELEMENTMAP::iterator gemIter;

	POSITIONDATA tempPos;
	SIZEDATA tempSize;
	
	float left = 0.f;
	float right = 0.f;
	float top = 0.f;
	float bottom = 0.f;

	gemIter = m_SubElements.begin();
	if (gemIter != m_SubElements.end())
	{
		// initialize left, right, top, bottom rectangle
		guiName = ((IObject*)gemIter->second)->GetName();
		guiType = ((IObject*)gemIter->second)->GetComponentType();
		IToolBox * ToolBox = EngineGetToolBox();
		static DWORD msgHash_GetPosition = CHashString(_T("GetPosition")).GetUniqueID();
		ToolBox->SendMessage(msgHash_GetPosition, sizeof(POSITIONDATA), &tempPos, guiName, guiType);
		static DWORD msgHash_GetSize = CHashString(_T("GetSize")).GetUniqueID();
		ToolBox->SendMessage(msgHash_GetSize, sizeof(SIZEDATA), &tempSize, guiName, guiType);

		left = m_fPosX;
		top = m_fPosY;
		right = (tempPos.m_fXPos + tempSize.m_fXSize);
		bottom = (tempPos.m_fYPos + tempSize.m_fYSize);

		for(gemIter++; gemIter != m_SubElements.end();
			gemIter++)
		{	
			guiName = ((IObject*)gemIter->second)->GetName();
			guiType = ((IObject*)gemIter->second)->GetComponentType();
			IToolBox * ToolBox = EngineGetToolBox();
			static DWORD msgHash_GetPosition = CHashString(_T("GetPosition")).GetUniqueID();
			ToolBox->SendMessage(msgHash_GetPosition, sizeof(POSITIONDATA), &tempPos, guiName, guiType);
			static DWORD msgHash_GetSize = CHashString(_T("GetSize")).GetUniqueID();
			ToolBox->SendMessage(msgHash_GetSize, sizeof(SIZEDATA), &tempSize, guiName, guiType);

			if (tempPos.m_fXPos < left)
			{
				left = tempPos.m_fXPos;
			}

			if (tempPos.m_fYPos < top)
			{
				top = tempPos.m_fYPos;
			}

			if ((tempPos.m_fXPos + tempSize.m_fXSize) > right)
			{
				right = (tempPos.m_fXPos + tempSize.m_fXSize);
			}

			if ((tempPos.m_fYPos + tempSize.m_fYSize) > bottom)
			{
				bottom = (tempPos.m_fYPos + tempSize.m_fYSize);
			}
		}
	}

	m_fSizeX = max<float>(right - left, 10.0f);
	m_fSizeY = max<float>(bottom - top, 10.0f);
}


DWORD CGUIGroup::OnGetPosition(DWORD msgSize, void *param)
{
	// in case something moved...
	SetInitialPosition();
	VERIFY_MESSAGE_SIZE(msgSize, sizeof(POSITIONDATA));
	CGUIElement::OnGetPosition(msgSize, param);

	return MSG_HANDLED_STOP;	
}

DWORD CGUIGroup::OnSetAttachOffset(DWORD msgSize, void *param)
{
	POSITIONDATA *pd;

	VERIFY_MESSAGE_SIZE(msgSize, sizeof(POSITIONDATA));

	pd = (POSITIONDATA *)param;

	m_fAttachOffsetX = pd->m_fXPos;
	m_fAttachOffsetY = pd->m_fYPos;

	return MSG_HANDLED_STOP;
}
DWORD CGUIGroup::OnSetPosition(DWORD msgSize, void *param)
{
	IHashString* guiName;
	IHashString* guiType;
	POSITIONDATA *pd;
	POSITIONDATA curPos;
	float dx;		/// delta X position for children
	float dy;		/// delta Y position for children

	VERIFY_MESSAGE_SIZE(msgSize, sizeof(POSITIONDATA));

	pd = (POSITIONDATA *)param;

	dx = pd->m_fXPos - GetPosX();
	dy = pd->m_fYPos - GetPosY();

	GUIELEMENTMAP::iterator gemIter;
	for(gemIter = m_SubElements.begin(); 
		gemIter != m_SubElements.end();
		gemIter++)
	{			
		guiName = ((IObject*)gemIter->second)->GetName();
		guiType = ((IObject*)gemIter->second)->GetComponentType();
		static DWORD msgHash_GetPosition = CHashString(_T("GetPosition")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GetPosition, sizeof(POSITIONDATA), &curPos, guiName, guiType);
		curPos.m_fXPos += dx;
		curPos.m_fYPos += dy;
		static DWORD msgHash_SetPosition = CHashString(_T("SetPosition")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_SetPosition, sizeof(POSITIONDATA), &curPos, guiName, guiType);		
	}

	// save our current position
	CGUIElement::OnSetPosition(msgSize, param);
	return MSG_HANDLED_PROCEED;
}

// get the size of the element
DWORD CGUIGroup::OnGetSize(DWORD msgSize, void *param)
{
	// in case something moved...
	SetInitialSize();
	VERIFY_MESSAGE_SIZE(msgSize, sizeof(SIZEDATA));
	CGUIElement::OnGetSize(msgSize, param);
	return MSG_HANDLED_STOP;
}

// set the size of the element
DWORD CGUIGroup::OnSetSize(DWORD msgSize, void *param)
{
	IHashString* guiName;
	IHashString* guiType;
	GUIELEMENTMAP::iterator gemIter;

	SIZEDATA *szData;
	POSITIONDATA tempPos;
	SIZEDATA tempSize;
	float dx;			/// delta X for moving relative to parent
	float dy;			/// delta Y for moving relative to parent
	float dsx;			/// delta X for scaling objects relative to parent
	float dsy;			/// delta Y for scaling objects relative to parent

	VERIFY_MESSAGE_SIZE(msgSize, sizeof(SIZEDATA));
	szData = (SIZEDATA *)param;

	// get scale factor
	dsx = (szData->m_fXSize == 0.0f) ? 0.0f : (szData->m_fXSize / GetWidth()); 
	dsy = (szData->m_fYSize == 0.0f) ? 0.0f : (szData->m_fYSize / GetHeight());

	for(gemIter = m_SubElements.begin(); gemIter != m_SubElements.end();
		gemIter++)
	{	
		guiName = ((IObject*)gemIter->second)->GetName();
		guiType = ((IObject*)gemIter->second)->GetComponentType();
		static DWORD msgHash_GetPosition = CHashString(_T("GetPosition")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GetPosition, sizeof(POSITIONDATA), &tempPos, guiName, guiType);
		static DWORD msgHash_GetSize = CHashString(_T("GetSize")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GetSize, sizeof(SIZEDATA), &tempSize, guiName, guiType);
		
		dx = tempPos.m_fXPos - GetPosX();
		dy = tempPos.m_fYPos - GetPosY();
		// scale position relative to parent
		dx *= dsx;
		dy *= dsy;
		tempPos.m_fXPos = GetPosX() + dx;
		tempPos.m_fYPos = GetPosY() + dy;
		tempSize.m_fXSize *= dsx;
		tempSize.m_fYSize *= dsy;

		static DWORD msgHash_SetPosition = CHashString(_T("SetPosition")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_SetPosition, sizeof(POSITIONDATA), &tempPos, guiName, guiType);
		static DWORD msgHash_SetSize = CHashString(_T("SetSize")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_SetSize, sizeof(SIZEDATA), &tempSize, guiName, guiType);
	}

	CGUIElement::OnSetSize(msgSize, param);
	return MSG_HANDLED_PROCEED;
}


DWORD CGUIGroup::OnAddGUIElement(DWORD msgSize, void *param)
{
	ADDGUIELEMENT *age;
	VERIFY_MESSAGE_SIZE(msgSize, sizeof(ADDGUIELEMENT));
	IObject * Object;
	
	IToolBox *toolBox = EngineGetToolBox();
	CGUIManager *manager;
	static CHashString hashString(_T("CGUIManager"));
	manager = dynamic_cast<CGUIManager*>(toolBox->GetComponent( &hashString ));

	if(manager == NULL)
	{
		EngineGetToolBox()->SetErrorValue(ERR_NULL_POINTER);
		EngineGetToolBox()->Log(LOGERROR, _T("could not cast IComponent from toolbox to CGUIManager"));
		return MSG_ERROR;
	}

	age = (ADDGUIELEMENT *)param;
	Object = manager->GetObject(age->m_GuiName, age->m_CompType);

	if(Object == NULL)
	{
		EngineGetToolBox()->SetErrorValue(ERR_NULL_POINTER);
		EngineGetToolBox()->Log(LOGERROR, _T("could not get object to add from CGUIManager"));
		return MSG_ERROR;
	}
	
	// check if the object to add is a group
	IHashString *compType = Object->GetComponentType();
	static CHashString group(_T("CGUIGroup"));
	if(compType->GetUniqueID() == group.GetUniqueID())
	{
		CGUIGroup *addgroup = dynamic_cast<CGUIGroup*>(Object);
		if(addgroup == NULL)
		{
			//log error
			EngineGetToolBox()->SetErrorValue(ERR_NULL_POINTER);
			EngineGetToolBox()->Log(LOGERROR, _T("could not cast to CGUIGroup"));
			return MSG_ERROR;
		}

		// check if adding this new group causes errors
		if( addgroup->GroupExists(GetName()) || GroupExists(addgroup->GetName()) 
			|| addgroup->GetName()->GetUniqueID() == GetName()->GetUniqueID() )
		{
			// cannot add this group 
			// log a warning?
			return MSG_NOT_HANDLED;
		}
	}
	
	IGUIElement *elem = dynamic_cast<IGUIElement*>(Object);
	elem->SetWidthRatio(m_fWidthRatio);
	elem->SetHeightRatio(m_fHeightRatio);
	elem->SetPageOffset(m_iPageOffsetX, m_iPageOffsetY);
	elem->SetZoomFactor(m_fZoomFactor);

	m_SubElements.insert(GEMPAIR(age->m_ZOrder, Object));

	// setup initial position
	SetInitialPosition();

	// setup initial size
	SetInitialSize();

	return MSG_HANDLED_PROCEED;
}

DWORD CGUIGroup::OnRemoveGUIElement(DWORD msgSize, void *param)
{
	GUIELEMENTMAP::iterator gemIter;
	IObject *curObject;
	IHashString *findName;

	VERIFY_MESSAGE_SIZE(msgSize, sizeof(IHashString));
	
	findName = (IHashString *)param;

	for (gemIter = m_SubElements.begin(); gemIter != m_SubElements.end(); gemIter++)
	{
		curObject = gemIter->second;
		if (*curObject->GetName() == *findName)
		{
			m_SubElements.erase(gemIter);
			break;
		}
	}

	// setup initial position
	SetInitialPosition();

	// setup initial size
	SetInitialSize();
	
	return MSG_HANDLED_PROCEED;
}

DWORD CGUIGroup::OnLeftMousePress(DWORD msgSize, void *params)
{
	DWORD retVal = MSG_NOT_HANDLED;
	return retVal;
}

// 
DWORD CGUIGroup::OnLeftMouseRelease(DWORD msgSize, void *params)
{
	DWORD retVal = MSG_NOT_HANDLED;
	return retVal;
}

DWORD CGUIGroup::OnMousePosition(DWORD msgSize, void *params)
{
	DWORD retVal = MSG_NOT_HANDLED;
	GUIELEMENTMAP::iterator gemIter;
	IGUIElement *guiElement;

	if(!m_bEnableGroup || !m_bShowGroup)
	{
		return MSG_NOT_HANDLED;
	}

	for(gemIter = m_SubElements.begin(); gemIter != m_SubElements.end();
		gemIter++)
	{
		guiElement = dynamic_cast<IGUIElement*>(gemIter->second);
		if(!guiElement)
		{	
			EngineGetToolBox()->SetErrorValue(ERR_NULL_POINTER);
			EngineGetToolBox()->Log(LOGERROR, _T("Could not grab IGUIElement from group's sublist of gui elements"));
			return MSG_ERROR;
		}
		retVal = guiElement->OnMousePosition(msgSize, params);
		if(retVal == MSG_HANDLED_STOP || retVal == MSG_ERROR)
		{
			return retVal;
		}
	}
	return retVal;
}

DWORD CGUIGroup::OnKeyPress(DWORD msgSize, void *params)
{
	DWORD retVal = MSG_NOT_HANDLED;
	return retVal;
}

bool CGUIGroup::InsideRect(int checkX, int checkY, int x1, int y1, int x2, int y2)
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

	if( left <= checkX && top <= checkY
		&& right >= checkX && bottom >= checkY )
	{
		return true;
	}
	else
	{
		return false;
	}
}

DWORD CGUIGroup::OnGetObjectInsideRect(DWORD size, void *params)
{
	DWORD retVal;

	GUIELEMENTMAP::iterator gemIter;
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

	for(gemIter = m_SubElements.begin(); gemIter != m_SubElements.end();
		gemIter++)
	{
		object = dynamic_cast<IGUIElement*>(gemIter->second);
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

DWORD CGUIGroup::OnGetObjectUnderCursor(DWORD size, void *params)
{
	GUIELEMENTMAP::iterator gemIter;
	IGUIElement *lesserGE = NULL;
	IGUIElement *newGE;
	OBJUNDERCURSOR *data;

	VERIFY_MESSAGE_SIZE(size, sizeof(OBJUNDERCURSOR));
	data = (OBJUNDERCURSOR*)params;

	float fCursorPosX = data->m_fPosX;
	float fCursorPosY = data->m_fPosY;
	data->m_Name = NULL;
	data->m_CompType = NULL;

	for(gemIter = m_SubElements.begin(); gemIter != m_SubElements.end();
		gemIter++)
	{
		newGE = dynamic_cast<IGUIElement*>(gemIter->second);
		if(!newGE)
		{	
			EngineGetToolBox()->SetErrorValue(ERR_NULL_POINTER);
			EngineGetToolBox()->Log(LOGERROR, _T("Could not grab IGUIElement from group's sublist of gui elements"));
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

DWORD CGUIGroup::OnDisable(DWORD msgSize, void *param)
{
	VERIFY_MESSAGE_SIZE(msgSize, 0);

	m_bEnableGroup = false;

	// traverse through all gui objects, disabling each?

	return MSG_HANDLED_STOP;
}

DWORD CGUIGroup::OnEnable(DWORD msgSize, void *param)
{
	VERIFY_MESSAGE_SIZE(msgSize, 0);

	m_bEnableGroup = true;

	// traverse through all gui objects, enabling each?

	return MSG_HANDLED_STOP;
}

DWORD CGUIGroup::OnHide(DWORD msgSize, void *param)
{
	VERIFY_MESSAGE_SIZE(msgSize, 0);

	m_bShowGroup = false;

	return MSG_HANDLED_STOP;
}

DWORD CGUIGroup::OnShow(DWORD msgSize, void *param)
{
	VERIFY_MESSAGE_SIZE(msgSize, 0);

	m_bShowGroup = true;

	return MSG_HANDLED_STOP;
}

bool CGUIGroup::GroupExists(IHashString *groupName)
{
	bool bVal = false;
	IObject *object;
	CGUIGroup *group;
	IHashString *objType;
	IHashString *objName;
	static CHashString groupType(_T("CGUIGroup"));
	GUIELEMENTMAP::iterator geIter = m_SubElements.begin();

	// traverse through all objects in map of sub elements
	for(; geIter != m_SubElements.end(); geIter++)
	{
		object = geIter->second;
		objType = object->GetComponentType();
		objName = object->GetName();
		
		// if object is a group
		if(objType->GetUniqueID() == groupType.GetUniqueID())
		{
			// cast object to group
			group = dynamic_cast<CGUIGroup*>(object);
			if(group == NULL)
			{
				//log error
				EngineGetToolBox()->SetErrorValue(ERR_NULL_POINTER);
				EngineGetToolBox()->Log(LOGERROR, _T("could not cast object to CGUIGroup"));
				bVal = true;
				break;
			}

			// compare the names
			if(objName->GetUniqueID() == groupName->GetUniqueID())
			{
				// names are the same..error
				bVal = true;
				break;
			}
			else
			{
				// check if groupName is in the group's subelement map
				if( group->GroupExists(groupName) )
				{
					bVal = true;
					break;
				}
			}
		}
	}
	return bVal;
}

/// Sets width factor for odd window aspect ratio adjustment
/// \param value - value to set ratio to
void CGUIGroup::SetWidthRatio(float value)
{
	GUIELEMENTMAP::iterator gemIter;
	IGUIElement *guiElement;

	if(!m_bEnableGroup || !m_bShowGroup)
	{
		return;
	}

	for(gemIter = m_SubElements.begin(); gemIter != m_SubElements.end();
		gemIter++)
	{
		guiElement = dynamic_cast<IGUIElement*>(gemIter->second);
		if(!guiElement)
		{	
			EngineGetToolBox()->SetErrorValue(ERR_NULL_POINTER);
			EngineGetToolBox()->Log(LOGERROR, _T("Could not grab IGUIElement from group's sublist of gui elements"));
			return;
		}
		guiElement->SetWidthRatio(value);
	}	
	m_fWidthRatio = value;
}

/// Sets height factor for odd window aspect ratio adjustment
/// \param value - value to set ratio to
void CGUIGroup::SetHeightRatio(float value)
{
	GUIELEMENTMAP::iterator gemIter;
	IGUIElement *guiElement;

	if(!m_bEnableGroup || !m_bShowGroup)
	{
		return;
	}

	for(gemIter = m_SubElements.begin(); gemIter != m_SubElements.end();
		gemIter++)
	{
		guiElement = dynamic_cast<IGUIElement*>(gemIter->second);
		if(!guiElement)
		{	
			EngineGetToolBox()->SetErrorValue(ERR_NULL_POINTER);
			EngineGetToolBox()->Log(LOGERROR, _T("Could not grab IGUIElement from group's sublist of gui elements"));
			return;
		}
		guiElement->SetHeightRatio(value);
	}	
	m_fHeightRatio = value;
}

/// Sets zoom factor for zooming in (used in editor)
/// \param value - value to set zoom to
void CGUIGroup::SetZoomFactor(float value)
{
	GUIELEMENTMAP::iterator gemIter;
	IGUIElement *guiElement;

	if(!m_bEnableGroup || !m_bShowGroup)
	{
		return;
	}

	for(gemIter = m_SubElements.begin(); gemIter != m_SubElements.end();
		gemIter++)
	{
		guiElement = dynamic_cast<IGUIElement*>(gemIter->second);
		if(!guiElement)
		{	
			EngineGetToolBox()->SetErrorValue(ERR_NULL_POINTER);
			EngineGetToolBox()->Log(LOGERROR, _T("Could not grab IGUIElement from group's sublist of gui elements"));
			return;
		}
		guiElement->SetZoomFactor(value);
	}	
	m_fZoomFactor = value;
}

void CGUIGroup::SetPageOffset(int xOff, int yOff)
{
	GUIELEMENTMAP::iterator gemIter;
	IGUIElement *guiElement;

	for(gemIter = m_SubElements.begin(); gemIter != m_SubElements.end();
		gemIter++)
	{
		guiElement = dynamic_cast<IGUIElement*>(gemIter->second);
		if(!guiElement)
		{	
			EngineGetToolBox()->SetErrorValue(ERR_NULL_POINTER);
			EngineGetToolBox()->Log(LOGERROR, _T("Could not grab IGUIElement from group's sublist of gui elements"));
			return;
		}
		guiElement->SetPageOffset(xOff, yOff);
	}	
	m_iPageOffsetX = xOff;
	m_iPageOffsetY = yOff;
}

DWORD CGUIGroup::OnGetGroupItemList(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(GUIELEMLIST));
	GUIELEMLIST *gel = (GUIELEMLIST*) param;

	GUIELEMENTMAP::iterator gemIter;
	IGUIElement *guiElement;

	for(gemIter = m_SubElements.begin(); gemIter != m_SubElements.end();
		gemIter++)
	{
		guiElement = dynamic_cast<IGUIElement*>(gemIter->second);
		if(!guiElement)
		{	
			EngineGetToolBox()->SetErrorValue(ERR_NULL_POINTER);
			EngineGetToolBox()->Log(LOGERROR, _T("Could not grab IGUIElement from group's sublist of gui elements"));
			return MSG_ERROR;
		}
		gel->m_vNames.push_back(guiElement->GetName()->GetString());
		gel->m_vTypes.push_back(guiElement->GetComponentType()->GetString());
	}	
	return MSG_HANDLED_STOP;
}

DWORD CGUIGroup::OnGetIsGroupVisible(DWORD size, void* param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(bool));
	bool *isVisible = (bool*) param;
	*isVisible = m_bShowGroup;
	return MSG_HANDLED_STOP;
}

UINT CGUIGroup::UpdateBuffer( void **pDest, UINT offset, IVertexBufferObject *vb)
{
	GUIELEMENTMAP::iterator gemIter;
	IGUIElement *guiElement;
	UINT writtenVerts = 0, curOffset = offset;

	for(gemIter = m_SubElements.begin(); gemIter != m_SubElements.end();
		gemIter++)
	{
		guiElement = dynamic_cast<IGUIElement*>(gemIter->second);
		if(!guiElement)
		{	
			EngineGetToolBox()->SetErrorValue(ERR_NULL_POINTER);
			EngineGetToolBox()->Log(LOGERROR, _T("Could not grab IGUIElement from group's sublist of gui elements"));
			return MSG_ERROR;
		}
		UINT tempWritten = guiElement->UpdateBuffer(pDest, curOffset, vb);
		writtenVerts += tempWritten;
		curOffset += tempWritten;
	}	
	return writtenVerts;
}

void CGUIGroup::SortElements()
{
	GUIELEMENTMAP::iterator gemIter;
	GUIELEMENTMAP sortedList;
	IGUIElement	*guiElement;		

	//this will resort the list of subguielements
	for (gemIter = m_SubElements.begin(); gemIter != m_SubElements.end(); gemIter++)
	{
		guiElement = dynamic_cast<IGUIElement*>(gemIter->second);
		if(guiElement == NULL)
		{
			EngineGetToolBox()->SetErrorValue(ERR_NULL_POINTER);
			EngineGetToolBox()->Log(LOGERROR, _T("could not cast IObject to IGUIElement"));
			continue;
		}
		int key = guiElement->GetZOrder();
		sortedList.insert(GEMPAIR(key, guiElement));
	}

	m_SubElements = sortedList;	
}