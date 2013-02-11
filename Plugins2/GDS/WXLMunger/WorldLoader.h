///============================================================================
/// \file		CWorldLoader.h
/// \brief		Declaration of visitor pattern to load a WXL into a heirarchy
/// \date		08-09-2005
/// \author		Patrick Ghiocel
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
#ifndef _CWORLDLOADER_H_
#define _CWORLDLOADER_H_

#include "Main.h"

class CWorldLoader
{
public:
	CWorldLoader();
	
    bool LoadWorld( StdString wszFilename );
	bool SaveWorld( StdString wszFilename );
	
private:
	IToolBox* m_ToolBox;

};
	
#endif //_CWORLDLOADER_H_