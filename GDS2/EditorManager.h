///============================================================================
/// \file	EditorManager.h
/// \brief	Registers editor plugins
/// \date	5-19-2005
/// \author	Kyle Swaim
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

#ifndef EDITORMANAGER_H
#define EDITORMANAGER_H

#include "StdAfx.h"


/// \brief	A map between editors and the objects they are registered to
typedef multimap<DWORD, DWORD> EDITORMAP;

/// \brief  A map between tools and pickable objects they are registered to.
typedef multimap<DWORD, DWORD> TOOLMAP;

/// \brief	A miltimap mapping the allowable links between objects
typedef multimap<DWORD, DWORD> LINKMAP;
typedef pair<DWORD, DWORD> LINKPAIR;

///============================================================================
/// \class	CEditorManager
/// \brief	Registers editor plugins and manages assiociations between objects
///			and the editors.
///============================================================================
class CEditorManager : public IComponent
{
private:
	/// \brief	Use the singleton template for this class
	SINGLETONCONSTRUCTOROVERRIDE( CEditorManager );
public:
	/// \brief	Constructor
	///			Private since this class is using the singleton template
	CEditorManager();

public:
	/// \brief	Destructor
	~CEditorManager();

	/// \brief	Serialize this class
	///			Nothing to serialize in this class
	/// \param	ar - the archive for serialization
	virtual void Serialize( IArchive &ar ) {};

	/// \brief	return a string of the component type
	/// \return a reference to this class's textual classname
	virtual IHashString *GetComponentType();

	/// \brief	Checks if this is similar to it's base class's component
	/// \param	compType - the type to compare this against
	/// \return	true if it is simlar, else false
	virtual bool IsKindOf( IHashString *compType );

	/// \brief	Get a singleton instance of this class
	/// \return	a singleton instance of this class
	static IComponent *Create( int nArgs, va_list argptr );

	/// \brief	Look to see if a link is valid
	/// \param	src the object linking uniqueID to another lobject
	/// \param	dst the object unique ID being linked to
	/// \return	true if valid, false if not valid
	bool IsLinkValid( DWORD src, DWORD dst );

	/// Enumerate the register editors and send a message
	/// \param	compType = the component type that has editors registered
	/// \param	mess = pointer to string of message
	/// \param	size = size for the message
	/// \param	params = parameter data (structure) for message
	/// \return message value, HANDLED, NOT_HANDLED, ERROR, etc.
	DWORD EnumEditorsMessage(IHashString *compType, DWORD msg, DWORD size, void *params);

	/// Enumerate the tools register and send message to appropriate tools
	/// \param  objectCompType = the pickable object component type
	/// \param  msg = message ID
	/// \param  size = size for the message
	/// \param  params = parameter data (structure) for message
	/// \return message value, HANDLED, NOT_HANDLED, ERROR, etc.
	DWORD CEditorManager::EnumToolsMessage(IHashString *objectCompType, DWORD msg, DWORD size, void *params);


	// ------------------ Messages ------------------ //

	/// \brief	Register an object with an editor
	/// \param	size - the size of the message
	/// \param	params - the message
	/// \return	return message
	DWORD OnRegisterEditor( DWORD size, void *params );

	/// \brief  Register pickable object with an tool. The pickable object
	///         usually a helper object only and not exists in hierarchy. Used
	///         only for some tool activation.
	/// \param  size - the size of the message
	/// \param  params - the message
	/// \return return message
	DWORD OnRegisterTool( DWORD size, void *params );

	/// \brief	Route the LeftClickView message to the specific manager/object
	/// \param	size - the size of the message
	/// \param	params - the message
	/// \return	return message
	DWORD OnLeftClickView( DWORD size, void *params );

	/// \brief	Route the LeftClickObject message to the specific manager/object
	/// \param	size - the size of the message
	/// \param	params - the message
	/// \return	return message
	DWORD OnLeftClickObject( DWORD size, void *params );

	/// \brief	Route the OnClick message to the specific manager/object
	/// \param	size - the size of the message
	/// \param	params - the message
	/// \return	return message
	DWORD OnDblClick( DWORD size, void *params );

	/// \brief	Given an Object type return the list of objects that
	///			it can be linked to. If no object is given default
	///			to returning objects that can be linked to the world
	///			object.
	/// \param	size - the size of the message
	/// \param	params - the message
	/// \return	return message
	DWORD OnGetLinkableObjects( DWORD size, void *params );

	/// Sets the collision groups used for object picking
	/// \param	size - the size of the message
	/// \param	params - the message
	/// \return	return message
	DWORD OnSetPickingGroups( DWORD size, void *params );

private:

	/// \brief	Populate the LINKMAP from an XML file
	/// \param	fileName - The name of the XML file to load
	void LoadLinkMap( StdString &fileName );

	/// Do a ray cast for specified inout_mouseParams.
	/// \param iButton - 0 for left mouse buttom, 1 for middle, 2 for right
	/// \param inout_mouseParams - mouse click params to perform raycast
	/// \param out_pHitPos - param to fill out with raycast results
	void DoMouseRaycast( int iButton, MOUSECLICKPARAMS *inout_mouseParams, Vec3 *out_pHitPos );

	/// \brief	The component type of this class
	CHashString m_HashName;
	/// \brief	The map of editors and objects
	EDITORMAP m_EditorMap;
	/// \breif  The map of tools and pickable objects
	TOOLMAP m_ToolMap;
	/// \brief	The map of objects that can be assiociated to other objects
	LINKMAP m_LinkMap;

	/// Current collision filter to use, default is filter nothing
	//unsigned int m_iCurrentCollisionGroup;
	COLLISIONGROUP m_iCurrentCollisionGroup;

	// storage pointer to the Elemental Engine toolbox
	IToolBox *m_ToolBox;

};

#endif	// EDITORMANAGER_H
