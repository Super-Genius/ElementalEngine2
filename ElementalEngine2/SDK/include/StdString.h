///============================================================================
/// \file		StdString.h
/// \brief		Header file for standard strings
/// \date		05-01-2005
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
#ifndef _STDSTRING_H
#define _STDSTRING_H

#include <string>
#include <cctype>
#include <algorithm>
using namespace std;

namespace ElementalEngine
{
// when copying to char *, make sure strings aren't bigger than this,
// if they are then maybe zero termination didn't happen?
#define MAXSTRLEN 65536

#ifdef  _UNICODE
class StdString : public wstring
#else
class StdString : public string
#endif
{
private:
	wstring wTempStr;
	string TempStr;
	unsigned int stringPosition;
public:
	StdString():stringPosition(0) {};

	StdString(char *src)
	{
		*this = src;
		stringPosition = 0;
	}

	StdString(const char *src)
	{
		*this = src;
		stringPosition = 0;
	}

	StdString(wchar_t *src)
	{
		*this = src;
		stringPosition = 0;
	}

	StdString(const wchar_t *src)
	{
		*this = src;
		stringPosition = 0;
	}

	StdString(const wstring &src)
	{
		*this = src;
		stringPosition = 0;
	}

	StdString(const string &src)
	{
		*this = src;
		stringPosition = 0;
	}

	// add this because constructors are here and since
	// these are in the header file, a default destructor
	// is added to ElementalEngine, which can break release/debug
	// mixing of .dlls
	~StdString()
	{
	}

	// ------ Assignment Operators ------- //
	__inline StdString &operator=(const wstring &src)
	{
#ifdef  _UNICODE
		this->wstring::operator=(src);
#else
		clear();
		for (size_t i=0; i < src.size(); i++)
			push_back( (char)src[i] );
#endif
		stringPosition = 0;
		return *this;
	}

	__inline StdString &operator=(const string &src)
	{
#ifndef  _UNICODE
		this->string::operator=(src);
#else
		clear();
		for (size_t i=0; i < src.size(); i++)
			push_back( (wchar_t)src[i] );
#endif
		stringPosition = 0;
		return *this;
	}

	__inline StdString &operator=(const char *src)
	{
#ifndef  _UNICODE
		this->string::operator=(src);
#else
		size_t len = strlen(src);
		clear();
		for (size_t i=0; i < len; i++, src++)
			push_back( (wchar_t)*src );
#endif
		stringPosition = 0;
		return *this;
	}

	__inline StdString &operator=(const wchar_t *src)
	{
#ifdef  _UNICODE
		this->wstring::operator=(src);
#else
		size_t len = wcslen(src);
		clear();
		for (size_t i=0; i < len; i++, src++)
			push_back( (char)*src );
#endif
		stringPosition = 0;
		return *this;
	}

	/// convert StdString& to string&
	/// \returns string & reference to string
	__inline operator string&()
	{
#ifdef  _UNICODE
		TempStr.clear();
		for (size_t i=0; i < size(); i++)
			TempStr.push_back( (char)((*this)[i]) );
		return TempStr;
#else
		return *this;
#endif
	}

	/// convert StdString& to const string&
	/// \returns string & reference to string
	__inline operator const string&()
	{
#ifdef  _UNICODE
		TempStr.clear();
		for (size_t i=0; i < size(); i++)
			TempStr.push_back( (char)((*this)[i]) );
		return TempStr;
#else
		return *this;
#endif
	}

	/// convert StdString& to wstring&
	/// \returns wstring & reference to wstring (wide string)
	__inline operator wstring&()
	{
#ifndef  _UNICODE
		wTempStr.clear();
		for (size_t i=0; i < size(); i++)
			wTempStr.push_back( (wchar_t)((*this)[i]) );
		return wTempStr;
#else
		return *this;
#endif
	}

	/// convert StdString& to const wstring&
	/// \returns wstring & reference to wstring (wide string)
	__inline operator const wstring&()
	{
#ifndef  _UNICODE
		wTempStr.clear();
		for (size_t i=0; i < size(); i++)
			wTempStr.push_back( (wchar_t)((*this)[i]) );
		return wTempStr;
#else
		return *this;
#endif
	}

	/// convert StdString to const char *
	/// \returns char * pointer to a character (non-wide) string
	__inline operator const char *() const
	{
		return (const char *)(((string &)*this).c_str());
	}

	/// convert StdString to w_char *
	/// \returns W_char * pointer to a character wide string
	__inline operator const wchar_t *() const
	{
		return (const wchar_t *)(((wstring &)*this).c_str());
	}

	/// sets the string position for the get token function
	/// \param value - position to set for tokenizing
	__inline void ResetTokenPosition(unsigned int value=0)
	{
		stringPosition = value;
	}

	/// Returns a substring consisting of characters between strPos and the next delimeter
	/// NOTE: will not include the delimeter, and each time you call the fn it starts where it left off
	/// \param delims - string of delimeters to check each char against
	/// \param token = token output string to write
	/// \return false if end of string is reached (no delimeter hit from position), true otherwise
	__inline bool GetToken(StdString delims, StdString &outputToken)
	{
		StdString fullStr;
		unsigned int i, j, count;
		count = 0;
		fullStr = *this;
		for (i=stringPosition; i<fullStr.length(); i++)
		{
			for (j=0; j<delims.length(); j++)
			{
				TCHAR testa, testb;
				testa = fullStr[i];
				testb = delims[j];
				if (fullStr[i] == delims[j])
				{
					outputToken = fullStr.substr(stringPosition, count);
					stringPosition += count + 1; // +1 to skip over delim
					return true;
				}				
			}
			count++;
		}
		outputToken = fullStr.substr(stringPosition, count); // no delims from strPos
		stringPosition += count;
		return false;
	}

	/// Conforms all forward slashes to back slashes
	/// \return reference to this string
	__inline StdString& ConformSlashes()
	{
		for (UINT i=0; i<length(); i++)
		{
			if ((*this)[i] == '/')
			{
				(*this)[i] = '\\';
			}
		}

		return *this;
	}

	/// Conforms all forward slashes to back slashes
	/// \return reference to this string
	__inline StdString& ConformSlashesForward()
	{
		for (UINT i=0; i<length(); i++)
		{
			if ((*this)[i] == '\\')
			{
				(*this)[i] = '/';
			}
		}

		return *this;
	}

	/// Lowers string and converts slashes for consistency
	/// \return reference to this string
	__inline StdString& MakeSafeFileName()
	{
		tolower();
		ConformSlashes();
		return *this;
	}

private:
	struct ToLower
	{
		char operator() (char c) const
		{
			return std::tolower(c);
		}
		wchar_t operator() (wchar_t c) const
		{
			return std::tolower(c);
		}
	};
	struct ToUpper
	{
		char operator() (char c) const
		{
			return std::toupper(c);
		}
		wchar_t operator() (wchar_t c) const
		{
			return std::toupper(c);
		}
	};
public:
	__inline StdString& tolower()
	{
	     std::transform (begin(), end(), begin(), ToLower());
		return *this;
	}
	__inline StdString& toupper()
	{
		std::transform (begin(), end(), begin(), ToUpper());
		return *this;
	}
};

}	//namespace ElementalEngine

#endif		// #ifndef _STDSTRING_H
