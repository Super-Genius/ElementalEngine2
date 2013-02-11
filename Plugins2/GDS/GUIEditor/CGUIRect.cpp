///============================================================================
/// \file		CGUIRect.h
/// \brief		Header file for a 2D rectangle gui element
///				Used to draw a rectangle around the selectable area of a gui element
/// \date		07-18-2005
/// \author		Halbert Nakagawa
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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT(CGUIRect);


REGISTER_MESSAGE_HANDLER(SetPosition, OnSetPosition, CGUIRect);
REGISTER_MESSAGE_HANDLER(SetSize, OnSetSize, CGUIRect);
REGISTER_MESSAGE_HANDLER(GetPosition, OnGetPosition, CGUIRect);
REGISTER_MESSAGE_HANDLER(GetSize, OnGetSize, CGUIRect);
REGISTER_MESSAGE_HANDLER(SetRect, OnSetRect, CGUIRect);
REGISTER_MESSAGE_HANDLER(GetHandleIndex, OnGetHandleIndex, CGUIRect);
REGISTER_MESSAGE_HANDLER(FollowObject, OnFollowObject, CGUIRect);

CGUIRect::CGUIRect(IHashString *parentName, IHashString *name) : 
	SCENEOBJECTTEMPLATE(CGUIManager, CGUIRect, IGUIElement, parentName, name)
{
	// grab the render interface
	GETRENDERERINTERFACEPARAMS renderInterface;

	static DWORD msgHash_GetRendererInterface = CHashString(_T("GetRendererInterface")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetRendererInterface, sizeof(GETRENDERERINTERFACEPARAMS), &renderInterface);

	m_Renderer = renderInterface.m_RendererInterface;

	m_iZOrder = -100;

	m_iPosX  = m_iPosY = 0;
	m_iSizeX = m_iSizeY = 0;
	m_iRectX1 = m_iRectX2 = m_iRectY1 = m_iRectY2 = 0;

	m_fWidthRatio = 1.f;
	m_fHeightRatio = 1.f;
	m_fZoomFactor = 1.f;
	m_iPageOffsetX = m_iPageOffsetY = 0;

	DWORD res;
	StdString rectFile, workingDir, resourceDir;
    EngineGetToolBox()->GetDirectories(&workingDir, &resourceDir);
	rectFile = workingDir;
	rectFile += _T("\\basedata\\editor\\textures\\rectwidget.tga");

	CHashString outlineFile = rectFile;
	TEXTUREOBJECTPARAMS tobj;
	tobj.bLoad = true;
	tobj.Name = &outlineFile;
	tobj.TextureObjectInterface = NULL;
	static DWORD msgHash_AddTexture = CHashString(_T("AddTexture")).GetUniqueID();
	res = EngineGetToolBox()->SendMessage(msgHash_AddTexture, sizeof(tobj), &tobj );
	if (res == MSG_HANDLED)
	{
		m_pRectOutline = dynamic_cast<ITextureObject*>(tobj.TextureObjectInterface);
	}
	else
	{
		m_pRectOutline = NULL;
	}

	m_VertexBuffer = NULL;
	m_VBOffset = 0;
}

// create function for this world loader
IComponent *CGUIRect::Create(int nArgs, va_list argptr)
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
		self = new CGUIRect(parentName, name);
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

bool CGUIRect::Update()
{
	return true;
}

IHashString* CGUIRect::GetComponentType()
{
	static CHashString CGUIRectCT(_T("CGUIRect"));
	return &CGUIRectCT;
}

bool CGUIRect::IsCursorOver(int iCursorPosX, int iCursorPosY)
{
	return false;
}

void CGUIRect::Serialize(IArchive &ar)
{
// no serialization for this guy!	
}

// render mask
bool CGUIRect::Render(UINT pass, IEffect * override)
{
	float fWidth = 2.f;

	if(m_iRectX1 == 0 && m_iRectX2 == 0
		&& m_iRectY1 == 0 && m_iRectY2 == 0)
	{
		return true;
	}
	else
	{

		m_Renderer->SetTexture(0, m_pRectOutline);
		m_Renderer->SetEffect(0, NULL);

		UINT curOffset = m_VBOffset;
		for (UINT i=0; i<17; i++)
		{
			m_Renderer->RenderVertexBuffer(m_VertexBuffer, curOffset, 2);
			curOffset += 6;
		}
		return true;
	}
}

