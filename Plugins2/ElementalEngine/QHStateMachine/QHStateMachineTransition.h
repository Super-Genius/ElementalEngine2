///============================================================================
/// \file		QHStateMachineEvent.h
/// \brief		Declaration of CQHStateMachineTransition Object Class
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

#ifndef __QHSTATEMACHINETRANSITION_H__
#define __QHSTATEMACHINETRANSITION_H__

#include "stdafx.h"

class CQHStateMachineTransition : public CObjectTemplate<IObject>
{
	public:
	// Default constructor
	CQHStateMachineTransition( IHashString *parentName, IHashString *name );

	// Default destructor
	~CQHStateMachineTransition();

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
	void Serialize( IArchive &ar );

	IHashString *GetTargetState();

	IHashString *GetTransitionName();

	void VisitNode( IVisitor *visitor );

	DWORD OnSetAbstract( DWORD size, void *params );
	DWORD OnGetAbstract( DWORD size, void *params );
	DWORD OnGetSchema( DWORD size, void *params );
	///	\brief	return true if state name is editable (object is not abstract)
	DWORD OnIsLabelEditble( DWORD size, void *params );

	int GetWeight();

private:

	///	\brief	return parent event object for transition
	CQHStateMachineEvent *GetEvent();

private:
	static const float m_fCurrentVersion;
	int				m_iWeight;
	CHashString		m_szName;
	CHashString		m_szTargetState;

	bool			m_bAbstract;
};

#endif //#ifndef __QHSTATEMACHINETRANSITION_H__