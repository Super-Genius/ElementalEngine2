///============================================================================
/// \file		SectorModifyVisitor.h
/// \brief		Header file for Visitor pattern to flag as modified all terrain sectors
/// \date		08-26-2005
/// \author		Brian Bazyk
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

#ifndef _SECTORMODIFYVISITOR_H_
#define _SECTORMODIFYVISITOR_H_

class CSectorModifyVisitor : public IVisitor
{
public:
	CSectorModifyVisitor( CTerrainEditor *pParent );
	/// tells the callee that we want to visit depth first
	bool IsHierarchicalVisitor();
	/// Visitor function callback
	bool Visit( IComponent * component, bool bVisitEnter);

private:
	/// the sector class name (hashed)
	CHashString m_SectorClassHashName;
	/// quick pointer ot toolbox.
	IToolBox *m_ToolBox;
	CTerrainEditor *m_pParent;
};


#endif // #ifndef _SECTORRENAMEVISITOR_H_
