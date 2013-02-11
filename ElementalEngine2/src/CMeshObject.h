///==========================================================================
/// \file	CMeshObject.h
/// \brief	Implementation of CMeshObject interface for rendering
/// \date	7/11/2005
/// \author	Marvin Gouw
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
#ifndef	_CMESHOBJECT_H_
#define _CMESHOBJECT_H_

#include "IMeshObject.h"
#include "CStreamObject.h"
#include <list>
using namespace std;

typedef map< DWORD, CStreamObject > STREAMOBJECTMAP;
///==========================================================================
/// \class	CMeshObject
/// \brief	Concrete class for IMeshObject
///==========================================================================
class CMeshObject : public CObjectTemplate<IMeshObject>
{
private:
	/// \brief The Default Constructor for CMeshObject
	/// \param parentName = name of parent in hierarchy
	/// \param name = IHashString * name of this object
	CMeshObject(IHashString *parentName, IHashString *name);

public:
	/// \brief	The Default Destructor for CMeshObject
	~CMeshObject();

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

	virtual bool SetStreamData( void *pIn, int numElements,  CHANNELDESCRIPTORLIST &flagArray );
	/// \brief you are responsible for freeing pOut, use GlobalFree to do this!
	virtual MESHSTREAMDESC GetStreamData( void * pOut, CHANNELDESCRIPTORLIST &flagArray, int numElements= 0 );
	virtual void GetMeshDescription(  CHANNELDESCRIPTORLIST &ChannelDescsOut);
	
private:
	void  SetStream( DWORD streamname, void * pIn, int numElements, int nStride, int nOffset, int nTotalStride  );
	int  CheckValidGetFlags( int numElementsRequested, CHANNELDESCRIPTORLIST &flagArray );
private:
	STREAMOBJECTMAP m_StreamData;
};

#endif	// _CMESHOBJECT_H_