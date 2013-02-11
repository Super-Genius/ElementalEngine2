//-----------------------------------------------------------------------------
//
// File:          CMesh.cpp
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Method implementations of class 'CMesh'
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

//@START_USER1
//@END_USER1


// Master include file
#include "StdAfx.h"


//@START_USER2
//@END_USER2




/*@NOTE_4402
Constructor method.
*/
CMesh::CMesh(LPCSTR name, BOOL isStatic) //@INIT_4402
: CBaseObject(&GUID_MESH, name)
    
{//@CODE_4402

        m_IsStatic = isStatic;
   
}//@CODE_4402


/*@NOTE_4395
Destructor method.
*/
CMesh::~CMesh()
{//@CODE_4395
    DestructorInclude();

    // Put in your own code
}//@CODE_4395


bool CMesh::CMesh::CMesh(DWORD count, DWORD* streams)
{//@CODE_4409
 
		return TRUE; 
}//@CODE_4409


DATASTREAM* CMesh::GetDataStream(DWORD index)
{//@CODE_4405

		return  m_DataStreams[index];
}//@CODE_4405


DWORD CMesh::GetDataStreamCount()
{//@CODE_4407

		return m_DataStreams.size();
}//@CODE_4407


CHardwareStream* CMesh::GetHWDataStream(DWORD index)
{//@CODE_4416

		return NULL;
}//@CODE_4416


void* CMesh::LockHardwareStream(DWORD index)
{//@CODE_4412

		return m_DataStreams[index]->m_Data;
}//@CODE_4412


BOOL CMesh::UnlockHardwareStream(DWORD index)
{//@CODE_4414

		return TRUE;
}//@CODE_4414


//{{AFX DO NOT EDIT CODE BELOW THIS LINE !!!

/*@NOTE_4396
Method which must be called first in a constructor.
*/
void CMesh::ConstructorInclude()
{
}


/*@NOTE_4397
Method which must be called first in a destructor.
*/
void CMesh::DestructorInclude()
{
}


// Methods for the relation(s) of the class

//}}AFX DO NOT EDIT CODE ABOVE THIS LINE !!!

//@START_USER3
