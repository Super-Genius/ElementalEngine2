///============================================================================
/// \file		CGUIBackground.h
/// \brief		Background color, used in editor
/// \date		09/12/2006
/// \author		Nick Tourte
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

#include "stdafx.h"
#include "CGUIBackground.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT(CGUIBackground);

REGISTER_MESSAGE_HANDLER(SetPosition, OnSetPosition, CGUIBackground);
REGISTER_MESSAGE_HANDLER(SetSize, OnSetSize, CGUIBackground);
REGISTER_MESSAGE_HANDLER(GetPosition, OnGetPosition, CGUIBackground);
REGISTER_MESSAGE_HANDLER(GetSize, OnGetSize, CGUIBackground);
REGISTER_MESSAGE_HANDLER(SetRect, OnSetRect, CGUIBackground);
REGISTER_MESSAGE_HANDLER(SetColor, OnSetColor, CGUIBackground);

CGUIBackground::CGUIBackground(IHashString *parentName, IHashString *name) : 
	SCENEOBJECTTEMPLATE(CGUIManager, CGUIBackground, IGUIElement, parentName, name)
{
	// grab the render interface
	GETRENDERERINTERFACEPARAMS renderInterface;

	static DWORD msgHash_GetRendererInterface = CHashString(_T("GetRendererInterface")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetRendererInterface, sizeof(GETRENDERERINTERFACEPARAMS), &renderInterface);

	m_Renderer = renderInterface.m_RendererInterface;

	m_iZOrder = 1000;

	m_iPosX  = m_iPosY = 0;
	m_iSizeX = m_iSizeY = 0;
	m_iRectX1 = m_iRectX2 = m_iRectY1 = m_iRectY2 = 0;

	m_fWidthRatio = 1.f;
	m_fHeightRatio = 1.f;
	m_fZoomFactor = 1.f;
	m_iPageOffsetX = m_iPageOffsetY = 0;

	m_iBGColor = 0xffffffff;

	m_VertexBuffer = NULL;
	m_VBOffset = 0;
}

// create function for this world loader
IComponent *CGUIBackground::Create(int nArgs, va_list argptr)
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
		self = new CGUIBackground(parentName, name);
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

bool CGUIBackground::Update()
{
	return true;
}

IHashString* CGUIBackground::GetComponentType()
{
	static CHashString CGUIBackgroundCT(_T("CGUIBackground"));
	return &CGUIBackgroundCT;
}

bool CGUIBackground::IsCursorOver(int iCursorPosX, int iCursorPosY)
{
	return false;
}

void CGUIBackground::Serialize(IArchive &ar)
{
// no serialization for this guy!	
}

// render mask
bool CGUIBackground::Render(UINT pass, IEffect * override)
{
	// TODO: support background texture
	m_Renderer->SetTexture(0, NULL);
	m_Renderer->SetEffect(0, NULL);
	m_Renderer->RenderVertexBuffer(m_VertexBuffer, m_VBOffset, 2);
    
	return true;
}

UINT CGUIBackground::UpdateBuffer( void **pDest, UINT offset, IVertexBufferObject *vb)
{
	if ((vb) &&(pDest))
	{
		m_VertexBuffer = vb;
		m_VBOffset = offset;
		UINT vertsWritten = 0;	
		
		float xScale = m_fWidthRatio * m_fZoomFactor;	
		float yScale = m_fHeightRatio * m_fZoomFactor;

		GUIVERTEX data[6];
		GetQuad(data, (float)m_iPosX, (float)m_iPosY, (float)m_iSizeX, (float)m_iSizeY, m_iBGColor);		

		UINT numVerts = 6;
		memcpy( (*pDest), data, numVerts*24 );	
		vertsWritten += numVerts;

			// advance the pointer
		(*pDest) = (void*)((char*)(*pDest) + (numVerts*24));					

		return vertsWritten;
	}
	return 0;
}

