///============================================================================
/// \file		NodeSystem.h
/// \brief		Declaration of NodeSystem Object Class
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

#ifndef __NODESYSTEM_H__
#define __NODESYSTEM_H__

#include "stdafx.h"

class CNodeSystem : public CSceneObject<ISceneObject>
{
public:
	// Default constructor
	CNodeSystem(  IHashString *parentName, IHashString *name  );

	// Default destructor
	~CNodeSystem();

	/// \fn		virtual void Init()
	/// \brief	Initialize the object
	void Init();

	/// \fn		virtual void Update()
	/// \brief	Update the object
	bool Update();

	/// \fn		virtual void DeInit()
	/// \brief	Deinitialize the object
	void DeInit();

	static IComponent *Create(int nArgs, va_list argptr);

	/// Serialize this component to the archive.
	///	\param ar = reference to the archive class
	virtual void Serialize( IArchive &ar );

	virtual OBJECTSORTTYPES GetSceneSortID() const { return RENDEROBJECT3D; }

	void AddNode( CNode *node );
	void AddNodeConnection( CNodeConnection *nodeConnection );
	void AddNodeFace( CNodeFace *nodeFace);
	void AddHeightNode( CHeightNode *heightNode);

	void RemoveNode( IHashString *nodeName );
	void RemoveNodeConnection( IHashString *nodeConnectionName );
	void RemoveNodeFace( IHashString *nodeFaceName );
	void RemoveHeightNode( IHashString *heightNodeName);

	IDTOOBJECTMAP *GetNodesMap();
	IDTOOBJECTMAP *GetHeightNodesMap();

	virtual void SetPosition( const Vec3 &position );
	virtual Vec3 &GetPosition();

	float GetPointSize();
	int GetPointR();
	int GetPointG();
	int GetPointB();

	float GetLineSize();
	int GetLineR();
	int GetLineG();
	int GetLineB();

	// Message Handlers
	DWORD OnGetGlobalPosition( DWORD size, void *params );
	DWORD OnSetGlobalPosition( DWORD size, void *params );
	DWORD OnGetSchema( DWORD size, void *params );
	DWORD OnGetVertices( DWORD size, void *params );
	DWORD OnGetOrderedVertices( DWORD size, void *params );
	DWORD OnGetBoundingBox( DWORD size, void *params );

private:
	void UpdateTransformsFromParent();

	float m_fVersion;
	float m_fPointSize;
	int m_iPointR;
	int m_iPointG;
	int m_iPointB;

	float m_fLineSize;
	int m_iLineR;
	int m_iLineG;
	int m_iLineB;

	IDTOOBJECTMAP m_Nodes;
	list<DWORD> m_OrderedNodes;
	IDTOOBJECTMAP m_NodeConnections;
	IDTOOBJECTMAP m_NodeFaces;
	IDTOOBJECTMAP m_HeightNodes;
};

#endif //#ifndef __NODESYSTEM_H__