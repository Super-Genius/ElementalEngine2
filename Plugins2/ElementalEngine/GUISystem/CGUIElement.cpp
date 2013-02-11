///==========================================================================
/// \file	CGUIElement.cpp
/// \brief	CGUIElement implementation
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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


REGISTER_COMPONENT(CGUIElement);

REGISTER_MESSAGE_HANDLER(SetPosition, OnSetPosition, CGUIElement);
REGISTER_MESSAGE_HANDLER(SetSize, OnSetSize, CGUIElement);
REGISTER_MESSAGE_HANDLER(GetPosition, OnGetPosition, CGUIElement);
REGISTER_MESSAGE_HANDLER(GetSize, OnGetSize, CGUIElement);
REGISTER_MESSAGE_HANDLER(GetZOrder, OnGetZOrder, CGUIElement);
REGISTER_MESSAGE_HANDLER(SetZOrder, OnSetZOrder, CGUIElement);

// mouse input messages
REGISTER_MESSAGE_HANDLER(MouseMove, OnMousePosition, CGUIElement);
REGISTER_MESSAGE_HANDLER(LeftMousePress, OnLeftMousePress, CGUIElement);
REGISTER_MESSAGE_HANDLER(LeftMouseRelease, OnLeftMouseRelease, CGUIElement);
REGISTER_MESSAGE_HANDLER(KeyPress, OnKeyPress, CGUIElement);

REGISTER_MESSAGE_HANDLER(RemoveFocus, OnRemoveFocus, CGUIElement);

// action handler messages
REGISTER_MESSAGE_HANDLER(Disable, OnDisable, CGUIElement);
REGISTER_MESSAGE_HANDLER(Enable, OnEnable, CGUIElement);

// keybindings
REGISTER_MESSAGE_HANDLER(GUIKeyLeft, OnKeyLeft, CGUIElement);
REGISTER_MESSAGE_HANDLER(GUIKeyRight, OnKeyRight, CGUIElement);
REGISTER_MESSAGE_HANDLER(GUIKeyHome, OnKeyHome, CGUIElement);
REGISTER_MESSAGE_HANDLER(GUIKeyEnd, OnKeyEnd, CGUIElement);
REGISTER_MESSAGE_HANDLER(GUIKeyDel, OnKeyDel, CGUIElement);
REGISTER_MESSAGE_HANDLER(GUIKeyBackSpace, OnKeyBackSpace, CGUIElement);
REGISTER_MESSAGE_HANDLER(GUIKeyEnter, OnKeyEnter, CGUIElement);


///
/// Constructor / Destructor
///

CGUIElement::CGUIElement(IHashString *parentName, IHashString *name): 
	SCENEOBJECTTEMPLATE(CGUIManager, CGUIElement, IGUIElement, parentName, name)
{
	m_fPosX = 0.f;
	m_fPosY = 0.f;
	m_fSizeX = 0.f;
	m_fSizeY = 0.f;
	m_iZOrder = 0;
	m_fWidthRatio = 1.f;
	m_fHeightRatio = 1.f;
	m_fZoomFactor = 1.f;
	m_bTransformIsDirty = false;
}

CGUIElement::~CGUIElement()
{
	//DeInit();
}

/// \brief The Initialization for the Object
/// \return pointer to an IComponent
IComponent *CGUIElement::Create(int nArgs, va_list argptr)
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
		self = new CGUIElement(parentName, name);
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

bool CGUIElement::Update()
{
	return true;
}

IHashString *CGUIElement::GetComponentType()
{
	static CHashString CGUIElementCT(_T("CGUIElement"));
	return &CGUIElementCT;
}

bool CGUIElement::IsKindOf(IHashString *compType)
{
	static CHashString elementType = _T("CGUIElement");
	return (compType->GetUniqueID() == elementType.GetUniqueID());
}

