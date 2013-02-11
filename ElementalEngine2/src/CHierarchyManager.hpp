///=====================================================================
/// \file	CHierarchyManager.hpp
/// \brief	Definition of CHierarchyManager Class
///         This files defines a CHierarchyManager which is the interface
///			between Hierarchy Module and Red Phoenix.
///			Inherits from IManager
/// \date	03/12/2005
/// \author	Kenneth Hurley
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
///=====================================================================

#ifndef _CHIERARCHYMANAGER_HPP_
#define _CHIERARCHYMANAGER_HPP_

#include "CManager.h"
#include "tree.h"

using namespace TREEGEMS;

typedef tree<IObject *> OBJECTHIERARCHY;
typedef map< DWORD, OBJECTHIERARCHY *> NAMESUBTREEMAP;

class ElementalEngine::IVisitor;

class CHierarchyManager : public CManager
{
public:

	/// \brief The Default Destructor for CHierarchyManager
	/// \return void
	~CHierarchyManager( );

	/// \brief	Function to get this component.  This is a singleton
	/// \param	void
	/// \return	IComponent*: This Manager
	static IComponent* Create( int nArgs, va_list argptr );

	/// Add and IObject to the hierarchy
	/// overrides Manager addobject, group becomes parent
	/// \param Object - the object to add to the hierarchy
	/// \param parent - parent to add under, "" = root
	/// \return bool = true, if success, else false (parent not found)
	bool AddObject(IObject *Object, IHashString *parent);

	/// Insert an IObject before/after another object
	/// \param addObject - the object to add to the hierarchy
	/// \param sibling - sibling name to insert before/after
	/// \param before - bool for insert before or after (default)
	/// \return bool = true, if success, else false (before not found)
	bool InsertObject(IObject *addObject, IHashString *sibling, bool before = false);

	/// Remove the object from the hierarchy
	/// \param name = the name of the object to remove from the hierarchy
	/// \return bool - true, if found and removed, else false
	bool RemoveObject(IHashString *name);

	/// Uses a visitor pattern to visit the object in the hierarchy, performs recursive
	//	traversal until the visitor pattern stops or all the nodes are exhausted
	/// \param curNode = the name of the object to start traversing from
	/// \param visitorPattern = the visitor pattern to use
	/// \return bool - true, if found and traversed, else false
	bool VisitNode( OBJECTHIERARCHY * curNode, IVisitor * visitorPattern );

	/// send a message down the hierarchy until it is handled or
	/// the end of the hierarchy is reached
	/// \param size = the size of the data associated with *data
	/// \param data = void pointer to data specific to message
	///	\param curNode = pointer in the hiearchy to forward the messages to
	///	\param compType = name of component type NULL - means any type
	/// \param msgFunction = pointer to the message function handler
	/// \return DWORD = message return code, MSG_HANDLED_STOP, MSG_HANDLED_PROCEED, 
	///						error
	DWORD SendMessage(DWORD size, void *data,
		OBJECTHIERARCHY *curNode, IHashString *compType, 
		MSGFUNCTION msgFunction);

	// ----- Message Handlers, must be public ----- //
	/// message to visit hierarchy
	DWORD OnVisitHierarchy(DWORD size, void *params);
    
	/// Deletes an hierarchy Object and removes it from the heirarchy system
	/// \param Object is the IObject derived class pointer to delete
	/// \return true if success, otherwise false
	bool DeleteObject( IObject* Object );

	/// message to remove object
	/// \param size = siz of the data associated with *data
	/// \param data = void pointer to data specific message
	/// \return DWORD = message return code,  MSG_HANDLED_STOP, MSG_HANDLED_PROCEED, 
	///						error
	DWORD OnRemoveObject(DWORD size, void *data);

	/// message to remove all objects from the hierarchy
	/// \param size = siz of the data associated with *data
	/// \param data = void pointer to data specific message
	/// \return DWORD = message return code,  MSG_HANDLED_STOP, MSG_HANDLED_PROCEED, 
	///						error
	DWORD OnClearHierarchy(DWORD size, void *data);

	/// message received when renaming an object
	/// \param size = size of the data associated with *data
	/// \param data = void pointer CHANGEOBJNAME
	/// \return DWORD = message return code,  MSG_HANDLED_STOP, MSG_HANDLED_PROCEED, 
	///						error
	DWORD OnChangeObjectName(DWORD size, void *data);

	/// message received when changed an object's parent
	DWORD OnChangeObjectParent(DWORD size, void *data);

	/// message to change the currently active scene
	DWORD OnSetActiveScene(DWORD size, void* data);

	/// message to get the name of the currently active scene
	DWORD OnGetActiveScene(DWORD size, void* data);

protected:
	/// CHierarchyManager message pump handler.  
	/// Called through C++ callback
	/// \param msg = the ID of the message to send
	/// \param size = the size of the data associated with *data
	/// \param data = void pointer to data specific to message
	///	\param name = the name of the object to send the message to,
	///					if NULL, then compType cannot be null and all
	///					components of type compType will be called if they
	///					have message handler registered
	///	\param compType = name of component type NULL - means any type
	/// \return DWORD = message return code, MSG_HANDLED_STOP, MSG_HANDLED_PROCEED, 
	///						error
	virtual DWORD PumpHandler(DWORD size, void *data, 
		IHashString *name, IHashString *compType, MSGFUNCTION msgFunction);

	SINGLETONCONSTRUCTOROVERRIDE(CHierarchyManager);
	/// \brief The Default Constructor for CHierarchyManager
	/// \return void
	CHierarchyManager();

	// data memebers
private:
	/// Directed Acyclic Graph Tree for hierarchy
	OBJECTHIERARCHY m_HierarchyTree;

	/// map names to IObjects storage
	NAMESUBTREEMAP m_NameToSubTree;

	// root object for the hierarchy
	CHashString m_RootObjectName;
	IObject *m_RootObject;
	CHashString m_hszActiveSceneName;
};

#endif
