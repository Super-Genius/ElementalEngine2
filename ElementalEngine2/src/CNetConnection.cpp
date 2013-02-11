///==========================================================================
/// \file	CNetConnection.h
/// \brief	Network Connection
/// \date	4/1/2005
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
///==========================================================================

#include "StdAfx.h"

#include "CNetConnection.h"

REGISTER_COMPONENT( CNetConnection );

/// \brief	Can't call the default constructor.
///			To instantiate this object call Create().
CNetConnection::CNetConnection(IHashString *parentName, IHashString *name) : 
	OBJECTTEMPLATE( CNetManager, CNetConnection, IObject, parentName, name )
{
}

/// \brief	Destructor for CEntity
CNetConnection::~CNetConnection()
{
	DeInit();
}

/// \brief	Create the a instance of IObject
/// \return	IComponent pointer
IComponent* CNetConnection::Create(int nArgs, va_list argptr)
{
	IHashString *name;
	IHashString *parentName;
	IObject *self;

	name = va_arg(argptr, IHashString *);
	parentName = va_arg(argptr, IHashString *);
	// create the object however you must create it
	try
	{
		// try and create an instance
		self = new CNetConnection(name, parentName);
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

/// \brief	The Update Function for the Object
/// \return	True on sucess, False on faulure
bool CNetConnection::Update()
{
	return true;
}

/// \brief	The DeInit Function for the Object
void CNetConnection::DeInit()
{
}