//-----------------------------------------------------------------------------
//
// File:          CHardwareVertexStream.h
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Declaration of class 'CHardwareVertexStream'
//
// Modifications: @INSERT_MODIFICATIONS(* )
* November 26, 2003 14:11 Kenneth Hurley
*     Added method 'DestructorInclude'
*     Added method 'ConstructorInclude'
*     Added method 'UnlockHardwareStream'
*     Added method 'LockHardwareStream'
*     Added method '~CHardwareVertexStream'
*     Added method 'CHardwareVertexStream'
*     Added method 'CHardwareVertexStream'
*     Added inheritance 'CBaseObject'
*     Added inheritance 'CHardwareStream'
*     Added member 'm_hVertexStream'
//
// Copyright 2003, Signature Devices, Inc.
// All rights are reserved. Reproduction in whole or part is prohibited
// without the written consent of the copyright owner.
//
//-----------------------------------------------------------------------------

#ifndef _CHARDWAREVERTEXSTREAM_H
#define _CHARDWAREVERTEXSTREAM_H

//@START_USER1
//@END_USER1


/*@NOTE_4174
C:\Development\Game Dev System\Plugins\src\include\HardwareStream.h(36)

 attributes
C:\Development\Game Dev System\Plugins\src\include\HardwareStream.h(36)

 {E942E72F-26AA-4a42-8EC8-B1328E086B51}
*/

class CHardwareVertexStream
    : public CHardwareStream
    , public CBaseObject
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
    HANDLE m_hVertexStream;

//
// Non-Grouped Methods
//

public:
    CHardwareVertexStream();
    CHardwareVertexStream(LPCSTR name);
    virtual ~CHardwareVertexStream();
    void* LockHardwareStream();
    BOOL UnlockHardwareStream();
};

#endif


#ifdef CB_INLINES
#ifndef _CHARDWAREVERTEXSTREAM_H_INLINES
#define _CHARDWAREVERTEXSTREAM_H_INLINES

//@START_USER3
//@END_USER3

#endif
#endif
