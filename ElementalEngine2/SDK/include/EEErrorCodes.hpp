///============================================================================
/// \file		EEErrorCodes.hpp
/// \brief		Header file for error codes for EE
/// \date		07-13-2005
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

#ifndef _EEERRORCODES_HPP_
#define _EEERRORCODES_HPP_

// tolua_begin
enum 
{
	LOGINFORMATION	= 0,
	LOGWARNING		= 1,
	LOGERROR		= 2,
	LOGFATALERROR
};
// tolua_end

enum MESSAGEERRORCODES
{
	ERR_MSG_SIZE_INVALID = 0xABADC0DE,
	ERR_NULL_POINTER,
	ERR_NAME_NOT_FOUND,
	ERR_PARENT_NOT_FOUND,
	ERR_SIBLING_NOT_FOUND,
	ERR_FILE_LOAD,
	ERR_LOAD_ERROR,
	ERR_OUT_OF_MEMORY
} ;

enum MESSAGEWARNINGCODES
{
	WARN_COMPONENT_NOT_FOUND = 0x0000AC0DE,
	WARN_OBJECT_NOT_FOUND,
	WARN_INVALID_FILE,
	WARN_INVALID_OPERATION,
	WARN_NAME_ALREADY_EXISTS,
	WARN_NAME_NOT_FOUND,
	WARN_PAGE_NOT_FOUND,
};



#endif // #ifndef _EEERRORCODES_HPP_
