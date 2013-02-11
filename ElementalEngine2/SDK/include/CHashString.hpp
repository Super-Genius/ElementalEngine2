///============================================================================
/// \file		CHashString.hpp
/// \brief		Header file for CHashString class
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

#ifndef _CHASHSTRING_HPP_
#define _CHASHSTRING_HPP_


#include "IHashString.hpp"
#include "Singleton.h"
#include <hash_map>


#ifdef _MSC_VER
using namespace stdext;
#endif

namespace ElementalEngine
{

/// CHashString is supposed to be used as a key in a HashTable. 
/// It is used like a normal string but with the added possibility of create it 
/// as a pointer to an external array of chars without committing copy.
class CHashString : public IHashString
{
private:
	DWORD m_UniqueID;			/// unique ID for self
	const TCHAR *m_szPtr;
	inline IToolBox* GetToolBox()
	{
		static IToolBox *pToolBox = EngineGetToolBox();
		return pToolBox;
	}
public:
	CHashString()
	{
		m_szPtr = NULL;
		m_UniqueID = 0;
	}

	///	Create CHashString from a zero terminated array of chars. 
	/// The original intent was, perhaps, to avoid making a copy
	/// of a constant string.  But a copy is made here all the same.
	/// This may change in the future.
	///	\param szName zero terminated array of chars
	CHashString(const TCHAR *szName)
	{
		Init(szName);
	}

	void Init(const TCHAR *szName)
	{
		HASHSTRINGMAP *hsm;
		HASHSTRINGMAP::iterator hsmIter;

		hsm = GetToolBox()->GetHashStringMap();
		hsmIter = hsm->find(szName);
		if (hsmIter != hsm->end())
		{
			// copy the old data
			m_UniqueID = hsmIter->second;
			m_szPtr = hsmIter->first;
		}
		else
		{
			// fill in new data
			m_UniqueID = GetToolBox()->GetNextUniqueID();
			// duplicate string. HashMap will free it in destructor
			//m_szPtr = _tcsdup(szName);
			// we need constant data here
			//(*hsm)[m_szPtr] = m_UniqueID;

			m_szPtr = GetToolBox()->AddHashString(szName, m_UniqueID);
		}

	}

	// since we can't determine if 
	CHashString(IHashString *src)
	{
		// copy the pointer that is in HashMap
		m_szPtr =  src->GetString();
		// copy the unique ID
		m_UniqueID = src->GetUniqueID();
	}

	~CHashString()
	{
		//printf("We are going away now!\n");
	}

	///	operator to compare CHashString to a const TCHAR * 
	///	\param szName zero terminated array of chars
	inline bool operator==(const TCHAR *szName)
	{
		CHashString hsz(szName);
		return (m_UniqueID == hsz.GetUniqueID());
	}

	inline bool operator==(IHashString &hs)
	{
		return (m_UniqueID == hs.GetUniqueID());
	}

	inline bool operator!=(IHashString &hs)
	{
		return (m_UniqueID != hs.GetUniqueID());
	}

	inline IHashString &operator=(const TCHAR *inStr)
	{
		Init(inStr);
		return *this;
	}

	inline IHashString &operator=(const IHashString& hs)
	{
		// Copy the pointer that is in HashMap from the other HashString. 
		// Since the pointers are stored in a central managed repository this is ok.
		m_szPtr =  hs.GetString();
		m_UniqueID = hs.GetUniqueID();
		return *this;
	}

	inline IHashString &operator=(const CHashString& hs)
	{
		// Copy the pointer that is in HashMap from the other HashString. 
		// Since the pointers are stored in a central managed repository this is ok.
		m_szPtr =  hs.GetString();
		m_UniqueID = hs.GetUniqueID();
		return *this;
	}
	/// operator< check if one IHashString is less than other
	/// \param hs = pointer to right IHashString
	/// \return bool if Left < Right
	bool operator<(const IHashString *hs) const
	{
		return (m_UniqueID < hs->GetUniqueID());
	}

	/// Get the unique ID associated with this HashString
	/// \return DWORD which is unique ID for this HashString
	inline DWORD GetUniqueID() const
	{
		return m_UniqueID;
	}

	/// Get the string pointer for this HashString
	/// \return const TCHAR * which is pointer to string
	inline const TCHAR* GetString() const
	{
		return m_szPtr;
	}

	/// check if string is empty
	///	\return true if hash string holds empty value
	inline bool IsEmpty() const
	{
		static DWORD hashEmpty = CHashString(_T("")).GetUniqueID();
		return hashEmpty == m_UniqueID;
	}
}; 

}	//namespace ElementalEngine


#endif // #ifndef _CHASHSTRING_HPP_
