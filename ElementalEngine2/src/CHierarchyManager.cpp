///=====================================================================
/// \file	CHierarchyManager.cpp
/// \brief	Implements of CHierarchyManager Class
///         This file implements a CHierarchyManager which is the interface
///			between DAG Module and Red Phoenix.
///			Inherits from IManager
/// \date	03/12/05
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

#include "StdAfx.h"
#include "CHierarchyManager.hpp"
#include "IVisitor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define EXPOSEROOTNODE true

REGISTER_COMPONENT_SINGLETON(CHierarchyManager);

REGISTER_MESSAGE_HANDLER(VisitHierarchy, OnVisitHierarchy, CHierarchyManager);
REGISTER_MESSAGE_HANDLER(RemoveObject, OnRemoveObject, CHierarchyManager);
REGISTER_MESSAGE_HANDLER(ClearHierarchy, OnClearHierarchy, CHierarchyManager);
REGISTER_MESSAGE_HANDLER(ChangeObjectName, OnChangeObjectName, CHierarchyManager);
REGISTER_MESSAGE_HANDLER_PRIORITY(ChangeObjectParent, OnChangeObjectParent, CHierarchyManager, 0x70000000 );
REGISTER_MESSAGE_HANDLER(SetActiveScene, OnSetActiveScene, CHierarchyManager );
REGISTER_MESSAGE_HANDLER(GetActiveScene, OnGetActiveScene, CHierarchyManager );

/// \brief The Default Constructor for CHierarchyManager
/// \return void
CHierarchyManager::CHierarchyManager() : 
	CManager(_T("CHierarchyManager")), 
	m_RootObjectName(_T("Root")), 
	m_hszActiveSceneName(_T("World"))
{
	// create root object
	m_RootObject = dynamic_cast<IObject *>(new 
			OBJECTTEMPLATE(\0, CHierarchyRoot, IObject, NULL, &m_RootObjectName) );
	assert(m_RootObject);

	m_HierarchyTree.value = m_RootObject; 

	m_NameToSubTree[m_RootObject->GetName()->GetUniqueID()] = &m_HierarchyTree;

	// register our message pump and that we do handle message hierarchically
	m_ToolBox->RegisterMessagePump(this, 
		(static_cast< MSGPUMPFUNC > (&CHierarchyManager::PumpHandler)), HIERARCHYPRIORITY, true, false);
}

// send a message to the hierarchy until somebody handles it or
// the end of the hierarchy is reached
DWORD CHierarchyManager::SendMessage(DWORD size, void *data,
		OBJECTHIERARCHY *curNode, IHashString *compType, 
		MSGFUNCTION msgFunction)
{
	OBJECTHIERARCHY::child_iterator ohcIter;
	DWORD retVal, childRet, localRet;
	IComponent *destObject;
	OBJECTHIERARCHY *childNode;

	retVal = MSG_NOT_HANDLED;
	childRet = MSG_NOT_HANDLED;
	localRet = MSG_NOT_HANDLED;
	for (ohcIter = curNode->begin_child(); ohcIter != curNode->end_child(); 
			++ohcIter)
	{
		childNode = &*ohcIter;
		// move up to IComponent
		destObject = dynamic_cast<IComponent *>((*childNode).value);
		assert(destObject);
		// see if this object type name matches the type name passed in
		if ( destObject->IsKindOf( compType ))
		{
			// send the message to the handler
			localRet = (destObject->*msgFunction)(size, data);
			// return on error or MSG_HANDLED_STOP
			if ((localRet == MSG_ERROR) || (localRet == MSG_HANDLED_STOP))
			{
				return localRet;
			}

			if (retVal == MSG_NOT_HANDLED)
			{
				// if this guy is ever MSG_HANDLED_PROCEED, that's what we'll ultimately return
				// if it fails the check above
				retVal = localRet;
			}
		}	

		// recurse children if any
		if (childNode->degree() != 0)
		{
			childRet = SendMessage(size, data, childNode, compType, msgFunction);
			if ((childRet == MSG_ERROR) || (childRet == MSG_HANDLED_STOP))
			{
				return childRet;
			}

			if (retVal == MSG_NOT_HANDLED)
			{
				// if this guy is ever MSG_HANDLED_PROCEED, that's what we'll ultimately return
				// if it fails the check above
				retVal = childRet;
			}
		}
	}
	
	return retVal;
}