void CGUIBackground::GetQuad(GUIVERTEX data[], float x, float y, float width, float height, DWORD color)
{
	// NOTE: winding is now cw
	data[0].pos[0] = x;
	data[0].pos[1] = y;
	data[0].pos[2] = 0.0f;
	data[0].u = 0.0f;
	data[0].v = 0.0f;
	data[0].color = color;

	data[1].pos[0] = x + width;
	data[1].pos[1] = y;
	data[1].pos[2] = 0.0f;
	data[1].u = 1.0f;
	data[1].v = 0.0f;
	data[1].color = color;

	data[2].pos[0] = x;
	data[2].pos[1] = y + height;
	data[2].pos[2] = 0.0f;
	data[2].u = 0.0f;
	data[2].v = 1.0f;
	data[2].color = color;

	data[3].pos[0] = x + width;
	data[3].pos[1] = y;
	data[3].pos[2] = 0.0f;
	data[3].u = 1.0f;
	data[3].v = 0.0f;
	data[3].color = color;

	data[4].pos[0] = x + width;
	data[4].pos[1] = y + height;
	data[4].pos[2] = 0.0f;
	data[4].u = 1.0f;
	data[4].v = 1.0f;
	data[4].color = color;

	data[5].pos[0] = x;
	data[5].pos[1] = y + height;
	data[5].pos[2] = 0.0f;
	data[5].u = 0.0f;
	data[5].v = 1.0f;
	data[5].color = color;
}

// draw gui element w/ greates zorder first
bool CGUIBackground::IsLessThan(const IRenderObject *obj, DWORD sortMethod, int curRenderPass)
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

Vec3 &CGUIBackground::GetPosition()
{
	static Vec3 tempVec(0,0,0);
	return tempVec;
}

void CGUIBackground::GetBoundingSphere( Vec3 &position, float &radius )
{
	position.SetX(0);
	position.SetY(0);
	position.SetZ(0);
	radius = 0.0f;

}

//------------Message Handlers-----------------//
DWORD CGUIBackground::OnSetPosition(DWORD msgSize, void *param)
{
	POSITIONDATA *pd;
	VERIFY_MESSAGE_SIZE(msgSize, sizeof(POSITIONDATA));
	pd = (POSITIONDATA *)param;
	
	// we only need the first two
	m_iPosX = (int)pd->m_fXPos;
	m_iPosY = (int)pd->m_fYPos;

	return MSG_HANDLED_STOP;	
}

DWORD CGUIBackground::OnGetPosition(DWORD msgSize, void *param)
{
	POSITIONDATA *pd;
	VERIFY_MESSAGE_SIZE(msgSize, sizeof(POSITIONDATA));
	pd = (POSITIONDATA *)param;
	
	// we only need the first two
	pd->m_fXPos = (float)m_iPosX;
	pd->m_fYPos = (float)m_iPosY;

	return MSG_HANDLED_STOP;	
}

DWORD CGUIBackground::OnSetSize(DWORD msgSize, void *param)
{
	SIZEDATA *pd;
	VERIFY_MESSAGE_SIZE(msgSize, sizeof(SIZEDATA));
	pd = (SIZEDATA *)param;

	// we only need the first two
	m_iSizeX = (int)pd->m_fXSize;
	m_iSizeY = (int)pd->m_fYSize;

	return MSG_HANDLED_STOP;	
}

// get the size of the element
DWORD CGUIBackground::OnGetSize(DWORD msgSize, void *param)
{
	SIZEDATA *pd;
	VERIFY_MESSAGE_SIZE(msgSize, sizeof(SIZEDATA));
	pd = (SIZEDATA *)param;

	// we only need the first two
	pd->m_fXSize = (float)m_iSizeX;
	pd->m_fYSize = (float)m_iSizeY;

	return MSG_HANDLED_STOP;	
}

DWORD CGUIBackground::OnSetRect(DWORD msgSize, void *param)
{
	SELECTRECT *selRect;
	VERIFY_MESSAGE_SIZE(msgSize, sizeof(SELECTRECT));
	selRect = (SELECTRECT*)param;

	m_iRectX1 = (int)selRect->x1;
	m_iRectX2 = (int)selRect->x2;
	m_iRectY1 = (int)selRect->y1;
	m_iRectY2 = (int)selRect->y2;

	return MSG_HANDLED_STOP;
}

IHashString* CGUIBackground::GetBaseSortClass()
{
	static CHashString sortClass(_T("IGUIElement"));
	return &sortClass;
}

DWORD CGUIBackground::OnMousePosition(DWORD msgSize, void *param)
{
	return MSG_HANDLED_PROCEED;
}

DWORD CGUIBackground::OnLeftMousePress(DWORD msgSize, void *param)
{
	return MSG_HANDLED_PROCEED;
}

DWORD CGUIBackground::OnLeftMouseRelease(DWORD msgSize, void *param)
{
	return MSG_HANDLED_PROCEED;
}

DWORD CGUIBackground::OnKeyPress(DWORD msgSize, void *param)
{
	return MSG_HANDLED_PROCEED;
}

DWORD CGUIBackground::OnSetColor(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(DWORD));
	DWORD col = *(DWORD*)param;
	m_iBGColor = col;
	return MSG_HANDLED_STOP;
}

