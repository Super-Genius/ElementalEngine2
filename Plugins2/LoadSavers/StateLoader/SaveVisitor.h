///============================================================================
/// \file		CSaveVisitor.h
/// \brief		Header file for visitor pattern to state machine hierarchy
/// \date		06-21-2007
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
#ifndef _CSAVEVISITOR_H_
#define _CSAVEVISITOR_H_

#include "StdAfx.h"

class CSaveVisitor : public IVisitor
{
public:
	CSaveVisitor( IXMLArchive* pArchive );
	~CSaveVisitor();

	bool Visit( IComponent *component, bool bVisitEnter );
	bool IsHierarchicalVisitor( void );

private:
	void BeginDerivedState();
	void BeginDerivedEvent();
	void CreateLabelArchive();
	void SaveEditingProperties( IHashString *name, IXMLArchive *ar );

private:
	StdString GetLabel( IObject *object );

private:
	IToolBox *m_pToolBox;
	IXMLArchive *m_pArchive;
	vector<bool> m_started;
	vector<IObject *> m_objects;
	IArchive *m_ar;
};

#endif		// #ifndef _CSAVEVISITOR_H_
