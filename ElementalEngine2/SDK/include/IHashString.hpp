///============================================================================
/// \file		IHashString.hpp
/// \brief		Interface file for HashString class
/// \date		03-19-2005
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

#ifndef _IHASHSTRING_HPP_
#define _IHASHSTRING_HPP_

namespace ElementalEngine
{
/// IHashString allows quick compares of strings if they are in a table
class IHashString
{
public:
	/// virtual destructor
	virtual ~IHashString() {};
	/// operator== compare hashstrings to string
	/// \param src = pointer to const TCHAR * null terminated string
	/// \return true if complete match
	virtual bool operator==(const TCHAR *src) = 0;

	/// operator== compare two hashstrings
	/// \param hs = hashstring to compare against
	/// \return true if complete match
	virtual bool operator==(IHashString& hs) = 0;

	/// operator!= compare two hashstrings
	/// \param hs = hashstring to compare against
	/// \return true if no match
	virtual bool operator!=(IHashString& hs) = 0;

	/// operator= assign this hashstring to a string
	/// \param inStr = string to set HashString to
	/// \return reference to self
	virtual IHashString &operator=(const TCHAR *inStr) = 0;

	/// operator= assign this hashstring to a string
	/// \param inStr = IHashString to set HashString to
	/// \return reference to self
	virtual IHashString &operator=(const IHashString& hs) = 0;

	/// operator< check if one IHashString is less than other
	/// \param hs = pointer to right IHashString
	/// \return bool if Left < Right
	virtual bool operator<(const IHashString *hs) const = 0;

	/// Get the unique ID associated with this HashString
	/// \return DWORD which is unique ID for this HashString
	virtual DWORD GetUniqueID() const = 0;

	/// Get the string pointer for this HashString
	/// \return const TCHAR * which is pointer to string
	virtual const TCHAR* GetString() const = 0;

	/// check if string is empty
	///	\return true if hash string holds empty value
	virtual bool IsEmpty() const = 0;

	/// Conversion from IHashString to DWORD
//	virtual operator DWORD() = 0;

	/// Conversion from IHashString to DWORD
//	virtual operator DWORD() const = 0;
}; 


}	//namespace ElementalEngine

#endif // #ifndef _IHASHSTRING_HPP_
