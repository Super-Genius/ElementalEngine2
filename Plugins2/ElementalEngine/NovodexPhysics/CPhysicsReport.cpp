///============================================================================
/// \file	CPhysicsReport.cpp
/// \brief	Callback report general physics events
/// \date	6-18-2007
/// \author	Kyle Swaim
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

CPhysicsReport::CPhysicsReport()
{
	m_ToolBox = EngineGetToolBox();
}

CPhysicsReport::~CPhysicsReport()
{
}

bool CPhysicsReport::onJointBreak( NxReal breakingImpulse, NxJoint &brokenJoint )
{
   	return false; // true to delete joint - true crashes it...
}