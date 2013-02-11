///============================================================================
/// \file		Node.h
/// \brief		Declaration of Node Object Class
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

#ifndef __NODE_H__
#define __NODE_H__

#include "stdafx.h"

class CNode : public CObjectTemplate<IObject>
{
public:
	// Default constructor
	CNode(  IHashString *parentName, IHashString *name  );

	// Default destructor
	~CNode();

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

	bool IsRendering();

	void GetLocalPosition( Vec3 *v );
	void GetGlobalPosition( Vec3 *v );

	void RegisterConnection( CNodeConnection *nodeConnection );
	void DeregisterConnection( CNodeConnection *nodeConnection );

	/// Messages
	DWORD OnGetGlobalPosition( DWORD size, void *params );
	DWORD OnSetGlobalPosition( DWORD size, void *params );
	DWORD OnGetSchema( DWORD size, void *params );

private:
	CNodeSystemManager *GetManager();

	float m_fVersion;
	CHashString m_hszLabel;

	CHashString m_hszNodeSystemComponentType;
	CNodeSystem *m_NodeSystem;

	CNodeRenderObject *m_RenderObject;
	std::list<CNodeConnection*> m_ConnectionList;
};

#endif //#ifndef __NODE_H__