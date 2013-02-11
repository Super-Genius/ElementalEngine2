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

#ifndef CDX9TextureObject_H
#define CDX9TextureObject_H

#include "ITextureObject.h"

namespace ElementalEngine
{

class CDX9TextureObject : public CObjectTemplate<ITextureObject >
{	
private:
	/// \brief The Default Constructor for CMeshObject
	/// \param parentName = name of parent in hierarchy
	/// \param name = IHashString * name of this object
	CDX9TextureObject(IHashString *parentName, IHashString *name);

public:
	/// \brief	The Default Destructor for CMeshObject
	~CDX9TextureObject();

	/// \brief	Create the a instance of IObject
	/// \return	IComponent pointer
	static IComponent *Create(int nArgs, va_list argptr);

	/// \brief	The Initialization for the Object
	/// \param	IManager* pManager: Parent Manager
	virtual void Init();
	/// \brief	The DeInit Function for the Object
	virtual void DeInit();

	/// \brief Loads a texture up
	/// \param file = the filename, relative texture pathNAME
	virtual bool LoadFromFile(  const TCHAR * file );

	/// \brief either reads or writes the entity information to a file
	virtual void Serialize(IArchive &ar){};

	virtual bool IsDataLoaded() const { return (m_Texture != NULL); }
	virtual bool ReadyData();

	LPDIRECT3DSURFACE9 GetRenderTargetSurface();
	void ReleaseRenderTargetSurface();

public:
	//Interface Functions:
	virtual IHashString *GetTextureName();
	virtual void* Read( int level, bool bForceUpdateRead );
	virtual bool Write( void *p, int level ,IHashString * informat  );
    virtual void * Lock( INT * pitch, int level );
	virtual void Unlock( int level );
	virtual int GetNumLevels();

	virtual unsigned int GetReferenceCount();
	virtual unsigned int IncrementRefCount(){ m_RefCount++; return m_RefCount; }
	virtual unsigned int DecrementRefCount(){ m_RefCount--; return m_RefCount;}
	
	virtual UINT GetHeight();
	virtual UINT GetWidth();
	virtual UINT GetColorDepth();

	virtual bool MakeBlankTexture( UINT sizex, UINT sizey, UINT colordepth, IHashString * format, UINT mips);
	virtual bool MakeRenderTarget( UINT sizex, UINT sizey, UINT colordepth, DWORD type = RENDER_TARGET_COLOR_BUFFER, bool bAutoGenMipMaps=false );

	virtual void SetTextureName(IHashString *newName);
	virtual void GenerateMips();
	virtual UINT GetLevelCount();
	virtual bool IsRenderTarget();
	//for DX9 adapters:
	void * GetAPITexture()
	{
		ReadyData();
		return (void*)m_Texture;
	}
	virtual TEXTURETYPE GetTextureType()
	{
		return EE_TEXTURE2D;
	}

private:
	bool LoadHDRFile( TCHAR * file );
	LPDIRECT3DTEXTURE9 m_Texture;
	LPDIRECT3DSURFACE9 m_RenderTargetSurface;
	unsigned int    m_RefCount;
	CHashString     m_Filename;
	void *m_pLocalBuffer;
	int m_iLocalBufferSize;
	UINT m_Height;
	UINT m_Width;
	DWORD m_ColorDepth;
	bool m_bRenderTarget;
	bool m_Compressed;
	bool m_AutoGenMips;
	IRenderer * m_Renderer;
};

} //namespace EE

#endif