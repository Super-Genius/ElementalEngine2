///============================================================================
/// \file		HeightmapObject.h
/// \brief		Header file for a Heightmap Object
/// \date		06-29-2005
/// \author		D. Patrick Ghiocel
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

#ifndef	CHEIGHTMAPOBJECT_H
#define	CHEIGHTMAPOBJECT_H

class CTerrainManager;

class CHeightmapObject : public CObjectTemplate<IObject>
{
protected:
	/// \brief Default Constructor
	/// \param parentName = name of parent in hierarchy
	/// \param name = IHashString * name of this object
	CHeightmapObject(IHashString *parentName, IHashString *name);

public:
	/// \briefDefault Destructor
	~CHeightmapObject();

	/// \brief Create an instance
	/// \return	Pointer to this
	static IComponent *Create(int nArgs, va_list argptr);
	
	/// \brief Serializes the object
	/// \param ar - The Archiver
	virtual void Serialize(IArchive &ar);
	
	/// \brief Inits the object
	virtual void Init();

	/// \brief Deinits the object
	virtual void DeInit();

	/* *** MESSAGES *** */
	/// \brief Sets the Heightmap Data
	/// \param size - Size of Message
	/// \param in_params - Parameters
	/// \return Result of message process
	DWORD OnSetHeightmapData(DWORD size, void *in_params);
	/// \brief Gets the Heightmap Data
	/// \param size - Size of Message
	/// \param in_params - Parameters
	/// \return Result of message process
	DWORD OnGetHeightmapData(DWORD size, void *in_params);
	
private:
	CTerrainManager *m_pManager;
	int m_iWidth;
	int m_iHeight;
	float* m_pHeightmapData;
	
	
};

#endif // #define CHEIGHTMAPOBJECT_H