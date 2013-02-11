//---------------------------------------------------------------------
// File:		DX8Mesh.h
// Description:	Base DX8Mesh class
// Created:		12/01/02
// Author:		Kenneth L. Hurley
//---------------------------------------------------------------------

#ifndef DX8MESH_H
#define DX8MESH_H

#ifndef BASEOBJECT_H
#include "BaseObject.h"
#endif

#ifndef HARDWARESTREAM_H
#include "HardwareStream.h"
#endif

#ifndef MESH_H
#include "mesh.h"
#endif

#pragma warning(disable : 4786)
#include <vector>

class CDX8Mesh : public CMesh
{
public:												

	// attributes
private:

	// operations
public:
    CDX8Mesh(LPCSTR name = NULL, BOOL isStatic = TRUE) : CMesh(name, isStatic)
    {
    }

	// this is to initialize with data from a pre-exisiting CMesh
	CDX8Mesh(CMesh *src, LPCSTR name = NULL, BOOL isStatic = TRUE) : CMesh(name, isStatic)
	{
		*((CMesh *)this) = *src;
	}

	virtual bool InterleaveStreams(DWORD count, DWORD *streams);
    virtual void *LockHardwareStream(DWORD index);
    virtual BOOL UnlockHardwareStream(DWORD index);
    virtual CHardwareStream *GetHWDataStream(DWORD index);
};


#endif	// #ifndef DX8MESH_H
