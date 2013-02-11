///============================================================================
/// \file		EEStdStreamProxy.h
/// \brief		Header file for proxy between std:streambuf and IEEStreams
/// \date		11-14-2006
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

#ifndef _EESTDSTREAMPROXY_H_
#define _EESTDSTREAMPROXY_H_

#include <iostream>
using namespace std;

namespace ElementalEngine
{

class CEEStdStreamProxy : public std::iostream
{
public:
	CEEStdStreamProxy(IEEStream *eeStream);
	~CEEStdStreamProxy(void);
};

} // namespace ElementalEngine

#endif // #ifndef _EESTDSTREAMPROXY_H_