// hierarchy pump message function
DWORD CHierarchyManager::PumpHandler(DWORD size, void *data, 
		IHashString *name, IHashString *compType, MSGFUNCTION msgFunction)
{
	NAMESUBTREEMAP::iterator nsmIter;
	OBJECTHIERARCHY *curNode;
	IComponent *destObject;
	DWORD retVal;

	// default to not handled
	retVal = MSG_NOT_HANDLED;
	// if NULL then is the root
	if (name == NULL)
	{
		curNode = &m_HierarchyTree;
	}
	else
	{
		// map name to hierarchy
		nsmIter = m_NameToSubTree.find(name->GetUniqueID());
		// make sure we found it
		if (nsmIter == m_NameToSubTree.end())
		{
			// this object probably doesn't belong to us....
			return MSG_NOT_HANDLED;
		}
		else
		{
			curNode = nsmIter->second;
			// move up to IComponent
		}
	}

	destObject = dynamic_cast<IComponent *>((*curNode).value);
	assert(destObject);

	// see if this object with this name matches the type
	//if (*compType != *destObject->GetComponentType())
	//Marv: The previous logic implemented is to recurse into the child nodes if
	//the message can be sent to the parent directly. Is this correct?
	if ( !destObject->IsKindOf(compType) )
	{
		// recurse through children, until somebody handles it 
		// or end of children is reached
		retVal = SendMessage(size, data, curNode, compType, msgFunction);
	}
	else
	{
		// send the message to the handler
		retVal = (destObject->*msgFunction)(size, data);
		// send to any children of us with same compType
		if (retVal == MSG_HANDLED_PROCEED)
		{
			// recurse through children, until somebody handles it 
			// or end of children is reached
			retVal = SendMessage(size, data, curNode, compType, msgFunction);
		}

	}

	return retVal;
}

/// \brief	Function to get this component.  This is a singleton
/// \param	void
/// \return	IComponent*: This Manager
IComponent *CHierarchyManager::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE(CHierarchyManager);
}

/// \brief The Default Destructor for CHierarchyManager
/// \return void
CHierarchyManager::~CHierarchyManager( )
{
	delete m_RootObject;

	// dump our message pump handler registration
	m_ToolBox->UnregisterMessagePump(this, HIERARCHYPRIORITY);

	m_ToolBox->UnRegisterComponent( GetComponentType() );
}

// add the object to the hierarchy
bool CHierarchyManager::AddObject(IObject *Object, IHashString *parent)
{
	NAMESUBTREEMAP::iterator nsmIter;
	OBJECTHIERARCHY *parentNode;
	OBJECTHIERARCHY *curNode;
	IHashString *theName;

	//if (parent != NULL)
	if( Object->GetParentName() != NULL ) 
	{
		// map parents name to IObject
		nsmIter = m_NameToSubTree.find(parent->GetUniqueID());
		// make sure we found it
		if (nsmIter == m_NameToSubTree.end())
		{
			EngineGetToolBox()->SetErrorValue(ERR_PARENT_NOT_FOUND);
			EngineGetToolBox()->Log(LOGERROR, _T("Unable to find parent %s\n"), 
				parent->GetString());
			return false;
		}
		parentNode = nsmIter->second;
	}
	else
	{
		parentNode = &m_HierarchyTree;
		Object->SetParentName( parentNode->value->GetName() );
	}

	// make sure we have a name for our object
	theName = Object->GetName();
	if (theName == NULL)
	{
		EngineGetToolBox()->SetErrorValue(ERR_NAME_NOT_FOUND);
		EngineGetToolBox()->Log(LOGERROR, _T("Object must be uniquely named\n"));
		return false;
	}

	// put it under our children
	parentNode->push_back(Object);
	curNode = parentNode->last_child();

	m_NameToSubTree[theName->GetUniqueID()] = curNode;

	return true;
}

