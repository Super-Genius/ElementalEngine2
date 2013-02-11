///=====================================================================
/// \file	StateMachineClipboardVisitor.h
/// \brief	Declaration of CStateMachineClipboardVisitor
/// \date	12/16/2007
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
#ifndef _STATE_MACHINE_CLIPBOARD_VISITOR_H_
#define _STATE_MACHINE_CLIPBOARD_VISITOR_H_

class CStateMachineClipboardVisitor : public IVisitor
{
public:
	CStateMachineClipboardVisitor( IXMLArchive *pArchive );
	virtual ~CStateMachineClipboardVisitor();

	virtual bool Visit( IComponent * component, bool bVisitEnter );
	virtual bool IsHierarchicalVisitor();

private:
	IToolBox	*m_pToolBox;
	IXMLArchive *m_pArchive;
};

#endif // _STATE_MACHINE_CLIPBOARD_VISITOR_H_