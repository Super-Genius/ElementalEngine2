//-----------------------------------------------------------------------------
//
// File:          CConfig.h
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Declaration of class 'CConfig'
//
// Modifications: @INSERT_MODIFICATIONS(* )
* November 26, 2003 14:11 Kenneth Hurley
*     Added method 'DestructorInclude'
*     Added method 'ConstructorInclude'
*     Added method 'SetValue'
*     Added method 'SetValue'
*     Added method 'SetValue'
*     Added method 'SetValue'
*     Added method 'IsDirty'
*     Added method 'GetValue'
*     Added method 'GetValue'
*     Added method 'GetValue'
*     Added method 'GetValue'
*     Added method '~CConfig'
*     Added member 'm_Options'
//
// Copyright 2003, Signature Devices, Inc.
// All rights are reserved. Reproduction in whole or part is prohibited
// without the written consent of the copyright owner.
//
//-----------------------------------------------------------------------------

#ifndef _CCONFIG_H
#define _CCONFIG_H

//@START_USER1
//@END_USER1



class CConfig
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
    MAPOP m_Options;

//
// Non-Grouped Methods
//

public:
    virtual ~CConfig();
    __inline BOOL GetValue(LPCSTR name, DWORD* value);
    __inline BOOL GetValue(LPCSTR name, void* *value);
    __inline BOOL GetValue(LPCSTR name, float* value);
    __inline BOOL GetValue(LPCSTR name, double* value);
    __inline BOOL IsDirty(LPCSTR name);
    __inline void SetValue(LPCSTR name, DWORD value);
    __inline void SetValue(LPCSTR name, void* value);
    __inline void SetValue(LPCSTR name, float value);
    __inline void SetValue(LPCSTR name, double value);
};

#endif


#ifdef CB_INLINES
#ifndef _CCONFIG_H_INLINES
#define _CCONFIG_H_INLINES

//@START_USER3
//@END_USER3

#endif
#endif
