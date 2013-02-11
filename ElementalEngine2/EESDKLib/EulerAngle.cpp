///==========================================================================
/// \file	EulerAngle.cpp
/// \brief	
/// \date	06/16/2007
/// \author	Marvin Gouw
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
///==========================================================================

#include "stdafx.h"
#include "EulerAngle.h"

IEulerImplementation *EulerAngle::impl = NULL;

namespace ElementalEngine
{

void* GetActiveMathImplementation( DWORD dwImplementationName );

int SetEulerImplementation()
{
	EulerAngle::impl = (IEulerImplementation*)GetActiveMathImplementation(CHashString(_T("EulerDefaultImplementation")).GetUniqueID());
	return true;
}

} //namespace ElementalEngine

