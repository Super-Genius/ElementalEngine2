///============================================================================
/// \file		ObjectCubeMapDefines.h
/// \brief		Header file for ObjectCubeMap plugin defines
/// \date		1-23-2007
/// \author		Marvin Gouw
/// \note		Copyright (c) 2006 Signature Devices, Inc.
///============================================================================

#ifndef _OBJECTCUBEMAPDEFINE_H_
#define _OBJECTCUBEMAPDEFINE_H_

// tolua_begin
namespace ElementalEngine
{
class Vec3;
class IHashString;
}

enum CUBEDIRECTION
{
	CUBEMAP_POS_X = 0,
	CUBEMAP_NEG_X = 1,
	CUBEMAP_POS_Y = 2,
	CUBEMAP_NEG_Y = 3,
	CUBEMAP_POS_Z = 4,
	CUBEMAP_NEG_Z = 5,
	CUBEMAP_TOTAL_DIR = 6
};

struct GENERATECUBEMAPMESSAGE
{
	IRenderObject * RenderObject;
	IHashString * filename;
	IObject * CubeMapObject;
	GENERATECUBEMAPMESSAGE()
	{
		RenderObject = NULL;
		filename = NULL;
		CubeMapObject = NULL;
	}
};

struct SETOBJECTCUBEMAPMSG
{
	void AddFace_NegX( StdString name ){ CubeNames[ CUBEMAP_NEG_X ] = name; }
	void AddFace_PosX( StdString name ){ CubeNames[ CUBEMAP_POS_X ] = name; }
	void AddFace_NegY( StdString name ){ CubeNames[ CUBEMAP_NEG_Y ] = name; }
	void AddFace_PosY( StdString name ){ CubeNames[ CUBEMAP_POS_Y ] = name; }
	void AddFace_NegZ( StdString name ){ CubeNames[ CUBEMAP_NEG_Z ] = name; }
	void AddFace_PosZ( StdString name ){ CubeNames[ CUBEMAP_POS_Z ] = name; }

	StdString CubemapTypename;
	StdString CubeNames[ 6 ];
};

struct ADDCUBEMAPMESSAGE
{
	IObject * CubeMapObject;
	ADDCUBEMAPMESSAGE()
	{
		CubeMapObject =NULL;
	}
};
// tolua_end

#endif // #ifndef _OBJECTCUBEMAPDEFINE_H_
