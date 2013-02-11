///==========================================================================
/// \file	LightMapObject.h
/// \brief	Implementation of LightMapObject interface for rendering
/// \date	3/9/2005
/// \author	Marvin Gouw
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
///==========================================================================

#ifndef	_LightMapObject_H_
#define _LightMapObject_H_

using namespace std;

struct BYTECOLOR
{
	BYTE r, g, b, a;
};
namespace ElementalEngine
{

///==========================================================================
/// \class	LightMapObject
/// \brief	Adapter for Cal 3d models
///==========================================================================
class LightMapObject : public CObjectTemplate< IObject>
{
private:
	/// \brief The Default Constructor for LightMapObject
	/// \param parentName = name of parent in hierarchy
	/// \param name = IHashString * name of this object
	LightMapObject(IHashString *parentName, IHashString *name, bool bAddToHier);

public:
	/// \brief	The Default Destructor for LightMapObject
	~LightMapObject();

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
	
	/// \brief either reads or writes the entity information to a file
	/// Currently Loads up the entity completely if reading
	virtual void Serialize(IArchive &ar);

	virtual bool IsKindOf( IHashString * compType );

	DWORD OnAddObject( DWORD size, void * params );
	DWORD OnGenerateLightmaps( DWORD size, void * params );
	DWORD OnSetLightmapOutputDirectory( DWORD size, void * params );
private:
	void NormalizeTexture( BYTECOLOR * buffer, floatColor * texels , float fHalfBloomRange = 4.f);
	void BlurTextureX( BYTECOLOR * buffer,  int extent );
	void BlurTextureY( BYTECOLOR * buffer,  int extent );
	void AddTextureToTexture( BYTECOLOR * dest, BYTECOLOR * src );
	//resize
	void SetTextureSize( int texsize )//square texture
	{
		m_SquaredTexSize = texsize*texsize;
		if( m_SquaredTexSize > m_BufferSize )
		{
			if( m_Texdata ){ delete[] m_Texdata; }
			if( m_TempBuffer ){ delete[] m_TempBuffer; }
			if( m_IndirectData ){ delete[] m_IndirectData; }
			m_BufferSize = m_SquaredTexSize;
			m_Texdata = new BYTECOLOR[ m_BufferSize ];
			m_TempBuffer = new BYTECOLOR[ m_BufferSize ];
			m_IndirectData = new BYTECOLOR[ m_BufferSize ];
		}
		m_TexSize = texsize;
	}

	bool m_bAddToHierarchy;
	IRenderer * m_IRenderer;
	
	map< DWORD, MeshParameterization * > m_MeshParams;
	map< DWORD, ITextureObject * > m_TextureObjects;
	LightMapGenerator * m_LightGen;
	map< DWORD, LightmapCache > m_UVCache;
	map< DWORD, DWORD > m_GlobalMeshCache;
	BYTECOLOR * m_Texdata;
	BYTECOLOR * m_IndirectData;
	BYTECOLOR *	m_TempBuffer;
	int		m_TexSize;
	int		m_SquaredTexSize;
	int		m_BufferSize;
	StdString	m_OutputDirectory;
};

} // namespace EE

#endif	// LightMapObjectOBJECT_HPP
