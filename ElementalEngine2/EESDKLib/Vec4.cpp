///==========================================================================
/// \file	Vec4.cpp
/// \brief	
/// \date	08/10/2005
/// \author	Brian Bazyk
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
#include "Vec4.h"

IVec4Implementation *Vec4::impl = NULL;

namespace ElementalEngine
{

void* GetActiveMathImplementation( DWORD dwImplementationName );

int SetVec4Implementation()
{
	Vec4::impl = (IVec4Implementation*)GetActiveMathImplementation(CHashString(_T("Vec4DefaultImplementation")).GetUniqueID());
//	EngineGetToolBox()->Log( LOG_SEVERE, _T("Math implementation for Vec4 not found!\n") );
	return true;
}

} //namespace ElementalEngine

