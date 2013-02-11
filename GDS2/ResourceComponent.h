///============================================================================
/// \file		ResourceComponent.h
/// \brief		Header file for Resource Component hooks into EE
/// \date		05-20-2005
/// \author		Josh Lulewicz
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

#ifndef RESOURCECOMPONENT_H
#define RESOURCECOMPONENT_H

class CResourceDoc;

/// \class CResourceComponent
/// \brief Component interface for resources
class CResourceComponent : public IComponent
{
	SINGLETON(CResourceComponent);

private:
	CResourceDoc *m_Parent;

public:
	/// Set Parent of the Component
	/// \param parent - parent of the component
	/// \return void
	void SetParent(CResourceDoc *parent);

	/// Create the component
	/// \param void
	/// \return IComponent
	static IComponent *Create(int nArgs, va_list argptr);

	/// Serialize object
	/// \param ar - archive
	/// \return void
	void Serialize(IArchive &ar);

	/// Get the Component Type
	/// \param void
	/// \return IHashString - name of component type
	IHashString *GetComponentType();

	/// <insert brief description here>
	/// \param <insert parameter(s) here>
	/// \return <insert return description here>
	bool IsKindOf(IHashString *compType);

	// ----------- Messages Functions ---------------//
	/// Add resource message
	/// \param size - size of the message
	/// \param param - TBD
	/// \return DWORD = message return code, MSG_HANDLED_STOP, 
	///					MSG_HANDLED_PROCEED, error
	DWORD OnAddResourceMsg(DWORD size, void *param);

	/// Remove resource message
	/// \param size - size of the message
	/// \param param - TBD
	/// \return DWORD = message return code, MSG_HANDLED_STOP, 
	///					MSG_HANDLED_PROCEED, error
	DWORD OnRemoveResourceMsg(DWORD size, void *param);

	/// Clear resources message
	/// \param size - size of the message
	/// \param param - TBD
	/// \return DWORD = message return code, MSG_HANDLED_STOP, 
	///					MSG_HANDLED_PROCEED, error
	DWORD OnClearResourcesMsg(DWORD size, void *param);

	/// Register resource collection message
	/// \param size - size of the message
	/// \param param - TBD
	/// \return DWORD = message return code, MSG_HANDLED_STOP, 
	///					MSG_HANDLED_PROCEED, error
	DWORD OnRegisterResourceCollectionMsg(DWORD size, void *param);

	/// Get currently selected resources in the currently active page
	/// \param size - size of the message
	/// \param param - list< RESOURCEPAGEINFO > * to be filled with currently selected resources
	/// \return DWORD = message return code, MSG_HANDLED_STOP, 
	///					MSG_HANDLED_PROCEED, error
    DWORD CResourceComponent::OnGetSelectedResourcesMsg( DWORD size, void *param );

	/// Set currently selected resources in the currently active page
	/// \param size - size of the message
	/// \param param - list< StdString > * filled with names of resource to be selected
	/// \return DWORD = message return code, MSG_HANDLED_STOP, 
	///					MSG_HANDLED_PROCEED, error
	DWORD OnSetSelectedResourcesMsg( DWORD size, void *param );
	
	/// Gets the resource info given a relative path
	/// \param size - size of the message
	/// \param param - list< RESOURCEPAGEINFO > * to be filled with currently selected resources
	/// \return DWORD = message return code, MSG_HANDLED_STOP, 
	///					MSG_HANDLED_PROCEED, error
	DWORD OnGetResourceInfoByRelativePath( DWORD size, void * param );
	
	/// Pauses the thumbnail generation thread
	/// \param size - size of the message
	/// \param param - list< RESOURCEPAGEINFO > * to be filled with currently selected resources
	/// \return DWORD = message return code, MSG_HANDLED_STOP, 
	///					MSG_HANDLED_PROCEED, error
	DWORD OnPauseThumbnailGenerationThread( DWORD size, void * param );

	/// \brief	Get the current active directory
	/// \param size - size of the message
	/// \param param - ptr to IHashString
	/// \return DWORD = message return code, MSG_HANDLED_STOP, 
	///					MSG_HANDLED_PROCEED, error
	DWORD OnGetResourceActiveDirectory( DWORD size, void *param );

	/// \brief	resets the current Resource directory
	/// \param size - size of the message
	/// \param param - ptr to IHashString
	/// \return DWORD = message return code, MSG_HANDLED_STOP, 
	///					MSG_HANDLED_PROCEED, error
	DWORD OnSetResourceDirectory(DWORD size, void *param);
};

#endif // #ifndef RESOURCECOMPONENT_H