// remove an object from the hierarchy
bool CHierarchyManager::RemoveObject(IHashString *name)
{
	NAMESUBTREEMAP::iterator nsmIter;
	OBJECTHIERARCHY *curNode;
	DELETEOBJECTPARAMS dop;

	// map name to hierarchy
	nsmIter = m_NameToSubTree.find(name->GetUniqueID());
	// make sure we found it, parent delete may have already removed us
	if (nsmIter == m_NameToSubTree.end())
	{
		return false;
	}

	curNode = nsmIter->second;

	// if curNode is the root (i.e. no parent)
	if (curNode == &m_HierarchyTree)
	{
		return false;
	}
	else
	{
		OBJECTHIERARCHY::child_iterator ohcIter;
		// delete all children of this node first
		ohcIter = curNode->begin_child();
		while (ohcIter != curNode->end_child())
		{
			IObject *obj;
			obj = ohcIter->value;
			dop.name = obj->GetName();
			static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof(DELETEOBJECTPARAMS), &dop, NULL, NULL);
			ohcIter = curNode->begin_child();
			
		}

		// remove map from object name to tree node ptr
		m_NameToSubTree.erase( nsmIter );
		// remove node and it's sub tree 
		m_HierarchyTree.erase( curNode );
	}
    
	return true;
}

// insert an IObject either before or after another object
bool CHierarchyManager::InsertObject(IObject *addObject, 
										IHashString *sibling, bool before)
{
	NAMESUBTREEMAP::iterator nsmIter;
	OBJECTHIERARCHY *siblingNode;
	OBJECTHIERARCHY *curNode;
	IHashString *theName;

	if (sibling == NULL)
	{
		EngineGetToolBox()->SetErrorValue(ERR_SIBLING_NOT_FOUND);
		EngineGetToolBox()->Log(LOGERROR, _T("Sibling name cannot be NULL\n"));
		return false;
	}

	// map siblings name to IObject
	nsmIter = m_NameToSubTree.find(sibling->GetUniqueID());
	// make sure we found it
	if (nsmIter == m_NameToSubTree.end())
	{
		EngineGetToolBox()->SetErrorValue(ERR_SIBLING_NOT_FOUND);
		EngineGetToolBox()->Log(LOGERROR, _T("Unable to find sibling %s\n"), 
			sibling->GetString());
		return false;
	}

	siblingNode = nsmIter->second;

	// make sure we have a name for our object
	theName = addObject->GetName();
	if (theName == NULL)
	{
		EngineGetToolBox()->SetErrorValue(ERR_NAME_NOT_FOUND);
		EngineGetToolBox()->Log(LOGERROR, _T("Object must be uniquely named\n"));
		return false;
	}

	if (!before)
	{
		// grab next sibling
		siblingNode = siblingNode->next_sibling();
	}

	curNode = OBJECTHIERARCHY::create_subtree(addObject);
	// insert it now
	siblingNode->insert_subtree(curNode, siblingNode);

	m_NameToSubTree[theName->GetUniqueID()] = curNode;

	return true;
}

// visit a node in the hierarchy
bool CHierarchyManager::VisitNode(OBJECTHIERARCHY * curNode, IVisitor * visitorPattern)
{		
	OBJECTHIERARCHY::child_iterator ohcIter;
	IComponent *destObject;
	OBJECTHIERARCHY *childNode;


	// start from node and visit it also
	if (curNode == NULL)
	{
		return false;
	}


#ifndef EXPOSEROOTNODE
	if (curNode != &m_HierarchyTree)
	{
#endif
		// Get the component base and visit it
		destObject = dynamic_cast<IComponent *>((*curNode).value);
		assert(destObject);
		// Apply the visitor pattern to this component
		if (!visitorPattern->Visit( destObject, true ))
			return false;
#ifndef EXPOSEROOTNODE
	}
#endif
	// recurse children if any
	for (ohcIter = curNode->begin_child(); ohcIter != curNode->end_child(); 
				ohcIter++)
	{
		childNode = &*ohcIter;
		//Visit all of its children since Visit() to the parent was successful
		VisitNode(childNode, visitorPattern);
	}

#ifndef EXPOSEROOTNODE
	if (curNode != &m_HierarchyTree)
	{
#endif
		if ( visitorPattern->IsHierarchicalVisitor() )
		{
			if( !visitorPattern->Visit( destObject, false ) )
			{
				return false;
			}
		}
#ifndef EXPOSEROOTNODE
	}
#endif

	// keep recursing
	return true;
}

