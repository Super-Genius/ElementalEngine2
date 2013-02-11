///============================================================================
/// \file	CAnimatedTexture.h
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

#ifndef CAnimatedTextureObject_H
#define CAnimatedTextureObject_H

#include "IAnimatedTexture.h"
#include "ITextureObject.h"
namespace ElementalEngine
{

struct UVRect
{
	float u1;
	float v1;
	float u2;
	float v2;
	UINT m_TextureIndex;
};

class CAnimatedTextureObject : public CObjectTemplate<IAnimatedTextureObject >
{	
private:
	/// \brief The Default Constructor for CMeshObject
	/// \param parentName = name of parent in hierarchy
	/// \param name = IHashString * name of this object
	CAnimatedTextureObject(IHashString *parentName, IHashString *name);

public:
	/// \brief	The Default Destructor for CMeshObject
	~CAnimatedTextureObject();

	/// \brief	Create the a instance of IObject
	/// \return	IComponent pointer
	static IComponent *Create(int nArgs, va_list argptr);

	/// \brief	The Initialization for the Object
	/// \param	IManager* pManager: Parent Manager
	virtual void Init();
	/// \brief	The DeInit Function for the Object
	virtual void DeInit();

	virtual bool IsDataLoaded() const { return (m_Texture.size() > 0); }

	/// \brief Loads a texture up
	/// \param file = the filename, relative texture pathNAME
	virtual bool LoadFromFile(  const TCHAR * file );

	/// \brief either reads or writes the entity information to a file
	virtual void Serialize(IArchive &ar);
	virtual void GenerateMips(){};
	virtual UINT GetLevelCount() { return 0; }
public:
	//Interface Functions:
	virtual IHashString *GetTextureName();

	virtual unsigned int GetReferenceCount();
	virtual unsigned int IncrementRefCount(){ m_RefCount++; return m_RefCount; }
	virtual unsigned int DecrementRefCount(){ m_RefCount--; return m_RefCount;}
	
	virtual UINT GetHeight();
	virtual UINT GetWidth();
	virtual UINT GetColorDepth();

	virtual void SetTextureName(IHashString *newName);
	virtual bool IsRenderTarget();
	
	virtual void* Read( UINT frame);
	virtual bool Write( UINT frame, void *p );
    virtual void * Lock( UINT frame, int * pitch );
	virtual void Unlock();
	virtual UINT GetNumFrames();
	virtual void GetFrame( UINT frame, float &u1, float &v1, float &u2, float &v2, ITextureObject **out  );

	virtual float GetFrameRate() { return m_fFrameRate; }
	//for DX9 adapters:
	void * GetAPITexture(){
		return NULL;
	}
	
	virtual TEXTURETYPE GetTextureType()
	{
		return EE_TEXTUREANIMATED2D;
	}

private:
	void BuildUVs();
	
	unsigned int    m_RefCount;
	CHashString     m_Filename;
	UINT m_Height;
	UINT m_Width;
	DWORD m_ColorDepth;
	UINT	m_NumFrames;
	UINT	m_NumTextures;
	vector< StdString > m_TextureFilenames;
	vector< ITextureObject *> m_Texture;
	vector< UVRect > m_FrameUVs;
	IRenderer * m_Renderer;
	float m_fFrameRate;
};

} //namespace EE

#endif