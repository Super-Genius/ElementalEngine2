///============================================================================
/// \file		C2DLine.cpp
/// \brief		Implementation file for CVis2DLine - Vision 2D Line Adapter
/// \date		04-04-2005
/// \author		Josh Lulewicz
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
#include "C2DLine.h"

REGISTER_COMPONENT(C2DLine);

C2DLine::C2DLine(IHashString *parentName, IHashString *name) 
	: SCENEOBJECTTEMPLATE( CRenderManager, C2DLine, IRenderObject, parentName, name )
{
	// grab the render interface
	GETRENDERERINTERFACEPARAMS renderInterface;

	static DWORD msgHash_GetRendererInterface = CHashString(_T("GetRendererInterface")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetRendererInterface, sizeof(GETRENDERERINTERFACEPARAMS), &renderInterface);

	m_Renderer = renderInterface.m_RendererInterface;
}

C2DLine::~C2DLine()
{
}

/// \brief The Initialization for the Object
/// \return pointer to an IComponent
IComponent *C2DLine::Create(int nArgs, va_list argptr)
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
		self = new C2DLine(parentName, name);
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

void C2DLine::Serialize(IArchive &ar)
{
	if(ar.IsReading())
	{
		ar.Read(m_fStartX);
		ar.Read(m_fStartY);
		ar.Read(m_fEndX);
		ar.Read(m_fEndY);
		ar.Read(m_iRed);
		ar.Read(m_iGreen);
		ar.Read(m_iBlue);
		ar.Read(m_fWidth);
	}
}

bool C2DLine::Render(UINT pass, IEffect * override )
{
	m_Renderer->Draw2DLine(m_fStartX, 
						   m_fStartY, 
						   m_fEndX, 
						   m_fEndY, 
						   m_iRed, 
						   m_iGreen, 
						   m_iBlue, 
						   m_fWidth);

	return true;
}

bool C2DLine::Build2DLine(float startX,
							 float startY,
							 float endX,
							 float endY,
							 int r, int g, int b,
							 float width)
{
	// build the line
	m_fStartX = startX;
	m_fStartY = startY;
	m_fEndX = endX;
	m_fEndY = endY;
	m_iRed = r;
	m_iGreen = g;
	m_iBlue = b;
	m_fWidth = width;

	return true;
}
