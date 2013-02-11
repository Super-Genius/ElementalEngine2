///============================================================================
/// \file		VisitNodeName.h
/// \brief		Header file for CVisitNodeName Class
/// \date		10-09-2005
/// \author		Josh Lulewicz
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

#ifndef _VISITNODENAME_H_
#define _VISITNODENAME_H_

class CVisitNodeName : public IVisitor
{
public:
	/// Constructor
	CVisitNodeName(CLuaHashStringList *hashList);
	/// Destructor
	~CVisitNodeName();

	/// Visit
	/// \param component -
	/// \param bVisitEnter - ?? 
	/// \return true on success, false on failure
	virtual bool Visit( IComponent * component, bool bVisitEnter );

	/// Is this visitor hierarchical?
	/// \param void
	/// \return true if it is, false if not
	virtual bool IsHierarchicalVisitor( void ) { return false; }
	
private:
	/// List of Lua HashStrings
	CLuaHashStringList *m_NameList;
};

#endif // #ifndef _VISITNODENAME_H_