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

#ifndef CHModelManager_H
#define CHModelManager_H

#include "CManager.h"

class CHModelManager : public CManager
{

public:

	/// \brief The Default Destructor for CHModelManager
	/// \return void
	~CHModelManager( );

	/// \brief	Function to get this component.  This is a singleton
	/// \param	void
	/// \return	IComponent*: This Manager
	static IComponent* Create( int nArgs, va_list argptr );

	/// \brief	Initializes the CHModelManager
	/// \param	void
	/// \return	bool: Success
	virtual bool Init( void );

	/// \brief	Update function for the CHModelManager
	/// \return	void
	virtual void Update( DWORD tickCount );
	
	virtual IHashString* GetComponentType();

	/// These funcs overridden to keep a map to search through with the pump, versus iterating through the
	/// entire set in the cmanager
	virtual bool AddObject(IObject *Object, IHashString *group);
	virtual bool DeleteObject(IObject *Object);

	HierarchicalModelType * GetHierarchicalModelType(IHashString *hszModelName);

private:
	CHashString m_ManagerComponentType;

	SINGLETONCONSTRUCTOROVERRIDE( CHModelManager );
	/// \brief The Default Constructor for CHModelManager
	/// \return void
	CHModelManager();
	
};

#endif