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

#ifndef WEATHERMAP_H
#define WEATHERMAP_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


enum WEATHER_EFFECT { DEFAULT_WEATHER = 0, SNOW_WEATHER, RAIN_WEATHER };
/*
class WeatherPGroup_cl;
class SplatPGroup_cl;
//typedef VSmartPtr<WeatherPGroup_cl> WeatherPGroupPtr;
//typedef VSmartPtr<SplatPGroup_cl> SplatPGroupPtr;

//////////////////////////////////////////////////////////////////////
//********************************************************************
// RainHeightValue_t : structure that represents a single height value
//********************************************************************
//////////////////////////////////////////////////////////////////////
struct RainHeightValue_t
{
  RainHeightValue_t() {iHeight=0;}
  inline float GetHeight() const {return (float)iHeight;}
  short iHeight;  // height value (-32000 to 32000)
//  UBYTE iNoise;   // height noise of the tile
//  UBYTE iFlags;   // additional flags (e.g. surface type), currently unused
};


//////////////////////////////////////////////////////////////////////
//********************************************************************
// RainHeightmap_cl : class that represents the rain heightmap
//    Only one instance has to exist for a map and can be shared
//    between weather particle groups
//********************************************************************
//////////////////////////////////////////////////////////////////////
class RainHeightmap_cl
{
public:
  // constructor
	VISION_APIFUNC RainHeightmap_cl();

  // destructor
	VISION_APIFUNC ~RainHeightmap_cl();

  // creates the map from current world
	void CreateFromCurrentWorld(int iSubdivX=256, int iSubdivY=256, bool bForceRecreate=false);

  // traces the height values
	void Create(const VisVector_cl &vMin, const VisVector_cl &vMax, int iSubdivX=256, int iSubdivY=256);

  // get the world extension (relevant min/max position)
	void GetWorldExtension(VisVector_cl &vMin, VisVector_cl &vMax);

  // frees the heightfield memory
	void ReleaseAll();

  // tries to load from file
  bool LoadFromFile(const char *szFilename);

  // saves to file
  bool SaveToFile(const char *szFilename);  

  // helper function to show progress while creating heightmap
	void ShowPercentage(const char *szTitle, float fVal);

  // function to check wether given world position is under the ground
  bool __fastcall IsBelowGround(const float *wrldpos,float &fHeight) const;

  // get the heightfield height at given position
	float __fastcall GetMinHeightAtPos(const float *wrldpos) const;

  // get the bilinearely interpolated heightfield height at given position
  float __fastcall GetMinHeightAtPos_Bilin(const float *wrldpos) const;

  // helper function to convert the world name to filename
  static void ConvertFilename(char *szDest, const char *szSource);

protected:
  VisBoundingBox_cl m_WorldExtension; // bounding box of worls extension
  VisVector_cl m_vInvScale;           // 1/bordersize for faster calculation
  float m_fMaxTraceHeight;            // the maximum height value for early out

  short m_iSizeX, m_iSizeY;           // number of subdivisions
  RainHeightValue_t *m_pHeightVal;    // array of height values
};



// number of cached particles
#define MAX_CACHED_COUNT  256


//////////////////////////////////////////////////////////////////////
//********************************************************************
// SplatPGroup_cl : A helper particle group that handles splats on
//    ground (e.g. raindrops)
//********************************************************************
//////////////////////////////////////////////////////////////////////
class SplatPGroup_cl : public VisParticleGroup_cl
{
public:
  // constructor
	SplatPGroup_cl(int iParticleCount, const char *szTexturename, VIS_TransparencyType eTransp=VIS_TRANSP_ALPHA);

  // get a pointer to valid particle
  Particle_t * __fastcall GetFreeParticle();

  // create a particle from source (rain-) particle
  void InitNewFromParticle(const Particle_t *pSrc, float fLifetime, float fSizeGrowth, float fAlpha);

  // tick function of particle group : handles fading out etc.
	void ModSysNotifyFunctionHandle();

private:
  int m_iCurrentValidCount; // upper limit of used particles
  int m_iCachedFreeCount;   // number of free particles in cache
  short m_iCachedFreeIndex[MAX_CACHED_COUNT]; // list of cached particle indices
};


//////////////////////////////////////////////////////////////////////
//********************************************************************
// WeatherPGroup_cl : Base class for weather particle groups
//    Can't be instanciated directly. Use derived classes
//********************************************************************
//////////////////////////////////////////////////////////////////////
class WeatherPGroup_cl : public VisParticleGroup_cl  
{
public:
  // constructor
	WeatherPGroup_cl(RainHeightmap_cl *pRainMap, int iParticleCount=5000, float fBoxRadius = 700.f);

  // destructor
	virtual ~WeatherPGroup_cl();

  // tick function
	void ModSysNotifyFunctionHandle();
  
  // overridable, to define the default particle
  virtual void InitRespawnParticle() = NULL;

  // updates the particle bounding box (around camera)
	void UpdateBoundingBox();

  // will be called if camera moves a lot
	void TeleportCamera(const VisVector_cl &vDiff);

  // sets the intensity of weather effect (0=disabled ... 1=fullintensity)
  void SetIntensity(float fIntensity);

  // gets the current intensity
  inline float GetIntensity() const {return m_fIntensity;}

  // respawns a particle after touching ground
	void __fastcall RespawnParticle(int iIndex, bool bTop);

  // initialises a particle for lying on the ground (e.g. snowflakes fade out)
	void __fastcall ParticleGroundInit(Particle_t *p);

  // global helper functions:

  // should be called at the beginning of the app to profile particle rendering
  static void InitWeatherPGroupProfiling();

  // allows to set a global scaling (depending on the world units used particles might jst look too big)
  static void SetGlobalParticleScaling(float fNewScale);

  // Get the current particle scaling
  static float GetGlobalParticleScaling();


  RainHeightmap_cl *m_pMap;         // pointer to the referenced heightmap

  float m_fBoxRadius;               // box radius
  VisBoundingBox_cl m_CurrentBBox;  // current particle bounding box (world coordinates)
  VisVector_cl m_vBoxDiag;          // diagonal vector through bounding box (used sometimes)
  VisVector_cl m_vCamPos;           // current camera position
  float m_fIntensity;               // current intensity
  int m_iActiveParticleCount;       // number of active particles
  int m_iMaxActiveIndex;            // index of the last active particle
  bool m_bHiddenFlag;               // flag that keeps the current "hidden" status of the particle group

  Particle_t m_RespawnParticle;     // particle template for respawning particles
  float m_fSizeVariation;           // variation of particle's size
  float m_fColorVariation;          // color intensity variation of particle color
  float m_fAlphaVariation;          // alpha intensity variation of particle alpha
  float m_fHSpeedVariation;         // horizontal speed variation
  float m_fVSpeedVariation;         // vertical speed variation
  int m_iMoveFlags;                 // special flags for moving the particles
  SplatPGroupPtr m_spSplatGroup;    // pointer to particle group that handles rain splashes on the ground (different texture...)
  static float g_fGlobalScale;      // global scaling of particles
};


//////////////////////////////////////////////////////////////////////
//********************************************************************
// RainPGroup_cl : weather particle group for rain
//********************************************************************
//////////////////////////////////////////////////////////////////////
class RainPGroup_cl : public WeatherPGroup_cl  
{
public:
  // constructor
	RainPGroup_cl(RainHeightmap_cl *pRainMap, int iParticleCount=5000, float fBoxRadius = 500.f);

  // overridden implementation that inits the respawn particle
	virtual void InitRespawnParticle();
};

//////////////////////////////////////////////////////////////////////
//********************************************************************
// SnowPGroup_cl : weather particle group for snow
//********************************************************************
//////////////////////////////////////////////////////////////////////
class SnowPGroup_cl : public WeatherPGroup_cl  
{
public:
  // constructor
	SnowPGroup_cl(RainHeightmap_cl *pRainMap, int iParticleCount=5000, float fBoxRadius = 500.f);

  // overridden implementation that inits the respawn particle
	virtual void InitRespawnParticle();
};
*/

#endif // #ifndef WEATHERMAP_H