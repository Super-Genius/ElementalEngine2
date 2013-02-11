///============================================================================
/// \file	CDX9CubeTextureObject.h
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

#ifndef CDX9CubeTextureObject_H
#define CDX9CubeTextureObject_H

#include "ITextureObject.h"

#define NUM_CUBE_FACES 6

namespace ElementalEngine
{

class CDX9CubeTextureObject : public CObjectTemplate<ICubeTextureObject >
{	
private:
	/// \brief The Default Constructor for CMeshObject
	/// \param parentName = name of parent in hierarchy
	/// \param name = IHashString * name of this object
	CDX9CubeTextureObject(IHashString *parentName, IHashString *name);

public:
	/// \brief	The Default Destructor for CMeshObject
	~CDX9CubeTextureObject();

	/// \brief	Create the a instance of IObject
	/// \return	IComponent pointer
	static IComponent *Create(int nArgs, va_list argptr);

	/// \brief	The Initialization for the Object
	/// \param	IManager* pManager: Parent Manager
	virtual void Init();
	/// \brief	The Update Function for the Object
	/// \return	True on sucess, False on faulure
	virtual bool Update();
	/// \brief	The DeInit Function for the Object
	virtual void DeInit();

	/// \brief Loads a texture up
	/// \param file = the filename, relative texture pathNAME
	virtual bool LoadFromFile(  const TCHAR * file );

	/// \brief either reads or writes the entity information to a file
	virtual void Serialize(IArchive &ar){};

	virtual bool IsDataLoaded() const { return (m_Texture != NULL); }

	void * GetAPITexture()
	{
		return (void*)m_Texture;
	}
public:
	//Interface Functions:
	virtual IHashString *GetTextureName();
	virtual void* Read( EE_CUBEMAPFACE faceindex );
	virtual bool Write( EE_CUBEMAPFACE faceindex, void *p );
    virtual void * Lock( EE_CUBEMAPFACE faceindex );
	virtual void Unlock( EE_CUBEMAPFACE faceindex );

	virtual unsigned int GetReferenceCount();
	virtual unsigned int IncrementRefCount(){ m_RefCount++; return m_RefCount; }
	virtual unsigned int DecrementRefCount(){ m_RefCount--; return m_RefCount;}
	
	virtual UINT GetHeight();
	virtual UINT GetWidth();
	virtual UINT GetColorDepth();

	virtual void				SetTextureName(IHashString *newName);
	virtual bool				InitializeCubemap( const UINT squaresize, const UINT colordepth, bool bRenderTarget );
	virtual bool				IsRenderTarget();
	
	virtual TEXTURETYPE GetTextureType()
	{
		return EE_TEXTURECUBE;
	}
	virtual void GenerateMips(){}
	virtual UINT GetLevelCount() { return 0; }
private:	
	/// converts the cube map face flag for D3D to use from the EE flag
	D3DCUBEMAP_FACES GetD3DCubeFaceEnum( EE_CUBEMAPFACE faceindex );
private:
	LPDIRECT3DCUBETEXTURE9  m_Texture;
	void * m_pLocalBuffer[ NUM_CUBE_FACES ];
	UINT			m_Size;
	UINT			m_ColorDepth;
	unsigned int    m_RefCount;
	CHashString     m_Filename;
	int				m_iLocalBufferSize;
	bool			m_bRenderTarget;
	IRenderer	*	m_Renderer;
};

}//namespace ee
#endif