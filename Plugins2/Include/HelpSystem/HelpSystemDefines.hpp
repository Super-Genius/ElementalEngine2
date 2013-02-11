///============================================================================
/// \file		HelpSystemDefines.hpp
/// \brief		Common definitions of Help System.
/// \date		11-13-2007
/// \author		Dmitriy S. Sergeyev
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

#ifndef _HELPSYSTEMDEFINES_H_
#define _HELPSYSTEMDEFINES_H_

struct DISPLAYHELPTOPICPARAMS
{
	CString chmName;    ///< The CHM name without ".chm" extension.
	CString topicPath;  ///< Topic path inside CHM.
};

#endif // #ifndef _HELPSYSTEMDEFINES_H_
