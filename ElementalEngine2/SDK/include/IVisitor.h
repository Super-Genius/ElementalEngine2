///============================================================================
/// \file	IVisitor.h
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

#ifndef _IVISITOR_H
#define _IVISITOR_H

namespace ElementalEngine
{

class IVisitor
{
public:
	virtual ~IVisitor() {};

	virtual bool Visit( IComponent * component, bool bVisitEnter ) = 0;

	virtual bool IsHierarchicalVisitor( void ) = 0;
};

}	//namespace ElementalEngine

#endif