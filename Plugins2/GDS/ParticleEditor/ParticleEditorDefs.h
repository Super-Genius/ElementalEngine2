///=====================================================================
/// \file	ParticleEditorDefs.h
/// \brief	Particle Editor
/// \date	08/27/2006
/// \author	Marvin Gouw
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
///=====================================================================

#pragma once
#ifndef _ParticleEditorDEFS_H__
#define _ParticleEditorDEFS_H__

#include "stdafx.h"

typedef unsigned char UBYTE;


struct RGBACOLOR
{
	UBYTE r,g,b,a;
	void Set(UBYTE inR, UBYTE inG, UBYTE inB, UBYTE inA = 0)
	{
		r = inR;
		g = inG;
		b = inB;
		a = inA;
	};
};
#endif
