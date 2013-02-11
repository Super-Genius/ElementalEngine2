///============================================================================
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

IFontImplementation * CDX9FontImplementation::s_instance = NULL;

IFontObject * CDX9FontImplementation::CreateFont()
{
	IFontObject * obj = NULL;
	try
	{
		obj = new EE_CDX9Font();
	}
	catch( ... )
	{
		EngineGetToolBox()->Log( LOGERROR, _T("Out of memory, could not create font object\n"));
		return NULL;
	}
	return obj;
}

void CDX9FontImplementation::DestroyFont( IFontObject * obj )
{
	if( obj )
	{
		delete obj;
	}
}

int CDX9FontImplementation::GetFontPixWidth(IHashString *fontFace, UINT fontsize, const TCHAR* text)
{
	return EE_CDX9Font::GetFontPixWidth(CHashString(fontFace), fontsize, text);
}

IFontImplementation * CDX9FontImplementation::Instance()
{
	if ( s_instance == NULL )
	{
		s_instance = (IFontImplementation*)new CDX9FontImplementation();
	}
	return s_instance;
}

void CDX9FontImplementation::Destroy()
{
	if ( s_instance != NULL )
	{
		delete s_instance;
	}
	s_instance = NULL;
}