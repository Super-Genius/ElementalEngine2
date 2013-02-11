//---------------------------------------------------------------------
// File:		Config.h
// Description:	string Configuration class
// Created:		5/27/02
// Author:		Kenneth L. Hurley
//---------------------------------------------------------------------

#ifndef CONFIG_H
#define CONFIG_H

#pragma warning(disable : 4786)
#include <map>
#include <string>

using namespace std;

typedef struct OPTIONINFO
{
	union 
	{
		DWORD m_ValueDWORD;
		void *m_ValuePtr;
		float m_ValueFloat;
		double m_ValueDouble;
	};
	BOOL m_IsDirty;
} OPTIONINFO;

using namespace std;

typedef map<string, OPTIONINFO> MAPOP;

class CConfig
{
	// attributes
private:
	MAPOP	m_Options;

public:

    // do we really need this?
	__inline BOOL IsDirty(LPCSTR name)
    {
        MAPOP::iterator itOI;
		itOI = m_Options.find(name);
		if (itOI != m_Options.end())
        {
            return itOI->second.m_IsDirty;
        }

        return FALSE;
    }

	__inline void SetValue(LPCSTR name, DWORD value)
	{
		m_Options[name].m_ValueDWORD = value;
		m_Options[name].m_IsDirty = TRUE;
	}

	__inline BOOL GetValue(LPCSTR name, DWORD *value)
	{
        MAPOP::iterator itOI;
		itOI = m_Options.find(name);
		if (itOI != m_Options.end())
		{
			*value = itOI->second.m_ValueDWORD;
			m_Options[name].m_IsDirty = FALSE;
			return TRUE;
		}

		return FALSE;
	}

	__inline void SetValue(LPCSTR name, void *value)
	{
		m_Options[name].m_ValuePtr = value;
		m_Options[name].m_IsDirty = TRUE;
	}

	__inline BOOL GetValue(LPCSTR name, void **value)
	{
        MAPOP::iterator itOI;
		itOI = m_Options.find(name);
		if (itOI != m_Options.end())
		{
			*value = itOI->second.m_ValuePtr;
            itOI->second.m_IsDirty = FALSE;
			return TRUE;
		}

		return FALSE;
	}

	__inline void SetValue(LPCSTR name, float value)
	{
		m_Options[name].m_ValueFloat = value;
        m_Options[name].m_IsDirty = TRUE;
	}

	__inline BOOL GetValue(LPCSTR name, float *value)
	{
        MAPOP::iterator itOI;
		itOI = m_Options.find(name);
		if (itOI != m_Options.end())
		{
			*value = itOI->second.m_ValueFloat;
            itOI->second.m_IsDirty = FALSE;
			return TRUE;
		}

		return FALSE;
	}

	__inline void SetValue(LPCSTR name, double value)
	{
		m_Options[name].m_ValueDouble = value;
        m_Options[name].m_IsDirty = FALSE;
	}

	__inline BOOL GetValue(LPCSTR name, double *value)
	{
        MAPOP::iterator itOI;
		itOI = m_Options.find(name);
		if (itOI != m_Options.end())
		{
			*value = itOI->second.m_ValueDouble;
            itOI->second.m_IsDirty = FALSE;
			return TRUE;
		}

		return FALSE;
	}
};


#endif	// CONFIG_H