UINT CGUIRect::UpdateBuffer( void **pDest, UINT offset, IVertexBufferObject *vb)
{
	if ((vb) && (pDest) && (m_pRectOutline))
	{
		m_VertexBuffer = vb;
		m_VBOffset = offset;
		UINT vertsWritten = 0;	
		
		unsigned int tWidth, tHeight, midX, midY;
		tWidth = m_pRectOutline->GetWidth();
		tHeight = m_pRectOutline->GetHeight();
		midX = tWidth / 2;
		midY = tHeight / 2;
		float uOff = (float)(midX + 1) / (float) tWidth;
		float vOff = (float)(midY + 1) / (float) tHeight;
				
		float quadXPos[4], quadYPos[4];

		float xPos = (float)m_iRectX1;
		float yPos = (float)m_iRectY1;
		float width = (float)(m_iRectX2 - m_iRectX1);
		float height = (float)(m_iRectY2 - m_iRectY1);

		quadXPos[0] = xPos;
		quadXPos[1] = xPos + (float) midX;
		quadXPos[2] = (xPos + width) - (float) midX;
		quadXPos[3] = xPos + width;

		quadYPos[0] = yPos;
		quadYPos[1] = yPos + (float) midY;
		quadYPos[2] = (yPos + height) - (float) midY;
		quadYPos[3] = yPos + height;

		GUIVERTEX data[17][6];

		// main outline, smart scaled
		GetQuad(data[0], quadXPos[0], quadYPos[0], (quadXPos[1]-quadXPos[0]), (quadYPos[1]-quadYPos[0]), -1, 0.0f, 0.0f, 0.5f, 0.5f);
		GetQuad(data[1], quadXPos[1], quadYPos[0], (quadXPos[2]-quadXPos[1]), (quadYPos[1]-quadYPos[0]), -1, 0.5f, 0.0f, uOff, 0.5f);
		GetQuad(data[2], quadXPos[2], quadYPos[0], (quadXPos[3]-quadXPos[2]), (quadYPos[1]-quadYPos[0]), -1, uOff, 0.0f, 1.0, 0.5f);

		GetQuad(data[3], quadXPos[0], quadYPos[1], (quadXPos[1]-quadXPos[0]), (quadYPos[2]-quadYPos[1]), -1, 0.0f, 0.5f, 0.5f, vOff);
		GetQuad(data[4], quadXPos[1], quadYPos[1], (quadXPos[2]-quadXPos[1]), (quadYPos[2]-quadYPos[1]), -1, 0.5f, 0.5f, uOff, vOff);
		GetQuad(data[5], quadXPos[2], quadYPos[1], (quadXPos[3]-quadXPos[2]), (quadYPos[2]-quadYPos[1]), -1, uOff, 0.5f, 1.0, vOff);

		GetQuad(data[6], quadXPos[0], quadYPos[2], (quadXPos[1]-quadXPos[0]), (quadYPos[3]-quadYPos[2]), -1, 0.0f, vOff, 0.5f, 1.0f);
		GetQuad(data[7], quadXPos[1], quadYPos[2], (quadXPos[2]-quadXPos[1]), (quadYPos[3]-quadYPos[2]), -1, 0.5f, vOff, uOff, 1.0f);
		GetQuad(data[8], quadXPos[2], quadYPos[2], (quadXPos[3]-quadXPos[2]), (quadYPos[3]-quadYPos[2]), -1, uOff, vOff, 1.0, 1.0f);

		// handles
		GetQuad(data[9], xPos - 16.f, yPos - 16.f, 16.f, 16.f, -1);
		GetQuad(data[10], xPos+width, yPos - 16.f, 16.f, 16.f, -1);
		GetQuad(data[11], xPos+width, yPos+height, 16.f, 16.f, -1);
		GetQuad(data[12], xPos - 16.f, yPos+height, 16.f, 16.f, -1);
		GetQuad(data[13], xPos - 16.f, (yPos+(height / 2.f))-8.f, 16.f, 16.f, -1);
		GetQuad(data[14], xPos+width, (yPos+(height / 2.f))-8.f, 16.f, 16.f, -1);
        GetQuad(data[15], (xPos+(width/2.f))-8.f, yPos-16.f, 16.f, 16.f, -1);
		GetQuad(data[16], (xPos+(width/2.f))-8.f, yPos+height, 16.f, 16.f, -1);
		
		for (UINT i=0; i<17; i++)
		{
			UINT numVerts = 6;
			memcpy( (*pDest), data[i], numVerts*24 );	
			vertsWritten += numVerts;

				// advance the pointer
			(*pDest) = (void*)((char*)(*pDest) + (numVerts*24));					
		}

		return vertsWritten;
	}
	return 0;
}

