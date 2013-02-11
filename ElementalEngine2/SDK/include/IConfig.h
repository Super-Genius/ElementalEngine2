///============================================================================
/// \file		IConfig.h
/// \brief		Header file for HashString based configuration
/// \date		01-25-2006
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

#ifndef _ICONFIG_H_
#define _ICONFIG_H_

namespace ElementalEngine
{

/// PARAM_TYPES
/// enum of every posisble type of param
typedef enum
{
	PARAM_INT,
	PARAM_LONG,
	PARAM_UINT,
	PARAM_ULONG,
	PARAM_LONGLONG,
	PARAM_ULONGLONG,
	PARAM_STRING,
	PARAM_VOIDPTR,
	PARAM_FLOAT,
	PARAM_DOUBLE
} PARAM_TYPES;

/// CONFIGPARAM
/// struct with a union of every possible type of param
typedef struct 
{
	PARAM_TYPES m_ParamType;
	union
	{
		int			iParam;		// parameter as an int
		long		lParam;		// long parameter
		UINT		uiParam;	// unsigned int parameter
		ULONG		ulParam;	// unsigned long parameter
		LONGLONG	llParam;	// longlong (__int64) parameter
		ULONGLONG	ullParam;	// longlong (__int64) parameter
		TCHAR *		strParam;	// string ptr parameter
		void *		voidParam;	// void * to anything
		float		fParam;		// single precision floating point parameter
		double		dParam;		// double precision floating point parameter
	};
} CONFIGPARAM;

/// IConfig
class IConfig
{
public:
	virtual ~IConfig() {};

	/// operators to set the values of this config struct from an int
	virtual void operator=(int inParam) = 0;
	/// operators to set the values of this config struct from an long
	virtual void operator=(long inParam) = 0;
	/// operators to set the values of this config struct from an unsigned int
	virtual void operator=(UINT inParam) = 0;
	/// operators to set the values of this config struct from an unsigned long
	virtual void operator=(ULONG inParam) = 0;
	/// operators to set the values of this config struct from a long long
	virtual void operator=(LONGLONG inParam) = 0;
	/// operators to set the values of this config struct from an unsigned long long
	virtual void operator=(ULONGLONG inParam) = 0;
	/// operators to set the values of this config struct from a tchar pointer
	virtual void operator=(TCHAR *inParam) = 0;
	/// operators to set the values of this config struct from a void pointer
	virtual void operator=(void *inParam) = 0;
	/// operators to set the values of this config struct from a float
	virtual void operator=(float inParam) = 0;
	/// operators to set the values of this config struct from a double
	virtual void operator=(double inParam) = 0;

	/// operators to convert values to an int
	virtual operator int() = 0;
	/// operators to convert values to a long
	virtual operator long() = 0;
	/// operators to convert values to an unsigned int
	virtual operator UINT() = 0;
	/// operators to convert values to an unsigned long
	virtual operator ULONG() = 0;
	/// operators to convert values to a long long
	virtual operator LONGLONG() = 0;
	/// operators to convert values to an unsigned long long
	virtual operator ULONGLONG() = 0;
	/// operators to convert values to a tchar pointer
	virtual operator TCHAR *() = 0;
	/// operators to convert values to a void pointer
	virtual operator void *() = 0;
	/// operators to convert values to a float
	virtual operator float() = 0;
	/// operators to convert values to a double
	virtual operator double() = 0;

	/// Get the type of this configuration
	virtual PARAM_TYPES GetType() = 0;
};

/// NAMECONFIGMAP
/// map of IHashString pointer names to IConfig pointers
typedef map<IHashString *, IConfig *> NAMECONFIGMAP;

} // namespace ElementalEngine
#endif // #ifndef _ICONFIG_H_
