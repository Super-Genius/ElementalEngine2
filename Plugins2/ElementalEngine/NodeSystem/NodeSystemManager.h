///============================================================================
/// \file		NodeSystemManager.h
/// \brief		Declaration of CNodeSystemManager Class
/// \date		07-11-2007
/// \author		Richard Nguyen
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

#ifndef __NODESYSTEMMANAGER_H__
#define __NODESYSTEMMANAGER_H__

class CNodeSystem;

class CNodeSystemManager : public CManager
{
private:
	SINGLETONCONSTRUCTOROVERRIDE( CNodeSystemManager );

public:
	// Default Constructor
	CNodeSystemManager();

	// Default Destructor
	virtual ~CNodeSystemManager( void );

	/// \brief	Function to get this component.
	///			Returns the singleton instance in this case
	static IComponent *Create(int nArgs, va_list argptr);

	/// Initializes the component and returns true on success,
	/// otherwise false
	virtual bool Init();

	/// \fn virtual void Update()
	/// \brief Function call back when system is being updated
	virtual void Update(DWORD tickCount);

	/// \brief Serialize this component to the archive.
	///	\param ar = reference to the archive class
	virtual void Serialize(IArchive &ar);

	/// \brief return a string of the component type
	/// \return IHashString *reference to textual classname
	virtual IHashString *GetComponentType();

	/// \brief Checks if this is similar to it's base classes component
	/// \return true if it is simlar, else false
	virtual bool IsKindOf(IHashString *compType) { return false; }


	/// Deletes an AI Object and removes it to the AI System
	/// \param Object is the IObject derived class pointer to delete
	/// \return true if success, otherwise false
	virtual bool DeleteObject( IObject* Object );

	CNodeSystem *GetNodeSystem( IHashString *nodeSystemName );
	CNode *GetNode( IHashString *nodeName );
	CHeightNode *GetHeightNode( IHashString *heightNodeName );

	bool IsRendering();

	/// Message Handlers
	DWORD OnEnableNodeSystemRender( DWORD size, void *params );
	DWORD OnDisableNodeSystemRender( DWORD size, void *params );

private:
	IObject *GetMapObject( IHashString *objectName, IHashString *componentType );
	
	CHashString m_hszNodeSystemComponentType;
	CHashString m_hszNodeComponentType;
	CHashString m_hszHeightNodeComponentType;
	CHashString m_hszNodeConnectionComponentType;
	CHashString m_hszNodeFaceComponentType;
	CHashString m_hszNodeRenderObjectType;
	CHashString m_hszNodeConnectionRenderObjectType;

	/// Helper function for constructor
	void RegisterLuaDefinitions( void );

	lua_State *m_LuaState;
	IToolBox *m_ToolBox;
	bool m_bRender;
};

#endif // #ifndef __NODESYSTEMMANAGER_H__