void CGUIRect::GetQuad(GUIVERTEX data[], float x, float y, float width, float height, DWORD color, 
		float uStart, float vStart, float uEnd, float vEnd)
{
	// NOTE: winding is now cw
	data[0].pos[0] = x;
	data[0].pos[1] = y;
	data[0].pos[2] = 0.0f;
	data[0].u = uStart;
	data[0].v = vStart;
	data[0].color = color;

	data[1].pos[0] = x + width;
	data[1].pos[1] = y;
	data[1].pos[2] = 0.0f;
	data[1].u = uEnd;
	data[1].v = vStart;
	data[1].color = color;

	data[2].pos[0] = x;
	data[2].pos[1] = y + height;
	data[2].pos[2] = 0.0f;
	data[2].u = uStart;
	data[2].v = vEnd;
	data[2].color = color;

	data[3].pos[0] = x + width;
	data[3].pos[1] = y;
	data[3].pos[2] = 0.0f;
	data[3].u = uEnd;
	data[3].v = vStart;
	data[3].color = color;

	data[4].pos[0] = x + width;
	data[4].pos[1] = y + height;
	data[4].pos[2] = 0.0f;
	data[4].u = uEnd;
	data[4].v = vEnd;
	data[4].color = color;

	data[5].pos[0] = x;
	data[5].pos[1] = y + height;
	data[5].pos[2] = 0.0f;
	data[5].u = uStart;
	data[5].v = vEnd;
	data[5].color = color;
}


// draw gui element w/ greates zorder first
bool CGUIRect::IsLessThan(const IRenderObject *obj, DWORD sortMethod, int curRenderPass)
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

Vec3 &CGUIRect::GetPosition()
{
	static Vec3 tempVec(0,0,0);
	return tempVec;
}

void CGUIRect::GetBoundingSphere( Vec3 &position, float &radius )
{
	position.SetX(0);
	position.SetY(0);
	position.SetZ(0);
	radius = 0.0f;

}

//------------Message Handlers-----------------//
DWORD CGUIRect::OnSetPosition(DWORD msgSize, void *param)
{
	POSITIONDATA *pd;
	VERIFY_MESSAGE_SIZE(msgSize, sizeof(POSITIONDATA));
	pd = (POSITIONDATA *)param;
	
	// we only need the first two
	m_iPosX = (int)pd->m_fXPos;
	m_iPosY = (int)pd->m_fYPos;

	return MSG_HANDLED_STOP;	
}

DWORD CGUIRect::OnGetPosition(DWORD msgSize, void *param)
{
	POSITIONDATA *pd;
	VERIFY_MESSAGE_SIZE(msgSize, sizeof(POSITIONDATA));
	pd = (POSITIONDATA *)param;
	
	// we only need the first two
	pd->m_fXPos = (float)m_iPosX;
	pd->m_fYPos = (float)m_iPosY;

	return MSG_HANDLED_STOP;	
}

DWORD CGUIRect::OnSetSize(DWORD msgSize, void *param)
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
DWORD CGUIRect::OnGetSize(DWORD msgSize, void *param)
{
	SIZEDATA *pd;
	VERIFY_MESSAGE_SIZE(msgSize, sizeof(SIZEDATA));
	pd = (SIZEDATA *)param;

	// we only need the first two
	pd->m_fXSize = (float)m_iSizeX;
	pd->m_fYSize = (float)m_iSizeY;

	return MSG_HANDLED_STOP;	
}

DWORD CGUIRect::OnSetRect(DWORD msgSize, void *param)
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

IHashString* CGUIRect::GetBaseSortClass()
{
	static CHashString sortClass(_T("IGUIElement"));
	return &sortClass;
}

DWORD CGUIRect::OnMousePosition(DWORD msgSize, void *param)
{
	return MSG_HANDLED_PROCEED;
}

DWORD CGUIRect::OnLeftMousePress(DWORD msgSize, void *param)
{
	return MSG_HANDLED_PROCEED;
}

DWORD CGUIRect::OnLeftMouseRelease(DWORD msgSize, void *param)
{
	return MSG_HANDLED_PROCEED;
}

DWORD CGUIRect::OnKeyPress(DWORD msgSize, void *param)
{
	return MSG_HANDLED_PROCEED;
}

