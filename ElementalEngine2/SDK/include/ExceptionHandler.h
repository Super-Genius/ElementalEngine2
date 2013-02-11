///==========================================================================
/// \file	ExceptionHandler.h
/// \brief	C function for doing mini dump, stack dump, etc
/// \date	05/22/2007
/// \author	Nick Tourte
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
///==========================================================================

#ifndef _EXEPTIONHANDLER_H_
#define _EXEPTIONHANDLER_H_

int __cdecl HandleException(PEXCEPTION_POINTERS pExceptPtrs);

#endif