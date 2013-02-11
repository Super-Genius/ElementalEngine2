//-----------------------------------------------------------------------------
//
// File:          CMesh.h
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Declaration of class 'CMesh'
//
// Modifications: @INSERT_MODIFICATIONS(* )
* November 26, 2003 14:11 Kenneth Hurley
*     Added method 'DestructorInclude'
*     Added method 'ConstructorInclude'
*     Added method 'UnlockHardwareStream'
*     Added method 'LockHardwareStream'
*     Added method 'GetHWDataStream'
*     Added method 'GetDataStreamCount'
*     Added method 'GetDataStream'
*     Added method 'CMesh::CMesh'
*     Added method '~CMesh'
*     Added method 'CMesh'
*     Added inheritance 'CBaseObject'
*     Added member 'm_HardwareStreams'
*     Added member 'm_IsStatic'
*     Added member 'm_DataStreams'
//
// Copyright 2003, Signature Devices, Inc.
// All rights are reserved. Reproduction in whole or part is prohibited
// without the written consent of the copyright owner.
//
//-----------------------------------------------------------------------------

#ifndef _CMESH_H
#define _CMESH_H

//@START_USER1
//@END_USER1


/*@NOTE_4394
C:\Development\Game Dev System\Plugins\src\include\Mesh.h(46)

 {3190234D-3E14-4bd1-BA63-0D9647C1D6E7}
*/

class CMesh
    : public CBaseObject
{

//@START_USER2
//@END_USER2

//
// Group: ClassBuilder methods
//

private:
    void ConstructorInclude();
    void DestructorInclude();

//
// Non-Grouped Members
//

private:
    * m_DataStreams;
    BOOL m_IsStatic;
    * m_HardwareStreams;

//
// Non-Grouped Methods
//

public:
    CMesh(LPCSTR name, BOOL isStatic);
    virtual ~CMesh();
    bool CMesh::CMesh(DWORD count, DWORD* streams);
    DATASTREAM* GetDataStream(DWORD index);
    DWORD GetDataStreamCount();
    CHardwareStream* GetHWDataStream(DWORD index);
    void* LockHardwareStream(DWORD index);
    BOOL UnlockHardwareStream(DWORD index);
};

#endif


#ifdef CB_INLINES
#ifndef _CMESH_H_INLINES
#define _CMESH_H_INLINES

//@START_USER3
//@END_USER3

#endif
#endif