DWORD CGUIRect::OnGetHandleIndex(DWORD msgSize, void *param)
{
	VERIFY_MESSAGE_SIZE(msgSize, sizeof(int));
	int *result = (int*) param;
	*result = -1;
	if ((m_iRectX1 != m_iRectX2) && (m_iRectY1 != m_iRectY2))
	{
		int halfX = ((m_iRectX2 - m_iRectX1) / 2) + m_iRectX1;
		int halfY = ((m_iRectY2 - m_iRectY1) / 2) + m_iRectY1;
		MOUSECOORDS mouse;
		static DWORD msgHash_GetGUIMousePosition = CHashString(_T("GetGUIMousePosition")).GetUniqueID();
		EngineGetToolBox()->SendMessage(msgHash_GetGUIMousePosition, sizeof(MOUSECOORDS), &mouse, NULL, NULL);
		if ((mouse.m_lXPos >= m_iRectX1) && (mouse.m_lXPos <= m_iRectX2) &&
			(mouse.m_lYPos >= m_iRectY1) && (mouse.m_lYPos <= m_iRectY2))
		{
			*result = 0; // center of box
		}
		else if ((mouse.m_lXPos >= m_iRectX1-(int)(16.f/m_fZoomFactor)) && (mouse.m_lXPos <= m_iRectX1) &&
			(mouse.m_lYPos >= m_iRectY1-(int)(16.f/m_fZoomFactor)) && (mouse.m_lYPos <= m_iRectY1))
		{
			*result = 1; // top left handle
		}
		else if ((mouse.m_lXPos >= halfX-(int)(8.f/m_fZoomFactor)) && (mouse.m_lXPos <= halfX+(int)(8.f/m_fZoomFactor)) &&
			(mouse.m_lYPos >= m_iRectY1-(int)(16.f/m_fZoomFactor)) && (mouse.m_lYPos <= m_iRectY1))
		{
			*result = 2; // top middle handle
		}
		else if ((mouse.m_lXPos >= m_iRectX2) && (mouse.m_lXPos <= m_iRectX2+(int)(16.f/m_fZoomFactor)) &&
			(mouse.m_lYPos >= m_iRectY1-(int)(16.f/m_fZoomFactor)) && (mouse.m_lYPos <= m_iRectY1))
		{
			*result = 3; // top right handle
		}
		else if ((mouse.m_lXPos >= m_iRectX2) && (mouse.m_lXPos <= m_iRectX2+(int)(16.f/m_fZoomFactor)) &&
			(mouse.m_lYPos >= halfY-(int)(8.f/m_fZoomFactor)) && (mouse.m_lYPos <= halfY+(int)(8.f/m_fZoomFactor)))
		{
			*result = 4; // middle right handle
		}
		else if ((mouse.m_lXPos >= m_iRectX2) && (mouse.m_lXPos <= m_iRectX2+(int)(16.f/m_fZoomFactor)) &&
			(mouse.m_lYPos >= m_iRectY2) && (mouse.m_lYPos <= m_iRectY2+(int)(16.f/m_fZoomFactor)))
		{
			*result = 5; // bottom right handle
		}
		else if ((mouse.m_lXPos >= halfX-(int)(8.f/m_fZoomFactor)) && (mouse.m_lXPos <= halfX+(int)(8.f/m_fZoomFactor)) &&
			(mouse.m_lYPos >= m_iRectY2) && (mouse.m_lYPos <= m_iRectY2+(int)(16.f/m_fZoomFactor)))
		{
			*result = 6; // bottom middle handle
		}
		else if ((mouse.m_lXPos >= m_iRectX1-(int)(16.f/m_fZoomFactor)) && (mouse.m_lXPos <= m_iRectX1) &&
			(mouse.m_lYPos >= m_iRectY2) && (mouse.m_lYPos <= m_iRectY2+(int)(16.f/m_fZoomFactor)))
		{
			*result = 7; // bottom left handle
		}
		else if ((mouse.m_lXPos >= m_iRectX1-(int)(16.f/m_fZoomFactor)) && (mouse.m_lXPos <= m_iRectX1) &&
			(mouse.m_lYPos >= halfY-(int)(8.f/m_fZoomFactor)) && (mouse.m_lYPos <= halfY+(int)(8.f/m_fZoomFactor)))
		{
			*result = 8; // middle left handle
		}
	}
	return MSG_HANDLED_STOP;	
}

DWORD CGUIRect::OnFollowObject(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(FINDOBJNAME));
	POSITIONDATA pd;
	SIZEDATA sd;
	FINDOBJNAME fon = *(FINDOBJNAME*) param;
	static DWORD msgHash_GetPosition = CHashString(_T("GetPosition")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_GetPosition, sizeof(POSITIONDATA), &pd, fon.searchName, fon.compType);
	static DWORD msgHash_GetSize = CHashString(_T("GetSize")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_GetSize, sizeof(SIZEDATA), &sd, fon.searchName, fon.compType);
	m_iRectX1 = (int)pd.m_fXPos;
	m_iRectX2 = (int)(pd.m_fXPos + sd.m_fXSize);
	m_iRectY1 = (int)pd.m_fYPos;
	m_iRectY2 = (int)(pd.m_fYPos + sd.m_fYSize);
	return MSG_HANDLED_STOP;
}