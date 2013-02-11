///============================================================================
/// \file		Config.h
/// \brief		Header file for concrete class for configuration
/// \date		02-08-2006
/// \author		Kenneth Hurley
/// \note   Elemental Engine
///         Copyright (c)  2005-2008 Signature Devices, Inc.
///
///         This code is redistributable under the terms of the EE License.
///
///         This code is distributed without warranty or implied warranty of
///			merchantability or fitness for a particular purpose. See the 
///			EE License for more details.
///
///         You should have received a copy of the EE License along with this
///			code; If not, write to Signature Devices, Inc.,
///			3200 Bridge Parkway Suite 102, Redwood City, CA 94086 USA.
///============================================================================

#ifndef _CONFIG_H_
#define _CONFIG_H_
#include "IConfig.h"

namespace ElementalEngine
{

class CConfig : public IConfig
{
private:
	CONFIGPARAM m_Param;
public:
	/// operators to set the values of this config struct
	__inline void operator=(int inParam)
	{
		m_Param.iParam = inParam;
		m_Param.m_ParamType = PARAM_INT;
	}

	__inline void operator=(long inParam)
	{
		m_Param.lParam = inParam;
		m_Param.m_ParamType = PARAM_LONG;
	}

	__inline void operator=(UINT inParam)
	{
		m_Param.uiParam = inParam;
		m_Param.m_ParamType = PARAM_UINT;
	}
	__inline void operator=(ULONG inParam)
	{
		m_Param.ulParam = inParam;
		m_Param.m_ParamType = PARAM_ULONG;
	}
	__inline void operator=(LONGLONG inParam)
	{
		m_Param.llParam = inParam;
		m_Param.m_ParamType = PARAM_LONGLONG;
	}

	__inline void operator=(ULONGLONG inParam)
	{
		m_Param.ullParam = inParam;
		m_Param.m_ParamType = PARAM_ULONGLONG;
	}

	__inline void operator=(TCHAR *inParam)
	{
		m_Param.strParam = inParam;
		m_Param.m_ParamType = PARAM_STRING;
	}
	__inline void operator=(void *inParam)
	{
		m_Param.voidParam = inParam;
		m_Param.m_ParamType = PARAM_VOIDPTR;
	}
	__inline void operator=(float inParam)
	{
		m_Param.fParam = inParam;
		m_Param.m_ParamType = PARAM_FLOAT;
	}

	__inline void operator=(double inParam)
	{
		m_Param.dParam = inParam;
		m_Param.m_ParamType = PARAM_DOUBLE;
	}

