///==========================================================================
/// \file	PrecacheManager.h
/// \brief	Declaration of Precache Manager
/// \date	12/06/2007
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

#ifndef	PRECACHEMANAGER_H
#define	PRECACHEMANAGER_H

#include "CManager.h"

class CPrecacheObject;

/// Defines the variables for a resource
typedef struct RESOURCEINFO
{
	bool m_bLoaded;
	unsigned int m_iRefCount;
		
	RESOURCEINFO()
	{
		m_bLoaded = false;
		m_iRefCount = 0;
	}

} RESOURCEINFO;

/// Maps a resource by file name to its variables
typedef map<DWORD, RESOURCEINFO> RESOURCEMAP;

/// Defines if a particular extension is precachable
typedef struct EXTENSIONINFO
{
	bool m_bIsPrecachable;
	CACHABLEQUERYCALLBACK m_pfnCallback;

	EXTENSIONINFO()
	{
		m_bIsPrecachable = false;
		m_pfnCallback = NULL;
	}
} EXTENSIONINFO;

/// Map an extension to its precachable state
typedef map<DWORD, EXTENSIONINFO> EXTENSIONMAP;

/// Stack of PrecacheObjects
typedef list< CPrecacheObject* > PRECACHESTACK;

class CPrecacheManager : public CManager
{
private:
	SINGLETONCONSTRUCTOROVERRIDE(CPrecacheManager);
	/// \brief The Default Constructor
	/// \return void
	CPrecacheManager();
public:
	/// \brief The Default Deconstructor
	/// \return void
	virtual ~CPrecacheManager();

	/// \brief	Function to get this component.  This is a singleton
	/// \param	void
	/// \return	IComponent*: This Manager
	static IComponent *Create(int nArgs, va_list argptr);

	virtual bool Init();
	virtual bool DeInit();
	virtual void Update(DWORD tickCount);

	virtual bool AddObject( IObject *Object, IHashString *group);
	virtual bool DeleteObject( IObject* Object );

	bool CheckExtension( StdString szFileName );
	void CreatePrecacheObject( IHashString* pFileName );

	// ------------------ Messages ------------------ //
	///	\brief	Start the manager
	///	\param	size = 0
	///	\param	data = void pointer to NULL
	///	\return DWROD = message return code, MSG_HANDLED_STOP, MSG_HANDLED_PROCEED,
	///						error
	DWORD OnStart(DWORD size, void *data);

	///	\brief	Stop the manager
	///	\param	size = 0
	///	\param	data = void pointer to NULL
	///	\return DWROD = message return code, MSG_HANDLED_STOP, MSG_HANDLED_PROCEED,
	///						error
	DWORD OnStop(DWORD size, void *data);


	///	\brief	Handles extension registration
	///	\param	size = 0
	///	\param	data = void pointer to NULL
	///	\return DWROD = message return code, MSG_HANDLED_STOP, MSG_HANDLED_PROCEED,
	DWORD OnLoadFile(DWORD size, void *data);

	///	\brief	Handles extension registration
	///	\param	size = 0
	///	\param	data = void pointer to NULL
	///	\return DWROD = message return code, MSG_HANDLED_STOP, MSG_HANDLED_PROCEED,
	DWORD OnRegisterLoadSave(DWORD size, void *data);

	///	\brief	Adds a Precache list to the queue
	///	\param	size = 0
	///	\param	data = void pointer to NULL
	///	\return DWROD = message return code, MSG_HANDLED_STOP, MSG_HANDLED_PROCEED,
	DWORD OnAddPrecache(DWORD size, void *data);

	///	\brief	Removes a Precache list from the queue
	///	\param	size = 0
	///	\param	data = void pointer to NULL
	///	\return DWROD = message return code, MSG_HANDLED_STOP, MSG_HANDLED_PROCEED,
	DWORD OnRemovePrecache(DWORD size, void *data);

	///	\brief	Updates with all precache changes
	///	\param	size = 0
	///	\param	data = void pointer to NULL
	///	\return DWROD = message return code, MSG_HANDLED_STOP, MSG_HANDLED_PROCEED,
	DWORD OnCommitPrecache(DWORD size, void *data);

private:
	/// CPrecacheManager running indicator
	bool m_bIsRunning;
	
	/// Map of resources by file name to variables
	RESOURCEMAP m_ResourceMap;

	/// Map an extensions to precachable state
	EXTENSIONMAP m_ExtensionMap;

	/// Maps of Precache Files (DWORD) to PrecacheClass
	PRECACHESTACK m_PrecacheStack;

	float m_fLoadingUpdateFPS;
	
};

#endif