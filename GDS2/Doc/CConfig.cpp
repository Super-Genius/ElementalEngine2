//-----------------------------------------------------------------------------
//
// File:          CConfig.cpp
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Method implementations of class 'CConfig'
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

//@START_USER1
//@END_USER1


// Master include file
#include "StdAfx.h"


//@START_USER2
//@END_USER2




/*@NOTE_4059
Destructor method.
*/
CConfig::~CConfig()
{//@CODE_4059
    DestructorInclude();

    // Put in your own code
}//@CODE_4059


__inline BOOL CConfig::GetValue(LPCSTR name, DWORD* value)
{//@CODE_4070

        MAPOP::iterator itOI;
		itOI = m_Options.find(name);
		if (itOI != m_Options.end())
		{
			*value = itOI->second.m_ValueDWORD;
			m_Options[name].m_IsDirty = FALSE;
			return TRUE;
		}

		return FALSE;
}//@CODE_4070


__inline BOOL CConfig::GetValue(LPCSTR name, void* *value)
{//@CODE_4076

        MAPOP::iterator itOI;
		itOI = m_Options.find(name);
		if (itOI != m_Options.end())
		{
			*value = itOI->second.m_ValuePtr;
            itOI->second.m_IsDirty = FALSE;
			return TRUE;
		}

		return FALSE;
}//@CODE_4076


__inline BOOL CConfig::GetValue(LPCSTR name, float* value)
{//@CODE_4082

        MAPOP::iterator itOI;
		itOI = m_Options.find(name);
		if (itOI != m_Options.end())
		{
			*value = itOI->second.m_ValueFloat;
            itOI->second.m_IsDirty = FALSE;
			return TRUE;
		}

		return FALSE;
}//@CODE_4082


__inline BOOL CConfig::GetValue(LPCSTR name, double* value)
{//@CODE_4089

        MAPOP::iterator itOI;
		itOI = m_Options.find(name);
		if (itOI != m_Options.end())
		{
			*value = itOI->second.m_ValueDouble;
            itOI->second.m_IsDirty = FALSE;
			return TRUE;
		}

		return FALSE;
}//@CODE_4089


__inline BOOL CConfig::IsDirty(LPCSTR name)
{//@CODE_4065

        MAPOP::iterator itOI;
		itOI = m_Options.find(name);
		if (itOI != m_Options.end())
        {
            return itOI->second.m_IsDirty;
        }

        return FALSE;
   
}//@CODE_4065


__inline void CConfig::SetValue(LPCSTR name, DWORD value)
{//@CODE_4067

		m_Options[name].m_ValueDWORD = value;
		m_Options[name].m_IsDirty = TRUE;
}//@CODE_4067


__inline void CConfig::SetValue(LPCSTR name, void* value)
{//@CODE_4073

		m_Options[name].m_ValuePtr = value;
		m_Options[name].m_IsDirty = TRUE;
}//@CODE_4073


__inline void CConfig::SetValue(LPCSTR name, float value)
{//@CODE_4079

		m_Options[name].m_ValueFloat = value;
        m_Options[name].m_IsDirty = TRUE;
}//@CODE_4079


__inline void CConfig::SetValue(LPCSTR name, double value)
{//@CODE_4085

		m_Options[name].m_ValueDouble = value;
        m_Options[name].m_IsDirty = FALSE;
}//@CODE_4085


//{{AFX DO NOT EDIT CODE BELOW THIS LINE !!!

/*@NOTE_4060
Method which must be called first in a constructor.
*/
void CConfig::ConstructorInclude()
{
}


/*@NOTE_4061
Method which must be called first in a destructor.
*/
void CConfig::DestructorInclude()
{
}


// Methods for the relation(s) of the class

//}}AFX DO NOT EDIT CODE ABOVE THIS LINE !!!

//@START_USER3
