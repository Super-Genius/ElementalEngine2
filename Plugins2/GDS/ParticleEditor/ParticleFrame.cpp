///============================================================================
/// \file       ParticleFrame.cpp
/// \brief      Implementation file for GDS Particle Editor plugin Frame
/// \date       09-13-2006
/// \author     Brendan Geiger
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
#include "ParticleFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CParticleFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CParticleFrame, CMDIChildWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()


CParticleFrame::CParticleFrame()
{
}

CParticleFrame::~CParticleFrame()
{
}

int CParticleFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	SINGLETONINSTANCE(CParticleEditor)->ShowTree();
	SINGLETONINSTANCE(CParticleEditor)->ShowProperties();
	SINGLETONINSTANCE(CParticleEditor)->ShowGraph();
	
	return 0;
}

BOOL CParticleFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	return CMDIChildWnd::OnCreateClient(lpcs, pContext);
	return TRUE;
}
