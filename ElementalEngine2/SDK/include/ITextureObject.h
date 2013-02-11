///============================================================================
/// \file	ITextureObject.h
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

#ifndef ITEXTUREOBJECT_H
#define ITEXTUREOBJECT_H

namespace ElementalEngine
{

enum EE_CUBEMAPFACE
{ 
	CUBEMAP_FACE_X = 0,
	CUBEMAP_FACE_NEGATIVE_X = 1,
	CUBEMAP_FACE_Y = 2,
	CUBEMAP_FACE_NEGATIVE_Y = 3,
	CUBEMAP_FACE_Z = 4,
	CUBEMAP_FACE_NEGATIVE_Z = 5
};

enum TEXTURETYPE
{
	EE_TEXTURECUBE,
	EE_TEXTURE2D,
	EE_TEXTUREANIMATED2D,
};

class IBaseTextureObject: public IObject
{
public:
	virtual ~IBaseTextureObject() {};
	virtual IHashString * GetTextureName() = 0;
	virtual unsigned int GetReferenceCount() = 0;
	virtual unsigned int IncrementRefCount() = 0;
	virtual unsigned int DecrementRefCount() = 0;
	virtual bool LoadFromFile(  const TCHAR * file ) = 0;
	virtual UINT GetHeight() = 0;
	virtual UINT GetWidth() = 0;
	virtual UINT GetColorDepth() = 0;
	virtual void SetTextureName(IHashString *newName) = 0;
	virtual bool IsRenderTarget() = 0;
	virtual void * GetAPITexture() = 0;
	virtual void GenerateMips() = 0;
	virtual UINT GetLevelCount() = 0;
	virtual TEXTURETYPE GetTextureType() = 0;
	virtual bool IsDataLoaded() const = 0;
};

class ITextureObject: public IBaseTextureObject
{
public:
	virtual ~ITextureObject() {};
	virtual void* Read( int level = 0, bool bForceUpdateRead=false ) = 0;
	virtual int GetNumLevels() = 0;
	virtual bool Write( void * p, int level = 0, IHashString * informat = NULL  ) =  0;
	virtual void * Lock( int * pitch, int level = 0 ) = 0;
	virtual void Unlock( int level = 0 ) = 0;
	virtual bool MakeBlankTexture( UINT sizex, UINT sizey, UINT  colordepth, IHashString * format = NULL, UINT mips = 0) = 0;
	virtual bool MakeRenderTarget( UINT sizex, UINT sizey, UINT colordepth, DWORD type = 0, bool bAutoGenMipMaps=false ) = 0;
};

class ICubeTextureObject: public IBaseTextureObject
{
public:
	virtual ~ICubeTextureObject() {};
	virtual void* Read( EE_CUBEMAPFACE faceindex ) = 0;
	virtual bool Write( EE_CUBEMAPFACE faceindex, void *p ) = 0;
    virtual void * Lock( EE_CUBEMAPFACE faceindex ) = 0;
	virtual void Unlock( EE_CUBEMAPFACE faceindex ) = 0;
	virtual bool			 InitializeCubemap( const UINT squaresize, const UINT colordepth, bool bRenderTarget = false ) = 0;
	//virtual ITextureObject * GetCubemapFace( EE_CUBEMAPFACE faceindex ) = 0;
};

}	//namespace ElementalEngine

#endif