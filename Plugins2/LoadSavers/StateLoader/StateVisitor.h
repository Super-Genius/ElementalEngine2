///=====================================================================
/// \file	StateVisitor.h
/// \brief	Declaration of CStateVisitor
/// \date	01/27/2008
/// \author	Andrey Ivanov
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
///=====================================================================
#ifndef __CSTATEVISITOR_H__
#define __CSTATEVISITOR_H__

typedef	map<DWORD, DWORD> Hash2Hash;

///	fills label to state name map for the state machine
class CStateVisitor : public IVisitor
{
public:
	CStateVisitor( Hash2Hash &label2State );
	~CStateVisitor();

	bool Visit( IComponent *component, bool bVisitEnter );
	bool IsHierarchicalVisitor( void );

private:
	Hash2Hash  &m_label2State;
	IArchive   *m_ar;
};

#endif // __CSTATEVISITOR_H__
