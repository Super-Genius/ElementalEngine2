///==========================================================================
/// \file	EEDefines.h
/// \brief	Contains common defines for Elemental Engine
/// \date	2/28/2005
/// \author Kyle Swaim
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

#ifndef _EEDEFINES_H_
#define _EEDEFINES_H_

#define MATHPRIORITY			0x0070
#define TIMEPRIORITY        	0x0078
#define HIERARCHYPRIORITY		0x0100
#define STATEMACHINEPRIORITY	0x0180
#define NODESYSTEMPRIORITY		0x0190
#define AIPRIORITY				0x01B0
#define PHYSICSPRIORITY			0x0200
#define SOUNDPRIORITY			0x0300
#define SHADERPRIORITY			0x05C0
#define ATTACHOBJECTPRIORITY	0x05D0
#define CAMERAPRIORITY			0x05E0
#define PARTICLEPRIORITY		0x05F7
#define RENDEROBJECTPRIORITY	0x0600
#define COORDINATETOOLPRIORITY	0x0610
#define LIGHTMAPPRIORITY		0x0620
#define HIEARCHICALMODELPRIORITY 0x0640
#define SCENEVISPRIORITY		0x0641
#define LIGHTPRIORITY			0x0642
#define SPRITEPRIORITY			0x0648
#define GUIPRIORITY				0x0660
#define FONTPRIORITY			0x0670
#define RENDERPRIORITY			0x0680
#define INPUTPRIORITY			0x0700
#define SPLINEPRIORITY			0x0800
#define PROXYPUMPPRIORITY		0x0900
#define PROFILERPRIORITY		0x0A00
#define NETWORKPRIORITY			0x0B00
#define DELAYEDMESSAGEPRIORITY	0x020000

//Used between EE State Machine and GDS State Editor
namespace ElementalEngine
{
    enum {PARM_STRING, PARM_SIGARGS, PARM_FLOAT, PARM_DOUBLE, PARM_INT, PARM_BOOL, PARM_TCHAR, PARM_LAST};
#if defined(KEYWORD_TRUE) || defined(KEYWORD_FALSE)
#error definition collision
#endif
#define KEYWORD_TRUE "true"
#define KEYWORD_FALSE "false"

struct INITDLLINFO
{
	void (*InitDLL)();
	DWORD (*GetDLLVersion)();
	DWORD (*GetPriority)();
	void (*DeInitDLL)();
	void **StaticLibs;

	inline bool operator<(const INITDLLINFO &rhs) const
	{
		const INITDLLINFO &lhs = *this;
		return (lhs.GetPriority() < rhs.GetPriority());
	}
};

}

#endif //_EEDEFINES_H_
