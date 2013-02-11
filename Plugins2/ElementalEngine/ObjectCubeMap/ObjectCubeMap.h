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

#ifndef OBJECTCUBEMAPOBJECT_H
#define OBJECTCUBEMAPOBJECT_H


namespace ElementalEngine
{
#define SAFEDELETE( arg ) if( arg ){ delete arg; arg = NULL; }
#define SAFEARRAYDELETE( arg ) if ( arg ){ delete[] arg; arg = NULL; }


class ObjectCubeMap : public CObjectTemplate<IObject>
{
public:
	ObjectCubeMap(IHashString *parentName, IHashString *name, bool bAddToHier);
	~ObjectCubeMap();
	static IComponent *Create(int nArgs, va_list argptr);

	/// \brief	Initialize the object
	virtual void Init();
	/// \brief	The Update Function for the Object
	/// \return	True on sucess, False on faulure
	virtual bool Update();
	virtual void DeInit();

	/// \brief	Serialize this component to the archive.
	///	\param ar = reference to the archive class
	virtual void Serialize(IArchive &ar);

	DWORD OnSetCubemapData( DWORD size, void * params );

	DWORD OnGetModelFileName( DWORD size, void * params );

	DWORD GetHashImage( int cubedirection )
	{
		if( cubedirection >= 0 &&
			cubedirection < CUBEMAP_TOTAL_DIR )
		{
			return m_Images[ cubedirection ];
		}
		return 0;
	}
	void SetFaceData( int cubedirection, DWORD hashFile )
	{
		if( cubedirection >= 0 &&
			cubedirection < CUBEMAP_TOTAL_DIR )
		{
			m_Images[ cubedirection ] = hashFile;
		}
	}
	void SetTypeData( IHashString * type )
	{
		if( type )
		{
			m_Type.Init( type->GetString() );
		}
	}
	IHashString * GetModelFileHash()
	{
		return &m_Type;
	}
private:
	DWORD			m_Images[ CUBEMAP_TOTAL_DIR ];
	CHashString		m_Type;
};



} //namespace EE

#endif
