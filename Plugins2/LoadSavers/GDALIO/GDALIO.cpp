///============================================================================
/// \file		GDALIO.cpp
/// \brief		Implementation file for GDAL file Loader
/// \date		10-22-2005
/// \author		Kenneth Hurley
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
#include "GDALIO.hpp"
#include "BaseMessages.h"

REGISTER_COMPONENT_SINGLETON(CGDALIO);
REGISTER_MESSAGE_HANDLER(LoadFile, OnLoadGDALFile, CGDALIO);
REGISTER_MESSAGE_HANDLER(SaveFile, OnSaveGDALFile, CGDALIO);

CHashString CGDALIO::m_HashName(_T("CGDALIO"));

LSRC_MESSAGE CGDALIO::m_LoadSaveRegisterData = {
		&CGDALIO::m_HashName,
		_T("Arc/Info Ascii Grid Files (*.grd)\0" "*.grd\0")
		_T("Arc/Info Binary Grid Files (*.adf)\0" "*.adf\0")
		_T("BSB Nautical Chart Format (*.kap)\0" "*.kap\0")
		_T("Microsoft Windows Device Independent Bitmap (*.bmp) \0" "*.bmp\0")
		_T("VTP Binary Terrain Format (*.bt)\0" "*.bt\0")
		_T("First Generation USGS DOQ (*.doq)\0" "*.doq\0")
		_T("Military Elevation Data (*.dt?)\0" "*.dt?\0")
		_T("ERMapper Compressed Wavelets (*.ecw)\0" "*.ecw\0")
		_T("ENVI Labelled Raster (*.hdr)\0" "*.hdr\0")
		_T("Envisat Image Product (*.n1)\0" "*.n1\0")
		_T("FITS (*.fits)\0" "*.fits\0")
		_T("Graphics Interchange Format (*.gif)\0" "*.gif\0") 
		_T("TIFF / GeoTIFF (*.tif)\0" "*.tif\0")
//		_T("JPEG JFIF (*.jpg)\0" "*.jpg;\0")
//		_T("JPEG2000 (*.jp2, *.j2k)\0" "*.jpg; *.j2k\0")
		_T("USGS ASCII DEM (*.dem)\0" "*.dem\0")
		_T("Erdas Imagine (*.img)\0" "*.img\0")
		_T("Raster Matrix Format (*.rsw, *.mtw)\0" "*.rsw;*.mtw\0")
		_T("USGS SDTS DEM (*catd.ddf)\0" "*catd.ddf\0")
		_T("X11 Pixmap (.xpm)\0" "*.xpm\0")
		_T("\0\0"),
		_T("Arc/Info Ascii Grid Files (*.grd)\0" "*.grd\0")
		_T("Arc/Info Binary Grid Files (*.adf)\0" "*.adf\0")
		_T("Microsoft Windows Device Independent Bitmap (*.bmp) \0" "*.bmp\0")
		_T("VTP Binary Terrain Format (*.bt)\0" "*.bt\0")
		_T("ERMapper Compressed Wavelets (*.ecw)\0" "*.ecw\0")
		_T("ENVI Labelled Raster (*.hdr)\0" "*.hdr\0")
		_T("FITS (*.fits)\0" "*.fits\0")
		_T("Graphics Interchange Format (*.gif)\0" "*.gif\0") 
		_T("TIFF / GeoTIFF (*.tif)\0" "*.tif\0")
//		_T("JPEG JFIF (*.jpg)\0" "*.jpg;\0")
//		_T("JPEG2000 (*.jp2, *.j2k)\0" "*.jpg; *.j2k\0") 
		_T("Erdas Imagine (*.img)\0" "*.img\0")
		_T("Raster Matrix Format (*.rsw, *.mtw)\0" "*.rsw;*.mtw\0")
		_T("X11 Pixmap (.xpm)\0" "*.xpm\0")
		_T("\0\0"),
		false, NULL,
};

CGDALIO::CGDALIO() 
{
	// save for convenience
	m_ToolBox = EngineGetToolBox();

	static DWORD msgHash_RegisterLoadSaveComponent = CHashString(_T("RegisterLoadSaveComponent")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_RegisterLoadSaveComponent, sizeof(LSRC_MESSAGE),&m_LoadSaveRegisterData) != MSG_HANDLED)
	{
		// log error RegisterLoadSaveComponent failed!
	}

}

// create function for this world loader
IComponent *CGDALIO::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE( CGDALIO );
}

IHashString *CGDALIO::GetComponentType()
{
	return &m_HashName;
}

bool CGDALIO::IsKindOf(IHashString *compType)
{
	return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
}

// load a file if we recognize the type.
DWORD CGDALIO::OnLoadGDALFile(DWORD size, void *params)
{
	return MSG_HANDLED_PROCEED;
}

// Save a file if we recognize the type.
DWORD CGDALIO::OnSaveGDALFile(DWORD size, void *params)
{
	return MSG_HANDLED_PROCEED;
}