// draw gui element w/ greates zorder first
bool CGUIElement::IsLessThan(const IRenderObject *obj, DWORD sortMethod, int curRenderPass)
{
	if(GetZOrder() > ((IGUIElement*)obj)->GetZOrder())
	{
		return true;
	}
	else 
	{
		return false;
	}
}

float CGUIElement::GetWidth()
{
	return m_fSizeX;
}

float CGUIElement::GetHeight()
{
	return m_fSizeY;
}

float CGUIElement::GetPosX()
{
	return m_fPosX;
}

float CGUIElement::GetPosY()
{
	return m_fPosY;
}
void CGUIElement::Serialize(IArchive &ar)
{
	StdString name;
	StdString type;
	
	int iOldZ = m_iZOrder;

	if(ar.IsReading())
	{
		ar.Read(m_fPosX);
		ar.Read(m_fPosY);
		ar.Read(m_fSizeX);
		ar.Read(m_fSizeY);
		ar.Read(m_iZOrder);

		// notify sort
		if(iOldZ != m_iZOrder)
		{
			NotifySort();
		}
	}
	else
	{
		name = GetName()->GetString();
		type = GetComponentType()->GetString(); //should return the type of the subclass (not CGUIElement)

		//start class
		//ar.StartClass(type); // 
		//ar.Write(name, _T("name"));

		ar.Write(m_fPosX, _T("PosX"));
		ar.Write(m_fPosY, _T("PosY"));
		ar.Write(m_fSizeX, _T("Width"));
		ar.Write(m_fSizeY, _T("Height"));
		ar.Write(m_iZOrder, _T("ZOrder"));

	}
}

