///============================================================================
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

#ifndef _OPENAL_STDAFX_H
#define _OPENAL_STDAFX_H

#include "Platform.h"

#include <assert.h>

#include "ElementalEngine.hpp"
#include "EEDefines.h"
#include "EE-SDK.h"
#include "CHashString.hpp"
#include "Register.h"
#include "BaseMessages.h"
#include "CObjectTemplate.hpp"
#include "ISoundObject.hpp"
#include "mathutils.h"
#include "Vec3.h"
#include "Matrix4x4.h"
#include "Matrix3x3.h"
#include "toluaxx.h"


//OpenAL header files
#include "al.h"
#include "alut.h"

#include "OpenALDefines.h"
#include "OpenALErrorString.h"

// some ogg vorbis includes
#include "ogg\ogg.h"
#include "vorbis\vorbisfile.h"

#include "ISoundBufferStream.h"
#include "CCachedSoundBufferStream.h"
#include "CThreadedOggDecoder.h"
#include "COGGSoundBufferStream.h"

#include "CMusicPlaylist.h"

#include "OpenALSoundUtility.h"
#include "CSoundObject.h"
#include "OpenALSoundObject.h"

#endif	// #ifndef _OPENAL_STDAFX_H
