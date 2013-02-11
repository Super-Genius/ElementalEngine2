///============================================================================
/// \file		NodeConnection.h
/// \brief		Declaration of NodeConnection Object Class
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

#ifndef __NODECONNECTION_H__
#define __NODECONNECTION_H__

#include "stdafx.h"

class CNodeConnection : public CObjectTemplate<IObject>
{
public:
	// Default constructor
	CNodeConnection(  IHashString *parentName, IHashString *name  );

	// Default destructor
	~CNodeConnection();

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

	Vec3 &GetNode1Position();
	Vec3 &GetNode2Position();

	IHashString *GetNode1Name();
	IHashString *GetNode2Name();

	bool IsRendering();

	/// Message Handlers
	DWORD OnGetSchema( DWORD size, void *params );

private:
	void SetBeginNode( IHashString *hszBeginNode );
	void SetEndNode( IHashString *hszEndNode );

	CNodeSystemManager *GetManager();
	CNodeSystem *m_NodeSystem;

	float m_fVersion;
	CHashString m_hszLabel;
	CHashString m_hszNode1;
	CHashString m_hszNode2;

	CNode *m_Node1;
	CNode *m_Node2;

	CNodeConnectionRenderObject *m_RenderObject;
};

#endif //#ifndef __NODECONNECTION_H__