bool CGUIElement::IsCursorOver(float fCursorPosX, float fCursorPosY)
{
	if(fCursorPosX < m_fPosX+m_fSizeX && fCursorPosX > m_fPosX &&
		fCursorPosY < m_fPosY+m_fSizeY && fCursorPosY > m_fPosY)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CGUIElement::NotifySort()
{
	CGUIManager *manager;
	static CHashString managerName(_T("CGUIManager"));
	manager = dynamic_cast<CGUIManager*>(EngineGetToolBox()->GetComponent( &managerName ));

	if(manager == NULL)
	{
		EngineGetToolBox()->SetErrorValue(ERR_NULL_POINTER);
		EngineGetToolBox()->Log(LOGERROR, _T("could not cast IComponent from toolbox to CGUIManager"));
	}

	manager->SortActivePage();
}

void CGUIElement::SetFocus()
{
	CGUIManager *manager;
	static CHashString managerName(_T("CGUIManager"));
	
	manager = dynamic_cast<CGUIManager*>(EngineGetToolBox()->GetComponent( &managerName ));

	if(manager == NULL)
	{
		EngineGetToolBox()->SetErrorValue(ERR_NULL_POINTER);
		EngineGetToolBox()->Log(LOGERROR, 
			_T("CGUIElement::OnLeftMousePress could not cast IComponent from toolbox to CGUIManager"));
	}
	
	// set focus 
	manager->SetFocus(this);
}

DWORD CGUIElement::OnSetPosition(DWORD msgSize, void *param)
{
	POSITIONDATA *pd;
	VERIFY_MESSAGE_SIZE(msgSize, sizeof(POSITIONDATA));
	pd = (POSITIONDATA *)param;
	
	// we only need the first two
	m_fPosX = pd->m_fXPos;
	m_fPosY = pd->m_fYPos;

	return MSG_HANDLED_STOP;	
}

DWORD CGUIElement::OnGetPosition(DWORD msgSize, void *param)
{
	POSITIONDATA *pd;
	VERIFY_MESSAGE_SIZE(msgSize, sizeof(POSITIONDATA));
	pd = (POSITIONDATA *)param;
	
	// we only need the first two
	pd->m_fXPos = m_fPosX;
	pd->m_fYPos = m_fPosY;

	return MSG_HANDLED_STOP;	
}

DWORD CGUIElement::OnSetSize(DWORD msgSize, void *param)
{
	SIZEDATA *pd;
	VERIFY_MESSAGE_SIZE(msgSize, sizeof(SIZEDATA));
	pd = (SIZEDATA *)param;

	// we only need the first two
	m_fSizeX = pd->m_fXSize;
	m_fSizeY = pd->m_fYSize;

	return MSG_HANDLED_STOP;	
}

// get the size of the element
DWORD CGUIElement::OnGetSize(DWORD msgSize, void *param)
{
	SIZEDATA *pd;
	VERIFY_MESSAGE_SIZE(msgSize, sizeof(SIZEDATA));
	pd = (SIZEDATA *)param;

	// we only need the first two
	pd->m_fXSize = m_fSizeX;
	pd->m_fYSize = m_fSizeY;

	return MSG_HANDLED_STOP;	
}

DWORD CGUIElement::OnLeftMousePress(DWORD msgSize, void *param)
{
	return MSG_HANDLED_PROCEED;
}

DWORD CGUIElement::OnLeftMouseRelease(DWORD msgSize, void *param)
{
	return MSG_HANDLED_PROCEED;
}

DWORD CGUIElement::OnMousePosition(DWORD msgSize, void *param)
{
	return MSG_HANDLED_PROCEED;
}

DWORD CGUIElement::OnKeyPress(DWORD msgSize, void *param)
{
	return MSG_HANDLED_PROCEED;
}

DWORD CGUIElement::OnSetZOrder(DWORD msgSize, void *param)
{
	ZORDERDATA *zData;
	VERIFY_MESSAGE_SIZE(sizeof(ZORDERDATA), msgSize);
	
	zData = ((ZORDERDATA*)param);

	m_iZOrder = zData->m_iZOrder;

	// notify the gui manager to sort active page
	NotifySort();

	return MSG_HANDLED_STOP;
}

DWORD CGUIElement::OnGetZOrder(DWORD msgSize, void *param)
{
	VERIFY_MESSAGE_SIZE(sizeof(ZORDERDATA), msgSize);

	((ZORDERDATA*)param)->m_iZOrder = m_iZOrder;

	return MSG_HANDLED_STOP;
}

void CGUIElement::GetBoundingSphere( Vec3 &position, float &radius )
{
	position.Set( 0,0,0 );
	radius = 0;
}

Vec3 &CGUIElement::GetPosition()
{
	static Vec3 tempVec(0,0,0);
	return tempVec;
}

IHashString* CGUIElement::GetBaseSortClass()
{
	static CHashString sortClass(_T("IGUIElement"));
	return &sortClass;
}

DWORD CGUIElement::OnDisable(DWORD msgSize, void *param)
{
	return MSG_NOT_HANDLED;
}

DWORD CGUIElement::OnEnable(DWORD msgSize, void *param)
{
	return MSG_NOT_HANDLED;
}

DWORD CGUIElement::OnRemoveFocus(DWORD msgSize, void *param)
{
	return MSG_NOT_HANDLED;
}

// keyboard bindings
DWORD CGUIElement::OnKeyLeft(DWORD msgSize, void *param)
{
	return MSG_HANDLED_PROCEED;
}
DWORD CGUIElement::OnKeyRight(DWORD msgSize, void *param)
{
	return MSG_HANDLED_PROCEED;
}
DWORD CGUIElement::OnKeyHome(DWORD msgSize, void *param)
{
	return MSG_HANDLED_PROCEED;
}
DWORD CGUIElement::OnKeyEnd(DWORD msgSize, void *param)
{
	return MSG_HANDLED_PROCEED;
}
DWORD CGUIElement::OnKeyDel(DWORD msgSize, void *param)
{
	return MSG_HANDLED_PROCEED;
}
DWORD CGUIElement::OnKeyBackSpace(DWORD msgSize, void *param)
{
	return MSG_HANDLED_PROCEED;
}
DWORD CGUIElement::OnKeyEnter(DWORD msgSize, void *param)
{
	return MSG_HANDLED_PROCEED;
}
