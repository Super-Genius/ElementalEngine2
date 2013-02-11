//---------------------------------------------------------------------
// File:		Mesh.h
// Description:	Base Mesh class
// Created:		6/27/02
// Author:		Kenneth L. Hurley
//---------------------------------------------------------------------

#ifndef MESH_H
#define MESH_H

#ifndef BASEOBJECT_H
#include "BaseObject.h"
#endif

#ifndef HARDWARESTREAM_H
#include "HardwareStream.h"
#endif

#pragma warning(disable : 4786)
#include <vector>

//
// format for the stream
enum FORMATFLAGS
{
    STREAM_BYTE,
    STREAM_WORD,
    STREAM_DWORD,
    STREAM_FLOAT
};

typedef LPCSTR DESCRIPTIONS;

typedef struct DATASTREAM
{
    FORMATFLAGS *m_type;
    DWORD m_Count;
    void *m_Data;
    DESCRIPTIONS *m_Description;
} DATASTREAM;

// {3190234D-3E14-4bd1-BA63-0D9647C1D6E7}
static const GUID GUID_MESH = 
{ 0x3190234d, 0x3e14, 0x4bd1, { 0xba, 0x63, 0xd, 0x96, 0x47, 0xc1, 0xd6, 0xe7 } };

class CMesh : public CBaseObject
{
public:												
	DECLARE_OVERRIDEABLE(CMesh);

	// attributes
private:
    vector<DATASTREAM *> m_DataStreams;
    BOOL m_IsStatic;
    vector<CHardwareStream *> m_HardwareStreams;

	// operations
public:
    CMesh(LPCSTR name = NULL, BOOL isStatic = TRUE) : CBaseObject(&GUID_MESH, name)
    {
        m_IsStatic = isStatic;
    }

	//---------------------------------------------------------------------
	// Function:	GetDataStream
	// Description:	Returns a Datastream from class
	// Parameters:	index = index number of datastream
	// Returns:		pointer to DATASTREAM structure
	//---------------------------------------------------------------------
	DATASTREAM *GetDataStream(DWORD index)
	{
		return  m_DataStreams[index];
	}

	//---------------------------------------------------------------------
	// Function:	GetDataStreamCount
	// Description:	Get the number of streams associated with this mesh
	// Parameters:	.
	// Returns:		DWORD = number of streams in mesh
	//---------------------------------------------------------------------
	DWORD GetDataStreamCount()
	{
		return m_DataStreams.size();
	}

	//---------------------------------------------------------------------
	// Function:	InterleaveStreams
	// Description:	Interleaves streams by rearranging pointer order
	//					This is useful when you want to go to hardware
	// Parameters:	count = number of streams to re-arrange
	//				stream order to rearrange in
	// Returns:		TRUE, if successful
	//---------------------------------------------------------------------
	virtual bool CMesh::InterleaveStreams(DWORD count, DWORD *streams)
	{ 
		return TRUE; 
	};

	//---------------------------------------------------------------------
	// Function:	LockHardwareStream
	// Description:	Locks a hardware stream to fill the buffer
	// Parameters:	index = which stream to lock
	// Returns:		void *pointer to data that was locked
	//---------------------------------------------------------------------
	virtual void *LockHardwareStream(DWORD index)
	{
		return m_DataStreams[index]->m_Data;
	}

	//---------------------------------------------------------------------
	// Function:	UnlockHardwareStream
	// Description:	Unlocks the hardware stream after filling data
	// Parameters:	index = hardware stream that was locked index
	// Returns:		TRUE, if success
	//---------------------------------------------------------------------
	virtual BOOL UnlockHardwareStream(DWORD index)
	{
		return TRUE;
	}

	//---------------------------------------------------------------------
	// Function:	GetHWDataStream
	// Description:	Returns a CHardwareStream pointer based on index
	// Parameters:	index = index of stream requested
	// Returns:		CHardwareStream class pointer
	//---------------------------------------------------------------------
	virtual CHardwareStream *GetHWDataStream(DWORD index)
	{
		return NULL;
	}

};

#endif	// #ifndef MESH_H
