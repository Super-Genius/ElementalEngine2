///============================================================================
/// \file		MessageError.hpp
/// \brief		Header file for Message Errors and such
/// \date		03-23-2005
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

#ifndef _MESSAGEERROR_HPP_
#define _MESSAGEERROR_HPP_


#include <assert.h>

namespace ElementalEngine
{
#define VERIFY_MESSAGE_SIZE(inSize, checkSize)										\
if (inSize != checkSize)															\
{																					\
	EngineGetToolBox()->SetErrorValue(ERR_MSG_SIZE_INVALID);						\
	EngineGetToolBox()->Log(LOGERROR, _T("%s(%d): Message size is %d and should be %d\n"),	\
		__FILE__, __LINE__, inSize, checkSize);										\
	return MSG_SIZE_INVALID;														\
}

#define MSG_NOT_HANDLED			0
#define MSG_HANDLED				1
#define MSG_HANDLED_PROCEED		2
#define MSG_HANDLED_STOP		3
#define MSG_SIZE_INVALID		4
#define MSG_ERROR				0xBAADDEAD
#define MSG_WARNING				0xBADDECAF

}	//namespace ElementalEngine

#endif		// #ifndef _MESSAGEERROR_HPP_