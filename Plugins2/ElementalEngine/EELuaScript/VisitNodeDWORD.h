///============================================================================
/// \file		VisitNodeDWORD.h
/// \brief		Header file for CVisitNodeDWORD class
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

#ifndef _VISITNODEDWORD_H_
#define _VISITNODEDWORD_H_

class CVisitNodeDWORD : public IVisitor
{
public:
	/// Constructor
	CVisitNodeDWORD(CLuaDWORDSet *dwordSet);
	/// Destructor
	~CVisitNodeDWORD();

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
	/// Set of DWORDs
	CLuaDWORDSet *m_DWORDSet;
};

#endif // #ifndef _VISITNODEDWORD_H_