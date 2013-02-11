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

#include "stdafx.h"
#include "WeatherMap.h"

//////////////////////////////////////////////////////////////////////
// Global helper functions
//////////////////////////////////////////////////////////////////////
/*
static Particle_t g_dummyParticle;
static int tempint;

#define PROFILING_GETFREEPARTICLE         2048
#define PROFILING_PARTICLEHANDLING        2049
#define PROFILING_GETHEIGHTVALUE          2050

#define PARTICLEMOVEMENT_VELOCITY         0x00000001
#define PARTICLEMOVEMENT_SNOWVELOCITY     0x00000002
#define PARTICLEMOVEMENT_LYONGROUND       0x00000004





inline float GetFloatRand()
{
  return (float)rand()*(1.f/(float)RAND_MAX);
}

inline float GetFloatRandNeg()
{
  return (float)rand()*(2.f/(float)RAND_MAX) - 1.f;
}

// FLOAT2INT_UNI
// calculates :       *((int *)&tempfloat)=*((int *)&infloat);
// CPU requirement :  all
inline int FLOAT2INT_UNI(float infloat) {
  __asm fld dword ptr infloat
  __asm fistp dword ptr tempint
  return(tempint);
}

//////////////////////////////////////////////////////////////////////
//********************************************************************
// Static functions
//********************************************************************
//////////////////////////////////////////////////////////////////////

float WeatherPGroup_cl::g_fGlobalScale = 1.f;

void WeatherPGroup_cl::InitWeatherPGroupProfiling()
{
  Vision::Profiling.AddGroup("Weather particle handling");
  Vision::Profiling.AddElement(PROFILING_GETFREEPARTICLE,"Get free particle",TRUE);
  Vision::Profiling.AddElement(PROFILING_PARTICLEHANDLING,"Particle handling",TRUE);
  Vision::Profiling.AddElement(PROFILING_GETHEIGHTVALUE,"Get height value",TRUE);
}

void WeatherPGroup_cl::SetGlobalParticleScaling(float fNewScale)
{
  g_fGlobalScale = fNewScale;
}

float WeatherPGroup_cl::GetGlobalParticleScaling()
{
  return g_fGlobalScale;
}


//////////////////////////////////////////////////////////////////////
//********************************************************************
// RainHeightmap_cl : represents the heightmap values in a map
//********************************************************************
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// RainHeightmap_cl() : constructor of the reain heightmap
//////////////////////////////////////////////////////////////////////
RainHeightmap_cl::RainHeightmap_cl()
{
  m_iSizeX = 0; 
  m_iSizeY = 0;
  m_pHeightVal = NULL;
}


//////////////////////////////////////////////////////////////////////
// ~RainHeightmap_cl() : destructor of the reain heightmap
//////////////////////////////////////////////////////////////////////
RainHeightmap_cl::~RainHeightmap_cl()
{
  ReleaseAll();
}


//////////////////////////////////////////////////////////////////////
// ReleaseAll() : releases the memory
//////////////////////////////////////////////////////////////////////
void RainHeightmap_cl::ReleaseAll()
{
  V_SAFE_DELETE_ARRAY(m_pHeightVal);
}


//////////////////////////////////////////////////////////////////////
// ShowPercentage() : when creating the heightmap, show the status
//////////////////////////////////////////////////////////////////////
void RainHeightmap_cl::ShowPercentage(const char *szTitle, float fVal)
{
  Vision::Message.Print(1,100,100,"%s : %.0f%%",szTitle,fVal*100.f); 
  Vision::RenderScene();
  Vision::Video.UpdateScreen();
}


///////////////////////////////////////////////////////////////////////////////
// Create() : create the heightmap with tracelines in the given min/max range
///////////////////////////////////////////////////////////////////////////////
void RainHeightmap_cl::Create(const VisVector_cl &vMin, const VisVector_cl &vMax, int iSubdivX, int iSubdivY)
{
  ReleaseAll();
  m_iSizeX = iSubdivX;
  m_iSizeY = iSubdivY;
  m_pHeightVal = new RainHeightValue_t[m_iSizeX*m_iSizeY];
  m_WorldExtension.m_vMin = vMin;
  m_WorldExtension.m_vMax = vMax;
  m_vInvScale = vMax-vMin;
  m_vInvScale.x = (float)m_iSizeX/m_vInvScale.x;
  m_vInvScale.y = (float)m_iSizeY/m_vInvScale.y;
  m_vInvScale.z = 1.f/m_vInvScale.z;
  VisTraceLineInfo_t traceInfo;
  m_fMaxTraceHeight = -500000.f;


  const int iTileSub = 8; // for higher accuracy
  const float fInvSub = 1.f/(float)iTileSub;

  int x,y;
  int xx,yy;

  for (y=0;y<m_iSizeY;y++)
  {
    for (x=0;x<m_iSizeX;x++)
    {
      float fMaxZ = -10000000.f;
      float fMinZ = 10000000.f;

      // scan a single tile
      for (yy=0;yy<iTileSub;yy++)
        for (xx=0;xx<iTileSub;xx++)
        {
          VisVector_cl start(
            vMin.x+((float)x + ((float)xx*fInvSub))/m_vInvScale.x,
            vMin.y+((float)y + ((float)yy*fInvSub))/m_vInvScale.y,
            vMax.z-5.f);
          VisVector_cl endpos(start.x, start.y, vMin.z+5.f);
          BOOL bNoHit = Vision::CollisionToolkit.TraceLine(start, endpos, VIS_TRACE_ALL, VIS_TRACE_NONE, NULL,NULL, &traceInfo);
          float z = (bNoHit)? -2000.f : traceInfo.touchPoint.z;
          if (z<fMinZ) fMinZ = z;
          if (z>fMaxZ) fMaxZ = z;
        }

      int iIndex = x + m_iSizeX*y;
  
      RainHeightValue_t *pHVal = &m_pHeightVal[iIndex];
      pHVal->iHeight = (short)fMinZ;
      int iNoise = (int)((fMaxZ-fMinZ)*0.5f);
      VASSERT(iNoise>=0);
      if (iNoise>255) iNoise = 255;
//      pHVal->iNoise = iNoise;

      // get the global max height (for early out)
      if (fMinZ>m_fMaxTraceHeight) m_fMaxTraceHeight = fMinZ;
    }
    ShowPercentage("CALCULATING HEIGHT VALUES",(float)(y+1)/(float)m_iSizeY);
  }

}


///////////////////////////////////////////////////////////////////////////////
// GetWorldExtension() : get the actual relevant world extension (no skybox!)
///////////////////////////////////////////////////////////////////////////////
void RainHeightmap_cl::GetWorldExtension(VisVector_cl &vMin, VisVector_cl &vMax)
{
  // first, get the extension of the whole world
  Vision::World.GetWorldExtensions(vMin.x,vMin.y,vMin.z, vMax.x,vMax.y,vMax.z);

  // now, find the min/max positions within this box with tracelines
  int count = 1000;
  VisVector_cl vMinPos   = vMin;
  VisVector_cl vInvScale = vMax-vMin;
  vInvScale.x = (float)count/vInvScale.x;
  vInvScale.y = (float)count/vInvScale.y;
  vInvScale.z = 1.f/vInvScale.z;
  VisTraceLineInfo_t traceInfo;

  const float large = 1000000000.f;
  vMin.SetXYZ(large,large,vMin.z);
  vMax.SetXYZ(-large,-large,vMax.z);

  // find the real world extension without skybox
  int x,y;
  for (y=0;y<count;y++)
  {
    for (x=0;x<count;x++)
    {
      VisVector_cl start(vMinPos.x+(float)x/vInvScale.x,vMinPos.y+(float)y/vInvScale.y,vMax.z-5.f);
      VisVector_cl endpos(start.x, start.y, vMinPos.z+5.f);
      BOOL bNoHit = Vision::CollisionToolkit.TraceLine(start, endpos, VIS_TRACE_ALL, VIS_TRACE_NONE, NULL,NULL, &traceInfo);
      if (bNoHit) continue;
      if (start.x<vMin.x) vMin.x = start.x;
      if (start.x>vMax.x) vMax.x = start.x;
      if (start.y<vMin.y) vMin.y = start.y;
      if (start.y>vMax.y) vMax.y = start.y;
    }
    ShowPercentage("CALCULATING WORLD EXTENSIONS",(float)(y+1)/(float)count);
  }
}


///////////////////////////////////////////////////////////////////////////////////
// CreateFromCurrentWorld() : load or create the heightmap from the current world
///////////////////////////////////////////////////////////////////////////////////
void RainHeightmap_cl::CreateFromCurrentWorld(int iSubdivX, int iSubdivY, bool bForceRecreate)
{
  const char *szWorldName = Vision::World.GetCurrentWorldName();
  char extFilename[MAX_PATH];
  ConvertFilename(extFilename,szWorldName);

  if (!bForceRecreate && LoadFromFile(extFilename)) return;

  VisVector_cl vMin,vMax;
  GetWorldExtension(vMin,vMax);
  Create(vMin,vMax,iSubdivX,iSubdivY);

  SaveToFile(extFilename);
}



///////////////////////////////////////////////////////////////////////////////////
// GetMinHeightAtPos() : do a non-interpolated height lookup at given worldpos
///////////////////////////////////////////////////////////////////////////////////
float __fastcall RainHeightmap_cl::GetMinHeightAtPos(const float *wrldpos) const
{
  VULPPROFILEFUNCTION(PROFILING_GETHEIGHTVALUE);
  int iIndX = FLOAT2INT_UNI((wrldpos[0]-m_WorldExtension.m_vMin.x)*m_vInvScale.x);
  int iIndY = FLOAT2INT_UNI((wrldpos[1]-m_WorldExtension.m_vMin.y)*m_vInvScale.y);
  if (iIndX<0) iIndX=0;
    else if (iIndX>=m_iSizeX) iIndX = m_iSizeX-1;
  if (iIndY<0) iIndY=0;
    else if (iIndY>=m_iSizeY) iIndY = m_iSizeY-1;

  VASSERT(m_pHeightVal);
  return m_pHeightVal[iIndX+iIndY*m_iSizeX].GetHeight();
}


///////////////////////////////////////////////////////////////////////////////////
// GetMinHeightAtPos() : do a bilinear interpolated height lookup at given worldpos -> slow!
///////////////////////////////////////////////////////////////////////////////////
float __fastcall RainHeightmap_cl::GetMinHeightAtPos_Bilin(const float *wrldpos) const
{
  VULPPROFILEFUNCTION(PROFILING_GETHEIGHTVALUE);
  float mapx = (wrldpos[0]-m_WorldExtension.m_vMin.x)*m_vInvScale.x;
  float mapy = (wrldpos[1]-m_WorldExtension.m_vMin.y)*m_vInvScale.y;
//  int iIndX0 = FLOAT2INT_UNI(mapx);
//  int iIndY0 = FLOAT2INT_UNI(mapy);
  int iIndX0 = (int)mapx;
  int iIndY0 = (int)mapy;
  int iIndX1 = iIndX0+1;
  int iIndY1 = iIndY0+1;

  if (iIndX0<0) iIndX0=0;
    else if (iIndX0>=m_iSizeX) iIndX0 = m_iSizeX-1;
  if (iIndY0<0) iIndY0=0;
    else if (iIndY0>=m_iSizeY) iIndY0 = m_iSizeY-1;
 
  if (iIndX1<0) iIndX1=0;
    else if (iIndX1>=m_iSizeX) iIndX1 = m_iSizeX-1;
  if (iIndY1<0) iIndY1=0;
    else if (iIndY1>=m_iSizeY) iIndY1 = m_iSizeY-1;

  iIndY0 *= m_iSizeX;
  iIndY1 *= m_iSizeX;

  VASSERT(m_pHeightVal);
  float h00 = m_pHeightVal[iIndX0+iIndY0].GetHeight();
  float h01 = m_pHeightVal[iIndX0+iIndY1].GetHeight();
  float h10 = m_pHeightVal[iIndX1+iIndY0].GetHeight();
  float h11 = m_pHeightVal[iIndX1+iIndY1].GetHeight();
  float fx1 = fmodf(mapx + 10000.f, 1.f);
  float fy1 = fmodf(mapy + 10000.f, 1.f);
  float fx0 = 1.f - fx1;
  return (h00*fx0 + h10*fx1)*(1.f-fy1) + (h01*fx0 + h11*fx1)*fy1;
}


///////////////////////////////////////////////////////////////////////////////////
// IsBelowGround() : tests a world pos to be below the ground
///////////////////////////////////////////////////////////////////////////////////
bool __fastcall RainHeightmap_cl::IsBelowGround(const float *wrldpos, float &fHeight) const
{
  if (wrldpos[2]>m_fMaxTraceHeight) return false; // early out
  fHeight = GetMinHeightAtPos_Bilin(wrldpos);
  //fHeight = GetMinHeightAtPos(wrldpos);
  return (fHeight>wrldpos[2]);
}



///////////////////////////////////////////////////////////////////////////////////
// ConvertFilename() : convert the world name to binary file name that stores height values
///////////////////////////////////////////////////////////////////////////////////
void RainHeightmap_cl::ConvertFilename(char *szDest, const char *szSource)
{
  sprintf(szDest,"%s.rmp",szSource);
}


///////////////////////////////////////////////////////////////////////////////////
// LoadFromFile() : load from binary file by nam
///////////////////////////////////////////////////////////////////////////////////
bool RainHeightmap_cl::LoadFromFile(const char *szFilename)
{
  IVFileInStream* pIn = Vision::File.GetManager()->Open(szFilename);
  if (!pIn)
    return false;
  bool ok = LoadFromFile(pIn);
  pIn->Close();

  return ok;
}

///////////////////////////////////////////////////////////////////////////////////
// SaveToFile() : save to binary file
///////////////////////////////////////////////////////////////////////////////////
bool RainHeightmap_cl::SaveToFile(const char *szFilename)
{
  IVFileOutStream* pOut = Vision::File.GetManager()->Create(szFilename);
  if (!pOut)
    return false;
  bool ok = SaveToFile(pOut);
  pOut->Close();
  
  return true;
}


#define LOADVAR(var) {pIn->Read(&var,sizeof(var));}
#define SAVEVAR(var) {pOut->Write(&var,sizeof(var));}


///////////////////////////////////////////////////////////////////////////////////
// LoadFromFile() : load from binary file by stream
///////////////////////////////////////////////////////////////////////////////////
bool RainHeightmap_cl::LoadFromFile(IVFileInStream* pIn)
{
  ReleaseAll();

  LOADVAR(m_iSizeX);
  LOADVAR(m_iSizeY);
  LOADVAR(m_WorldExtension);
  LOADVAR(m_vInvScale);
  LOADVAR(m_fMaxTraceHeight);
  int count = m_iSizeX*m_iSizeY;
  VASSERT(count>0);

  VASSERT(m_pHeightVal==NULL);
  m_pHeightVal = new RainHeightValue_t[count];
  pIn->Read(m_pHeightVal,count*sizeof(RainHeightValue_t));

  return true;
}

///////////////////////////////////////////////////////////////////////////////////
// SaveToFile() : save to a binary file stream
///////////////////////////////////////////////////////////////////////////////////
bool RainHeightmap_cl::SaveToFile(IVFileOutStream* pOut)
{
  SAVEVAR(m_iSizeX);
  SAVEVAR(m_iSizeY);
  SAVEVAR(m_WorldExtension);
  SAVEVAR(m_vInvScale);
  SAVEVAR(m_fMaxTraceHeight);
  int count = m_iSizeX*m_iSizeY;
  VASSERT(count>0);

  pOut->Write(m_pHeightVal,count*sizeof(RainHeightValue_t));

  return true;
}


//////////////////////////////////////////////////////////////////////
//********************************************************************
// WeatherPGroup_cl : Base class for weather particle effects
//********************************************************************
//////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////
// WeatherPGroup_cl() : constructor of the weather particle group base class
///////////////////////////////////////////////////////////////////////////////////
WeatherPGroup_cl::WeatherPGroup_cl(RainHeightmap_cl *pRainMap, int iParticleCount, float fBoxRadius)
{
  // init the particle group
  Init(Vision::Game.GetContainerEntity(),1, iParticleCount);
  memset(GetParticles(),0,sizeof(Particle_t)*iParticleCount);

  m_pMap = pRainMap;
  m_fBoxRadius = fBoxRadius*g_fGlobalScale;

  // intensity = 0.f
  m_bHiddenFlag = false;
  m_iActiveParticleCount = 1; // force set intensity
  m_iMaxActiveIndex = -1;
  SetIntensity(0.f);

  UpdateBoundingBox();
}


///////////////////////////////////////////////////////////////////////////////////
// SetIntensity() : sets the weather intensity (0..1). 
//    Actual intensity depends on particle count and box size
///////////////////////////////////////////////////////////////////////////////////
void WeatherPGroup_cl::SetIntensity(float fIntensity)
{
  if (fIntensity<0.f) fIntensity = 0.f;
    else if (fIntensity>1.f) fIntensity = 1.f;

  m_fIntensity = fIntensity;
  int iOldCount = m_iActiveParticleCount;
  m_iActiveParticleCount = (int)((float)GetNumOfParticles()*m_fIntensity);

  if (iOldCount==m_iActiveParticleCount) return;

  bool bTopRespawn = (m_iActiveParticleCount-iOldCount)<20;

  int i;
  for (i=iOldCount;i<m_iActiveParticleCount;i++)
    RespawnParticle(i, bTopRespawn);
}




///////////////////////////////////////////////////////////////////////////////////
// ~WeatherPGroup_cl() : destructor of the weather particle group base class
///////////////////////////////////////////////////////////////////////////////////
WeatherPGroup_cl::~WeatherPGroup_cl()
{

}


///////////////////////////////////////////////////////////////////////////////////
// UpdateBoundingBox() : updates the particle bounding box position from camera pos
///////////////////////////////////////////////////////////////////////////////////
void WeatherPGroup_cl::UpdateBoundingBox()
{
  VisVector_cl rad(m_fBoxRadius,m_fBoxRadius,m_fBoxRadius);

  VisVector_cl vNewPos;
  Vision::Camera.GetCurrentCameraPosition(vNewPos);
  VisVector_cl diff = vNewPos-m_vCamPos;

  if (diff.GetLen()>(2.f*m_fBoxRadius)) // tweaked threshold
    TeleportCamera(diff);

  m_vCamPos = vNewPos;
  m_CurrentBBox.m_vMin = m_vCamPos - rad;
  m_CurrentBBox.m_vMax = m_vCamPos + rad;

  m_vBoxDiag = m_CurrentBBox.m_vMax - m_CurrentBBox.m_vMin;
}


///////////////////////////////////////////////////////////////////////////////////
// TeleportCamera() : shifts all particles because camera moved a lot
///////////////////////////////////////////////////////////////////////////////////
void WeatherPGroup_cl::TeleportCamera(const VisVector_cl &vDiff)
{
  int i;
  Particle_t *p = GetParticles();

  for (i=0;i<=m_iMaxActiveIndex;i++,p++) if (p->valid)
  {
    p->pos[0] += vDiff.x;
    p->pos[1] += vDiff.y;
    p->pos[2] += vDiff.z;
  }
}

#define ADD_COLOR_CLAMP(dstbyte,val) \
{\
  int _iVal = dstbyte+val;\
  if (_iVal<0) dstbyte=0;\
    else if (_iVal>255) dstbyte=255;\
      else dstbyte=_iVal;\
}

///////////////////////////////////////////////////////////////////////////////////
// RespawnParticle() : respawns a particle (e.g. when it touches ground)
///////////////////////////////////////////////////////////////////////////////////
void __fastcall WeatherPGroup_cl::RespawnParticle(int iIndex, bool bTop)
{
  VASSERT(iIndex>=0 && iIndex<GetNumOfParticles())

  Particle_t *p = &GetParticles()[iIndex];
  if (iIndex>=m_iActiveParticleCount)
  {
    p->valid = 0;
    return;
  }

  if (iIndex>m_iMaxActiveIndex)
    m_iMaxActiveIndex = iIndex;

  *p = m_RespawnParticle;
  p->pos[0] = m_CurrentBBox.m_vMin.x + GetFloatRand()*m_vBoxDiag.x;
  p->pos[1] = m_CurrentBBox.m_vMin.y + GetFloatRand()*m_vBoxDiag.y;
  if (bTop)
  {
    p->pos[2] = m_CurrentBBox.m_vMax.z;
  } else
  {
    p->pos[2] = m_CurrentBBox.m_vMin.z + GetFloatRand()*m_vBoxDiag.z;
  }

  p->size += m_fSizeVariation*GetFloatRand();
  p->velocity[0] += m_fHSpeedVariation * GetFloatRandNeg();
  p->velocity[1] += m_fHSpeedVariation * GetFloatRandNeg();
  p->velocity[2] += m_fVSpeedVariation * GetFloatRandNeg();
  p->normal[1] = VIS_2PI * GetFloatRand(); // snow

  if (m_fColorVariation>0.f)
  {
    int d = (int)(m_fColorVariation* GetFloatRandNeg());
    // since d can be negative, we have to clamp in both directions
    ADD_COLOR_CLAMP(p->color.r, d);
    ADD_COLOR_CLAMP(p->color.g, d);
    ADD_COLOR_CLAMP(p->color.b, d);
  }
  if (m_fAlphaVariation>0.f)
  {
    int a = (int)(m_fAlphaVariation*GetFloatRandNeg());
    ADD_COLOR_CLAMP(p->color.a, a);
  }

  if (p->distorted)
  {
    float f = m_RespawnParticle.distortion[0];
    p->distortion[0] = f*p->velocity[0];
    p->distortion[1] = f*p->velocity[1];
    p->distortion[2] = f*p->velocity[2];
  }
}


///////////////////////////////////////////////////////////////////////////////////
// ParticleGroundInit() : initialises the particle on the ground (e.g. snow)
///////////////////////////////////////////////////////////////////////////////////
void __fastcall WeatherPGroup_cl::ParticleGroundInit(Particle_t *p)
{
  const float fLifetime = 1.f;
  p->energy = 1;
  p->velocity[0] = 1.f; // always 1
  p->velocity[1] = 1.f/fLifetime; // speed
  p->velocity[2] = (float)((UBYTE)p->color[3]); // alpha

  p->distorted = 1;
  p->usenormal = 1;

  if (p->usenormal)
  {
    p->normal[0] = 0.f;
    p->normal[1] = 0.f;
    p->normal[2] = 1.f;
  }

  if (p->distorted)
  {
    VisVector_cl right(GetFloatRandNeg(),GetFloatRandNeg(),0.f);
    right.SetLen(p->size);
    p->distortion[0] = right.x;
    p->distortion[1] = right.y;
    p->distortion[2] = right.z;
    p->pos[0] -= 0.5f*p->distortion[0];
    p->pos[1] -= 0.5f*p->distortion[1];
    p->pos[2] -= 0.5f*p->distortion[2];
  }
}



///////////////////////////////////////////////////////////////////////////////////
// ModSysNotifyFunctionHandle() : tick function of the weather group
///////////////////////////////////////////////////////////////////////////////////
void WeatherPGroup_cl::ModSysNotifyFunctionHandle()
{
  VULPPROFILEFUNCTION(PROFILING_PARTICLEHANDLING);

  // early out : show or hide particle group
  if (m_iMaxActiveIndex<0) 
  {
    if (!m_bHiddenFlag) Hide();
    m_bHiddenFlag = true;
    return;
  }
  if (m_bHiddenFlag)
  {
    m_bHiddenFlag = false;
    Show();
  }

  UpdateBoundingBox();
  
  float dtime = Vision::Timer.GetTimeDifference();
  float fHeight;

  Particle_t *p = GetParticles();
  int iParticleCount = m_iMaxActiveIndex;
  m_iMaxActiveIndex = -1;

  int i;
  for (i=0;i<=iParticleCount;i++,p++) if (p->valid)
  {
    m_iMaxActiveIndex = i;
    if (p->energy) // particle is on the ground (snow)
    {
      // fade out
      p->velocity[0] -= dtime * p->velocity[1];
      if (p->velocity[0]<=0.f) 
        RespawnParticle(i,true);
      else
        p->color[3] = (UBYTE)(p->velocity[0]*p->velocity[2]);
    } else
    {
      // particle is falling
      if (m_iMoveFlags & PARTICLEMOVEMENT_VELOCITY)
      {
        p->pos[0] += dtime * p->velocity[0];
        p->pos[1] += dtime * p->velocity[1];
        p->pos[2] += dtime * p->velocity[2];
      }

      if (m_iMoveFlags & PARTICLEMOVEMENT_SNOWVELOCITY)
      {
        p->normal[1] = fmodf(p->normal[1] + dtime*GetFloatRand(), VIS_2PI);
        p->normal[0] = fmodf(p->normal[0] + 0.2f*dtime*(1.5f+sinf(p->normal[1])), VIS_2PI);

        p->velocity[0] += 4.f*sinf(p->normal[0]*7.f) * dtime;
        p->velocity[1] += 4.f*sinf(p->normal[0]*6.f) * dtime;
        p->velocity[2] += 1.f*sinf(p->normal[0]*2.f) * dtime;
      }

      // box wrapping
      if (p->pos[0]<m_CurrentBBox.m_vMin.x || p->pos[0]>m_CurrentBBox.m_vMax.x)
        p->pos[0] = m_CurrentBBox.m_vMin.x + fmodf(p->pos[0]-m_CurrentBBox.m_vMin.x + m_vBoxDiag.x*1000.f, m_vBoxDiag.x);

      if (p->pos[1]<m_CurrentBBox.m_vMin.y || p->pos[1]>m_CurrentBBox.m_vMax.y)
        p->pos[1] = m_CurrentBBox.m_vMin.y + fmodf(p->pos[1]-m_CurrentBBox.m_vMin.y + m_vBoxDiag.y*1000.f, m_vBoxDiag.y);

      if (p->pos[2]>m_CurrentBBox.m_vMax.z)
      {
        RespawnParticle(i,false);
        continue;
      }
      else
      if (p->pos[2]<m_CurrentBBox.m_vMin.z) // hit the box ground
      {
        RespawnParticle(i,true);
        continue;
      } 
      if (m_pMap->IsBelowGround(p->pos,fHeight)) // hit the landscape ground
      {
        p->pos[2] = fHeight + 1.f;
        if (m_iMoveFlags & PARTICLEMOVEMENT_LYONGROUND)
        {
          ParticleGroundInit(p); // e.g. snow lies on the ground
        }
        else
        {
          if (m_spSplatGroup)
          {
            VisVector_cl playerdist(m_vCamPos.x-p->pos[0], m_vCamPos.y-p->pos[1], m_vCamPos.z-p->pos[2]);

            float fAlpha = 300.f - 300.f*(playerdist.GetLen()/m_fBoxRadius);
            m_spSplatGroup->InitNewFromParticle(p,0.1f+0.2f*GetFloatRand(),50.f+50.f*GetFloatRand(), fAlpha);
          }
          RespawnParticle(i,true);
        }
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////
//********************************************************************
// RainPGroup_cl : weather particle effect for rain
//********************************************************************
//////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////
// RainPGroup_cl() : constructor for rain particle group
///////////////////////////////////////////////////////////////////////////////////
RainPGroup_cl::RainPGroup_cl(RainHeightmap_cl *pRainMap, int iParticleCount, float fBoxRadius) :
  WeatherPGroup_cl(pRainMap, iParticleCount, fBoxRadius)
{
  InitRespawnParticle();
  m_iMoveFlags = PARTICLEMOVEMENT_VELOCITY;

  // rain splats on the ground, we need a different particle group for this
  m_spSplatGroup = new SplatPGroup_cl(GetNumOfParticles(),"Textures\\Sky\\RainSplat");
}



///////////////////////////////////////////////////////////////////////////////////
// InitRespawnParticle() : overoaded function to define the default particle
///////////////////////////////////////////////////////////////////////////////////
void RainPGroup_cl::InitRespawnParticle()
{
  SetTexture("Textures\\Sky\\RainParticle.tex");
  SetTransparencyType(VIS_TRANSP_ALPHA);

  memset(&m_RespawnParticle,0,sizeof(m_RespawnParticle));
  m_RespawnParticle.valid = 1;
  m_RespawnParticle.distorted = 1;
  m_RespawnParticle.distortion[0] = 0.05f; // length factor for speed
  m_RespawnParticle.color = V_RGBA(145,150,155,80);
  m_RespawnParticle.velocity[0] = 0.f*g_fGlobalScale;
  m_RespawnParticle.velocity[1] = 400.f*g_fGlobalScale;
  m_RespawnParticle.velocity[2] = -1200.f*g_fGlobalScale;
  m_RespawnParticle.size = 3.f*g_fGlobalScale;

  m_fSizeVariation = 1.f*g_fGlobalScale;
  m_fColorVariation = 20.f;
  m_fAlphaVariation = 40.f;
  m_fHSpeedVariation = 40.f*g_fGlobalScale;
  m_fVSpeedVariation = 100.f*g_fGlobalScale;
}




//////////////////////////////////////////////////////////////////////
//********************************************************************
// SnowPGroup_cl : weather particle effect for snow
//********************************************************************
//////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////
// SnowPGroup_cl() : constructor for snow particle group
///////////////////////////////////////////////////////////////////////////////////
SnowPGroup_cl::SnowPGroup_cl(RainHeightmap_cl *pRainMap, int iParticleCount, float fBoxRadius) :
  WeatherPGroup_cl(pRainMap, iParticleCount, fBoxRadius)
{
  InitRespawnParticle();
  m_iMoveFlags = PARTICLEMOVEMENT_SNOWVELOCITY | PARTICLEMOVEMENT_VELOCITY | PARTICLEMOVEMENT_LYONGROUND;

}



///////////////////////////////////////////////////////////////////////////////////
// InitRespawnParticle() : overoaded function to define the default particle
///////////////////////////////////////////////////////////////////////////////////
void SnowPGroup_cl::InitRespawnParticle()
{
  SetTexture("Textures\\Sky\\SnowParticle.tex");
  SetTransparencyType(VIS_TRANSP_ALPHA);

  memset(&m_RespawnParticle,0,sizeof(m_RespawnParticle));
  m_RespawnParticle.valid = 1;
  m_RespawnParticle.color = V_RGBA(255,255,255,255);
  m_RespawnParticle.velocity[0] = 0.f;
  m_RespawnParticle.velocity[1] = 0.f;
  m_RespawnParticle.velocity[2] = -150.f*g_fGlobalScale;
  m_RespawnParticle.size = 2.5f*g_fGlobalScale;

  m_fSizeVariation = 1.5f*g_fGlobalScale;
  m_fColorVariation = 0.f;
  m_fAlphaVariation = 0.f;
  m_fHSpeedVariation = 20.f*g_fGlobalScale;
  m_fVSpeedVariation = 40.f*g_fGlobalScale;
}




//////////////////////////////////////////////////////////////////////
//********************************************************************
// SnowPGroup_cl : particle effect for tiny splats on the ground (rain)
//********************************************************************
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// SplatPGroup_cl : constructor for particle group
//////////////////////////////////////////////////////////////////////
SplatPGroup_cl::SplatPGroup_cl(int iParticleCount, const char *szTexturename, VIS_TransparencyType eTransp)
{
  // init the particle group
  Init(Vision::Game.GetContainerEntity(),1, iParticleCount);
  
  SetTexture(szTexturename);
  SetTransparencyType(eTransp);

  Particle_t *p = GetParticles();
  memset(p,0,sizeof(Particle_t)*iParticleCount);

  m_iCurrentValidCount = 0;
  m_iCachedFreeCount = 0;
}


//////////////////////////////////////////////////////////////////////
// GetFreeParticle : get a pointer to a free particle
//////////////////////////////////////////////////////////////////////
Particle_t * __fastcall SplatPGroup_cl::GetFreeParticle()
{
  VULPPROFILEFUNCTION(PROFILING_GETFREEPARTICLE);

  Particle_t *p = GetParticles();

  // still cached -> good luck
  if (m_iCachedFreeCount>0)
  {
    m_iCachedFreeCount--;
    int iIndex = m_iCachedFreeIndex[m_iCachedFreeCount];
    if (iIndex>=m_iCurrentValidCount)
      m_iCurrentValidCount = iIndex+1;
    return &p[iIndex];
  }

  // fill up cached list (slow!)
  int i;
  int count = GetNumOfParticles();
  for (i=0;i<count;i++,p++) if (!p->valid)
  {
    if (m_iCachedFreeCount<MAX_CACHED_COUNT)
        m_iCachedFreeIndex[m_iCachedFreeCount++] = i;
    else
    {
      if (i>=m_iCurrentValidCount)
        m_iCurrentValidCount = i+1;
      return p; // list is filled up
    }
  }

  if (m_iCachedFreeCount>0) 
    return GetFreeParticle();

  // no free particle at all
  return &g_dummyParticle;
}


//////////////////////////////////////////////////////////////////////
// ModSysNotifyFunctionHandle : tick function
//////////////////////////////////////////////////////////////////////
void SplatPGroup_cl::ModSysNotifyFunctionHandle()
{
  VULPPROFILEFUNCTION(PROFILING_PARTICLEHANDLING);
  float dtime = Vision::Timer.GetTimeDifference();
  int i;
  Particle_t *p = GetParticles();
  int count = __min(m_iCurrentValidCount,GetNumOfParticles());
  m_iCurrentValidCount = 0;

  for (i=0;i<count;i++,p++) if (p->valid)
  {
    m_iCurrentValidCount = i+1;

    // handle fade out
    p->velocity[0] -= dtime * p->velocity[1];
    if (p->velocity[0]<=0.f)
    {
      p->valid = 0;
      // add to cache list
      if (m_iCachedFreeCount<MAX_CACHED_COUNT)
        m_iCachedFreeIndex[m_iCachedFreeCount++] = i;
      continue;
    }
    // alpha fadeout
    p->color[3] = (UBYTE)(p->velocity[0]*p->velocity[2]);
    // increase size
    p->size += dtime * p->normal[0];
  } else
  {
    // add (at least the first one) to cache list
    if (m_iCachedFreeCount==0)
      m_iCachedFreeIndex[m_iCachedFreeCount++] = i;
  }

#ifdef _DEBUG
  // there must not be any active particle beyond our upper limit m_iCurrentValidCount
  p = GetParticles();
  const int iParticleCount = GetNumOfParticles();
  for (i=count;i<iParticleCount;i++)
    VASSERT(!p[i].valid)
#endif
}



///////////////////////////////////////////////////////////////////////////////////////
// InitNewFromParticle : initialises a particle from a source (rain) particle
///////////////////////////////////////////////////////////////////////////////////////
void SplatPGroup_cl::InitNewFromParticle(const Particle_t *pSrc, float fLifetime, float fSizeGrowth, float fAlpha)
{
  if (fAlpha<10.f) return;
  if (fAlpha>255.f) fAlpha=255.f;

  Particle_t *p = GetFreeParticle();

  p->valid = 1;
  // copy pos, size and color from source particle
  memcpy(&p->pos[0],&pSrc->pos[0],sizeof(p->pos));
  p->color = pSrc->color;
  p->size = pSrc->size * 2.f;
  p->usenormal = p->distorted = 0;

  // fade out
  p->velocity[0] = 1.f;           // start with 1.f
  p->velocity[1] = 1.f/fLifetime; // decrementor
  p->velocity[2] = fAlpha;        // initial alpha value
  p->color.a = (UBYTE)fAlpha;

  p->normal[0] = fSizeGrowth*WeatherPGroup_cl::g_fGlobalScale; // size increase speed
}

*/