// visit the hierarchy starting at node name.
DWORD CHierarchyManager::OnVisitHierarchy(DWORD size, void *params)
{
	VISITHIERARCHYDATA *vhd;
	OBJECTHIERARCHY *startNode;
	NAMESUBTREEMAP::iterator nsmIter;

	VERIFY_MESSAGE_SIZE(sizeof(VISITHIERARCHYDATA), size);

	vhd = (VISITHIERARCHYDATA *)params;
	IHashString *rootNodeName = vhd->rootNodeName;

	if (rootNodeName == NULL)
	{
		rootNodeName = &m_hszActiveSceneName;
	}

	// map name to hierarchy
	nsmIter = m_NameToSubTree.find(rootNodeName->GetUniqueID());
	// make sure we found it, parent delete may have already removed us
	if (nsmIter == m_NameToSubTree.end())
	{
		EngineGetToolBox()->Log(LOGERROR, _T("VisitHierarchy: Root object not found in hierarchy!\n"));
		return MSG_ERROR;
	}
	startNode = nsmIter->second;

	if (vhd->name != NULL)
	{
		// map name to hierarchy
		nsmIter = m_NameToSubTree.find(vhd->name->GetUniqueID());
		// make sure we found it
		if (nsmIter == m_NameToSubTree.end())
		{
			EngineGetToolBox()->SetErrorValue(WARN_NAME_NOT_FOUND);
			EngineGetToolBox()->Log(LOGWARNING, _T("Unable to find object to traverse %s\n"), 
				vhd->name->GetString());
			return MSG_WARNING;
		}

		// grab the heirarchy node
		startNode = nsmIter->second;
	}

	VisitNode(startNode, vhd->visitor);

	return MSG_HANDLED_PROCEED;
}

// remove an object specified
DWORD CHierarchyManager::OnRemoveObject(DWORD size, void *data)
{
	VERIFY_MESSAGE_SIZE(sizeof(IHashString*), size);

	RemoveObject((IHashString *)data);

	return MSG_HANDLED_PROCEED;
}

DWORD CHierarchyManager::OnClearHierarchy(DWORD size, void* data)
{
	DELETEOBJECTPARAMS dop;
	NAMESUBTREEMAP::iterator nsmIter;
	nsmIter = m_NameToSubTree.begin();
	while( m_NameToSubTree.size() > 1 )
	{
		if( nsmIter->second != &m_HierarchyTree )
		{
			CHashString hszObjDeleteName(m_ToolBox->GetHashString(nsmIter->first));
			dop.name = &hszObjDeleteName;
			static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof(DELETEOBJECTPARAMS), &dop, NULL, NULL);
			nsmIter = m_NameToSubTree.begin();
		}
		else
		{
			nsmIter++;
		}
	}
		
	return MSG_HANDLED_PROCEED;
}

// come here when an object changes names
DWORD CHierarchyManager::OnChangeObjectName(DWORD size, void* data)
{
	NAMESUBTREEMAP::iterator nsmIter;
	NAMESUBTREEMAP::iterator nsmNewNameIter;
	CHANGEOBJNAME *ncp;

	VERIFY_MESSAGE_SIZE(sizeof(CHANGEOBJNAME), size);

	ncp = (CHANGEOBJNAME *)data;
	
	// map name to hierarchy
	nsmIter = m_NameToSubTree.find(ncp->oldName->GetUniqueID());
	// make sure we found it, parent delete may have already removed us
	if (nsmIter == m_NameToSubTree.end())
	{
		return MSG_NOT_HANDLED;
	}

	// make sure we don't already have an entry by this name
	nsmNewNameIter = m_NameToSubTree.find(ncp->newName->GetUniqueID());
	if (nsmNewNameIter != m_NameToSubTree.end())
	{
		EngineGetToolBox()->SetErrorValue(WARN_NAME_ALREADY_EXISTS);
		EngineGetToolBox()->Log(LOGWARNING, _T("Unable to rename object %s to %s, already exists\n"), 
			ncp->oldName->GetString(), ncp->newName->GetString());
		return MSG_WARNING;
	}

	OBJECTHIERARCHY *renamedNode = nsmIter->second;
	m_NameToSubTree[ncp->newName->GetUniqueID()] = renamedNode;
	m_NameToSubTree.erase(nsmIter);

	// rename the parent of all the renamed node's children
	IObject *destObject;
	OBJECTHIERARCHY *childNode;
	OBJECTHIERARCHY::child_iterator ohcIter;
	for (ohcIter = renamedNode->begin_child(); ohcIter != renamedNode->end_child(); 
			ohcIter++)
	{
		childNode = &*ohcIter;
		// try to move up to IObject
		destObject = dynamic_cast<IObject*>((*childNode).value);
		if (destObject)
		{
			destObject->SetParentName( ncp->newName );
		}
	}

	return MSG_HANDLED_PROCEED;
	
}

