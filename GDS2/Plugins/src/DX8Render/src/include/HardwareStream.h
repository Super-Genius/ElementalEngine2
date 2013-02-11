//---------------------------------------------------------------------
// File:		HardwareStream.h
// Description:	string HardwareStreamuration class
// Created:		5/27/02
// Author:		Kenneth L. Hurley
//---------------------------------------------------------------------

#ifndef HARDWARESTREAM_H
#define HARDWARESTREAM_H

#ifndef BASEOBJECT_H
#include "BaseObject.h"
#endif

enum HS_TYPE
{
	HS_VERTEXTYPE,
	HS_INDEXTYPE
};

class CHardwareStream
{
	// attributes
private:

public:
	virtual void *LockHardwareStream() = 0;
	virtual BOOL UnlockHardwareStream() = 0;

};

// {E942E72F-26AA-4a42-8EC8-B1328E086B51}
static const GUID GUID_HARDVS = 
{ 0xe942e72f, 0x26aa, 0x4a42, { 0x8e, 0xc8, 0xb1, 0x32, 0x8e, 0x8, 0x6b, 0x51 } };

class CHardwareVertexStream : public CHardwareStream, public CBaseObject
{
private:
	HANDLE m_hVertexStream;
public:
	CHardwareVertexStream() : CHardwareStream(), CBaseObject(&GUID_HARDVS) {};
	CHardwareVertexStream(LPCSTR name) : CHardwareStream(),
			CBaseObject(&GUID_HARDVS, name) {};
	virtual void *LockHardwareStream();
	virtual BOOL UnlockHardwareStream();
};

// {B247A198-D713-4186-AF55-710E79E8EC87}
static const GUID GUID_HARDIS = 
{ 0xb247a198, 0xd713, 0x4186, { 0xaf, 0x55, 0x71, 0xe, 0x79, 0xe8, 0xec, 0x87 } };

class CHardwareIndexStream : public CHardwareStream, public CBaseObject
{
private:
	HANDLE m_hIndexStream;
public:
	CHardwareIndexStream() : CHardwareStream(), CBaseObject(&GUID_HARDVS) {};
	CHardwareIndexStream(LPCSTR name) : CHardwareStream(),
			CBaseObject(&GUID_HARDVS, name) {};
	virtual void *LockHardwareStream();
	virtual BOOL UnlockHardwareStream();
};

#endif	// HARDWARESTREAM_H