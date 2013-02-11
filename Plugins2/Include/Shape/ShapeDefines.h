///============================================================================
/// \file		ShapeDefines.h
/// \brief		Header file for Shape Definitions
/// \date		11-13-2006
/// \author		Kyle Swaim
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

#ifndef	SHAPEDEFINES_H
#define	SHAPEDEFINES_H

struct ADDAXISALIGNEDBOXPARAMS
{
	ADDAXISALIGNEDBOXPARAMS()
	{
		name = NULL;
		red = 255;
		green = 255;
		blue = 255;
		alpha = 200;
	}

	IHashString *name;
	Vec3 min;
	Vec3 max;
	int red;
	int green;
	int blue;
	int alpha;
	Vec3 offset;
};

struct ADDOBJECTORIENTEDBOXPARAMS
{
	ADDOBJECTORIENTEDBOXPARAMS()
	{
		name = NULL;
		red = 255;
		green = 255;
		blue = 255;
	}

	IHashString *name;
	Vec3 min;
	Vec3 max;
	int red;
	int green;
	int blue;
	Matrix4x4 localTransform;
};

struct ADDSPHEREPARAMS
{
	ADDSPHEREPARAMS()
	{
		name = NULL;
		radius = 1.0f;
		red = 255;
		green = 255;
		blue = 255;
		resolution = 4;
	}

	IHashString *name;
	float radius;
	int red;
	int green;
	int blue;
	int resolution;
	Vec3 offset;
};

struct ADDLINEPARAMS
{
	ADDLINEPARAMS()
	{
		name = NULL;
		red = 255;
		green = 255;
		blue = 255;
		width = 1.0f;
	}

	IHashString *name;
	Vec3 start;
	Vec3 end;
	int red;
	int blue;
	int green;
	float width;
	Vec3 offset;
};

struct ADDPOINTPARAMS
{
	ADDPOINTPARAMS()
	{
		name = NULL;
		red = 255;
		green = 255;
		blue = 255;
		size = 1;
	}

	IHashString *name;
	int red;
	int blue;
	int green;
	float size;
	Vec3 offset;
};

struct REMOVEAXISALIGNEDBOXPARAMS
{
	IHashString *name;
};

struct REMOVEOBJECTORIENTEDBOXPARAMS
{
	IHashString *name;
};

struct REMOVESPHEREPARAMS
{
	IHashString *name;
};

struct REMOVELINEPARAMS
{
	IHashString *name;
};

struct REMOVEPOINTPARAMS
{
	IHashString *name;
};

#endif // SHAPEDEFINES_H