DWORD CHierarchyManager::OnChangeObjectParent(DWORD size, void* data)
{
//	assert( false ); // UNDONE: this function doesn't work yet, still buggy. Do not use!
//	return MSG_ERROR;

    VERIFY_MESSAGE_SIZE(sizeof(CHANGEOBJECTPARENT), size);
    CHANGEOBJECTPARENT *pParams = (CHANGEOBJECTPARENT*)data;

    NAMESUBTREEMAP::iterator objectItr, parentItr;

    objectItr = m_NameToSubTree.find(pParams->objectName->GetUniqueID());
    parentItr = m_NameToSubTree.find(pParams->newParentName->GetUniqueID());

    // both the object and the new parent in hierarchy must exist
    if (objectItr == m_NameToSubTree.end() || parentItr == m_NameToSubTree.end())
        return MSG_ERROR;

	OBJECTHIERARCHY::TreeT *objectNode = objectItr->second;
    OBJECTHIERARCHY::TreeT *parentNode = parentItr->second;

	// new parent can't be a child of the object
    if (parentNode->is_descendant_of(*objectNode))
        return MSG_ERROR;

    // object cannot be root
    if (objectNode->is_root())
        return MSG_ERROR;

    // object must be IObject type
    IObject *pObject = dynamic_cast<IObject*>(objectNode->value);
    if (!pObject)
        return MSG_ERROR;

	// object cannot alread by child of new parent
	if (*pObject->GetParentName() == *pParams->newParentName)
		return MSG_ERROR;

    // splice object and children from the hierarchy
    OBJECTHIERARCHY tempTree( *objectNode );
	m_HierarchyTree.erase( objectNode );
    // remove map from object name to tree node ptr
    m_NameToSubTree.erase( objectItr );

    // change object's parent
    pObject->SetParentName( pParams->newParentName );

	// add object back to hierarchy
    OBJECTHIERARCHY *curNode;
    parentNode->push_back( tempTree );
    curNode = parentNode->last_child();
	assert( *curNode->value->GetName() == *pParams->objectName );

	// Restore map of IHashStringID to Node ptr:
	OBJECTHIERARCHY::iterator itrNode = curNode->begin();
	while (itrNode != curNode->end())
	{
		DWORD nodeID = itrNode->value->GetName()->GetUniqueID();
		m_NameToSubTree[nodeID] = &*itrNode;
		itrNode++;
	}

	return MSG_HANDLED_PROCEED;
}

// Deletes an hierarchy Object and removes it from the heirarchy system
bool CHierarchyManager::DeleteObject( IObject* Object )
{
	assert(Object);
	// remove from hierarchy tree, including children
	RemoveObject(Object->GetName());

	return true;
}

DWORD CHierarchyManager::OnSetActiveScene(DWORD size, void* data)
{
	VERIFY_MESSAGE_SIZE(sizeof(GETSETACTIVESCENE), size);
	GETSETACTIVESCENE *pParams = (GETSETACTIVESCENE*)data;

	assert( pParams->pSceneName != NULL );
	if (pParams->pSceneName == NULL)
		return MSG_ERROR;

	m_hszActiveSceneName = pParams->pSceneName;
	return MSG_HANDLED_PROCEED;
}

DWORD CHierarchyManager::OnGetActiveScene(DWORD size, void* data)
{
	VERIFY_MESSAGE_SIZE(sizeof(GETSETACTIVESCENE), size);
	GETSETACTIVESCENE *pParams = (GETSETACTIVESCENE*)data;

	pParams->pSceneName = &m_hszActiveSceneName;

	return MSG_HANDLED_PROCEED;
}
