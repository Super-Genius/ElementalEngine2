///============================================================================
/// \file		QHStateMachineActionHandler.h
/// \brief		Declaration of CQHStateMachineActionHandler Object Class
/// \date		09-25-2006
/// \author		Richard Nguyen
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

#ifndef __QHSTATEMACHINEACTIONHANDLER_H__
#define __QHSTATEMACHINEACTIONHANDLER_H__

#include "stdafx.h"

class CQHStateMachineActionHandler : public CObjectTemplate<IObject>
{
public:
	// Default constructor
	CQHStateMachineActionHandler( IHashString *parentName, IHashString *name );

	// Default destructor
	virtual ~CQHStateMachineActionHandler();

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

	bool SendActionMessage(IArchive *arc, IHashString *zParentName);

	void VisitNode( IVisitor *visitor );

	DWORD OnGetSchema( DWORD size, void *params );
	DWORD OnSetAbstract( DWORD size, void *params );
	DWORD OnGetAbstract( DWORD size, void *params );
	///	\brief	return true if state name is editable (object is not abstract)
	DWORD OnIsLabelEditble( DWORD size, void *params );

private:
	static const float m_fCurrentVersion;
	CHashString		m_szName;
	CHashString		m_szActionName;

	bool			m_bAbstract;

	IActionHandler *m_pAH;
};

#endif // #ifndef __QHSTATEMACHINEACTIONHANDLER_H__