///============================================================================
/// \file		GDALIO.h
/// \brief		Header file for importing GeoSpatial Data through GDAL
/// \date		10-30-2005
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

#ifndef _GDALIO_H
#define _GDALIO_H

class CGDALIO
{
public:
	CGDALIO(void);
	~CGDALIO(void);

	/// Import data from a GeoSpatial data file
	/// Opens up a file dialog, then imports terrain
	/// sectors into Elemental Engine
	/// \returns bool, false if nothing done or error, true otherwise
	bool Import(TERRAINCREATIONPARAMS *tcParms);
};

#endif	//#ifndef _GDALIO_H
