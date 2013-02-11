///============================================================================
/// \file		LoadSaveMessages.hpp
/// \brief		Header file for Predefined LoadSaveMessages/structures
/// \date		04-21-2005
/// \author		Kenneth Hurley
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

#ifndef _LOADSAVEMESSAGES_HPP_
#define _LOADSAVEMESSAGES_HPP_

///-------------------------------------------------------------
/// LoadSaveManager LoadSaveMessages
///-------------------------------------------------------------

/// filter strings structures for load/save extension matching
typedef struct
{
	LPCTSTR m_LoadFilter;			/// the filters used to determine extensions for loading
	LPCTSTR m_SaveFilter;			/// the filters used to determine extensions for saving
} FILTERSTRINGS;

typedef bool (*CACHABLEQUERYCALLBACK)(IHashString *);

/// full message structure including component name
typedef struct LSRC_MESSAGE
{
	IHashString *m_ComponentName;	/// name of component that handles this extension
	FILTERSTRINGS m_FilterStrings;	/// structure of pointers to filters
	bool m_bPrecachable;
	CACHABLEQUERYCALLBACK m_pfnCallback;
} LSRC_MESSAGE;

typedef map <DWORD, FILTERSTRINGS *> LOADSAVEMAP;
typedef list<FILTERSTRINGS *> FILTERSTRINGSLIST;

#endif		// #ifndef _LOADSAVEMESSAGES_HPP_