	/// operators to convert values
	__inline operator int()
	{
		int retVal;
		switch (m_Param.m_ParamType)
		{
			case PARAM_INT:
				return (int)m_Param.iParam;
			case PARAM_LONG:
				return (int)m_Param.lParam;
			case PARAM_UINT:
				return (int)m_Param.uiParam;
			case PARAM_ULONG:
				return (int)m_Param.ulParam;
			case PARAM_LONGLONG:
				return (int)m_Param.llParam;
			case PARAM_ULONGLONG:
				return (int)m_Param.ullParam;
			case PARAM_FLOAT:
				return (int)m_Param.fParam;
			case PARAM_DOUBLE:
				return (int)m_Param.dParam;
			case PARAM_STRING:
				retVal = atoi(m_Param.strParam);
				return retVal;
			default:
				assert(0);
				return 0;
		}
	}
	__inline operator long()
	{
		long retVal;
		switch (m_Param.m_ParamType)
		{
			case PARAM_INT:
				return (long)m_Param.iParam;
			case PARAM_LONG:
				return (long)m_Param.lParam;
			case PARAM_UINT:
				return (long)m_Param.uiParam;
			case PARAM_ULONG:
				return (long)m_Param.ulParam;
			case PARAM_LONGLONG:
				return (long)m_Param.llParam;
			case PARAM_ULONGLONG:
				return (long)m_Param.ullParam;
			case PARAM_FLOAT:
				return (long)m_Param.fParam;
			case PARAM_DOUBLE:
				return (long)m_Param.dParam;
			case PARAM_STRING:
				retVal = atol(m_Param.strParam);
				return retVal;
			default:
				assert(0);
				return 0;
		}
	}
	__inline operator UINT()
	{
		UINT retVal;
		switch (m_Param.m_ParamType)
		{
			case PARAM_INT:
				return (UINT)m_Param.iParam;
			case PARAM_LONG:
				return (UINT)m_Param.lParam;
			case PARAM_UINT:
				return (UINT)m_Param.uiParam;
			case PARAM_ULONG:
				return (UINT)m_Param.ulParam;
			case PARAM_LONGLONG:
				return (UINT)m_Param.llParam;
			case PARAM_ULONGLONG:
				return (UINT)m_Param.ullParam;
			case PARAM_FLOAT:
				return (UINT)m_Param.fParam;
			case PARAM_DOUBLE:
				return (UINT)m_Param.dParam;
			case PARAM_STRING:
				retVal = (UINT)atol(m_Param.strParam);
				return retVal;
			default:
				assert(0);
				return 0;
		}
	}
	__inline operator ULONG()
	{
		ULONG retVal;
		switch (m_Param.m_ParamType)
		{
			case PARAM_INT:
				return (ULONG)m_Param.iParam;
			case PARAM_LONG:
				return (ULONG)m_Param.lParam;
			case PARAM_UINT:
				return (ULONG)m_Param.uiParam;
			case PARAM_ULONG:
				return (ULONG)m_Param.ulParam;
			case PARAM_LONGLONG:
				return (ULONG)m_Param.llParam;
			case PARAM_ULONGLONG:
				return (ULONG)m_Param.ullParam;
			case PARAM_FLOAT:
				return (ULONG)m_Param.fParam;
			case PARAM_DOUBLE:
				return (ULONG)m_Param.dParam;
			case PARAM_STRING:
				retVal = (ULONG)atol(m_Param.strParam);
				return retVal;
			default:
				assert(0);
				return 0;
		}
	}
	__inline operator LONGLONG()
	{
		LONGLONG retVal;
		switch (m_Param.m_ParamType)
		{
			case PARAM_INT:
				return (LONGLONG)m_Param.iParam;
			case PARAM_LONG:
				return (LONGLONG)m_Param.lParam;
			case PARAM_UINT:
				return (LONGLONG)m_Param.uiParam;
			case PARAM_ULONG:
				return (long)m_Param.ulParam;
			case PARAM_LONGLONG:
				return (LONGLONG)m_Param.llParam;
			case PARAM_ULONGLONG:
				return (LONGLONG)m_Param.ullParam;
			case PARAM_FLOAT:
				return (LONGLONG)m_Param.fParam;
			case PARAM_DOUBLE:
				return (LONGLONG)m_Param.dParam;
			case PARAM_STRING:
				retVal = _atoi64(m_Param.strParam);
				return retVal;
			default:
				assert(0);
				return 0;
		}
	}
	__inline operator ULONGLONG()
	{
		ULONGLONG retVal;
		switch (m_Param.m_ParamType)
		{
			case PARAM_INT:
				return (ULONGLONG)m_Param.iParam;
			case PARAM_LONG:
				return (ULONGLONG)m_Param.lParam;
			case PARAM_UINT:
				return (ULONGLONG)m_Param.uiParam;
			case PARAM_ULONG:
				return (ULONGLONG)m_Param.ulParam;
			case PARAM_LONGLONG:
				return (ULONGLONG)m_Param.llParam;
			case PARAM_ULONGLONG:
				return (ULONGLONG)m_Param.ullParam;
			case PARAM_FLOAT:
				return (ULONGLONG)m_Param.fParam;
			case PARAM_DOUBLE:
				return (ULONGLONG)m_Param.dParam;
			case PARAM_STRING:
				retVal = _atoi64(m_Param.strParam);
				return retVal;
			default:
				assert(0);
				return 0;
		}
	}
	__inline operator TCHAR *()
	{
		static TCHAR retStr[256];
		switch (m_Param.m_ParamType)
		{
			case PARAM_INT:
				sprintf(retStr, "%.200i", m_Param.iParam);
				return retStr;
			case PARAM_LONG:
				sprintf(retStr, "%.200li", m_Param.lParam);
				return retStr;
			case PARAM_UINT:
				sprintf(retStr, "%.200u", m_Param.uiParam);
				return retStr;
			case PARAM_ULONG:
				sprintf(retStr, "%.200lu", m_Param.ulParam);
				return retStr;
			case PARAM_LONGLONG:
				sprintf(retStr, "%.200I64i", m_Param.llParam);
				return retStr;
			case PARAM_ULONGLONG:
				sprintf(retStr, "%.200I64u", m_Param.ullParam);
				return retStr;
			case PARAM_STRING:
				return m_Param.strParam;
			case PARAM_VOIDPTR:
				return (TCHAR *)m_Param.voidParam;
			case PARAM_FLOAT:
				sprintf(retStr, "%.200f", m_Param.fParam);
				return retStr;
			case PARAM_DOUBLE:
				sprintf(retStr, "%.200F", m_Param.dParam);
				return retStr;
			default:
				assert(0);
				return NULL;
		}
	}
	__inline operator void *()
	{
		switch (m_Param.m_ParamType)
		{
			case PARAM_INT:
			case PARAM_LONG:
			case PARAM_UINT:
			case PARAM_ULONG:
			case PARAM_LONGLONG:
			case PARAM_ULONGLONG:
			case PARAM_FLOAT:
			case PARAM_DOUBLE:
				return NULL;
			case PARAM_STRING:
				return (void *)m_Param.strParam;
			case PARAM_VOIDPTR:
				return m_Param.voidParam;
			default:
				assert(0);
				return NULL;
		}
	}
	__inline operator float()
	{
		float retVal;
		switch (m_Param.m_ParamType)
		{
			case PARAM_INT:
				return (float)m_Param.iParam;
			case PARAM_LONG:
				return (float)m_Param.lParam;
			case PARAM_UINT:
				return (float)m_Param.uiParam;
			case PARAM_ULONG:
				return (float)m_Param.ulParam;
			case PARAM_LONGLONG:
				return (float)m_Param.llParam;
			case PARAM_ULONGLONG:
				return (float)m_Param.ullParam;
			case PARAM_FLOAT:
				return (float)m_Param.fParam;
			case PARAM_DOUBLE:
				return (float)m_Param.dParam;
			case PARAM_STRING:
				retVal = (float)atof(m_Param.strParam);
				return retVal;
			default:
				assert(0);
				return 0.0f;
		}
	}
	__inline operator double()
	{
		double retVal;
		switch (m_Param.m_ParamType)
		{
			case PARAM_INT:
				return (double)m_Param.iParam;
			case PARAM_LONG:
				return (double)m_Param.lParam;
			case PARAM_UINT:
				return (double)m_Param.uiParam;
			case PARAM_ULONG:
				return (double)m_Param.ulParam;
			case PARAM_LONGLONG:
				return (double)m_Param.llParam;
			case PARAM_ULONGLONG:
				return (double)m_Param.ullParam;
			case PARAM_FLOAT:
				return (double)m_Param.fParam;
			case PARAM_DOUBLE:
				return (double)m_Param.dParam;
			case PARAM_STRING:
				retVal = atof(m_Param.strParam);
				return retVal;
			default:
				assert(0);
				return 0.0;
		}
	}

	/// Get the type of this configuration
	__inline PARAM_TYPES GetType()
	{
		return m_Param.m_ParamType;
	}
};

};

#endif // #ifndef _CONFIG_H_
