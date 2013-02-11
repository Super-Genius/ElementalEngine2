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

#ifndef OBJECTCUBEGENERATOR_H
#define OBJECTCUBEGENERATOR_H

#define CUBEMAPSIZE 512
#define NUMTILES 4
#define TILESIZE (CUBEMAPSIZE / NUMTILES)

#define SRCSIZE 128
#define DESTSIZE (SRCSIZE * NUMTILES)
#define BPP 4
#define DESTSIZEBYTES (DESTSIZE * BPP)
#define SRCSIZEBYTES (SRCSIZE * BPP)

#define VISION_INIT_SIZE 256


class ObjectCubeGenerator
{
public:
	void Generate(  TCHAR * filename, ObjectCubeMap * map, IRenderObject * renderobject );
protected:
	bool WriteBits(BYTE *buffer, int xtile, int ytile, ITextureObject * renderTarget );
	bool CreateTexture(TCHAR * szFilename, int counter, BYTE *buffer, ObjectCubeMap * map );
	
};

#endif
