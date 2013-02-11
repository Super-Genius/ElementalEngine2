///============================================================================
/// \file		LuaHashString.h
/// \brief		Header file for CLuaHashString
/// \date		05-27-2005
/// \author		Josh Lulewicz
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

#ifndef _LUAHASHSTRING_H_
#define _LUAHASHSTRING_H_

// tolua_begin
class CLuaHashString : 	public IHashString 
{
public:
	/// Default Constructor
	CLuaHashString();
	/// Construct a Lua HashString from a TCHAR*
	/// \param szName - string to us for creation
	CLuaHashString(const TCHAR *szName);
	/// Construct a Lua HashString from a StdString
	/// \param szName - string to us for creation
	CLuaHashString(StdString *szName);
	/// Construct a Lua HashString from a IHashString
	/// \param hashString - IHashString pointer
	CLuaHashString(IHashString *hashString);


	/// virtual destructor
	virtual ~CLuaHashString();

	void Init(const TCHAR *szName);

	void Init(StdString szName)
	{
		Init((const TCHAR*) szName);
	}

	/// operator== compare hashstrings to string
	/// \param src = pointer to const TCHAR * null terminated string
	/// \return true if complete match
	virtual bool operator==(const TCHAR *src);

	/// operator== compare two hashstrings
	/// \param hs = hashstring to compare against
	/// \return true if complete match
	virtual bool operator==(IHashString& hs);

// tolua_end
	/// operator!= compare two hashstrings
	/// \param hs = hashstring to compare against
	/// \return true if no match
	virtual bool operator!=(IHashString& hs);

	/// operator= assign this hashstring to a string
	/// \param inStr = string to set HashString to
	/// \return reference to self
	virtual IHashString &operator=(const TCHAR *inStr);

	/// operator= assign this hashstring to a string
	/// \param inStr = IHashString to set HashString to
	/// \return reference to self
	virtual IHashString &operator=(const IHashString& hs);

	/// operator= assign this hashstring to a string
	/// \param inStr = CHashString to set HashString to
	/// \return reference to self
	virtual	IHashString& operator=(const CHashString &hs);

// tolua_begin
	/// operator< check if one IHashString is less than other
	/// \param hs = pointer to right IHashString
	/// \return bool if Left < Right
	virtual bool operator<(const IHashString *hs) const;

	/// Get the unique ID associated with this HashString
	/// \return DWORD which is unique ID for this HashString
	virtual DWORD GetUniqueID() const;

	/// Get the string pointer for this HashString
	/// \return const TCHAR * which is pointer to string
	virtual const TCHAR* GetString() const;

	/// check if string is empty
	///	\return true if hash string holds empty value
	virtual bool IsEmpty() const;

	/// Get the string pointer for this HashString
	/// \returns const TCHAR * which is pointer to string
	virtual StdString GetStdString();

// tolua_end
private:
	DWORD m_UniqueID;			/// unique ID for self
	const TCHAR *m_szPtr;
	IToolBox *m_ToolBox;		/// pointer to global toolbox singleton
// tolua_begin
};

// tolua_end

#endif // #ifndef _LUAHASHSTRING_H_
