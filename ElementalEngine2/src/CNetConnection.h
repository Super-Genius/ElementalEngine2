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

#ifndef CNETCONNECTION_H
#define	CNETCONNECTION_H

///==========================================================================
/// \class	CNetConnection
/// \brief	Network Connection
///==========================================================================
class CNetConnection : public CObjectTemplate<IObject>
{
private:
	/// \brief	Can't call the default constructor.
	///			To instantiate this object call Create().
	/// \param parentName = name of parent in hierarchy
	/// \param name = IHashString * name of this object
	CNetConnection(IHashString *parentName, IHashString *name);

public:
	/// \brief	Destructor for CEntity
	virtual ~CNetConnection();

	/// \brief	Create the a instance of IObject
	/// \return	IComponent pointer
	static IComponent *Create(int nArgs, va_list argptr);

	/// \brief	The Update Function for the Object
	/// \return	True on sucess, False on faulure
	virtual bool Update();
	/// \brief	The DeInit Function for the Object
	virtual void DeInit();
};

#endif	// CNETCONNECTION_H