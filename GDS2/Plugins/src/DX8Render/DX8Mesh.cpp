//---------------------------------------------------------------------
// File:		DX8Mesh.cpp
// Description:	Base DX8Mesh class
// Created:		12/01/02
// Author:		Kenneth L. Hurley
//---------------------------------------------------------------------

#include "StdAfx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//---------------------------------------------------------------------
// Function:	InterleaveStreams
// Description:	Interleaves streams by rearranging pointer order
//					This is useful when you want to go to hardware
// Parameters:	count = number of streams to re-arrange
//				stream order to rearrange in
// Returns:		TRUE, if successful
//---------------------------------------------------------------------
bool CDX8Mesh::InterleaveStreams(DWORD count, DWORD *streams)
{ 
	return TRUE; 
};

//---------------------------------------------------------------------
// Function:	LockHardwareStream
// Description:	Locks a hardware stream to fill the buffer
// Parameters:	index = which stream to lock
// Returns:		void *pointer to data that was locked
//---------------------------------------------------------------------
void *CDX8Mesh::LockHardwareStream(DWORD index)
{
	return NULL;
}

//---------------------------------------------------------------------
// Function:	UnlockHardwareStream
// Description:	Unlocks the hardware stream after filling data
// Parameters:	index = hardware stream that was locked index
// Returns:		TRUE, if success
//---------------------------------------------------------------------
BOOL CDX8Mesh::UnlockHardwareStream(DWORD index)
{
	return TRUE;
}

//---------------------------------------------------------------------
// Function:	GetHWDataStream
// Description:	Returns a CHardwareStream pointer based on index
// Parameters:	index = index of stream requested
// Returns:		CHardwareStream class pointer
//---------------------------------------------------------------------
CHardwareStream *CDX8Mesh::GetHWDataStream(DWORD index)
{
	return NULL;
}


