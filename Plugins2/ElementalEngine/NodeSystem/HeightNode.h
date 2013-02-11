///============================================================================
/// \file		HeightNode.h
/// \brief		Declaration of HeightNode Object Class
/// \date		08-08-2007
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

#ifndef __HEIGHTNODE_H__
#define __HEIGHTNODE_H__

#include "stdafx.h"

class CHeightNode : public CObjectTemplate<IObject>
{
public:
	// Default constructor
	CHeightNode(  IHashString *parentName, IHashString *name  );

	// Default destructor
	~CHeightNode();

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

	CHeightNode *GetConnectedHeightNode();
	float GetHeight();
	void GetLocalPosition( Vec3 *v );
	void GetGlobalPosition( Vec3 *v );

	/// Message Handlers
	DWORD OnGetGlobalPosition( DWORD size, void *params );
	DWORD OnSetGlobalPosition( DWORD size, void *params );
	DWORD OnGetSchema( DWORD size, void *params );

private:
	CNodeSystemManager *GetManager();
	CNodeSystem *m_NodeSystem;

	float m_fVersion;
	CHashString m_hszLabel;
	float m_fHeight;
	CHashString m_hszConnectedHeightNode;
	CHeightNode *m_ConnectedHeightNode;

	CHeightNodeRenderObject *m_RenderObject;
};

#endif //#ifndef __HEIGHTNODE_H__