#ifndef PLATFORM_H__
#define PLATFORM_H__

#ifdef WIN32
#include "PlatformWin32.h"
#elif defined XBOX
#include "PlatformXbox360.h"
#elif defined RVL
#include "PlatformWii.h"
#endif

#ifdef EE_BIG_ENDIAN

#ifdef __cplusplus
inline float FloatSwap( float f )
{
	union
	{
		float f;
		unsigned char b[4];
	} dat1, dat2;

	dat1.f = f;

	dat2.b[0] = dat1.b[3];
	dat2.b[1] = dat1.b[2];
	dat2.b[2] = dat1.b[1];
	dat2.b[3] = dat1.b[0];

	return dat2.f;
}
#endif

#define EE_ENDIANSWAP32( i ) (((i&0x000000FF)<<24)|((i&0x0000FF00)<<8)|((i&0x00FF0000)>>8)|((i&0xFF000000)>>24))

//#define EE_ENDIANSWAP32F( f ) ((float)EE_ENDIANSWAP32((int)f))
#define EE_ENDIANSWAP32F( f ) FloatSwap( f )
#define EE_ENDIANSWAP16( i ) (((i&0xFF)<<8)|((i&0xFF00)>>8))
#else
#define EE_ENDIANSWAP32( i ) (i)
#define EE_ENDIANSWAP32F( f ) (f)
#define EE_ENDIANSWAP16( i ) (i)
#endif

#endif
