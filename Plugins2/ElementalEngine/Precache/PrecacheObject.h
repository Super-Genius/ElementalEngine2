///==========================================================================
/// \file	Precache.h
/// \brief	Implementation of Precache Resource File
/// \date	12/18/2007
/// \author	Patrick Ghiocel
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
///==========================================================================

#ifndef	PRECACHE_H
#define	PRECACHE_H

// Set of Filename listings
typedef set< DWORD > RESOURCESET;

class CPrecacheObject: public CObjectTemplate<IObject>
{
protected:
	/// Default Constructor
	/// \param parentName = name of parent in hierarchy
	/// \param name = IHashString * name of this object
	CPrecacheObject(IHashString *parentName, IHashString *name, bool bAddToHier);

public:
	virtual ~CPrecacheObject();

	/// \brief Create an instance
	/// \return	Pointer to this
	static IComponent *Create(int nArgs, va_list argptr);

	/// \brief	Initialize the object
	void Init( void );

	/// \brief	Update the object
	/// \return success
	bool Update( void );

	/// \brief	Deinitialize the object
	void DeInit( void );

	/// \brief Serialize function
	void Serialize( IArchive &ar );

	/// \brief Add a Resource to this precache
	void AddResource( IHashString *filename );

	/// \brief Remove a Resource to this precache
	void RemoveResource( IHashString *filename );

	/// \brief Returns point to list of all resources to this precache
	RESOURCESET* GetResourceList( void );

private:
	/// point to the EE toolbox
	IToolBox* m_ToolBox;

	/// Version of Precache
	unsigned int m_iVersion;

	/// Resource List of Precache
	RESOURCESET m_ResourceSet;
};

#endif //PRECACHE_H